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

#include "MainMenu.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../../Playback/Playback.h"

#include "../../StatusBar.h"

#include "../ChildWindows/ChildWindow.h"

#include "../Abouts/Abouts.h"

#include "../ConfigurationGUIs/ConfigurationGUIs.h"

#include "../ConfigurationGUIs/Panels/GUI/GUIPanel.h"

#include "../ConfigurationGUIs/Panels/Playback/PlaybackPanel.h"

#include "../FileChoosers/FileChoosers.h"

#include "../PlaylistComboBoxes/PlaylistComboBox.h"

#include "../PlaylistComboBoxes/PlaylistComboBoxes.h"

#include "../Playlists/Playlists.h"

#include "../../GUI.h"

#include "MainMenus.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <gtkmm/accelmap.h>

#include <gtkmm/box.h>

#include <gtkmm/checkmenuitem.h>

#include <gtkmm/frame.h>

#include <gtkmm/menu.h>

#include <gtkmm/menubar.h>

#include <gtkmm/menuitem.h>

#include <gtkmm/notebook.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/separatormenuitem.h>

#include <gtkmm/treerowreference.h>

#include <string>





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

MainMenu::MainMenu(Base& base_ref, MainMenus& main_menus_ref,
                   bool normal_menu)

// Inherited Class

: GUIElement(base_ref, main_menus_ref())



// General

, menu_(Gtk::manage(new Gtk::Menu))

, menu_bar_(Gtk::manage(new Gtk::MenuBar))



// About

, about_menu_(Gtk::manage(new Gtk::Menu))

, about_menu_item_(Gtk::manage(new Gtk::MenuItem("_About", true)))

, about_omp_menu_item_(Gtk::manage(new Gtk::MenuItem("_About OMP", true)))



// Edit

, configuration_menu_item_
    (Gtk::manage(new Gtk::MenuItem("_Configuration", true)))

, edit_menu_(Gtk::manage(new Gtk::Menu))

, edit_menu_item_(Gtk::manage(new Gtk::MenuItem("_Edit", true)))



// File

, add_file_menu_item_(Gtk::manage(new Gtk::MenuItem("_Add File(s)", true)))

, file_menu_(Gtk::manage(new Gtk::Menu))

, file_menu_item_(Gtk::manage(new Gtk::MenuItem("_File", true)))

, quit_menu_item_(Gtk::manage(new Gtk::MenuItem("_Quit OMP", true)))



// Library

, library_menu_(Gtk::manage(new Gtk::Menu))

, library_menu_item_(Gtk::manage(new Gtk::MenuItem("_Library", true)))



// Playback : General

, playback_menu_(Gtk::manage(new Gtk::Menu))

, playback_menu_item_(Gtk::manage(new Gtk::MenuItem("_Playback", true)))




// Playback : Looping

, looping_none_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("None")))

, looping_playback_menu_(Gtk::manage(new Gtk::Menu))

, looping_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("_Looping", true)))

, looping_playlist_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Repeat Playlist")))

, looping_track_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Repeat Track")))



// Playback : Options

, cursor_follows_playback_playback_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("_Cursor Follows Playback", true)))

, flush_queue_playback_menu_item_(Gtk::manage(new Gtk::MenuItem
    ("_Flush Playback Queue", true)))

, playback_follows_cursor_playback_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("_Playback Follows Cursor", true)))

, queue_playback_check_menu_item_(Gtk::manage(new Gtk::CheckMenuItem
    ("_Queue", true)))

, queue_saved_playback_check_menu_item_(Gtk::manage(new Gtk::CheckMenuItem
    ("Saved Playback _Queue", true)))

, selected_playlist_only_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Selected Playlist View Only", true)))

, start_at_pregap_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Start At Pregap", true)))

, stop_after_current_track_playback_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("_Stop After Current Track", true)))



// Playback : Order

, order_normal_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Normal")))

, order_playback_menu_(Gtk::manage(new Gtk::Menu))

, order_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("_Order", true)))

, order_random_tracks_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Random Tracks")))

, order_shuffle_tracks_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Shuffle Tracks")))



// Playback : State Changers

, next_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("Next", true)))

, pause_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("Pause", true)))

, play_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("Play", true)))

, stop_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("Stop", true)))



// View MenuItems

, hide_duplicates_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Hide _Duplicates", true)))

, hide_header_bar_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Hide _Header Bar", true)))

, hide_status_bar_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Hide _Status Bar", true)))

, hide_tabs_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Hide _Tabs", true)))

, view_menu_(Gtk::manage(new Gtk::Menu))

, view_menu_item_(Gtk::manage(new Gtk::MenuItem("_View", true)))

{

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  // 
  main_menus_ref() . push_front(this);

  // Adds the new FileChooser's to the iterator to it's it storage variable.
  set_gui_elements_it(main_menus_ref() . begin());





  // 
  box() . set_orientation(Gtk::Orientation::HORIZONTAL);



  // 
  Gtk::Box* new_vert_box_ptr = Gtk::manage(new Gtk::Box);

  // 
  new_vert_box_ptr -> set_orientation(Gtk::Orientation::VERTICAL);



  // 
  box() . set_center_widget(*new_vert_box_ptr);

  // 
  new_vert_box_ptr -> set_center_widget(*menu_bar_);




  //                //
  // Main MenuItems ///////////////////////////////////////////////////////////
  //                // 

  // 
  if(normal_menu)
  {

    // 
    menu_ -> append(*file_menu_item_);
  
    // 
    menu_ -> append(*edit_menu_item_);
  
    // 
    menu_ -> append(*view_menu_item_);
  
    // 
    menu_ -> append(*playback_menu_item_);
  
    // 
  //  menu_ -> append(*library_menu_item_);
  
    // 
    menu_ -> append(*about_menu_item_);

  }

  // 
  else
  {

    // 
    menu_bar_ -> append(*file_menu_item_);

    // 
    menu_bar_ -> append(*edit_menu_item_);

    // 
    menu_bar_ -> append(*view_menu_item_);

    // 
    menu_bar_ -> append(*playback_menu_item_);

    // 
  //  menu_bar_ -> append(*library_menu_item_);

    // 
    menu_bar_ -> append(*about_menu_item_);

  }



  // 
  file_menu_item_ -> set_submenu(*file_menu_);

  // 
  edit_menu_item_ -> set_submenu(*edit_menu_);

  // 
  view_menu_item_ -> set_submenu(*view_menu_);

  // 
  playback_menu_item_ -> set_submenu(*playback_menu_);

  // 
//  library_menu_item_ -> set_submenu(*library_menu_);

  // 
  about_menu_item_ -> set_submenu(*about_menu_);





  //                 //
  // About MenuItems //////////////////////////////////////////////////////////
  //                 //

  // 
  about_menu_ -> append(*about_omp_menu_item_);



  // 
  about_menu_
    -> set_tooltip_text("Opens a window with some information about OMP.");

  // 
  about_menu_item_
    -> set_tooltip_text("Information about OMP and how to use it.");



  // 
  about_omp_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Open_About));





  //                //
  // Edit MenuItems ///////////////////////////////////////////////////////////
  //                //

  // 
  edit_menu_ -> append(*configuration_menu_item_);



  // 
  edit_menu_item_ -> set_tooltip_text
    ("Tools for editing with various parts of OMP");



  // 
  configuration_menu_item_ -> set_tooltip_text
    ("Opens a window with OMP's configuration GUI.");



  // 
  configuration_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Configuration));





  //                //
  // File MenuItems ///////////////////////////////////////////////////////////
  //                //

  // 
  file_menu_ -> append(*add_file_menu_item_);

  // 
  file_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  file_menu_ -> append(*quit_menu_item_);



  //
  file_menu_item_ -> set_tooltip_text
    ("File opening tools and the quit button.");



  // 
  add_file_menu_item_ -> set_tooltip_text
    ("Opens a window that is used to add tracks to the currently active " \
     "playlist view.");

  //
  quit_menu_item_ -> set_tooltip_text("Cleanly exits OMP.");



  // 
  quit_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Quit));

  // 
  add_file_menu_item_ -> signal_activate()
     . connect(sigc::mem_fun(*this, &MainMenu::Add_File));





  //                    //
  // Playback MenuItems ///////////////////////////////////////////////////////
  //                    //

  // 
  playback_menu_ -> append(*play_playback_menu_item_);

  // 
  playback_menu_ -> append(*pause_playback_menu_item_);

  // 
  playback_menu_ -> append(*stop_playback_menu_item_);

  // 
  playback_menu_ -> append(*next_playback_menu_item_);

  // 
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*looping_playback_menu_item_);

  // 
//  playback_menu_ -> append(*order_playback_menu_item_);



  // 
  playback_menu_item_
    -> set_tooltip_text("Playback related settings and tools.");



  // 
  stop_after_current_track_playback_check_menu_item_ -> set_active(false);



  // 
  order_playback_menu_item_ -> set_submenu(*order_playback_menu_);

  // 
  looping_playback_menu_item_ -> set_submenu(*looping_playback_menu_);



  // 
  looping_playback_menu_ -> append(*looping_none_playback_radio_menu_item_);

  // 
//  looping_playback_menu_ -> append(*looping_track_playback_radio_menu_item_);

  // 
  looping_playback_menu_ -> append(*looping_playlist_playback_radio_menu_item_);



  // 
  order_playback_menu_ -> append(*order_normal_playback_radio_menu_item_);

  // 
  order_playback_menu_ -> append(*order_random_tracks_playback_radio_menu_item_);

  // 
  order_playback_menu_ -> append(*order_shuffle_tracks_playback_radio_menu_item_);



  // 
  cursor_follows_playback_playback_check_menu_item_
    -> set_active(config() . get("playback.cursor_follows_playback"));

  // 
  playback_follows_cursor_playback_check_menu_item_
    -> set_active(config() . get("playback.playback_follows_cursor"));

  // 
  queue_saved_playback_check_menu_item_
    -> set_active(config() . get("playback.queue_saved"));

  // 
  selected_playlist_only_check_menu_item_
    -> set_active(config() . get("playback.selected_playlist_only"));

  // 
  start_at_pregap_check_menu_item_
    -> set_active(config() . get("playback.start_at_pregap"));



  // 
  looping_track_playback_radio_menu_item_
    -> join_group(*looping_none_playback_radio_menu_item_);

  // 
  looping_playlist_playback_radio_menu_item_
    -> join_group(*looping_none_playback_radio_menu_item_);

  // 
  order_random_tracks_playback_radio_menu_item_
    -> join_group(*order_normal_playback_radio_menu_item_);

  // 
  order_shuffle_tracks_playback_radio_menu_item_
    -> join_group(*order_normal_playback_radio_menu_item_);



  // 
  looping_track_playback_radio_menu_item_ -> set_sensitive(false);

  // 
  order_random_tracks_playback_radio_menu_item_ -> set_sensitive(false);

  // 
  order_shuffle_tracks_playback_radio_menu_item_ -> set_sensitive(false);



  // 
  string looping_type = config() . get("playback.looping");

  // 
  if(looping_type == "none")
  {

    // 
    looping_none_playback_radio_menu_item_ -> set_active(true);

  }

  // 
  else if(looping_type == "playlist")
  {

    // 
    looping_playlist_playback_radio_menu_item_ -> set_active(true);

  }



  // 
  order_normal_playback_radio_menu_item_ -> set_active(true);



  // 
  looping_none_playback_radio_menu_item_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &MainMenu::Change_Playback_Looping_Type));



  // 
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*cursor_follows_playback_playback_check_menu_item_);

  // 
  playback_menu_ -> append(*selected_playlist_only_check_menu_item_);

  // 
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*playback_follows_cursor_playback_check_menu_item_);

  // 
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*start_at_pregap_check_menu_item_);

  // 
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*queue_saved_playback_check_menu_item_);

  // 
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*stop_after_current_track_playback_check_menu_item_);

  // 
//  playback_menu_ -> append(*flush_queue_playback_menu_item_);



  // 
  play_playback_menu_item_ -> set_tooltip_text
    ("Starts playback at the selected track or the next track in the " \
     "playback queue if the playback queue is not empty.");

  // 
  pause_playback_menu_item_ -> set_tooltip_text("Pauses playback.");

  // 
  stop_playback_menu_item_ -> set_tooltip_text("Stops playback.");

  // 
  next_playback_menu_item_
    -> set_tooltip_text("Skips playback to the next track.");



  // 
  looping_playback_menu_item_ -> set_tooltip_text
    ("A list of options for what type of playback looping is occurring.");

  //
  looping_none_playback_radio_menu_item_ -> set_tooltip_text
    ("No looping occurs. Playback will stop if the end of the playlist is " \
     "reached.");

  //
  looping_playlist_playback_radio_menu_item_ -> set_tooltip_text
    ("Playlist looping occurs. Playback will loop to the beginning of the " \
     "playlist if the end of the playlist is reached.");



  // 
  stop_after_current_track_playback_check_menu_item_ -> set_tooltip_text
    ("Stops playback after the currently playing track has ended.");

  // 
  playback_follows_cursor_playback_check_menu_item_ -> set_tooltip_text
    ("Makes playback move to the currently selected track once the " \
     "current track has finished.");

  // 
  cursor_follows_playback_playback_check_menu_item_ -> set_tooltip_text
    ("The cursor automatically moves to the next track once it starts. ");

  // 
  selected_playlist_only_check_menu_item_ -> set_tooltip_text
    ("Only the currently selected playlist view will have its cursor " \
     "changed to the next track when Cursor Follows Plackback is on.");

  //  
  queue_saved_playback_check_menu_item_ -> set_tooltip_text
    ("Saves the tracks in the playback track queue when OMP is closed.");



  // 
  selected_playlist_only_check_menu_item_ -> set_margin_start(20);



  // 
  next_playback_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(playback(), &Playback::Next_Track));

  // 
  play_playback_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Play));

  // 
  pause_playback_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Pause));

  // 
  stop_playback_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Stop));



  // 
  start_at_pregap_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Start_At_Pregap_Start));

  // 
  stop_after_current_track_playback_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Stop_After_Current_Track));

  // 
  cursor_follows_playback_playback_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Cursor_Follows_Playback));

  // 
  playback_follows_cursor_playback_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Playback_Follows_Cursor));

  // 
  queue_playback_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Queue));

  // 
  queue_saved_playback_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Queue_Saved));

  // 
  flush_queue_playback_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Flush_Queue));

  // 
  selected_playlist_only_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Selected_Playlist_Only));





  //                //
  // View MenuItems ///////////////////////////////////////////////////////////
  //                //

  // 
  view_menu_ -> append(*hide_duplicates_check_menu_item_);

  // 
  view_menu_ -> append(*hide_header_bar_check_menu_item_);

  // 
  view_menu_ -> append(*hide_status_bar_check_menu_item_);

  // 
  view_menu_ -> append(*hide_tabs_check_menu_item_);



  // 
  view_menu_item_ -> set_tooltip_text
    ("Settings and tools for modifying aspects of the GUI.");



  // 
  hide_duplicates_check_menu_item_
    -> set_active(config() . get("gui.hide_duplicates"));

  // 
  hide_header_bar_check_menu_item_
    -> set_active(config() . get("gui.hide_header_bar"));

  // 
  hide_status_bar_check_menu_item_
    -> set_active(config() . get("gui.hide_status_bar"));

  // 
  hide_tabs_check_menu_item_
    -> set_active((config() . get("gui.tabs.hide")));



  // 
  hide_duplicates_check_menu_item_ -> set_tooltip_text
    ("Hides the duplicate menu bar and playlist combobox under the cover " \
     "art in the Main Content tab.");

  // 
  hide_header_bar_check_menu_item_ -> set_tooltip_text
    ("Hides the header bar. Requires OMP to be restarted in order to " \
     "take effect.");

  // 
  hide_status_bar_check_menu_item_ -> set_tooltip_text
    ("Hides the status bar at the bottom of the main window.");

  // 
  hide_tabs_check_menu_item_ -> set_tooltip_text
    ("Hide the tabs of OMP.");



  // 
  hide_duplicates_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun
        (*this, &MainMenu::Hide_Duplicates));

  // 
  hide_header_bar_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun
        (*this, &MainMenu::Hide_Header_Bar));

  // 
  hide_status_bar_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun
        (*this, &MainMenu::Hide_Status_Bar));

  // 
  hide_tabs_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MainMenu::Hide_Tabs));



  // 
  menu_ -> show_all_children();

  // 
  box() . show_all_children();

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//         //
// General ////////////////////////////////////////////////////////////////////
//         //

void MainMenu::Apply_Saved_Values()
{

  //          //
  // Playback /////////////////////////////////////////////////////////////////
  //          //

  // 
  string looping_type = config() . get("playback.looping");

  // 
  if(looping_type == "none")
  {

    // 
    looping_none_playback_radio_menu_item_ -> set_active(true);

  }

  // 
  else if(looping_type == "playlist")
  {

    // 
    looping_playlist_playback_radio_menu_item_ -> set_active(true);

  }



  // 
  cursor_follows_playback_playback_check_menu_item_
    -> set_active(config() . get("playback.cursor_follows_playback"));

  // 
  playback_follows_cursor_playback_check_menu_item_
    -> set_active(config() . get("playback.playback_follows_cursor"));

  // 
  queue_saved_playback_check_menu_item_
    -> set_active(config() . get("playback.queue_saved"));

  // 
  selected_playlist_only_check_menu_item_
    -> set_active(config() . get("playback.selected_playlist_only"));

  // 
  start_at_pregap_check_menu_item_
    -> set_active(config() . get("playback.start_at_pregap"));





  //      //
  // View /////////////////////////////////////////////////////////////////////
  //      //

  // 
  hide_duplicates_check_menu_item_
    -> set_active(config() . get("gui.hide_duplicates"));

  // 
  hide_header_bar_check_menu_item_
    -> set_active(config() . get("gui.hide_header_bar"));

  // 
  hide_status_bar_check_menu_item_
    -> set_active(config() . get("gui.hide_status_bar"));

  // 
  hide_tabs_check_menu_item_
    -> set_active((config() . get("gui.tabs.hide")));

}





//       //
// About //////////////////////////////////////////////////////////////////////
//       //

void MainMenu::Open_About()
{

  abouts() . Open_About();

}





//      //
// Edit ///////////////////////////////////////////////////////////////////////
//      //

void MainMenu::Configuration()
{

  config_guis() . Open_Configuration();

}





//      //
// File ///////////////////////////////////////////////////////////////////////
//      //

void MainMenu::Add_File()
{

  // 
  file_choosers() . Create();

}

void MainMenu::Quit()
{

  // 
  gui() . Quit(true);

}





//          //
// Playback ///////////////////////////////////////////////////////////////////
//          //

// Looping

void MainMenu::Change_Playback_Looping_Type()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  if(looping_none_playback_radio_menu_item_ -> get_active())
  {

    // 
    config() . set("playback.looping", "none");

  }

  // 
  else if(looping_playlist_playback_radio_menu_item_ -> get_active())
  {

    // 
    config() . set("playback.looping", "playlist");

  }



  // 
  config_guis() . Save_Changes();

  // 
  config_guis() . Mark_Unsaved_Changes(false);



  // 
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    if(looping_none_playback_radio_menu_item_ -> get_active())
    {

      // 
      config_guis_it -> playback_panel() . none_looping_radio_button()
        . set_active(true);

    }

    // 
    else if(looping_playlist_playback_radio_menu_item_ -> get_active())
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
    if(looping_none_playback_radio_menu_item_ -> get_active())
    {

      // 
      menu_bars_it -> looping_none_playback_radio_menu_item()
        . set_active(true);

    }

    // 
    else if(looping_playlist_playback_radio_menu_item_ -> get_active())
    {

      // 
      menu_bars_it -> looping_playlist_playback_radio_menu_item()
        . set_active(true);

    }

  } 

  // 
  main_menus() . set_disable_menubar_functions_flag(false);



  // 
  cout << "\n\nMenubar changer\n\n";



  // 
  playback() . Reset_Track_Queue();

}



// Options

void MainMenu::Cursor_Follows_Playback()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active
    = (cursor_follows_playback_playback_check_menu_item_ -> get_active());



  // 
  config() . set("playback.cursor_follows_playback", active);



  // 
  config_guis() . Save_Changes();



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . cursor_follows_playback_check_button()
      . set_active(active);

  }



  // 
  main_menus() . set_disable_menubar_functions_flag(true);



  // 
  config_guis() . set_disable_functions(false);

  // 
  config_guis() . Mark_Unsaved_Changes(false);

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

void MainMenu::Playback_Follows_Cursor()
{

  //
  playback() . Reset_Track_Queue();



  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active
    = (playback_follows_cursor_playback_check_menu_item_ -> get_active());



  // 
  config() . set("playback.playback_follows_cursor", active);



  // 
  config_guis() . Save_Changes();



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
  config_guis() . Mark_Unsaved_Changes(false);



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

void MainMenu::Selected_Playlist_Only()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active
    = (selected_playlist_only_check_menu_item_ -> get_active());



  // 
  config() . set("playback.selected_playlist_only", active);



  // 
  config_guis() . Save_Changes();



  // 
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  { 

    // 
    config_guis_it -> playback_panel() . selected_playlist_only_check_button()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);

  // 
  config_guis() . Mark_Unsaved_Changes(false);



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

void MainMenu::Start_At_Pregap_Start()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active
    = (start_at_pregap_check_menu_item_ -> get_active());



  // 
  config() . set("playback.start_at_pregap", active);



  // 
  config_guis() . Save_Changes();



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
  config_guis() . Mark_Unsaved_Changes(false);



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

}

void MainMenu::Stop_After_Current_Track()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active = (stop_after_current_track_playback_check_menu_item_ -> get_active());



  // 
  playback() . stop_after_current_track() = active;



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



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel()
      . stop_after_current_track_check_button() . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);

}



// Queue

void MainMenu::Queue()
{

  playback() . Reset_Track_Queue();



  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active
    = (queue_playback_check_menu_item_ -> get_active());



  // 
  config() . set("playback.queue", active);



  // 
  config_guis() . Save_Changes();



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . queue_check_button()
      . set_active(active);

  }



  // 
  main_menus() . set_disable_menubar_functions_flag(true);



  // 
  config_guis() . set_disable_functions(false);

  // 
  config_guis() . Mark_Unsaved_Changes(false);

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

void MainMenu::Queue_Saved()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active
    = (queue_saved_playback_check_menu_item_ -> get_active());



  // 
  config() . set("playback.queue_saved", active);



  // 
  config_guis() . Save_Changes();



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . queue_saved_check_button()
      . set_active(active);

  }



  // 
  main_menus() . set_disable_menubar_functions_flag(true);



  // 
  config_guis() . set_disable_functions(false);

  // 
  config_guis() . Mark_Unsaved_Changes(false);

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

void MainMenu::Flush_Queue()
{

  playlists() . Flush_Playback_Queue();

}



// State Changers

void MainMenu::Pause()
{

  playback() . Pause();

}

void MainMenu::Play()
{

  playback() . Play(Gtk::TreeRowReference());

}

void MainMenu::Stop()
{

  playback().Stop();

}





//      //
// View ///////////////////////////////////////////////////////////////////////
//      //

void MainMenu::Hide_Tabs()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  { 

    // 
    return;

  }



  // 
  bool active = hide_tabs_check_menu_item_ -> get_active();



  // 
  gui() . main_window_notebook() . set_show_tabs(!active);



  // 
  config() . set("gui.tabs.hide", active);

  // 
  config_guis() . Save_Changes();



  // 
  config_guis() . Mark_Unsaved_Changes(false);


 
  //
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> gui_panel() . hide_tabs_check_button()
      . set_active(active);

  }

  //
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : main_menus()())
  {

    // 
    menubars_it -> hide_tabs_check_menu_item() . set_active(active);

  } 

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}

void MainMenu::Hide_Duplicates()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    return;

  }



  // 
  bool active = (hide_duplicates_check_menu_item_ -> get_active());



  // 
  if(active)
  {

    // 
    gui() . duplicates_box() . hide();

  }

  else
  {

    // 
    gui() . duplicates_box() . show();

  }



  // 
  config() . set("gui.hide_duplicates", active);

  // 
  config_guis() . Save_Changes();



  // 
  config_guis() . Mark_Unsaved_Changes(false);



  //
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> gui_panel() . hide_duplicates_check_button()
      . set_active(active);

  }

  //
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : main_menus()())
  {

    // 
    menubars_it -> hide_duplicates_check_menu_item() . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}

void MainMenu::Hide_Header_Bar()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    return;

  }



  // 
  bool active = (hide_header_bar_check_menu_item_ -> get_active());



  // 
  config() . set("gui.hide_header_bar", active);

  // 
  config_guis() . Save_Changes();



  // 
  config_guis() . Mark_Unsaved_Changes(false);



  // 
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> gui_panel() . hide_header_bar_check_button()
      . set_active(active);

  } 

  //
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : main_menus()())
  {

    // 
    menubars_it -> hide_header_bar_check_menu_item() . set_active(active);

  }

  // 
  main_menus() . set_disable_menubar_functions_flag(false);

}

void MainMenu::Hide_Status_Bar()
{

  // 
  if(main_menus() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active = (hide_status_bar_check_menu_item_ -> get_active());



  //  
  if(active)
  {

    // 
    status_bar() . status_bar_frame() . hide();

  }

  else
  {

    // 
    status_bar() . status_bar_frame() . show();

  }



  // 
  config() . set("gui.hide_status_bar", active);



  // 
  config_guis() . Save_Changes();



  // 
  config_guis() . Mark_Unsaved_Changes(false);



  //
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> gui_panel() . hide_status_bar_check_button()
      . set_active(active);

  }

  //
  config_guis() . set_disable_functions(false);



  // 
  main_menus() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : main_menus()())
  {

    // 
    menubars_it -> hide_status_bar_check_menu_item() . set_active(active);

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

Gtk::Menu& MainMenu::menu()
{

  return *menu_;

}

//         //
// Looping ////////////////////////////////////////////////////////////////////
//         //

Gtk::RadioMenuItem& MainMenu::looping_none_playback_radio_menu_item()
{

  return *looping_none_playback_radio_menu_item_;

}

Gtk::RadioMenuItem& MainMenu::looping_playlist_playback_radio_menu_item()
{

  return *looping_playlist_playback_radio_menu_item_;

}

Gtk::RadioMenuItem& MainMenu::looping_track_playback_radio_menu_item()
{

  return *looping_track_playback_radio_menu_item_;

}





//          //
// Playback ///////////////////////////////////////////////////////////////////
//          //

Gtk::CheckMenuItem& MainMenu::cursor_follows_playback_playback_check_menu_item()
{

  return *cursor_follows_playback_playback_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::playback_follows_cursor_playback_check_menu_item() 
{

  return *playback_follows_cursor_playback_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::queue_playback_check_menu_item() 
{

  return *queue_playback_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::queue_saved_playback_check_menu_item() 
{

  return *queue_saved_playback_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::selected_playlist_only_check_menu_item()
{

  return *selected_playlist_only_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::start_at_pregap_check_menu_item()
{

  return *start_at_pregap_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::stop_after_current_track_playback_check_menu_item()
{

  return *stop_after_current_track_playback_check_menu_item_;

}





//      //
// View ///////////////////////////////////////////////////////////////////////
//      //

Gtk::CheckMenuItem& MainMenu::hide_duplicates_check_menu_item()
{

  return *hide_duplicates_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::hide_header_bar_check_menu_item()
{

  return *hide_header_bar_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::hide_status_bar_check_menu_item()
{

  return *hide_status_bar_check_menu_item_;

}

Gtk::CheckMenuItem& MainMenu::hide_tabs_check_menu_item()
{

  return *hide_tabs_check_menu_item_;

}
