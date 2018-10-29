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

#include "Playlists.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Configuration/Configuration.h"

#include "../../../Playback/Playback.h"

#include "../../../Playback/TrackBin.h"

#include "../../../Scrobbling/Scrobbling.h"

#include "../../../Metadata/Track.h"

#include "../../GUI.h"

#include "../../Tagview.h"

#include "../FileChoosers/FileChoosers.h"

#include "../PlaylistComboBoxes/PlaylistComboBoxColumnRecord.h"

#include "../PlaylistComboBoxes/PlaylistComboBoxes.h"

#include "PlaylistColumnRecord.h"

#include "PlaylistChangesCancelDialog.h"

#include "PlaylistCreateDialog.h"

#include "PlaylistDeleteDialog.h"

#include "PlaylistMenu.h"

#include "PlaylistsDatabase.h"

#include "PlaylistTreeStore.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtk/gtk.h>

#include <glibmm/main.h>

#include <gtkmm/combobox.h>

#include <gtkmm/dialog.h>

#include <gtkmm/progressbar.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/treerowreference.h>

#include <memory>

#include <thread>





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

Playlists::Playlists(Base& base_ref)

// Inherited Class

: GUIElementList(base_ref)



// Columns

, playlist_column_record_(new PlaylistColumnRecord)



// Database

, database_(new PlaylistsDatabase(base_ref))



// Flags

, changing_track_(false)

, database_extraction_complete_(false)

, disable_on_selection_changed_(false)

, drag_occurred_(false)

, inserting_(false)

, new_selection_(false)

, rebuild_databases_(false)

, rebuilding_databases_(false)

, skip_button_press_(false)



// Playlist Creation

, playlist_changes_cancel_dialog_(new PlaylistChangesCancelDialog(base_ref))

, playlist_create_dialog_(new PlaylistCreateDialog(base_ref))

, playlist_delete_dialog_(new PlaylistDeleteDialog(base_ref))



// Status Bars

, extracting_playlists_(0)

, playlist_status_str_("")



// Tracks

, empty_track_(new Track())



// Tree Row References

, last_non_queue_row_ref_(new Gtk::TreeRowReference)

, selected_row_ref_(new Gtk::TreeRowReference)

{

  debug("Beginning of Playlists constructor!");



  // 
  playlist_status_str_mutex_ . lock();

  // 
  playlist_status_str_ = "Adding Extracted Tracks To Playlists";

  // 
  playlist_status_str_mutex_ . unlock();



  // 
  vector<string> playlist_names;

  // 
  database() . Playlist_Names(playlist_names);



  // 
  list<pair<string, bool>> column_names
    {{"album", true}, {"album_artists",true}, {"artists", true},
     {"bit_depth", false}, {"bit_rate", false}, {"channels", false}, 
     {"codec", false}, {"date", true}, {"disc_number", true},
     {"disc_total", true}, {"genres", true}, {"length", false},
     {"mime", false}, {"sample_rate", false}, {"title", true}, 
     {"track_number", true}, {"track_total", true}};



  // 
  for(auto column_names_it : column_names)
  {

    // 
    string column_title_config_str = "gui.playlist.columns.";

    // 
    column_title_config_str += column_names_it . first;

    // 
    column_title_config_str += ".title";



    // 
    string title = config() . get(column_title_config_str);



    // 
    columns_ . emplace_back();

    // 
    columns_ . back() . title_ = title;

    // 
    columns_ . back() . name_ = column_names_it . first;

    // 
    columns_ . back() . editable_ = column_names_it . second;

  }



  // Queue Playlist Creation
  {

    // Adds a new PlaylistTreeStore to the back of playlist_treestores_.
    playlist_treestores_
      . push_back(PlaylistTreeStore::create(base_ref,
                                            *playlist_column_record_));

    // Sets the new PlaylistTreeStore's name to Queue.
    playlist_treestores_ . back() -> set_name("Queue");



    // 
    queue_playlist_treestore_ = playlist_treestores_ . back();



    // 
    database() . Extract_Tracks(queue_playlist_treestore_);



    // 
    queue_playlist_treestore_ -> appending_rows_total() 
      = (playlist_treestores_ . back() -> add_track_queue() . size());



    // 
    extracting_playlists_++;

  }



  // Library Playlist Creation
  {

    // Adds a new PlaylistTreeStore to the back of playlist_treestores_.
    playlist_treestores_
      . push_back(PlaylistTreeStore::create(base_ref,
                                            *playlist_column_record_));

    // Sets the new PlaylistTreeStore's name to Library.
    playlist_treestores_ . back() -> set_name("Library");



    // 
    library_playlist_treestore_ = playlist_treestores_ . back();



    // 
    database() . Extract_Tracks(library_playlist_treestore_);



    // 
    library_playlist_treestore_ -> appending_rows_total()
      = (playlist_treestores_ . back() -> add_track_queue() . size());



    // 
    extracting_playlists_++;

  }



  // 
  for(auto it : playlist_names)
  {

    // 
    if((it == "Queue") || (it == "Library"))
    {

      // 
      continue;

    }



    // Adds a new PlaylistTreeStore to the back of playlist_treestores_.
    playlist_treestores_
      . push_back(PlaylistTreeStore::create(base_ref,
                                            *playlist_column_record_));

    // Sets the new PlaylistTreeStore's name to Queue.
    playlist_treestores_ . back() -> set_name(it);



    // 
    database() . Extract_Tracks(playlist_treestores_ . back());



    // 
    playlist_treestores_ . back() -> appending_rows_total() 
      = (playlist_treestores_ . back() -> add_track_queue() . size());



    // 
    extracting_playlists_++;

  }



  // 
  for(auto playlist_treestores_it : playlist_treestores())
  {

    // 
    Append_Rows(playlist_treestores_it, true);

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Playlists::~Playlists()
{

  // Deletes the memory at database_.
  delete database_;

  // Deletes the memory at empty_track_.
  delete empty_track_;

  // Deletes the memory at playlist_column_record_.
  delete playlist_column_record_;

  // Deletes the memory at selected_row_ref_.
  delete selected_row_ref_;

  // 
  delete playlist_create_dialog_;



  //
  for(auto playlists_it : (*this)())
  {

    // 
    list<string> column_order;



    // 
    for(auto columns_it : playlists_it -> get_columns())
    {

      // 
      string column_title = columns_it -> get_title();

      // 
      string column_name = playlists() . Find_Column_Name(column_title);



      // 
      column_order . push_back(column_name);



      // 
      string column_position_config_str
        = "gui.playlist.view.";

      // 
      column_position_config_str
        += playlists_it -> playlist_view_name();

      // 
      column_position_config_str
        += ".columns.";

      // 
      column_position_config_str
        += column_name + ".size";



      // 
      int width = columns_it -> get_fixed_width();

      // 
      config() . set(column_position_config_str, width);

    }



    // 
    string column_order_config_str
      = "gui.playlist.view.";

    // 
    column_order_config_str
      += playlists_it -> playlist_view_name();

    // 
    column_order_config_str
        += ".column_order";



    // 
    config() . set(column_order_config_str, column_order);

  }



  // 
  config() . write_file();

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

bool Playlists::Add_Playlist(const char* name)
{ 

  debug("Add_Playlist beginning!");



  // 
  if(!(database() . Create_Playlist(name)))
  {

    return false;

  }



  // Adds a new PlaylistTreeStore to the back of playlist_treestores_.
  playlist_treestores_
    . push_back(PlaylistTreeStore::create(base(), *playlist_column_record_));

  // Sets the name of the new PlaylistTreeStore.
  playlist_treestores_ . back() -> set_name(name);



  // Gets an iterator to the end of playlist_treestores_.
  auto playlist_treestores_it = playlist_treestores_ . end();

  // Decrements the iterator so it points to the last PlaylistTreeStore.
  playlist_treestores_it--;



  // Iterators through the Playlists to create a new RadioMenuItem for each.
  for(auto playlist_it : (*this)())
  {

    // Creates a new RadioMenuItem for the new PlaylistTreeStore.
    auto new_playlists_menu_radio_menu_item
      = new Gtk::RadioMenuItem
              (playlist_it -> menu() . playlists_menu_radio_button_group(),
               name);

    // Adds the new RadioMenuItem to playlist_radio_menu_items.
    playlist_it -> menu() . playlists_menu_radio_menu_items()
      . push_back(new_playlists_menu_radio_menu_item);

    // Adds the new RadioMenuItem to the PlaylistMenu.
    playlist_it -> menu() . playlists_menu()
      . append(*new_playlists_menu_radio_menu_item);

    // Sets the callback for the new RadioMenuItem's activate signal.
    new_playlists_menu_radio_menu_item -> signal_activate()
      . connect(sigc::mem_fun(*playlist_it, &Playlist::Change_Playlist));



    // Makes the new RadioMenuItem visible.
    new_playlists_menu_radio_menu_item -> show();

  } 



  // 
  playlist_comboboxes() . Add_Playlist(name, playlist_treestores_it);



  // 
  return true;

}

void Playlists::Append_Rows(Glib::RefPtr<PlaylistTreeStore> playlist_treestore,
                            bool database_extraction)
{

  // 
  static mutex append_rows_mutex;

  // 
  append_rows_mutex . lock();



  // 
  if(playlist_treestore -> appending())
  {

    // 
    append_rows_mutex . unlock();



    // 
    return;

  }



  // 
  if(rebuilding_databases())
  {

    // 
    database() . quit_rebuilding() = true;

    // 
    playlist_treestore -> rebuild_database() = true;



    // 
    sigc::connection program_conn = Glib::signal_timeout() . connect
    (

      // 
      [this, playlist_treestore]() -> bool
      { 

        // 
        if(rebuilding_databases())
        {

          // 
          return true;

        }



        // 
        Append_Rows(playlist_treestore);

        // 
        return false;

      },



      // 
      3, Glib::PRIORITY_HIGH_IDLE

    );



    // 
    append_rows_mutex . unlock();



    // 
    return;

  }



  // 
  playlist_treestore -> appending() = true;

  // 
  append_rows_mutex . unlock();



  // 
  sigc::connection program_conn = Glib::signal_timeout() . connect
  (

    // 
    [this, playlist_treestore, database_extraction]() -> bool
    { 

      // 
      if(playlist_treestore -> pause_appending())
      {

        // 
        return true;

      }

      // 
      else if((playlist_treestore -> add_track_queue() . empty()))
      {

        // 
        for(auto playlists_it : playlists()())
        {

          // 
          if(playlist_treestore == (playlists_it -> playlist_treestore()))
          {

              // 
              playlists_it -> progress_bar()
                . set_text("No Playlists Modifications Occurring");

              // 
              playlists_it -> progress_bar() . set_fraction(1);

          }

        }



        // 
        if(database_extraction)
        {

          // 
          extracting_playlists_--;

          // 
          playlist_treestore -> extraction_complete() = true;



          // 
          if(extracting_playlists_ <= 0)
          {

            // 
            database_extraction_complete_ = true;




            // 
            if(rebuild_databases_)
            {

              // 
              database() . Rebuild_Database();

            }

          }

        }

        // 
        else
        {

          // 
          if(rebuild_databases_)
          {

            // 
            database() . Rebuild_Database();

          }

        }



        // 
        playback() . Reset_Track_Queue();



        // 
        playlist_treestore -> appending() = false;

        // 
        playlist_treestore -> pause_changes() = false;



        // 
        for(auto file_chooser : file_choosers()())
        {

          // 
          if(playlist_treestore == playlists() . selected_playlist_treestore())
          {

            //
            if(!(playlist_treestore -> Changes_Occurring()))
            {

              // 
              file_chooser -> okay_button() . set_sensitive(true);

            }

          }

        }



        // 
        return false;

      }



      //  
      if(base() . quitting())
      {

        // 
        return false;

      }



      // 
      Gtk::TreeRow row = *(playlist_treestore -> append());



      // 
      shared_ptr<Track> new_track_sptr
        = playlist_treestore -> add_track_queue() . front() . second;

      // 
      int id = playlist_treestore -> add_track_queue() . front() . first;

      // 
      playlist_treestore -> add_track_queue() . pop_front();



      // 
      Fill_Row(row, new_track_sptr);



      // 
      row[playlist_column_record() . id_] = id;



      // 
      (playlist_treestore -> appending_rows_done())++;



      //
      string playlist_status
        = "Adding Extracted Tracks To Playlists: "
        + to_string(playlist_treestore -> appending_rows_done())
        + " / " + to_string(playlist_treestore -> appending_rows_total());

      // 
      double completion_fraction
        = (playlist_treestore -> appending_rows_done())
            / double(playlist_treestore -> appending_rows_total());



      // 
      for(auto playlists_it : playlists()())
      {

        // 
        if(playlists_it -> playlist_treestore() -> pause_appending())
        {

          // 
          continue;

        }

        // 
        if(playlist_treestore == (playlists_it -> playlist_treestore()))
        {

          // 
          playlists_it -> row_count_label()
            . set_text(to_string(playlist_treestore -> children() . size()));



          // 
          playlists_it -> progress_bar() . set_text(playlist_status);

          // 
          playlists_it -> progress_bar() . set_fraction(completion_fraction);

        }

        else
        {

          // 
          if(!(playlists_it -> Changes_Occurring()))
          {

            // 
            playlists_it -> progress_bar()
              . set_text("No Playlists Modifications Occurring");

            // 
            playlists_it -> progress_bar() . set_fraction(1);

          }

        }

      }



      // 
      return true;

    },



    // 
    3, Glib::PRIORITY_HIGH_IDLE

  );

} 

void Playlists::Change_Track()
{

  debug("Inside Change_Track");



  //
  string filename_str = playlists() . playing_track() . filename() . raw();



  // 
  if(playback() . delete_playback_queue_row())
  {

    // 
    Gtk::TreeIter first_in_playback_queue_row_it
      = playlists() . queue_playlist_treestore() -> children() . begin();



    // 
    Gtk::TreeRow first_in_playback_queue_row = *first_in_playback_queue_row_it;

    // 
    vector<int> id;

    // 
    id . push_back
           (first_in_playback_queue_row[playlist_column_record() . id_]);

    // 
    playlists() . database() . Delete_Rows("Queue", id);



    // 
    for(auto playlist : playlists()())
    {

      // 
      playlist -> selection_conn_ . block(true);

    }

    // 
    playlists() . set_disable_on_selection_changed(true);




    // 
    playlists() . queue_playlist_treestore()
      -> erase(first_in_playback_queue_row_it);



    // 
    for(auto playlist : playlists()())
    {

      if(playlist -> playlist_treestore() -> get_name() == "Queue")
      {

        // 
        playlist -> row_count_label() . set_text
          (to_string(playlist -> playlist_treestore() -> children() . size()));

      }



      // 
      playlist -> selection_conn_ . block(false);

    }

    // 
    playlists() . set_disable_on_selection_changed(false);



    // 
    playback() . delete_playback_queue_row() = false;

  }



  // 
  if(config() . get("playback.cursor_follows_playback"))
  {


    debug("Before Select_Track");



    // 
    if(playing_row_ref())
    {

      //
      Select_Row(playing_row_ref(), 
                 bool(config() . get("playback.selected_playlist_only")));



      // 
      if((playlists() . playing_playlist_treestore())
           != (playlists() . queue_playlist_treestore()))
      {

        //
        set_selected_row_ref(playing_row_ref());

      }

    }



    //
    playlists() . selected_playlist() . Add_Selected_Tracks_Times();



    //
    gui() . tagviews() . front() -> Update_Tags("Selected & Playing",
                                                playlists().playing_track());



    //
    debug("After cursor movement");

  }

  else
  {

    //
    gui() . tagviews() . front()
      -> Update_Tags("Playing", playlists() . playing_track());

  }



  //
  gui() . Load_Cover_Art(filename_str);



  //
  changing_track_.store(false, memory_order_relaxed);



  // 
  if(playing_playlist_treestore() != queue_playlist_treestore())
  {


    if(!playing_row_ref())
    {


    }


    // 
    *last_non_queue_row_ref_ = playing_row_ref();

  }

  // 
  else
  {

      // 
      playback() . delete_playback_queue_row() = true;

  }

}

void Playlists::Delete_Current_Playlist(bool delete_playlist_combobox_playlist)
{

  // Stores the name of the playlist to delete;
  string playlist_name;

  // 
  if(delete_playlist_combobox_playlist)
  {

    // 
    Glib::ustring* playlist_name_ustr_ptr
      = playlist_comboboxes() . active_row_name();



    // 
    playlist_name = *playlist_name_ustr_ptr;

    

    // 
    delete playlist_name_ustr_ptr;

  }

  // 
  else
  {

    // 
    playlist_name = selected_playlist_treestore() -> get_name();

  }



  // 
  if(delete_playlist_combobox_playlist)
  {

    // 
    if((*(playlist_comboboxes() . active_treestore_it())) -> Changes_Occurring())
    {

      // 
      return;

    }

  }

  // 
  else
  {

    // 
    if((*(selected_playlist() . playlist_treestore_it())) -> Changes_Occurring())
    {

      // 
      return;

    }

  }



  stringstream debug_ss;

  debug_ss << "Deleting playlist name: " << playlist_name;

  debug(debug_ss . str() . c_str());



  // Removes the playlist from the database.
  database() . Delete_Playlist(playlist_name . c_str());



  debug("After playlist database entry deleted.");



  // 
  auto active_combobox_row_it = playlist_comboboxes() . active_row_it();



  // 
  if(delete_playlist_combobox_playlist)
  {

    // Erases the 
    playlist_treestores()
      . erase(playlist_comboboxes() . active_treestore_it());

  }

  // 
  else
  {

    // Erases the 
    playlist_treestores()
      . erase(selected_playlist() . playlist_treestore_it());

  }



  // 
  playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(true);



  // 
  int count = 0;

  // 
  int deleted_combobox_row_number
    = playlist_comboboxes() . active_row_number();



  debug_ss . str("");

  debug_ss << "Active combobox row number: " << deleted_combobox_row_number;

  debug(debug_ss . str() . c_str());



  // 
  Glib::ustring active_combobox_row_name_str
    = config() . get("gui.playlist_combobox.active");



  // True when the playlist deing deleted and the combobox are the same.
  if((active_combobox_row_name_str == playlist_name)
        || delete_playlist_combobox_playlist)
  {

    debug("Playlist name is the same as the playlist combobox name.");



    // 
    auto previous_combobox_row_it = active_combobox_row_it;



    debug("After previous_combobox_row_it assignment.");



    // 
    previous_combobox_row_it--;



    debug("After previous_combobox_row_it decrement.");



    // 
    Glib::ustring previous_combobox_row_name_str
      = (*previous_combobox_row_it)
          [playlist_comboboxes() . column_record() . name_];



    debug_ss . str("");

    debug_ss << "Previous combobox row name str: "
             << previous_combobox_row_name_str;

    debug(debug_ss . str() . c_str());



    // 
    for(auto playlist_comboboxes_it : playlist_comboboxes()())
    {

      // 
      playlist_comboboxes_it -> playlist_combobox()
        . set_active(previous_combobox_row_it);

    }

    // 
    playlist_comboboxes()
      . set_active_row_number(deleted_combobox_row_number - 1);



    debug("Setting all comboboxes to the new setting");



    // 
    config() . set("gui.playlist_combobox.active",
                   previous_combobox_row_name_str);



    // 
    playlist_comboboxes() . treestore() -> erase(active_combobox_row_it);



    debug("After combobox row deletion.");

   }

  // True when the playlist being deleted and the combobox are not the same.
  else
  {

    debug("Playlist name is NOT the same as the playlist combobox name.");



    // 
    auto playlist_comboboxes_treestore_it 
      = playlist_comboboxes() . treestore() -> children() . begin();



    // 
    while(playlist_comboboxes_treestore_it
            != playlist_comboboxes() . treestore() -> children() . end())
    {

      // 
      Glib::ustring deleted_combobox_row_name_str
        = (*playlist_comboboxes_treestore_it)
            [playlist_comboboxes() . column_record() . name_];



      // 
      if(playlist_name == deleted_combobox_row_name_str)
      {

        // 
        playlist_comboboxes() . treestore()
          -> erase(playlist_comboboxes_treestore_it);



        // 
        int active_combobox_row_number = deleted_combobox_row_number;

        // 
        deleted_combobox_row_number = count;

        // 
        count = 0;
  


        // 
        if(deleted_combobox_row_number < active_combobox_row_number)
        {

          // 
          playlist_comboboxes()
            . set_active_row_number(active_combobox_row_number - 1);

        }



        // 
        break;

      }



      // 
      playlist_comboboxes_treestore_it++;

      // 
      count++;

    } 

  }



  debug("Removing Playlist Menu entry and changing playlist one back");



  // 
  for(auto playlists_it : playlists()())
  {

    // 
    auto playlists_menu_radio_menu_items_it
      = playlists_it -> menu() . playlists_menu_radio_menu_items() . begin();



    // 
    for(auto menu_it
          : playlists_it -> menu() . playlists_menu() . get_children())
    {

      // 
      if(count == deleted_combobox_row_number)
      {

        debug_ss . str("");

        debug_ss << "Deleted menu row number: " << count;

        debug(debug_ss . str() . c_str());




        // 
        playlists_it -> menu() . playlists_menu() . remove(*menu_it);

        // 
        auto previous_menu_it = playlists_menu_radio_menu_items_it;

        // 
        previous_menu_it--;

        // 
        (*previous_menu_it) -> set_active(true);

        // 
        playlists_it -> menu() . playlists_menu_radio_menu_items()
          . erase(playlists_menu_radio_menu_items_it);



        // 
        break;

      }



      // 
      count++;

      // 
      playlists_menu_radio_menu_items_it++;

    }



    //
    count = 0;

  }



  // 
  auto selected_playlist_treestore_it
    = selected_playlist() . playlist_treestore_it();

  // 
  auto library_playlist_treestore_it = playlist_treestores() . begin();

  // 
  library_playlist_treestore_it++;



  // 
  for(auto playlist_comboboxes_it : playlist_comboboxes()())
  { 

    // 
    if((selected_playlist_treestore_it == playlists() . playlist_treestores() . begin())
         ||
       (selected_playlist_treestore_it == library_playlist_treestore_it))
    {

      // 
      playlist_comboboxes_it
        -> remove_playlist_button() . set_sensitive(false);

      // 
      playlist_comboboxes_it
        -> playlist_combobox_entry() . set_sensitive(false);

    }

    // 
    else
    {

      // 
      playlist_comboboxes_it
        -> remove_playlist_button() . set_sensitive(true);

      // 
      playlist_comboboxes_it
        -> playlist_combobox_entry() . set_sensitive(true);

    }

  }



  // 
  playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(false);

}

bool Playlists::Editable_Column(const char* column_name)
{

  // 
  for(auto column : columns())
  {

    // 
    if(column_name == column . name_)
    {

      // 
      if(column . editable_)
      {

        // 
        return true;

      }

    }

  }



  // 
  return false;

}

void Playlists::Fill_Row
  (Gtk::TreeRow& new_tree_row, shared_ptr<Track> new_track_sptr)
{

  //
  int track_number = new_track_sptr -> track_number();

  // 
  if(track_number == 0)
  {

  } 

  // 
  else
  { 

    // 
    new_tree_row[playlist_column_record() . track_number_]
      = to_string(new_track_sptr -> track_number());

  }

  // 
  new_tree_row[playlist_column_record() . track_number_int_]
    = new_track_sptr -> track_number();



  //
  int track_total = new_track_sptr -> track_total();

  // 
  if(track_total == 0)
  { 

  }

  // 
  else
  { 

    // 
    new_tree_row[playlist_column_record() . track_total_]
      = to_string(new_track_sptr -> track_total());

  }

  // 
  new_tree_row[playlist_column_record() . track_total_int_]
    = new_track_sptr -> track_total();



  //
  int date = new_track_sptr -> date();

  // 
  if(date == 0)
  { 

  } 

  // 
  else
  { 

    // 
    new_tree_row[playlist_column_record() . date_] = to_string(date);

  }

  // 
  new_tree_row[playlist_column_record() . date_int_] = date;



  //
  int disc_number = new_track_sptr -> disc_number();

  // 
  if(disc_number == 0)
  { 

  } 

  // 
  else
  {  

    // 
    new_tree_row[playlist_column_record() . disc_number_]
      = to_string(disc_number);

  }

  // 
  new_tree_row[playlist_column_record() . disc_number_int_] = disc_number;



  //
  int disc_total = new_track_sptr -> disc_total();

  // 
  if(disc_total == 0)
  { 

  } 

  // 
  else
  {  

    // 
    new_tree_row[playlist_column_record() . disc_total_]
      = to_string(disc_total);

  }

  // 
  new_tree_row[playlist_column_record() . disc_total_int_] = disc_total;



  // 
  if(new_track_sptr -> title() . empty())
  {

    // 
    new_tree_row[playlist_column_record() . title_]
      = new_track_sptr -> filename();

  }

  // 
  else
  {

    // 
    new_tree_row[playlist_column_record() . title_]
      = new_track_sptr -> title();

  }



  // 
  Glib::ustring* album_artists = new_track_sptr -> album_artists_string();

  // 
  new_tree_row[playlist_column_record() . album_artists_] = *album_artists;

  // 
  delete album_artists;



  // 
  Glib::ustring* artists = new_track_sptr -> artists_string();

  // 
  new_tree_row[playlist_column_record() . artists_] = *artists;

  // 
  delete artists;



  //
  new_tree_row[playlist_column_record() . album_] = new_track_sptr -> album();



  // 
  Glib::ustring* genres = new_track_sptr -> genres_string();

  // 
  new_tree_row[playlist_column_record() . genres_] = *genres;

  // 
  delete genres;



  // 
  new_tree_row[playlist_column_record().length_]
    = new_track_sptr -> length();

  // 
  new_tree_row[playlist_column_record() . filename_]
    = new_track_sptr -> filename();



  // 
  int bit_depth = new_track_sptr -> bit_depth();
  
  // 
  if(bit_depth == 0)
  {

    // 
    new_tree_row[playlist_column_record() . bit_depth_] = "-";

  }

  // 
  else
  {

    new_tree_row[playlist_column_record() . bit_depth_]
      = to_string(new_track_sptr -> bit_depth());

  }



  // 
  new_tree_row[playlist_column_record(). bit_rate_]
    = new_track_sptr -> bit_rate();



  // 
  new_tree_row[playlist_column_record() . codec_]
    = new_track_sptr -> codec();



  // 
  new_tree_row[playlist_column_record() . mime_]
    = new_track_sptr -> mime();



  // 
  new_tree_row[playlist_column_record() . sample_rate_]
    = new_track_sptr -> sample_rate();



  // 
  new_tree_row[playlist_column_record() . channels_]
    = new_track_sptr -> channels();



  //
  new_tree_row[playlist_column_record() . track_] = new_track_sptr;

}

string Playlists::Find_Column_Name(string& column_title)
{ 

  // 
  for(auto columns_it : columns_)
  {

    // 
    if((columns_it . title_) == column_title)
    { 

      // 
      return columns_it . name_;

    }

  }



  // 
  return "";

}

string Playlists::Find_Column_Title(string& column_name)
{ 

  // 
  return Find_Column_Title(column_name . c_str());

}

string Playlists::Find_Column_Title(const char* column_name)
{ 

  // 
  for(auto columns_it : columns_)
   {

    // 
    if((columns_it . name_) == column_name)
    {  

      // 
      return columns_it . title_;

    }

  }



  // 
  return "";

}

void Playlists::Flush_Playback_Queue()
{

  // 
  queue_playlist_treestore() -> clear();

}

void Playlists::Open_Changes_Cancel_Dialog(bool release_twice)
{ 

  // 
  playlist_changes_cancel_dialog_ -> Run(release_twice);

} 

void Playlists::Open_Create_Playlist_Dialog()
{

  // 
  playlist_create_dialog_ -> Run();

} 

void Playlists::Open_Delete_Playlist_Dialog
  (bool delete_playlist_combobox_playlist)
{ 

  // 
  playlist_delete_dialog_ -> Run(delete_playlist_combobox_playlist);

} 

bool Playlists::Rename_Playlist
  (Glib::RefPtr<PlaylistTreeStore> playlist_treestore,
   std::string& new_playlist_name)
{

  // 
  playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(true);



  // 
  debug("Inside playlist combobox entry changed");



  // 
  Glib::ustring old_name_ustr = playlist_treestore -> get_name();



  // 
  if(new_playlist_name == "")
  {

    // 
    playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(false);



    // 
    return false;

  }



  // 
  bool result
    = playlists() . database() . Rename_Playlist(old_name_ustr . c_str(), 
                                                 new_playlist_name . c_str());

  // 
  if(!result)
  {

    // 
    debug("The playlist was not able to be renamed in the database.");



    // 
    playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(false);



    // 
    return false;

  }



  // 
  if(playlist_treestore == (*(playlist_comboboxes() . active_treestore_it())))
  {

    // 
    for(auto playlist_combobox : playlist_comboboxes()())
    {

      // 
      playlist_combobox -> playlist_combobox_entry()
        . set_text(new_playlist_name);

    }



    // 
    config() . set("gui.playlist_combobox.active", new_playlist_name);



    // 
    Gtk::TreeRow active_row = *(playlist_comboboxes() . active_row_it());

    // 
    active_row[playlist_comboboxes() . column_record() . name_] = new_playlist_name;

  }

  // 
  else
  {

    // 
    auto playlist_comboboxes_treestore = playlist_comboboxes() . treestore();



    // 
    for(auto row : playlist_comboboxes_treestore -> children())
    {

      // 
      string row_name = Glib::ustring(row[playlist_comboboxes() . column_record() . name_]);

      // 
      if(row_name == old_name_ustr)
      {

        // 
        row[playlist_comboboxes() . column_record() . name_]
          = new_playlist_name;



        // 
        break;

      }

    }

  }



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
      playlists_it -> name_label() . set_text(new_playlist_name);



      // 
      string setting_name = "gui.playlist.view.";

      // 
      setting_name += playlists_it -> playlist_view_name();

      // 
      setting_name += ".active";



      // 
      config()  . set(setting_name . c_str(), new_playlist_name);



      // 
      config() . write_file();

    }



    // 
    for(auto playlists_menu_it 
         : 
        playlists_it -> menu() . playlists_menu() . get_children())
    {

      // 
      Gtk::RadioMenuItem* playlist_radio_menu_item_ptr
        = dynamic_cast<Gtk::RadioMenuItem*>(playlists_menu_it);



      // 
      string menu_playlist_name
        = playlist_radio_menu_item_ptr -> get_label();



      // 
      if(menu_playlist_name == old_name_ustr)
      {

        // 
        playlist_radio_menu_item_ptr -> set_label(new_playlist_name);

        // 
        playlists_it -> menu() . change_playlist_menu_item()
          . set_label("Playlist: " + new_playlist_name);

      }

    }

  }



  // 
  playlist_treestore -> set_name(new_playlist_name); 



  // 
  playlist_comboboxes() . set_on_playlist_combobox_changed_disabled(false);



  // 
  return true;

}

void Playlists::Scroll_To_Row(Gtk::TreeRowReference desired_row_ref,
                              bool only_use_selected_playlist_view)
{

  // 
  disable_on_selection_changed_ = true;



  // 
  list<Playlist*>::iterator playlists_it = (*this)().begin();

  // 
  Glib::RefPtr<PlaylistTreeStore> row_playlist_treestore
    = Glib::RefPtr<PlaylistTreeStore>::cast_dynamic
        (desired_row_ref . get_model());



  // 
  if(only_use_selected_playlist_view)
  {

    // 
    if((selected_playlist() . playlist_treestore()) == row_playlist_treestore)
    {

      // 
      if(!(desired_row_ref . is_valid()))
      {


      } 

      // 
      else
      { 

        // 
        selected_playlist() . scroll_to_row(desired_row_ref . get_path());

       }

     } 

  }

  // 
  else
   {

    // 
    while(playlists_it != ((*this)().end()))
    {
  
      // 
      if(((*playlists_it) -> playlist_treestore()) == row_playlist_treestore)
      {
  
        // 
        if(!(desired_row_ref . is_valid()))
        {
  
  
        } 
  
        // 
        else
        { 
  
          // 
          (*playlists_it) -> scroll_to_row(desired_row_ref . get_path());
  
        }
  
      } 
 

 
      // 
      playlists_it++;
  
    }

  }



  // 
  disable_on_selection_changed_ = false;

}

void Playlists::Select_Row(Gtk::TreeRowReference desired_row_ref,
                           bool only_use_selected_playlist_view)
{

  // 
  disable_on_selection_changed_ = true;



  // 
  list<Playlist*>::iterator playlists_it = (*this)().begin();

  // 
  Glib::RefPtr<PlaylistTreeStore> row_playlist_treestore
    = Glib::RefPtr<PlaylistTreeStore>::cast_dynamic
        (desired_row_ref . get_model());



  // 
  if(only_use_selected_playlist_view)
  {

    // 
    if((selected_playlist() . playlist_treestore()) == row_playlist_treestore)
    {

      // 
      if(!(desired_row_ref . is_valid()))
      {


      } 

      // 
      else
      { 

        // 
        selected_playlist() . playlist_treeselection() -> unselect_all();



        // 
        selected_playlist() . set_cursor(desired_row_ref . get_path());

       }

     } 

  }

  // 
  else
   {

    // 
    while(playlists_it != ((*this)().end()))
    {
  
      // 
      if(((*playlists_it) -> playlist_treestore()) == row_playlist_treestore)
      {
  
        // 
        if(!(desired_row_ref . is_valid()))
        {
  
  
        } 
  
        // 
        else
        { 
  
          // 
          (*playlists_it) -> playlist_treeselection() -> unselect_all();
  
  
  
          // 
          (*playlists_it) -> set_cursor(desired_row_ref . get_path());
  
        }
  
      } 


 
      // 
      playlists_it++;
  
    }

  }



  // 
  disable_on_selection_changed_ = false;

}

void Playlists::Unselect_Row(Gtk::TreeRowReference desired_row_ref)
{


  disable_on_selection_changed_ = true;

  list<Playlist*>::iterator playlists_it = (*this)().begin();

  Glib::RefPtr<PlaylistTreeStore> row_playlist_treestore
    = Glib::RefPtr<PlaylistTreeStore>::cast_dynamic(desired_row_ref . get_model());




  while(playlists_it != ((*this)().end()))
  {

    if(((*playlists_it) -> playlist_treestore()) == row_playlist_treestore)
    {

      if(!(desired_row_ref . is_valid()))
      {


      }
      else
      {

        (*playlists_it) -> playlist_treeselection()
          -> unselect(desired_row_ref . get_path());

      }

    }


    playlists_it++;

  }

  disable_on_selection_changed_ = false;


}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

//           //
// Clipboard //////////////////////////////////////////////////////////////////
//           //

list<shared_ptr<Track>>& Playlists::clipboard_tracks()
{

  return clipboard_tracks_;

}





//         //
// Columns ////////////////////////////////////////////////////////////////////
//         //

std::list<PlaylistColumn>& Playlists::columns()
{

  return columns_;

}

PlaylistColumnRecord& Playlists::playlist_column_record()
{

  return *playlist_column_record_;

}





//          //
// Database ///////////////////////////////////////////////////////////////////
//          //

PlaylistsDatabase& Playlists::database()
{

  return *database_;

}





//       //
// Flags //////////////////////////////////////////////////////////////////////
//       //

std::atomic<bool>& Playlists::changing_track()
{

  return changing_track_;

}

std::atomic<bool>& Playlists::database_extraction_complete()
{

  return database_extraction_complete_;

}

bool Playlists::disable_on_selection_changed()
{

  return disable_on_selection_changed_;

}

bool Playlists::inserting()
{

  return inserting_;

}

std::atomic<bool>& Playlists::new_selection()
{ 

  return new_selection_;

}

std::atomic<bool>& Playlists::rebuild_databases()
{

  return rebuild_databases_;

}

std::atomic<bool>& Playlists::rebuilding_databases()
{ 

  return rebuilding_databases_;

}

bool Playlists::skip_button_press()
{

  return skip_button_press_;

}





//           //
// Playlists //////////////////////////////////////////////////////////////////
//           //

Playlist& Playlists::selected_playlist()
{

  return *selected_playlist_;

}





//        //
// Tracks /////////////////////////////////////////////////////////////////////
//        //

Track& Playlists::empty_track()
{

  return *empty_track_;

}

Track& Playlists::next_track()
{

  if(playback().track_queue().size() < 2)
  {

    return *empty_track_;

  }

  auto track_queue_it = playback().track_queue().begin();

  track_queue_it++;

  return (*track_queue_it) -> track();

}

Track& Playlists::playing_track()
{

  if(playback().track_queue().empty())
  {

    return *empty_track_;

  }

  return playback().track_queue().front() -> track();

}

Track& Playlists::previous_track()
{

   return *empty_track_;

}

Track& Playlists::queue_track()
{

   return *empty_track_;

}

Track& Playlists::selected_track()
{

  // 
  if(selected_row_ref())
  {

    // 
    Gtk::TreeRow row
      = *(selected_row_ref() . get_model()
            -> get_iter(selected_row_ref() . get_path()));

    // 
    shared_ptr<Track> temp_track_ptr
      = row[playlist_column_record() . track_];



    // 
    return *temp_track_ptr;

  }

  // 
  else
  { 

   // 
   return *empty_track_;

  }

}





//                   //
// TreeRowReferences //////////////////////////////////////////////////////////
//                   //

Gtk::TreeRowReference Playlists::last_non_queue_row_ref()
{ 

  // 
  if(!(*last_non_queue_row_ref_))
  { 

    return Gtk::TreeRowReference();

  }



  // 
  return *last_non_queue_row_ref_;

}

Gtk::TreeRowReference Playlists::next_playing_row_ref()
{

  if(playback().track_queue().size() < 2)
  {

    return Gtk::TreeRowReference();

  }

  auto track_queue_it = playback().track_queue().begin();

  track_queue_it++;

  return (*track_queue_it) -> row_ref();

}

Gtk::TreeRowReference Playlists::playing_row_ref()
{

  if(playback().track_queue().empty())
  {

    return Gtk::TreeRowReference();

  }



  // 
  Gtk::TreeRowReference temp_row_ref = playback().track_queue().front() -> row_ref();



  // 
  if(!temp_row_ref)
  {

    return Gtk::TreeRowReference();

  }

  

  return temp_row_ref;

}

Gtk::TreeRowReference Playlists::selected_row_ref()
{

  // 
  if(!(*selected_row_ref_))
  { 

    // 
    return Gtk::TreeRowReference();

  }



  // 
  return *selected_row_ref_;

}





//            //
// TreeStores /////////////////////////////////////////////////////////////////
//            //

Glib::RefPtr<PlaylistTreeStore> Playlists::hovering_playlist_treestore()
{

  return hovering_playlist_treestore_;

}

Glib::RefPtr<PlaylistTreeStore> Playlists::library_playlist_treestore()
{

  return library_playlist_treestore_;

}

Glib::RefPtr<PlaylistTreeStore> Playlists::playing_playlist_treestore()
{

  if(playing_row_ref())
  {


    // 
    Glib::RefPtr<PlaylistTreeStore> new_playing_playlist_treestore
      = Glib::RefPtr<PlaylistTreeStore>
          ::cast_dynamic(playing_row_ref() . get_model());

    // 
    return new_playing_playlist_treestore;

  }



  // 
  return Glib::RefPtr<PlaylistTreeStore>();

}

std::list<Glib::RefPtr<PlaylistTreeStore>>& Playlists::playlist_treestores()
{

  return playlist_treestores_;

}

Glib::RefPtr<PlaylistTreeStore> Playlists::queue_playlist_treestore()
{

  return queue_playlist_treestore_;

}

Glib::RefPtr<PlaylistTreeStore> Playlists::selected_playlist_treestore()
{

  return selected_playlist_ -> playlist_treestore();

}





//         //
//         //
// Setters ////////////////////////////////////////////////////////////////////
//         //
//         //

//       //
// Flags //////////////////////////////////////////////////////////////////////
//       //

void Playlists::set_disable_on_selection_changed(bool new_value)
{

  disable_on_selection_changed_ = new_value;

}

void Playlists::set_inserting(bool new_value)
{

  inserting_ = new_value;

}

void Playlists::set_skip_button_press(bool new_value)
{

  skip_button_press_ = new_value;

}





//           //
// Playlists //////////////////////////////////////////////////////////////////
//           //

void Playlists::set_selected_playlist(Playlist& new_selected_playlist)
{

  selected_playlist_ = &new_selected_playlist;

}





//                   //
// TreeRowReferences //////////////////////////////////////////////////////////
//                   //

void Playlists::set_last_non_queue_row_ref(Gtk::TreeRowReference new_row_ref)
{ 

  *last_non_queue_row_ref_ = new_row_ref;

}

void Playlists::set_selected_row_ref(Gtk::TreeRowReference new_row_ref)
{

  *selected_row_ref_ = new_row_ref;

}





//                //
// TreeTreeStores /////////////////////////////////////////////////////////////
//                //

void Playlists::set_hovering_playlist_treestore
  (const Glib::RefPtr<PlaylistTreeStore>& new_treestore)
{

  hovering_playlist_treestore_ = new_treestore;

}
