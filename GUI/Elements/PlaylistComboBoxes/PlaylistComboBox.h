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

#ifndef PLAYLIST_COMBOBOX_H 
#define PLAYLIST_COMBOBOX_H





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

#include <glibmm/refptr.h>

#include <iostream>

#include <list>

#include <string>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class Base;

namespace Gtk
{

  class Box;

  class Button;

  class ComboBox;

  class Entry;

  class Label;

  class StackSwitcher;

  class TreeStore;

}

class PlaylistComboBoxColumnRecord;

class PlaylistTreeStore;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class PlaylistComboBox : public GUIElement<PlaylistComboBox>
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    PlaylistComboBox
      (Base& base_ref, PlaylistComboBoxes& playlist_comboboxes_ref);





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    void Add_Playlist_Button_Pressed();

    void Playlist_ComboBox_Changed();

    void Playlist_Combo_Box_Entry_Changed();

    void Remove_Playlist_Button_Pressed();

    void Set_Row_Active(int row_number);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::ComboBox& playlist_combobox();

    PlaylistComboBoxColumnRecord& playlist_combobox_column_record();

    Gtk::Entry& playlist_combobox_entry();

    Gtk::Button& remove_playlist_button();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  private:

    Gtk::Button* add_playlist_button_;

    bool constructed_;

    Gtk::ComboBox* playlist_combobox_;

    Gtk::Box* playlist_combobox_box_;

    Gtk::StackSwitcher *playlist_combobox_buttons_stackswitcher_;

    Gtk::Entry* playlist_combobox_entry_;

    Gtk::Label* playlist_combobox_label_;

    Gtk::Button* remove_playlist_button_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
