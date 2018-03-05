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

#include "MenuBar.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"

#include "../Configuration/Configuration.h"

#include "../Playback/Playback.h"

#include "Elements/ChildWindows/ChildWindow.h"

#include "Elements/Abouts/Abouts.h"

#include "Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.h"

#include "Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.h"

#include "Elements/FileChoosers/FileChoosers.h"

#include "Elements/PlaylistComboBoxes/PlaylistComboBox.h"

#include "Elements/PlaylistComboBoxes/PlaylistComboBoxes.h"

#include "Elements/Playlists/Playlists.h"

#include "GUI.h"





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

MenuBar::MenuBar(Base& base_ref, Gtk::Window& window_ref)

// Inherited Class

: Parts(base_ref)



// General

, menu_bar_(Gtk::manage(new Gtk::MenuBar))

, MenuBar_HBox(Gtk::manage(new Gtk::Box))

, MenuBar_VBox(Gtk::manage(new Gtk::Box))


// Menus

, File_Menu(Gtk::manage(new Gtk::Menu))

, Edit_Menu(Gtk::manage(new Gtk::Menu))

, View_Menu(Gtk::manage(new Gtk::Menu))

, Library_Menu(Gtk::manage(new Gtk::Menu))

, About_Menu(Gtk::manage(new Gtk::Menu))



// Playback

, playback_menu_(Gtk::manage(new Gtk::Menu))

, playback_menu_item_(Gtk::manage(new Gtk::MenuItem("_Playback", true)))

, play_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("Play", true)))

, pause_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("Pause", true)))

, stop_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("Stop", true)))

, order_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("_Order", true)))

, order_playback_menu_(Gtk::manage(new Gtk::Menu))

, looping_playback_menu_item_(Gtk::manage(new Gtk::MenuItem("_Looping", true)))

, looping_playback_menu_(Gtk::manage(new Gtk::Menu))

, order_normal_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Normal")))

, order_random_tracks_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Random Tracks")))

, order_shuffle_tracks_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Shuffle Tracks")))

, looping_none_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("None")))

, looping_playlist_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Repeat Playlist")))

, looping_track_playback_radio_menu_item_
    (Gtk::manage(new Gtk::RadioMenuItem("Repeat Track")))

, stop_after_current_track_playback_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("_Stop After Current Track", true)))

, cursor_follows_playback_playback_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("_Cursor Follows Playback", true)))

, playback_follows_cursor_playback_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("_Playback Follows Cursor", true)))

, queue_playback_check_menu_item_(Gtk::manage(new Gtk::CheckMenuItem
    ("_Queue", true)))

, queue_saved_playback_check_menu_item_(Gtk::manage(new Gtk::CheckMenuItem
    ("Saved Playback _Queue", true)))

, flush_queue_playback_menu_item_(Gtk::manage(new Gtk::MenuItem
    ("_Flush Playback Queue", true)))



// View MenuItems

, hide_duplicates_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Hide _Duplicates", true)))

, hide_header_bar_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Hide _Header Bar", true)))

, hide_status_bar_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Hide _Status Bar", true)))

, hide_tabs_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("Hide _Tabs", true)))

{

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  // 
  MenuBar_HBox -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  // 
  MenuBar_VBox -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // 
  MenuBar_HBox -> set_center_widget(*MenuBar_VBox);

  // 
  MenuBar_VBox -> set_center_widget(*menu_bar_);




  //                //
  // Main MenuItems ///////////////////////////////////////////////////////////
  //                // 

  // 
  File_MenuItem = Gtk::manage(new Gtk::MenuItem("_File", true));

  // 
  Edit_MenuItem = Gtk::manage(new Gtk::MenuItem("_Edit", true));

  // 
  View_MenuItem = Gtk::manage(new Gtk::MenuItem("_View", true));

  // 
  Library_MenuItem = Gtk::manage(new Gtk::MenuItem("_Library", true));

  // 
  About_MenuItem = Gtk::manage(new Gtk::MenuItem("_About", true));



  // 
  menu_bar_ -> append(*File_MenuItem);

  // 
  menu_bar_ -> append(*Edit_MenuItem);

  // 
  menu_bar_ -> append(*View_MenuItem);

  // 
  menu_bar_ -> append(*playback_menu_item_);

  // 
  menu_bar_ -> append(*Library_MenuItem);

  // 
  menu_bar_ -> append(*About_MenuItem);



  // 
  File_MenuItem -> set_submenu(*File_Menu);

  // 
  Edit_MenuItem -> set_submenu(*Edit_Menu);

  // 
  View_MenuItem -> set_submenu(*View_Menu);

  // 
  playback_menu_item_ -> set_submenu(*playback_menu_);

  // 
  Library_MenuItem -> set_submenu(*Library_Menu);

  // 
  About_MenuItem -> set_submenu(*About_Menu);





  //                //
  // File MenuItems ///////////////////////////////////////////////////////////
  //                //

  // 
  Add_File_MenuItem = Gtk::manage(new Gtk::MenuItem("_Add File(s)", true));

  // 
  Open_File_MenuItem = Gtk::manage(new Gtk::MenuItem("_Open File(s)", true));

  // 
  Add_Folder_MenuItem = Gtk::manage(new Gtk::MenuItem("_Add Folder(s)", true));

  // 
  Open_Folder_MenuItem = Gtk::manage(new Gtk::MenuItem("_Open Folder(s)",
                                                       true));

  // 
  Quit_MenuItem = Gtk::manage(new Gtk::MenuItem("_Quit OMP", true));



  // 
  File_Menu -> append(*Add_File_MenuItem);

  // 
  File_Menu -> append(*Open_File_MenuItem);

  // 
  Open_File_MenuItem -> set_sensitive(false);

  // 
  File_Menu -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  File_Menu -> append(*Add_Folder_MenuItem);

  // 
  Add_Folder_MenuItem -> set_sensitive(false);

  // 
  File_Menu -> append(*Open_Folder_MenuItem);

  // 
  Open_Folder_MenuItem -> set_sensitive(false);

  // 
  File_Menu -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  File_Menu -> append(*Quit_MenuItem);



  // 
  Quit_MenuItem -> signal_activate()
    . connect(sigc::mem_fun(*this, &MenuBar::Quit));

  // 
  Add_File_MenuItem -> signal_activate()
     . connect(sigc::mem_fun(*this, &MenuBar::Add_File));





  //                //
  // Edit MenuItems ///////////////////////////////////////////////////////////
  //                //

  // 
  Cut_MenuItem = Gtk::manage(new Gtk::MenuItem("_Cut", true));

  // 
  Copy_MenuItem = Gtk::manage(new Gtk::MenuItem("_Copy", true));

  // 
  Paste_MenuItem = Gtk::manage(new Gtk::MenuItem("_Paste", true));

  // 
  Remove_MenuItem = Gtk::manage(new Gtk::MenuItem("_Remove", true));

  // 
  Configuration_MenuItem
    = Gtk::manage(new Gtk::MenuItem("_Configuration", true));



  // 
  Edit_Menu -> append(*Cut_MenuItem);

  // 
  Cut_MenuItem -> set_sensitive(false);

  // 
  Edit_Menu -> append(*Copy_MenuItem);

  // 
  Copy_MenuItem -> set_sensitive(false);

  // 
  Edit_Menu -> append(*Paste_MenuItem);

  // 
  Paste_MenuItem -> set_sensitive(false);

  // 
  Edit_Menu -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  Edit_Menu -> append(*Remove_MenuItem);

  // 
  Remove_MenuItem -> set_sensitive(false);

  // 
  Edit_Menu -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  Edit_Menu -> append(*Configuration_MenuItem);



  // 
  Configuration_MenuItem -> signal_activate()
    . connect(sigc::mem_fun(*this, &MenuBar::Configuration));





  //                //
  // View MenuItems ///////////////////////////////////////////////////////////
  //                //

  // 
  hide_duplicates_check_menu_item_
    -> set_active(config() . get("gui.hide_duplicates"));


  // 
  View_Menu -> append(*hide_duplicates_check_menu_item_);

  // 
  hide_duplicates_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun
        (*this, &MenuBar::On_Hide_Duplicates_Check_Menu_Item_Activate_Signal));



  // 
  hide_header_bar_check_menu_item_
    -> set_active(config() . get("gui.hide_header_bar"));

  // 
  View_Menu -> append(*hide_header_bar_check_menu_item_);

  // 
  hide_header_bar_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun
        (*this, &MenuBar::On_Hide_Header_Bar_Check_Menu_Item_Activate_Signal));



  // 
  hide_status_bar_check_menu_item_
    -> set_active(config() . get("gui.hide_status_bar"));

  // 
  View_Menu -> append(*hide_status_bar_check_menu_item_);


  // 
  hide_status_bar_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun
        (*this, &MenuBar::On_Hide_Status_Bar_Check_Menu_Item_Activate_Signal));



  // 
  hide_tabs_check_menu_item_
    -> set_active((config() . get("gui.tabs.hide")));

  // 
  View_Menu -> append(*hide_tabs_check_menu_item_);


  // 
  hide_tabs_check_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MenuBar::Hide_Tabs));





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
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*looping_playback_menu_item_);

  // 
  playback_menu_ -> append(*order_playback_menu_item_);



  // 
  stop_after_current_track_playback_check_menu_item_ -> set_active(false);

  // 
  cursor_follows_playback_playback_check_menu_item_
    -> set_active(base().config().get("playback.cursor_follows_playback"));

  // 
  playback_follows_cursor_playback_check_menu_item_
    = Gtk::manage(new Gtk::CheckMenuItem("_Playback Follows Cursor", true));

  // 
  playback_follows_cursor_playback_check_menu_item_
    -> set_active(base().config().get("playback.playback_follows_cursor"));



  // 
  queue_saved_playback_check_menu_item_
    -> set_active(config() . get("playback.queue_saved"));



  // 
  order_playback_menu_item_ -> set_submenu(*order_playback_menu_);

  // 
  looping_playback_menu_item_ -> set_submenu(*looping_playback_menu_);

  // 
  looping_playback_menu_ -> append(*looping_none_playback_radio_menu_item_);

  // 
  looping_playback_menu_ -> append(*looping_track_playback_radio_menu_item_);

  // 
  looping_playback_menu_ -> append(*looping_playlist_playback_radio_menu_item_);



  // 
  order_playback_menu_ -> append(*order_normal_playback_radio_menu_item_);

  // 
  order_playback_menu_ -> append(*order_random_tracks_playback_radio_menu_item_);

  // 
  order_playback_menu_ -> append(*order_shuffle_tracks_playback_radio_menu_item_);



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
         &MenuBar::On_None_Looping_Playback_Radio_Menu_Item_Toggled_Signal));

  // 
  looping_playlist_playback_radio_menu_item_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &MenuBar
            ::On_Playlist_Looping_Playback_Radio_Menu_Item_Toggled_Signal));



  // 
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*stop_after_current_track_playback_check_menu_item_);

  // 
  playback_menu_ -> append(*cursor_follows_playback_playback_check_menu_item_);

  // 
  playback_menu_ -> append(*playback_follows_cursor_playback_check_menu_item_);

  // 
  playback_menu_ -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  playback_menu_ -> append(*queue_saved_playback_check_menu_item_);

  // 
  playback_menu_ -> append(*flush_queue_playback_menu_item_);



  // 
  play_playback_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MenuBar::Play));

  // 
  pause_playback_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MenuBar::Pause));

  // 
  stop_playback_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(*this, &MenuBar::Stop));



  // 
  stop_after_current_track_playback_check_menu_item_ -> signal_activate().connect(
    sigc::mem_fun(*this, &MenuBar::Stop_After_Current_Track));

  // 
  cursor_follows_playback_playback_check_menu_item_ -> signal_activate().connect(
    sigc::mem_fun(*this, &MenuBar::Cursor_Follows_Playback));

  // 
  playback_follows_cursor_playback_check_menu_item_ -> signal_activate().connect(
    sigc::mem_fun(*this, &MenuBar::Playback_Follows_Cursor));

  // 
  queue_playback_check_menu_item_ -> signal_activate().connect(
    sigc::mem_fun(*this, &MenuBar::Queue));

  // 
  queue_saved_playback_check_menu_item_ -> signal_activate().connect(
    sigc::mem_fun(*this, &MenuBar::Queue_Saved));

  // 
  flush_queue_playback_menu_item_ -> signal_activate().connect(
    sigc::mem_fun(*this, &MenuBar::Flush_Queue));

  // 
  flush_queue_playback_menu_item_ -> set_sensitive(false);





  //                 //
  // About MenuItems //////////////////////////////////////////////////////////
  //                 //

  // 
  Help_MenuItem = Gtk::manage(new Gtk::MenuItem("_Help", true));

  // 
  Help_MenuItem -> set_sensitive(false);

  // 
  About_Menu -> append(*Help_MenuItem);



  // 
  About_Base_MenuItem
    = Gtk::manage(new Gtk::MenuItem("_About OMP", true));

  // 
  About_Menu -> append(*About_Base_MenuItem);

  // 
  About_Menu -> append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  About_Base_MenuItem -> signal_activate()
    . connect(sigc::mem_fun(*this, &MenuBar::Open_About));





  //                     //
  // Ending Construction //////////////////////////////////////////////////////
  //                     //

/* 
  Gtk::AccelMap::add_entry("<Window>/Edit/Configuration",
                           GDK_KEY_j, Gdk::CONTROL_MASK);

  Edit_Menu -> set_accel_group(window_ref . get_accel_group());

  Edit_Menu -> set_accel_path("<Window>/Edit");

  menu_bar_ -> accelerate(window_ref);
*/



  // 
  box() . show_all_children();

}





//       //
// About //////////////////////////////////////////////////////////////////////
//       //

void MenuBar::Open_About()
{

  abouts() . Open_About();

}





//      //
// Edit ///////////////////////////////////////////////////////////////////////
//      //

void MenuBar::Configuration()
{

  config_guis() . Open_Configuration();

}





//      //
// File ///////////////////////////////////////////////////////////////////////
//      //

void MenuBar::Add_File()
{

  file_choosers() . Add_Files();

}

void MenuBar::Quit()
{

  gui().Quit();

}





//          //
// Playback ///////////////////////////////////////////////////////////////////
//          //

// Looping

void MenuBar::On_None_Looping_Playback_Radio_Menu_Item_Toggled_Signal()
{

  // 
  if(gui() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  config() . set("playback.looping", "none");



  // 
  config_guis() . Save_Changes();

  // 
  config_guis() . Mark_Unsaved_Changes(false);



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . none_looping_radio_button()
      . set_active(true);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : gui() . menubars())
  { 

    // 
    menu_bars_it -> looping_none_playback_radio_menu_item()
      . set_active(true);

  } 

  // 
  gui() . set_disable_menubar_functions_flag(false);



  // 
  playback() . Reset_Track_Queue();

}

void MenuBar::On_Playlist_Looping_Playback_Radio_Menu_Item_Toggled_Signal()
{

  // 
  if(gui() . disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  config() . set("playback.looping", "playlist");



  // 
  config_guis() . Save_Changes();

  // 
  config_guis() . Mark_Unsaved_Changes(false);



  // 
  config_guis() . set_disable_functions(true);

  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> playback_panel() . playlist_looping_radio_button()
      . set_active(true);

  }

  // 
  config_guis() . set_disable_functions(false);



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : gui() . menubars())
  { 

    // 
    menu_bars_it -> looping_playlist_playback_radio_menu_item()
      . set_active(true);

  } 

  // 
  gui() . set_disable_menubar_functions_flag(false);



  // 
  playback() . Reset_Track_Queue();

}



// Options

void MenuBar::Cursor_Follows_Playback()
{

  // 
  if(gui().disable_menubar_functions_flag())
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
  gui() . set_disable_menubar_functions_flag(true);



  // 
  config_guis() . set_disable_functions(false);

  // 
  config_guis() . Mark_Unsaved_Changes(false);

  // 
  for(auto menu_bars_it : gui() . menubars())
  { 

    // 
    menu_bars_it -> cursor_follows_playback_playback_check_menu_item()
      . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);

}

void MenuBar::Playback_Follows_Cursor()
{

  //
  playback() . Reset_Track_Queue();



  // 
  if(gui().disable_menubar_functions_flag())
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
  gui() . set_disable_menubar_functions_flag(true);



  // 
  config_guis() . set_disable_functions(false);

  // 
  config_guis() . Mark_Unsaved_Changes(false);

  // 
  for(auto menu_bars_it : gui() . menubars())
  { 

    // 
    menu_bars_it -> playback_follows_cursor_playback_check_menu_item()
      . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);


}

void MenuBar::Stop_After_Current_Track()
{

  // 
  if(gui().disable_menubar_functions_flag())
  {

    // 
    return;

  }



  // 
  bool active = (stop_after_current_track_playback_check_menu_item_ -> get_active());



  // 
  playback() . stop_after_current_track() = active;



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menu_bars_it : gui() . menubars())
  { 

    // 
    menu_bars_it -> stop_after_current_track_playback_check_menu_item()
      . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);



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

}



// Queue

void MenuBar::Queue()
{

  playback() . Reset_Track_Queue();



  // 
  if(gui().disable_menubar_functions_flag())
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
  gui() . set_disable_menubar_functions_flag(true);



  // 
  config_guis() . set_disable_functions(false);

  // 
  config_guis() . Mark_Unsaved_Changes(false);

  // 
  for(auto menu_bars_it : gui() . menubars())
  { 

    // 
    menu_bars_it -> queue_playback_check_menu_item()
      . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);


}

void MenuBar::Queue_Saved()
{

  // 
  if(gui().disable_menubar_functions_flag())
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
  gui() . set_disable_menubar_functions_flag(true);



  // 
  config_guis() . set_disable_functions(false);

  // 
  config_guis() . Mark_Unsaved_Changes(false);

  // 
  for(auto menu_bars_it : gui() . menubars())
  { 

    // 
    menu_bars_it -> queue_saved_playback_check_menu_item()
      . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);


}

void MenuBar::Flush_Queue()
{

  playlists() . Flush_Playback_Queue();

}



// State Changers

void MenuBar::Pause()
{

  playback() . Pause();

}

void MenuBar::Play()
{

  playback() . Play(Gtk::TreeRowReference());

}

void MenuBar::Stop()
{

  playback().Stop();

}





//      //
// View ///////////////////////////////////////////////////////////////////////
//      //

void MenuBar::Hide_Tabs()
{

  // 
  if(gui() . disable_menubar_functions_flag())
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
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_tabs_check_menu_item() . set_active(active);

  } 

  // 
  gui() . set_disable_menubar_functions_flag(false);

}

void MenuBar::On_Hide_Duplicates_Check_Menu_Item_Activate_Signal()
{

  // 
  if(gui().disable_menubar_functions_flag())
  {

    return;

  }



  // 
  bool active = (hide_duplicates_check_menu_item_ -> get_active());



  // 
  if(active)
  {

    // 
    gui() . menubars() . back() -> box() . hide();

    // 
    playlist_comboboxes()() . back() -> box() . hide();

  }

  else
  {

    // 
    gui() . menubars() . back() -> box() . show();

    // 
    playlist_comboboxes()() . back() -> box() . show();

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
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_duplicates_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);

}

void MenuBar::On_Hide_Header_Bar_Check_Menu_Item_Activate_Signal()
{

  // 
  if(gui() . disable_menubar_functions_flag())
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
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_header_bar_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);

}

void MenuBar::On_Hide_Status_Bar_Check_Menu_Item_Activate_Signal()
{

  // 
  if(gui().disable_menubar_functions_flag())
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
    gui() . status_bar_frame() . hide();

  }

  else
  {

    // 
    gui() . status_bar_frame() . show();

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
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_status_bar_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

//         //
// General //////////////////////////////////////////////////////////////////
//         //

Gtk::Box& MenuBar::box()
{

  return *MenuBar_HBox;

}





//         //
// Looping ////////////////////////////////////////////////////////////////////
//         //

Gtk::RadioMenuItem& MenuBar::looping_none_playback_radio_menu_item()
{

  return *looping_none_playback_radio_menu_item_;

}

Gtk::RadioMenuItem& MenuBar::looping_playlist_playback_radio_menu_item()
{

  return *looping_playlist_playback_radio_menu_item_;

}

Gtk::RadioMenuItem& MenuBar::looping_track_playback_radio_menu_item()
{

  return *looping_track_playback_radio_menu_item_;

}





//          //
// Playback ///////////////////////////////////////////////////////////////////
//          //

Gtk::CheckMenuItem& MenuBar::stop_after_current_track_playback_check_menu_item()
{

  return *stop_after_current_track_playback_check_menu_item_;

}

Gtk::CheckMenuItem& MenuBar::cursor_follows_playback_playback_check_menu_item()
{

  return *cursor_follows_playback_playback_check_menu_item_;

}

Gtk::CheckMenuItem& MenuBar::playback_follows_cursor_playback_check_menu_item() 
{

  return *playback_follows_cursor_playback_check_menu_item_;

}

Gtk::CheckMenuItem& MenuBar::queue_playback_check_menu_item() 
{

  return *queue_playback_check_menu_item_;

}

Gtk::CheckMenuItem& MenuBar::queue_saved_playback_check_menu_item() 
{

  return *queue_saved_playback_check_menu_item_;

}





//      //
// View ///////////////////////////////////////////////////////////////////////
//      //

Gtk::CheckMenuItem& MenuBar::hide_duplicates_check_menu_item()
{

  return *hide_duplicates_check_menu_item_;

}

Gtk::CheckMenuItem& MenuBar::hide_header_bar_check_menu_item()
{

  return *hide_header_bar_check_menu_item_;

}

Gtk::CheckMenuItem& MenuBar::hide_status_bar_check_menu_item()
{

  return *hide_status_bar_check_menu_item_;

}

Gtk::CheckMenuItem& MenuBar::hide_tabs_check_menu_item()
{

  return *hide_tabs_check_menu_item_;

}
