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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef PLAYBACK_H
#define PLAYBACK_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                   //
//                   //
// Inherited Headers //////////////////////////////////////////////////////////
//                   //
//                   //

#include "../Parts.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <list>

#include <memory>

#include <vector>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

//                            //
//                            //
// Class Forward Declarations /////////////////////////////////////////////////
//                            //
//                            //

class Base;

class Configuration;

class Flags;

class GUI;

namespace Gtk
{

  class TreeRowReference;

}

class Track;

class TrackBin;





//                             //
//                             //
// Struct Forward Declarations ////////////////////////////////////////////////
//                             //
//                             //

struct _GMainLoop;
typedef struct _GMainLoop GMainLoop;

struct _GstBin;
typedef struct _GstBin GstBin;

struct _GstBus;
typedef struct _GstBus GstBus;

struct _GstElement;
typedef struct _GstElement GstElement;

struct _GstMessage;
typedef struct _GstMessage GstMessage;

struct _GstPad;
typedef struct _GstPad GstPad;

struct _GstPadProbeInfo;
typedef struct _GstPadProbeInfo GstPadProbeInfo;

struct _GstTagList;
typedef struct _GstTagList GstTagList;





//          //
//          //
//          //
// Typedefs ///////////////////////////////////////////////////////////////////
//          //
//          //
//          //

typedef int gboolean;

typedef char gchar;

typedef signed long gint64;

typedef void* gpointer;

typedef unsigned int guint; 

typedef unsigned long gulong;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Playback : public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Creates the playback class.
    //
    //
    //
    // Arguments: 
    //
    //   base: A reference to the Base class of the program in order to set up
    //         the quick access references in the inherited Parts class.
    //
    //   argc: The argument count of the comnmand line run of the program.
    //
    //   argv: The arguments provided when the program was run.
    //
    //////////////////////////////////////////////////////////////////////// */
    Playback(Base& base, int argc, char* argv[]);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Used to destroy any data that needs it when the playback class ends.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    ~Playback();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //           //
  // Callbacks ////////////////////////////////////////////////////////////////
  //           //

  private:

    /* //////////////// ////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Callback for the bus to handle messages sent to the bus.
    //
    //
    //
    // Arguments: 
    //
    //   bus ptr: A pointer to the GstBus used by the playback pipeline.
    //
    //   msg_ptr: Message sent to the bus.
    //
    //   playback_vptr: A void pointer to the playback class instance.
    //
    //////////////////////////////////////////////////////////////////////// */
    static gboolean Bus_Call(GstBus* bus_ptr,
                             GstMessage* msg_ptr,
                             void* playback_vptr);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Stops the current track once it ends if stop_after_current_track_
    //   is true.
    //
    //
    // Arguments: 
    //
    //   playback_vptr: A void pointer to the playback class instance.
    //
    //////////////////////////////////////////////////////////////////////// */
    static guint Idle_Change_Track(void* playback_vptr);

    /* ///////////////// ///////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Handles everything to do with changing the track that must be done in
    //   the main thread.
    //
    //
    //
    // Arguments: 
    //
    //   playback_vptr: A void pointer to the playback class instance.
    //
    //////////////////////////////////////////////////////////////////////// */
    static guint Idle_Stop(void* playback_vptr);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Connects a demuxer to the decoder using a callback that waits until a 
    //   parser srcpad is immediately avilable.
    //
    //
    //
    // Arguments: 
    //
    //   parser_ptr: A pointer to the parser of a track.
    //
    //   parser_srcpad_ptr: A pointer to the srcpad of a parser.
    //
    //   decoder_vptr: A void pointer to the decoder of a track.
    //
    //////////////////////////////////////////////////////////////////////// */
    static void On_Pad_Added(GstElement* parser_ptr,
                             GstPad* parser_srcpad_ptr,
                             void* decoder_vptr);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Interprets what to do when certain events are coming through the
    //   srcpad of the concat element. It triggers events when a new stream
    //   comes through in order to change the track.
    //
    //
    //
    // Arguments: 
    //
    //   concat_srcpad_ptr: A pointer to the srcpad of the concat element.
    //
    //   info_ptr: A struct containing information about the probe.
    //
    //   playback_vptr: A void pointer to the playback class instance.
    //
    //////////////////////////////////////////////////////////////////////// */
    static gulong Playback_Event_Probe(GstPad* concat_srcpad_ptr,
                                       GstPadProbeInfo* info_ptr,
                                       void* playback_vptr);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   A timeout that queuries pipeline position and duration in order to
    //   stores them as atomic variables.
    //
    //
    //
    // Arguments: 
    //
    //   timeout_number: Number in milliseconds to call the the timeout again
    //   after of the timeout has finished.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Store_Pipeline_Position_And_Duration(int timeout_number);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Probes the srcpad of the track in order to stall playback or handle
    //   certain playback events involved with the track.
    //
    //
    //
    // Arguments: 
    //
    //   trackbin_srcpad_ptr: The sourcepad of the trackbin.
    //
    //   info_ptr: A struct containing information about the probe.
    //
    //   playback_vptr: A void pointer to the playback class instance.
    //
    /////////////////// ///////////////////////////////////////////////////// */
    static gulong Track_Probe(GstPad* trackbin_srcpad_ptr,
                              GstPadProbeInfo* info_ptr,
                              void* playback_vptr);





  //                 //
  // Object Creators //////////////////////////////////////////////////////////
  //                 //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Creates a new bin for an individual track.
    //
    //
    //
    // Arguments: 
    //
    //   new_track: pointer to the Track variable that was created with new. 
    //
    //   new_track_treeiter: 
    //
    //   attach: Determines whether to attach the track to the pipeline.
    //
    //////////////////////////////////////////////////////////////////////// */
    std::shared_ptr<TrackBin> Create_Track_Bin_Sptr
                                (Track* new_track, 
                                 Gtk::TreeRowReference new_tree_row_ref,
                                 bool attach = false);





  //       //
  // Queue ////////////////////////////////////////////////////////////////////
  //       //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Starts queueing the next tracks after the first one is played.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Queue_Tracks();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Trashes the queued tracks and requeues data.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Reset_Track_Queue();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   The time left in the queue after the currently playing track.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    long long Track_Time_In_Queued();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Determines whether the track time is at it's very minumum time for
    //   playback to be allowed to continue.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Track_Time_In_Queued_At_Minimum();





  //                //
  // State Changers ///////////////////////////////////////////////////////////
  //                //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Changes playback to the next track.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Next_Track();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Pauses the playback if playing.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Pause();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Starts the playback from the track chosen.
    //
    //
    //
    // Arguments: 
    //
    //   playing_track_iter: The iterator to the row of the track.
    //
    //   start_paused: Whether to start the song paused or not.
    //
    //   start time: The time to start the track at.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Play(Gtk::TreeRowReference playing_row_ref,
              bool start_paused = false, long long start_time = -1LL);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Quits the loop so the program can close.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Quit();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Stops the playback if paused or playing.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Stop();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Updates the position of the pipeline.
    //
    //
    //
    // Arguments: 
    //
    //   new_position: The new position to be seeked to.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Update_Track_Position(gint64 new_position);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Updates the start and end position of an element provided.
    //
    //
    //
    // Arguments: 
    //
    //   seeking_element: Element being seeked.
    //
    //   new_start: Where to set the start of the seek to.
    //
    //   new_end: Where to set the end of the seek to. If it is -1 the end is
    //   just the end of the file.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Update_Seek_Positions(GstElement* seeking_element,
                               gint64 new_start, 
                               gint64 new_end);





  //                  //
  // State Indicators /////////////////////////////////////////////////////////
  //                  //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Returns the calculated duration of the track.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    long long Duration();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Determines whether playback is paused or not.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Paused();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Determines whether playback is playing or not.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Playing();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Returns the calculated position of the track.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    long long Position();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Determines whether the playback is stopped or not.
    //
    //
    //
    // Arguments: 
    //
    //   
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Stopped();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //                //
  // Pipeline Parts ///////////////////////////////////////////////////////////
  //                //

  public:

    // Returns a pointer to the playback pipeline bus.
    GstBus* bus();

    // Returns a pointer to the playback pipeline concat element.
    GstElement* concat();

    // Returns a pointer to the main loop used by the pipeline.
    GMainLoop* loop();

    // Returns a copy of the second shared_ptr TrackBin of the track queue.
    std::shared_ptr<TrackBin> next_trackbin();

    // Returns a pointer to the playback pipeline.
    GstElement* pipeline();

    // Returns a copy of the first shared_ptr TrackBin of the track queue.
    std::shared_ptr<TrackBin> playing_trackbin();

    // Returns a pointer to the ReplayGain element.
    GstElement* replay_gain();

    // Returns a pointer to the ReplayGain limiter element.
    GstElement* replay_gain_limiter();

     // Returns a reference to the track queue list.
    std::list<std::shared_ptr<TrackBin>>& track_queue();

    // Returns a reference to the trackbins track.
    std::vector<std::shared_ptr<TrackBin>>& trackbins_trash();

    // Returns a reference to the volume element.
    GstElement* volume();





  //                  //
  // State Indicators /////////////////////////////////////////////////////////
  //                  //

  public:

    // Returns a copy of the boolean that indicates the backend is quitting.
    bool& backend_quitting();

    // Returns a bool idicating the sink needs to be changed when playback 
    // next starts.
    std::atomic<bool>& change_sink();

    // 
    std::atomic<bool>& delete_playback_queue_row();

    // Returns the variable indicating the first track is playing.
    std::atomic<bool>& first_track();

    // Returns the playlist end indicator boolean.
    std::atomic<bool>& playlist_end();

    // Returns a variable only allowing Stop(), Pause(), or Play() runs to
    // happen one at a time.
    std::atomic<bool>& playback_state_change();

    // Returns the variable that indiciates whether the preparing trackbins 
    // thread is active.
    std::atomic<bool>& preparing_trackbins_thread_active();

    // Returns the variable to stop playback after the current track. 
    std::atomic<bool>& stop_after_current_track();

    // Indicates the track queue is active.
    std::atomic<bool>& track_queue_active();

    // Indicates the track queue is full.
    std::atomic<bool>& track_queue_full();





  //              //
  // Time Storage /////////////////////////////////////////////////////////////
  //              //

  public:

    // Returns the duration of the pipeline.
    long long duration();

    // Returns the position of the pipeline.
    long long position();





  //        //
  // Tracks ///////////////////////////////////////////////////////////////////
  //        //

  public:

    // Returns the empty track. 
    Track& empty_track();

    // Returns the Track of the second track in the queue.
    Track& next_track();

    // Returns the Track of the first track in the queue.
    Track& playing_track();

    // Returns the last track in the queue. 
    Track& previous_track();

    // Returns the first track in the Queue, which is different than the track
    // queue. This queue is what is played before anything else.
    Track& queue_track();

    // Returns the track that is selected.
    Track& selected_track();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //
   
  //                //
  // Pipeline Parts ///////////////////////////////////////////////////////////
  //                //

  private:

    // The playback pipeline's bus.
    GstBus* bus_;

    // The id of the bus watch.
    guint bus_watch_id_;

    // Element that concatenates the streams of each track together.
    GstElement* concat_;

    // The main loop that runs GStreamer.
    GMainLoop* loop_;

    // The pointer that holds the playback pipeline's location.
    GstElement* pipeline_;

    // The pointer that leads to the ReplayGain element.
    GstElement* replay_gain_;

    // The pointer that leads to the ReplayGain limiter element.
    GstElement* replay_gain_limiter_;

    // The pointer that holds the sink element of the pipeline.
    GstElement* sink_;

    // List of all of the tracks queued for playback. 
    std::list<std::shared_ptr<TrackBin>> track_queue_;

    // The old trackbins that need to be destroyed from the main thread.
    std::vector<std::shared_ptr<TrackBin>> trackbins_trash_;

    // The pointer that holds the volume element's location.
    GstElement* volume_;





  //                  //
  // State Indicators /////////////////////////////////////////////////////////
  //                  //
 
  private:

    // Stores whether the backend is quitting or not.
    bool backend_quitting_;

    // Indicates the sink needs to be changed when playback next starts.
    std::atomic<bool> change_sink_;

    // 
    std::atomic<bool> delete_playback_queue_row_;

    // Stores whether the playback is at the first track to play since stopped
    // or not.
    std::atomic<bool> first_track_;

    // Stores whether playback is paused or not.
    bool paused_;

    // Only allows Stop(), Pause(), or Play() runs to happen one at a time.
    std::atomic<bool> playback_state_change_;

    // Stores whether the preparing trackbins thread is active or not.
    std::atomic<bool> preparing_trackbins_thread_active_; 

    // Stores whether to quit the track queueing process partway through.
    std::atomic<bool> quit_queue_;

    // Stores whether the track queue is being reset.
    std::atomic<bool> resetting_track_queue_;

    // Stores whether to stop the song after the current track.
    std::atomic<bool> stop_after_current_track_; 

    // Stores whether the track queue is active or not.
    std::atomic<bool> track_queue_active_;

    // Stores whether the track queue is full or not.
    std::atomic<bool> track_queue_full_;

    // Stores whether the track queue has been run at least once during the
    // current track's lifetime.
    std::atomic<bool> track_queue_ran_;





  //              //
  // Time Storage /////////////////////////////////////////////////////////////
  //              //

  private:

    // Stores the duration of the playback pipeline.
    std::atomic<long long> duration_;

    // Stores the position of the playback pipeline.
    std::atomic<long long> position_;





  //        //
  // Tracks ///////////////////////////////////////////////////////////////////
  //        //

  private: 

    // Stores a pointer to the empty track's instance.
    Track* empty_track_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
