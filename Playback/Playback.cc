/* ////////////////////////////////////////////////////////////////////////////   
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  The developer(s) of the OMP audio player hereby grant(s) permission
//  for non-GPL compatible GStreamer plugins to be used and distributed
//  together with GStreamer and OMP. This permission is above and beyond
//  the permissions granted by the GPL license by which OMP is covered.
//  If you modify this code, you may extend this exception to your version
//  of the code, but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version.
//
//  Libraries used by OMP:
//
//    - clastfm: http://liblastfm.sourceforge.net/ 
//
//    - gstreamer: https://gstreamer.freedesktop.org/ 
//
//    - gtkmm: https://www.gtkmm.org/en/
//
//    - libconfig: http://www.hyperrealm.com/libconfig/
//
//    - standard C and C++ libraries
//
//    - taglib: http://taglib.org/
//
//////////////////////////////////////////////////////////////////////////// */





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//              //
//              //
// Class Header ///////////////////////////////////////////////////////////////
//              //
//              //

#include "Playback.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"

#include "../Configuration/Configuration.h"

#include "../GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "../GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.h"

#include "../GUI/Elements/MainMenus/MainMenu.h"

#include "../GUI/Elements/MainMenus/MainMenus.h"

#include "../GUI/Elements/Playlists/PlaylistColumnRecord.h"

#include "../GUI/Elements/Playlists/PlaylistTreeStore.h"

#include "../GUI/Elements/Playlists/Playlists.h"

#include "../GUI/Elements/Playlists/PlaylistsDatabase.h"

#include "../GUI/GUI.h"

#include "../GUI/Seekbar.h"

#include "../GUI/StatusBar.h"

#include "../Metadata/Metadata.h"

#include "../Scrobbling/Scrobbling.h"

#include "../Metadata/TimeConversion.h"

#include "../Metadata/Track.h"

#include "ParserAndDecoder.h"

#include "TrackBin.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <chrono>

#include <glibmm/main.h>

#include <glib-object.h>

#include <gio/gio.h>

#include <gst/audio/audio.h>

#include <gst/gst.h>

#include <gtkmm/checkbutton.h>

#include <gtkmm/checkmenuitem.h>

#include <gtkmm/treerowreference.h>

#include <mutex>

#include <sigc++/sigc++.h>

#include <string.h>

#include <thread>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

using namespace std;





//        //
//        //
//        //
// Macros /////////////////////////////////////////////////////////////////////
//        //
//        //
//        //

namespace sigc
{

  SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE

}





//                 //
//                 //
//                 //
// Class Functions ////////////////////////////////////////////////////////////
//                 //
//                 //
//                 //

//             //
//             //
// Constructor ////////////////////////////////////////////////////////////////
//             //
//             //

Playback::Playback(Base& base, int argc, char* argv[])

// Inherited Class

: Parts(base, true)



// State Indicators

, backend_quitting_(false)

, change_sink_(false)

, delete_playback_queue_row_(false)

, first_track_(true)

, playback_state_change_(false)

, quit_queue_(false)

, resetting_track_queue_(false)

, stop_after_current_track_(false)

, track_data_queue_active_(false)

, track_data_queue_thread_active_(false)

, track_queue_full_(false)



// Time Storage

, duration_(0LL)

, position_(0LL)



// Tracks

, empty_track_(new Track)

 {

  // Initiates GStreamer so that its functions and data types can be used.
  gst_init(NULL, NULL);



  // Creates a new g_main_loop and assigns it to the pointer loop_.
  loop_ = g_main_loop_new (NULL, FALSE);



  // Acquire the choosen sink from the configuration.
  string sink_str = (config().get("output.sink"));

  // Converted the sink string to a const char*.
  const char* const_sink_c_str = sink_str.c_str();

  // Converts the sink const char* to just a char*.
  char* sink_c_str = const_cast<char*>(const_sink_c_str);



  stringstream debug_ss;

  debug_ss << "sink_c_str: \"" << sink_c_str << "\"";

  debug(debug_ss . str() . c_str());



  // Creates a new pipeline.
  pipeline_ = gst_pipeline_new ("audio-player");

  // Element responsible for linking the data of each track so gapless
  // playback is achieved.
  concat_ = gst_element_factory_make("concat", "stream-concatenator");

  // Used to change the volume of the music from the program.
  volume_ = gst_element_factory_make("volume", "main_volume");

  // Used to change the volume of the replay gain volume of tracks without
  // replay gain data.
  replay_gain_ = gst_element_factory_make("rgvolume", "rg-pre-amp");

  // Used to change the volume of the replay gain volume of tracks without
  // replay gain data.
  replay_gain_limiter_ = gst_element_factory_make("rglimiter", "rg-limiter");

  // Audio sink where the audioi data goes to once it is processed.
  sink_ = gst_element_factory_make(sink_c_str, "audio-output");



  // Initializes the elements of the pipeline. The Play function returns if 
  // any element fails while initializing.
  if (!pipeline_ || !concat_ || !replay_gain_ || !replay_gain_limiter_ 
        || !volume_ || !sink_)
  {

    debug("One element could not be initalized! Playback is stopped.");



    // Ends the play function if the creation of one of the elements failed.
    return;

  }



  debug("Before assign volume level");



  // Assigns the volume level to the volume element.
  g_object_set (G_OBJECT (volume_),
                "volume", (gdouble)(config().get("playback.volume")), NULL);



  debug("After assign volume level.");



  // Assigns the correct pre-amp value to the rg pre-amp.
  g_object_set(G_OBJECT(replay_gain_), "pre-amp", 
               (double(config().get("replaygain.pre_amp"))), NULL);



  // Assigns the correct pre-amp value to the rg pre-amp.
  g_object_set(G_OBJECT(replay_gain_), "fallback-gain",
               double(config().get("replaygain.fallback_gain")), NULL);



  // Gets a copy of the ReplayGain type.
  string gain_type = config().get("replaygain.gain_type");

  // True if album mode.
  if(gain_type == "album")
  {

    // Sets the ReplayGain type as album.
    g_object_set(G_OBJECT(replay_gain_), "album-mode", TRUE, NULL);

  }

  // True if track mode.
  else
  {

    // Sets the ReplayGain type as track.
    g_object_set(G_OBJECT(replay_gain_), "album-mode", FALSE, NULL);

  }



  // Adds elements to the pipeline.
  gst_bin_add_many(GST_BIN (pipeline_), concat_, replay_gain_,
                   replay_gain_limiter_, volume_, sink_, NULL);



  debug("After elements added to pipeline!");



  // Attaches the concat element to the other elements.
  if(gst_element_link_many(concat_, replay_gain_, replay_gain_limiter_,
                           volume_, sink_, NULL) 
       == TRUE)
  {

    debug("Main play linking succeeded!");

  }

  // True if the linking of the elements fails.
  else
  { 

    debug("Main play linking failed!");



    // Stops the playback.
    Stop();

  }



  debug("Before bus creation!");



  // Creates the bus message handler and adds it to the pipeline.
  bus_ = gst_pipeline_get_bus(GST_PIPELINE (pipeline_));

  // Stores the bus watch id.
  bus_watch_id_ = gst_bus_add_watch(bus_, Bus_Call, this);

  // Unreferences the bus, so it is only referenced by the pipeline.
  gst_object_unref(bus_);



  debug("Aftrer bus creation!");



  // Pad pointer to hold a pointer to the pad of the sink's sink pad. 
  GstPad* temp_pad;

  // Stores a pointer to the sink's sink pad and adds a reference.
  temp_pad = gst_element_get_static_pad(concat_, "src");

  // Adds the playback probe to the sink's sink pad.
  gst_pad_add_probe(temp_pad, GST_PAD_PROBE_TYPE_DATA_BOTH,
                    (GstPadProbeCallback) Playback_Event_Probe,
                    this, NULL);

  // Decrements the sink's sink pad.
  g_object_unref(temp_pad);



  // Creates a slot for the time storage Glib::signal_timeout.
  sigc::slot<bool> store_pipeline_position_and_duration_slot
    = sigc::bind(
        sigc::mem_fun(*this, 
                      &Playback::Store_Pipeline_Position_And_Duration), 0);

  // Creates a connection for the playback time Glib::signal_timeout so it
  // can begin storing the playback duration and position in the pipeline.
  sigc::connection store_pipeline_position_and_duration_conntection
    = Glib::signal_timeout() 
        . connect(store_pipeline_position_and_duration_slot,
                  2, Glib::PRIORITY_HIGH);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Playback::~Playback()
{

  // Destroys the object memory for the empty track.
  delete empty_track_;



  // Stops playback if the program is still playing or paused.
  if(Playing() || Paused())
  { 

    // Stops playback.
    Stop();

  }



  // True if the main GStreamer loop is not NULL. It then quits the main loop,
  // unreferences it, and then sets it equal to NULL.
  if(loop_ != NULL)
  { 

    // Quits the main loop.
    g_main_loop_quit(loop_);

    // Unreferences the main loop.
    g_main_loop_unref(loop_);

    // Sets the main loop equal to NULL.
    loop_ = NULL;

  }

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//           //
// Callbacks //////////////////////////////////////////////////////////////////
//           //

gboolean Playback::Bus_Call(GstBus* bus_ptr, GstMessage* msg_ptr,
                            void* playback_vptr)
{

  // Creates a pointer to the Playback class instance.
  Playback *playback_ptr = static_cast<Playback*>(playback_vptr);



  // Creates a pointer to the playback_ptr's main loop.
  GMainLoop *loop_ptr = playback_ptr -> loop();



  // Determines what msg_ptr is going through the bus_ptr for the current loop.
  switch (GST_MESSAGE_TYPE (msg_ptr))
  {

    // True if the end of stream is reached in the bus_ptr.
    case GST_MESSAGE_EOS:
    { 

      playback_ptr -> debug("End of the stream in the bus_ptr!"); 



      // Quits the main loop.
      g_main_loop_quit(loop_ptr);

      // Forces the playback_ptr to stop.
      playback_ptr -> Stop();



      // Ends the switch.
      break;

    }

    // Bus msg_ptr if an error occurs.
    case GST_MESSAGE_ERROR:
    {

      // Holds the debug info_ptrrmation in a string.
      gchar *debug_info_ptr;

      // Holds the extracted error info_ptrrmation.
      GError *error;



      // Parses the error msg_ptr into the error variables. 
      gst_message_parse_error (msg_ptr, &error, &debug_info_ptr);

      // Fress the debug_info_ptr's memeory.
      g_free (debug_info_ptr);



      stringstream debug_ss;

      debug_ss << "Error: " << error -> message;

      playback_ptr -> debug(debug_ss . str() . c_str());



      // Frees the error msg_ptr's memory.
      g_error_free (error);



      // Ends the main loop.
      g_main_loop_quit(loop_ptr);



      //  Sets playback as not changing states.
      playback_ptr -> playback_state_change() = false;



      // Stops playback.
      playback_ptr -> Stop();



      // Exits the switch.
      break;

    } 

    // Default if the msg_ptr is none of the ones already handled.
    default:
    {

      // Exists the switch.
      break;

    } 

  }



  // Propagates the bus_ptr callback.
  return TRUE;

}

guint Playback::Idle_Change_Track(void* playback_vptr)
{

  // Creates a pointer to the Playback class instance.
  Playback *playback_ptr = static_cast<Playback*>(playback_vptr);



  playback_ptr -> debug("Beginning of Idle_Change_Track");



  // Starts the scrobbling loop. 
  playback_ptr -> base() . scrobbling() . Scrobble_Playing_Track_Lastfm();



  // Updates the next track as playing.
  playback_ptr -> base() . scrobbling() . Update_Playing_Track_Lastfm();



  // Clears the trackbins trash.
  playback_ptr -> trackbins_trash() . clear();



  // Calls the change track function in the playlists() class instance.
  playback_ptr -> playlists() . Change_Track();



  // True if the program just began the first track.
  if(playback_ptr -> first_track())
  {

  } 

  // Code run if not on the first track since starting playback_ptr.
  else
  {

    // Queues tracks if necessary.
    playback_ptr -> Fill_Track_Data_Queue();

  }  



  // Sets the first_track variable to false.
  playback_ptr -> first_track()  = false;



  // Ends the callback.
  return FALSE;

}

guint Playback::Idle_Stop(void* playback_vptr)
{

  // Creates a pointer to the Playback class instance.
  Playback *playback_ptr = static_cast<Playback*>(playback_vptr);



  playback_ptr -> debug("Beginning of Idle_Stop");



  // 
  playback_ptr -> Stop();



  // 
  playback_ptr -> stop_after_current_track() = false;



  // 
  playback_ptr -> main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : playback_ptr -> main_menus()())
  { 

    // 
    menu_bars_it -> stop_after_current_track_playback_check_menu_item()
      . set_active(false);

  }

  // 
  playback_ptr -> main_menus() . set_disable_menubar_functions_flag(false);



  // 
  playback_ptr -> config_guis() . set_disable_functions(true);

  for(auto config_guis_it : playback_ptr -> config_guis()())
  {

    // 
    config_guis_it -> playback_panel()
      . stop_after_current_track_check_button() . set_active(false);

  }

  // 
  playback_ptr -> config_guis() . set_disable_functions(false);




  // Ends the callback.
  return FALSE;

}

void Playback::On_Pad_Added(GstElement* parser_ptr, GstPad* parser_srcpad_ptr,
                            void* decoder_vptr)
{

  // Converts the decoder void pointer into a GstElement pointer.
  GstElement* decoder_ptr = (GstElement*)decoder_vptr;



  // Creates a GstPad sink pointer for the decoder.
  GstPad* decoder_sinkpad_ptr;

  // Creates the sink pad and assigns it to the GstPad pointer temp_sinkpad.
  decoder_sinkpad_ptr = gst_element_get_static_pad(decoder_ptr, "sink");



  // Links the sink pad to the previous element's source pad.
  gst_pad_link(parser_srcpad_ptr, decoder_sinkpad_ptr);



  // Unreferences temp_sinkpad.
  gst_object_unref(decoder_sinkpad_ptr);

}

gulong Playback::Track_Probe(GstPad* trackbin_srcpad_ptr,
                             GstPadProbeInfo* info_ptr, void* playback_vptr)
{

  // Creates a pointer to the Playback class instance.
  Playback *playback_ptr = static_cast<Playback*>(playback_vptr);



  // Gets the event passing through the trackbin_srcpad_ptr.
  GstEvent *event = GST_PAD_PROBE_INFO_EVENT(info_ptr);

  // True if the event is the end of the stream.
  if(GST_EVENT_TYPE(event) == GST_EVENT_EOS)
  {

    // 
    if(playback_ptr -> stop_after_current_track())
    {

      // Calls the change track function to finish changing the track.
      g_idle_add((GSourceFunc) Idle_Stop, playback_ptr);



      // 
      return GST_PAD_PROBE_DROP;

    }

    // 
    else
    {

      // Sets the flag to indicate the track is changing. Happens for the first
      // time when the first track starts.
      playback_ptr -> playlists() . changing_track() = true;
 


      // Waits for several conditions until allowing the stream to end.
      while(((playback_ptr -> track_data_queue_thread_active())
              && 
            (playback_ptr -> track_queue() . size() <= 1))
                ||
            (playback_ptr -> Track_Time_In_Queued_At_Minimum()
              &&
            (playback_ptr -> track_data_queue_thread_active())));

    }

  }



  // Default return value to propagate the probe.
  return GST_PAD_PROBE_OK;

}

gulong Playback::Playback_Event_Probe(GstPad* concat_srcpad_ptr,
                                      GstPadProbeInfo* info_ptr,
                                      void* playback_vptr)
{

  // Creates a pointer to the Playback class instance.
  Playback *playback_ptr = static_cast<Playback*>(playback_vptr);



  // Gets the event passing through the concat_srcpad_ptr.
  GstEvent *event = GST_PAD_PROBE_INFO_EVENT(info_ptr);

  // True if the event is NULL.
  if(event == NULL)
  { 

    playback_ptr -> debug("Null event probed at playback_ptr_queue_ src concat_srcpad_ptr!");

  }

  // True if the probe is for a buffer or buffer list.
  else if(!GST_IS_EVENT(event))
  {

    // Waits until the track is done changing.
    while(playback_ptr -> playlists() . changing_track());

  }

  // True if the event is the end of the stream.
  else if(GST_EVENT_TYPE(event) == GST_EVENT_EOS)
  {

    playback_ptr -> debug("End of Stream probed at playback_ptr_queue_ src concat_srcpad_ptr!");

  }

  // True if the event is the beginning of the stream. 
  else if(GST_EVENT_TYPE(event) == GST_EVENT_STREAM_START)
  {

    playback_ptr -> debug("Stream start probed at concat src concat_srcpad_ptr!");



    // True if playback_ptr is on the first track since the music was last stopped. 
    if(!(playback_ptr -> first_track()))
    {

      // Waits until the track queue has the next track to continue playing. 
      while(((playback_ptr -> track_queue() . size()) == 1))
      {

        playback_ptr -> debug("Track queue equal to 1");

      }

      // Gets a pointer to the now previous track that is at the front of the
      // track queue.
      shared_ptr<TrackBin> old_trackbin = playback_ptr -> track_queue() . front();



      // Detach's the previous track from concat.
      old_trackbin -> Detach();



      // 
      playback_ptr -> debug("After Detach()");



      // Removes the previous track from the track_queue.
      playback_ptr -> track_queue() . pop_front();
    
      // Puts the old trackbin in the trackbins_track.
      playback_ptr -> trackbins_trash() . push_back(old_trackbin);



      playback_ptr -> debug("After removal from track_queue_");

    }



    // Waits until the the track queue has been filled.
    while((playback_ptr -> track_queue() . size()) < 1);



    // Gets a pointer to the playing track.
    Track* playing_track_ptr = &(playback_ptr -> track_queue() . front() -> track());



    // True if the playing track is not a normal track.
    if((playing_track_ptr -> type()) != TrackType::NORMAL)
    {

      // 
      GstTagList* rg_tag_list = gst_tag_list_new_empty();



      // True if the playing track has ReplayGain album gain tags.
      if((playing_track_ptr -> replaygain_album_gain()) != -1000)
      {

        // Stores a copy of the album gain.
        double album_gain = playing_track_ptr -> replaygain_album_gain();

        // Adds the tag to the tag list.
        gst_tag_list_add(rg_tag_list, GST_TAG_MERGE_REPLACE,
                         "replaygain-album-gain", album_gain, NULL);

      }

      // True if the playing track has ReplayGain album peak tags.
      if((playing_track_ptr -> replaygain_album_peak()) != -1000)
      {

        // Stores a copy of the album peak.
        double album_peak = playing_track_ptr -> replaygain_album_peak();

        // Adds the tag to the tag list.
        gst_tag_list_add(rg_tag_list, GST_TAG_MERGE_REPLACE,
                         "replaygain-album-peak", album_peak, NULL);


      }

      // True if the playing track has ReplayGain track gain tags.
      if((playing_track_ptr -> replaygain_track_gain()) != -1000)
      {

        // Stores a copy of the track album.
        double track_gain = playing_track_ptr -> replaygain_track_gain();

        // Adds the tag to the tag list.
        gst_tag_list_add(rg_tag_list, GST_TAG_MERGE_REPLACE,
                         "replaygain-track-gain", track_gain, NULL);


      }

      // True if the playing track has ReplayGain track peak tags.
      if((playing_track_ptr -> replaygain_track_peak()) != -1000)
      {

        // Stores a copy of the track peak.
        double track_peak = playing_track_ptr -> replaygain_track_peak();

        // Adds the tag to the tag list.
        gst_tag_list_add(rg_tag_list, GST_TAG_MERGE_REPLACE,
                         "replaygain-track-peak", track_peak, NULL);


      }



      // Send the the tag as a tag event downstream.
      gst_element_send_event((GstElement*)(playback_ptr -> replay_gain()), gst_event_new_tag(rg_tag_list));



      // Unrefs the tag list.
//      gst_tag_list_unref(rg_tag_list);


    }



    playback_ptr -> debug("After replay gain non-normal set");



    // Calls the change track function to finish changing the track.
    g_idle_add((GSourceFunc) Idle_Change_Track, playback_ptr);

  }



  // Default return value to propagate the probe.
  return GST_PAD_PROBE_OK;

}

bool Playback::Store_Pipeline_Position_And_Duration(int timeout_number)                   
{                                                                               
                                                                                
  // Ensures the Timeout continues during the program's entire lifetime
  // until the user prompts to close it or it is force-closed.
  if(backend_quitting_)
  {

    // Exists the Timeout.
    return false;

  }



  // Retrieves and sets the position and duration of the pipeline 
  // into a variable if the music is playing.
  if(Playing() || Paused())
  {

    // Temporarily stores the position of the pipeline.
    gint64 temp_position;

    // Temporarily stores the duration of the pipeline.
    gint64 temp_duration;

    // Retrieves the duration and position if the query returns true.
    if(gst_element_query_position(pipeline_, GST_FORMAT_TIME, &temp_position)  
        &&                                                                      
       gst_element_query_duration(pipeline_, GST_FORMAT_TIME, &temp_duration))
    { 

      // Stores the newly retrieved position time.
      position_ . store(temp_position, memory_order_relaxed);

      // Stores the newly retrieved position time.
      duration_ . store(temp_duration, memory_order_relaxed);
                                                                              
    }

    // Sets the position and duration variables to 0 if the query fails.
    else
    {

      // Sets the position variable to 0.
      position_ = 0LL;

      // Sets the duration variables to 0.
      duration_ = 0LL;

    }

  } 

  // Sets the position and duration variables to 0 if not playing or paused.
  else
  {

    // Sets the position variable to 0.
    position_ = 0LL;

    // Sets the duration variables to 0.
    duration_ = 0LL;

  }



  // Returns true, continuing the Timeout.
  return true;

}





//                 //
// Object Creators ////////////////////////////////////////////////////////////
//                 //

std::shared_ptr<TrackBin> Playback::Create_Track_Bin_Sptr
  (Track* new_track, Gtk::TreeRowReference new_tree_row_ref, bool attach)
{

  // Pointer to the converter element for the track bin.
  GstElement* conv;

  // Pointer to the decoder element for the track bin.
  GstElement* decoder;

  // Pointer to the parser element for the track bin.
  GstElement* demuxer; 

  // Pointer to the parser element for the track bin.
  GstElement* parser; 

  // Pointer to the queue element for the track bin.
  GstElement* queue;

  // Pointer to the source element for the track bin.
  GstElement* source;



  // Creates a unique numbered name based on the tracks's title.
  Glib::ustring track_bin_name 
    = new_track -> title() + to_string(rand() % 2000000);

  // Creates the GstBin for the TrackBin.
  GstElement* track_bin = gst_bin_new(track_bin_name . c_str());



  // Creates the converter GSTElement for the TrackBin.
  conv = gst_element_factory_make ("audioconvert", "converter");

  // Creates the queue GstElement for the TrackBin.
  queue = gst_element_factory_make ("queue", "track-queue");

  // Creates the source GstElement for the TrackBin.
  source = gst_element_factory_make ("filesrc", "file-source");



  // Creates a ParserAndDecoder using the determined mime_type.
  ParserAndDecoder p_and_d(base(), *new_track);



  // assigns the new parser to the parser pointer.
  demuxer = p_and_d . demuxer_;

  // assigns the new parser to the parser pointer.
  parser = p_and_d . parser_;

  // Assigns the new decoder to the decoder pointer.
  decoder = p_and_d . decoder_;



  // Checks for errors in the creation of the pipeline elements.
  if (!track_bin || !source || !decoder || !conv || !queue)
  { 

    debug("At least one GstElement failed to be made in Create_Track_Bin!");



    // Checks if the creation of the track_bin was successful.
    if(!track_bin)
    {

      debug("track_bin failed!");

    }

    // Checks if the creation of the source was successful.
    if(!source)
    {

      debug("source failed!");

    }

    // Checks if the creation of the parser was successful.
    if(!parser)
    {

      debug("parser failed!");

    }

    // Checks if the creation of the decoder was successful.
    if(!decoder)
    {

      debug("decoder failed!");

    }

    // Checks if the creation of the converter was successful.
    if(!conv)
    {

      debug("converter failed!");

    }

    // Checks if the creation of the queue was successful.
    if(!queue)
    {

      debug("queue failed!");

    }

  }



  // Assigns the dithering type to the converter. This turns dithering off
  // completely.
  g_object_set(G_OBJECT(conv), "dithering", GST_AUDIO_DITHER_NONE, NULL);



  // Assigns a filename to the source element 
  g_object_set(G_OBJECT(source), "location", 
                (new_track -> filename() . c_str()), NULL);



  debug("After GstElements added to track_bin");



  // True if the codec requires both a demuxer has to wait for
  // the pad-added signal to connect the demuxer and decoder.
  if((parser == nullptr) && (demuxer != nullptr))
  {

    // Adds all of the elements to the track_bin.
    gst_bin_add_many(GST_BIN(track_bin), source, demuxer, decoder, conv, queue,
                     NULL);



    // True if the source and demuxer elements successfully connected.
    if(gst_element_link(source, demuxer) == TRUE)
    {

    }

    // True if the linking of the source and parser fails somehow. 
    else
    {

      debug("Source and parser could not be linked!");

    }



    // True if the decoder, converter, and queue are successfully connected.
    if(gst_element_link_many(decoder, conv, queue, NULL) == TRUE)
    {

    }

    // True if the connection fails because of an error.
    else
    {

      debug("decoder, conv and queue could not be linked!");

    }


 
    // A callback that is run when a src pad is added to the parser. The
    // callback connected the parser and decoder.
    g_signal_connect(demuxer, "pad-added", 
                     G_CALLBACK (On_Pad_Added), decoder);

  }

  // TrackBin that only needs a parser and decoder. 
  else if((parser != nullptr) && (demuxer == nullptr))
  {

    // Adds all of the elements to the track_bin.
    gst_bin_add_many(GST_BIN(track_bin), source, parser, decoder, conv, queue,
                     NULL);



    // True if the source, parser, decoder, converter and queue were linked.
    if(gst_element_link_many(source, parser, decoder, conv, queue,
                             NULL) 
         == TRUE)
    {

      debug("Parser and decoder track_bin linking completed!");

    }

    // True if the elements fail to link.
    else
    {
    
      debug("Parser and decoder track_bin linking could not be completed!");
 
    }

  }

  // True if the codec requires a demuxer, parser and decoder.
  // The demuxer has to wait for the pad-added signal to connect the parser.
  else if((parser != nullptr) && (demuxer != nullptr))
  {

    // Adds all of the elements to the track_bin.
    gst_bin_add_many(GST_BIN(track_bin), source, demuxer, parser, decoder,
                     conv, queue, NULL);



    // True if the source and demuxer elements successfully connected.
    if(gst_element_link(source, demuxer) == TRUE)
    {

    }

    // True if the linking of the source and parser fails somehow. 
    else
    {

      debug("Source and parser could not be linked!");

    }



    // True if the decoder, converter, and queue are successfully connected.
    if(gst_element_link_many(parser, decoder, conv, queue, NULL)
         == TRUE)
    {

    }

    // True if the connection fails because of an error.
    else
    {

      debug("decoder, conv and queue could not be linked!");

    }


 
    // A callback that is run when a src pad is added to the parser. The
    // callback connected the parser and decoder.
    g_signal_connect(demuxer, "pad-added", 
                     G_CALLBACK (On_Pad_Added), parser);

  }

  // True if the codec requires only a decoder, but does not need
  // to wait for the pad-added signal.
  else
  {

    // Adds all of the elements to the track_bin.
    gst_bin_add_many(GST_BIN(track_bin), source, decoder,
                     conv, queue, NULL);



    // True if the source, decoder, converter and queue were linked.
    if(gst_element_link_many(source, decoder, conv, queue, NULL) 
         == TRUE)
    {

      debug("Decoder only track_bin linking completed!");

    }

  }
 

 
  // Creates a pointer for a GstPad. 
  GstPad* temp_pad_ptr;

  // Gets the src pad of the last element of the track_bin, which is the queue. 
  temp_pad_ptr = gst_element_get_static_pad(queue, "src");

  // Adds the queue src pad as a ghost pad on the end of the track_bin.
  gst_element_add_pad(track_bin, gst_ghost_pad_new("src", temp_pad_ptr));

  // Unreferences the static pad for the src pad of the queue.
  gst_object_unref(temp_pad_ptr);



  // Adds a reference to the new track bin so it does not get destroyed at the
  // end of the function.
  gst_object_ref(track_bin);



  // Ensures the track bin's creation did not fail.
  if(!track_bin)
  {

    debug("track_bin failed at end of Create_Track_Bin");

  }



  // Creates a new TrackBin sptr using the track bin.
  std::shared_ptr<TrackBin> trackbin_sptr
    (new TrackBin(base(), new_track, track_bin, new_tree_row_ref, 
                  source, parser, decoder, demuxer, conv, queue));



  // New pad pointer to store the location of the trackbin's src pad.
  GstPad* temp_pad;

  // Sets the temporary pad pointer equal to the trackbin's src pad.
  temp_pad 
    = gst_element_get_static_pad(trackbin_sptr -> track_bin(), "src");

  // Creates a new pad probe for the track 
  gst_pad_add_probe(temp_pad, GST_PAD_PROBE_TYPE_DATA_BOTH, 
                      (GstPadProbeCallback)Track_Probe, this, NULL);    

  // Unreferences the src pad of the track bin.
  g_object_unref(temp_pad);



  // If true, attaches the new TrackBin to the concat element.
  if(attach)
  {

    debug("Attaching TrackBin to concat element!");



    // Attaches the new TrackBin to the concat element.
    trackbin_sptr -> Attach();



    debug("Attached!");



    // Holds return value for state change.
    GstStateChangeReturn ret;

    // Sets the new track to playing.
    ret = gst_element_set_state((GstElement*)(trackbin_sptr -> track_bin()),
                                GST_STATE_PLAYING);

    // If the state change fails, this loops continues until successful.
    while(ret == GST_STATE_CHANGE_FAILURE)
    {

      debug("State change failed!");



      // Sets the new track to playing.
      ret = gst_element_set_state((GstElement*)(trackbin_sptr -> track_bin()),
                                  GST_STATE_PLAYING);

    }

  }



  // Retrieves and stores the duration of the trackbin once it is available.
  std::thread track_seeking_thread([trackbin_sptr, this]
  { 

    debug("Inside Create_Trackbin_Sptr thread");



    // True if the new track is from a single file release.
    // Then, it handles seeking the track.
    if((trackbin_sptr -> track() . type()) == TrackType::SINGLE_FILE)
    {

      // Waits until the track has finished prerolling.
      while(gst_element_get_state(trackbin_sptr -> track_bin(), 
                                  NULL, NULL, 100) 
            == GST_STATE_CHANGE_ASYNC)
      {

        debug("waiting for seek!");

      }



      // Stores the  start time of the track.
      long int start;

      // True if the track has a pregap and pregap use is on.
      if((trackbin_sptr -> track() . Pregap()))
      {

        // True if OMP has a pregap and should start at it.
        if(this -> config() . get("playback.start_at_pregap"))
        {

          // Stores the pregap_start time of the track.
          start = trackbin_sptr -> track() . pregap_start();

        }

        // True if OMP is set to start at the normal start.
        else
        {

          // Stores the  start time of the track.
          start = trackbin_sptr -> track() . start();

        }

      }

      // Sets the track start to the normal start, ignoring the pregap.
      else
      {

        // Stores the  start time of the track.
        start = trackbin_sptr -> track() . start();

      }

      // Stores the end time of the track.
      long int end = trackbin_sptr -> track() . end();



      stringstream debug_ss;

      debug_ss << "Start: " << start 
               << "\n\nEnd: " << end;

      this -> debug(debug_ss . str() . c_str());



      // Return value to determine if the seek worked.
      bool seek_worked = false;

      if(start <= 20000000000)
      {

        // False until the seek succeeds.
        while(!seek_worked)
        {

          // Starts the track at the normal start and sets the end.
          seek_worked = this -> Update_Seek_Positions
                                  (trackbin_sptr -> track_bin(),
                                   20000000001, end);

          // Outputs if the seek fails.
          if(!seek_worked)
          {

//            debug("SEEK FAILED");

          }

        }


        // Waits until the track has finished seeking.
        while(gst_element_get_state(trackbin_sptr -> track_bin(), 
                                    NULL, NULL, 100) 
              == GST_STATE_CHANGE_ASYNC)
        {

          debug("waiting for seek!");

        }



        // Rests the seek_worked flag for the second seek.
        seek_worked = false;

        // False until the seek succeeds.
        while(!seek_worked)
        {

          // Starts the track at the normal start and sets the end.
          seek_worked = this -> Update_Seek_Positions
                                  (trackbin_sptr -> track_bin(), start, -1);

          // Outputs if the seek fails.
          if(!seek_worked)
          {

//            debug("SEEK FAILED");

          }

        }



        // Waits until the track has finished seeking.
        while(gst_element_get_state(trackbin_sptr -> track_bin(),
                                    NULL, NULL, 100)
              == GST_STATE_CHANGE_ASYNC)
        {

//          debug("waiting for seek!");

        }



        // Sets the track as having finished its seeking.
        trackbin_sptr -> seeked() = true;

      }

      else
      {

        // False until the seek succeeds.
        while(!seek_worked)
        {

          // Starts the track at the normal start and sets the end.
          seek_worked = this -> Update_Seek_Positions
                                  (trackbin_sptr -> track_bin(),
                                   start, end);

          // Outputs if the seek fails.
          if(!seek_worked)
          {

//            debug("SEEK FAILED");

          }

          // Waits until the track has finished seeking.
          while(gst_element_get_state(trackbin_sptr -> track_bin(),
                                      NULL, NULL, 100)
                == GST_STATE_CHANGE_ASYNC)
          {

//            debug("waiting for seek!");

          }



          // Sets the track as having finished its seeking.
          trackbin_sptr -> seeked() = true;
        

        }

      }


    }

  });

  // Detaches the thread so the main thread doesn't have to wait to join it. 
  track_seeking_thread . detach();



  // Returns the newly created TrackBin shared pointer.
  return trackbin_sptr;

}





//       //
// Queue //////////////////////////////////////////////////////////////////////
//       //

bool Playback::Fill_Track_Data_Queue() 
{

  debug("Inside Fill_Track_Data_Queue.");



  // True if OMP is quitting or the track queue is already active.
  if(!(track_data_queue_mutex_ . try_lock()) || backend_quitting_ || track_data_queue_active_
       || track_data_queue_thread_active_ || Stopped())
  {

    debug("Track queue is already active.");



    // Indicates the track queue was ran for the current track.
    track_queue_ran_ = true;



    // Ends Fill_Track_Data_Queue.
    return false;

  } 



  // Retrieves the minimum buffer time from the configuration setting.
  long long minimum_buffer_time_nanoseconds
    = 1000000LL * int(config() . get("output.buffer_time"));

  // Retrieves the current time in the track queue not including playing track.
  long long current_buffer_time_nanoseconds = Track_Time_In_Queued();



  // Adds more tracks to the track queue if needed and available.
  if(current_buffer_time_nanoseconds < minimum_buffer_time_nanoseconds)
  {

    //                      //
    // Variable Declaration ///////////////////////////////////////////////////
    //                      //

    debug("Track queue is not full.");



    // Marks the track data queue as active.
    track_data_queue_active_ = true;

    // Sets the track's queue to not full.
    track_queue_full_ = false;



    // Saves the looping type of playback.
    string looping_type = config() . get("playback.looping");

    // Sets the playback order to be used to by the track queue.
    Glib::ustring playback_order = "linear";



    // Makes the data queue start with the playback queue if set to true.
    bool start_with_playback_queue = false;

    // Makes the data queue start with the selected track if set to true. If
    // the playback queue is also set to start first, the selected track is
    // used after the playback queue if needed. 
    bool unused_playback_follows_cursor = false;



    // Reference to the row that is next to be readied for playback.
    Gtk::TreeRowReference next_track_row_ref;

    // Iterator to the row that is next to be readied for playback.
    Gtk::TreeIter next_track_row_it;

    // RefPtr to the TreeStore that the next_track_row_ref is from.
    Glib::RefPtr<PlaylistTreeStore> next_track_row_treestore;



    // Reference to the selected row of the active playlist view if it exists.
    Gtk::TreeRowReference selected_row_ref = playlists() . selected_row_ref();

    // Iterator to the selected row in the active playlist view.
    Gtk::TreeIter selected_row_it;

    // True if a selected row exists in the active playlist.
    if(selected_row_ref)
    {

      // Creates an iterator for the selected row that was found using 
      // the selected row's reference.
      selected_row_it
        = selected_row_ref . get_model()
            -> get_iter(selected_row_ref . get_path());

    }



    // Stores an iterator to the playing row.
    Gtk::TreeIter playing_row_it
      = playlists() . playing_row_ref() . get_model()
          -> get_iter(playlists() . playing_row_ref() . get_path());



    // Reference to the currently last track in the track data queue.
    Gtk::TreeRowReference last_in_track_data_queue_row_ref
      = track_queue_ . back() -> row_ref();

    // RefPtr to the treestore for the last track in the track data queue.
    Glib::RefPtr<PlaylistTreeStore> last_in_track_data_queue_treestore
      = Glib::RefPtr<PlaylistTreeStore>::cast_dynamic
          (last_in_track_data_queue_row_ref . get_model());



    // Iterator to the next track in the track playback queue.
    Gtk::TreeIter next_in_track_playback_queue_row_it;





    //                                 //
    // Track Playback Queue Assessment ////////////////////////////////////////
    //                                 //

    debug("Before checking if track playback queue has tracks not in the " \
          "track data queue.");



    // True if the playback queue is not empty.
    if((playlists() . queue_playlist_treestore() -> children() . size()) > 0)
    {

      // True if playback is currently occurring from the playback queue.
      if(playlists() . playing_playlist_treestore()
           == playlists() . queue_playlist_treestore())
      { 

        // True if the last track in the track data queue is from the 
        // track playback queue.
        if((playlists() . queue_playlist_treestore())
             == last_in_track_data_queue_treestore)
        {

          // Gets an iterator to the last track in the track data queue.
          next_in_track_playback_queue_row_it
            = last_in_track_data_queue_treestore
                -> get_iter(last_in_track_data_queue_row_ref . get_path());



          // Iterates the last track to see if there are tracks left to be
          // added to the track data queue from the track playback queue.
          next_in_track_playback_queue_row_it++;

          // True if there is at least one more track to be queued from the
          // track playback queue.
          if(next_in_track_playback_queue_row_it)
          {

            // Makes the track data queue start with the track playback queue.
            start_with_playback_queue = true;

          }

        }

      }

      // True if playback is not occurring from the track playback queue.
      else
      {

        // Sets the next track to be queued as the start of the playback queue.
        next_in_track_playback_queue_row_it
          = playlists() . queue_playlist_treestore() -> children() . begin();

        // Makes the track data queue start with the track playback queue.
        start_with_playback_queue = true;

      }

    }





    //                                               //
    // Next Normal Track Data to Queue Determination //////////////////////////
    //                                               //

    // True if the selected track is determined to be set to play next. 
    if(

       // True if playback follows cursor is enabled
       (config() . get("playback.playback_follows_cursor")) 

         &&

       // True if there is a valid selected row.
       (selected_row_ref)

         &&

       // True if the selected treestore is not the playing treestore or the 
       // the selected row is not the playing row.
       (((selected_row_ref . get_model())
           != 
        (playlists() . playing_playlist_treestore()))
           ||
       (selected_row_it != playing_row_it))

         &&

       // True if the selected row is useable for playback follows cursor.
       (playlists() . new_selection()))
    {

      // Sets the next normal track to have data queued as the track of the 
      // selected row.
      next_track_row_it = selected_row_it;

      // Sets the next track to queue as the selected row's track.
      next_track_row_ref = playlists() . selected_row_ref();

      // Sets the treestore of the next track as the selected row's treestore.
      next_track_row_treestore
        = Glib::RefPtr<PlaylistTreeStore>
            ::cast_dynamic(next_track_row_ref . get_model());



      // Sets the selected row as not being an unused new selection.
      playlists() . new_selection() = false;

      // Indicates the new playback follows cursor track has not been used yet.
      unused_playback_follows_cursor = true;

    }

    // True if the next track to play will be what's next in the playlist 
    // based on the playback order setting.
    else
    { 

      // True if data queueing starts with the playback queue.
      if(start_with_playback_queue)
      {

        // True if the last non-queue track to be played is valid.
        if(playlists() . last_non_queue_row_ref())
        {

          // Sets the next normal track to be queued as the last non-queue 
          // track to be played.
          next_track_row_ref = playlists() . last_non_queue_row_ref();



          // Gets the row it for the next normal track in the track data queue.
          next_track_row_it
            = (next_track_row_ref . get_model())
                -> get_iter(next_track_row_ref . get_path());

          // Gets the treestore for the next track.
          next_track_row_treestore
            = Glib::RefPtr<PlaylistTreeStore>
                ::cast_dynamic(next_track_row_ref . get_model());



          // Iterators the next track row it so it is finally the next row 
          // after the last normal track to be played.
          next_track_row_it++;



          // True if playback looping is set to loop a playlist.
          if(looping_type == "playlist")
          {

            // True if the end of a playlist has been reached.
            if(!next_track_row_it)
            {

              // Sets the next track as the beginning of the playlist.
              next_track_row_it 
                = next_track_row_ref . get_model() -> children() . begin();

            }

          }

        }

      }

      // True if the track playback queue will not be used when queueing more 
      // tracks in the track data queue.
      else
      {

        // True if the Track Playback Queue was the last playlist to queue
        // tracks from. The last non-Track Playback Queue track is then used.
        if((playlists() . queue_playlist_treestore())
             == last_in_track_data_queue_treestore)
        { 

          // Stores the last non-queu track row reference.
          next_track_row_ref = playlists() . last_non_queue_row_ref();



          // Checks if the row reference is valid and retireves more
          // information if it is.
          if(next_track_row_ref)
          {

            // Stores the TreeStore of the next track row reference.
            next_track_row_treestore
              = Glib::RefPtr<PlaylistTreeStore>::cast_dynamic
                  (next_track_row_ref . get_model());



            // Stores the row iterator for the next track row.
            next_track_row_it
              = next_track_row_treestore 
                  -> get_iter(next_track_row_ref . get_path());

          }

        }

        // Uses the last track in the track data queue if it is valid. It will 
        // be used to find the next normal track to use in queueing track data.
        else if(last_in_track_data_queue_row_ref)
        {

          // Sets next_track_row_ref equal to the last track in the data queue.
          next_track_row_ref = last_in_track_data_queue_row_ref;



          // Gets the row it for the last track in the track_queue.
          next_track_row_it
            = (next_track_row_ref . get_model())
                -> get_iter(next_track_row_ref . get_path());

          // Stores a copy of the RefPtr to PlaylistTreeStore for the the
          // last track in the data queue.
          next_track_row_treestore
            = Glib::RefPtr<PlaylistTreeStore>
                ::cast_dynamic(next_track_row_ref . get_model());

        }



        // Find the next track row if the last one used is valid so that
        // next_track_row_it is finally the real next track row needed.
        if(next_track_row_it)
        {

          // Iterators the row it.
          next_track_row_it++;



          // Sets next_track_row_it to the beginning row of the playlist if
          // it has reached the end and playlist looping is enabled.
          if(looping_type == "playlist")
          {

            // True if next_track_row_it is invalid.
            if(!next_track_row_it)
            {

              // Stores the PlaylistTreeStore of next_track_row_it.
              next_track_row_treestore
              = Glib::RefPtr<PlaylistTreeStore>
                  ::cast_dynamic(next_track_row_ref . get_model());

              // Stores the beginning of the playlist as the new
              // next_track_row_it.
              next_track_row_it 
                = next_track_row_ref . get_model() -> children() . begin();

            }

          }

        }

      }



      // True if there is not a row linearly after the last track in the 
      // track_queue.
      if((!next_track_row_ref) || (!next_track_row_it))
      {

        // True if the is also no tracks in the track playback queue to
        // queue data from.
        if(!start_with_playback_queue)
        {

          // Sets the track data queue worker thread as inactive.
          track_data_queue_thread_active_ = false;

          // Sets the track queue as full.
          track_queue_full_ = true;

          // Unlocks the mutex for modifying modifying the track data queue.
          track_data_queue_mutex_ . unlock();

          // Sets the track data queue as inactive.
          track_data_queue_active_ = false;



          // Ends the track loop.
          return false;

        }

      } 

    }





    //                                                //
    // Track Metadata Collection Before Worker Thread /////////////////////////
    //                                                //


    // Sets the new buffer's total time to the current buffer's total time.
    long long new_buffer_time_nanoseconds = current_buffer_time_nanoseconds;



    // Creates the new list for preparing_tracks. 
    shared_ptr<list<shared_ptr<Track>>> 
      preparing_tracks_data(new list<shared_ptr<Track>>); 

    // Creates the new list of row iterators for preparing_tracks.
    shared_ptr<list<Gtk::TreeRowReference>> 
      preparing_track_row_refs(new list<Gtk::TreeRowReference>);



    // Saves the data for the amount of tracks needed to get to the minumum.
    while(new_buffer_time_nanoseconds < minimum_buffer_time_nanoseconds)
    {

      debug("Creating track queue row data!");



      // Stores the track's tree row to get a copy of it's Track instance.
      Gtk::TreeRow current_track_row;

      // Converts the current row TreeIter to a TreeRowReference.
      Gtk::TreeRowReference current_track_row_ref;



      // Starts gathering row information from the Track Playback Queue if
      // there are tracks in it not yet queued in the Track Data Queue.
      if(next_in_track_playback_queue_row_it)
      {

        // Creates a TreeRow to the next track to be added to the track queue.
        current_track_row = *next_in_track_playback_queue_row_it;

        // Converts the current row TreeIter to a TreeRowReference.
        current_track_row_ref
          = Gtk::TreeRowReference
              ((playlists() . queue_playlist_treestore()),
               Gtk::TreePath(next_in_track_playback_queue_row_it));

      } 

      // True if the is a another track from the normal playing playlist
      // available to queue data from.
      else if(next_track_row_it)
      {

        // Indicates there is no longer a selected track to start playback
        // from if there ever was one.
        unused_playback_follows_cursor = false;



        // Creates a TreeRow to the next track to be added to the track queue.
        current_track_row = *next_track_row_it;

        // Converts the current row TreeIter to a TreeRowReference.
        current_track_row_ref = Gtk::TreeRowReference((next_track_row_ref . get_model()),
                                  Gtk::TreePath(next_track_row_it));

      }

      // True if there is no longer any valid rows to queue data from.
      else
      {

        // Breaks from the data gathering loop.
        break;

      }



      // Adds the track row ref to preparing_track_row_refs!
      preparing_track_row_refs -> push_back(current_track_row_ref);

        

      // Creates a Track shared_ptr from the next track being prepared
      // for the queue.
      shared_ptr<Track> temp_track_sptr
        = (current_track_row[playlists() . playlist_column_record() . track_]);

      // Adds the shared_ptr<Track> to preparing_tracks_data!
      preparing_tracks_data -> push_back(temp_track_sptr);



      // Adds the new track's time to what would be in the track queue after
      // it is added.
      new_buffer_time_nanoseconds += temp_track_sptr -> duration();



      // True if the new track(s) will not put the track queue at its minumum.
      if(next_in_track_playback_queue_row_it)
      { 

        // Iterates to the next row in the Track Playback Queue.
        next_in_track_playback_queue_row_it++;

      }

      // True if the new tracks will be enough to reach the minimum time.
      else if(next_track_row_it)
      {

        debug("NORMAL TRACK ITERATION");



        // Iterates to the next normal track if available.
        next_track_row_it++;

 

        // True if the Playlist Repeat Looping setting is chosen.
        if(looping_type == "playlist")
        {

          // True if next_track_row_it is not valid because the end of the
          // playlist has been reached.
          if(!next_track_row_it)
          {

            debug("Playlist Repeat occurring!");



            // Sets the beginning row of the playlist as next_track_row_it.
            next_track_row_it
              = next_track_row_ref . get_model() -> children() . begin();

          }

        }

      }

      // True if there are no more tracks available to queue data from.
      else
      {

        // Exits the loop for getting more tracks' data.
        break;

      }

    }



    // True if the playing playlist did not use a 
    // selected row for Playback Follows Cursor.
    if(unused_playback_follows_cursor)
    {

      // Sets the playlist as having a new selection if it was unused.
      playlists() . new_selection() = true;

    }





    //                                                  //
    // Track Data Queue TrackBin Creation Worker Thread ///////////////////////
    //                                                  //

    // Marks the preparing trackbins thread to active!
    track_data_queue_thread_active_ = true;



    // Starts the thread that prepares the tracks.
    std::thread trackbin_preparation_thread
      ([this, playback_order, preparing_tracks_data, preparing_track_row_refs, 
        current_buffer_time_nanoseconds, minimum_buffer_time_nanoseconds]()
    {

      // Creates an iterator to the beginning of preparing_tracks_data.
      auto preparing_tracks_data_it = preparing_tracks_data -> begin();

      // Creates an iterator to the beginning of preparing_track_row_refs.
      auto preparing_track_row_refs_it = preparing_track_row_refs -> begin();



      // Loops through all of the tracks that will be prepared and creates
      // the trackbins for them.
      do
      {

        // True if the track queue is needed to end the track queue early. 
        if(quit_queue_)
        {

          // Exits the loop.
          break;

        }



        debug("Inside Fill_Track_Data_Queue thread.");



        // Gets the row it for the last track in the track_queue.
        Gtk::TreeIter current_track_row_it
           = (preparing_track_row_refs_it -> get_model())
                -> get_iter(preparing_track_row_refs_it -> get_path());

        // Stores the current track TreeRow using an iterator to it.
        Gtk::TreeRow current_track_row = *current_track_row_it;


 
        // Creates a Track shared_ptr from the next track being prepared
        // for the queue.
        shared_ptr<Track> temp_track_sptr
          = (current_track_row
               [playlists() . playlist_column_record() . track_]);

        // Gets a pointer to the Track instance.
        Track* new_track = new Track(*temp_track_sptr);



        // Gets a copy of the track's row iterator.
        auto temp_track_row_ref = *preparing_track_row_refs_it;

        // Creates of new TrackBin for the Track.
        auto new_trackbin_sptr
          = Create_Track_Bin_Sptr(new_track, temp_track_row_ref, true); 



        // True if the track being queued is from a single-file album.
        if(new_trackbin_sptr -> track() . type() == TrackType::SINGLE_FILE)
        {

          // Waits until the track has been properly queued.
          while(!(new_trackbin_sptr -> seeked()));

        }



        // Ads the new track to the track queue.
        track_queue_ . push_back(new_trackbin_sptr);



        // Increments the preparing_tracks_data iterator.
        preparing_tracks_data_it++;

        // Increments the track_row_its iterator.
        preparing_track_row_refs_it++;

      } 
      // Repeats the loop until the end of preparing tracks data is reached.
      while(preparing_tracks_data_it != (preparing_tracks_data -> end()));



      debug("Track queueing finished!");



      // Sets the flag for the track data queue worker thread to inactive.
      track_data_queue_thread_active_ = false;

      // Sets the track queue to full.
      track_queue_full_ = true;

      // Sets the track queue to ran.
      track_queue_ran_ = false;

      // Sets the track data queue to inactive.
      track_data_queue_active_ = false;

      // Releases the track data queue mutex.
      track_data_queue_mutex_ . unlock();

    });

    // Detaches the thread so the main thread my continue unhindred and the
    // new thread can do work and then end itself when done.
    trackbin_preparation_thread . detach();

  } 

  // True if the queue is full already.
  else
  { 

    // Sets the flag for the track data queue worker thread to inactive.
    track_data_queue_thread_active_ = false;

    // Sets the track queue to full.
    track_queue_full_ = true;

    // Sets the track queue to ran.
    track_queue_ran_ = false;

    // Sets the track data queue to inactive.
    track_data_queue_active_ = false;

    // Releases the track data queue mutex.
    track_data_queue_mutex_ . unlock();



    debug("Queue full!");

  }



  // Returns true to indicate normal operation occurred.
  return true;

}  

bool Playback::Reset_Track_Queue()
{

  // True if the track queue is already resetting.
  if(resetting_track_queue_)
  {

    // 
    return false;

  }

  // Sets the resettings track queue flag.
  resetting_track_queue_ = true;



  // True if the track queue is active.
  if(track_data_queue_thread_active_)
  {

    // Sets the track queue to quit.
    quit_queue_ = true;



    // Waits for the track queue to not be active.
    while(track_data_queue_thread_active_);



    // Sets the quit queue flag to false.
    quit_queue_ = false;

  }



  // Stores an iterator to the beginning of the track queue.
  auto track_queue_it = track_queue_ . begin();

  // Checks to see if the iterator is not the end.
  if(track_queue_it == track_queue_ . end())
  {

    // Sets the resetting the track queue flag to false.
    resetting_track_queue_ = false;



    // Returns, exiting the function.
    return false;

  }

  // Iterates the track queue iterator to the track after the playing track.
  track_queue_it++;



  debug("Before detaching TrackBins.");



  // Detaches all of the TrackBins after the playing track.
  while(track_queue_it != track_queue_ . end())
  {

    // Stores a shared_ptr to the TrackBin of the iterator.
    shared_ptr<TrackBin> temp_track_sptr = *track_queue_it;

    // Detaches the TrackBin sptr.
    temp_track_sptr -> Detach();



    // Iterates to the next TrackBin in the track queue.
    track_queue_it++;

  }



  debug("After detaching TrackBins");



  // Sets the track queue it back to the beginning.
  track_queue_it = track_queue_ . begin();

  // Iterates the track queue it to the second track.
  track_queue_it++;



  debug("Before eraseing TrackBins from track date queue.");



  // Erases all of the track queue TrackBin sptrs after the first track.
  track_queue_ . erase(track_queue_it, track_queue_ . end());



  // Sets the track_queue_full_ to false.
  track_queue_full_ = false;

  // Sets the resetting_track_queue_ flag to false.
  resetting_track_queue_ = false;



  // Calls the function to requeue tracks.
  Fill_Track_Data_Queue();



  // Returns as successful, ending the function.
  return true;

}

long long Playback::Track_Time_In_Queued()
{

  // An iterator for the track queue that starts at the beginning track, 
  // which is the playing track.
  auto track_queue_it = track_queue_ . begin();



  // Will hold the calculated current time of music stored in the track
  // queue's buffer.
  long long current_buffer_time_nanoseconds = 0LL;



  // Iterates the track queue so it is set to the track after the playing
  // track.
  track_queue_it++;

  // Iterates through the rest of the track queue to calculate the total
  // time in the track queue buffer.
  while(track_queue_it != track_queue_ . end())
  { 

    // Adds the duration of the of the current track in the track queue 
    // to current_buffer_time_nanoseconds. 
    current_buffer_time_nanoseconds
      += (*track_queue_it) -> track() . duration();

    // Increments the track queue iterator.
    track_queue_it++;

  } 



  // Returns the total time of the queue buffer after the playing track.
  return current_buffer_time_nanoseconds;

}

bool Playback::Track_Time_In_Queued_At_Minimum()
{ 

  // An iterator for the track queue that starts at the beginning track, 
  // which is the playing track.
  auto track_queue_it = track_queue_ . begin();

  // Iterates the track queue so it is set to the track after the playing
  // track.
  track_queue_it++;



  // Will hold the calculated current time of music stored in the track
  // queue's buffer.
  long long current_buffer_time_nanoseconds = 0LL;



  // Iterates through the rest of the track queue to calculate the total
  // time in the track queue buffer.
  while(track_queue_it != track_queue_ . end())
  { 

    // Adds the duration of the of the current track in the track queue 
    // to current_buffer_time_nanoseconds. 
    current_buffer_time_nanoseconds
      += (*track_queue_it) -> track() . duration();



    // Checks if the current buffer time is 10 seconds or greater. 
    if(current_buffer_time_nanoseconds >= 10000000000LL)
    {

      // Returns track if the track queue is at the minimum.
      return true;

    }



    // Increments the track queue iterator.
    track_queue_it++;

  } 



  // Returns that the track queue isn't over 10 seconds.
  return false;

}





//                //
// State Changers /////////////////////////////////////////////////////////////
//                //

void Playback::Next_Track()
{

  // 
  static mutex next_track_mutex;

  // 
  lock_guard<mutex> next_track_mutex_lock_guard(next_track_mutex);



  // 
  if(Stopped())
  {

    // 
    return;

  } 

  // 
  else if(Paused())
  {

    // 
    auto track_queue_it = track_queue_ . begin();

    // 
    if(track_queue_it == track_queue_ . end())
    {

      // 
      Stop();



      // 
      return;

    }

    // 
    track_queue_it++;

    //  
    if(track_queue_it == track_queue_ . end())
    {

      // 
      Stop();



      // 
      return;

    }
 
    // 
    Gtk::TreeRowReference next_track_row_ref = (*track_queue_it) -> row_ref();

    if(!next_track_row_ref)
    {

      // 
      Stop();



      // 
      return;

    }



    // 
    Stop();



    // 
    while(!Stopped())
    {

    }



    // 
    Play(next_track_row_ref, true);



    // 
    return;

  }



  // True if the playing track is of a single-file album.
  if(playing_track() . type() == TrackType::SINGLE_FILE)
  {

    // True if the playing track is the last track of a single-file album.
    if((playing_track() . end()) == -1)
    {

      // Seeks the playing track to the end.
      Update_Track_Position(duration_);

    }

    // True if the playing track is not the last track of a single-file album.
    else
    {

      // Seeks the playing track to the end.
      Update_Track_Position(playing_track() . end());

    }

  }

  // True if the playing track is a normal track.
  else
  {

    // Seeks the playing track to the end.
    Update_Track_Position(duration_ - 1);

  }

}

void Playback::Pause()
{

  // Returns the function if a playback state change is already occurring. 
  if(playback_state_change_ == true)
  {

    // Returns the functions.
    return;

  }

  // Sets playback_state_change_ to true.
  playback_state_change_ = true;



  // The loops is NULL, so nothing happens.
  if(loop_ == NULL)
  {

    debug("There is no song playing to be paused!");

  }

  // True if the program is playing and playback is desired to be paused.
  else if(Playing())
  { 

    // Updates the playback status label.
    status_bar() . Playback_Status_Paused();



    // Pauses the scrobbling loop.
    scrobbling() . set_scrobbling_loop_paused(true);



    // Pauses the pipeline.
    gst_element_set_state(pipeline_, GST_STATE_PAUSED);

    // Waits until the track has finished prerolling.
    while(gst_element_get_state(pipeline_, NULL, NULL, 100) 
          == GST_STATE_CHANGE_ASYNC)
    {

      debug("Waiting to Pause");

    }

   }

  else if(Stopped())
  {

    // Sets playback_state_change_ to false.
    playback_state_change_ = false;



    // 
    Play(Gtk::TreeRowReference(), true);

  }



  // Sets playback_state_change_ to false.
  playback_state_change_ = false;

}

void Playback::Play(Gtk::TreeRowReference playing_row_ref,
                    bool start_paused, long long start_time)
{

  debug("Play function started.");



  // Returns the function if a playback state change is already occurring. 
  if(playback_state_change_)
  {

    debug("Returning because playback state change is occurring!");



    // Returns the functions.
    return;

  }

  // Sets playback_state_change_ to true.
  playback_state_change_ = true;

  // 
  bool queue_track = false;




  // Starts the music if nothing is paused.
  if(!Paused())
  {

    // Determines if the playing track's row ref is valid or not.
    if(!playing_row_ref)
    {

      debug("Playing track row ref is invalid!");



      // 
      if((playlists() . queue_playlist_treestore() -> children() . size()) > 0)
      {

        // 
        if(Playing())
        {

          // 
          return;

        }




        // 
        queue_track = true;



        // 
        Gtk::TreePath first_queue_row_path
          (playlists() . queue_playlist_treestore() -> children() . begin());



        // 
        playing_row_ref
          = Gtk::TreeRowReference(playlists() . queue_playlist_treestore(), 
                                  first_queue_row_path);

      }

      // 
      else
      {

        // Assigns the selected row ref to playing_row_ref.
        playing_row_ref = playlists() . selected_row_ref();

      }



      // Checks if the new playing_row_ref is valid.
      if(!playing_row_ref)
      {

        debug("Exiting Play");



        // Sets playback_state_change_ to false.
        playback_state_change_ = false;



        // Ends the play function.
        return;

      }

      // 
      else if(Playing())
      {

        //
        playback_state_change_ = false;



        // 
        Stop();



        // 
        playback_state_change_ = true;

      }

    }

    // 
    else
    {

      // 
      if(Playing())
      {

        // 
        playback_state_change_ = false;



        // 
        Stop();



        // 
        playback_state_change_ = true;


      }
 
    }



    // Waits until the track queue from the last playback is finished.
    while(track_data_queue_thread_active_);



    // Clears the track queue before starting new playback.
    track_queue_ . clear();



    // 
    playlists() . new_selection() = false;



    // 
    playlists() . set_last_non_queue_row_ref(Gtk::TreeRowReference());



    // 
    delete_playback_queue_row_ = false;



    // Changes the output sink if needed.
    if(change_sink_)
    {

      // Unlinks the sink from the playback pipeline.
      gst_element_unlink(volume_, sink_);

      // Removes the sink from the playback pipeline.
      gst_bin_remove((GstBin*)(pipeline_), sink_);

 

      // Acquires the choosen sink from the configuration.
      string sink_str = (config() . get("output.sink"));

      // Converts the sink string to a const char*.
      const char* const_sink_c_str = sink_str . c_str();

      // Converts the sink const char* to just a char*.
      char* sink_c_str = const_cast<char*>(const_sink_c_str);



      // Audio sink where the audio data goes to once it is processed.
      sink_ = gst_element_factory_make(sink_c_str, "audio-output");



      // Adds the new sink to the pipeline.
      gst_bin_add((GstBin*)(pipeline_), sink_);

      // Connects the new sink to the pipeline.
      gst_element_link(volume_, sink_);



      // Sets change_sink_ to false.
      change_sink_ = false;

    }




   // Sets the first track flag to true. 
    first_track_ = true;

    // Sets the changing track variable to false.
    playlists() . changing_track() = false;



    // Updates the playback status label.
    status_bar() . Playback_Status_Playing();



    // Dereferences the playing track's TreeIter pointer and then dereferences
    // that iterator to get a copy of the TreeRow.
    Gtk::TreeRow row;

    // 
    if(queue_track)
    {

      // 
      row = *(playlists() . queue_playlist_treestore()
                              -> get_iter(playing_row_ref . get_path()));

    }

    // 
    else
    {

      // 
      row = *(playlists() . selected_playlist_treestore()
                              -> get_iter(playing_row_ref . get_path()));

    }

    // Makes a new track_sptr and assigns it to a temporary variable. 
    Track* temp_track_ptr
      = new Track(*shared_ptr<Track>
          ((row[playlists() . playlist_column_record() . track_])));



    // Stores the return value of the state change.
    GstStateChangeReturn ret; 

    // Attempts to change the pipeline to the playing state.
    ret = gst_element_set_state (pipeline_, GST_STATE_PAUSED);



    // 
    if(ret)
    {

    }



    // Creates the TrackBin for the playing track.
    auto trackbin_sptr = Create_Track_Bin_Sptr(temp_track_ptr,
                                               playing_row_ref, true);



    // Adds the new trackbin_sptr to the track queue.
    track_queue_ . push_back(trackbin_sptr);



    // True if the new track is from a single-file rip.
    if((playing_track().type()) == TrackType::SINGLE_FILE)
    {
  
      debug("SINGLE FILE");



      // True if the playing TrackBin has been successfully seeked.
      while(!(playing_trackbin() -> seeked()))
      {

      }



      debug("SEEKED");



      // 
      if(!start_paused)
      {

        // Holds whether the state change succeeded or not.
        GstStateChangeReturn ret; 

        // Attempts to change the pipeline to the playing state.
        ret = gst_element_set_state (pipeline_, GST_STATE_PLAYING);

        // True if the pipeline fails to start.
        if(ret == GST_STATE_CHANGE_FAILURE)
        {

          debug("Unable to set the pipeline to the playing state.");



          // Stops playback after playing the single-file cue track failed.
          Stop();



          // Ends the Timeout function.
          return;

        }

        // Waits until the track has finished prerolling.
        while(gst_element_get_state(pipeline_, NULL, NULL, 100) 
            == GST_STATE_CHANGE_ASYNC)
        {

          debug("waiting for seek!");

        }

      }


      // Starts queueing more tracks.
      Fill_Track_Data_Queue();



      // Sets playback_state_change_ to false.
      playback_state_change_ = false;



      // 
      sigc::connection start_loop_conn = Glib::signal_timeout() . connect(
      [this]() -> bool
      {

        g_main_loop_run(this -> loop());


        return false;

      }, 2);

    }

    // Handles playing if the track is a normal track. 
    else
    {
  
      debug("Setting normal track to playing state!");



      // Holds whether the state change succeeded or not.
      GstStateChangeReturn ret; 

      // Changes the pipeline and all of its elements to the playing state. 
      ret = gst_element_set_state (pipeline_, GST_STATE_PLAYING);
  
      // True if the playback fails to start. 
      if(ret == GST_STATE_CHANGE_FAILURE)
      {
  
        debug("Unable to set the pipeline to the playing state.");
 


        // Stops and resets playback. 
        Stop();



        // Ends the Play function.
        return;
  
      }

      // Waits until the track has finished prerolling.
      while(gst_element_get_state(pipeline_, NULL, NULL, 100) 
            == GST_STATE_CHANGE_ASYNC)
      {

        debug("waiting for seek!");

      }



      // Starts queueing more tracks.
      Fill_Track_Data_Queue();



      // Sets playback_state_change_ to false.
      playback_state_change_ = false;



      // 
      sigc::connection start_loop_conn = Glib::signal_timeout() . connect(
      [this]() -> bool
      {

        // 
        g_main_loop_run(this -> loop());



        // 
        return false;

      }, 2);

    }

  }

  // Unpauses the music.
  else
  {

    if((playlists() . playing_row_ref() . get_path())  
       != (playlists() . selected_row_ref() . get_path()))
    {

      // 
      playback_state_change_ = false;



      // 
      Stop();

      // 
      Play(playlists() . selected_row_ref());



      // 
      return;

    }



    // Changes the pipeline's playback state to playing.
    gst_element_set_state(pipeline_, GST_STATE_PLAYING);

    // Waits until the track has finished prerolling.
    while(gst_element_get_state(pipeline_, NULL, NULL, 100) 
          == GST_STATE_CHANGE_ASYNC)
    {

      debug("waiting for seek!");

    }



    // Updates the playback status label.
    status_bar() . Playback_Status_Playing();



    // Starts scrobbling again.
    scrobbling() . Scrobble_Playing_Track_Lastfm();

    // Updates the playing track on last.fm.
    scrobbling() . Update_Playing_Track_Lastfm();



    // Sets playback_state_change_ to false.
    playback_state_change_ = false;

  } 
 
}

void Playback::Quit()
{

  // True if the main GStreamer loop is not NULL. It then quits the main loop,
  // unreferences it, and then sets it equal to NULL.
  if(loop_ != NULL)
  { 

    // Quits the main loop.
    g_main_loop_quit(loop_);

    // Unreferences the main loop.
    g_main_loop_unref(loop_);

    // Sets the main loop equal to NULL.
    loop_ = NULL;

  }

}

void Playback::Stop()
{

  debug("Stopping playback.");



  // Returns the function if a playback state change is already occurring. 
  if(playback_state_change_ == true)
  {

    // Returns the functions.
    return;

  }



  //
  quit_queue_ = true;



  // Sets playback_state_change_ to true.
  playback_state_change_ = true;



  // 
  playlists() . new_selection() = false;

  playlists() . changing_track() = false;



  // 
  if(delete_playback_queue_row_)
  {

    // 
    Gtk::TreeIter first_in_playback_queue_row_it
      = playlists() . queue_playlist_treestore() -> children() . begin();



    // 
    Gtk::TreeRow first_in_playback_queue_row = *first_in_playback_queue_row_it;

    // 
    vector<int> id;

    // 
    id . push_back(first_in_playback_queue_row
                     [playlists() . playlist_column_record() . id_]);

    // 
    playlists() . database() . Delete_Rows("Queue", id);



    // 
    for(auto playlist : playlists()())
    {

      // 
      playlist -> selection_conn_ . block(true);

    }

    // 
    playlists() . set_disable_on_selection_changed(true);




    // 
    playlists() . queue_playlist_treestore()
      -> erase(first_in_playback_queue_row_it);



    // 
    for(auto playlist : playlists()())
    {

      if(playlist -> playlist_treestore() -> get_name() == "Queue")
      {

        // 
        playlist -> row_count_label() . set_text
          (to_string(playlist -> playlist_treestore() -> children() . size()));

      }



      // 
      playlist -> selection_conn_ . block(false);

    }

    // 
    playlists() . set_disable_on_selection_changed(false);



    // 
    playback() . delete_playback_queue_row() = false;

  }



  // True if the pipeline is not nullptr.
  if(pipeline_ != nullptr)
  {

    // Sets the flag to tell the queue to stop running. 
    quit_queue_ = true;

    // Waits until the queue isn't active.
    while(track_data_queue_thread_active_);

    // Sets the queue to not quit.
    quit_queue_ = false;



    // Sets the pipeline's state to NULL.
    gst_element_set_state(pipeline_, GST_STATE_NULL);

    // Waits until the track has finished prerolling.
    while(gst_element_get_state(pipeline_, NULL, NULL, 100) 
          != GST_STATE_CHANGE_SUCCESS)
    {

//      debug("Waiting finish stopping!");

    }

  }



  //
  for(auto it : track_queue_)
  {

    it -> Detach();

  }



  // Clears the track queue.
  track_queue_ . clear();

  // Clears the trackbins_track.
  trackbins_trash_ . clear();



  // Updates the TagView as not playing.
  gui() . Update_Tagview("Selected", selected_track());
 


  // Updates the playback status label.
  status_bar() . Playback_Status_Stopped();



  //
  string filename_str = playlists() . selected_track() . filename() . raw();

  //
  gui() . Load_Cover_Art(filename_str);




  // Sets playback_state_change_ to false.
  playback_state_change_ = false;

}

void Playback::Update_Track_Position(gint64 new_position)
{

  // True if the track is changing and then exits the function.
  if(playlists() . changing_track())
  {

    // Exits the function.
    return;

  }



  stringstream debug_ss;

  debug_ss << "New position: " << new_position;
  
  debug(debug_ss . str() . c_str());
  
  
  
  // True if the seeking occurs correctly or false if not. 
  bool handled
    = gst_element_seek((GstElement*)(pipeline_), 1.0, GST_FORMAT_TIME,
                       (GstSeekFlags)(GST_SEEK_FLAG_FLUSH), 
                       GST_SEEK_TYPE_SET, new_position,
                       GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
  
  
  
  handled ? debug("HANDLED") : debug("Not handled");

}

bool Playback::Update_Seek_Positions(GstElement *seeking_element,
                                     gint64 new_start, 
                                     gint64 new_end)
{

  // Holds the return value of the attempted seek.
  gboolean seek_worked = FALSE;



  // True if the end of the file does not need seeking.
  if(new_end == -1)
  {

    // Seeks to the beginning of the track.
    seek_worked = gst_element_seek(seeking_element,
                                   1.0, GST_FORMAT_TIME,
                                   (GstSeekFlags)(GST_SEEK_FLAG_FLUSH), 
                                   GST_SEEK_TYPE_SET, new_start,
                                   GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);

  }

  // True if the end of the file does need seeked.
  else
  {

    // Seeks the beginning and end of the file so it is just the track.
    seek_worked = gst_element_seek(seeking_element,
                                   1.0, GST_FORMAT_TIME,
                                   (GstSeekFlags)(GST_SEEK_FLAG_FLUSH), 
                                   GST_SEEK_TYPE_SET, new_start,
                                   GST_SEEK_TYPE_SET, new_end); 

  }



  // Returns whether the seek worked or not.
  return (seek_worked == TRUE);

}





//                  //
// State Indicators ///////////////////////////////////////////////////////////
//                  //

long long Playback::Duration()
{

  // Gets a copy of the current playing track. 
  auto playing_track_copy = playing_track();



  // True if the track is a normal individual track file.
  if(playing_track_copy . Normal())
  { 

    // Returns the pipeline duration because it is the same as the track.
    return duration();

  }

  // True if the track is not normal. 
  else
  { 

    // True if the playing track is from a single-file album. 
    if(playing_track_copy . type() == TrackType::SINGLE_FILE)
    {

      // True if the playing track is the last track of a release.
      if(playing_track_copy.end() == -1)
      {

        // Returns the calculated start position. 
        return (duration() - playing_track_copy . start());

      }

      // True if the playing track is not the last track of a release.
      else
      {

        // Returns the calculated state position.
        return (playing_track_copy.end() - playing_track_copy.start());

      }

    }

  }

}

bool Playback::Paused()
{

  // True if the pipeline is nullptr and not active.
  if(pipeline_ == nullptr)
  {

    // Returns as not paused.
    return false;

  }



  // Used to store the current state of the pipeline.
  GstState state;

  // Retrives the current state of the pipeline.
  gst_element_get_state(pipeline_, &state, NULL, 0);



  // Returns whether the pipeline was determined to be paused or not. 
  return (state == GST_STATE_PAUSED);

}

bool Playback::Playing()
{

  // True if the pipeline is nullptr and not active.
  if(pipeline_ == nullptr)
  {

    // Returns as not playing.
    return false;

  }



  // Used to store the current state of the pipeline.
  GstState state;

  // Retrives the current state of the pipeline.
  gst_element_get_state(pipeline_, &state, NULL, 0);



  // Returns whether the pipeline was determined to be playing or not. 
  return (state == GST_STATE_PLAYING);

}

long long Playback::Position()
{

  // Gets a copy of the current playing track. 
  auto playing_track_copy = playing_track();



  // True if the track is a normal individual track file.
  if(playing_track_copy.Normal())
  {

    // Returns the position of the playback pipeline. 
    return position();

  }

  // True if the track is not a normal individual track file. 
  else
  {

    // True if the track is from a normal single-file release.
    if(playing_track_copy.type() == TrackType::SINGLE_FILE)
    {

      // Calculates the real position of the track. 
      return (position() - playing_track_copy.start());

    }

  }

}

bool Playback::Stopped()
{

  // True if there is no pipeline object.
  if(pipeline_ == nullptr)
  {

    // Returns trust because playback is definitely stopped. 
    return true;

  }



  // Used to store the retrieved state of the playback pipeline. 
  GstState state;

  // Retrieves the state of the playback pipeline.
  gst_element_get_state(pipeline_, &state, NULL, 0);



  // Returns whether the pipeline is stopped or not.
  return (state == GST_STATE_NULL);

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

//                //
// Pipeline Parts /////////////////////////////////////////////////////////////
//                //

GstBus* Playback::bus()
{ 

  return bus_; 

}

GstElement* Playback::concat()
{ 

  return concat_; 

}

GMainLoop* Playback::loop()
{ 

  return loop_; 

}

std::shared_ptr<TrackBin> Playback::next_trackbin()
{ 

  // Returns a nullptr if the track queue is empty.
  if(track_queue_.empty())
  {

    // Returns a nullptr.
    return nullptr;

  }



  // Retrieves the iterator to the first track of the track queue.
  auto new_trackbin = (track_queue_.begin());



  // Iterates the iterator to the next TrackBin.
  new_trackbin++;



  // Dereferences the iterator and returns a shared_ptr to the next TrackBin.
  return *new_trackbin;

}

GstElement* Playback::pipeline()
{ 

  return pipeline_; 

}

std::shared_ptr<TrackBin> Playback::playing_trackbin()
{ 

  // Returns a nullptr if the track queue is empty.
  if(track_queue_.empty())
  {

    // Returns a nullptr.
    return nullptr;

  }



  // Returns the front of the track queue, which is the playing track.
  return track_queue_.front(); 

}

GstElement* Playback::replay_gain()
{ 

  return replay_gain_; 

}

GstElement* Playback::replay_gain_limiter()
{ 

  return replay_gain_limiter_; 

}

std::list<std::shared_ptr<TrackBin>>& Playback::track_queue()
{

  return track_queue_;

}

std::vector<std::shared_ptr<TrackBin>>& Playback::trackbins_trash()
{ 

  return trackbins_trash_; 

}

GstElement* Playback::volume()
{ 

  return volume_; 

}





//                  //
// State Indicators ///////////////////////////////////////////////////////////
//                  //

bool& Playback::backend_quitting()
{ 

  return backend_quitting_; 

}

std::atomic<bool>& Playback::change_sink()
{

  return change_sink_; 

}

std::atomic<bool>& Playback::delete_playback_queue_row()
{

  return delete_playback_queue_row_;

}

std::atomic<bool>& Playback::first_track()
{ 

  return first_track_; 

}

std::atomic<bool>& Playback::playback_state_change()
{ 

  return playback_state_change_;

}

std::atomic<bool>& Playback::stop_after_current_track()
{

  return stop_after_current_track_;

}

std::atomic<bool>& Playback::track_data_queue_active()
{

  return track_data_queue_active_;

}

std::atomic<bool>& Playback::track_data_queue_thread_active()
{

  return track_data_queue_thread_active_;

}

std::atomic<bool>& Playback::track_queue_full()
{

  return track_queue_full_;

}





//              //
// Time Storage ///////////////////////////////////////////////////////////////
//              //

long long Playback::position()
{

  return position_.load(); 

}

long long Playback::duration()
{

  return duration_.load();

}





//        //
// Tracks /////////////////////////////////////////////////////////////////////
//        //

Track& Playback::empty_track()
{

  return *empty_track_;

}

Track& Playback::next_track()
{

  return playlists().next_track();

}

Track& Playback::playing_track()
{

  return playlists().playing_track();

}

Track& Playback::previous_track()
{

  return playlists().previous_track();

}

Track& Playback::queue_track()
{  

  return playlists().queue_track();

}

Track& Playback::selected_track()
{ 

  return playlists().selected_track();

}  
