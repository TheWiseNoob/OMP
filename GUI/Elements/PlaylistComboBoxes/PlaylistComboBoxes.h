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





#ifndef PLAYLIST_COMBOBOXES_H
#define PLAYLIST_COMBOBOXES_H





#include "../../GUIElementList.h"
#include "PlaylistComboBox.h"



#include <gtkmm/treestore.h>
#include <glibmm/refptr.h>
#include <list>
#include <memory>



class PlaylistComboBoxColumnRecord;

class PlaylistTreeStore;





class PlaylistComboBoxes : public GUIElementList<PlaylistComboBox>
{ 

  public:

    PlaylistComboBoxes(Base& base,
                       Playlists& temp_playlists);

    virtual ~PlaylistComboBoxes();

  //Public Member Functions

    void Add_Playlist
      (const char* name, 
       std::list<Glib::RefPtr<PlaylistTreeStore>>::iterator 
         playlist_treestore_it);

    void Remove_Playlist(Gtk::TreeIter playlist_comboboxes_treestore_it);

  //Getters and Setters

    Glib::RefPtr<Gtk::TreeStore>& playlist_combobox_treestore();

    PlaylistComboBoxColumnRecord& playlist_combobox_column_record();

    bool on_playlist_combobox_changed_disabled()
    {

      return on_playlist_combobox_changed_disabled_;

    }

    void set_on_playlist_combobox_changed_disabled(bool new_value)
    {

      on_playlist_combobox_changed_disabled_ = new_value;

    }

  private:

    Glib::RefPtr<Gtk::TreeStore> playlist_combobox_treestore_;

    PlaylistComboBoxColumnRecord* playlist_combobox_column_record_;

    bool on_playlist_combobox_changed_disabled_;



};





#endif
