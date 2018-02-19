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

#include "Playlist.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../../Metadata/Metadata.h"

#include "../../../Playback/Playback.h"

#include "../../../Playback/TrackBin.h"

#include "../../../TimeConversion.h"

#include "../../../Metadata/Track.h"

#include "../../GUI.h"

#include "../PlaylistComboBoxes/PlaylistComboBox.h"

#include "PlaylistColumnRecord.h"

#include "PlaylistMenu.h"

#include "Playlists.h"

#include "PlaylistsDatabase.h"

#include "PlaylistTreeStore.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gdkmm/display.h>

#include <gdkmm/device.h>

#include <gdkmm/devicemanager.h>

#include <gdkmm/window.h>

#include <gtkmm/box.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/scrolledwindow.h>

#include <gtkmm/treepath.h>

#include <gtkmm/treerowreference.h>

#include <gtkmm/treeselection.h>

#include <pangomm/fontdescription.h>





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

Playlist::Playlist(Base& base_ref, Playlists& playlists_ref,
                   const char* new_playlist_view_name)

// Inherited Class

: GUIElement(base_ref, playlists_ref())



// General

, playlist_view_name_(new_playlist_view_name)


// Flags

, clipboard_event_(false)

, constructed_(false)

, deleting_(false)

, selecting_flag_(false)



// GUI Parts

, filename_box_(Gtk::manage(new Gtk::Box))

, filename_frame_(Gtk::manage(new Gtk::Frame))

, filename_label_(Gtk::manage(new Gtk::Label))

, playlist_frame_(Gtk::manage(new Gtk::Frame))

, playlist_scrolled_window_(Gtk::manage(new Gtk::ScrolledWindow))



// Drag and Drop

, dest_row_drop_position_(0)

, dest_tree_row_ref_(new Gtk::TreeRowReference)

, drag_tree_row_ref_(new Gtk::TreeRowReference)

, handle_multirow_drag_(false)

, previous_button_press_event_time_(0L)

, x_click_(0)

, y_click_(0)

{

  // 
  playlists_ref() . push_back(this);



  // Sets the orientation of the GUIElement box() as vertical.
  box() . set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Puts the playlist_frame_ in the top of the GUIElement box().
  box() . pack_start(*playlist_frame_, Gtk::PACK_EXPAND_WIDGET);

  // Puts the filename_frame_ in the bottom of the GUIElement box().
  box() . pack_end(*filename_frame_, Gtk::PACK_SHRINK);

  // Adds a bottom margin to GUIElement box.
  box() . set_margin_bottom(5);



  // Adds the filename_box_ to the filename_frame_.
  filename_frame_ -> add(*filename_box_);

  // Sets filename_frame_ to expand horizontally.
  filename_frame_ -> set_hexpand(true);

  // Adds filename_label_ as a center widget in filename_box_.
  filename_box_ -> set_center_widget(*filename_label_);

  // Sets the filename label to line wrap if not enough room is available.
  filename_label_ -> set_line_wrap(true);

  // Sets the max lines to wrap before abbreviating with an ellipsis.
  filename_label_ -> set_lines(2);



  // Sets the behaviour of the scrobbable window of the playlist to appear
  // automatically when needed.
  playlist_scrolled_window_ -> set_policy(Gtk::POLICY_AUTOMATIC,
                                          Gtk::POLICY_AUTOMATIC);

  // Adds the TreeView to playlist_scrolled_window_.
  playlist_scrolled_window_ -> add(*this);



  // Add's playlist_scrolled_window to playlist_frame_.
  playlist_frame_ -> add(*playlist_scrolled_window_);



  debug("Before model set!");



  // 
  string setting_name = "gui.playlist.view.";

  // 
  setting_name += playlist_view_name_;

  // 
  setting_name += ".active";



  // 
  string playlist_treestore_name = config() . get(setting_name . c_str());



  // Assigns the TreeModel to the TreeView.
  for(auto playlist_treestores_it : playlists_ref . playlist_treestores())
  {

    // 
    if((playlist_treestores_it -> get_name()) == playlist_treestore_name)
    {

      // 
      set_playlist_treestore(playlist_treestores_it);

    }

  }



  debug("After model set!");



  // Makes the TreeView's Columns headers clickable.
  set_headers_clickable(true);

  // Set the rows to be a source of dragging.
  enable_model_drag_source();

  // Sets the rows as being a source for dropping.
  enable_model_drag_dest();



  // Suggests to draw rows contrastinly, but it depends on the theme engine.
  set_rules_hint(true);



  // 
  list<string> column_order_list;

  // 
  string column_order_str = "gui.playlist.view.";

  // 
  column_order_str += playlist_view_name_;

  // 
  column_order_str += ".column_order";

  // 
  config() . get_list(column_order_str, column_order_list);



  // 
  for(auto column_order_list_it : column_order_list)
  {

    // 
    string title = playlists_ref . Find_Column_Title(column_order_list_it);

    // 
    Gtk::TreeViewColumn* new_column;



    //
    if(column_order_list_it == "track_number")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . track_num_col));

    }

    //
    else if(column_order_list_it == "title")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . title_col));

    }

    //
    else if(column_order_list_it == "artists")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . artist_col));

    }

    //
    else if(column_order_list_it == "album_artists")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . album_artist_col));

    }

    //
    else if(column_order_list_it == "album")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . album_col));

    }

    //
    else if(column_order_list_it == "genres")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . genre_col));

    }
    
    //
    else if(column_order_list_it == "length")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . length_col));

    }

    //
    else if(column_order_list_it == "date")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . date_col));

    }

    //
    else if(column_order_list_it == "track_total")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . track_total_col));

    }
    
    //
    else if(column_order_list_it == "bit_rate")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . bit_rate_col));

    }

    //
    else if(column_order_list_it == "bit_depth")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . bit_depth_col));

    }

    //
    else if(column_order_list_it == "sample_rate")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . sample_rate_col));

    }

    //
    else if(column_order_list_it == "channels")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . channels_col));

    }

    //
    else if(column_order_list_it == "codec")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . codec_col));

    }

    //
    else if(column_order_list_it == "mime")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . mime_col));

    }



    // Appends a track # column.
    append_column(*new_column);

    // 
    new_column -> set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);



    // 
    string column_size_config_str
      = "gui.playlist.view.";

    // 
    column_size_config_str += playlist_view_name_;

    // 
    column_size_config_str += ".columns." + column_order_list_it + ".size";

    // 
    new_column -> set_fixed_width(config() . get(column_size_config_str));

  }



  debug("After columns appended!");



  // Iterates through the columns to modify settings.
  for(auto columns_it : get_columns())
  {

    // Makes the current column's headers reorderable.
    columns_it -> set_reorderable();

    // Makes the column header resizeable.
    columns_it -> set_resizable(true);



    // Callback for when the header is clicked.
    columns_it -> signal_clicked()
                    . connect(sigc::mem_fun(*this, &Playlist::Header_Clicked));



    // True if the current column is track number.
    if(columns_it -> get_title() == "#")
    { 

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . track_num_int_col);
    }

    // True if the current column is for the track number tag.
    else if(columns_it -> get_title() == "Title")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . title_col);

    }

    // True if the current column is for the artist tag.
    else if(columns_it -> get_title() == "Artist(s)")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . artist_col);

    }

    // True if the current column is for the album artists tag.
    else if(columns_it -> get_title() == "Album Artist(s)")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . artist_col);

    }

    // True if the current column is for the album tag.
    else if(columns_it -> get_title() == "Album")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . album_col);

    }

    // True if the current column is for the genre tag.
    else if(columns_it -> get_title() == "Genre")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . genre_col);

    }

    // True if the current column is for the track length.
    else if(columns_it -> get_title() == "Length")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . length_col);

    }

    // True if the current column is for the genre tag.
    else if(columns_it -> get_title() == "Date")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . date_col);

    }

    // True if the current column is for the track total tag.
    else if(columns_it -> get_title() == "Track Total")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . track_total_int_col);

    }

    // True if the current column is for the bit depth.
    else if(columns_it -> get_title() == "Bit Depth")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . bit_depth_col);

    }

    // True if the current column is for the bitrate.
    else if(columns_it -> get_title() == "Bitrate")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . bit_rate_col);

    }

    // True if the current column is for the sample rate.
    else if(columns_it -> get_title() == "Sample Rate")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . sample_rate_col);

    }

    // True if the current column is for the channel count.
    else if(columns_it -> get_title() == "Channels")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . channels_col);

    }

    // True if the current column is for the codec.
    else if(columns_it -> get_title() == "Codec")
    {

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . codec_col);

    }

    // True if the current column is mime type.
    else if(columns_it -> get_title() == "Mime")
    { 

      // Sets the sort column for the column.
      columns_it -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . mime_col);

    }

  }



  debug("After column changes!");



  // Sets a RefPtr to the TreeView's selection.
  playlist_treeselection_ = this -> get_selection();



  debug("After playlist_treesection assignment");



  // Assigns the TreeView's selection changed function.
  playlist_treeselection_ -> signal_changed()
    . connect(sigc::mem_fun(*this, &Playlist::On_Selection_Changed));



  // Sets the TreeView's TreeSelection to be able to select multiple rows.
  playlist_treeselection_ -> set_mode(Gtk::SELECTION_MULTIPLE);



  // Creates a new PlaylistMenu for the playlist TreeView.
  menu_ = new PlaylistMenu(base_ref, *this, playlists_ref);



  // 
  setting_name = "gui.playlist.view.";

  // 
  setting_name += playlist_view_name_;

  // 
  setting_name += ".locked";



  // 
  bool playlist_active = config() . get(setting_name . c_str());



  // 
  menu_ -> lock_check_menu_item() . set_active(playlist_active);



  // Mouse button press shortcuts overload function signal connection.
  signal_button_press_event()
    . connect(sigc::mem_fun(*this, &Playlist::on_button_press_event), false);

  // Mouse button release shortcuts overload function signal connection.
  signal_button_release_event()
    . connect(sigc::mem_fun(*this, &Playlist::on_button_release_event), false);

  // Drag data received overload function signal connection.
  signal_drag_data_received()
    . connect(sigc::mem_fun(*this, &Playlist::on_drag_data_received_signal),
              false);

  // Keyboard button press shortcuts overload function signal connection.
  signal_key_press_event()
    . connect(sigc::mem_fun(*this, &Playlist::on_key_press_event), false);

  // Keyboard button release shortcuts overload function signal connection.
  signal_key_release_event()
    . connect(sigc::mem_fun(*this, &Playlist::on_key_release_event), false);

  // Mouse movement overload function signal connection.
  signal_motion_notify_event()
    . connect(sigc::mem_fun(*this, &Playlist::on_motion_notify_event), false);




  // Sets the class as having finished construction
  constructed_ = true;



  debug("Playlist constructed!");

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Playlist::~Playlist()
{

  debug("Beginning Playlist destruction");



  // Deletes the PlaylistMenu.
  delete menu_;



  // Deletes the destination tree row ref for drag and drop.
  delete dest_tree_row_ref_;

  // Deletes the drug tree row ref for drag and drop.
  delete drag_tree_row_ref_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//                //
// Drag Callbacks /////////////////////////////////////////////////////////////
//                //

void Playlist::on_drag_begin(const Glib::RefPtr< Gdk::DragContext >& context)
{

  // Sets the icon shown while dragging.
//  drag_source_set_icon("list-add");

}

void Playlist::on_drag_data_received_signal
       (const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
        const Gtk::SelectionData& selection_data, guint info, guint time)
{

  debug("In on_drag_data_received");



  // Holds the retrieved drop position of the row. 
  Gtk::TreeViewDropPosition temp_drop_position;

  // Holds the path of the row the drag is currently over.
  Gtk::TreePath temp_tree_path;



  debug("Before get_drag_dest_row");



  // Retrieves the current drag position and row the cursor is over.
  get_drag_dest_row(temp_tree_path, temp_drop_position);

  // Stores the row position as an int.
  dest_row_drop_position_ = temp_drop_position;

  // Stores the row path as a row reference.
  (*dest_tree_row_ref_) = Gtk::TreeRowReference(playlist_treestore(),
                                                temp_tree_path);




  // Sets the hovering playlist treestore.
  playlists() . set_hovering_playlist_treestore(playlist_treestore());

} 

bool Playlist::on_drag_drop
       (const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, 
        guint time)
{

  debug("on_drag_drop");



  // 
  bool drag_in_queue = false;

  // 
  if((playlists() . selected_playlist_treestore())
       == (playlists() . queue_playlist_treestore()))
  {

    // 
    if((playlists() . hovering_playlist_treestore())
         != (playlists() . queue_playlist_treestore()))
    {

      // 
      return false;

    }  

    // 
    else
    {

      // 
      drag_in_queue = true;



      // 
      if((dest_tree_row_ref_ -> get_path())
           == (playlists() . playing_row_ref() . get_path()))
      {

        // 
        return false;

      }


    }

  }



  // Holds the size of the selected rows.
  int selected_rows_size
    = playlists() . selected_playlist() . playlist_treeselection()
        -> get_selected_rows() . size();

  // True if there are no selected rows.
  if(selected_rows_size == 0)
  {

    // The drag returns false.
    return false;

  }



  // Sets the treestore to be unsorted.
  playlist_treestore()
    -> set_sort_column(Gtk::TreeSortable::DEFAULT_UNSORTED_COLUMN_ID, 
                       Gtk::SORT_DESCENDING);



  // Stores the TreePaths of the selected rows in a vector.
  vector<Gtk::TreePath> selected_rows_paths
    = playlists() . selected_playlist() . playlist_treeselection()
                                            -> get_selected_rows();



  debug("Drag destination before");



  // True if the destination of the drug rows is before the destination row.
  bool before_row = false;

  // True if the drop position is before the destination row.
  if((dest_row_drop_position_
       == 
     (unsigned int)(Gtk::TREE_VIEW_DROP_BEFORE))
       ||
     (dest_row_drop_position_
       == (unsigned int)(Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE)))
  {

    // Sets the drag and drop position to be before the destination row.
    before_row = true;

  }



  // True if the playing track is in the drug rows.
  bool playing_row_present = false;

  // Indicates the row of the playing track if present.
  int playing_row_position = 1;

  // Will hold the Tracks of each row.
  vector<shared_ptr<Track>> selected_tracks_sptrs;

  // Will hold the converted TreeRowReferences of the selected rows.
  vector<Gtk::TreeRowReference> selected_row_refs;

  // Retrives the Track sptrs for each row and stores them and a
  // reference to each row.
  for(auto it : selected_rows_paths)
  { 

    // Converts the current row path to an iterator.
    Gtk::TreeIter temp_tree_iter
      = playlists() . selected_playlist_treestore() -> get_iter(it); 

    // Converts the iterator to a row ref using its TreeStore.
    Gtk::TreeRowReference temp_row_ref
      (playlists() . selected_playlist_treestore(), it);

    // Stores the new TreeRowReference in selected_track_sptrs.
    selected_row_refs . push_back(temp_row_ref);



    // 
    if((!(playback() . Stopped()))
        && 
       (temp_row_ref . get_path())
           == 
         (playlists() . playing_row_ref() . get_path()))
    {

      // Sets the playing row as present in the drug tracks.
      playing_row_present = true;



      // 
      if(drag_in_queue)
      {

        // 
        return false;

      }

    }



    // A temporary row.
    Gtk::TreeRow temp_row;

    // Converts the row iterator into a row.
    temp_row = *temp_tree_iter;

    // Gets a copy of the row's Track shared_ptr.
    shared_ptr<Track> temp_track
      = (temp_row[playlists() . selected_playlist() 
                                  . playlist_column_record() .track_col]);

    // Pushes the retrieved Track sptr to the back of selected_tracks_sptrs.
    selected_tracks_sptrs . push_back(temp_track);



    // 
    if(!playing_row_present)
    {

      playing_row_position++;

    }

  }



  // True if the loop is on the first row in order to make a row ref to it.
  bool first_selected_row = true;

  // Will hold a row ref to the first moved row.
  Gtk::TreeRowReference first_row_ref;

  // Will hold a row ref to the last moved row.
  Gtk::TreeRowReference last_row_ref;



  debug("Before drag insertion.");



  // True if the new row is before the drag and drop destination row.
  if(before_row)
  {

    // Holds an iterator to the beginning of the selected tracks. 
    auto selected_tracks_sptrs_it = selected_tracks_sptrs . begin();

    // Holds the current row number of the drug tracks in this loop.
    int current_row_number = 1;



    // Loops through the vector of selected track sptrs.
    while(selected_tracks_sptrs_it != selected_tracks_sptrs . end())
    {

      // Holds an iterator to the destination row.
      Gtk::TreeIter dest_row_it;

      // Holds an iterator to the new row.
      Gtk::TreeIter new_row_iter;



      // True if the destination row is not valid.
      if(!(*dest_tree_row_ref_))
      {

        // Appends a new row to an empty playlist.
        new_row_iter = playlist_treestore() -> append();

      }

      // True if the destination row is valid.
      else
      {

        // Creates an iterator to the destination row.
        dest_row_it 
          = dest_tree_row_ref_ 
              -> get_model() -> get_iter(dest_tree_row_ref_ -> get_path());



        // Creates a new row before the destination row.
        new_row_iter
          = playlist_treestore() -> insert(dest_row_it);

      }



      // True if the loop is one the first iteration.
      if(first_selected_row)
      {

        // Saves a reference to the first new row.
        first_row_ref = Gtk::TreeRowReference(playlist_treestore(), 
                                              Gtk::TreePath(new_row_iter));



        // Sets the flag for indicating the loop is not on the first row.
        first_selected_row = false;

      }



      // Dereferences the iterator into a new TreeRow.
      Gtk::TreeRow new_row = *new_row_iter;

      // Fills the row with the Track's data.
      playlists() . Fill_Row(new_row, *selected_tracks_sptrs_it);



      // Creates a reference to the current row each time until it's the last.
      last_row_ref = Gtk::TreeRowReference(playlist_treestore(), 
                                           Gtk::TreePath(new_row_iter));



      // True if the playing row is present and has already been found.
      if(playing_row_present)
      {

        // True if the row of the loop iteration is the playing track.
        if(current_row_number == playing_row_position)
        {

          // Sets playing_row_ref to the playing track's new position.
          playback() . track_queue() . front() -> row_ref() = last_row_ref;



          // Sets the playing row to not present row this code is ignored now.
          playing_row_present = false;

        }



        // Decrements the row position since they are inserted backwards.
        current_row_number++;

      }



      // Increments the track sptrs it.
      selected_tracks_sptrs_it++;

    }

  }

  // True if the new row is after the drag and drop destination row.
  else
  {

    // Holds an iterator to the reverse beginning of the selected tracks. 
    auto selected_tracks_sptrs_it = selected_tracks_sptrs . rbegin();

    // Holds the current row number of the drug tracks in this loop.
    int current_row_number = selected_tracks_sptrs . size();



    // Loops through the vector of selected track sptrs backward.
    while(selected_tracks_sptrs_it != selected_tracks_sptrs . rend())
    {

      // Holds an iterator to the destination row.
      Gtk::TreeIter dest_row_it;

      // Holds an iterator to the new row.
      Gtk::TreeIter new_row_iter;



      // True if the destination row is not valid.
      if(!(*dest_tree_row_ref_))
      {

        // Appends a new row to an empty playlist.
        new_row_iter = playlist_treestore() -> append();

      }

      // True if the destination row is valid.
      else
      {

        // Creates an iterator to the destination row.
        dest_row_it 
          = dest_tree_row_ref_ 
              -> get_model() -> get_iter(dest_tree_row_ref_ -> get_path());



        // Creates a new row before the destination row.
        new_row_iter
          = playlist_treestore() -> insert_after(dest_row_it);

      }



      // True if the loop is one the first iteration.
      if(first_selected_row)
      {

        // Saves a reference to the first new row.
        first_row_ref = Gtk::TreeRowReference(playlist_treestore(), 
                                              Gtk::TreePath(new_row_iter));



        // Sets the flag for indicating the loop is not on the first row.
        first_selected_row = false;

      }



      // Dereferences the iterator into a new TreeRow.
      Gtk::TreeRow new_row = *new_row_iter;

      // Fills the row with the Track's data.
      playlists() . Fill_Row(new_row, *selected_tracks_sptrs_it);



      // Creates a reference to the current row each time until it's the last.
      last_row_ref = Gtk::TreeRowReference(playlist_treestore(), 
                                           Gtk::TreePath(new_row_iter));



      // True if the playing row is present and has already been found.
      if(playing_row_present)
      {

        // True if the row of the loop iteration is the playing track.
        if(current_row_number == playing_row_position)
        {

          // Sets playing_row_ref to the playing track's new position.
          playback() . track_queue() . front() -> row_ref() = last_row_ref;



          // Sets the playing row to not present row this code is ignored now.
          playing_row_present = false;

        }



        // Decrements the row position since they are inserted backwards.
        current_row_number--;

      }



      // Increments the track sptrs it.
      selected_tracks_sptrs_it++;

    }

  }  



  debug("After drag insertion.");



  // Iterates through the old selected rows to remove them.
  for(auto it : selected_row_refs)
  {

    // Erases the row.
    playlists() . selected_playlist_treestore() 
      -> erase(playlists() . selected_playlist_treestore()
                               -> get_iter(it . get_path()));

  }



  //
  playlists() . set_disable_on_selection_changed(true);

  // Unselects everything in the selected playlist.
  playlists() . selected_playlist() . playlist_treeselection() -> unselect_all();

  // Unselects everything in the destination/hovering playlist.
  playlist_treeselection_ -> unselect_all();

  //
  playlists() . set_disable_on_selection_changed(false);



  // 
  if(playlists() . selected_playlist_treestore() != playlist_treestore_)
  {

    // 
    string playlist_name_str
      = playlists() . selected_playlist_treestore() -> get_name();

    // 
    playlists() . database() . Clear_Playlist(playlist_name_str . c_str());

    // 
    playlists() . database()
      . Add_Tracks(playlist_name_str . c_str(), playlists() . selected_playlist_treestore());

  }

  // 
  string playlist_name_str = playlist_treestore_ -> get_name();

  // 
  playlists() . database() . Clear_Playlist(playlist_name_str . c_str());

  // 
  playlists() . database() . Add_Tracks(playlist_name_str . c_str(), playlist_treestore_);



  // Selects all of the new rows.
  playlist_treeselection_ -> select(first_row_ref . get_path(),
                                    last_row_ref . get_path());



  // True if the drag destination is before the destination row.
  if(before_row)
  {

    // Sets the selected row ref as the first row.
    playlists() . set_selected_row_ref(first_row_ref);

  }

  // True if the drag destination is after the destination row.
  else
  {

    // Sets the selected row ref as the last row.
    playlists() . set_selected_row_ref(last_row_ref);

  }



  debug("end on_drag_drop");



  // Resets the track queue after dragging.
  playback() . Reset_Track_Queue();



  // 
  playlists() . drag_occurred_ = true;



  // Returns false, not perpetuating the drag drop. 
  return false;

} 

void Playlist::on_drag_end(const Glib::RefPtr<Gdk::DragContext>& context)
{ 

  debug("In on_drag_end");



  if(!(playlists() . drag_occurred_))
  {


  }



  playlists() . drag_occurred_ = false;

  // Sets the on_selection_changed flag to false.
  playlists() . set_disable_on_selection_changed(false);

  // Sets the drag_occurring flag to false.
  playlists() . set_drag_occurring(false);

}





//               //
// Key Callbacks //////////////////////////////////////////////////////////////
//               //

bool Playlist::on_key_press_event(GdkEventKey* event)
{

  // True if the delete key is pressed.
  if((event -> keyval == GDK_KEY_Delete))
  {

    if((playlist_treestore() -> children().size()) <= 0)
    {
      return true;
    }

    Delete_Selected_Rows();

    //Returning true, cancelling the propagation of the event.
    return true;

  }

  // True if the control key and 'c' are pressed.
  else if((event -> keyval == GDK_KEY_c)
            &&
          (event -> state & GDK_CONTROL_MASK))
  {

    // Copies the selected tracks.
    Copy_Selected_Rows();



    // Returns true for the key press, stopping propagation.
    return true;

  } 

  // True if the control key and 'x' are pressed.
  else if((event -> keyval == GDK_KEY_x)
            &&
          (event -> state & GDK_CONTROL_MASK))
  {

    // Cuts the selected tracks.
    Cut_Selected_Rows();



    // Returns true for the key press, stopping propagation.
    return true;

  }

  // True if the control key and 'v' are pressed.
  else if((event -> keyval == GDK_KEY_v)
             &&
          (event -> state & GDK_CONTROL_MASK))
  {

    // Pastes the tracks from the clipboard.
    Paste_Clipboard_Rows();



    // Returns true for the key press, stopping propagation.
    return true;

  }

  // True if the control key and 'a' are pressed.
  else if((event -> state & GDK_CONTROL_MASK)
            &&
          (event -> keyval == GDK_KEY_a))
  {

    // Selects all of the rows.
    playlist_treeselection_ -> select_all();



    // Returns true for the key press, stopping propagation.
    return true;

  }



  // Returns the event to the normal key press callback.
  return Gtk::TreeView::on_key_press_event(event);

}

bool Playlist::on_key_release_event(GdkEventKey* event)
{

  // True if the control key is released.
  if(event -> state & GDK_CONTROL_MASK)
  {

    // Sets the selection flag as false.
    selecting_flag_ = false;

  }



  // True if the control key is pressed and the 'a' key is released.
  if((event -> state & GDK_CONTROL_MASK)
       &&
     (event -> keyval == GDK_KEY_a))
  {

    // Sets the clipboard_event_ flag to false.
    clipboard_event_ = false;



    // Returns true to not propagate the normal event.
    return true;

  } 

  else if(event -> keyval == GDK_KEY_Escape)
  {


  }



  // Returns the normal key press event.
  return Gtk::TreeView::on_key_release_event(event);

}





//                 //
// Mouse Callbacks ////////////////////////////////////////////////////////////
//                 //

bool Playlist::on_button_press_event(GdkEventButton* event)
{ 

  debug("In button press event");



  // True if the button press event is disabled for one occurrence.
  if(playlists() . skip_button_press())
  {

    // Enables the button press event.
    playlists() . set_skip_button_press(false);



    // Returns true to not propagate the normal keyboard event.
    return true;

  }



  // Sets the selected playlist as the playlist clicked.
  playlists() . set_selected_playlist(*this);



  // True if the control key is being held.
  if(event -> state & GDK_CONTROL_MASK)
  { 

    // Sets the selecting flag to true. 
    selecting_flag_ = true;

  }



  // True if the right mouse button has been clicked.
  if((event -> type == GDK_BUTTON_PRESS) && (event -> button == 3))
  { 

    debug("Inside right button click");



    // Store the x coordinate of the mouse in the TreeView.
    int x = event -> x;

    // Store the y coordinate of the mouse in the TreeView.
    int y = event -> y;



    // Will hold the path of a row. 
    Gtk::TreePath row_tree_path;

    // True if a row exists at the coordinates and then stores it as a path.
    if((this -> get_path_at_pos(x, y, row_tree_path)))
    {

      // True if the row being right-clicked on is not selected.
      if(!(playlist_treeselection_ -> is_selected(row_tree_path)))
      {

        //
        playlists() . set_disable_on_selection_changed(true);

        // Unselects all of the rows.
        playlist_treeselection_ -> unselect_all();

        //
        playlists() . set_disable_on_selection_changed(false);



        // Selects the row being right-clicked.
        playlist_treeselection_ -> select(row_tree_path);

      } 



      // True if there is at least one track in the clipboard. 
      if(!(playlists().clipboard_tracks()) . empty())
      {

        // Sets the paste playlist menu item to clickable.
        menu_ -> paste_menu_item() . set_sensitive(true);

      } 

      // True if there are no tracks in the clipboard.
      else
      {

        // Sets the paste playlist menu item to unclickable.
        menu_ -> paste_menu_item() . set_sensitive(false);

      }



      // Sets the cut playlist menu item to clickable.
      menu_ -> cut_menu_item() . set_sensitive(true);

      // Sets the copy playlist menu item to clickable.
      menu_ -> copy_menu_item() . set_sensitive(true);

      // Sets the edit playlist menu item to unclickable.
      menu_ -> edit_menu_item() . set_sensitive(false);

      // Sets the queue playlist menu item to clickable.
      menu_ -> queue_menu_item() . set_sensitive(true);

      // Sets the delete playlist menu item to clickable.
      menu_ -> delete_menu_item() . set_sensitive(true);



      debug("Before playlist_Menu popup");



      // Opens the playlist menu.
      menu_ -> popup(event -> button, event -> time);

    } 

    // True if one the playlist's column headers is right-clicked.
    else if(y < 0)
    {

    } 

    // True if a no row or column header is present where the mouse was
    // right-clicked in the TreeView. 
    else
    { 

      // True if there is at least one track in the clipboard. 
      if(!(playlists() . clipboard_tracks()) . empty())
      { 

        // Sets the paste playlist menu item to clickable.
        menu_ -> paste_menu_item() . set_sensitive(true);

      }

      // True if there are no tracks in the clipboard.
      else
      { 

        // Sets the paste playlist menu item to unclickable.
        menu_ -> paste_menu_item() . set_sensitive(false);

      }



      // Sets the cut playlist menu item to unclickable.
      menu_ -> cut_menu_item() . set_sensitive(true);

      // Sets the copy playlist menu item to unclickable.
      menu_ -> copy_menu_item() . set_sensitive(true);

      // Sets the edit playlist menu item to ununclickable.
      menu_ -> edit_menu_item() . set_sensitive(false);

      // Sets the queue playlist menu item to unclickable.
      menu_ -> queue_menu_item() . set_sensitive(true);

      // Sets the delete playlist menu item to unclickable.
      menu_ -> delete_menu_item() . set_sensitive(true);



      debug("Before playlist_Menu popup");



      // Opens the playlist menu.
      menu_ -> popup(event -> button, event -> time);

    } 



    // Returns true, preventing other signal handlers from triggering.
    return true;

  }

  // True if the left mouse button has been clicked.
  else if((event -> type == GDK_BUTTON_PRESS) && (event -> button == 1))
  {

    debug("In left mouse click");



    // Sets the keyboard focus to this TreeView.
    this -> grab_focus();



    // Stores the x coordinate of the mouse in the TreeView.
    int x = event -> x;

    // Stores the y coordinate of the mouse in the TreeView.
    int y = event -> y;

    // Stores the x coordinate of the mouse in the row's cell.
    int x_cell;

    // Stores the y coordinate of the mouse in the row's cell.
    int y_cell;



    // True if the control key is being held.
    if((event -> state & GDK_CONTROL_MASK)
       ||
       (event -> state & GDK_SHIFT_MASK))
    {

      debug("Control or shift held");

    }

    // True if the control key is not held.
    else
    {

      // Will hold the path of a row. 
      Gtk::TreePath row_tree_path;

      // Will hold a pointer to the column of the row the mouse is over.
      Gtk::TreeView::Column* row_column;


      // 
      bool get_path_result;

      // True if there is no row at the position of the mouse. 
      get_path_result = this -> get_path_at_pos(x,y, row_tree_path,
                                                row_column, x_cell, y_cell);



      // 
      if(!get_path_result)
      {

        // True if there is nothing at the mouse coordinates.
        if((this -> is_blank_at_pos(x,y)))
        { 

          // True if the config is set to not deselect when empty space in
          // the playlist is clicked.
          if(!(config() . get("gui.playlist.empty_space_playlist_deselects")))
          {

            // Passes the button event to the normal signal handler.
            return Gtk::TreeView::on_button_press_event(event);

          }



          // Unselects all of the track in the TreeView.
          playlist_treeselection_ -> unselect_all();



          // Sets the selected track time addition of the status bar to 0/
          Add_Selected_Tracks_Times();



          // Assigns an empty string.
          string covername_str = "";

          // Sends an empty string to the cover art changer so it knows to set
          // the cover image to the default.
          gui() . Load_Cover_Art(covername_str);



          // Updates the tagview to nothing selected.
          gui() . Update_Tagview("Selected", playback() . empty_track());



          // Sets the filename label to blank. 
          filename_label_ -> set_text("");



          // Passes the button event to the normal signal handler.
          return Gtk::TreeView::on_button_press_event(event);

        }

      }

      else
      {

        // 
        set_search_column(row_column -> get_sort_column_id());

      }



      // True if there is a row at this position, but no text.
      if((this -> is_blank_at_pos(x,y))
           && (config() . get("gui.playlist.empty_space_row_deselects")))
      {

        // Unselects all of the track in the TreeView.
        playlist_treeselection_ -> unselect_all();



        // Sets the selected track time addition of the status bar to 0/
        Add_Selected_Tracks_Times();



        // Assigns an empty string.
        string covername_str = "";

        // Sends an empty string to the cover art changer so it knows to set
        // the cover image to the default.
        gui() . Load_Cover_Art(covername_str);



        // Updates the tagview to nothing selected.
        gui() . Update_Tagview("Selected", playback() . empty_track());



        // Sets the filename label to blank. 
        filename_label_ -> set_text("");



        // Returns true to not propagate the event.
        return true;

      } 

      // True if there is a row at x and y and there is text.
      else
      {

        // True if the row of the button press is not selected.
        if(!(playlist_treeselection_ -> is_selected(row_tree_path)))
        {

          // Disables the selection_changed() signal callback.
          playlists() . set_disable_on_selection_changed(true);

          // Unselects all of the rows.
          playlist_treeselection_ -> unselect_all();

          // Enables the selection_changed() signal callback.
          playlists() . set_disable_on_selection_changed(false);



          debug("Before set_cursor in mouse press override");



          // Selects the row of the button press.
          set_cursor(row_tree_path);

         }

        

        // Stores a reference to the row being drug.
        *drag_tree_row_ref_
          = Gtk::TreeRowReference(playlist_treestore(), row_tree_path);



        // True if the previous button press event didn't just occur.
        if((previous_button_press_event_time_) != (event -> time))
        {

          // True if the last button press was less than .25 seconds ago.
          if(((event -> time) - previous_button_press_event_time_) < 250)
          {

            debug("Double click playing");



            // Sets the previous button press event time.
            previous_button_press_event_time_ = event -> time; 



            if(playlist_treestore_ != playlists() . queue_playlist_treestore())
            {

              // Starts playback.
              playback() . Play(playlists() . selected_row_ref());

            }


            handle_multirow_drag_ = false;



            // Returns false to not continue the event.
            return true;

          } 

        }



        debug("Drag occurring");



        // Sets previous_button_press_event_time.       
        previous_button_press_event_time_ = event -> time; 



        // Sets the multirow_drag flag to true. 
        handle_multirow_drag_ = true;

        // Saves the x coordinate of the mouse position upon clicked.
        x_click_ = x;

        // Saves the y coordinate of the mouse position upon clicked.
        y_click_ = y;



        // Prevents the normal mouse behviour from propagating.
        return false;

      } 

    }

  } 



  // Propagates normal mouse button behaviour.
  return Gtk::TreeView::on_button_press_event(event);

}

bool Playlist::on_button_release_event(GdkEventButton* event)
{

  debug("In on_button_release_event");



  // If true, it means a button press happened without dragging a row.
  if(handle_multirow_drag_)
  {

    debug("In handle_multirow_drag in button release.");



    // Sets handle_multirow_drag_ to false.
    handle_multirow_drag_ = false;



    //
    playlists() . set_disable_on_selection_changed(true);

    // Unselects everything.
    playlist_treeselection_ -> unselect_all();

    //
    playlists() . set_disable_on_selection_changed(false);





    // Sets the cursor at the row that was selected.
    set_cursor((*drag_tree_row_ref_) . get_path());

  } 



  // Sends the event to the normal button release function.
  return Gtk::TreeView::on_button_release_event(event);

}

bool Playlist::on_motion_notify_event(GdkEventMotion* motion_event)
{ 

  // 
  bool minimum_dist = false;



  // 
  int x = motion_event -> x;

  // 
  int y = motion_event -> y;



  // 
  if(x > x_click_)
  {

    // 
    if(x >= (x_click_ + 5))
    {

      // 
      minimum_dist = true;

    }

  }

  // 
  else if(x < x_click_)
  {

    // 
    if(x <= (x_click_ - 5))
    {

      // 
      minimum_dist = true;

    }

  }



  // 
  if(y > y_click_)
  {

    // 
    if(y >= (y_click_ + 5))
    {

      // 
      minimum_dist = true;

    }

  }

  // 
  else if(y < y_click_)
  {

    // 
    if(y <= (y_click_ - 5))
    {

      // 
      minimum_dist = true;

    }

  }



  // True if a drag has occurred.
  if(handle_multirow_drag_ && minimum_dist)
  { 

    // Sets handle_multirow_drag_ to false.
    handle_multirow_drag_ = false;


    // True if a drag is already occurring.
    if(playlists() . drag_occurring())
    { 
  
      // Calls the normal montion_notify event if a drag is already occurring.
      return Gtk::TreeView::on_motion_notify_event(motion_event);

    } 



    // Will hold the TargetList for the playlist.
    Glib::RefPtr<Gtk::TargetList> playlist_targetlist;

    // Stores a RefPtr to the drag destiation TargetList.
    playlist_targetlist = drag_dest_get_target_list();



    // Sets the playlist as a drag occurring.
    playlists() . set_drag_occurring(true);

    // Disables the selection change flag.
    playlists() . set_disable_on_selection_changed(true);




    // Starts a drag event.
    drag_begin(playlist_targetlist, Gdk::DragAction::ACTION_MOVE, 1, 
               NULL, x_click_, y_click_); 



    // Doesn't allow the mouse movement's normal event to propagate. 
    return false;

  }



  // Calls the normal montion_notify event if a drag is already occurring.
  return Gtk::TreeView::on_motion_notify_event(motion_event);

}





//                  //
// Playlist Actions ///////////////////////////////////////////////////////////
//                  //

void Playlist::Add_Selected_Tracks_Times()
{

  // Holds the total duration of the tracks.
  long long total_duration_nanoseconds = 0LL;



  // 
  int row_count = 0;



  // Iterates through all of the Playlists.
  for(auto playlists_it : playlists()())
  {

    // Creates a vector of paths to the selected rows.
    vector<Gtk::TreeModel::Path> selected_rows
      = playlists_it -> playlist_treeselection() -> get_selected_rows();

    // Iterates through all of the selection rows.
    for(auto selected_rows_it : selected_rows)
    { 

      // Creates an iterator to the treemodel of the selected rows.
      Gtk::TreeModel::iterator selected_row_treemodel_it
        = playlists_it -> playlist_treestore() -> get_iter(selected_rows_it);



      // Will hold a copy of the selected row.
      Gtk::TreeRow row;

      // Derferences the iterator into a row.
      row = *selected_row_treemodel_it;

      // Makes a copy to the Track sptr of the row.
      shared_ptr<Track> temp_track_sptr
        = row[playlists() . playlist_column_record() . track_col];



      // Adds the current Track's duration to the total duration.
      total_duration_nanoseconds += temp_track_sptr -> duration();

      // 
      row_count++;

    }

  }



  // Converts the added time to a time format string.
  std::string* time_str_ptr
    = time_converter()
        . Nanoseconds_To_Time_Format(total_duration_nanoseconds);

  // Sets the StatusBars' label for the selected track times.
  gui() . set_selected_time_label(time_str_ptr -> c_str());

  // 
  gui() . set_selected_rows_count_label(row_count);

  // Deletes the time time format string.
  delete time_str_ptr;

}

void Playlist::Change_Playlist()
{ 

  // True if the Playlist is still constructing or if selection is disabled.
  if(!constructed_ || (playlists() . disable_on_selection_changed()))
  { 

    // Returns, ending the Change_Playlist function.
    return;

  }



  // Disables the selection changed callback.
  playlists() . set_disable_on_selection_changed(true);



  // Creates an iterator for the list of the PlaylistMenu's MenuRadioItems.
  auto playlist_menu_radio_menu_items_it
    = menu_ -> playlists_menu_radio_menu_items() . begin();

  // Iterates through the the playlist treestores.
  for(auto playlist_treestores_it : playlists() . playlist_treestores())
   {

    // True if the current playlist RadioMenuItem is selected.
    if((*playlist_menu_radio_menu_items_it) -> get_active())
     {

      // Sets the current playlist to the label of change_playlist_menu_item.
      menu_ -> change_playlist_menu_item()
        . set_label("Playlist: " + playlist_treestores_it -> get_name());



      // Sets the playlist treestore as the current one of the iteration.
      set_playlist_treestore(playlist_treestores_it);



      // 
      string setting_name = "gui.playlist.view.";

      // 
      setting_name += playlist_view_name_;

      // 
      setting_name += ".active";



      // 
      config()
        . set(setting_name . c_str(), playlist_treestores_it -> get_name());



      // 
      config() . write_file();



      // Breaks out of the loop.
      break;

    }



    // Increments the it of the playlist menu's RadioMenuItems.
    playlist_menu_radio_menu_items_it++;

  }



  // Enables the selection changed function.
  playlists() . set_disable_on_selection_changed(false);

}

void Playlist::Header_Clicked()
{

  // 
  if((playlist_treestore_ == playlists() . selected_playlist_treestore())
       ||
     (playlist_treestore_ == playlists() . playing_playlist_treestore()))
   {
  
    // Resets the track queue.
    playback() . Reset_Track_Queue();

  }



  // 
  if((playlist_treestore_ -> children() . size()) > 0)
   {

    // 
    string playlist_name_str = playlist_treestore_ -> get_name();



    // 
    playlists() . database() . Clear_Playlist(playlist_name_str . c_str());

    // 
    playlists() . database()
      . Add_Tracks(playlist_name_str . c_str(), playlist_treestore_);

  }

}

void Playlist::Lock()
{

  // 
  string setting_name = "gui.playlist.view.";

  // 
  setting_name += playlist_view_name_;

  // 
  setting_name += ".locked";



  // 
  bool playlist_active = menu_ -> lock_check_menu_item() . get_active();



  // 
  config() . set(setting_name . c_str(), playlist_active);



  // 
  config() . write_file();


}

void Playlist::On_Selection_Changed()
{

  debug("In On_Selection_Changed() before return conditions");



  // True if rows are being inserted.
  if(playlists() . inserting())
  {

    // Ends On_Selection_Changed.
    return;

  }

  // True if rows are being deleting_.
  else if(deleting_)
  {

    // Ends On_Selection_Changed.
    return;

  }

  // True if the flag to disable On_Selection_Changed is true. 
  else if((playlists() . disable_on_selection_changed()))
  {

    // Ends On_Selection_Changed.
    return;

  }

  // True if a clipboard event is happening.
  else if(clipboard_event_)
  {

    // Ends On_Selection_Changed.
    return;

  }

  // True if the playlist is empty or no rows are selected.
  else if(((playlist_treestore() -> children().size()) == 0)
            ||
          ((playlist_treeselection_ -> count_selected_rows()) == 0))
  {

    // Makes a new empty TreeRowReference.
    Gtk::TreeRowReference temp_row_ref;

    // Sets the selected_row_ref to an empty row ref.
    playlists() . set_selected_row_ref(temp_row_ref);



    // Makes an empty string.
    string temp_string = "";

    // Sets the cover art to an empty string.
    gui() . Load_Cover_Art(temp_string);



    // Sets the TagViews as nothing selected.
    gui() . Update_Tagview("Selected", playback() . empty_track());



    // Sets the selected time label to 0.
    Add_Selected_Tracks_Times();



    // Sets the filename label as empty.
    filename_label_ -> set_text("");



    //
    if(!(playback() . Stopped()))
    {

      if(playlist_treestore_ != playlists() . queue_playlist_treestore())
      {

        // 
        playlists() . new_selection() = false;



        // Resets the queue used to ready the next track.
        playback() . Reset_Track_Queue();

      } 

    }



    // Ends On_Selection_Changed.
    return;

  }



  debug("On_Selection_Changed() occurring");



  // Calls the function to add the times of all selected tracks.
  Add_Selected_Tracks_Times();



  // Creates a vector of the selected rows.
  vector<Gtk::TreeModel::Path> selected_rows
    = playlist_treeselection_ -> get_selected_rows();

  // Creates a TreeRowReference to the first selected track.
  Gtk::TreeRowReference selected_row_ref(playlist_treestore(),
                                         selected_rows[0]);



  // Sets selected_row_ref to the new selected row. 
  playlists() . set_selected_row_ref(selected_row_ref);

  // Sets the selected_playlists to the last clicked playlist.
  playlists() . set_selected_playlist(*this);

  // Tests if the selected_row_ref is valid.
  if(!selected_row_ref)
  { 

    // Returns if that selected_row_ref is invalid.
    return;

  }



  // 
  if(playlist_treestore_ != playlists() . queue_playlist_treestore())
  {

    // 
    playlists() . new_selection() = true;

  }



  // An iterator to the first selected row.
  Gtk::TreeIter first_selected_row_iter
    = playlist_treestore() -> get_iter(selected_row_ref . get_path());

  // 
  if((config() . get("playback.playback_follows_cursor"))
       && 
     (!(playback() . Stopped()))
       &&
     (playlist_treestore_ != playlists() . queue_playlist_treestore()))
  {

    // 
    Gtk::TreeIter playing_row_iter
      = playlists() . playing_playlist_treestore()
          -> get_iter(playlists() . playing_row_ref() . get_path());



    // 
    if((playlist_treestore_ != playlists() . playing_playlist_treestore())
         ||
       (first_selected_row_iter != playing_row_iter))
    {

      // 
      playback() . Reset_Track_Queue();

    }

  }



  // A new TreeRow variable to hold the selected row.
  Gtk::TreeRow first_selected_row;

  // Sets the row to point to the selected row.
  first_selected_row = *first_selected_row_iter;

  // Retrieves a copy of the selected row's track iterator.
  shared_ptr<Track> temp_track_ptr
      = first_selected_row[playlist_column_record() . track_col];



  // Stores a reference to selected row's track.
  Track &new_created_track = *temp_track_ptr;

  // Updates the TagViews.
  gui() . Update_Tagview("Selected", new_created_track);



  // Sets the filename label to the newly selected track.
  filename_label_ -> set_label(new_created_track . filename());



  // Loads cover art for selected track.
  string filename_string = new_created_track.filename();

  // Sets the cover art of the selected track.
  gui().Load_Cover_Art(filename_string);

}





//                  //
// Row Manipulators ///////////////////////////////////////////////////////////
//                  //

void Playlist::Copy_Selected_Rows()
{

  // True if the playlist's treestore is empty.
  if((playlist_treestore() -> children().size()) <= 0)
  {

    // Exits the function.
    return;

  }



  // Sets the clipboard event flag as true.
  clipboard_event_ = true;

  // Clears the clipboard.
  playlists() . clipboard_tracks() . clear();



  // Creates of vector of the paths of all of the selected rows.
  vector<Gtk::TreeModel::Path> selected_rows
    = playlist_treeselection_ -> get_selected_rows();



  // True if there are no selected rows.
  if((selected_rows.size()) < 1)
  {

    // Exits the function.
    return;

  } 



  // Gets an iterator to to the beginning of the selected rows.
  vector<Gtk::TreeModel::Path>::iterator selected_rows_it
    = selected_rows . begin();

  // An iterator used for converting the selected row path to an iterator.
  Gtk::TreeModel::iterator selected_row_it;

  // Iterates through the selected rows. 
  while(selected_rows_it != (selected_rows . end()))
  {

    // Creates an iterator to the current selected row.
    selected_row_it = playlist_treestore() -> get_iter(*(selected_rows_it));



    // A new row for holding the dereferenced treestore iterator.
    Gtk::TreeRow row;

    // Dereferences the treestore iterator into a row.
    row = *selected_row_it;



    // Creates a copy of the current row's Track shared_ptr.
    shared_ptr<Track> temp_track_sptr = (row[playlist_column_record() . track_col]);

    // Pushes the Track sptr to the back of the clipboard tracks.
    playlists().clipboard_tracks().push_back(temp_track_sptr);



    // Iterates the selected rows it.
    selected_rows_it++;

  }



  // Sets the clipboard event flag to false.
  clipboard_event_ = false;

} 

void Playlist::Cut_Selected_Rows()
{

  // True if the Playlist's treestore is empty.
  if((playlist_treestore() -> children().size()) <= 0)
  {

    // Exits the function.
    return;

  }



  // Copies the selected rows.
  Copy_Selected_Rows();



  // Deletes the selected rows.
  Delete_Selected_Rows();

}

void Playlist::Delete_Selected_Rows()
{

  // 
  deleting_ = true;

  //
  clipboard_event_ = true;



  //
  playlists() . set_selected_row_ref(Gtk::TreeRowReference());



  // 
  vector<Gtk::TreeRowReference> selected_rows_refs;

  // 
  vector<Gtk::TreeModel::Path> selected_rows_paths
      = playlist_treeselection_ -> get_selected_rows();



  // 
  bool restart_playback_if_queue_not_empty = false;
  


  //
  for(auto row_path : selected_rows_paths)
  {

    // 
    if(playlist_treestore_ == playlists() . playing_playlist_treestore())
    {

      // 
      if(row_path == (playlists() . playing_row_ref() . get_path()))
      {

        // 
        if(playlist_treestore_ == playlists() . queue_playlist_treestore())
        {

          // 
          playback() . delete_playback_queue_row() = false;


          // 
          restart_playback_if_queue_not_empty = true;

        }

        //  
        playback(). Stop();

      }

    }


    // 
    selected_rows_refs
      . push_back(Gtk::TreeRowReference(playlist_treestore_, row_path));

  }



  // 
  playlists() . set_disable_on_selection_changed(true);



  // 
  vector<int> ids;

  // 
  for(auto row_refs_it : selected_rows_refs)
  {

    // Skips deleting the row if it's the deleted queue row.
    if(!row_refs_it)
    {

      // 
      continue;

    }

    // 
    Gtk::TreeModel::iterator selected_rows_it
        = playlist_treestore() -> get_iter(row_refs_it . get_path());

    // 
    Gtk::TreeRow current_row = *selected_rows_it;



    // 
    ids . push_back(int(current_row[playlist_column_record() . id_]));



    // 
    playlist_treestore() -> erase(selected_rows_it);

  }



  // 
  playlist_treeselection_ -> unselect_all();



  // 
  playlists() . set_disable_on_selection_changed(false);



  // Makes a new empty TreeRowReference.
  Gtk::TreeRowReference temp_row_ref;

  // Sets the selected_row_ref to an empty row ref.
  playlists() . set_selected_row_ref(temp_row_ref);



  // Makes an empty string.
  string temp_string = "";

  // Sets the cover art to an empty string.
  gui() . Load_Cover_Art(temp_string);



  // Sets the TagViews as nothing selected.
  gui() . Update_Tagview("Selected", playback() . empty_track());



  // Sets the filename label as empty.
  filename_label_ -> set_text("");



  // 
  if(restart_playback_if_queue_not_empty)
  {

    // 
    if(!(playlists() . queue_playlist_treestore() -> children() . empty()))
    {

      // 
      playback() . Play(Gtk::TreeRowReference());

    }

  }

  else
  {

    //
    playback() . Reset_Track_Queue();  

  }



  // 
  string playlist_name_str = playlist_treestore_ -> get_name();

  // 
  playlists() . database() . Delete_Rows(playlist_name_str . c_str(), ids);



  // Sets the selected time label to 0.
  Add_Selected_Tracks_Times();



  //
  clipboard_event_ = false;

  // 
  deleting_ = false;

}

void Playlist::Paste_Clipboard_Rows()
{

  // 
  if((playlists() . clipboard_tracks() . empty()))
  {

    return;

  }

  // 
  else if(clipboard_event_)
  {

    return;

  }



  // 
  clipboard_event_ = true;



  // 
  for(auto clipboard_it : playlists() . clipboard_tracks())
  {

    // 
    Gtk::TreeRow row;

    // 
    row = *(playlist_treestore() -> append());

    // 
    playlists() . Fill_Row(row, clipboard_it);



    // 
    playlist_treeselection_ -> select(row);

  }



  // 
  playback() . Reset_Track_Queue();



  // 
  string playlist_name = playlist_treestore_ -> get_name();

  // 
  playlists() . database() . Clear_Playlist(playlist_name . c_str());

  // 
  playlists() . database()
    . Add_Tracks(playlist_name . c_str(), playlist_treestore_);



  // 
  Add_Selected_Tracks_Times();



  // 
  clipboard_event_ = false;

}

void Playlist::Queue_Rows()
{

  // 
  for(auto selected_row_path : playlist_treeselection_ -> get_selected_rows())
  {

    // 
    Gtk::TreeIter selected_row_it
      = playlist_treestore_ -> get_iter(selected_row_path);

    // 
    Gtk::TreeRow selected_row = *selected_row_it;

    // 
    shared_ptr<Track> row_track_sptr
      = selected_row[playlist_column_record() . track_col];



    // 
    Gtk::TreeRow new_queue_row
      = *(playlists() . queue_playlist_treestore() -> append());

    // 
    playlists() . Fill_Row(new_queue_row, row_track_sptr);

  }



  // 
  playlists() . database() . Clear_Playlist("Queue");



  // 
  playlists() . database()
    . Add_Tracks("Queue", playlists() . queue_playlist_treestore());



  // 
  playback() . Reset_Track_Queue();

}





//                  //
// State Indicators ///////////////////////////////////////////////////////////
//                  //

bool Playlist::Locked()
{

  return menu_ -> lock_check_menu_item() . get_active();

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

Glib::ustring Playlist::active_playlist()
{

  return playlist_treestore() -> get_name();

}

Gtk::Label& Playlist::filename_label()
{

  return *filename_label_;

}

PlaylistMenu& Playlist::menu()
{

  return *menu_;

}

PlaylistColumnRecord& Playlist::playlist_column_record()
{

   return playlists().playlist_column_record();

}

Glib::RefPtr<Gtk::TreeSelection>& Playlist::playlist_treeselection()
{ 

  return playlist_treeselection_; 

}

Glib::RefPtr<PlaylistTreeStore> Playlist::playlist_treestore()
{

  return playlist_treestore_;

}

const char* Playlist::playlist_view_name()
{

  return playlist_view_name_;

}





//         //
//         //
// Setters //////////////////////////////////////////////////////////////////
//         //
//         //

void Playlist::set_playlist_treestore
  (Glib::RefPtr<PlaylistTreeStore> new_treestore)
{

  this -> set_model(new_treestore);

  playlist_treestore_ = new_treestore;

}
