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

#include "PlaylistCreateDialog.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../ChildWindows/ChildWindow.h"

#include "../../GUI.h"

#include "Playlists.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/applicationwindow.h>

#include <gtkmm/box.h>

#include <gtkmm/button.h>

#include <gtkmm/window.h>

#include <gtkmm/dialog.h>

#include <gtkmm/entry.h>

#include <gtkmm/infobar.h>





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

PlaylistCreateDialog::PlaylistCreateDialog(Base& base_ref)

// Inherited Class

: Parts(base_ref)



// General

, add_playlist_button_(Gtk::manage(new Gtk::Button("Create Playlist")))

, button_box_(Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL)))

, cancel_button_(Gtk::manage(new Gtk::Button("Cancel")))

, info_bar_(Gtk::manage(new Gtk::InfoBar))

, info_bar_label_(Gtk::manage(new Gtk::Label("", 0.5, 0.5)))

, playlist_creation_window_(new Gtk::Window)

, playlist_creation_window_box_(Gtk::manage(new Gtk::Box))

, playlist_name_entry_(Gtk::manage(new Gtk::Entry))

{

  // 
  playlist_creation_window_ -> add(*playlist_creation_window_box_);



  // 
  playlist_creation_window_box_
    -> append(*info_bar_, Gtk::PACK_SHRINK);

  // 
  playlist_creation_window_box_
    -> pack_end(*button_box_, Gtk::PACK_SHRINK);

  // 
  playlist_creation_window_box_
    -> pack_end(*playlist_name_entry_, Gtk::PACK_EXPAND_PADDING);



  // 
  playlist_creation_window_box_ -> set_margin_start(3);

  // 
  playlist_creation_window_box_ -> set_margin_end(3);

  // 
  playlist_creation_window_box_ -> set_margin_top(3);

  // 
  playlist_creation_window_box_ -> set_margin_bottom(3);



  // 
  button_box_ -> append(*cancel_button_, Gtk::PACK_SHRINK);

  // 
  button_box_ -> pack_end(*add_playlist_button_, Gtk::PACK_SHRINK);



  // 
  info_bar_ -> set_center_widget(*info_bar_label_);


  // 
  playlist_creation_window_box_ -> set_orientation(Gtk::Orientation::VERTICAL);



  // 
  playlist_creation_window_ -> set_position(Gtk::WIN_POS_CENTER);

  // 
  playlist_creation_window_ -> set_size_request(200, 100);

  // 
  playlist_creation_window_ -> set_resizable(false);

  // 
  playlist_creation_window_
    -> set_title("Create New Playlist");



  // 
  playlist_name_entry_ -> signal_activate()
    . connect(sigc::mem_fun
        (*this, 
         &PlaylistCreateDialog::On_Playlist_Name_Entry_Activate_Signal));

  // 
  add_playlist_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &PlaylistCreateDialog::On_Add_Playlist_Button_Clicked_Signal));

  // 
  cancel_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &PlaylistCreateDialog::On_Cancel_Button_Clicked_Signal));



  //
  playlist_creation_window_  -> signal_key_press_event()
    . connect(sigc::mem_fun(*this, &PlaylistCreateDialog::On_Key_Press_Event));

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaylistCreateDialog::~PlaylistCreateDialog()
{

  delete playlist_creation_window_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void PlaylistCreateDialog::On_Add_Playlist_Button_Clicked_Signal()
{

  On_Playlist_Name_Entry_Activate_Signal();

}

void PlaylistCreateDialog::On_Cancel_Button_Clicked_Signal()
{

  playlist_creation_window_ -> hide();

  info_bar_ -> hide();

  playlist_name_entry_ -> set_text("");

}

void PlaylistCreateDialog::On_Playlist_Name_Entry_Activate_Signal()
{

  // 
  string new_playlist_name = playlist_name_entry_ -> get_text() . c_str();



  // 
  if(new_playlist_name != "")
  {

    // 
    if(playlists() . Add_Playlist(new_playlist_name . c_str()))
    {

      // 
      playlist_name_entry_ -> set_text("");



      // 
      playlist_creation_window_ -> hide();

    }

    // True if the playlist with that name already exists.
    else
    {

      // 
      string info_bar_label_text = "\"";

      // 
      info_bar_label_text += new_playlist_name + "\" already exists!";

      // 
      info_bar_label_ -> set_text(info_bar_label_text); 



      // 
      info_bar_label_ -> show();

      // 
      info_bar_ -> show();

    }

  }

  // True if no name was put in the entry.
  else
  {

    // 
    info_bar_label_ -> set_text("No name entered!"); 



    // 
    info_bar_label_ -> show();

    // 
    info_bar_ -> show();

  } 

}

bool PlaylistCreateDialog::On_Key_Press_Event(GdkEventKey* event)
{

  // Is true if the escape key is pressed.
  if((event -> keyval == GDK_KEY_Escape))
  {

    playlist_creation_window_ -> hide();

    info_bar_ -> hide();

    playlist_name_entry_ -> set_text("");


    // 
    return true;

  }



  // Allows normal keyboard event propagation.
  return false;

}

void PlaylistCreateDialog::Run()
{

  // 
  playlist_creation_window_ 
    -> set_transient_for(gui() . main_window() -> window());

  // 
  playlist_creation_window_ -> set_modal(true);



  // 
  playlist_name_entry_ -> show();

  // 
  button_box_ -> show();

  // 
  cancel_button_ -> show();

  // 
  add_playlist_button_ -> show();

  // 
  playlist_creation_window_box_ -> show();



  // 
  info_bar_ -> hide();



  // 
  playlist_creation_window_ -> set_position(Gtk::WIN_POS_CENTER);



  // 
  playlist_creation_window_ -> show();

}
