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

#include "ConfigurationGUI.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Configuration/Configuration.h"

#include "../../../Base.h"

#include "../../GUI.h"

#include "../../SpinButtonScale.h"

#include "ConfigurationGUIs.h"

#include "Panels/Artwork/ArtworkPanel.h"

#include "Panels/GUI/GUIPanel.h"

#include "Panels/KeyboardShortcuts/KeyboardShortcutsPanel.h"

#include "Panels/Output/OutputPanel.h"

#include "Panels/Playback/PlaybackPanel.h"

#include "Panels/Playlist/PlaylistPanel.h"

#include "Panels/ReplayGain/ReplayGainPanel.h"

#include "Panels/Scrobbling/ScrobblingPanel.h"

#include "PanelTreeViewColumnRecord.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <iostream>

#include <glibmm/ustring.h>

#include <gtkmm/box.h>

#include <gtkmm/button.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/revealer.h>

#include <gtkmm/scrolledwindow.h>

#include <gtkmm/treepath.h>

#include <gtkmm/treestore.h>

#include <gtkmm/treeview.h>

#include <gtkmm/treeviewcolumn.h>





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

ConfigurationGUI::ConfigurationGUI
  (Base& base, ConfigurationGUIs& config_guis_ref)

// Inherited Class

: GUIElement(base, config_guis_ref())



// Action Buttons Objects

, action_buttons_box_(Gtk::manage(new Gtk::Box))

, action_buttons_frame_(Gtk::manage(new Gtk::Frame))

, load_default_values_button_(Gtk::manage(new Gtk::Button))

, save_changes_button_(Gtk::manage(new Gtk::Button))

, undo_changes_button_(Gtk::manage(new Gtk::Button))



// Panel Objects

, active_panel_box_(nullptr)

, active_panel_name_ustr_(new Glib::ustring(""))

, panel_box_(Gtk::manage(new Gtk::Box))

, panel_frame_(Gtk::manage(new Gtk::Frame))

, panel_frame_label_(Gtk::manage(new Gtk::Label))

, panel_revealer_(Gtk::manage(new Gtk::Revealer))



// Panel TreeView Objects

, panel_treeview_(Gtk::manage(new Gtk::TreeView))

, panel_treeview_box_(Gtk::manage(new Gtk::Box))

, panel_treeview_column_record_(new PanelTreeViewColumnRecord)

, panel_treeview_frame_(Gtk::manage(new Gtk::Frame))

, panel_treeview_scrolledwindow_(Gtk::manage(new Gtk::ScrolledWindow))

, panel_treeview_treestore_
    (Gtk::TreeStore::create(*panel_treeview_column_record_))

{ 

  debug("Inside ConfigGUI creation.");



  // Disables ConfigGUI functions.
  config_guis_ref . set_disable_functions(true);



  // Sets the orientation of the GUIElement's box to horizontal.
  box() . set_orientation(Gtk::ORIENTATION_HORIZONTAL);





  //                //
  // Panel TreeView ///////////////////////////////////////////////////////////
  //                //

  // Adds the panel treeview box to the GUIElement's box.
  box() . pack_start(*panel_treeview_box_, Gtk::PACK_SHRINK);

  // Adds panel_treeview_frame_ into to panel_treeview_box_.
  panel_treeview_box_ -> pack_start(*panel_treeview_frame_, Gtk::PACK_SHRINK);

  // Adds panel_treeview_scrolledwindow_ into panel_treeview_frame_.
  panel_treeview_frame_ -> add(*panel_treeview_scrolledwindow_);

  // Adds panel_treeview_ into panel_treeview_scrolledwindow_.
  panel_treeview_scrolledwindow_ -> add(*panel_treeview_);



  // Sets the TreeModel of the panel TreeView as a TreeStore.
  panel_treeview_ -> set_model(panel_treeview_treestore_);



  // Adds a new column to the TreeView named Name.
  panel_treeview_
    -> append_column("Name", panel_treeview_column_record_ -> name_);

//  // Adds a new column to the TreeView named Type.
//  panel_treeview_
//    -> append_column("Type", panel_treeview_column_record_ -> type_);



  // Makes rows active on a single click.
  panel_treeview_ -> set_activate_on_single_click(true);



  // Sets the function that is triggered when a row is clicked.
  panel_treeview_ -> signal_row_activated()
    . connect(sigc::mem_fun(*this, &ConfigurationGUI::Panel_Chooser));



  // Sets the TreeView's ScrolledWindow to only be scrollable vertically.
  panel_treeview_scrolledwindow_ -> set_policy(Gtk::POLICY_NEVER,
                                               Gtk::POLICY_AUTOMATIC);

  // Sets the minimum size of the TreeView's ScrolledWindow.
  panel_treeview_scrolledwindow_ -> set_size_request(150, 200);





  //           //
  // Panel Box ////////////////////////////////////////////////////////////////
  //           //

  // Adds panel_box_ to the GUIElement box.
  box() . pack_start(*panel_box_, Gtk::PACK_EXPAND_WIDGET);




  // Sets the start margin around the panel_box_. 
  panel_box_ -> set_margin_start(5);

  // Sets the end margin around the panel_box_. 
  panel_box_ -> set_margin_end(5);

  // Sets the top margin around the panel_box_. 
  panel_box_ -> set_margin_top(5);

  // Sets the bottom margin around the panel_box_. 
  panel_box_ -> set_margin_bottom(5);



  // Sets the orientation of the panel_box_ to vertical.
  panel_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // Adds panel_revealer_ to panel_box_.
  panel_box_ -> pack_start(*panel_revealer_, Gtk::PACK_EXPAND_WIDGET);

  // Adds panel_frame_ to panel_revealer_.
  panel_revealer_ -> add(*panel_frame_);

  // Sets the panel transition as 200.
  panel_revealer_ -> set_transition_duration(200);

  // Sets the panel transition as crossfade.
  panel_revealer_
    -> set_transition_type(Gtk::REVEALER_TRANSITION_TYPE_CROSSFADE);



  // Adds panel_frame_label_ to panel_frame_'s label.
  panel_frame_ -> set_label_widget(*panel_frame_label_);

  // Sets the text of panel_frame_label_.
  panel_frame_label_ -> set_text("No Row Selection!");



  // Used for changing font aspects.
  Pango::FontDescription panel_frame_label_font;

  // Sets the FonrDescription as being bold.
  panel_frame_label_font . set_weight(Pango::Weight::WEIGHT_BOLD);

  // Overrides the panel label's font with the new bold font.
  panel_frame_ -> get_label_widget() -> override_font(panel_frame_label_font);





  //                //
  // Action Buttons ///////////////////////////////////////////////////////////
  //                //

  // Adds action_buttons_frame_ to panel_box_.
  panel_box_ -> pack_end(*action_buttons_frame_, Gtk::PACK_SHRINK);

  // Adds action_buttons_box_ to action_buttons_frame_.
  action_buttons_frame_ -> add(*action_buttons_box_);



  // Sets the margin of the top side of the action_buttons_box_.
  action_buttons_box_ -> set_margin_top(4);

  // Sets the margin of the bottom side of the action_buttons_box_.
  action_buttons_box_ -> set_margin_bottom(4);

  // Sets the margin of the left side of the action_buttons_box_.
  action_buttons_box_ -> set_margin_left(4);

  // Sets the margin of the right side of the action_buttons_box_.
  action_buttons_box_ -> set_margin_right(4);



  // Sets the margin of the top side of the action_buttons_frame_.
  action_buttons_frame_ -> set_margin_top(3);



  // Adds undo_changes_button to the left side of the action_buttons_box_.
  action_buttons_box_ -> pack_start(*undo_changes_button_, Gtk::PACK_SHRINK);

  // Adds load_default_values_button to the center of the action_buttons_box_.
  action_buttons_box_ -> set_center_widget(*load_default_values_button_);

  // Adds undo_changes_button to the right side of the action_buttons_box_.
  action_buttons_box_ -> pack_end(*save_changes_button_, Gtk::PACK_SHRINK);



  // Sets the label of the undo_changes_button.
  undo_changes_button_ -> set_label("Undo Changes");

  // Sets the label of the load_default_values_button.
  load_default_values_button_ -> set_label("Load Default Values");

  // Sets the label of the save_changes_button.
  save_changes_button_ -> set_label("Save Changes");



  // Sets the icon of undo_changes_button_.
  undo_changes_button_ 
    -> set_image_from_icon_name("edit-undo-symbolic",
                                Gtk::ICON_SIZE_SMALL_TOOLBAR);

  // Sets the icon of save_changes_button_.
  save_changes_button_ 
    -> set_image_from_icon_name("document-save-symbolic",
                                Gtk::ICON_SIZE_SMALL_TOOLBAR);



  // Sets undo_changes_button_ to always show its image.
  undo_changes_button_ -> set_always_show_image(true);

  // Sets save_changes_button_ to always show its image.
  save_changes_button_ -> set_always_show_image(true);



  // Sets the right margin of undo_changes_button_.
  undo_changes_button_ -> set_margin_right(4);

  // Sets the left margin of save_changes_button_.
  save_changes_button_ -> set_margin_left(4);



  // Sets the clicked callback of the undo_changes_button_.
  undo_changes_button_ -> signal_clicked()
    . connect(sigc::mem_fun(config_guis_ref, 
                            &ConfigurationGUIs::Undo_Changes));

  // Sets the clicked callback of the load_default_values_button_.
  load_default_values_button_ -> signal_clicked()
    . connect(sigc::mem_fun(config_guis_ref,
                            &ConfigurationGUIs::Load_Default_Values));

  // Sets the clicked callback of the save_changes_button_.
  save_changes_button_ -> signal_clicked()
    . connect(sigc::mem_fun(config_guis_ref,
                            &ConfigurationGUIs::Save_Changes));



  //       //
  // Panel ////////////////////////////////////////////////////////////////////
  //       //

  // Creates a new GUIPanel.
  gui_panel_ = Gtk::manage(new GUIPanel(base, *this));

  // 
  artwork_panel_
    = Gtk::manage(new ArtworkPanel(base, *this, &(gui_panel_ -> row()), config_guis_ref . filename_liststore()));

  // 
  playlist_panel_
    = Gtk::manage(new PlaylistPanel(base, *this, &(gui_panel_ -> row())));

  // Creates a new .
  keyboard_shortcuts_panel_
    = Gtk::manage(new KeyboardShortcutsPanel(base, *this));

  // Creates a new OutputPanel.
  output_panel_ = Gtk::manage(new OutputPanel(base, *this));

  // Creates a new PlaybackPanel.
  playback_panel_ = Gtk::manage(new PlaybackPanel(base, *this));

  // Creates a new ReplayGainPanel.
  replay_gain_panel_
    = Gtk::manage(new ReplayGainPanel(base, *this, 
                                      &(output_panel_ -> row())));

  // Creates a new ScrobblingPanel.
  scrobbling_panel_ = Gtk::manage(new ScrobblingPanel(base, *this));



  // Expands all of the rows of the TreeView's TreeStore.
  panel_treeview_ -> expand_all();



  //
  Glib::ustring temp_active_panel_name_ustr
    = config() . get("gui.configuration.active_panel");

  // 
  *active_panel_name_ustr_ = temp_active_panel_name_ustr;

  // 
  for(auto panel_rows_it : panel_treeview_treestore_ -> children())
  {

    // Converts the iterator to a TreePath.
    const Gtk::TreeModel::Path panel_path
      = panel_treeview_treestore_ -> get_path(panel_rows_it);

    // 
    Gtk::TreeModel::iterator panel_treestore_it
      = panel_treeview_treestore_ -> get_iter(panel_path);

    // Converts the iterator into a row.
    Gtk::TreeModel::Row row = *panel_treestore_it;

    // 
    Glib::ustring current_panel_name_str
      = row[panel_treeview_column_record_ -> name_];

    // 
    if(*active_panel_name_ustr_ == current_panel_name_str)
    {

      // Sets the cursor of the TreeView to the newly created TreePath.
      panel_treeview_ -> set_cursor(panel_path);



      // Calls Panel_Chooser to set the current panel.
      Panel_Chooser(panel_path, NULL);



      // 
      return;

    }



    // 
    for(auto child_panel_rows_it : row . children())
    {
  
      // Converts the iterator to a TreePath.
      const Gtk::TreePath inner_panel_path
        = panel_treeview_treestore_ -> get_path(child_panel_rows_it);
  
      // 
      panel_treestore_it
        = panel_treeview_treestore_ -> get_iter(inner_panel_path);
  
      // Converts the iterator into a row.
      row = *panel_treestore_it;
  
      // 
      current_panel_name_str
        = row[panel_treeview_column_record_ -> name_];
  
      // 
      if(*active_panel_name_ustr_ == current_panel_name_str)
      {
  
        // Sets the cursor of the TreeView to the newly created TreePath.
        panel_treeview_ -> set_cursor(inner_panel_path);
  
  
  
        // Calls Panel_Chooser to set the current panel.
        Panel_Chooser(inner_panel_path, NULL);
 


        // 
        return;
 
      }
  
    }

  }



  // Enables ConfigGUI functions.
  config_guis_ref . set_disable_functions(false);

 }





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

ConfigurationGUI::~ConfigurationGUI()
{

  // Destroy the ColumnRecord of the panel TreeView.
  delete panel_treeview_column_record_;

  // 
  delete active_panel_name_ustr_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void ConfigurationGUI::Apply_Saved_Values()
{

  // Iterators through the list of ConfigGUI Apply_Saved_Values functions.
  for(auto functions_it : apply_saved_values_list())
  {

    // Calls the function pointer.
    (functions_it)();

  }

}

void ConfigurationGUI::Mark_Unsaved_Changes(bool new_value)
{

  // 
  load_default_values_button_
    -> set_sensitive(!(config() . get("default_values")));



  if(new_value)
  {

    // 
    save_changes_button_ -> set_sensitive(true);

    // 
    undo_changes_button_ -> set_sensitive(true);

  }

  // 
  else
  { 

    // 
    save_changes_button_ -> set_sensitive(false);

    // 
    undo_changes_button_ -> set_sensitive(false);

  }

}

void ConfigurationGUI::Panel_Chooser(const Gtk::TreePath& path,
                                     Gtk::TreeViewColumn* column)
{

  // Converts the panel TreePath to an iterator.
  Gtk::TreeModel::iterator panel_treestore_it
    = panel_treeview_treestore_ -> get_iter(path);



  // True if the iterator is valid.
  if(panel_treestore_it)
  {

    // Hides the panel.
    panel_revealer_ -> set_reveal_child(false);

    // Removes the current panel.
    panel_frame_ -> remove();



    // Converts the iterator into a row.
    Gtk::TreeModel::Row row = *panel_treestore_it;

    // Copies a string of the panel's name.
    *active_panel_name_ustr_
      = row[panel_treeview_column_record_ -> name_];

    // 
    config() . set("gui.configuration.active_panel", *active_panel_name_ustr_);

    // Copies a pointer of the panel's box.
    active_panel_box_
      = row[panel_treeview_column_record_ -> box_];



    // Adds the panel to the panel frame.
    panel_frame_ -> add(*active_panel_box_);

    // Sets panel_frame_'s label as the panel's name.
    panel_frame_label_ -> set_text(*active_panel_name_ustr_);



    // Shows all of the children in the ConfigGUI box.
    box() . show_all_children();

    // Reveals the panel.
    panel_revealer_ -> set_reveal_child(true);

  }

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

//         //
// General //////////////////////////////////////////////////////////////////
//         //

std::list<std::function<void(void)>>& 
  ConfigurationGUI::apply_saved_values_list()
{

  // 
  return apply_saved_values_list_;

}





//               //
// gtkmm Objects ////////////////////////////////////////////////////////////
//               //

PanelTreeViewColumnRecord& ConfigurationGUI::panel_treeview_column_record()
{

  return *panel_treeview_column_record_;

}

Glib::RefPtr<Gtk::TreeStore>& ConfigurationGUI::panel_treeview_treestore()
{

  return panel_treeview_treestore_;

}





//        //
// Panels ///////////////////////////////////////////////////////////////////
//        //

GUIPanel& ConfigurationGUI::gui_panel()
{ 

  return *gui_panel_;

}

OutputPanel& ConfigurationGUI::output_panel()
{

  return *output_panel_;

}

PlaybackPanel& ConfigurationGUI::playback_panel()
{ 

  return *playback_panel_;

}

PlaylistPanel& ConfigurationGUI::playlist_panel()
{

  return *playlist_panel_;

}

ReplayGainPanel& ConfigurationGUI::replay_gain_panel()
{

  return *replay_gain_panel_;

}

ScrobblingPanel& ConfigurationGUI::scrobbling_panel()
{

  return *scrobbling_panel_;

}
