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

#include "KeyboardShortcutsPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../../../Configuration/Configuration.h"

#include "../../../../../KeyboardShortcuts/KeyboardShortcuts.h"

#include "../../ConfigurationGUI.h"

#include "../../ConfigurationGUIs.h"

#include "KeyboardShortcutsPanelColumnRecord.h"





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

#include <map>





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

KeyboardShortcutsPanel::KeyboardShortcutsPanel
  (Base& base_ref, ConfigurationGUI& new_config_gui,
   Glib::RefPtr<Gtk::ListStore> new_keyboard_shortcuts_liststore)

// Inherited Class

: Panel(base_ref, new_config_gui, "Keyboard Shortcuts")



// General 

, clear_key_button_(Gtk::manage(new Gtk::Button("Clear Key")))

, keyboard_shortcuts_display_box_(Gtk::manage(new Gtk::Box))

, keyboard_shortcuts_display_inner_box_(Gtk::manage(new Gtk::Box))

, keyboard_shortcuts_key_label_(Gtk::manage(new Gtk::Label("None")))

, keyboard_shortcuts_label_(Gtk::manage(new Gtk::Label()))

, load_default_button_(Gtk::manage(new Gtk::Button("Load Default")))



// Keyboard Shortcuts TreeView

, keyboard_shortcuts_treeview_(Gtk::manage(new Gtk::TreeView))

, keyboard_shortcuts_treeview_box_(Gtk::manage(new Gtk::Box))

, keyboard_shortcuts_treeview_columnrecord_(new KeyboardShortcutsPanelColumnRecord)

, keyboard_shortcuts_treeview_frame_(Gtk::manage(new Gtk::Frame))

, keyboard_shortcuts_treeview_scrolled_window_(Gtk::manage(new Gtk::ScrolledWindow))

{

  //                   //
  // Filename TreeView ////////////////////////////////////////////////////////
  //                   //

  // Adds the output sink's box to the panel's box.
  box() . append(*keyboard_shortcuts_treeview_box_);

  // Adds the output sink's box to the panel's box.
  keyboard_shortcuts_treeview_box_ -> append(*keyboard_shortcuts_treeview_frame_, 
                                       Gtk::PACK_EXPAND_WIDGET);

  // Adds the output sink's box to the panel's box.
  keyboard_shortcuts_treeview_frame_ -> add(*keyboard_shortcuts_treeview_scrolled_window_);

  // Adds the output sink's box to the panel's box.
  keyboard_shortcuts_treeview_scrolled_window_ -> add(*keyboard_shortcuts_treeview_);



  // Sets the margin of the keyboard_shortcuts treeview box.
  keyboard_shortcuts_treeview_box_ -> set_margin_top(2);

  // Sets the margin of the keyboard_shortcuts treeview box.
  keyboard_shortcuts_treeview_box_ -> set_margin_bottom(2);

  // Sets the margin of the keyboard_shortcuts treeview box.
  keyboard_shortcuts_treeview_box_ -> set_margin_start(2);

  // Sets the margin of the keyboard_shortcuts treeview box.
  keyboard_shortcuts_treeview_box_ -> set_margin_end(2);



  // 
  keyboard_shortcuts_treeview_ -> set_enable_search(false);



  // Sets the behaviour of the scrobbable window of the keyboard_shortcuts treeview to 
  // appear automatically when needed.
  keyboard_shortcuts_treeview_scrolled_window_ -> set_policy(Gtk::POLICY_AUTOMATIC,
                                          Gtk::POLICY_AUTOMATIC);



  // 
  keyboard_shortcuts_liststore_ = new_keyboard_shortcuts_liststore;



  // 
  keyboard_shortcuts_treeview_ -> set_model(keyboard_shortcuts_liststore_);



  // 
  keyboard_shortcuts_treeview_
    -> set_tooltip_text("Double click a row to set the key binding of that" \
                        " keyboard shortcut.");



  // Appends a keyboard shortcut name column.
  keyboard_shortcuts_treeview_
    -> append_column
         ("Shortcut", keyboard_shortcuts_treeview_columnrecord_ -> description_col_);

  // Appends a keyboard shortcut column.
  keyboard_shortcuts_treeview_
    -> append_column
         ("Key", keyboard_shortcuts_treeview_columnrecord_ -> label_col_);



  keyboard_shortcuts_treeview_ -> signal_row_activated()
    . connect(sigc::mem_fun(*this, &KeyboardShortcutsPanel::Row_Activated));





  //                  //
  // Filename Buttons /////////////////////////////////////////////////////////
  //                  //

  // Adds the output sink's box to the panel's box.
  box() . pack_end(*keyboard_shortcuts_display_box_, Gtk::PACK_SHRINK);



  // 
  keyboard_shortcuts_display_box_
    -> set_orientation(Gtk::Orientation::HORIZONTAL);



  // 
  keyboard_shortcuts_display_box_ -> append(*load_default_button_, 0, 1);

  // 
  keyboard_shortcuts_display_box_
    -> set_center_widget(*keyboard_shortcuts_display_inner_box_);

  // 
  keyboard_shortcuts_display_box_ -> pack_end(*clear_key_button_, 0, 1);



  // 
  keyboard_shortcuts_display_inner_box_
    -> set_orientation(Gtk::Orientation::HORIZONTAL);



  // 
  keyboard_shortcuts_display_inner_box_
    -> append(*keyboard_shortcuts_label_, Gtk::PACK_SHRINK);

  // 
  keyboard_shortcuts_display_inner_box_
    -> append(*keyboard_shortcuts_key_label_, Gtk::PACK_SHRINK);



  // 
  keyboard_shortcuts_label_ -> set_markup("<u>Key Press</u>: ");



  // 
  load_default_button_ -> set_margin_start(3);

  // 
  load_default_button_ -> set_margin_end(3);

  // 
  load_default_button_ -> set_margin_bottom(3);



  // 
  clear_key_button_ -> set_margin_start(3);

  // 
  clear_key_button_ -> set_margin_end(3);

  // 
  clear_key_button_ -> set_margin_bottom(3);



  clear_key_button_ -> signal_clicked()
    . connect(sigc::mem_fun(*this, &KeyboardShortcutsPanel::Clear_Key));

  load_default_button_ -> signal_clicked()
    . connect(sigc::mem_fun(*this, &KeyboardShortcutsPanel::Load_Default));





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

KeyboardShortcutsPanel::~KeyboardShortcutsPanel()
{

  // 
  delete keyboard_shortcuts_treeview_columnrecord_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void KeyboardShortcutsPanel::Apply_Saved_Values()
{

  std::map<string, string> keys
    {{"add_files", "Add Files To Active Playlist"},
     {"close_secondary", "Close Active Secondary Window"},
     {"configuration", "Configuration Window"},
     {"copy_rows", "Copy Selected Rows"},
     {"create_playlist", "Create A New Playlist"},
     {"cut_rows", "Cut Selected Rows"},
     {"edit_row", "Edit Row Metadata"},
     {"delete_playlist", "Delete Active Playlist"},
     {"delete_rows", "Delete Selected Rows"},
     {"next_track", "Next Track"},
     {"paste_rows", "Paste Clipboard Rows"},
     {"pause", "Pause"},
     {"play", "Play"},
     {"rename_playlist", "Rename Active Playlist"},
     {"select_all_rows", "Select All Rows"},
     {"select_all_rows", "System Menu Pops Up"},
     {"stop", "Stop"},
     {"stop_after_current", "Stop After Current Track"}};



  // 
  keyboard_shortcuts_liststore_ -> clear();

  //
  for(auto keys_it : keys)
  {

    // 
    Gtk::TreeIter new_keyboard_shortcuts_row_iter
      = keyboard_shortcuts_liststore_ -> append();

    // 
    Gtk::TreeRow new_keyboard_shortcuts_row = *new_keyboard_shortcuts_row_iter;

    // 
    new_keyboard_shortcuts_row[keyboard_shortcuts_treeview_columnrecord_ -> description_col_] = keys_it . second;

    // 
    new_keyboard_shortcuts_row[keyboard_shortcuts_treeview_columnrecord_ -> name_col_] = keys_it . first;



    // 
    string config_str = "keyboard_shortcuts.keys." + keys_it . first;

    // 
    string key_name = config() . get(config_str);

    // 
    guint keyval;

    // 
    GdkModifierType mods;

    // 
    gtk_accelerator_parse(key_name . c_str(), &keyval, &mods);

    // 
    string key_label = gtk_accelerator_get_label(keyval, mods);

    // 
    new_keyboard_shortcuts_row[keyboard_shortcuts_treeview_columnrecord_ -> key_col_] = key_name;

    // 
    new_keyboard_shortcuts_row[keyboard_shortcuts_treeview_columnrecord_ -> label_col_] = key_label;

  }

}

void KeyboardShortcutsPanel::Clear_Key()
{

  // 
  auto keyboard_treeselection
    = keyboard_shortcuts_treeview_ -> get_selection();



  // 
  auto selected_rows = keyboard_treeselection -> get_selected_rows();



  // 
  if((selected_rows . size()) < 1)
  {

    // 
    return;

  }



  // 
  Gtk::TreeRow key_row
    = *(keyboard_shortcuts_liststore_ -> get_iter(selected_rows[0]));



  // 
  Glib::ustring config_name
    = key_row[keyboard_shortcuts_treeview_columnrecord_ -> name_col_];



  // 
  string final_config_name = "keyboard_shortcuts.keys.";

  // 
  final_config_name += config_name;



  // Loads the default value for a specific configuration setting.
  config() . set(final_config_name . c_str(), "");

  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  key_row[keyboard_shortcuts_treeview_columnrecord_ -> label_col_]
    = "";

  // 
  key_row[keyboard_shortcuts_treeview_columnrecord_ -> key_col_]
    = "";

}

void KeyboardShortcutsPanel::Load_Default()
{

  // 
  auto keyboard_treeselection
    = keyboard_shortcuts_treeview_ -> get_selection();



  // 
  auto selected_rows = keyboard_treeselection -> get_selected_rows();



  if((selected_rows . size()) < 1)
  {

    // 
    return;

  }



  // 
  Gtk::TreeRow key_row
    = *(keyboard_shortcuts_liststore_ -> get_iter(selected_rows[0]));



  // 
  Glib::ustring config_name
    = key_row[keyboard_shortcuts_treeview_columnrecord_ -> name_col_];



  // 
  string final_config_name = "keyboard_shortcuts.keys.";

  // 
  final_config_name += config_name;



  // Loads the default value for a specific configuration setting.
  config() . Load_Default_Value(final_config_name . c_str());

  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  string key_name_str = config() . get(final_config_name);



  // 
  guint keyval;

  // 
  GdkModifierType mods;

  // 
  gtk_accelerator_parse(key_name_str . c_str(), &keyval, &mods);

  // 
  string key_label = gtk_accelerator_get_label(keyval, mods);

  // 
  key_row[keyboard_shortcuts_treeview_columnrecord_ -> label_col_]
    = key_label;

  // 
  key_row[keyboard_shortcuts_treeview_columnrecord_ -> key_col_]
    = key_name_str;

}

void KeyboardShortcutsPanel::Update_Key(const char* name, const char* label)
{

  // 
  keyboard_shortcuts() . set_skip_window_key_callback(true);



  list<string> modifiers
    {"Control L", "Control R", "Alt L", "Alt R", "Shift L", "Shift R",
     "Super L", "Super R"};

  // 
  for(auto mod_it : modifiers)
  {

    // 
    if(mod_it == label)
    {

      // 
      return;

    }

  }



  // 
  Gtk::TreeRow key_row
    = *(keyboard_shortcuts_liststore_ -> get_iter(editing_path_));



  // 
  Glib::ustring config_name
    = key_row[keyboard_shortcuts_treeview_columnrecord_ -> name_col_];



  // 
  string final_config_name = "keyboard_shortcuts.keys.";

  // 
  final_config_name += config_name;



  // 
  string name_str = name;

  // 
  config() . set(final_config_name, name_str);



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  key_row[keyboard_shortcuts_treeview_columnrecord_ -> label_col_] = label;



  // 
  keyboard_shortcuts() . set_updating_binding(false);

}

void KeyboardShortcutsPanel::Row_Activated
  (const Gtk::TreePath& path, Gtk::TreeViewColumn* column)
{

  // 
  Gtk::TreePath clicked_row_path
    (keyboard_shortcuts_treeview_ -> get_selection() -> get_selected());

  // 
  editing_path_ = clicked_row_path . to_string();



  // 
  if(keyboard_shortcuts() . updating_binding())
  {

    // 
    for(auto config_guis_it : config_guis()())
    {

      // 
      config_guis_it -> Apply_Saved_Values();

    }



    // 
    keyboard_shortcuts() . set_updating_binding(false);

  }



  // 
  Gtk::TreeRow clicked_row 
    = *(keyboard_shortcuts_liststore_ -> get_iter(editing_path_));

  // 
  clicked_row[keyboard_shortcuts_treeview_columnrecord_ -> label_col_]
    = "~ Enter A New Binding ~";



  // 
  keyboard_shortcuts() . set_updating_binding(true);

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

Gtk::Label& KeyboardShortcutsPanel::keyboard_shortcuts_key_label()
{

  return *keyboard_shortcuts_key_label_;

}
