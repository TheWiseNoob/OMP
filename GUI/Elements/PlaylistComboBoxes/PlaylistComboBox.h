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





#ifndef PLAYLIST_COMBOBOX_H 
#define PLAYLIST_COMBOBOX_H





#include <gtkmm/box.h>
#include "../../GUIElement.h"
#include "../../../Parts.h"



#include <glibmm/refptr.h>
#include <string>
#include <iostream>
#include <list>



class Base;
class PlaylistTreeStore;
class PlaylistComboBoxColumnRecord;

namespace Gtk
{

  class TreeStore;
  class ComboBox;
  class Label;
  class Entry;
  class StackSwitcher;
  class Button;

}





class PlaylistComboBox : public Gtk::Box, public GUIElement<PlaylistComboBox>
{

  public:

    PlaylistComboBox(Base &new_main,
                     PlaylistComboBoxes& temp_playlist_comboboxes);


    void On_Playlist_ComboBox_Changed();

    void On_Playlist_Combo_Box_Entry_Changed();

    void Set_Row_Active(int row_number);

    void On_Add_Playlist_Button_Pressed();

    void On_Remove_Playlist_Button_Pressed();


    std::list<Glib::RefPtr<PlaylistTreeStore>>::iterator active_row_treestore_it();

    Gtk::ComboBox& playlist_combobox()
    {

      return *playlist_combobox_;

    }


    PlaylistComboBoxColumnRecord& playlist_combobox_column_record();

    Gtk::Entry& playlist_combobox_entry()
    {

      return *playlist_combobox_entry_;

    }

    Gtk::Button& remove_playlist_button()
    {

      return *remove_playlist_button_;

    }


  private:

    //Child Widgets                                                             
    Gtk::ComboBox* playlist_combobox_;

    Gtk::Box* playlist_combobox_box_;

    Gtk::Label* playlist_combobox_label_;

    Gtk::Entry* playlist_combobox_entry_;

    Gtk::StackSwitcher *playlist_combobox_buttons_stackswitcher_;

    Gtk::Button* add_playlist_button_;

    Gtk::Button* remove_playlist_button_;

    bool constructed_;

    int active_row_number_;



};





#endif
