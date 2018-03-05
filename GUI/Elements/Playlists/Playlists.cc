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

#include "../PlaylistComboBoxes/PlaylistComboBoxes.h"

#include "PlaylistColumnRecord.h"

#include "PlaylistCreateDialog.h"

#include "PlaylistMenu.h"

#include "PlaylistsDatabase.h"

#include "PlaylistTreeStore.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/dialog.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/treerowreference.h>

#include <memory>





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

, disable_on_selection_changed_(false)

, drag_occurring_(false)

, inserting_(false)

, new_selection_(false)

, skip_button_press_(false)

, drag_occurred_(false)



// Playlist Creation

, playlist_create_dialog_(new PlaylistCreateDialog(base_ref))



// Tracks

, empty_track_(new Track())



// Tree Row References

, last_non_queue_row_ref_(new Gtk::TreeRowReference)

, selected_row_ref_(new Gtk::TreeRowReference)

{

  debug("Beginning of Playlists constructor!");



  // 
  vector<string> playlist_names;

  // 
  database() . Playlist_Names(playlist_names);



  // 
  list<string> column_names
    {"track_number", "title", "artists", "album_artists", "album", "genres",
     "length", "date", "track_total", "bit_rate", "bit_depth", "sample_rate",
     "channels", "codec", "mime"};

  // 
  for(auto column_names_it : column_names)
  {

    // 
    string column_title_config_str = "gui.playlist.columns.";

    // 
    column_title_config_str += column_names_it;

    // 
    column_title_config_str += ".title";



    // 
    string title = config() . get(column_title_config_str);



    // 
    columns_ . emplace_back();

    // 
    columns_ . back() . title_ = title;

    // 
    columns_ . back() . name_ = column_names_it;

  }



  // Queue Playlist Creation
  {

    // Adds a new PlaylistTreeStore to the back of playlist_treestores_.
    playlist_treestores_
      . push_back(PlaylistTreeStore::create(base_ref, *playlist_column_record_));

    // Sets the new PlaylistTreeStore's name to Queue.
    playlist_treestores_ . back() -> set_name("Queue");



    // 
    vector<Track*>* tracks = new vector<Track*>;



    // 
    vector<int> ids;

    // 
    database() . Extract_Tracks("Queue", tracks, &ids);



    // 
    Append_Rows(tracks, playlist_treestores_ . back(), false, &ids);

  }



  // Library Playlist Creation
  {

    // Adds a new PlaylistTreeStore to the back of playlist_treestores_.
    playlist_treestores_
      . push_back(PlaylistTreeStore::create(base_ref, *playlist_column_record_));

    // Sets the new PlaylistTreeStore's name to Queue.
    playlist_treestores_ . back() -> set_name("Library");



    // 
    vector<Track*>* tracks = new vector<Track*>;



    // 
    vector<int> ids;

    // 
    database() . Extract_Tracks("Library", tracks, &ids);



    // 
    Append_Rows(tracks, playlist_treestores_ . back(), false, &ids);

  }



  // 
  for(auto it : playlist_names)
  {

    // 
    if((it == "Queue") || (it == "Library"))
    {

      continue;

    }



    // Adds a new PlaylistTreeStore to the back of playlist_treestores_.
    playlist_treestores_
      . push_back(PlaylistTreeStore::create(base_ref, *playlist_column_record_));



    // Sets the new PlaylistTreeStore's name to Queue.
    playlist_treestores_ . back() -> set_name(it . c_str());



    // 
    vector<Track*>* tracks = new vector<Track*>;



    // 
    vector<int> ids;

    // 
    database() . Extract_Tracks(it . c_str(), tracks, &ids);



    // 
    Append_Rows(tracks, playlist_treestores_ . back(), false, &ids);

  }



  // Sets queue_playlist_treestore_ to the new PlaylistTreeStore.
  queue_playlist_treestore_ = playlist_treestores_ . front();

  auto playlist_treestores_it = playlist_treestores_ . begin();

  playlist_treestores_it++;

  library_playlist_treestore_ = *playlist_treestores_it;

  selected_playlist_treestore_ = *playlist_treestores_it;

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
  playlist_comboboxes().Add_Playlist(name, playlist_treestores_it);



  // 
  return true;

}

void Playlists::Append_Rows
  (vector<Track*>* tracks, Glib::RefPtr<PlaylistTreeStore> playlist_treestore,
   bool append_to_database, vector<int>* ids)
{

  // 
  set_inserting(true);



  // 
  vector<int>::iterator ids_it;

  // 
  if(ids != nullptr)
  {

    // 
    ids_it = ids -> begin();

  }



  // 
  for(auto it : *tracks)
  { 

    // 
    Gtk::TreeRow row = *(playlist_treestore -> append());



    // 
    Fill_Row(row, shared_ptr<Track>(it));



    // 
    if(ids != nullptr)
    {

      // 
      row[playlist_column_record() . id_] = *ids_it;



      // 
      ids_it++;

    }

  }



  // 
  tracks -> clear();

  // 
  delete tracks;



  // 
  playback() . Reset_Track_Queue();



  // 
  if(append_to_database)
  {

    // 
    database() . Clear_Playlist(playlist_treestore -> get_name() . c_str());



    // 
    database()
      . Add_Tracks(playlist_treestore -> get_name() . c_str(),
                   playlist_treestore);

  }



  // 
  set_inserting(false);

}

void Playlists::Change_Track()
{

  debug("Inside Change_Track");



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
    playlists() . set_disable_on_selection_changed(true);


    // 
    for(auto it : playlists()())
    {


      if((it -> playlist_treestore()) == (playlists() . queue_playlist_treestore()))
      {

        //
        it -> playlist_treeselection() -> set_mode(Gtk::SELECTION_NONE);

      }

    }


    // 
    playlists() . queue_playlist_treestore()
      -> erase(first_in_playback_queue_row_it);


    for(auto it : playlists()())
    {

      if((it -> playlist_treestore()) == (playlists() . queue_playlist_treestore()))
      {

        it -> playlist_treeselection() -> set_mode(Gtk::SELECTION_MULTIPLE);

      }

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
      Select_Row(playing_row_ref());



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
    gui() . tagviews() . front() -> update_tags("Selected & Playing",
                                                playlists().playing_track());



    //
    debug("After cursor movement");

  }

  else
  {

    //
    gui() . tagviews() . front()
      -> update_tags("Playing", playlists() . playing_track());

  }



  //
  string filename_str = playlists() . playing_track() . filename() . raw();

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

void Playlists::Fill_Row
  (Gtk::TreeRow& new_tree_row, shared_ptr<Track> new_track_sptr)
{

  //
  int track_number = new_track_sptr -> track_number();

  if(track_number == 0)
  {

  }
  else
  {

    // 
    new_tree_row[playlist_column_record().track_num_col]
      = to_string(new_track_sptr -> track_number());

  }

  // 
  new_tree_row[playlist_column_record().track_num_int_col]
    = new_track_sptr -> track_number();



  //
  int track_total = new_track_sptr -> track_total();

  if(track_total == 0)
  {

  }
  else
  {

    // 
    new_tree_row[playlist_column_record().track_total_col]
      = to_string(new_track_sptr -> track_total());

  }

  // 
  new_tree_row[playlist_column_record().track_total_int_col]
    = new_track_sptr -> track_total();



  //
  int date = new_track_sptr -> date();

  if(date == 0)
  { 

  }
  else
  {

    // 
    new_tree_row[playlist_column_record().date_col] = to_string(date);

  }

  // 
  new_tree_row[playlist_column_record().date_int_col] = date;



  // 
  if(new_track_sptr -> title() . empty())
  {

    // 
    new_tree_row[playlist_column_record().title_col]
      = new_track_sptr -> filename();

  }

  // 
  else
  {

    // 
    new_tree_row[playlist_column_record().title_col]
      = new_track_sptr -> title();

  }



  // 
  Glib::ustring* album_artists = new_track_sptr -> album_artists_string();

  // 
  new_tree_row[playlist_column_record().album_artist_col] = *album_artists;

  // 
  delete album_artists;



  // 
  Glib::ustring* artists = new_track_sptr -> artists_string();

  // 
  new_tree_row[playlist_column_record().artist_col] = *artists;

  // 
  delete artists;



  //
  new_tree_row[playlist_column_record().album_col] = new_track_sptr -> album();



  // 
  Glib::ustring* genres = new_track_sptr -> genres_string();

  // 
  new_tree_row[playlist_column_record().genre_col] = *genres;

  // 
  delete genres;



  // 
  new_tree_row[playlist_column_record().length_col]
    = new_track_sptr -> length();

  // 
  new_tree_row[playlist_column_record().filename_col]
    = new_track_sptr -> filename();



  // 
  int bit_depth = new_track_sptr -> bit_depth();
  
  // 
  if(bit_depth == 0)
  {

    // 
    new_tree_row[playlist_column_record().bit_depth_col] = "-";

  }

  // 
  else
  {

    new_tree_row[playlist_column_record().bit_depth_col]
      = to_string(new_track_sptr -> bit_depth());

  }



  // 
  new_tree_row[playlist_column_record().bit_rate_col]
    = new_track_sptr -> bit_rate();



  // 
  new_tree_row[playlist_column_record().codec_col]
    = new_track_sptr -> codec();



  // 
  new_tree_row[playlist_column_record().mime_col]
    = new_track_sptr -> mime();



  // 
  new_tree_row[playlist_column_record().sample_rate_col]
    = new_track_sptr -> sample_rate();



  // 
  new_tree_row[playlist_column_record().channels_col]
    = new_track_sptr -> channels();



  //
  new_tree_row[playlist_column_record().track_col] = new_track_sptr;

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

  queue_playlist_treestore() -> clear();


}

void Playlists::Open_Create_Playlist_Dialog()
{

  playlist_create_dialog_ -> Run();

//  playlists().Add_Playlist("");



} 

void Playlists::Scroll_To_Row(Gtk::TreeRowReference desired_row_ref)
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

        Gtk::TreePath temp_tree_path(desired_row_ref . get_path());

        (*playlists_it) -> scroll_to_row(temp_tree_path);

      }

    }

    playlists_it++;

  }

  disable_on_selection_changed_ = false;


}

void Playlists::Select_Row(Gtk::TreeRowReference desired_row_ref)
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

        (*playlists_it) -> playlist_treeselection() -> unselect_all();

        (*playlists_it) -> set_cursor(desired_row_ref . get_path());

      }

    }

    playlists_it++;

  }

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

bool Playlists::disable_on_selection_changed()
{

  return disable_on_selection_changed_;

}

bool Playlists::drag_occurring()
{

  return drag_occurring_;

}

bool Playlists::inserting()
{

  return inserting_;

}

std::atomic<bool>& Playlists::new_selection()
{ 

  return new_selection_;

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

  if(selected_row_ref())
  {

    Gtk::TreeRow row
      = *(selected_row_ref() . get_model() -> get_iter(selected_row_ref() . get_path()));

    shared_ptr<Track> temp_track_ptr
      = row[playlist_column_record().track_col];

    return *temp_track_ptr;

  }

  else
  {

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

void Playlists::set_drag_occurring(bool new_value)
{

  drag_occurring_ = new_value;

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
