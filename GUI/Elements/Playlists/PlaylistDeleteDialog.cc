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

#include "PlaylistDeleteDialog.h"





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

#include <gtkmm/label.h>

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

PlaylistDeleteDialog::PlaylistDeleteDialog(Base& base_ref)

// Inherited Class

: Parts(base_ref)



// General

, button_box_(Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL)))

, cancel_button_(Gtk::manage(new Gtk::Button("Cancel")))

, delete_playlist_button_(Gtk::manage(new Gtk::Button("Delete Playlist")))

, playlist_creation_window_(new Gtk::Window)

, playlist_creation_window_box_(Gtk::manage(new Gtk::Box))

, playlist_prompt_label_(Gtk::manage(new Gtk::Label))

{

  // 
  playlist_creation_window_ -> add(*playlist_creation_window_box_);



  // 
  playlist_creation_window_box_
    -> pack_end(*button_box_, Gtk::PACK_SHRINK);

  // 
  playlist_creation_window_box_
    -> pack_end(*playlist_prompt_label_, Gtk::PACK_EXPAND_PADDING);



  // 
  button_box_ -> pack_start(*cancel_button_, Gtk::PACK_SHRINK);

  // 
  button_box_ -> pack_end(*delete_playlist_button_, Gtk::PACK_SHRINK);



  // 
  playlist_creation_window_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // 
  playlist_creation_window_ -> set_position(Gtk::WIN_POS_CENTER);

  // 
  playlist_creation_window_ -> set_size_request(200, 100);

  // 
  playlist_creation_window_ -> set_resizable(false);

  // 
  playlist_creation_window_
    -> set_title("Delete Playlist Confirmation Dialog");



  // 
  playlist_prompt_label_ -> set_text("Delete playlist?");
  



  // 
  delete_playlist_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &PlaylistDeleteDialog::On_Delete_Playlist_Button_Clicked_Signal));

  // 
  cancel_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &PlaylistDeleteDialog::On_Cancel_Button_Clicked_Signal));



  //
  playlist_creation_window_  -> signal_key_press_event()
    . connect(sigc::mem_fun(*this, &PlaylistDeleteDialog::On_Key_Press_Event));

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaylistDeleteDialog::~PlaylistDeleteDialog()
{

  delete playlist_creation_window_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void PlaylistDeleteDialog::On_Delete_Playlist_Button_Clicked_Signal()
{

  // 
  playlists() . Delete_Current_Playlist(delete_playlist_combobox_playlist_);



  // 
  playlist_creation_window_ -> hide();

}

void PlaylistDeleteDialog::On_Cancel_Button_Clicked_Signal()
{

  // 
  playlist_creation_window_ -> hide();

}

bool PlaylistDeleteDialog::On_Key_Press_Event(GdkEventKey* event)
{

  // Is true if the escape key is pressed.
  if((event -> keyval == GDK_KEY_Escape))
  {

    // 
    playlist_creation_window_ -> hide();



    // 
    return true;

  }



  // Allows normal keyboard event propagation.
  return false;

}

void PlaylistDeleteDialog::Run(bool new_delete_playlist_combobox_playlist)
{

  // 
  delete_playlist_combobox_playlist_ = new_delete_playlist_combobox_playlist;



  // 
  playlist_creation_window_ 
    -> set_transient_for(gui() . main_window() -> window());

  // 
  playlist_creation_window_ -> set_modal(true);



  // 
  playlist_prompt_label_ -> show();

  // 
  button_box_ -> show();

  // 
  cancel_button_ -> show();

  // 
  delete_playlist_button_ -> show();

  //
  delete_playlist_button_ -> grab_focus();

  // 
  playlist_creation_window_box_ -> show();



  //
  Glib::ustring playlist_prompt_label_ustr
    = "Permanently delete <b>"
       + playlists() . selected_playlist() . active_playlist_name() + "</b>?";

  // 
  playlist_prompt_label_ -> set_markup(playlist_prompt_label_ustr);



  // 
  playlist_creation_window_ -> set_position(Gtk::WIN_POS_CENTER);



  // 
  playlist_creation_window_ -> show();

}
