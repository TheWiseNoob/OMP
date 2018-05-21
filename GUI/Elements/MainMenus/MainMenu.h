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

#ifndef MAIN_MENU_H
#define MAIN_MENU_H





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

#include "../../GUIElement.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <string>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

// gtkmm

namespace Gtk
{

  class Box;

  class CheckMenuItem;

  class Menu;

  class MenuBar;

  class MenuItem;

  class RadioMenuItem;

  class SeparatorMenuItem;

  class Window;

}



// Parts

class Base;

class GUI;

class MainMenus;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class MainMenu : public GUIElement<MainMenu>
{ 

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    MainMenu(Base& base_ref, MainMenus& main_menus_ref, 
             bool normal_menu = false);





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //       //
  // About ////////////////////////////////////////////////////////////////////
  //       //

  public:

    void Open_About();






  //      //
  // Edit /////////////////////////////////////////////////////////////////////
  //      //

  public:

    void Configuration();





  //      //
  // File /////////////////////////////////////////////////////////////////////
  //      //

  public:

    void Add_File();

    void Quit();





  //          //
  // Playback /////////////////////////////////////////////////////////////////
  //          //

  public:

    // Looping

    void Change_Playback_Looping_Type();



    // Options

    void Stop_After_Current_Track();

    void Cursor_Follows_Playback();

    void Playback_Follows_Cursor();



    // Queue

    void Queue();

    void Queue_Saved();

    void Flush_Queue();



    // State Changers

    void Pause();

    void Play();

    void Stop();





  //      //
  // View /////////////////////////////////////////////////////////////////////
  //      //

  public:

    void Hide_Tabs();

    void Hide_Duplicates();

    void Hide_Status_Bar();

    void Hide_Header_Bar();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  public:

    Gtk::Menu& menu();





  //         //
  // Looping //////////////////////////////////////////////////////////////////
  //         //

  public:

    Gtk::RadioMenuItem& looping_none_playback_radio_menu_item();

    Gtk::RadioMenuItem& looping_playlist_playback_radio_menu_item();

    Gtk::RadioMenuItem& looping_track_playback_radio_menu_item();





  //          //
  // Playback /////////////////////////////////////////////////////////////////
  //          //

  public:

    // 
    Gtk::CheckMenuItem& playback_follows_cursor_playback_check_menu_item();

    // 
    Gtk::CheckMenuItem& cursor_follows_playback_playback_check_menu_item();

    // 
    Gtk::CheckMenuItem& queue_playback_check_menu_item();

    // 
    Gtk::CheckMenuItem& queue_saved_playback_check_menu_item();

    // 
    Gtk::CheckMenuItem& stop_after_current_track_playback_check_menu_item();





  //      //
  // View /////////////////////////////////////////////////////////////////////
  //      //

  public:

    //
    Gtk::CheckMenuItem& hide_duplicates_check_menu_item();

    // 
    Gtk::CheckMenuItem& hide_header_bar_check_menu_item();

    //
    Gtk::CheckMenuItem& hide_status_bar_check_menu_item();

    //
    Gtk::CheckMenuItem& hide_tabs_check_menu_item();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  private:

    Gtk::Menu* menu_;

    Gtk::MenuBar* menu_bar_;





  //       //
  // About ////////////////////////////////////////////////////////////////////
  //       //

  private:

    // 
    Gtk::Menu* about_menu_;

    // 
    Gtk::MenuItem* about_menu_item_;

    // 
    Gtk::MenuItem *about_omp_menu_item_;





  //      //
  // Edit /////////////////////////////////////////////////////////////////////
  //      //

  public:

    Gtk::MenuItem* configuration_menu_item_;

    Gtk::Menu* edit_menu_;

    Gtk::MenuItem* edit_menu_item_;





  //      //
  // File /////////////////////////////////////////////////////////////////////
  //      //

  private:

    Gtk::MenuItem* add_file_menu_item_;

    Gtk::Menu* file_menu_;

    Gtk::MenuItem* file_menu_item_;

    Gtk::MenuItem* quit_menu_item_;





  //         //
  // Library //////////////////////////////////////////////////////////////////
  //         //

  private:

    Gtk::Menu* library_menu_;

    Gtk::MenuItem* library_menu_item_;





  //          //
  // Playback /////////////////////////////////////////////////////////////////
  //          //

  private:

    // General

    Gtk::Menu* playback_menu_;

    Gtk::MenuItem* playback_menu_item_;



    // Options

    Gtk::CheckMenuItem* cursor_follows_playback_playback_check_menu_item_;

    Gtk::MenuItem* flush_queue_playback_menu_item_;

    Gtk::CheckMenuItem* playback_follows_cursor_playback_check_menu_item_;

    Gtk::CheckMenuItem* stop_after_current_track_playback_check_menu_item_;

    Gtk::CheckMenuItem* queue_playback_check_menu_item_;

    Gtk::CheckMenuItem* queue_saved_playback_check_menu_item_;



    // Order

    Gtk::MenuItem* order_playback_menu_item_;

    Gtk::Menu* order_playback_menu_;

    Gtk::RadioMenuItem* order_normal_playback_radio_menu_item_;

    Gtk::RadioMenuItem* order_random_tracks_playback_radio_menu_item_;

    Gtk::RadioMenuItem* order_shuffle_tracks_playback_radio_menu_item_;



    // Looping

    Gtk::MenuItem* looping_playback_menu_item_;

    Gtk::Menu* looping_playback_menu_;

    Gtk::RadioMenuItem* looping_none_playback_radio_menu_item_;

    Gtk::RadioMenuItem* looping_playlist_playback_radio_menu_item_;

    Gtk::RadioMenuItem* looping_track_playback_radio_menu_item_;



    // State Changers

    Gtk::MenuItem* next_playback_menu_item_;

    Gtk::MenuItem* pause_playback_menu_item_;

    Gtk::MenuItem* play_playback_menu_item_;

    Gtk::MenuItem* stop_playback_menu_item_;
    




  //      //
  // View /////////////////////////////////////////////////////////////////////
  //      //

  private:

    Gtk::CheckMenuItem* hide_duplicates_check_menu_item_;

    Gtk::CheckMenuItem* hide_header_bar_check_menu_item_;

    Gtk::CheckMenuItem* hide_status_bar_check_menu_item_;

    Gtk::CheckMenuItem* hide_tabs_check_menu_item_;

    Gtk::Menu* view_menu_;

    Gtk::MenuItem* view_menu_item_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
