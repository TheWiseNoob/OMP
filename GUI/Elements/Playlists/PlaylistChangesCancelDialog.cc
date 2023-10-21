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

#include "PlaylistChangesCancelDialog.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../../Playback/Playback.h"

#include "../ChildWindows/ChildWindow.h"

#include "../../GUI.h"

#include "Playlists.h"

#include "PlaylistsDatabase.h"





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

PlaylistChangesCancelDialog::PlaylistChangesCancelDialog(Base& base_ref)

// Inherited Class

: Parts(base_ref)



// General

, button_box_(Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL)))

, cancel_button_(Gtk::manage(new Gtk::Button("Keep OMP Open")))

, delete_playlist_button_
    (Gtk::manage(new Gtk::Button("Force Close & Cancel Database Updates")))

, playlist_creation_window_(new Gtk::Window)

, playlist_creation_window_box_(Gtk::manage(new Gtk::Box))

, playlist_prompt_label_(Gtk::manage(new Gtk::Label))

, release_twice_(false)

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
  button_box_ -> append(*cancel_button_, Gtk::PACK_SHRINK);

  // 
  button_box_ -> pack_end(*delete_playlist_button_, Gtk::PACK_SHRINK);



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
    -> set_title("Force Close Playlists Rebuild Dialog");



  // 
  playlist_prompt_label_ -> set_text("Delete playlist?");



  // 
  delete_playlist_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &PlaylistChangesCancelDialog
                   ::On_Changes_Cancel_Playlist_Button_Clicked_Signal));

  // 
  cancel_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &PlaylistChangesCancelDialog
                   ::On_Cancel_Button_Clicked_Signal));



  //
  playlist_creation_window_  -> signal_key_press_event()
    . connect(sigc::mem_fun(*this, &PlaylistChangesCancelDialog
                                      ::On_Key_Press_Event));

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaylistChangesCancelDialog::~PlaylistChangesCancelDialog()
{

  delete playlist_creation_window_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void PlaylistChangesCancelDialog::
  On_Changes_Cancel_Playlist_Button_Clicked_Signal()
{

  // 
  base() . quitting() = true;



  // 
  playback() . Stop();



  // 
  playback() . Quit();



  // 
  if(!(config() . get("playback.queue_saved")))
  {

    // 
    playlists() . database() . Clear_Playlist("Queue");

  }



  //
  playlists() . database() . Cleanup_Database();



  // 
  if(release_twice_)
  {

    // Releases the application window.
    gui() . main_window() -> window() . get_application() -> release();

  }

  // Releases the application window.
  gui() . main_window() -> window() . get_application() -> release();



  // 
  base() . quitting() = true;



  // 
  playlist_creation_window_ -> hide();

}

void PlaylistChangesCancelDialog::On_Cancel_Button_Clicked_Signal()
{

  // 
  playlist_creation_window_ -> hide();

}

bool PlaylistChangesCancelDialog::On_Key_Press_Event(GdkEventKey* event)
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

void PlaylistChangesCancelDialog::Run(bool set_release_twice)
{

  // 
  release_twice_ = set_release_twice;



  // 
  Glib::ustring active_playlist_name_str
    = playlists() . selected_playlist() . active_playlist_name();



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
    = "If you force close the program, pending changes to the playlists\n" \
      "database will be lost.";

  // 
  playlist_prompt_label_ -> set_markup(playlist_prompt_label_ustr);



  // 
  playlist_prompt_label_ -> set_line_wrap(true);



  // 
  playlist_creation_window_ -> set_position(Gtk::WIN_POS_CENTER);



  // 
  playlist_creation_window_ -> show();

}
