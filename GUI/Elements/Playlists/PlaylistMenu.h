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

#ifndef PLAYLISTS_MENU_H
#define PLAYLISTS_MENU_H





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

#include "../../../Parts.h"

#include <gtkmm/menu.h>





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/radiobuttongroup.h>

#include <list>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

namespace Gtk
{

  class CheckMenuItem;

  class MenuItem;

  class RadioButtonGroup;

  class RadioMenuItem;

}

class Playlist;

class Playlists;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class PlaylistMenu : public Gtk::Menu, public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    PlaylistMenu(Base& base_ref, Playlist& playlist_ref,
                 Playlists& playlists_ref, const char* playlist_name);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~PlaylistMenu();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::MenuItem& change_playlist_menu_item();

    Gtk::MenuItem& copy_menu_item();

    Gtk::MenuItem& cut_menu_item();

    Gtk::MenuItem& delete_menu_item();

    Gtk::MenuItem& edit_menu_item();

    Gtk::CheckMenuItem& lock_check_menu_item();

    Gtk::MenuItem& queue_menu_item();

    Gtk::MenuItem& paste_menu_item();

    Gtk::Menu& playlists_menu();

    Gtk::RadioButtonGroup& playlists_menu_radio_button_group();

    std::list<Gtk::RadioMenuItem*>& playlists_menu_radio_menu_items();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  private:

    // 
    Gtk::MenuItem* add_playlist_menu_item_;

    // 
    Gtk::MenuItem* change_playlist_menu_item_;

    // 
    Gtk::MenuItem* copy_menu_item_;

    // 
    Gtk::MenuItem* cut_menu_item_;

    // 
    Gtk::MenuItem* delete_menu_item_;

    // 
    Gtk::MenuItem* edit_menu_item_;

    //                   
    Gtk::CheckMenuItem* lock_check_menu_item_;

    // 
    Gtk::MenuItem* queue_menu_item_;

    // 
    Gtk::MenuItem* paste_menu_item_;

    // 
    Playlist& playlist_;

    // 
    Gtk::Menu* playlists_menu_;

    // 
    Gtk::RadioButtonGroup playlists_menu_radio_button_group_;

    // 
    std::list<Gtk::RadioMenuItem*> playlists_menu_radio_menu_items_;

    // 
    Gtk::MenuItem* remove_playlist_menu_item_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
