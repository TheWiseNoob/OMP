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





#include "PlaylistMenu.h"





#include "../../../Playback/Playback.h"
#include "PlaylistTreeStore.h"
#include "Playlist.h"
#include "Playlists.h"

#include <iostream>

#include <gtkmm/menuitem.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/radiomenuitem.h>
#include <gtkmm/radiobuttongroup.h>
#include <gtkmm/separatormenuitem.h>





PlaylistMenu::PlaylistMenu(Base& base, Playlist& new_playlist,
                           Playlists& temp_playlists)
: Parts(base)
, playlist_(new_playlist)
, playlists_menu_(Gtk::manage(new Gtk::Menu))
, change_playlist_menu_item_(Gtk::manage(new Gtk::MenuItem("Playlist: " + playlist_.active_playlist())))
, edit_menu_item_(Gtk::manage(new Gtk::MenuItem("_Edit", true)))
, delete_menu_item_(Gtk::manage(new Gtk::MenuItem("_Delete", true)))
, queue_menu_item_(Gtk::manage(new Gtk::MenuItem("_Queue", true)))
, cut_menu_item_(Gtk::manage(new Gtk::MenuItem("Cut")))
, copy_menu_item_(Gtk::manage(new Gtk::MenuItem("Copy")))
, paste_menu_item_(Gtk::manage(new Gtk::MenuItem("Paste")))
, lock_check_menu_item_(Gtk::manage(new Gtk::CheckMenuItem("_Lock Playlist", true)))
{

  // 
  append(*change_playlist_menu_item_);

  // 
  append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  append(*edit_menu_item_);

  // 
  append(*delete_menu_item_);

  // 
  append(*queue_menu_item_);

  // 
  append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  append(*cut_menu_item_);

  // 
  append(*copy_menu_item_);

  // 
  append(*paste_menu_item_);

  // 
  append(*Gtk::manage(new Gtk::SeparatorMenuItem));

  // 
  append(*lock_check_menu_item_);



  // 
  Gdk::RGBA change_playlist_menu_item_rgba;

  // 
  change_playlist_menu_item_rgba . set_rgba(0.0,0.0,0.0,.5);



  Pango::FontDescription change_playlist_menu_item_font_description;
  change_playlist_menu_item_font_description.set_weight(Pango::WEIGHT_HEAVY);
  change_playlist_menu_item_font_description.set_stretch(Pango::STRETCH_ULTRA_EXPANDED);
  change_playlist_menu_item_ 
    -> override_background_color(change_playlist_menu_item_rgba, 
                                 Gtk::STATE_FLAG_NORMAL);
  change_playlist_menu_item_ 
    -> override_font(change_playlist_menu_item_font_description);
  change_playlist_menu_item_ -> set_submenu(*playlists_menu_);


  // 
  int count = 0;



  // 
  for(auto playlist_treestores_it : temp_playlists.playlist_treestores())
  {

    Gtk::RadioMenuItem *new_playlists_menu_radio_menu_item
      = Gtk::manage(new Gtk::RadioMenuItem
                          (playlists_menu_radio_button_group())); 
 
    playlists_menu_radio_menu_items_
      . push_back(new_playlists_menu_radio_menu_item);
 
    new_playlists_menu_radio_menu_item 
      -> set_label(playlist_treestores_it -> get_name()); 

    if(count == 1)
    {

      new_playlists_menu_radio_menu_item -> set_active(true);

      change_playlist_menu_item_
        -> set_label("Playlist: " + playlist_treestores_it -> get_name()); 

    }



    // 
    playlists_menu_ -> append(*new_playlists_menu_radio_menu_item); 



    // 
    new_playlists_menu_radio_menu_item -> signal_activate()
      . connect(sigc::mem_fun(new_playlist, &Playlist::Change_Playlist));



    // 
    count++;

   } 
 


  // 
  copy_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(new_playlist, &Playlist::Copy_Selected_Rows));

  // 
  cut_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(new_playlist, &Playlist::Cut_Selected_Rows));

  // 
  delete_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(new_playlist, &Playlist::Delete_Selected_Rows));

  // 
  paste_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(new_playlist, &Playlist::Paste_Clipboard_Rows));

  // 
  queue_menu_item_ -> signal_activate()
    . connect(sigc::mem_fun(new_playlist, &Playlist::Queue_Rows));





  // 
  show_all_children();

}






PlaylistMenu::~PlaylistMenu()
{

}





Gtk::RadioButtonGroup& PlaylistMenu::playlists_menu_radio_button_group()
{

  return playlists_menu_radio_button_group_;

}
