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

#include "ArtworkPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../../../Configuration/Configuration.h"

#include "../../ConfigurationGUI.h"

#include "../../ConfigurationGUIs.h"

#include "ArtworkPanelColumnRecord.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/ustring.h>

#include <gtkmm/box.h>

#include <gtkmm/frame.h>

#include <gtkmm/liststore.h>

#include <gtkmm/treeview.h>

#include <gtkmm/scrolledwindow.h>





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

ArtworkPanel::ArtworkPanel
  (Base& base_ref, ConfigurationGUI& new_config_gui,
   Gtk::TreeRow* parent_row_ptr,
   Glib::RefPtr<Gtk::ListStore> new_filename_liststore)

// Inherited Class

: Panel(base_ref, new_config_gui, "Artwork", parent_row_ptr, true)



// Filename Buttons

, add_filename_button_(Gtk::manage(new Gtk::Button))

, filename_buttons_box_(Gtk::manage(new Gtk::Box))

, remove_filename_button_(Gtk::manage(new Gtk::Button))



// Filename TreeView

, filename_treeview_(Gtk::manage(new Gtk::TreeView))

, filename_treeview_box_(Gtk::manage(new Gtk::Box))

, filename_treeview_columnrecord_(new ArtworkPanelColumnRecord)

, filename_treeview_frame_(Gtk::manage(new Gtk::Frame))

, filename_treeview_scrolled_window_(Gtk::manage(new Gtk::ScrolledWindow))

{

  //                   //
  // Filename TreeView ////////////////////////////////////////////////////////
  //                   //

  // Adds the output sink's box to the panel's box.
  box() . pack_start(*filename_treeview_box_, Gtk::PACK_EXPAND_WIDGET);

  // Adds the output sink's box to the panel's box.
  filename_treeview_box_ -> pack_start(*filename_treeview_frame_, 
                                       Gtk::PACK_EXPAND_WIDGET);

  // Adds the output sink's box to the panel's box.
  filename_treeview_frame_ -> add(*filename_treeview_scrolled_window_);

  // Adds the output sink's box to the panel's box.
  filename_treeview_scrolled_window_ -> add(*filename_treeview_);



  // Sets the margin of the filename treeview box.
  filename_treeview_box_ -> set_margin_top(2);

  // Sets the margin of the filename treeview box.
  filename_treeview_box_ -> set_margin_bottom(2);

  // Sets the margin of the filename treeview box.
  filename_treeview_box_ -> set_margin_left(2);

  // Sets the margin of the filename treeview box.
  filename_treeview_box_ -> set_margin_right(2);



  // Sets the behaviour of the scrobbable window of the filename treeview to 
  // appear automatically when needed.
  filename_treeview_scrolled_window_ -> set_policy(Gtk::POLICY_AUTOMATIC,
                                          Gtk::POLICY_AUTOMATIC);



  // 
  filename_liststore_ = new_filename_liststore;



  // 
  filename_treeview_ -> set_model(filename_liststore_);



  // 
  filename_treeview_ -> set_reorderable();



  // 
  filename_treeview_
    -> set_tooltip_text("Filenames to use when OMP is searching for front " \
                        "cover art. The first filename descendingly to have " \
                        "a match will be used. Filenames can be rearranged " \
                        "by dragging.");



  // Appends a Filename column.
  filename_treeview_
    -> append_column_editable
         ("Filename", filename_treeview_columnrecord_ -> filename_col_);





  //                  //
  // Filename Buttons /////////////////////////////////////////////////////////
  //                  //

  // Adds the output sink's box to the panel's box.
  box() . pack_end(*filename_buttons_box_, Gtk::PACK_SHRINK);



  // 
  filename_buttons_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);



  // 
  filename_buttons_box_ -> pack_end(*add_filename_button_, Gtk::PACK_SHRINK);

  // 
  filename_buttons_box_
    -> pack_start(*remove_filename_button_, Gtk::PACK_SHRINK);



  // 
  add_filename_button_ 
    -> set_image_from_icon_name("list-add-symbolic",
                                Gtk::ICON_SIZE_SMALL_TOOLBAR);

  // 
  remove_filename_button_ 
    -> set_image_from_icon_name("list-remove-symbolic",
                                Gtk::ICON_SIZE_SMALL_TOOLBAR);



  // 
  add_filename_button_ -> set_always_show_image(true);

  // 
  remove_filename_button_ -> set_always_show_image(true);



  // 
  add_filename_button_ -> set_margin_bottom(2);

  // 
  add_filename_button_ -> set_margin_left(2);

  // 
  add_filename_button_ -> set_margin_right(2);



  // 
  remove_filename_button_ -> set_margin_bottom(2);

  // 
  remove_filename_button_ -> set_margin_left(2);

  // 
  remove_filename_button_ -> set_margin_right(2);



  // 
  add_filename_button_ -> set_tooltip_text("Adds a row for a new filename.");

  // 
  remove_filename_button_
    -> set_tooltip_text("Removes the selected filename row.");



  //
  add_filename_button_ -> signal_clicked()
    . connect(sigc::mem_fun(*this, &ArtworkPanel::Add_Filename));

  //
  remove_filename_button_ -> signal_clicked()
    . connect(sigc::mem_fun(*this, &ArtworkPanel::Remove_Filename));



  // 
  filename_treeview_ -> signal_drag_end()
    . connect(sigc::mem_fun
        (*this, &ArtworkPanel::On_Filename_TreeView_Row_Drag_End_Signal));



  // 
  filename_liststore_ -> signal_row_changed()
    . connect(sigc::mem_fun(*this,
                            &ArtworkPanel::On_List_Store_Row_Changed_Signal));





  //       //
  // Other ////////////////////////////////////////////////////////////////////
  //       //

  // Adds the Apply_Saved_Value function to the ConfigGUI's list.
  new_config_gui . Add_Apply_Saved_Value_Function(*this,
                                                  &Panel::Apply_Saved_Values);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

ArtworkPanel::~ArtworkPanel()
{

  // 
  delete filename_treeview_columnrecord_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void ArtworkPanel::Add_Filename()
{

  debug("Add_Filename");


  // 
  Gtk::TreeIter new_filename_row_iter = filename_liststore_ -> append();

//  // 
//  Gtk::TreeRow new_filename_row = *new_filename_row_iter;

  // 
  Gtk::TreeViewColumn* filename_column = filename_treeview_ -> get_column(0);

  //
  Gtk::CellRenderer* current_filename_row_cell_renderer
    = filename_treeview_ -> get_column_cell_renderer(0);



  // 
  filename_treeview_
    -> set_cursor(Gtk::TreePath(new_filename_row_iter), *filename_column,
                  *current_filename_row_cell_renderer, true);



  // 
  list<string> filenames_str_list;

  // 
  for(auto liststore_it : filename_liststore_ -> children())
  {

    // 
    Glib::ustring filename_ustr
      = liststore_it[filename_treeview_columnrecord_ -> filename_col_]; 

    // 
    filenames_str_list . push_back(filename_ustr);

  }

  // 
  config() . set("gui.artwork.front_names", filenames_str_list);



  // 
  config_guis() . Mark_Unsaved_Changes(true);


}

void ArtworkPanel::Apply_Saved_Values()
{

  // 
  list<string> cover_image_filename_str_list;

  // 
  config() . get_list("gui.artwork.front_names",
                      cover_image_filename_str_list);



  //
  Gtk::CellRenderer* filename_row_cell_renderer
    = filename_treeview_ -> get_column_cell_renderer(0);

  filename_row_cell_renderer -> signal_editing_started()
    . connect(sigc::mem_fun
        (*this, &ArtworkPanel::On_Cell_Renderer_Editing_Started_Signal));



  // 
  filename_liststore_ -> clear();

  //
  for(auto filenames_it : cover_image_filename_str_list)
  {

    // 
    Gtk::TreeIter new_filename_row_iter = filename_liststore_ -> append();

    // 
    Gtk::TreeRow new_filename_row = *new_filename_row_iter;

    // 
    new_filename_row[filename_treeview_columnrecord_ -> filename_col_] = filenames_it;

  }

}

void ArtworkPanel::On_Cell_Editable_Editing_Done_Signal()
{

  debug("Editing finished!");



  // 
  list<string> filename_str_list;

  // 
  for(auto filename_row_it : filename_liststore_ -> children())
  {

    Glib::ustring filename_str
      = filename_row_it[filename_treeview_columnrecord_ -> filename_col_];
    
    // 
    filename_str_list . push_back(filename_str);

  }

  // 
  config() . set("gui.artwork.front_names", filename_str_list);



  // 
  config_guis() . Mark_Unsaved_Changes(true);

}

void ArtworkPanel::On_Cell_Renderer_Editing_Started_Signal
  (Gtk::CellEditable* editable, const Glib::ustring& path)
{

  debug("Editing started!");



  // 
  editable -> signal_editing_done()
    . connect(sigc::mem_fun
        (*this, &ArtworkPanel::On_Cell_Editable_Editing_Done_Signal));

}

void ArtworkPanel::On_Filename_TreeView_Row_Drag_End_Signal
  (const Glib::RefPtr<Gdk::DragContext>& context)
{

  // 
  list<string> filename_str_list;

  // 
  for(auto filename_row_it : filename_liststore_ -> children())
  {

    Glib::ustring filename_str
      = filename_row_it[filename_treeview_columnrecord_ -> filename_col_];
    
    // 
    filename_str_list . push_back(filename_str);

  }

  // 
  config() . set("gui.artwork.front_names", filename_str_list);



  // 
  config_guis() . Mark_Unsaved_Changes(true);

}

void ArtworkPanel::On_List_Store_Row_Changed_Signal
  (const Gtk::TreePath& path, const Gtk::TreeIter& iter)
{

  // 
  filename_treeview_ -> set_cursor(path);

  // 
  filename_treeview_ -> scroll_to_row(path);

}

void ArtworkPanel::Remove_Filename()
{

  debug("Add_Filename");



  // 
  filename_liststore_ -> erase(filename_treeview_ -> get_selection() -> get_selected());



  // 
  list<string> filenames_str_list;

  // 
  for(auto liststore_it : filename_liststore_ -> children())
  {

    // 
    Glib::ustring filename_ustr
      = liststore_it[filename_treeview_columnrecord_ -> filename_col_]; 

    // 
    filenames_str_list . push_back(filename_ustr);

  }

  // 
  config() . set("gui.artwork.front_names", filenames_str_list);



  // 
  config_guis() . Mark_Unsaved_Changes(true);


}
