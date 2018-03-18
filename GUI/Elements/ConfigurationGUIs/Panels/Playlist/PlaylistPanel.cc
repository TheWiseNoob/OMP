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

#include "PlaylistPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../../../Configuration/Configuration.h"

#include "../../ConfigurationGUI.h"

#include "../../ConfigurationGUIs.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/box.h>

#include <gtkmm/checkbutton.h>





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

PlaylistPanel::PlaylistPanel(Base& base_ref, ConfigurationGUI& config_gui_ref,
                             Gtk::TreeRow* parent_row_ptr)

// Inherited Class

: Panel(base_ref, config_gui_ref, "Playlist", parent_row_ptr)



// Selection

, selection_box_(Gtk::manage(new Gtk::Box))

, empty_space_playlist_checkbutton_(Gtk::manage(new Gtk::CheckButton))

, empty_space_row_checkbutton_(Gtk::manage(new Gtk::CheckButton))

{

  //           //
  // Selection ////////////////////////////////////////////////////////////////
  //           //

  // 
  box() . set_center_widget(*selection_box_);

  // 
  selection_box_ -> pack_start(*empty_space_playlist_checkbutton_,
                               Gtk::PACK_EXPAND_PADDING);

  // 
  selection_box_ -> pack_end(*empty_space_row_checkbutton_,
                               Gtk::PACK_EXPAND_PADDING);



  // 
  box() . set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  // 
  selection_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // 
  box() . set_hexpand(true);

  // 
  box() . set_vexpand(true);

  // 
  selection_box_ -> set_hexpand(true);

  // 
  selection_box_ -> set_vexpand(true);



  //
  empty_space_playlist_checkbutton_
    -> set_label("Deselect all when empty space in the playlist is clicked.");

  //
  empty_space_row_checkbutton_
    -> set_label("Deselect all when empty space in a row is clicked.");



  // 
  empty_space_playlist_checkbutton_ -> set_margin_left(2);

  // 
  empty_space_playlist_checkbutton_ -> set_margin_right(2);

  // 
  empty_space_row_checkbutton_ -> set_margin_left(2);

  // 
  empty_space_row_checkbutton_ -> set_margin_right(2);



  // 
  empty_space_playlist_checkbutton_
    -> set_tooltip_text("If you click in the playlist where there is not a " \
                        "row, everything in that playlist is deselected.");

  // 
  empty_space_row_checkbutton_
    -> set_tooltip_text("If you click on a row where there is no text, " \
                        "everything in that playlist is deselected.");



  //
  empty_space_playlist_checkbutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &PlaylistPanel::On_Empty_Space_Playlist_Check_Button_Toggled_Signal));

  //
  empty_space_row_checkbutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &PlaylistPanel::On_Empty_Space_Row_Check_Button_Toggled_Signal));





  //       //
  // Other ////////////////////////////////////////////////////////////////////
  //       //

  // Adds the Apply_Saved_Value function to the ConfigGUI's list.
  config_gui_ref . Add_Apply_Saved_Value_Function(*this,
                                                  &Panel::Apply_Saved_Values);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaylistPanel::~PlaylistPanel()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void PlaylistPanel::Apply_Saved_Values()
{

  // 
  empty_space_row_checkbutton_
    -> set_active(config() . get("gui.playlist.empty_space_row_deselects"));

  // 
  empty_space_playlist_checkbutton_
    -> set_active(config() . get("gui.playlist.empty_space_row_deselects"));

}

void PlaylistPanel::On_Empty_Space_Playlist_Check_Button_Toggled_Signal()
{

  // 
  bool active = empty_space_playlist_checkbutton_ -> get_active();



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // 
  for(auto it : config_guis()())
  {

    // 
    it -> playlist_panel() . empty_space_playlist_checkbutton()
      . set_active(active);

  }



  // 
  config() . set("gui.playlist.empty_space_playlist_deselects", active); 

}

void PlaylistPanel::On_Empty_Space_Row_Check_Button_Toggled_Signal()
{ 

  // 
  bool active = empty_space_row_checkbutton_ -> get_active();



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // 
  for(auto it : config_guis()())
  {

    // 
    it -> playlist_panel() . empty_space_row_checkbutton()
      . set_active(active);

  }



  // 
  config() . set("gui.playlist.empty_space_row_deselects", active); 

}




 
//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::CheckButton& PlaylistPanel::empty_space_playlist_checkbutton()
{

  // 
  return *empty_space_playlist_checkbutton_;

}

Gtk::CheckButton& PlaylistPanel::empty_space_row_checkbutton()
{

  // 
  return *empty_space_row_checkbutton_;

}
