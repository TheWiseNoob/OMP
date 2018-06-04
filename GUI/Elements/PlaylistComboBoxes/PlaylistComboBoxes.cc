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

#include "PlaylistComboBoxes.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../GUI.h"

#include "../Playlists/Playlist.h"

#include "../Playlists/PlaylistMenu.h"

#include "../Playlists/Playlists.h"

#include "../Playlists/PlaylistTreeStore.h"

#include "PlaylistComboBoxColumnRecord.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/combobox.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/treeiter.h>

#include <memory>





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

PlaylistComboBoxes::PlaylistComboBoxes
  (Base& base_ref, Playlists& playlists_ref)

// Inherited Class

: GUIElementList(base_ref)

, active_row_number_(-1)

{

  // 
  playlist_combobox_column_record_ = new PlaylistComboBoxColumnRecord;

  // 
  treestore_ 
    = Gtk::TreeStore::create(*playlist_combobox_column_record_);



  // 
  on_playlist_combobox_changed_disabled_ = false;



  // 
  for(auto playlist_treestores_it 
        = playlists_ref . playlist_treestores() . begin();
      playlist_treestores_it 
        != playlists_ref . playlist_treestores() . end();
      playlist_treestores_it++)
  {

    // 
    Gtk::TreeRow row = *(treestore_ -> append());



    // 
    row[playlist_combobox_column_record_ -> name_]
      = ((*playlist_treestores_it) -> get_name());

    // 
    row[playlist_combobox_column_record_ -> playlist_treestore_it_]
      = playlist_treestores_it;

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaylistComboBoxes::~PlaylistComboBoxes()
{

  delete playlist_combobox_column_record_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void PlaylistComboBoxes::Add_Playlist
  (const char* name, 
   list<Glib::RefPtr<PlaylistTreeStore>>::iterator playlist_treestore_it)
{

  //     
  Gtk::TreeRow row = *(treestore_ -> append());

  // 
  row[playlist_combobox_column_record_ -> name_] = name;

  // 
  row[playlist_combobox_column_record_ -> playlist_treestore_it_]
    = playlist_treestore_it;



  // 
  for(auto playlist_comboboxes_it : playlist_comboboxes()())
  { 

    // 
    auto treestore_it = treestore_ -> children() . end();

    // 
    treestore_it--;

    // 
    playlist_comboboxes_it -> playlist_combobox() . set_active(treestore_it);
 
  }

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::TreeIter PlaylistComboBoxes::active_row_it()
{

  // 
  return (treestore_ -> children())[active_row_number_];

}

Glib::ustring* PlaylistComboBoxes::active_row_name()
{

  // 
  Glib::ustring active_row_name_ustr
    = (*((treestore_ -> children())[active_row_number_]))[playlist_comboboxes()
                                    . column_record() . name_];



  // 
  return new Glib::ustring(active_row_name_ustr);

}

int PlaylistComboBoxes::active_row_number()
{

  return active_row_number_;

}

std::list<Glib::RefPtr<PlaylistTreeStore>>::iterator 
  PlaylistComboBoxes::active_treestore_it()
{

  // 
  return (*((treestore_ -> children())[active_row_number_]))
           [playlist_comboboxes() . column_record() . playlist_treestore_it_];

}

bool PlaylistComboBoxes::on_playlist_combobox_changed_disabled()
{

  return on_playlist_combobox_changed_disabled_;

}

PlaylistComboBoxColumnRecord& PlaylistComboBoxes::column_record()
{

  return *playlist_combobox_column_record_;

}

Glib::RefPtr<Gtk::TreeStore>& PlaylistComboBoxes::treestore()
{

  return treestore_;

}




//         //
//         //
// Setters //////////////////////////////////////////////////////////////////
//         //
//         //

void PlaylistComboBoxes::set_active_row_number(int new_value)
{

  active_row_number_ = new_value;

}

void PlaylistComboBoxes::set_on_playlist_combobox_changed_disabled
       (bool new_value)
{

  on_playlist_combobox_changed_disabled_ = new_value;

}

