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





#include "PlaylistComboBoxes.h"

#include "PlaylistComboBox.h"
#include "PlaylistComboBoxColumnRecord.h"
#include "../Playlists/Playlist.h"
#include "../Playlists/Playlists.h"
#include "../Playlists/PlaylistTreeStore.h"
#include "../Playlists/PlaylistMenu.h"
#include "../../GUI.h"
#include "../../../Base.h"

#include <memory>
#include <gtkmm/combobox.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/radiomenuitem.h>



using namespace std;





PlaylistComboBoxes::PlaylistComboBoxes(Base& base, 
                                       Playlists& temp_playlists)
: GUIElementList(base)
{


  playlist_combobox_column_record_ = new PlaylistComboBoxColumnRecord;

  playlist_combobox_treestore_ 
    = Gtk::TreeStore::create(*playlist_combobox_column_record_);


  on_playlist_combobox_changed_disabled_ = false;


  for(auto playlist_treestores_it 
        = temp_playlists.playlist_treestores().begin();
      playlist_treestores_it 
        != temp_playlists.playlist_treestores().end();
      playlist_treestores_it++)
  {


    Gtk::TreeRow row;

    row = *(playlist_combobox_treestore_ -> append());


    row[playlist_combobox_column_record_ -> name_]
      = ((*playlist_treestores_it) -> get_name());


    row[playlist_combobox_column_record_ -> playlist_treestore_it_]
      = playlist_treestores_it;


  }


  gui_elements().push_back(new PlaylistComboBox(base, *this));
  gui_elements().push_back(new PlaylistComboBox(base, *this));

}





PlaylistComboBoxes::~PlaylistComboBoxes()
{

  delete playlist_combobox_column_record_;

}





void PlaylistComboBoxes::Add_Playlist
  (const char* name, 
   list<Glib::RefPtr<PlaylistTreeStore>>::iterator playlist_treestore_it)
{



    
  Gtk::TreeRow row;

  row = *(playlist_combobox_treestore_ -> append());

  row[playlist_combobox_column_record_ -> name_] = name;

  row[playlist_combobox_column_record_ -> playlist_treestore_it_]
    = playlist_treestore_it;

  for(auto it : playlist_comboboxes()())
  {

    auto treestore_it = playlist_combobox_treestore_ -> children().end();

    treestore_it--;

    it -> playlist_combobox().set_active(treestore_it);
 
  }

}





void PlaylistComboBoxes::Remove_Playlist(Gtk::TreeIter 
                                           playlist_combobox_treestore_it)
{

  if(gui_elements().size() < 1)
  {

    return;

  }
  

  auto previous_row_it = playlist_combobox_treestore_it;


  previous_row_it--;


  playlist_comboboxes().set_on_playlist_combobox_changed_disabled(true);

  for(auto playlists_it : playlists()())
  {

    int count = 0;

    int temp_active_row_number 
      = playlist_comboboxes()().front() -> playlist_combobox()
          .get_active_row_number();

    auto playlists_menu_radio_menu_items_it
      = playlists_it -> menu().playlists_menu_radio_menu_items().begin();

    for(auto menu_it : playlists_it -> menu().playlists_menu().get_children())
    {

      if(count == temp_active_row_number)
      {

        playlists_it -> menu().playlists_menu()
          .remove(*menu_it);

        auto previous_menu_it = playlists_menu_radio_menu_items_it;

        previous_menu_it--;

        playlists_it -> menu().playlists_menu_radio_menu_items()
          .erase(playlists_menu_radio_menu_items_it);

        (*previous_menu_it) -> set_active(true);

        break;

      }

      count++;

      playlists_menu_radio_menu_items_it++;

    }

  }

  for(auto it : gui_elements())
  {

    it -> playlist_combobox().set_active(previous_row_it);

  }


  

  playlist_comboboxes().set_on_playlist_combobox_changed_disabled(false);


  playlist_combobox_treestore_ -> erase(playlist_combobox_treestore_it);


}





Glib::RefPtr<Gtk::TreeStore>& PlaylistComboBoxes::playlist_combobox_treestore()
{

  return playlist_combobox_treestore_;

}





PlaylistComboBoxColumnRecord& PlaylistComboBoxes::playlist_combobox_column_record()
{

  return *playlist_combobox_column_record_;

}
