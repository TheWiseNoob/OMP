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

#include "StatusBar.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"

#include "../Configuration/Configuration.h"

#include "../Metadata/TimeConversion.h"

#include "../Metadata/Track.h"

#include "../Playback/Playback.h"

#include "Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "Elements/Playlists/Playlists.h"

#include "Elements/Playlists/PlaylistTreeStore.h"

#include "GUI.h"

#include "Tagview.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gdkmm/rectangle.h>

#include <gdkmm/rgba.h>

#include <glibmm/main.h>

#include <gtkmm/box.h>

#include <gtkmm/eventbox.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/separator.h>

#include <gtkmm/treerowreference.h>

#include <gtkmm/volumebutton.h>

#include <iostream>

#include <vector>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

using namespace std;





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

StatusBar::StatusBar(Base& base_ref)

// Inherited Class

: Parts(base_ref)



// General

, inner_box_(Gtk::manage(new Gtk::Box))

, event_box_(Gtk::manage(new Gtk::EventBox))

, status_bar_frame_(Gtk::manage(new Gtk::Frame))



// Playback Status

, playback_status_image_(Gtk::manage(new Gtk::Image))

, playback_status_label_(Gtk::manage(new Gtk::Label("Stopped")))



// Playing Track Time

, time_label_box_(Gtk::manage(new Gtk::Box))

, time_left_label_(Gtk::manage(new Gtk::Label("00:00.00", 0.5, 0.5, false)))

, time_right_label_(Gtk::manage(new Gtk::Label("00:00.00", 0.5, 0.5, false)))



// Selected Rows Time

, selected_rows_count_label_(Gtk::manage(new Gtk::Label("0")))

, selected_time_label_(Gtk::manage(new Gtk::Label()))



// Volume

, main_volume_button_(Gtk::manage(new Gtk::VolumeButton))

, main_volume_button_box_(Gtk::manage(new Gtk::Box))

{

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  // Adds the Status Bar's event box to the status_bar_frame_.
  status_bar_frame_ -> add(*event_box_);

  // Adds a box to the event_box_.
  event_box_ -> add(*inner_box_);

  // Sets the orientation of the inner inner_box_.
  inner_box_ -> set_orientation(Gtk::Orientation::HORIZONTAL);



  // Sets the upper margin of the inner_box_.
  inner_box_ ->set_margin_top(0);

  // Sets the lower margin of the inner_box_.
  inner_box_ -> set_margin_bottom(0);

  // Sets the end margin of the inner_box_.
  inner_box_ -> set_margin_end(0);

  // Sets the start margin of the inner_box_.
  inner_box_ -> set_margin_start(0);



  // 
  inner_box_ -> set_tooltip_text
    ("Double click this status bar to select the playing track.");





  //                 //
  // Volume Creation //////////////////////////////////////////////////////////
  //                 //

  // Adds the volume box to the header bar.
  inner_box_ -> append(*main_volume_button_box_, Gtk::PACK_SHRINK);

  // Creates a Gtk::Sepratator pointer.
  Gtk::Separator* separator;

  // Creates the Gtk::Separator object.
  separator = Gtk::manage(new Gtk::Separator);

  // Sets the separator orientation to vertical.
  separator -> set_orientation(Gtk::Orientation::VERTICAL);

  // Adds the separator to the start of the header bar.
  inner_box_ -> append(*separator, Gtk::PACK_SHRINK);

  // Sets the left margin of the main_volume_button_box_.
  main_volume_button_box_ -> set_margin_left(5);

  // Sets the right margin of the main_volume_button_box_.
  main_volume_button_box_ -> set_margin_right(5);

  // Adds main_volume_button_ to main_volume_button_box_.
  main_volume_button_box_ -> set_center_widget(*main_volume_button_);

  // Sets the value of the main_volume_button_.
  main_volume_button_ -> set_value(base().config().get("playback.volume"));

  // Attachs the function called when the value of the button changes.
  main_volume_button_ -> signal_value_changed()
    . connect(sigc::mem_fun(*this, 
                           &StatusBar::On_Signal_Value_Changed_Main_Volume_Button));





  //                 //
  // Playback Status //////////////////////////////////////////////////////////
  //                 //

  // 
  inner_box_ -> append(*playback_status_image_, false, false, 3);

  // Adds the playback status label to inner_box_.
  inner_box_ -> append(*playback_status_label_, Gtk::PACK_SHRINK);



  // Sets the padding of the playback_status_label_. 
  playback_status_label_ -> set_margin_right(5);



  // 
  playback_status_image_ -> set_from_icon_name
    ("media-playback-stop-symbolic", Gtk::ICON_SIZE_LARGE_TOOLBAR);

  // 
  playback_status_image_
    -> set_tooltip_text("The current status of playback.");

  // 
  playback_status_label_
    -> set_tooltip_text("The current status of playback.");





  //                    //
  // Playing Track Time ///////////////////////////////////////////////////////
  //                    //

  // 
  time_left_label_ -> set_width_chars(9);

  // 
  time_right_label_ -> set_width_chars(9);



  // Sets the orientation of the time_label_box_.
  time_label_box_ -> set_orientation(Gtk::Orientation::VERTICAL);

  // Adds a label to the time_label_box_.
  time_label_box_ -> append(*time_left_label_, false, false, 5);

  // 
  time_label_box_
    -> set_center_widget(*Gtk::manage(new Gtk::Separator));

  // Adds a label to the time_label_box_.
  time_label_box_ -> pack_end(*time_right_label_, false, false, 5);

  // 
  time_label_box_ -> set_orientation(Gtk::Orientation::HORIZONTAL);

  // Adds the time_label_box_ to the Status Bar.
  inner_box_ -> set_center_widget(*time_label_box_);



  // Adds the selected tracks total time display label to inner_box_.
  inner_box_ -> pack_end(*selected_time_label_, Gtk::PACK_SHRINK);

  // Sets the padding of the selected_time_label_. 
  selected_time_label_ -> set_padding(10, 10);

  // Sets the default time of the selected_time_label_.
  selected_time_label_ -> set_label("00:00.00");

  // 
  selected_time_label_ -> set_tooltip_text
    ("Total added time of all tracks selected in all playlists.");

  // Adds the selected tracks total time display label to inner_box_.
  inner_box_ -> pack_end(*Gtk::manage(new Gtk::Separator()),
                              Gtk::PACK_SHRINK);

  // Adds the selected tracks total time display label to inner_box_.
  inner_box_ -> pack_end(*selected_rows_count_label_, Gtk::PACK_SHRINK);

  // Sets the padding of the selected_time_label_. 
  selected_rows_count_label_ -> set_padding(10, 10);

  // 
  selected_rows_count_label_
    -> set_tooltip_text("Total selected tracks in all playlists.");



  // Creates a slot for the time display.
  sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,
      &StatusBar::Display_Time), 0);

  // Uses the slot to make a Glib::signal_timeout.
  sigc::connection conn
    = Glib::signal_timeout().connect(my_slot, 20, Glib::PRIORITY_HIGH);



  // Enables overriding of the enter event for the event_box_.
  event_box_ -> set_events(Gdk::ENTER_NOTIFY_MASK);

  // Enables overriding of the leave event for the event_box_.
  event_box_ -> set_events(Gdk::LEAVE_NOTIFY_MASK);

  // Enables overriding of the button press event for the 
  // event_box_.
  event_box_ -> set_events(Gdk::BUTTON_PRESS_MASK);

  // Enables overriding of the button release event for the 
  // event_box_.
  event_box_ -> set_events(Gdk::BUTTON_RELEASE_MASK);



  // Overrides the enter event for the event_box_.
  event_box_ -> signal_enter_notify_event()
    . connect(sigc::mem_fun(*this, &StatusBar::Status_Bar_Event_Box_Enter));

  // Overrides the leave event for the event_box_.
  event_box_ -> signal_leave_notify_event()
    . connect(sigc::mem_fun(*this, &StatusBar::Status_Bar_Event_Box_Leave));

  // Overrides the button press event for the event_box_.
  event_box_ -> signal_button_press_event()
    . connect(sigc::mem_fun(*this, &StatusBar::Status_Bar_Event_Box_Button_Press));

  // Overrides the button release event for the event_box_.
  event_box_ -> signal_button_release_event()
    . connect(sigc::mem_fun(*this, &StatusBar::Status_Bar_Event_Box_Button_Release));



  // Sets the visibility of the Status Bar based on the
  if(config() . get("gui.hide_status_bar"))
  {

    // 
    status_bar_frame_ -> hide();

  }

  // 
  else
  {

    // 
    status_bar_frame_ -> show();

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

StatusBar::~StatusBar()
{

}






//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

bool StatusBar::Display_Time(int timeout_number)
{

  // Ends the Glib::signal_timeout if the program is closing.
  if(base().playback().backend_quitting())
  {

    // Ends the Glib::signal_timeout.
    return false;

  }




  // Sets the StatusBar time if music is playing.
  if((!(playback().Stopped())))
  {

    // Creates position string pointer and assigns it a new object of the 
    // current position.
    std::string* position_str_ptr
      = base().time_converter()
                 .Nanoseconds_To_Time_Format(playback().Position());

    // Creates duration string pointer and assigns it a new object of the 
    // current duration.
    std::string* duration_str_ptr
      = base().time_converter()
                 .Nanoseconds_To_Time_Format(playback().Duration());

    // Sets time_label_ with final_time_string.
    time_left_label_ -> set_label(*position_str_ptr);

    // Sets time_label_ with final_time_string.
    time_right_label_ -> set_label(*duration_str_ptr);

    // Deletes the position string memeory.
    delete position_str_ptr;

    // Deletes the duration string memeory.
    delete duration_str_ptr;

  }

  else if(playlists() . changing_track())
  {

    debug("hanging track");

  }

  // Is true if playback is stopped.
  else
  { 

    // Sets the time label to the zero value.
    time_left_label_ -> set_label("00:00.00");

    // Sets the time label to the zero value.
    time_right_label_ -> set_label("00:00.00");

    // Lets the timeout propagate.
    return true;

  }


  // Lets the timeout propagate.
  return true;


}

void StatusBar::On_Signal_Value_Changed_Main_Volume_Button(double new_volume)
{

  // Sets the new volume level in the configuration
  base().config().set("playback.volume", new_volume);

  // Writes the configuration to the file.
  base().config().write_file();



  // Sets volume level of the volume element in the playback pipeline.
  g_object_set (G_OBJECT(playback().volume()),
                "volume", gdouble(config().get("playback.volume")),
                NULL);

}

void StatusBar::Playback_Status_Paused()
{

  // 
  playback_status_image_ -> set_from_icon_name
    ("media-playback-pause-symbolic", Gtk::ICON_SIZE_LARGE_TOOLBAR);




  //
  playback_status_label_ -> set_text("Paused");

}

void StatusBar::Playback_Status_Playing()
{

  // 
  playback_status_image_ -> set_from_icon_name
    ("media-playback-start-symbolic", Gtk::ICON_SIZE_LARGE_TOOLBAR);




  //
  playback_status_label_ -> set_text("Playing");

}

void StatusBar::Playback_Status_Stopped()
{

  // 
  playback_status_image_ -> set_from_icon_name
    ("media-playback-stop-symbolic", Gtk::ICON_SIZE_LARGE_TOOLBAR);




  //
  playback_status_label_ -> set_text("Stopped");

}

bool StatusBar::Status_Bar_Event_Box_Button_Press(GdkEventButton* event)
{

  // Makes a new Gdk::RGBA.
  Gdk::RGBA inner_box_rgba;

  // Sets the RGBA to black and 24% opacity.
  inner_box_rgba.set_rgba(0.0,0.0,0.0,0.24);

  // Sets the inner_box_ to be the color of the new RGBA.
  inner_box_ -> override_background_color(inner_box_rgba,
                                               Gtk::STATE_FLAG_NORMAL);



  // Makes the function do nothing if nothing is playing.
  if(!(playback() . Playing()))
  { 

    // 
    return false;

  }



  // Is true if the StatusBar was double-clicked.
  if(((event -> type) == GDK_2BUTTON_PRESS)
       || ((event -> type) == GDK_3BUTTON_PRESS))
  { 

    // 
    bool double_button_press = true;

    // 
    if((event -> type) == GDK_3BUTTON_PRESS)
    {

      // 
      double_button_press = false;

    }



    // Selects the playing track.
    playlists()
      . Select_Row(playlists() . playing_row_ref(), double_button_press);



    // Scrolls to the playing track.
    playlists()
      . Scroll_To_Row(playlists() . playing_row_ref(), double_button_press);



    // Gets the filename of the playing track.
    string temp_string = playlists() . playing_track() . filename();



    // Updates the Tagviews as Playing.
    if(playback() . Playing())
    {

      // Updates the Tagviews as Playing while including the playing track.
      gui() . Update_Tagview("Playing", playback() . playing_track());

    }

    // Updates the Tagviews as Paused.
    else if(playback() . Paused())
    {

      // Updates the Tagviews as Paused while including the paused track.
      gui() . Update_Tagview("Paused", playback() . playing_track());

    }

    // Updates the Tagviews as Idle.
    else
    { 

      // Sets the Tagviews as Idle.
      gui() . Update_Tagview("Idle", playback() . empty_track());

    }



    // Sets the cover art.
    gui() . Load_Cover_Art(temp_string);



    // Sets the selected time label.
    playlists()() . front() -> Add_Selected_Tracks_Times();



    // 
    playlists() . set_selected_row_ref(playlists() . playing_row_ref());



    // 
    playback() . Reset_Track_Queue();

  }



  // Ends the function call.
  return false;

}

bool StatusBar::Status_Bar_Event_Box_Button_Release(GdkEventButton* event)
{

  // Makes a new Gdk::RGBA.
  Gdk::RGBA inner_box_rgba;

  // Sets the RGBA to black and 12% opacity.
  inner_box_rgba.set_rgba(0.0,0.0,0.0,0.12);

  // Sets the inner_box_ to be the color of the new RGBA.
  inner_box_ -> override_background_color(inner_box_rgba,
                                               Gtk::STATE_FLAG_NORMAL);

  // Ends the function call.
  return false;

}

bool StatusBar::Status_Bar_Event_Box_Enter(GdkEventCrossing* event)
{

  // Makes a new Gdk::RGBA.
  Gdk::RGBA inner_box_rgba;

  // Sets the RGBA to black and 12% opacity.
  inner_box_rgba.set_rgba(0.0,0.0,0.0,0.12);

  // Sets the inner_box_ to be the color of the new RGBA.
  inner_box_ -> override_background_color(inner_box_rgba,
                                               Gtk::STATE_FLAG_NORMAL);

  // Ends the function call.
  return false;

}

bool StatusBar::Status_Bar_Event_Box_Leave(GdkEventCrossing* event)
{

  // Makes a new Gdk::RGBA.
  Gdk::RGBA inner_box_rgba;

  // Sets the RGBA to black and 0% opacity.
  inner_box_rgba.set_rgba(0.0,0.0,0.0,0.00);

  // Sets the inner_box_ to be the color of the new RGBA.
  inner_box_ -> override_background_color(inner_box_rgba,
                                          Gtk::STATE_FLAG_NORMAL);

  // Ends the function call.
  return false;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::Frame& StatusBar::status_bar_frame()
{

  return *status_bar_frame_;

}





//         //
//         //
// Setters ////////////////////////////////////////////////////////////////////
//         //
//         //

void StatusBar::set_selected_rows_count_label(int new_row_count)
{ 

  // Sets the label used for displaying the amount of selected rows.
  selected_rows_count_label_ -> set_label(to_string(new_row_count));

}

void StatusBar::set_selected_time_label(const char* new_time)
{ 

  // Sets the label used for displaying the summed time of the selected tracks.
  selected_time_label_ -> set_label(new_time); 

}

