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
//  The developer(s) of the Moyσikh audio player hereby grant(s) permission
//  for non-GPL compatible GStreamer plugins to be used and distributed
//  together with GStreamer and Moyσikh. This permission is above and beyond
//  the permissions granted by the GPL license by which Moyσikh is covered.
//  If you modify this code, you may extend this exception to your version
//  of the code, but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version.
//
//  Libraries used by Moyσikh:
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





#include "PlaylistComboBox.h"





#include "PlaylistComboBoxColumnRecord.h"
#include "../../../Base.h"
#include "PlaylistComboBoxes.h"
#include "../Playlists/Playlists.h"
#include "../Playlists/PlaylistMenu.h"
#include "../Playlists/PlaylistsDatabase.h"
#include "../Playlists/PlaylistTreeStore.h"
#include "../../../Configuration/Configuration.h"
#include "../../GUI.h"

#include <gtkmm/combobox.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/button.h>
#include <gtkmm/radiomenuitem.h>





PlaylistComboBox::PlaylistComboBox
  (Base &base, 
   PlaylistComboBoxes& temp_playlist_comboboxes)
: GUIElement(base, temp_playlist_comboboxes())
, active_row_number_(1)
{

  constructed_ = false;

  playlist_combobox_box_ = Gtk::manage(new Gtk::Box);

  box().pack_start(*this, Gtk::PACK_EXPAND_WIDGET);

  this -> set_center_widget(*playlist_combobox_box_);

  playlist_combobox_ = Gtk::manage(new Gtk::ComboBox(true));

  add_playlist_button_ = Gtk::manage(new Gtk::Button);
  remove_playlist_button_ = Gtk::manage(new Gtk::Button);
  playlist_combobox_buttons_stackswitcher_
    = Gtk::manage(new Gtk::StackSwitcher);
  

  playlist_combobox_box_ -> pack_end(*playlist_combobox_buttons_stackswitcher_, 
                                     Gtk::PACK_SHRINK);
  playlist_combobox_buttons_stackswitcher_ 
    -> pack_end(*remove_playlist_button_, Gtk::PACK_SHRINK);
  playlist_combobox_buttons_stackswitcher_ 
    -> pack_end(*add_playlist_button_, Gtk::PACK_SHRINK);

  add_playlist_button_ -> set_margin_left(3);
  add_playlist_button_ 
    -> set_image_from_icon_name("list-add-symbolic",
                                Gtk::ICON_SIZE_SMALL_TOOLBAR);
  remove_playlist_button_ 
    -> set_image_from_icon_name("list-remove-symbolic",
                                Gtk::ICON_SIZE_SMALL_TOOLBAR);

  playlist_combobox_label_ = Gtk::manage(new Gtk::Label);

//  playlist_combobox_label_ -> set_label("Playlist: ");


  playlist_combobox_ -> set_model
    (temp_playlist_comboboxes.playlist_combobox_treestore());

  
  add_playlist_button_ -> signal_clicked().connect(
    sigc::mem_fun((*this), &PlaylistComboBox::On_Add_Playlist_Button_Pressed)); 

  remove_playlist_button_ -> signal_clicked().connect(
    sigc::mem_fun((*this), 
                  &PlaylistComboBox::On_Remove_Playlist_Button_Pressed)); 


  this -> set_margin_start(5);                                        
  this -> set_margin_end(5);                                          
  this -> set_margin_top(5);                                          
  this -> set_margin_bottom(5);                                       

  playlist_combobox_box_ -> pack_start(*playlist_combobox_label_, 
                                       Gtk::PACK_EXPAND_WIDGET);
  playlist_combobox_box_ -> set_center_widget(*playlist_combobox_); 

  playlist_combobox_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);

 
//  playlist_combobox_entry_ -> set_text("Playlist");


  playlist_combobox_ -> set_id_column(0);

  playlist_combobox_ 
    -> set_entry_text_column
         (temp_playlist_comboboxes.playlist_combobox_column_record().name_);

  playlist_combobox_entry_ = playlist_combobox_ -> get_entry(); 

  playlist_combobox_entry_ -> set_sensitive(true);


  auto temp_playlist_combobox_treestore 
    = (temp_playlist_comboboxes.playlist_combobox_treestore());

  playlist_combobox_ 
    -> set_active(++(temp_playlist_combobox_treestore -> children().begin()));

  playlist_combobox_ 
    -> set_entry_text_column
         (temp_playlist_comboboxes.playlist_combobox_column_record().name_);

  playlist_combobox_entry_ -> signal_activate().connect(
    sigc::mem_fun((*this), 
                  &PlaylistComboBox::On_Playlist_Combo_Box_Entry_Changed)); 

  playlist_combobox_ -> signal_changed()
    .connect(sigc::mem_fun((*this),
                           &PlaylistComboBox::On_Playlist_ComboBox_Changed)); 


  int active_row_number = 1;

  playlist_combobox_ -> set_active(active_row_number);


  if(active_row_number < 2)
  {

    playlist_combobox_entry() . set_sensitive(false);

    remove_playlist_button() . set_sensitive(false);

  }

  



  constructed_ = true;

}





void PlaylistComboBox::On_Playlist_ComboBox_Changed()
{                      

  if((!constructed_)
       ||
     (playlist_comboboxes().on_playlist_combobox_changed_disabled()))
  {


    return;

  }

  playlist_comboboxes().set_on_playlist_combobox_changed_disabled(true);


  //
  Gtk::TreeModel::iterator iter = playlist_combobox_ -> get_active();

  static Gtk::TreeModel::iterator playlist_combobox_iterator;

  if(iter)                                                                      
  {
      
    playlist_combobox_iterator = iter;
                              
    iter = playlist_combobox_iterator;
                                       
    Gtk::TreeModel::Row row = *iter;                                            

    if(row)                                                                     
    { 
                                                                          
      //Get the data for the selected row, using our knowledge of the tree      
      //model:                                                                  
      Glib::ustring name = row[playlist_combobox_column_record().name_];
  
      list<Playlist*>::iterator playlists_it 
        = playlists()().begin();


      list<PlaylistComboBox*>::iterator playlist_comboboxes_it 
        = playlist_comboboxes()().begin();

      std::list<Glib::RefPtr<PlaylistTreeStore>>::iterator treestore_it
        = row[playlist_combobox_column_record().playlist_treestore_it_];
    
      while(playlists_it != (playlists()().end()))
      { 
   
        if((*playlists_it) -> Locked())
        {
 
 
        }
        else
        {



//          (*playlists_it) -> menu().playlists_menu()
//            .set_active(playlist_combobox().get_active_row_number());
          if((playlist_combobox().get_active_row_number()) != -1)
          {

            active_row_number_ = playlist_combobox().get_active_row_number();

          }

          int count = 0;

          for(Gtk::Widget* menu_it 
               : 
              (*playlists_it) -> menu().playlists_menu().get_children())
          {

            if(count == active_row_number_)
            {


              Gtk::RadioMenuItem* temp_radio_menu_item_ptr
                = dynamic_cast<Gtk::RadioMenuItem*>(menu_it);

              temp_radio_menu_item_ptr -> set_active(true);

              (*playlists_it) -> menu().change_playlist_menu_item()
                .set_label("Playlist: " 
                            + playlist_combobox_entry_ -> get_text());

              break;

            }

            count++;

          }




        }

        playlists_it++;

      }

      while(playlist_comboboxes_it  != playlist_comboboxes()().end())                  
      {                                                                           

        for(auto it : playlist_comboboxes()())
        {

          if(((*treestore_it) == playlists() . queue_playlist_treestore())
               ||
             ((*treestore_it) == playlists() . library_playlist_treestore()))
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

        (*treestore_it) -> set_name(playlist_combobox_entry_ -> get_text());
                                                                      

        playlist_comboboxes_it++;                                                 
                                                                                
      }      
                              
    }                                                                           

  }                                                                             

  playlist_comboboxes().set_on_playlist_combobox_changed_disabled(false);


}

void PlaylistComboBox::On_Playlist_Combo_Box_Entry_Changed()
{

  // 
  if((!constructed_)
       ||
     (playlist_comboboxes().on_playlist_combobox_changed_disabled()))
  { 



    return;

  }



  // 
  playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(true);



  // 
  int row_number = 0;

  // 
  auto treestore_it 
    = playlist_comboboxes() . playlist_combobox_treestore()
                                -> children() . begin();

  while(row_number != active_row_number_)
  {

    row_number++;

    treestore_it++;

  }



  Gtk::TreeRow row = *treestore_it;
  
  Glib::ustring new_name = playlist_combobox_entry_ -> get_text();

  Glib::ustring name = row[playlist_combobox_column_record().name_];


  bool result 
    = playlists() . database() . Rename_Playlist(name . c_str(), 
                                                 new_name . c_str());

  if(!result)
  {

    playlist_comboboxes().set_on_playlist_combobox_changed_disabled(false);


    playlist_combobox_entry_ -> set_text(name);

    return;

   }


 

  row[playlist_combobox_column_record().name_] = new_name; 

  // 
  for(auto it : playlist_comboboxes()())
  {

    it -> playlist_combobox_entry().set_text(playlist_combobox_entry_ -> get_text());    

  }



  for(auto playlists_it : playlists()())
  {

    int count = 0;


    for(auto playlists_menu_it 
         : 
        playlists_it -> menu().playlists_menu().get_children())
    {

      if(count == active_row_number_)
      {

        Gtk::RadioMenuItem* temp_radio_menu_item_ptr
          = dynamic_cast<Gtk::RadioMenuItem*>(playlists_menu_it);

        temp_radio_menu_item_ptr -> set_label(new_name);

        if(temp_radio_menu_item_ptr -> get_active())
        {

          playlists_it -> menu().change_playlist_menu_item()
            .set_label("Playlist: " + new_name);

        }

      }

      count++;

    }

  }


  playlist_comboboxes().set_on_playlist_combobox_changed_disabled(false);


  }

void PlaylistComboBox::Set_Row_Active(int row_number)
{

  playlist_combobox_ -> set_active(row_number); 

}

void PlaylistComboBox::On_Add_Playlist_Button_Pressed()
{

  playlists() . Open_Create_Playlist_Dialog();

}

void PlaylistComboBox::On_Remove_Playlist_Button_Pressed()
{


  for(auto it : playlist_comboboxes()())
  {

    it -> remove_playlist_button().set_sensitive(false);

  }


  auto temp_active_row_treestore_it = active_row_treestore_it();

  Gtk::TreeIter temp_active_row_it;

  if(temp_active_row_treestore_it != playlists().playlist_treestores().end())
  {

    string playlist_name = (*temp_active_row_treestore_it) -> get_name();



    playlists() . database() . Delete_Playlist(playlist_name . c_str());



    playlists().playlist_treestores().erase(temp_active_row_treestore_it);

    temp_active_row_it = playlist_combobox_ -> get_active();

  }
  //Potential bug solver
  else
  {

    playlists().playlist_treestores()
      .erase(++(playlists().playlist_treestores().begin()));

    temp_active_row_it 
      = (++(playlist_comboboxes().playlist_combobox_treestore() 
                                    -> children().begin()));

    

  }
  
  playlist_comboboxes().Remove_Playlist(temp_active_row_it);





  auto library_playlist_treestore_it
    = playlists() .playlist_treestores() . begin();

  library_playlist_treestore_it++;

  for(auto it : playlist_comboboxes()())
  {

    if((active_row_treestore_it() == playlists() . playlist_treestores() . begin())
         ||
       (active_row_treestore_it() == library_playlist_treestore_it))
    {

      it -> remove_playlist_button().set_sensitive(false);
      it -> playlist_combobox_entry().set_sensitive(false);

    }
    else
    {

      it -> remove_playlist_button().set_sensitive(true);
      it -> playlist_combobox_entry().set_sensitive(true);

    }

  }


}





std::list<Glib::RefPtr<PlaylistTreeStore>>::iterator 
  PlaylistComboBox::active_row_treestore_it()
{

  if(!(playlist_combobox_ -> get_active()))
  {

    return playlists().playlist_treestores().end();

  }

  Gtk::TreeRow row = *(playlist_combobox_ -> get_active());


  std::list<Glib::RefPtr<PlaylistTreeStore>>::iterator active_row_treestore_it 
    = row[playlist_combobox_column_record().playlist_treestore_it_];


  if(active_row_treestore_it == playlists().playlist_treestores().end())
  {


  }

  return active_row_treestore_it;

}

PlaylistComboBoxColumnRecord& PlaylistComboBox::playlist_combobox_column_record()
{

  return playlist_comboboxes().playlist_combobox_column_record();

}
