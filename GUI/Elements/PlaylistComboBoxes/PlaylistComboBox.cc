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

#include "PlaylistComboBox.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../GUI.h"

#include "../Playlists/PlaylistMenu.h"

#include "../Playlists/Playlists.h"

#include "../Playlists/PlaylistsDatabase.h"

#include "../Playlists/PlaylistTreeStore.h"

#include "PlaylistComboBoxColumnRecord.h"

#include "PlaylistComboBoxes.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/button.h>

#include <gtkmm/combobox.h>

#include <gtkmm/entry.h>

#include <gtkmm/label.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/stackswitcher.h>





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

PlaylistComboBox::PlaylistComboBox
  (Base &base_ref, PlaylistComboBoxes& playlist_comboboxes_ref)

// Inherited Class

: GUIElement(base_ref, playlist_comboboxes_ref())



// General Initialization

, add_playlist_button_(Gtk::manage(new Gtk::Button))

, constructed_(false)

, playlist_combobox_(Gtk::manage(new Gtk::ComboBox(true)))

, playlist_combobox_box_(Gtk::manage(new Gtk::Box))

, playlist_combobox_buttons_stackswitcher_(Gtk::manage(new Gtk::StackSwitcher))

, playlist_combobox_entry_(playlist_combobox_ -> get_entry())

, playlist_combobox_label_(Gtk::manage(new Gtk::Label))

, remove_playlist_button_(Gtk::manage(new Gtk::Button))

{ 

  //          //
  // ComboBox /////////////////////////////////////////////////////////////////
  //          //

  // 
  box() . set_center_widget(*playlist_combobox_box_);

  // 
  playlist_combobox_box_
    -> pack_end(*playlist_combobox_buttons_stackswitcher_, Gtk::PACK_SHRINK);

  // 
  playlist_combobox_buttons_stackswitcher_ 
    -> pack_start(*playlist_combobox_, Gtk::PACK_EXPAND_WIDGET);



  // 
  playlist_combobox_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);



  // 
  playlist_combobox_
    -> set_model(playlist_comboboxes_ref . treestore());

  //
  playlist_combobox_ -> set_tooltip_text
    ("Changes the playlist of all playlist views that aren't locked.");

  //
  playlist_combobox_entry_ -> set_tooltip_text
    ("Renames the currently selected playlist of this playlist combobox.");



  // 
  box() . set_margin_start(5);                                        

  // 
  box() . set_margin_end(5);                                          

  // 
  box() . set_margin_top(5);                                          

  // 
  box() . set_margin_bottom(5);                                       



  // 
  playlist_combobox_ -> set_id_column(0);

  // 
  playlist_combobox_
    -> set_entry_text_column(playlist_comboboxes_ref
                               . column_record() . name_);



  // 
  playlist_combobox_entry_ -> set_sensitive(true);



  // 
  auto temp_treestore 
    = playlist_comboboxes_ref . treestore();

  // 
  playlist_combobox_ -> set_entry_text_column
    (playlist_comboboxes_ref . column_record() . name_);

  // 
  playlist_combobox_entry_ -> signal_activate() . connect
    (sigc::mem_fun((*this), 
                   &PlaylistComboBox::Playlist_Combo_Box_Entry_Changed));

  // 
  playlist_combobox_ -> signal_changed() . connect
    (sigc::mem_fun((*this), &PlaylistComboBox::Playlist_ComboBox_Changed)); 



  // 
  if(playlist_comboboxes_ref . active_row_number() < 0)
  {

    // 
    int count = 0;

    // 
    string active_playlist_name
      = config() . get("gui.playlist_combobox.active");



    // 
    for(auto playlist_comboboxes_treestore_it
          : playlist_comboboxes_ref
              . treestore() -> children())
    {
  
      // 
      Gtk::TreeRow row = *playlist_comboboxes_treestore_it;
  
      // 
      Glib::ustring playlist_name
        = row[playlist_comboboxes_ref . column_record() . name_];



      // 
      if(playlist_name == active_playlist_name)
      {
 
        // 
        playlist_comboboxes_ref . set_active_row_number(count);


 
        // 
        break;
  
      }



      // 
      count++;
  
    }

  }



  // 
  playlist_combobox_
    -> set_active(playlist_comboboxes_ref . active_row_number());



  // 
  if(playlist_comboboxes_ref . active_row_number() < 2)
  {

    // 
    playlist_combobox_entry() . set_sensitive(false);

    // 
    remove_playlist_button() . set_sensitive(false);

  }





  //         //
  // Buttons //////////////////////////////////////////////////////////////////
  //         //

  // 
  playlist_combobox_buttons_stackswitcher_ 
    -> pack_end(*remove_playlist_button_, Gtk::PACK_SHRINK);

  // 
  playlist_combobox_buttons_stackswitcher_ 
    -> pack_end(*add_playlist_button_, Gtk::PACK_SHRINK);



  // 
  add_playlist_button_ -> set_image_from_icon_name
    ("list-add-symbolic", Gtk::ICON_SIZE_SMALL_TOOLBAR);

  // 
  remove_playlist_button_ -> set_image_from_icon_name
    ("list-remove-symbolic", Gtk::ICON_SIZE_SMALL_TOOLBAR);



  // 
  add_playlist_button_ -> set_tooltip_text
    ("Opens a dialog window for creating a new playlist.");

  // 
  remove_playlist_button_ -> set_tooltip_text
    ("Opens a dialog window for deleting the current playlist of this " \
     "playlist combobox.");



  // 
  add_playlist_button_ -> signal_clicked() . connect(sigc::mem_fun
    ((*this), &PlaylistComboBox::Add_Playlist_Button_Pressed)); 

  // 
  remove_playlist_button_ -> signal_clicked() . connect(sigc::mem_fun
    ((*this), &PlaylistComboBox::Remove_Playlist_Button_Pressed)); 

 



  // 
  constructed_ = true;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void PlaylistComboBox::Add_Playlist_Button_Pressed()
{

  playlists() . Open_Create_Playlist_Dialog();

}

void PlaylistComboBox::Playlist_ComboBox_Changed()
{                      

  // 
  if((!constructed_)
       ||
     (playlist_comboboxes() . on_playlist_combobox_changed_disabled()))
  { 

    // 
    return;

  }



  // 
  playlist_comboboxes().set_on_playlist_combobox_changed_disabled(true);



  //
  Gtk::TreeModel::iterator iter = playlist_combobox_ -> get_active();

  // 
  static Gtk::TreeModel::iterator playlist_combobox_iterator;



  // 
  if(iter)                                                                      
  {

    //     
    playlist_combobox_iterator = iter;



    // 
    Gtk::TreeModel::Row row = *iter;                                            



    // 
    if(row)                                                                     
    {  
                                                                          
      // 
      Glib::ustring name = row[playlist_combobox_column_record() . name_];

      // 
      config() . set("gui.playlist_combobox.active", name);

      



      // 
      list<PlaylistComboBox*>::iterator playlist_comboboxes_it 
        = playlist_comboboxes()() . begin();

      // 
      list<Glib::RefPtr<PlaylistTreeStore>>::iterator playlist_treestore_it
        = row[playlist_combobox_column_record() . playlist_treestore_it_];

      // 
      auto playlist_treestore = *playlist_treestore_it;



      // 
      for(auto playlists_it : playlists()())
      {

        // 
        if(playlists_it -> Locked())
        {
 
 
        } 

        // 
        else
        {

          // 
          if((playlist_combobox() . get_active_row_number()) != -1)
          { 

            // 
            playlist_comboboxes() . set_active_row_number
              (playlist_combobox() . get_active_row_number());

          }



          // 
          int count = 0;



          // 
          for(Gtk::Widget* menu_it 
                : 
              playlists_it -> menu() . playlists_menu() . get_children())
          {

            // 
            if(count == playlist_comboboxes() . active_row_number())
            {

              // 
              Gtk::RadioMenuItem* temp_radio_menu_item_ptr
                = dynamic_cast<Gtk::RadioMenuItem*>(menu_it);



              // 
              temp_radio_menu_item_ptr -> set_active(true);



              // 
              playlists_it -> menu() . change_playlist_menu_item()
                . set_label("Playlist: " 
                              + playlist_combobox_entry_ -> get_text());



              // 
              string setting_name = "gui.playlist.view.";

              // 
              setting_name += playlists_it -> playlist_view_name();

              // 
              setting_name += ".active";



              // 
              config() . set(setting_name . c_str(),
                             playlist_treestore -> get_name());



              // 
              config() . write_file();



              // 
              break;

            }



            // 
            count++;

          } 

        } 



        // 
        playlists_it++;

      } 



      // 
      while(playlist_comboboxes_it  != playlist_comboboxes()().end())                  
      {

        for(auto it : playlist_comboboxes()())
        {

          if((playlist_treestore == playlists() . queue_playlist_treestore())
               ||
             (playlist_treestore == playlists() . library_playlist_treestore()))
          {

            it -> playlist_combobox_entry().set_sensitive(false);
            it -> remove_playlist_button().set_sensitive(false);

          }
          else
          {

            it -> playlist_combobox_entry().set_sensitive(true);
            it -> remove_playlist_button().set_sensitive(true);

          }

        }

                                                                                
        (*playlist_comboboxes_it) -> playlist_combobox_entry()
          .set_text(playlist_combobox_entry_ -> get_text()); 

        playlist_treestore -> set_name(playlist_combobox_entry_ -> get_text());
                                                                      

        playlist_comboboxes_it++;                                                 
                                                                                
      }      
                              
    }

  }



  // 
  playlist_comboboxes().set_on_playlist_combobox_changed_disabled(false);

}

void PlaylistComboBox::Playlist_Combo_Box_Entry_Changed()
{

  debug("Beginning of combobox entry changed");



  // 
  if((!constructed_)
       ||
     (playlist_comboboxes() . on_playlist_combobox_changed_disabled()))
  { 

    // 
    return;

  }



  // 
  playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(true);



  // 
  debug("Inside playlist combobox entry changed");



  // 
  Glib::ustring new_name_ustr = playlist_combobox_entry_ -> get_text();

  // 
  Glib::ustring old_name_ustr = config() . get("gui.playlist_combobox.active");



  // 
  if(new_name_ustr == "")
  {

    // 
    playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(false);



    // 
    return;

  }



  // 
  for(auto playlist_comboboxes_it : playlist_comboboxes()())
  {

    // 
    playlist_comboboxes_it -> playlist_combobox_entry()
      . set_text(new_name_ustr);

  }



  // 
  bool result
    = playlists() . database() . Rename_Playlist(old_name_ustr . c_str(), 
                                                 new_name_ustr . c_str());

  // 
  if(!result)
  {

    // 
    debug("The playlist was not able to be renamed in the database.");



    // 
    playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(false);

    // 
    playlist_combobox_entry_ -> set_text(old_name_ustr);



    // 
    return;

  }



  // 
  config() . set("gui.playlist_combobox.active", new_name_ustr);



  // 
  Gtk::TreeRow active_row = *(playlist_comboboxes() . active_row_it());

  // 
  active_row[playlist_combobox_column_record() . name_] = new_name_ustr;



  // 
  for(auto playlists_it : playlists()())
  {

    // 
    int count = 0;



    // 
    if(old_name_ustr == (playlists_it -> active_playlist_name()))
    {

      // 
      debug("Setting name of active playlist.");



      // 
      string setting_name = "gui.playlist.view.";

      // 
      setting_name += playlists_it -> playlist_view_name();

      // 
      setting_name += ".active";



      // 
      config()  . set(setting_name . c_str(), new_name_ustr);



      // 
      config() . write_file();

    }



    // 
    for(auto playlists_menu_it 
         : 
        playlists_it -> menu() . playlists_menu() . get_children())
    {

      // 
      if(count == playlist_comboboxes() . active_row_number())
      {

        // 
        debug("Renaming the playlist menu item");



        // 
        Gtk::RadioMenuItem* playlist_radio_menu_item_ptr
          = dynamic_cast<Gtk::RadioMenuItem*>(playlists_menu_it);



        // 
        playlist_radio_menu_item_ptr -> set_label(new_name_ustr);



        // 
        if(playlist_radio_menu_item_ptr -> get_active())
        {

          playlists_it -> menu() . change_playlist_menu_item()
            . set_label("Playlist: " + new_name_ustr);

        }

      }



      // 
      count++;

    }

  }



  // 
  list<Glib::RefPtr<PlaylistTreeStore>>::iterator active_row_treestore_it 
    = active_row[playlist_combobox_column_record() . playlist_treestore_it_];
  
  // 
  (*active_row_treestore_it) -> set_name(new_name_ustr); 



  // 
  playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(false);

}

void PlaylistComboBox::Remove_Playlist_Button_Pressed()
{

  // 
  playlists() . Open_Delete_Playlist_Dialog(true);

}

void PlaylistComboBox::Set_Row_Active(int row_number)
{

  playlist_combobox_ -> set_active(row_number); 

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::ComboBox& PlaylistComboBox::playlist_combobox()
{

  return *playlist_combobox_;

}

PlaylistComboBoxColumnRecord& PlaylistComboBox::playlist_combobox_column_record()
{

  return playlist_comboboxes() . column_record();

}

Gtk::Entry& PlaylistComboBox::playlist_combobox_entry()
{

  return *playlist_combobox_entry_;

}

Gtk::Button& PlaylistComboBox::remove_playlist_button()
{

  return *remove_playlist_button_;

}
