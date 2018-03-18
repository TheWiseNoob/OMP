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

#ifndef MENUBAR_H
#define MENUBAR_H





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

#include <string>
#include <atomic>





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





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class MenuBar : public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    MenuBar(Base& base_ref, Gtk::Window& window_ref);





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

    void On_None_Looping_Playback_Radio_Menu_Item_Toggled_Signal();

    void On_Playlist_Looping_Playback_Radio_Menu_Item_Toggled_Signal();
    


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

    void On_Hide_Duplicates_Check_Menu_Item_Activate_Signal();

    void On_Hide_Status_Bar_Check_Menu_Item_Activate_Signal();

    void On_Hide_Header_Bar_Check_Menu_Item_Activate_Signal();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  public:

    Gtk::Box& box();





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

    Gtk::MenuBar* menu_bar_;

    std::list<MenuBar*>::iterator menubars_it;

    Gtk::Box* MenuBar_HBox;

    Gtk::Box* MenuBar_VBox;





  //       //
  // About ////////////////////////////////////////////////////////////////////
  //       //

  private:

    // 
    Gtk::MenuItem *About_MenuItem;

    // 
    Gtk::Menu* About_Menu;

    //About MenuItems
    Gtk::MenuItem *About_Base_MenuItem,
                  *Help_MenuItem;





  //      //
  // Edit /////////////////////////////////////////////////////////////////////
  //      //

  public:

    Gtk::MenuItem *Edit_MenuItem;
    Gtk::Menu* Edit_Menu;

    //Edit MenuItems
    Gtk::MenuItem *Cut_MenuItem,
                  *Copy_MenuItem,
                  *Paste_MenuItem,
                  *Remove_MenuItem,
                  *Configuration_MenuItem;






  //      //
  // File /////////////////////////////////////////////////////////////////////
  //      //

  private:

    Gtk::MenuItem *File_MenuItem;

    Gtk::Menu* File_Menu;

    Gtk::MenuItem* Add_File_MenuItem;

    Gtk::MenuItem* Open_File_MenuItem;

    Gtk::MenuItem* Add_Folder_MenuItem;

    Gtk::MenuItem* Open_Folder_MenuItem;

    Gtk::MenuItem* Quit_MenuItem;





  //         //
  // Library //////////////////////////////////////////////////////////////////
  //         //

  private:

    Gtk::MenuItem *Library_MenuItem;

    Gtk::Menu* Library_Menu;





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

    Gtk::MenuItem* pause_playback_menu_item_;

    Gtk::MenuItem* play_playback_menu_item_;

    Gtk::MenuItem* stop_playback_menu_item_;
    




  //      //
  // View /////////////////////////////////////////////////////////////////////
  //      //

  private:

    Gtk::MenuItem *View_MenuItem;

    Gtk::Menu* View_Menu;

    Gtk::CheckMenuItem* hide_duplicates_check_menu_item_;

    Gtk::CheckMenuItem* hide_header_bar_check_menu_item_;

    Gtk::CheckMenuItem* hide_status_bar_check_menu_item_;

    Gtk::CheckMenuItem* hide_tabs_check_menu_item_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
