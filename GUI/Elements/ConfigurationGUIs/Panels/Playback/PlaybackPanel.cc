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

#include "PlaybackPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../../../Configuration/Configuration.h"

#include "../../../../../GUI/GUI.h"

#include "../../../../../Playback/Playback.h"

#include "../../../MainMenus/MainMenu.h"

#include "../../../MainMenus/MainMenus.h"

#include "../../ConfigurationGUI.h"

#include "../../ConfigurationGUIs.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/box.h>

#include <gtkmm/checkmenuitem.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/radiobutton.h>

#include <gtkmm/radiomenuitem.h>





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

PlaybackPanel::PlaybackPanel(Base& base_ref, 
                             ConfigurationGUI& new_config_gui)

// Inherited Class

: Panel(base_ref, new_config_gui, "Playback")



// General

, looping_and_order_box_(Gtk::manage(new Gtk::Box))



// Looping

, looping_box_(Gtk::manage(new Gtk::Box))

, looping_frame_(Gtk::manage(new Gtk::Frame))

, none_looping_radio_button_(Gtk::manage(new Gtk::RadioButton))

, track_looping_radio_button_(Gtk::manage(new Gtk::RadioButton))

, playlist_looping_radio_button_(Gtk::manage(new Gtk::RadioButton))



// Options

, options_box_(Gtk::manage(new Gtk::Box))

, left_options_box_(Gtk::manage(new Gtk::Box))

, right_options_box_(Gtk::manage(new Gtk::Box))

, cursor_follows_playback_check_button_(Gtk::manage(new Gtk::CheckButton))

, playback_follows_cursor_check_button_(Gtk::manage(new Gtk::CheckButton))

, queue_check_button_(Gtk::manage(new Gtk::CheckButton))

, queue_saved_check_button_(Gtk::manage(new Gtk::CheckButton))

, selected_playlist_only_check_button_(Gtk::manage(new Gtk::CheckButton))

, start_at_pregap_check_button_(Gtk::manage(new Gtk::CheckButton))

, stop_after_current_track_check_button_(Gtk::manage(new Gtk::CheckButton))



// Order

, order_box_(Gtk::manage(new Gtk::Box))

, order_frame_(Gtk::manage(new Gtk::Frame))

, normal_order_radio_button_(Gtk::manage(new Gtk::RadioButton))

, shuffle_tracks_order_radio_button_(Gtk::manage(new Gtk::RadioButton))

, random_tracks_order_radio_button_(Gtk::manage(new Gtk::RadioButton))

{

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  // 
  box() . append(*options_box_);

  // 
  box() . append(*looping_and_order_box_);



  // 
  box() . set_orientation(Gtk::Orientation::VERTICAL);

  // 
  looping_and_order_box_ -> set_orientation(Gtk::Orientation::HORIZONTAL);





  //         //
  // Looping //////////////////////////////////////////////////////////////////
  //         //

  // 
  looping_and_order_box_
    -> append(*looping_frame_);

  // 
  looping_frame_ -> add(*looping_box_);

  // 
  looping_box_
    -> append(*none_looping_radio_button_, Gtk::PACK_EXPAND_PADDING);

//  // 
//  looping_box_
//    -> append(*track_looping_radio_button_, Gtk::PACK_EXPAND_PADDING);

  // 
  looping_box_
    -> append(*playlist_looping_radio_button_, Gtk::PACK_EXPAND_PADDING);



  // 
  looping_frame_ -> set_label("Looping");



  // 
  looping_frame_ -> set_margin_top(3);

  // 
  looping_frame_ -> set_margin_bottom(3);

  // 
  looping_frame_ -> set_margin_start(3);

  // 
  looping_frame_ -> set_margin_end(3);



  // 
  looping_box_ -> set_orientation(Gtk::Orientation::HORIZONTAL);



  //
  none_looping_radio_button_ -> set_tooltip_text
    ("No looping occurs. Playback will stop if the end of the playlist is " \
     "reached.");

  //
  playlist_looping_radio_button_ -> set_tooltip_text
    ("Playlist looping occurs. Playback will loop to the beginning of the " \
     "playlist if the end of the playlist is reached.");



  // 
  track_looping_radio_button_
    -> join_group(*none_looping_radio_button_);

  // 
  playlist_looping_radio_button_
    -> join_group(*none_looping_radio_button_);



  // 
  none_looping_radio_button_ -> set_label("None");

  // 
  track_looping_radio_button_ -> set_label("Repeat Track");

  // 
  playlist_looping_radio_button_ -> set_label("Repeat Playlist");



  // 
  none_looping_radio_button_ -> set_margin_bottom(3);

  // 
  playlist_looping_radio_button_ -> set_margin_bottom(3);





  //         //
  // Options //////////////////////////////////////////////////////////////////
  //         //

  // 
  options_box_ -> append(*left_options_box_);

  // 
  options_box_ -> append(*right_options_box_);



  // 
  left_options_box_ -> append(*cursor_follows_playback_check_button_,
                                  Gtk::PACK_EXPAND_PADDING);

  // 
  left_options_box_ -> append(*selected_playlist_only_check_button_,
                                  Gtk::PACK_EXPAND_PADDING);

  // 
  left_options_box_ -> append(*playback_follows_cursor_check_button_,
                                  Gtk::PACK_EXPAND_PADDING);

  // 
  right_options_box_ -> append(*start_at_pregap_check_button_,
                                   Gtk::PACK_EXPAND_PADDING);

  // 
  right_options_box_ -> append(*queue_saved_check_button_,
                                    Gtk::PACK_EXPAND_PADDING);

  // 
  right_options_box_ -> append(*stop_after_current_track_check_button_,
                                   Gtk::PACK_EXPAND_PADDING);



  // 
  options_box_ -> set_orientation(Gtk::Orientation::HORIZONTAL);

  // 
  left_options_box_ -> set_orientation(Gtk::Orientation::VERTICAL);

  // 
  right_options_box_ -> set_orientation(Gtk::Orientation::VERTICAL);



  // 
  options_box_ -> set_margin_start(3);

  // 
  options_box_ -> set_margin_end(3);

  // 
  selected_playlist_only_check_button_ -> set_margin_start(20);



  // 
  cursor_follows_playback_check_button_
    -> set_label("Cursor Follows Playback");

  // 
  selected_playlist_only_check_button_
    -> set_label("Selected Playlist View Only");

  // 
  playback_follows_cursor_check_button_
    -> set_label("Playback Follows Cursor");

  // 
  queue_check_button_ -> set_label("Enable Track Queue");

  // 
  queue_saved_check_button_ -> set_label("Save Playback Queue");

  // 
  start_at_pregap_check_button_ -> set_label("Start At Pregap Start");

  // 
  stop_after_current_track_check_button_
    -> set_label("Stop After Current Track");



  // 
  cursor_follows_playback_check_button_
    -> set_tooltip_text("Makes the playlist cursor follow playback movement " \
                        "if the playing playlist is active.");

  // 
  selected_playlist_only_check_button_ -> set_tooltip_text
    ("Only the currently selected playlist view will have its cursor " \
     "changed to the next track when Cursor Follows Plackback is on.");

  // 
  playback_follows_cursor_check_button_
    -> set_tooltip_text("If a track is selected while this is enabled, " \
                        "playback will move to that track after the current " \
                        "track is finished playing.");

  // 
  queue_saved_check_button_
    -> set_tooltip_text
         ("If enabled, the playback queue is saved when OMP closes.");

  // 
  start_at_pregap_check_button_
    -> set_tooltip_text
         ("OMP will start playback at the beginning of a " \
          "track's pregap if the pregap exists and this " \
          "setting is enabled.\n\n" \
          "A pregap is the occasional extra time of a track at the " \
          "beginning of CD's track. The pregap appears as negative time " \
          "on a CD player and usually requires the CD being reound from the " \
          "beginning of a track.");

  // 
  stop_after_current_track_check_button_
    -> set_tooltip_text("Playback stops after the currently playing " \
                        "track ends.");



  // 
  track_looping_radio_button_ -> set_sensitive(false);  

  // 
  shuffle_tracks_order_radio_button_ -> set_sensitive(false);  

  // 
  random_tracks_order_radio_button_ -> set_sensitive(false);  



  // 
  cursor_follows_playback_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &PlaybackPanel
            ::On_Cursor_Follows_Playback_Check_Button_Toggled_Signal));

  // 
  playback_follows_cursor_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &PlaybackPanel
            ::On_Playback_Follows_Cursor_Check_Button_Toggled_Signal));

  // 
  queue_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &PlaybackPanel::On_Queue_Check_Button_Toggled_Signal));

  // 
  queue_saved_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &PlaybackPanel::On_Queue_Saved_Check_Button_Toggled_Signal));

  // 
  selected_playlist_only_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun(*this, &PlaybackPanel::Selected_Playlist_Only));

  // 
  start_at_pregap_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &PlaybackPanel::On_Start_At_Pregap_Check_Button_Toggled_Signal));

  // 
  stop_after_current_track_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &PlaybackPanel
                   ::On_Stop_After_Current_Track_Check_Button_Toggled_Signal));



  // 
  none_looping_radio_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &PlaybackPanel
                   ::On_Looping_Radio_Button_Toggled_Signal));





  //       //
  // Order ////////////////////////////////////////////////////////////////////
  //       //

  // 
//  looping_and_order_box_
//    -> append(*order_frame_);

  // 
  order_frame_ -> add(*order_box_);

  // 
  order_box_
    -> append(*normal_order_radio_button_, Gtk::PACK_EXPAND_PADDING);

  // 
  order_box_
    -> append(*shuffle_tracks_order_radio_button_, Gtk::PACK_EXPAND_PADDING);

  // 
  order_box_
    -> append(*random_tracks_order_radio_button_, Gtk::PACK_EXPAND_PADDING);



  // 
  order_frame_ -> set_label("Order");



  // 
  order_frame_ -> set_margin_top(3);

  // 
  order_frame_ -> set_margin_bottom(3);

  // 
  order_frame_ -> set_margin_start(3);

  // 
  order_frame_ -> set_margin_end(3);



  // 
  order_box_ -> set_orientation(Gtk::Orientation::VERTICAL);



  // 
  shuffle_tracks_order_radio_button_
    -> join_group(*normal_order_radio_button_);
  // 
  random_tracks_order_radio_button_
    -> join_group(*normal_order_radio_button_);



  // 
  normal_order_radio_button_ -> set_label("Normal");

  // 
  shuffle_tracks_order_radio_button_ -> set_label("Shuffle Tracks");

  // 
  random_tracks_order_radio_button_ -> set_label("Random Tracks");





  //       //
  // Other ////////////////////////////////////////////////////////////////////
  //       //

  // Adds the Apply_Saved_Value function to the ConfigGUI's list.
  new_config_gui . Add_Apply_Saved_Value_Function(*this,
                                                  &Panel::Apply_Saved_Values);



  // 
  box() . show_all_children();

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaybackPanel::~PlaybackPanel()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void PlaybackPanel::Apply_Saved_Values()
{

  // 
  bool active;





  //                         //
  // Cursor Follows Playback //
  //                         //

  // 
  active = config() . get("playback.cursor_follows_playback");

  // 
  cursor_follows_playback_check_button_ -> set_active(active);





  //         //
  // Looping //
  //         //

  // 
  string looping_type = config() . get("playback.looping");

  // 
  if(looping_type == "none")
  {

    // 
    none_looping_radio_button_ -> set_active(true);

  }

  // 
  else if(looping_type == "playlist")
  {

    // 
    playlist_looping_radio_button_ -> set_active(true);

  }





  //                         //
  // Playback Follows Cursor //
  //                         //

  // 
  active = config() . get("playback.playback_follows_cursor");

  // 
  playback_follows_cursor_check_button_ -> set_active(active);





  //             //
  // Queue Saved //
  //             //

  // 
  active = config() . get("playback.queue_saved");

  // 
  queue_saved_check_button_ -> set_active(active);





  //                        //
  // Selected Playlist Only //
  //                        //

  // 
  active = config() . get("playback.selected_playlist_only");

  // 
  selected_playlist_only_check_button_ -> set_active(active);






  //                 //
  // Start At Pregap //
  //                 //

  // 
  active = config() . get("playback.start_at_pregap");

  // 
  start_at_pregap_check_button_ -> set_active(active);



  //                    //
  // Stop After Current //
  //                    //

  // 
  active = playback() . stop_after_current_track();

  // 
  stop_after_current_track_check_button_ -> set_active(active);

}





//         //
// Looping ////////////////////////////////////////////////////////////////////
//         //

void PlaybackPanel::On_Looping_Radio_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  // 
  if(none_looping_radio_button_ -> get_active())
  {

    // 
    config() . set("playback.looping", "none");

  }

  // 
  else if(playlist_looping_radio_button_ -> get_active())
  {

    // 
    config() . set("playback.looping", "playlist");

  }



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    if(none_looping_radio_button_ -> get_active())
    {

      // 
      config_guis_it -> playback_panel() . none_looping_radio_button()
        . set_active(true);

    }

    // 
    else if(playlist_looping_radio_button_ -> get_active())
    {

      // 
      config_guis_it -> playback_panel() . playlist_looping_radio_button()
        . set_active(true);

    }

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : main_menus()())
  { 

    // 
    if(none_looping_radio_button_ -> get_active())
    {

      // 
      menu_bars_it -> looping_none_playback_radio_menu_item()
        . set_active(true);

    }

    // 
    else if(playlist_looping_radio_button_ -> get_active())
    {

      // 
      menu_bars_it -> looping_playlist_playback_radio_menu_item()
        . set_active(true);

    }

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);



  // 
  playback() . Reset_Track_Queue();

}





//         //
// Options ////////////////////////////////////////////////////////////////////
//         //

void PlaybackPanel::On_Cursor_Follows_Playback_Check_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  // 
  bool active = cursor_follows_playback_check_button_ -> get_active();



  // 
  config() . set("playback.cursor_follows_playback", active);



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . cursor_follows_playback_check_button()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : main_menus()())
  { 

    // 
    menu_bars_it -> cursor_follows_playback_playback_check_menu_item()
      . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}

void PlaybackPanel::On_Playback_Follows_Cursor_Check_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  //
  playback() . Reset_Track_Queue();



  // 
  bool active = playback_follows_cursor_check_button_ -> get_active();



  // 
  config() . set("playback.playback_follows_cursor", active);



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . playback_follows_cursor_check_button()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : main_menus()())
  { 

    // 
    menu_bars_it -> playback_follows_cursor_playback_check_menu_item()
      . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}

void PlaybackPanel::On_Queue_Check_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  //
  playback() . Reset_Track_Queue();



  // 
  bool active = queue_check_button_ -> get_active();



  // 
  config() . set("playback.queue", active);



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . queue_check_button()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : main_menus()())
  { 

    // 
    menu_bars_it -> queue_playback_check_menu_item()
      . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}

void PlaybackPanel::On_Queue_Saved_Check_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  // 
  bool active = queue_saved_check_button_ -> get_active();



  // 
  config() . set("playback.queue_saved", active);



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . queue_saved_check_button()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : main_menus()())
  { 

    // 
    menu_bars_it -> queue_saved_playback_check_menu_item()
      . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}

void PlaybackPanel::On_Start_At_Pregap_Check_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  // 
  bool active = start_at_pregap_check_button_ -> get_active();



  // 
  config() . set("playback.start_at_pregap", active);



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . start_at_pregap_check_button()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : main_menus()())
  { 

    // 
    menu_bars_it -> start_at_pregap_check_menu_item()
      . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);



  // 
  playback() . Reset_Track_Queue();

}

void PlaybackPanel::On_Stop_After_Current_Track_Check_Button_Toggled_Signal()
{

  // 
  bool active = stop_after_current_track_check_button_ -> get_active();



  // 
  playback() . stop_after_current_track() = active;



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . stop_after_current_track_check_button()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : main_menus()())
  { 

    // 
    menu_bars_it -> stop_after_current_track_playback_check_menu_item()
      . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}

void PlaybackPanel::Selected_Playlist_Only()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  // 
  bool active = selected_playlist_only_check_button_ -> get_active();



  // 
  config() . set("playback.selected_playlist_only", active);



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . selected_playlist_only_check_button()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : main_menus()())
  { 

    // 
    menu_bars_it -> selected_playlist_only_check_menu_item()
      . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

//         //
// Looping ////////////////////////////////////////////////////////////////////
//         //

Gtk::RadioButton& PlaybackPanel::none_looping_radio_button()
{

  // 
  return *none_looping_radio_button_;

}

Gtk::RadioButton& PlaybackPanel::playlist_looping_radio_button()
{

  // 
  return *playlist_looping_radio_button_;

}





//         //
// Options ////////////////////////////////////////////////////////////////////
//         //

Gtk::CheckButton& PlaybackPanel::cursor_follows_playback_check_button()
{

  return *cursor_follows_playback_check_button_;

}

Gtk::CheckButton& PlaybackPanel::playback_follows_cursor_check_button()
{

  return *playback_follows_cursor_check_button_;

}

Gtk::CheckButton& PlaybackPanel::queue_check_button()
{

  return *queue_check_button_;

}

Gtk::CheckButton& PlaybackPanel::queue_saved_check_button()
{

  return *queue_saved_check_button_;

}

Gtk::CheckButton& PlaybackPanel::selected_playlist_only_check_button()
{ 

  return *selected_playlist_only_check_button_;

}

Gtk::CheckButton& PlaybackPanel::start_at_pregap_check_button()
{

  return *start_at_pregap_check_button_;

}

Gtk::CheckButton& PlaybackPanel::stop_after_current_track_check_button()
{

  return *stop_after_current_track_check_button_;

}
