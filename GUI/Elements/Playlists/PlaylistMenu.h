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
//    - boost: http://www.boost.org/
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





#ifndef PLAYLISTS_MENU_H
#define PLAYLISTS_MENU_H





#include <gtkmm/menu.h>
#include "../../../Parts.h"



#include <gtkmm/radiobuttongroup.h>

#include <list>



class Playlist;
class Playlists;

namespace Gtk
{

  class MenuItem;
  class CheckMenuItem;
  class RadioButtonGroup;
  class RadioMenuItem;

}





class PlaylistMenu : public Gtk::Menu, public Parts
{

  public:

    PlaylistMenu(Base& base, Playlist& new_playlist, 
                 Playlists& temp_playlists);

    ~PlaylistMenu();


    void Queue();


    Gtk::RadioButtonGroup& playlists_menu_radio_button_group();

    Gtk::Menu& playlists_menu()
    {

      return *playlists_menu_;

    }

    Gtk::MenuItem& change_playlist_menu_item()
    {

      return *change_playlist_menu_item_;

    }

    Gtk::MenuItem& edit_menu_item()
    {

      return *edit_menu_item_;

    }

    Gtk::MenuItem& delete_menu_item()
    {

      return *delete_menu_item_;

    }

    Gtk::MenuItem& queue_menu_item()
    {

      return *queue_menu_item_;

    }

    Gtk::MenuItem& cut_menu_item()
    {

      return *cut_menu_item_;

    }

    Gtk::MenuItem& copy_menu_item()
    {

      return *copy_menu_item_;

    }

    Gtk::MenuItem& paste_menu_item()
    {

      return *paste_menu_item_;

    }

    Gtk::CheckMenuItem& lock_check_menu_item()
    {

      return *lock_check_menu_item_;

    }

    std::list<Gtk::RadioMenuItem*>& playlists_menu_radio_menu_items()
    {

      return playlists_menu_radio_menu_items_;

    }

  private:

    Playlist& playlist_;

    Gtk::Menu *playlists_menu_;

    Gtk::MenuItem *change_playlist_menu_item_,
                  *edit_menu_item_,
                  *delete_menu_item_,
                  *queue_menu_item_,
                  *cut_menu_item_,
                  *copy_menu_item_,
                  *paste_menu_item_;
                  
    Gtk::CheckMenuItem *lock_check_menu_item_;
    
    Gtk::RadioButtonGroup playlists_menu_radio_button_group_;

    std::list<Gtk::RadioMenuItem*> playlists_menu_radio_menu_items_;

};





#endif
