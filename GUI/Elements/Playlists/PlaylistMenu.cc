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

#include "PlaylistMenu.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Playback/Playback.h"

#include "Playlist.h"

#include "Playlists.h"

#include "PlaylistTreeStore.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/checkmenuitem.h>

#include <gtkmm/menuitem.h>

#include <gtkmm/radiobuttongroup.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/separatormenuitem.h>

#include <iostream>

#include <string>





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

PlaylistMenu::PlaylistMenu(Base& base, Playlist& playlist_ref,
                           Playlists& playlists_ref, const char* playlist_name)

// Inherited Class

: Parts(base)



// General

, add_playlist_menu_item_
    (Gtk::manage(new Gtk::MenuItem("Create New Playlist")))

, change_playlist_menu_item_(Gtk::manage(new Gtk::MenuItem()))

, copy_menu_item_(Gtk::manage(new Gtk::MenuItem("Copy Row(s)")))

, cut_menu_item_(Gtk::manage(new Gtk::MenuItem("Cut Row(s)")))

, delete_menu_item_(Gtk::manage(new Gtk::MenuItem("_Delete Row(s)", true)))

, edit_menu_item_(Gtk::manage(new Gtk::MenuItem("_Edit", true)))

, lock_check_menu_item_
    (Gtk::manage(new Gtk::CheckMenuItem("_Lock Playlist", true)))

, queue_menu_item_(Gtk::manage(new Gtk::MenuItem("_Queue Row(s)", true)))

, paste_menu_item_(Gtk::manage(new Gtk::MenuItem("Paste Row(s)")))

, playlist_(playlist_ref)

, playlists_menu_(Gtk::manage(new Gtk::Menu))

, delete_playlist_menu_item_
    (Gtk::manage(new Gtk::MenuItem("Delete Current Playlist")))

{

//           //
// Playlists ////////////////////////////////////////////////////////////////
//           //

// 
append(*change_playlist_menu_item_);

// 
append(*add_playlist_menu_item_);



//
string menu_item_name_str = "Playlist: ";

// 
menu_item_name_str += playlist_name;

// 
change_playlist_menu_item_ -> set_label(menu_item_name_str);




// 
add_playlist_menu_item_ -> signal_activate()
. connect(sigc::mem_fun(playlists_ref,
		    &Playlists::Open_Create_Playlist_Dialog));

// 
delete_playlist_menu_item_ -> signal_activate()
. connect(sigc::bind
(sigc::mem_fun(playlists_ref,
	       &Playlists::Open_Delete_Playlist_Dialog), false));



//
append(*delete_playlist_menu_item_);

// 
append(*lock_check_menu_item_);



// 
Gdk::RGBA change_playlist_menu_item_rgba;

// 
change_playlist_menu_item_rgba . set_rgba(0.0,0.0,0.0,.5);



// 
Pango::FontDescription change_playlist_menu_item_font_description;

// 
change_playlist_menu_item_font_description . set_weight(Pango::WEIGHT_HEAVY);

// 
change_playlist_menu_item_font_description
. set_stretch(Pango::STRETCH_ULTRA_EXPANDED);

// 
change_playlist_menu_item_ 
-> override_background_color(change_playlist_menu_item_rgba, 
			 Gtk::STATE_FLAG_NORMAL);

// 
change_playlist_menu_item_ 
-> override_font(change_playlist_menu_item_font_description);



// 
change_playlist_menu_item_ -> set_submenu(*playlists_menu_);



// 
int count = 0;



// 
string playlist_treestore_name = playlist_name;



// 
for(auto playlist_treestores_it : playlists_ref . playlist_treestores())
{ 

// 
Gtk::RadioMenuItem *playlist_refs_menu_radio_menu_item
= Gtk::manage(new Gtk::RadioMenuItem
		  (playlists_menu_radio_button_group()));

// 
playlists_menu_radio_menu_items_
. push_back(playlist_refs_menu_radio_menu_item);

// 
playlist_refs_menu_radio_menu_item 
-> set_label(playlist_treestores_it -> get_name()); 



// 
if((playlist_treestores_it -> get_name()) == playlist_treestore_name)
{

// 
playlist_refs_menu_radio_menu_item -> set_active(true);

// 
change_playlist_menu_item_
-> set_label("Playlist: " + playlist_treestores_it -> get_name()); 

}



// 
playlists_menu_ -> append(*playlist_refs_menu_radio_menu_item); 



// 
playlist_refs_menu_radio_menu_item -> signal_activate()
. connect(sigc::mem_fun(playlist_ref, &Playlist::Change_Playlist));



// 
count++;

}



// 
lock_check_menu_item_ -> signal_toggled()
. connect(sigc::mem_fun(playlist_ref, &Playlist::Lock));





/*
//          //
// Metadata /////////////////////////////////////////////////////////////////
//          //

// 
append(*Gtk::manage(new Gtk::SeparatorMenuItem));

// 
append(*edit_menu_item_);
*/





//       //
// Queue ////////////////////////////////////////////////////////////////////
//       //

// 
append(*Gtk::manage(new Gtk::SeparatorMenuItem));

// 
append(*queue_menu_item_);



// 
queue_menu_item_ -> signal_activate()
. connect(sigc::mem_fun(playlist_ref, &Playlist::Queue_Rows));





//         //
// Editing //////////////////////////////////////////////////////////////////
//         //

// 
append(*Gtk::manage(new Gtk::SeparatorMenuItem));

// 
append(*cut_menu_item_);

// 
append(*copy_menu_item_);

// 
append(*delete_menu_item_);

// 
append(*paste_menu_item_);



// 
copy_menu_item_ -> signal_activate()
. connect(sigc::mem_fun(playlist_ref, &Playlist::Copy_Selected_Rows));

// 
cut_menu_item_ -> signal_activate()
. connect(sigc::mem_fun(playlist_ref, &Playlist::Cut_Selected_Rows));

// 
delete_menu_item_ -> signal_activate()
. connect(sigc::mem_fun(playlist_ref, &Playlist::Delete_Selected_Rows));

// 
paste_menu_item_ -> signal_activate()
. connect(sigc::mem_fun(playlist_ref, &Playlist::Paste_Clipboard_Rows));





// 
show_all_children();

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaylistMenu::~PlaylistMenu()
{

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::MenuItem& PlaylistMenu::change_playlist_menu_item()
{

  return *change_playlist_menu_item_;

}

Gtk::MenuItem& PlaylistMenu::copy_menu_item()
{

  return *copy_menu_item_;

}

Gtk::MenuItem& PlaylistMenu::cut_menu_item()
{

  return *cut_menu_item_;

}

Gtk::MenuItem& PlaylistMenu::delete_menu_item()
{

  return *delete_menu_item_;

}

Gtk::MenuItem& PlaylistMenu::delete_playlist_menu_item()
{

  return *delete_playlist_menu_item_;

}

Gtk::MenuItem& PlaylistMenu::edit_menu_item()
{

  return *edit_menu_item_;

}

Gtk::CheckMenuItem& PlaylistMenu::lock_check_menu_item()
{

  return *lock_check_menu_item_;

}

Gtk::MenuItem& PlaylistMenu::queue_menu_item()
{

  return *queue_menu_item_;

}

Gtk::MenuItem& PlaylistMenu::paste_menu_item()
{

  return *paste_menu_item_;

}

Gtk::Menu& PlaylistMenu::playlists_menu()
{

  return *playlists_menu_;

}

Gtk::RadioButtonGroup& PlaylistMenu::playlists_menu_radio_button_group()
{

  return playlists_menu_radio_button_group_;

}

std::list<Gtk::RadioMenuItem*>& PlaylistMenu::playlists_menu_radio_menu_items()
{

  return playlists_menu_radio_menu_items_;

}
