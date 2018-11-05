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

#include "Playlist.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../../Metadata/Metadata.h"

#include "../../../Metadata/TimeConversion.h"

#include "../../../Metadata/Track.h"

#include "../../../Playback/Playback.h"

#include "../../../Playback/TrackBin.h"

#include "../../GUI.h"

#include "../../StatusBar.h"

#include "../PlaylistComboBoxes/PlaylistComboBox.h"

#include "../PlaylistComboBoxes/PlaylistComboBoxes.h"

#include "PlaylistColumnRecord.h"

#include "PlaylistMenu.h"

#include "PlaylistRenameDialog.h"

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

#include <glibmm/main.h>

#include <gtkmm/box.h>

#include <gtkmm/eventbox.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/progressbar.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/separator.h>

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

, editing_canceled_(false)

, playlist_view_name_(new_playlist_view_name)

, rename_playlist_dialog_(new PlaylistRenameDialog(base_ref))

, right_click_row_tree_row_ref_(new Gtk::TreeRowReference)



// Flags

, clipboard_event_(false)

, constructed_(false)

, selecting_flag_(false)



// GUI Parts

, playlist_box_(Gtk::manage(new Gtk::Box))

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



// Status Bar

, copy_progress_bar_(new Gtk::ProgressBar)

, name_label_(Gtk::manage(new Gtk::Label))

, name_label_event_box_(Gtk::manage(new Gtk::EventBox))

, progress_bar_(Gtk::manage(new Gtk::ProgressBar))

, progress_bars_box_(Gtk::manage(new Gtk::Box))

, row_count_label_(Gtk::manage(new Gtk::Label("0")))

, status_box_(Gtk::manage(new Gtk::Box))

, status_frame_(Gtk::manage(new Gtk::Frame))

{

  // Adds the new FileChooser object to the FileChoosers list.
  playlists_ref() . push_front(this);

  // Adds the new FileChooser's to the iterator to it's it storage variable.
  set_gui_elements_it(playlists_ref() . begin());



  // Sets the orientation of the GUIElement box() as vertical.
  box() . set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Puts the playlist_frame_ in the top of the GUIElement box().
  box() . pack_start(*playlist_box_, Gtk::PACK_EXPAND_WIDGET);



  // Sets the orientation of the playlist_box_ as vertical.
  playlist_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Puts the status_frame_ in the bottom of the GUIElement box().
  playlist_box_ -> pack_start(*playlist_frame_, Gtk::PACK_EXPAND_WIDGET);

  // Puts the status_frame_ in the bottom of the GUIElement box().
  playlist_box_ -> pack_end(*status_frame_, Gtk::PACK_SHRINK);



  // Adds a bottom margin to GUIElement box.
  box() . set_margin_bottom(5);



  // Sets the behaviour of the scrobbable window of the playlist to appear
  // automatically when needed.
  playlist_scrolled_window_ -> set_policy(Gtk::POLICY_AUTOMATIC,
                                          Gtk::POLICY_AUTOMATIC);

  // Adds the TreeView to playlist_scrolled_window_.
  playlist_scrolled_window_ -> add(*this);



  // Add's playlist_scrolled_window to playlist_frame_.
  playlist_frame_ -> add(*playlist_scrolled_window_);



  // 
  this -> set_tooltip_text
    ("- Double left-click quickly (< 0.25s) to start playback at the clicked row.\n\n" \
     "- Double left-click slowly (> 0.25s and < 0.5s) to edit the clicked metadata field.\n\n" \
     "- Single right-click to open the playlist menu.\n\n" \
     "- Single left-click and hold to drag the selected row or rows.");



  // 
  string setting_name = "gui.playlist.view.";

  // 
  setting_name += playlist_view_name_;

  // 
  setting_name += ".active";



  // 
  string playlist_treestore_name = config() . get(setting_name . c_str());



  // Assigns the TreeModel to the TreeView.
  for(auto playlist_treestores_it
        = playlists_ref . playlist_treestores() . begin();
      playlist_treestores_it != playlists_ref . playlist_treestores() . end();
      playlist_treestores_it++)
  {

    // 
    if(((*playlist_treestores_it) -> get_name()) == playlist_treestore_name)
    {

      // 
      set_playlist_treestore((*playlist_treestores_it));

    }

  }



  debug("After model set!");



  // 
  set_enable_search(false);

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
    int column_num = -1;



    //
    if(column_order_list_it == "album")
    {

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("album"),
         playlists_ref . playlist_column_record() . album_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . album_);

    }

    //
    else if(column_order_list_it == "album_artists")
    {

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("album_artists"),
         playlists_ref . playlist_column_record() . album_artists_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . album_artists_);

    }

    //
    else if(column_order_list_it == "artists")
    {

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("artists"),
         playlists_ref . playlist_column_record() . artists_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . artists_);

    }

    //
    else if(column_order_list_it == "bit_depth")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . bit_depth_));

      // Appends a track # column.
      append_column(*new_column);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . bit_depth_);

    }

    //
    else if(column_order_list_it == "bit_rate")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . bit_rate_));

      // Appends a track # column.
      append_column(*new_column);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . bit_rate_);

    }

    //
    else if(column_order_list_it == "channels")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . channels_));

      // Appends a track # column.
      append_column(*new_column);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . channels_);

    }

    //
    else if(column_order_list_it == "codec")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . codec_));

      // Appends a track # column.
      append_column(*new_column);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . codec_);

    }

    //
    else if(column_order_list_it == "date")
    {

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("date"),
         playlists_ref . playlist_column_record() . date_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . date_);

    } 

    //
    else if(column_order_list_it == "disc_number")
    { 

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("disc_number"),
         playlists_ref . playlist_column_record() . disc_number_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . disc_number_int_);

    }

    //
    else if(column_order_list_it == "disc_total")
    { 

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("disc_total"),
         playlists_ref . playlist_column_record() . disc_total_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . disc_total_int_);


    }

    //
    else if(column_order_list_it == "genres")
    {

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("genres"),
         playlists_ref . playlist_column_record() . genres_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . genres_);

    }
    
    //
    else if(column_order_list_it == "length")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . length_));

      // Appends a track # column.
      append_column(*new_column);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . length_);

    }

    //
    else if(column_order_list_it == "mime")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . mime_));

      // Appends a track # column.
      append_column(*new_column);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . mime_);

    }

    //
    else if(column_order_list_it == "sample_rate")
    {

      // 
      new_column
        = Gtk::manage(new Gtk::TreeViewColumn
            (title, playlists_ref . playlist_column_record() . sample_rate_));

      // Appends a track # column.
      append_column(*new_column);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . sample_rate_);

    }

    //
    else if(column_order_list_it == "title")
    {

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("title"),
         playlists_ref . playlist_column_record() . title_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . title_);

    }

    //
    else if(column_order_list_it == "track_number")
    {

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("track_number"),
         playlists_ref . playlist_column_record() . track_number_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . track_number_int_);

    }

    //
    else if(column_order_list_it == "track_total")
    {

      // Appends
      column_num = append_column_editable
        (playlists_ref . Find_Column_Title("track_total"),
         playlists_ref . playlist_column_record() . track_total_);



      // 
      new_column = get_column(column_num - 1);



      // Sets the sort column for the column.
      new_column -> set_sort_column(playlists_ref . playlist_column_record()
                                                      . track_total_int_);

    } 



    // 
    new_column -> set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);



    // 
    const char* column_name = column_order_list_it . c_str();

    // 
    if(playlists_ref . Editable_Column(column_name))
    {

      //
        Gtk::CellRenderer* row_cell_renderer
          = get_column_cell_renderer(column_num - 1);



      // 
      row_cell_renderer -> signal_editing_started()
        . connect(sigc::mem_fun(*this, &Playlist::Editing));

      // 
      row_cell_renderer -> signal_editing_canceled()
        . connect(sigc::mem_fun(*this, &Playlist::Editing_Canceled));

    }



    // 
    string column_size_config_str
      = "gui.playlist.view.";

    // 
    column_size_config_str += playlist_view_name_;

    // 
    column_size_config_str += ".columns." + column_order_list_it + ".size";

    // 
    new_column -> set_fixed_width(config() . get(column_size_config_str));



    // Makes the current column's headers reorderable.
    new_column -> set_reorderable();

    // Makes the column header resizeable.
    new_column -> set_resizable(true);



    // 
    new_column -> get_button() -> add_events(Gdk::BUTTON_PRESS_MASK);

    // Callback for right when the header is clicked
    new_column -> get_button() -> signal_pressed()
      . connect(sigc::mem_fun(*this,
                              &Playlist::On_Button_Press_Event_Column_Header));

    // Callback for when after the header is clicked.
    new_column -> signal_clicked()
                    . connect(sigc::mem_fun(*this, &Playlist::Header_Clicked));

  }




  // Sets a RefPtr to the TreeView's selection.
  playlist_treeselection_ = this -> get_selection();



  debug("After playlist_treesection assignment");



  // Assigns the TreeView's selection changed function.
  signal_cursor_changed()
    . connect(sigc::mem_fun(*this, &Playlist::On_Cursor_Changed));

  // Assigns the TreeView's selection changed function.
  selection_conn_ = (playlist_treeselection_ -> signal_changed()
    . connect(sigc::mem_fun(*this, &Playlist::On_Selection_Changed)));



  // Sets the TreeView's TreeSelection to be able to select multiple rows.
  playlist_treeselection_ -> set_mode(Gtk::SELECTION_MULTIPLE);



  // Creates a new PlaylistMenu for the playlist TreeView.
  menu_
    = new PlaylistMenu(base_ref, *this,
                       playlists_ref, playlist_treestore_name . c_str());



  // 
  setting_name = "gui.playlist.view.";

  // 
  setting_name += playlist_view_name_;

  // 
  setting_name += ".locked";



  // 
  bool playlist_locked = config() . get(setting_name . c_str());



  // 
  menu_ -> lock_check_menu_item() . set_active(playlist_locked);



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

  // Mouse movement overload function signal connection.
  signal_motion_notify_event()
    . connect(sigc::mem_fun(*this, &Playlist::on_motion_notify_event), false);





  //            //
  // Status Bar ///////////////////////////////////////////////////////////////
  //            //

  // Adds the status_box_ to the status_frame_.
  status_frame_ -> add(*status_box_);

  // Sets status_frame_ to expand horizontally.
  status_frame_ -> set_hexpand(true);



  // 
  status_box_ -> pack_start(*name_label_event_box_, Gtk::PACK_SHRINK);

  // 
  status_box_ -> pack_start(*Gtk::manage(new Gtk::Separator), Gtk::PACK_SHRINK);

  // 
  status_box_ -> pack_start(*progress_bars_box_, Gtk::PACK_EXPAND_WIDGET);

  // 
  status_box_ -> pack_start(*Gtk::manage(new Gtk::Separator), Gtk::PACK_SHRINK);

  // 
  status_box_ -> pack_start(*row_count_label_, Gtk::PACK_SHRINK);



  // 
  name_label_event_box_ -> add(*name_label_);



  // 
  progress_bars_box_ -> pack_end(*progress_bar_, Gtk::PACK_SHRINK);

  // 
  progress_bars_box_ -> pack_end(*copy_progress_bar_, Gtk::PACK_SHRINK);



  // 
  progress_bars_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // 
  name_label_ -> set_text(playlist_treestore_name);



  // 
  name_label_event_box_ -> set_margin_left(3);

  // 
  name_label_event_box_ -> set_margin_right(3);



  // 
  row_count_label_ -> set_margin_left(3);

  // 
  row_count_label_ -> set_margin_right(3);



  // 
  copy_progress_bar_ -> set_fraction(0.0);

  // 
  progress_bar_ -> set_fraction(0.0);



  // 
  copy_progress_bar_ -> set_show_text(true);

  // 
  progress_bar_ -> set_show_text(true);



  // 
  copy_progress_bar_ -> set_valign(Gtk::ALIGN_CENTER);

  // 
  progress_bar_ -> set_valign(Gtk::ALIGN_CENTER);



  // 
  copy_progress_bar_ -> set_hexpand(true);

  // 
  progress_bar_ -> set_hexpand(true);



  // 
  copy_progress_bar_ -> set_margin_top(3);

  // 
  copy_progress_bar_ -> set_margin_bottom(3);

  // 
  copy_progress_bar_ -> set_margin_left(3);

  // 
  copy_progress_bar_ -> set_margin_right(3);

  // 
  progress_bar_ -> set_margin_top(3);

  // 
  progress_bar_ -> set_margin_bottom(3);

  // 
  progress_bar_ -> set_margin_left(3);

  // 
  progress_bar_ -> set_margin_right(3);



  // 
  copy_progress_bar_
    -> set_tooltip_text("Displays the progress of the tracks being copied.");

  // 
  progress_bar_
    -> set_tooltip_text("Displays the progress of the action occurring.");

  // 
  name_label_event_box_ -> set_tooltip_text
    ("Playlist name of the playlist active in the playlist view.");

  // 
  row_count_label_
    -> set_tooltip_text("Number of track rows in the active playlist " \
                        "of the playlist view.");



  // 
  progress_bar_ -> set_pulse_step(0.01);



  // 
  name_label_event_box_ -> set_events(Gdk::BUTTON_PRESS_MASK);

  // Overrides the button press event for the event_box_.
  name_label_event_box_ -> signal_button_press_event()
    . connect(sigc::mem_fun(*this, &Playlist::Rename_Playlist_Double_Click));



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



  // 
  delete copy_progress_bar_;



  // 
  delete right_click_row_tree_row_ref_;

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



  

  if(playlists() . hovering_playlist_treestore())
  {

    // 
    playlists() . hovering_playlist_treestore() -> drag_occurring() = false;

  }

  // Sets the hovering playlist treestore.
  playlists() . set_hovering_playlist_treestore(playlist_treestore());

  // 
  playlist_treestore_ -> drag_occurring() = true;

} 

bool Playlist::on_drag_drop
       (const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, 
        guint time)
{

  if(!dest_tree_row_ref_)
  {

    // 
    return false;

  }


  debug("on_drag_drop");



  // Will be set to true if the drag was done in the queue.
  static bool drag_in_queue;

  // Will be set to true if the drag was done in the queue.
  drag_in_queue = false;



  // True if a drag was started in the Playback Queue.
  if((playlists() . selected_playlist_treestore())
       == (playlists() . queue_playlist_treestore()))
  { 

    // True if the drag from the Track Playback Queue was to a playlist that
    // isn't the queue.
    if((playlists() . hovering_playlist_treestore())
         != (playlists() . queue_playlist_treestore()))
    {

      // Returns false.
      return false;

    }  

    // True if the drag is in the Playback Queue.
    else
    {

      // Sets the drag to be happening in the Playback Queue.
      drag_in_queue = true;



      // Prevents a Playback Queue drag from being drug to the top.
      if((dest_tree_row_ref_ -> get_path())
           == (playlists() . playing_row_ref() . get_path()))
      {

        // Ends the drag.
        return false;

      }


    }

  }



  // 
  if(playlists() . rebuilding_databases())
  {

    // 
    playlists() . database() . quit_rebuilding() = true;

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



  // 
  static bool different_playlists;

  // 
  different_playlists = false;



  // 
  static Playlist* source_playlist;

  // 
  source_playlist = &(playlists() . selected_playlist());



  //
  if((source_playlist -> gui_elements_it()) != (gui_elements_it()))
  {

    // 
    different_playlists = true;

  }
  


  // Sets the treestore to be unsorted.
  playlist_treestore()
    -> set_sort_column(Gtk::TreeSortable::DEFAULT_UNSORTED_COLUMN_ID, 
                       Gtk::SORT_ASCENDING);



  // Stores the TreePaths of the selected rows in a vector.
  static vector<Gtk::TreePath> selected_rows_paths;

  // Stores the TreePaths of the selected rows in a vector.
  selected_rows_paths
    = playlists() . selected_playlist() . playlist_treeselection()
                                            -> get_selected_rows();



  // True if the destination of the drug rows is before the destination row.
  static bool before_row;

  // True if the destination of the drug rows is before the destination row.
  before_row = false;



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
  static bool playing_row_present;

  // True if the playing track is in the drug rows.
  playing_row_present = false;



  // Indicates the row of the playing track if present.
  static int playing_row_position;

  // Indicates the row of the playing track if present.
  playing_row_position = 1;



  // Will hold the Tracks of each row.
  static vector<shared_ptr<Track>> selected_tracks_sptrs;

  // 
  selected_tracks_sptrs . clear();



  // Will hold the converted TreeRowReferences of the selected rows.
  static vector<Gtk::TreeRowReference> selected_rows_refs;

  // 
  selected_rows_refs . clear();



  // 
  static Glib::RefPtr<PlaylistTreeStore> selected_playlist_treestore;

  // 
  selected_playlist_treestore = playlists() . selected_playlist_treestore();



  // 
  static Glib::RefPtr<PlaylistTreeStore> hovering_playlist_treestore;

  // 
  hovering_playlist_treestore = playlists() . hovering_playlist_treestore();



  // True if the loop is on the first row in order to make a row ref to it.
  static bool first_selected_row;

  // 
  first_selected_row = true;



  // Will hold a row ref to the first moved row.
  static Gtk::TreeRowReference first_row_ref;

  // Will hold a row ref to the last moved row.
  static Gtk::TreeRowReference last_row_ref;



  // Size of the 
  static int size;

  size = selected_rows_paths . size();



  // Holds the current row number of the drug tracks in this loop.
  static int current_row_number;

  // 
  if(before_row)
  {

    // 
    current_row_number = 1;

  }

  // 
  else
  {

    // 
    current_row_number = size;

  }



  // 
  sigc::connection program_conn = Glib::signal_timeout() . connect
  (

    // 
    [this]() -> bool
    {  

      // 
      int count = 0;



      // 
      while(count < 7)
      {

        // 
        if(selected_rows_paths . empty())
        {

          // 
          if(selected_tracks_sptrs . empty())
          {

            // 
            if(selected_rows_refs . empty())
            {

              // 
              if(different_playlists)
              {

                // 
                source_playlist -> playlist_treeselection() -> unselect_all();

              }

              // True if the drag destination is before the destination row.
              if(before_row)
              {

                // Sets the selected row ref as the first row.
                playlists() . set_selected_row_ref(first_row_ref);

              }

              // True if the drag destination is after the destination row.
              else
              {

                // 
                set_cursor(Gtk::TreePath(last_row_ref . get_path()));

                // 
                playlist_treeselection_ -> select(first_row_ref . get_path(),
                                                  last_row_ref . get_path());



                // Sets the selected row ref as the last row.
                playlists() . set_selected_row_ref(last_row_ref);

              }



              // 
              hovering_playlist_treestore -> rebuild_database() = true;

              // 
              hovering_playlist_treestore -> restart_changes() = true;



              // 
              if(selected_playlist_treestore != hovering_playlist_treestore)
              {

                // 
                selected_playlist_treestore -> rebuild_database() = true;

                // 
                selected_playlist_treestore -> restart_changes() = true;

              }



              // 
              if(playlists() . database_extraction_complete())
              {

                // 
                if(!(playlists() . rebuilding_databases()))
                {

                  // 
                  playlists() . database() . Rebuild_Database();

                }

              }



              // Resets the track queue after dragging.
              playback() . Reset_Track_Queue();



              // 
              selected_playlist_treestore -> drag_occurring() = false;

              // 
              hovering_playlist_treestore -> drag_occurring() = false;

              // 
              playlists() . drag_occurred_ = true;



              // 
              return false;

            }



            // 
            Gtk::TreeIter old_row_it;

            // 
            Gtk::TreePath old_path_it;

            // 
            if(before_row)
            {

              // 
              old_path_it = (selected_rows_refs . front() . get_path());

              // 
              old_row_it
                = selected_playlist_treestore
                    -> get_iter(selected_rows_refs . front() . get_path());

            }

            // 
            else
            {

              // 
              old_path_it = (selected_rows_refs . back() . get_path());

              // 
              old_row_it
                = selected_playlist_treestore
                    -> get_iter(selected_rows_refs . back() . get_path());

            }



            // 
            selection_conn_ . block(true);

            // 
            playlists() . set_disable_on_selection_changed(true);



            // 
            if(playlist_treestore_ == selected_playlist_treestore)
            {

              // 
              playlist_treeselection_ -> unselect(old_path_it);

            }

            // 
            selected_playlist_treestore -> erase(old_row_it);



            // 
            selection_conn_ . block(false);
  
            // 
            playlists() . set_disable_on_selection_changed(false);



            // 
            if(before_row)
            {

              // Increments the track sptrs forward it.
              selected_rows_refs . erase(selected_rows_refs . begin());

            }

            // 
            else
            {

              // Increments the track sptrs forward it.
              selected_rows_refs . pop_back();

            }



            // 
            for(auto playlists_it : playlists()())
            {

              // 
              if((playlists_it -> playlist_treestore()
                 == selected_playlist_treestore)
                 &&
                 (playlists_it -> playlist_treestore()
                 == hovering_playlist_treestore))
              {

                // 
                int progress_size = size - (selected_rows_refs . size());

                // 
                int playlist_size
                  = playlists_it -> playlist_treestore() -> children() . size();

                // 
                Glib::ustring status_str
                  = "Deleting Drug Tracks: "
                      + to_string(progress_size)
                      + " / " + to_string(size);



                // 
                playlists_it -> row_count_label()
                  . set_label(to_string(playlist_size));

                // 
                playlists_it -> progress_bar() . set_fraction(double(progress_size) / double(size));

                // 
                playlists_it -> progress_bar() . set_text(status_str);

              }

            }



            // 
            count++;



            // 
            if(count < 7)
            {

              // 
              continue;

            }

            // 
            else
            {

              // 
              return true;

            }

          }



          // Holds an iterator to the destination row.
          Gtk::TreeIter dest_row_it;

          // Holds an iterator to the new row.
          Gtk::TreeIter new_row_iter;



          // True if the destination row is not valid.
          if(!(*dest_tree_row_ref_))
          {

            // Appends a new row to an empty playlist.
            new_row_iter = hovering_playlist_treestore -> append();

          }

          // True if the destination row is valid.
          else
          {

            // Creates an iterator to the destination row.
            dest_row_it 
              = dest_tree_row_ref_ 
                  -> get_model() -> get_iter(dest_tree_row_ref_ -> get_path());



            // 
            if(before_row)
            {

              // Creates a new row before the destination row.
              new_row_iter
                = hovering_playlist_treestore -> insert(dest_row_it);

            }

            // 
            else
            {

              // Creates a new row after the destination row.
              new_row_iter
                = hovering_playlist_treestore -> insert_after(dest_row_it);

            }

          }



          // Dereferences the iterator into a new TreeRow.
          Gtk::TreeRow new_row = *new_row_iter;

          // 
          if(before_row)
          {

            // Fills the row with the Track's data.
            playlists() . Fill_Row(new_row, selected_tracks_sptrs . front());

          }

          // 
          else
          {

            // Fills the row with the Track's data.
            playlists() . Fill_Row(new_row, selected_tracks_sptrs . back());

          }



          if(playlist_treestore_ == hovering_playlist_treestore)
          {

            // 
            playlist_treeselection_ -> select(new_row_iter);



            // 
            if(first_selected_row)
            {

              // 
              grab_focus();



              // 
              if(before_row)
              {

                // 
                set_cursor(Gtk::TreePath(new_row_iter));

              }

            }

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



          // Creates a reference to the current row each time until it's the last.
          last_row_ref = Gtk::TreeRowReference(hovering_playlist_treestore, 
                                               Gtk::TreePath(new_row_iter));



          // True if the playing row is present and has already been found.
          if(playing_row_present)
          {

            // True if the row of the loop iteration is the playing track.
            if(current_row_number == playing_row_position)
            {

              // Sets playing_row_ref to the playing track's new position.
              playback() . track_queue() . front() -> row_ref() = last_row_ref;



              // Sets the playing row to not present row this
              // code is ignored now.
              playing_row_present = false;

            }

          }



          // 
          if(before_row)
          {

            // Increments the track sptrs forward it.
            selected_tracks_sptrs . erase(selected_tracks_sptrs . begin());

            // Increments the row position since it is inserted forward.
            current_row_number++;

          }

          // 
          else
          {

            // Increments the track sptrs forward it.
            selected_tracks_sptrs . pop_back();

            // Decrements the row position since they are inserted backwards.
            current_row_number--;

          }



          // 
          for(auto playlists_it : playlists()())
          {

            // 
            if((playlists_it -> playlist_treestore()
               == selected_playlist_treestore)
               &&
               (playlists_it -> playlist_treestore()
                 == hovering_playlist_treestore))
            {

              // 
              int progress_size = size - (selected_tracks_sptrs . size());

              // 
              int playlist_size
                = playlists_it -> playlist_treestore() -> children() . size();

              // 
              Glib::ustring status_str
                = "Pasting New Tracks Based On Drug Tracks: "
                    + to_string(progress_size)
                    + " / " + to_string(size);



              // 
              playlists_it -> row_count_label()
                . set_label(to_string(playlist_size));

              // 
              playlists_it -> progress_bar() . set_fraction(double(progress_size) / double(size));

              // 
              playlists_it -> progress_bar() . set_text(status_str);

            }

          }



          // 
          count++;



          // 
          if(count < 5)
          {

            // 
            continue;

          }

          // 
          else
          {

            // 
            return true;

          }

        }



        // Converts the current row path to an iterator.
        Gtk::TreeIter temp_tree_iter
          = playlists() . selected_playlist_treestore()
              -> get_iter(selected_rows_paths . front()); 

        // Converts the iterator to a row ref using its TreeStore.
        Gtk::TreeRowReference temp_row_ref
          (playlists() . selected_playlist_treestore(), selected_rows_paths . front());

        // Stores the new TreeRowReference in selected_track_sptrs.
        selected_rows_refs . push_back(temp_row_ref);



        // 
        selected_rows_paths . erase(selected_rows_paths . begin());



        // 
        if((!(playback() . Stopped()))
             && 
           ((temp_row_ref . get_path())
            == 
           (playlists() . playing_row_ref() . get_path())))
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
                                  . playlist_column_record() . track_]);

        // Pushes the retrieved Track sptr to the back of
        // selected_tracks_sptrs.
        selected_tracks_sptrs . push_back(temp_track);



        // True if the playing row is not determined to be present.
        if(!playing_row_present)
        {

          // Increments the playing row position.
          playing_row_position++;

        }



        // 
        for(auto playlists_it : playlists()())
        {

          // 
          if((playlists_it -> playlist_treestore()
               == selected_playlist_treestore)
             &&
             (playlists_it -> playlist_treestore()
               == hovering_playlist_treestore))
          {

            // 
            int progress_size
              = selected_tracks_sptrs . size();

            // 
            int playlist_size
              = playlists_it -> playlist_treestore() -> children() . size();

            Glib::ustring status_str
              = "Collecting Track Data For Drag: " + to_string(progress_size)
                  + " / " + to_string(size);



            // 
            playlists_it -> row_count_label()
              . set_label(to_string(playlist_size));

            // 
            playlists_it -> progress_bar() . set_fraction(double(progress_size) / double(size));

            // 
            playlists_it -> progress_bar() . set_text(status_str);

          }

        }



        // 
        count++;

      }



      // 
      return true;

    },



    // 
    8, Glib::PRIORITY_HIGH_IDLE

  );



  // Returns false, not perpetuating the drag drop. 
  return false;

} 

void Playlist::on_drag_end(const Glib::RefPtr<Gdk::DragContext>& context)
{ 

  debug("In on_drag_end");


 
  //  
  playlists() . drag_occurred_ = false;

  // Sets the on_selection_changed flag to false.
  playlists() . set_disable_on_selection_changed(false);

  // Sets the drag_occurring flag to false.
  playlist_treestore_ -> drag_occurring() = false;

}

bool Playlist::on_drag_failed(const Glib::RefPtr<Gdk::DragContext>& context,
                              Gtk::DragResult result)
 {

  // 
  context -> drag_finish(false, false, 0);

  // 
  context -> drop_finish(false, 0);

  // 
  context -> drag_refuse(0);

  // 
  context -> drop_reply(false, 0);



  playlists() . drag_occurred_ = false;

  // Sets the on_selection_changed flag to false.
  playlists() . set_disable_on_selection_changed(false);

  // Sets the drag_occurring flag to false.
  playlist_treestore_ -> drag_occurring() = false;



  // 
  set_reorderable(false);

  // 
  set_reorderable(true);



  // 
  return false;

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

    // Stores the x coordinate of the mouse in the row's cell.
    int x_cell;

    // Stores the y coordinate of the mouse in the row's cell.
    int y_cell;



    // Will hold the path of a row. 
    Gtk::TreePath row_tree_path;

    // Will hold a pointer to the column of the row the mouse is over.
    Gtk::TreeView::Column* row_column;


    // 
    bool get_path_result;

    // True if there is no row at the position of the mouse. 
    get_path_result
      = this -> get_path_at_pos(x,y, row_tree_path,
                                row_column, x_cell, y_cell);



    // 
    if(get_path_result)
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



      // Sets the cut playlist menu item to clickable.
      menu_ -> cut_menu_item() . set_sensitive(true);

      // Sets the copy playlist menu item to clickable.
      menu_ -> copy_menu_item() . set_sensitive(true);

      // Sets the queue playlist menu item to clickable.
      menu_ -> queue_menu_item() . set_sensitive(true);

      // Sets the delete playlist menu item to clickable.
      menu_ -> delete_menu_item() . set_sensitive(true);



      // 
      Gtk::TreeRow row
        = *(playlist_treestore_ -> get_iter(row_tree_path));



      // 
      shared_ptr<Track> row_track_sptr
        = row[playlist_column_record() . track_];



      // 
      (*right_click_row_tree_row_ref_)
        = Gtk::TreeRowReference(playlist_treestore_, row_tree_path);

      // 
      right_click_row_column_ = row_column;

      //
      right_click_x_ = x;

      //
      right_click_y_ = y;

      //
      right_click_x_cell_ = x_cell;

      //
      right_click_y_cell_ = y_cell;



      // 
      string column_title = row_column -> get_title();



      // 
      string column_name
        = playlists() . Find_Column_Name(column_title);



      // 
      if(playlists() . Editable_Column(column_name . c_str()))
      {

        // 
        if((row_track_sptr -> type()) == TrackType::NORMAL)
        {

          // Sets the edit playlist menu item to unclickable.
          menu_ -> edit_menu_item() . set_sensitive(true);

        }

        // 
        else
        {

          // 
          if((column_name == "disc_total") || (column_name == "disc_number"))
          {

            // Sets the edit playlist menu item to unclickable.
            menu_ -> edit_menu_item() . set_sensitive(true);

          }

          // 
          else
          {

            // Sets the edit playlist menu item to unclickable.
            menu_ -> edit_menu_item() . set_sensitive(false);

          }

        }

      }

      // 
      else
      {

        // Sets the edit playlist menu item to unclickable.
        menu_ -> edit_menu_item() . set_sensitive(false);

      }



      // 
      Glib::ustring active_playlist_name_str = active_playlist_name();

      // 
      if((active_playlist_name_str == "Library")
         || (active_playlist_name_str == "Queue"))
      {

        // 
        menu_ -> delete_playlist_menu_item() . set_sensitive(false);

      }

      // 
      else
      {

        // 
        menu_ -> delete_playlist_menu_item() . set_sensitive(true);

      }



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



      // 
      Glib::ustring active_playlist_name_str = active_playlist_name();

      // 
      if((active_playlist_name_str == "Library")
         || (active_playlist_name_str == "Queue"))
      {

        // 
        menu_ -> delete_playlist_menu_item() . set_sensitive(false);

      }

      // 
      else
      {

        // 
        menu_ -> delete_playlist_menu_item() . set_sensitive(true);

      }



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



        // Returns true to not propagate the event.
        return true;

      } 

      // True if there is a row at x and y and there is text.
      else
      {

        // 
        Gtk::TreeRow row
          = *(playlist_treestore_ -> get_iter(row_tree_path));



        // 
        shared_ptr<Track> row_track_sptr
          = row[playlist_column_record() . track_];


        // 
        if((row_track_sptr -> type()) == TrackType::NORMAL)
        {

          // 
          (*right_click_row_tree_row_ref_)
            = Gtk::TreeRowReference(playlist_treestore_, row_tree_path);

          // 
          right_click_row_column_ = row_column;

          //
          right_click_x_ = x;

          //
          right_click_y_ = y;

          //
          right_click_x_cell_ = x_cell;

          //
          right_click_y_cell_ = y_cell;

        }

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



         }



        // 
        grab_focus();

        // Selects the row of the button press.
        set_cursor(row_tree_path, *row_column);

        

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

          // True if the last button press was less than .25 seconds ago.
          else if(((event -> time) - previous_button_press_event_time_) < 500)
          {

            debug("Long double click editing.");



            // Sets the previous button press event time.
            previous_button_press_event_time_ = event -> time; 



            // 
            (*right_click_row_tree_row_ref_)
              = Gtk::TreeRowReference(playlist_treestore_, row_tree_path);

            // 
            right_click_row_column_ = row_column;

            //
            right_click_x_ = x;

            //
            right_click_y_ = y;

            //
            right_click_x_cell_ = x_cell;

            //
            right_click_y_cell_ = y_cell;



            // 
            Edit();



            // 
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

void Playlist::On_Button_Press_Event_Column_Header()
{

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



    // 
    if(((*drag_tree_row_ref_) . get_path()))
    {

      // Sets the cursor at the row that was selected.
      set_cursor((*drag_tree_row_ref_) . get_path(), *right_click_row_column_);

    }

  } 



  // Sends the event to the normal button release function.
  return Gtk::TreeView::on_button_release_event(event);

}

bool Playlist::on_motion_notify_event(GdkEventMotion* motion_event)
{

  // 
  if(playlist_treestore_ -> drag_occurring())
  {

    // Calls the normal montion_notify event if a drag is already occurring.
    return Gtk::TreeView::on_motion_notify_event(motion_event);

  }

  // 
  if(clipboard_event_)
  {

    // Calls the normal montion_notify event if a drag is already occurring.
    return Gtk::TreeView::on_motion_notify_event(motion_event);

  }



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
    if(playlist_treestore_ -> drag_occurring())
    { 
  
      // Calls the normal montion_notify event if a drag is already occurring.
      return Gtk::TreeView::on_motion_notify_event(motion_event);

    } 



    // Will hold the TargetList for the playlist.
    Glib::RefPtr<Gtk::TargetList> playlist_targetlist;

    // Stores a RefPtr to the drag destiation TargetList.
    playlist_targetlist = drag_dest_get_target_list();



    // Sets the playlist as a drag occurring.
    playlist_treestore_ -> drag_occurring() = true;

    // Disables the selection change flag.
    playlists() . set_disable_on_selection_changed(true);



    // Starts a drag event.
    drag_begin(playlist_targetlist, Gdk::DragAction::ACTION_MOVE, 1, 
               (GdkEvent*)(motion_event), x, y); 



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
        = row[playlists() . playlist_column_record() . track_];



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
  status_bar() . set_selected_time_label(time_str_ptr -> c_str());

  // 
  status_bar() . set_selected_rows_count_label(row_count);

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



  debug("Before loop");



  // Iterates through the the playlist treestores.
  for(auto playlist_treestores_it
        = playlists() . playlist_treestores() . begin();
      playlist_treestores_it != playlists() . playlist_treestores() . end();
      playlist_treestores_it++)
   {

    // True if the current playlist RadioMenuItem is selected.
    if((*playlist_menu_radio_menu_items_it) -> get_active())
    {

      // 
      string playlist_name = (*playlist_treestores_it) -> get_name();



      // Sets the current playlist to the label of change_playlist_menu_item.
      menu_ -> change_playlist_menu_item()
        . set_label("Playlist: " + playlist_name);



      // 
      name_label_ -> set_text(playlist_name);



      // Sets the playlist treestore as the current one of the iteration.
      set_playlist_treestore((*playlist_treestores_it));



      debug("After set_playlist_treestore");



      // 
      string setting_name = "gui.playlist.view.";

      // 
      setting_name += playlist_view_name_;

      // 
      setting_name += ".active";



      // 
      config() . set(setting_name . c_str(), playlist_name);



      // 
      config() . write_file();



      // Breaks out of the loop.
      break;

    }



    // Increments the it of the playlist menu's RadioMenuItems.
    playlist_menu_radio_menu_items_it++;

  }



  // Sets the row count of the currently active playlist treestore.
  row_count_label_
    -> set_text(to_string(playlist_treestore_ -> children() . size()));



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
  playlists() . rebuild_databases() = true;

  // 
  playlist_treestore_ -> rebuild_database() = true;

  // 
  playlist_treestore_ -> restart_changes() = true;



  // 
  if(playlists() . database_extraction_complete())
  {

    // 
    if(!(playlists() . rebuilding_databases()))
    {

      // 
      playlists() . database() . Rebuild_Database();

    }

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

void Playlist::On_Cursor_Changed()
{

  // Will hold the path of a row. 
  Gtk::TreePath row_tree_path;



  // 
  get_cursor(row_tree_path, right_click_row_column_);



  if(!row_tree_path)
  {

    // 
    return;

  }



  // 
  Gtk::TreeRow row
    = *(playlist_treestore_ -> get_iter(row_tree_path));



  // 
  if(row_tree_path)
  {

    // 
    shared_ptr<Track> row_track_sptr
      = row[playlist_column_record() . track_];

    // 
    (*right_click_row_tree_row_ref_)
      = Gtk::TreeRowReference(playlist_treestore_, row_tree_path);

    // 
    if(!(*right_click_row_tree_row_ref_))
    {

      return;

    } 

  }

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

  // True if rows are being deleted.
  else if(playlist_treestore_ -> deleting())
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
  else if(((playlist_treestore() -> children() . size()) == 0)
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
      = first_selected_row[playlist_column_record() . track_];



  // Stores a reference to selected row's track.
  Track &new_created_track = *temp_track_ptr;

  // Updates the TagViews.
  gui() . Update_Tagview("Selected", new_created_track);



  // Loads cover art for selected track.
  string filename_string = new_created_track . filename();

  // Sets the cover art of the selected track.
  gui() . Load_Cover_Art(filename_string);

}

void Playlist::Rename_Playlist()
{

  // 
  rename_playlist_dialog_ -> Run();

}

bool Playlist::Rename_Playlist_Double_Click(GdkEventButton* event)
{ 

  // Is true if the StatusBar was double-clicked.
  if((event -> type) == GDK_2BUTTON_PRESS)
  {

    // 
    rename_playlist_dialog_ -> Run();

  }



  // 
  return false;

}





//                  //
// Row Manipulators ///////////////////////////////////////////////////////////
//                  //

void Playlist::Copy_Selected_Rows(bool cut)
{

  // 
  static mutex copying_mutex;



  // 
  if(playlist_treestore_ -> drag_occurring())
  {

    // 
    return;

  }



  // 
  if((!(playlist_treestore_ -> deleting()))
       && (!(copying_mutex . try_lock())))
  {

    // 
    return;

   }



  // True if the playlist's treestore is empty.
  if((playlist_treestore() -> children() . size()) <= 0)
  {

    // 
    copying_mutex . unlock();



    // Exits the function.
    return;

  }



  // Sets the clipboard event flag as true.
  clipboard_event_ = true;



  // Creates of vector of the paths of all of the selected rows.
  static vector<Gtk::TreeModel::Path> selected_rows;

  // 
  selected_rows = playlist_treeselection_ -> get_selected_rows();

  // Gets an iterator to to the beginning of the selected rows.
  static vector<Gtk::TreeModel::Path>::iterator selected_rows_it;

  // 
  selected_rows_it = selected_rows . begin();

  // 
  static int row_count;

  row_count = 1;



  // True if there are no selected rows.
  if((selected_rows . size()) < 1)
  {

    // 
    copying_mutex . unlock();

    // 
    clipboard_event_ = false;



    // Exits the function.
    return;

  } 



  // 
  copy_progress_bar_ -> show_now();



  // Clears the clipboard.
  playlists() . clipboard_tracks() . clear();



  // 
  for(auto playlist_combobox_ptr : (playlist_comboboxes()()))
  {

    // 
    playlist_combobox_ptr -> box() . set_sensitive(false);

  }

  // 
  for(auto playlist_ptr : playlists()())
  {

    // 
    playlist_ptr -> menu() . change_playlist_menu_item()
      . set_sensitive(false);

  }



  // 
  sigc::connection program_conn = Glib::signal_timeout() . connect
  (

    // 
    [this, cut]() -> bool
    {  

      // True if there are no selected rows.
      if(selected_rows_it == (selected_rows . end()))
      {

        // 
        copying_mutex . unlock();

        // 
        clipboard_event_ = false;



        // 
        copy_progress_bar_ -> hide();



        if(cut)
        {

          // 
          Delete_Selected_Rows();

        }



        // 
        for(auto playlist_combobox_ptr : playlist_comboboxes()())
        {

          // 
          playlist_combobox_ptr -> box() . set_sensitive(true);

        }

        // 
        for(auto playlist_ptr : playlists()())
        {

          // 
          playlist_ptr -> menu() . change_playlist_menu_item()
            . set_sensitive(true);

        }





        // Exits the function.
        return false;

      }  



      // 
      int count = 0;



      // Iterates through the selected rows. 
      while((selected_rows_it != (selected_rows . end())) && (count < 35))
      { 

        // An iterator used for converting the selected row path to an iterator.
        Gtk::TreeModel::iterator selected_row_it
          = playlist_treestore() -> get_iter(*(selected_rows_it));



        // A new row for holding the dereferenced treestore iterator.
        Gtk::TreeRow row;

        // Dereferences the treestore iterator into a row.
        row = *selected_row_it;



        // Creates a copy of the current row's Track shared_ptr.
        shared_ptr<Track> temp_track_sptr
          = (row[playlist_column_record() . track_]);

        // Pushes the Track sptr to the back of the clipboard tracks.
        playlists() . clipboard_tracks() . push_back(temp_track_sptr);



        // Iterates the selected rows it.
        selected_rows_it++;

        // 
        count++;

        // 
        row_count++;



        // 
        copy_progress_bar_
          -> set_fraction(row_count / double(selected_rows . size()));

        string progress_bar_text_str
          = "Copying: " + to_string(row_count)
              + " / " + to_string(selected_rows . size());

        // 
        copy_progress_bar_ -> set_text(progress_bar_text_str);

       }



      // 
      return true;

    },



    // 
    5, Glib::PRIORITY_HIGH_IDLE

  );

}  

void Playlist::Cut_Selected_Rows()
{

  // True if the Playlist's treestore is empty.
  if((playlist_treestore() -> children().size()) <= 0)
  {

    // Exits the function.
    return;

  }



  // 
  if(playlist_treestore_ -> drag_occurring())
  {

    // 
    return;

  }



  // Copies the selected rows.
  Copy_Selected_Rows(true);

}

void Playlist::Delete_Selected_Rows()
{

  // 
  static mutex deleting_rows_mutex;



  // 
  if(playlist_treestore_ -> drag_occurring())
  {

    // 
    return;

  }



  // 
  if((!(deleting_rows_mutex . try_lock()))
       && (!(playlist_treestore_ -> deleting())))
  {

    // 
    return;
  
  }

  // 
  playlist_treestore_ -> deleting() = true;

  // 
  playlist_treestore_ -> pause_appending() = true;

  //
  clipboard_event_ = true;



  //
  playlists() . set_selected_row_ref(Gtk::TreeRowReference());



  // 
  vector<Gtk::TreeModel::Path> selected_rows_paths_vector
    = playlist_treeselection_ -> get_selected_rows();

  // 
  static list<Gtk::TreeModel::Path> selected_rows_paths;

  // 
  selected_rows_paths
    . assign(selected_rows_paths_vector . begin(),
             selected_rows_paths_vector . end());



  // 
  static list<Gtk::TreeRowReference> selected_rows_refs;

  // 
  selected_rows_refs . clear();



  //
  static int total_tracks;

  // 
  total_tracks = selected_rows_paths . size();



  // 
  static int track_count;

  // 
  track_count = 0;



  // 
  static bool restart_playback_if_queue_not_empty;

  // 
  restart_playback_if_queue_not_empty = false;


  // 
  static vector<int> ids;

  // 
  ids . clear();



  // 
  sigc::connection program_conn = Glib::signal_timeout() . connect
  (

    // 
    [this]() -> bool
    {  

      // 
      int loop_count = 0;



      // 
      while(loop_count < 10)
      {

        // 
        if(selected_rows_paths . empty())
        {

          // 
          if(selected_rows_refs . empty())
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

            // 
            else
            {

              //
              playback() . Reset_Track_Queue();  

            }



            // 
            selection_conn_ . block(true);

            // 
            playlists() . set_disable_on_selection_changed(true);



            // 
            playlist_treeselection_ -> unselect_all();



            // 
            selection_conn_ . block(false);
  
            // 
            playlists() . set_disable_on_selection_changed(false);



            // 
            Glib::ustring playlist_status
              = "No Playlists Modifications Occurring";


            // 
            for(auto playlists_it : playlists()())
            {

              // 
              if(playlist_treestore_ == playlists_it -> playlist_treestore())
              {

                // 
                playlists_it -> progress_bar() . set_text(playlist_status);

                // 
                playlists_it -> progress_bar() . set_fraction(1);



                // Sets the row count of the currently active playlist treestore.
                playlists_it -> row_count_label()
                  . set_text(to_string(playlist_treestore_ -> children() . size()));

              }

            }



            // Sets the selected time label to 0.
            Add_Selected_Tracks_Times();



            // 
            if(playlists() . rebuilding_databases())
            {

              // 
              playlists() . rebuild_databases() = true;

              // 
              playlist_treestore_ -> rebuild_database() = true;

              // 
              playlist_treestore_ -> restart_changes() = true;



              // 
              if(playlists() . database_extraction_complete())
              {

                // 
                if(!(playlists() . rebuilding_databases()))
                {

                  //
                  clipboard_event_ = false;

                  // 
                  playlist_treestore_ -> deleting() = false;

                  // 
                  playlist_treestore_ -> pause_appending() = false;

                  // 
                  deleting_rows_mutex . unlock();



                  // 
                  playlists() . database() . Rebuild_Database();

                }

              }

            }

            // 
            else
            {

              // 
              string playlist_name_str = playlist_treestore_ -> get_name();

              // 
              playlists() . database() . Delete_Rows(playlist_name_str . c_str(), ids);

            }



            //
            clipboard_event_ = false;

            // 
            playlist_treestore_ -> deleting() = false;

            // 
            playlist_treestore_ -> pause_appending() = false;

            // 
            deleting_rows_mutex . unlock();



            // 
            return false;

          }



          // 
          if(track_count == total_tracks)
          {

            // 
            track_count = 1;



            // 
            for(auto playlists_it : playlists()())
            {

              // 
              playlists_it -> selection_conn_ . block(true);

              // 
              playlists() . set_disable_on_selection_changed(true);



              // 
//              playlists_it -> playlist_treeselection() -> unselect_all();



              // 
              playlists_it -> selection_conn_ . block(false);
  
              // 
              playlists() . set_disable_on_selection_changed(false);

            }

          }



          // 
          Gtk::TreeRowReference row_ref = selected_rows_refs . front();



          // Skips deleting the row if it's the deleted queue row.
          if(!row_ref)
          {

            // 
            continue;

          }



          // 
          Gtk::TreeModel::iterator selected_rows_it
            = playlist_treestore() -> get_iter(row_ref . get_path());



          // 
          selection_conn_ . block(true);

          // 
          playlists() . set_disable_on_selection_changed(true);



          // 
          playlist_treestore_ -> erase(selected_rows_it);



          // 
          selection_conn_ . block(false);
  
          // 
          playlists() . set_disable_on_selection_changed(false);



          // 
          selected_rows_refs . pop_front();



          // 
          Glib::ustring playlist_status
            = "Deleting Track Rows: " + to_string(track_count)
                + " / " + to_string(total_tracks);

          // 
          double completion_fraction = track_count / double(total_tracks);


          // 
          for(auto playlists_it : playlists()())
          {

            // 
            if(playlist_treestore_ == playlists_it -> playlist_treestore())
            {

              // 
              playlists_it -> progress_bar() . set_text(playlist_status);

              // 
              playlists_it -> progress_bar() . set_fraction(completion_fraction);



              // Sets the row count of the currently active playlist treestore.
              playlists_it -> row_count_label()
                . set_text(to_string(playlist_treestore_ -> children()
                                       . size()));

            }

          }



          // 
          loop_count++;

          // 
          track_count++;



          // 
          return true;

        }



        // 
        auto row_path = selected_rows_paths . front();

        // 
        selected_rows_paths . pop_front();

       

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
        if(!row_path)
        {

          // 
          continue;

        }



        // 
        Gtk::TreeRowReference row_ref(playlist_treestore_, row_path);



        // Skips deleting the row if it's the deleted queue row.
        if(!row_ref)
        {

          // 
          continue;

        }



        // 
        selected_rows_refs . push_back(row_ref);



        // 
        Gtk::TreeModel::iterator selected_rows_it
            = playlist_treestore() -> get_iter(row_ref . get_path());

        // 
        Gtk::TreeRow current_row = *selected_rows_it;



        // 
        ids . push_back(int(current_row[playlist_column_record() . id_]));



        // 
        loop_count++;

        // 
        track_count++;



        // 
        Glib::ustring playlist_status
          = "Acquiring Track Data To Delete: " + to_string(track_count)
              + " / " + to_string(total_tracks);

        // 
        double completion_fraction = track_count / double(total_tracks);


        // 
        for(auto playlists_it : playlists()())
        {

          // 
          if(playlist_treestore_ == playlists_it -> playlist_treestore())
          {

            // 
            playlists_it -> progress_bar() . set_text(playlist_status);

            // 
            playlists_it -> progress_bar() . set_fraction(completion_fraction);



            // Sets the row count of the currently active playlist treestore.
            playlists_it -> row_count_label()
              . set_text(to_string(playlist_treestore_ -> children() . size()));

          }

        }

      }



      // 
      return true;

    },



    // 
    2, Glib::PRIORITY_HIGH_IDLE

  );

}

void Playlist::Edit()
{

  //
  Gtk::CellRenderer* row_cell_renderer
    = get_column_cell_renderer
        (Find_Column_Position(Active_Column_Name() . raw() . c_str()));



  // 
  Gtk::TreeIter row_iter
    = playlist_treestore_
        -> get_iter(right_click_row_tree_row_ref_ -> get_path());



  // 
  Gtk::TreeRow row = *row_iter;



  // 
  shared_ptr<Track> track_sptr = row[playlist_column_record() . track_];



  // 
  if(track_sptr -> type() == TrackType::NORMAL)
  {

    // 
    if(!(playlists() . Editable_Column(Active_Column_Name() . c_str())))
    {

      // 
      return;

    }

  }

  // 
  else
  {

    // 
    if((Active_Column_Name() == "disc_number")
         || (Active_Column_Name() == "disc_total"))
    {

    }

    // 
    else
    {

      // 
      return;

    }

  }

  

  // 
  if(Active_Column_Name() == "artists")
  {

    // 
    int pos = 1;

    // 
    Glib::ustring artists_ustr;



    // 
    for(auto artist : track_sptr -> artists())
    {

      // 
      if(pos == 1)
      {

        // 
        artists_ustr = *artist;

      }

      // 
      else
      { 

        // 
        artists_ustr += "; " + *artist;

      }



      // 
      pos++;

    }



    // 
    row[playlist_column_record() . artists_]  = artists_ustr;

  }

  // 
  else if(Active_Column_Name() == "album_artists")
  {

    // 
    int pos = 1;

    // 
    Glib::ustring album_artists_ustr;



    // 
    for(auto album_artist : track_sptr -> album_artists())
    {

      // 
      if(pos == 1)
      {

        // 
        album_artists_ustr = *album_artist;

      }

      // 
      else
      { 

        // 
        album_artists_ustr += "; " + *album_artist;

      }



      // 
      pos++;

    }



    // 
    row[playlist_column_record() . album_artists_]  = album_artists_ustr;

  }

  // 
  else if(Active_Column_Name() == "genres")
  { 

    // 
    int pos = 1;

    // 
    Glib::ustring genres_ustr;



    // 
    for(auto genre : track_sptr -> genres())
    {

      // 
      if(pos == 1)
      {

        // 
        genres_ustr = *genre;

      }

      // 
      else
      { 

        // 
        genres_ustr += "; " + *genre;

      }



      // 
      pos++;

    }



    // 
    row[playlist_column_record() . genres_]  = genres_ustr;

  }



  // 
  set_cursor(right_click_row_tree_row_ref_ -> get_path(),
             *right_click_row_column_, *row_cell_renderer, true);

}

void Playlist::Editing(Gtk::CellEditable* editable, const Glib::ustring& path)
{

  // 
  editable -> signal_editing_done()
    . connect(sigc::mem_fun
        (*this, &Playlist::Editing_Finished));

}

void Playlist::Editing_Canceled()
{

  // 
  editing_canceled_ = true;

}

void Playlist::Editing_Finished()
{ 

  // 
  if(editing_canceled_)
  {

    // 
    editing_canceled_ = false;



    // 
    return;

  }



  // 
  Gtk::TreeIter row_iter
    = playlist_treestore_
        -> get_iter(right_click_row_tree_row_ref_ -> get_path());



  // 
  Gtk::TreeRow row = *row_iter;



  // 
  shared_ptr<Track> track_sptr = row[playlist_column_record() . track_];

  

  // 
  if((Active_Column_Name()) == "album")
  {

    // 
    Glib::ustring album_ustr = row[playlist_column_record() . album_];



    // 
    track_sptr -> set_album(album_ustr);

  }

  // 
  else if((Active_Column_Name()) == "artists")
  {

    // 
    track_sptr -> clear_artists();



    // 
    Glib::ustring artists_ustr = row[playlist_column_record() . artists_];

    // 
    track_sptr
      -> set_artists(track_sptr -> Multiple_Values_Tag_Decode(artists_ustr));



    // 
    Glib::ustring* conv_artist_ustr_ptr = track_sptr -> artists_string();

    // 
    row[playlist_column_record() . artists_] = *conv_artist_ustr_ptr;



    // 
    delete conv_artist_ustr_ptr;

  }

  // 
  else if((Active_Column_Name()) == "album_artists")
  {

    // 
    Glib::ustring album_artists_ustr
      = row[playlist_column_record() . album_artists_];

    // 
    track_sptr -> set_album_artists
      (track_sptr -> Multiple_Values_Tag_Decode(album_artists_ustr));



    // 
    Glib::ustring* conv_album_artists_ustr_ptr
      = track_sptr -> album_artists_string();

    // 
    row[playlist_column_record() . album_artists_]
      = *conv_album_artists_ustr_ptr;



    // 
    delete conv_album_artists_ustr_ptr;

  }

  // 
  else if((Active_Column_Name()) == "date")
  {

    // 
    Glib::ustring date_ustr
      = row[playlist_column_record() . date_];



    // 
    int date_int = 0;

    // 
    if(!date_ustr . empty())
    {

      // 
      date_int = stoi(date_ustr);

    }



    // 
    row[playlist_column_record() . date_int_] = date_int;


    // 
    track_sptr -> set_date(date_int);

  }

  // 
  else if((Active_Column_Name()) == "disc_number")
  {

    // 
    Glib::ustring disc_number_ustr
      = row[playlist_column_record() . disc_number_];



    // 
    int disc_number_int = 0;

    // 
    if(!disc_number_ustr . empty())
    {

      // 
      disc_number_int = stoi(disc_number_ustr);

    }



    // 
    row[playlist_column_record() . disc_number_int_] = disc_number_int;


    // 
    track_sptr -> set_disc_number(disc_number_int);

  }

  // 
  else if((Active_Column_Name()) == "disc_total")
  {

    // 
    Glib::ustring disc_total_ustr
      = row[playlist_column_record() . disc_total_];



    // 
    int disc_total_int = 0;

    // 
    if(!disc_total_ustr . empty())
    {

      // 
      disc_total_int = stoi(disc_total_ustr);

    }



    // 
    row[playlist_column_record() . disc_total_int_] = disc_total_int;


    // 
    track_sptr -> set_disc_total(disc_total_int);

  }

  // 
  else if((Active_Column_Name()) == "genres")
  {

    // 
    Glib::ustring genres_ustr = row[playlist_column_record() . genres_];

    // 
    track_sptr -> set_genres
      (track_sptr -> Multiple_Values_Tag_Decode(genres_ustr));



    // 
    Glib::ustring* conv_genres_ustr_ptr = track_sptr -> genres_string();

    // 
    row[playlist_column_record() . genres_] = *conv_genres_ustr_ptr;



    // 
    delete conv_genres_ustr_ptr;

  }

  // 
  else if((Active_Column_Name()) == "title")
  {

    // 
    Glib::ustring title_ustr = row[playlist_column_record() . title_];



    // 
    track_sptr -> set_title(title_ustr);

  }

  // 
  else if((Active_Column_Name()) == "track_number")
  {

    // 
    Glib::ustring track_number_ustr
      = row[playlist_column_record() . track_number_];



    // 
    int track_number_int = 0;

    // 
    if(!track_number_ustr . empty())
    {

      // 
      track_number_int = stoi(track_number_ustr);

    }



    // 
    row[playlist_column_record() . track_number_int_] = track_number_int;


    // 
    track_sptr -> set_track_number(track_number_int);

  }

  // 
  else if((Active_Column_Name()) == "track_total")
  {

    // 
    Glib::ustring track_total_ustr
      = row[playlist_column_record() . track_total_];



    // 
    int track_total_int = 0;

    // 
    if(!track_total_ustr . empty())
    {

      // 
      track_total_int = stoi(track_total_ustr);

    }



    // 
    row[playlist_column_record() . track_total_int_] = track_total_int;



    // 
    track_sptr -> set_track_total(track_total_int);

  }



  // 
  const char* active_column_name_c_str = Active_Column_Name() . c_str();

  // 
  metadata() . Write_Tag(active_column_name_c_str, track_sptr);



  // 
  playlists() . rebuild_databases() = true;

  // 
  playlist_treestore_ -> rebuild_database() = true;

  // 
  playlist_treestore_ -> restart_changes() = true;



  // 
  if(playlists() . database_extraction_complete())
  {

    // 
    if(!(playlists() . rebuilding_databases()))
    {

      // 
      playlists() . database() . Rebuild_Database();

    }

  }

}

void Playlist::Paste_Clipboard_Rows()
{

  // 
  static mutex pasting_mutex;



  // 
  if(!(pasting_mutex . try_lock()))
  {

    // 
    return;

  }



  // 
  if(playlists() . rebuilding_databases())
  { 

    // 
    pasting_mutex . unlock();



    // 
    return;

  }



  // 
  auto pasting_playlist_treestore = playlist_treestore_;



  // 
  if((playlists() . clipboard_tracks() . empty()))
  {

    // 
    pasting_mutex . unlock();



    // 
    return;

  }



  // 
  clipboard_event_ = true;

  // 
  static auto clipboard_tracks_it = playlists() . clipboard_tracks() . begin();

  // 
  clipboard_tracks_it = playlists() . clipboard_tracks() . begin();



  //
  static int total_tracks;

  // 
  total_tracks = playlists() . clipboard_tracks() . size();



  // 
  static int track_count;

  // 
  track_count = 0;



  // 
  sigc::connection program_conn = Glib::signal_timeout() . connect
  (

    // 
    [this, pasting_playlist_treestore]() -> bool
    {  

      // 
      int loop_count = 0;



      // 
      while(loop_count < 1)
      {

        // 
        selection_conn_ . block(true);



        // 
        if(clipboard_tracks_it == playlists() . clipboard_tracks() . end())
        {

          // 
          if((pasting_playlist_treestore == playlists() . selected_playlist_treestore())
               ||
             (pasting_playlist_treestore == playlists() . playing_playlist_treestore()))
          { 

            // Resets the track queue.
            playback() . Reset_Track_Queue();

          } 



          // 
          playlists() . rebuild_databases() = true;

          // 
          pasting_playlist_treestore -> rebuild_database() = true;

          // 
          pasting_playlist_treestore -> restart_changes() = true;



          // 
          if(playlists() . database_extraction_complete())
          {

            // 
            if(!(playlists() . rebuilding_databases()))
            {

              // 
              playlists() . database() . Rebuild_Database();

            }

          }



          // 
          Add_Selected_Tracks_Times();



          // 
          selection_conn_ . block(false);



          // 
          pasting_mutex . unlock();

          //
          clipboard_event_ = false;



          // 
          return false;

        }



        // 
        Gtk::TreeRow row;

        // 
        Gtk::TreeIter track_iter = pasting_playlist_treestore -> append();

        // 
        row = *track_iter;

        // 
        playlists() . Fill_Row(row, *clipboard_tracks_it);



        // 
        if(playlist_treestore_ == pasting_playlist_treestore)
        {

          // 
          playlist_treeselection_ -> select(row);

        }



        // 
        clipboard_tracks_it++;

        // 
        loop_count++;

        // 
        track_count++;



        // 
        Glib::ustring playlist_status
          = "Pasting Tracks: " + to_string(track_count)
              + " / " + to_string(total_tracks);

        // 
        double completion_fraction = track_count / double(total_tracks);



        // 
        if(pasting_playlist_treestore != playlist_treestore())
        {

            // 
            progress_bar()
              . set_text("No Playlists Modifications Occurring");

            // 
            progress_bar() . set_fraction(1);

        }



        // 
        for(auto playlists_it : playlists()())
        {

          // 
          if(pasting_playlist_treestore == playlists_it -> playlist_treestore())
          {

            // 
            playlists_it -> progress_bar() . set_text(playlist_status);

            // 
            playlists_it -> progress_bar() . set_fraction(completion_fraction);



            // Sets the row count of the currently active playlist treestore.
            playlists_it -> row_count_label()
              . set_text(to_string(pasting_playlist_treestore -> children() . size()));

          }

          // 
          else
          {

            // 
            if(!(playlists_it -> Changes_Occurring()))
            {

              // 
              playlists_it -> progress_bar()
                . set_text("No Playlists Modifications Occurring");

              // 
              playlists_it -> progress_bar()
                . set_fraction(1);

            }

          }

        }

      }



      // 
      selection_conn_ . block(false);



      // 
      return true;

    },



    // 
    5, Glib::PRIORITY_HIGH_IDLE

  );

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
      = selected_row[playlist_column_record() . track_];



    // 
    for(auto playlist : playlists()())
    {

      // 
      playlist -> selection_conn_ . block(true);

    }

    // 
    playlists() . set_disable_on_selection_changed(true);



    // 
    Gtk::TreeRow new_queue_row
      = *(playlists() . queue_playlist_treestore() -> append());

    // 
    playlists() . Fill_Row(new_queue_row, row_track_sptr);



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

  }



  // 
  playlists() . database() . Clear_Playlist("Queue");



  // 
  playlists() . database() . Add_Tracks(playlists() . queue_playlist_treestore());



  // 
  playback() . Reset_Track_Queue();

}

void Playlist::Select_All_Rows()
{

  //
  clipboard_event_ = true;



  // Selects all of the rows.
  playlist_treeselection_ -> select_all();



  // 
  clipboard_event_ = false;

}





//                  //
// State Indicators ///////////////////////////////////////////////////////////
//                  //

Glib::ustring Playlist::Active_Column_Name()
{

  // 
  string column_title = right_click_row_column_ -> get_title();



  // 
  return playlists() . Find_Column_Name(column_title);

}

bool Playlist::Changes_Occurring()
{

  // 
  if(playlist_treestore_ -> deleting())
  {

    // 
    return true;

  }

  // 
  else if(playlist_treestore_ -> drag_occurring())
  {

    // 
    return true;

  }

  // 
  else if(!(playlist_treestore_ -> extraction_complete()))
  {

    // 
    return true;

  }

  // 
  else if(playlist_treestore_ -> rebuilding_database())
  {

    // 
    return true;

  }

  // 
  else if(clipboard_event_)
  {

    // 
    return true;

  }



  // 
  return false;

}

int Playlist::Find_Column_Position(const char* column_name)
{

  // 
  int count = 0;



  // 
  for(auto column : get_columns())
  {

    // 
    if((playlists() . Find_Column_Title(column_name))
         == (column -> get_title()))
    {

      // 
      return count;

    }



    // 
    count++;

  }



  // 
  return -1;

}
 
bool Playlist::Locked()
{

  return menu_ -> lock_check_menu_item() . get_active();

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

Glib::ustring Playlist::active_playlist_name()
{

  return playlist_treestore() -> get_name();

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

std::list<Glib::RefPtr<PlaylistTreeStore>>::iterator 
  Playlist::playlist_treestore_it()
{

  // 
  auto playlist_treestores_it
    = playlists() . playlist_treestores() .begin();

  // 
  for(; playlist_treestores_it != playlists() . playlist_treestores() . end();
      playlist_treestores_it++)
  {

    // 
    if((*playlist_treestores_it) == playlist_treestore_)
    {

      // 
      return playlist_treestores_it;

    }

  }



  // 
  return playlists() . playlist_treestores() . end();

}

const char* Playlist::playlist_view_name()
{

  return playlist_view_name_;

}





//            //
// Status Bar /////////////////////////////////////////////////////////////////
//            //

Gtk::ProgressBar& Playlist::copy_progress_bar()
{

  return *copy_progress_bar_;

}

Gtk::Label& Playlist::name_label()
{  

  return *name_label_;

}

Gtk::ProgressBar& Playlist::progress_bar()
{ 

  return *progress_bar_;

}

Gtk::Label& Playlist::row_count_label()
{   

  return *row_count_label_;

}





//         //
//         //
// Setters //////////////////////////////////////////////////////////////////
//         //
//         //

void Playlist::set_playlist_treestore
  (Glib::RefPtr<PlaylistTreeStore> new_treestore)
{

  // 
  debug("Before treestore set");



  // 
  new_treestore -> pause_changes() = true;

  // 
  new_treestore -> mutex() . lock();



  // 
  selection_conn_ . block(true);



  // 
  this -> set_model(new_treestore);

  // 
  playlist_treestore_ = new_treestore;



  // 
  selection_conn_ . block(false);



  // 
  new_treestore -> mutex() . unlock();

  // 
  new_treestore -> pause_changes() = false;



  // 
  debug("After treestore set");

}
