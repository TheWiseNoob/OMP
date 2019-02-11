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

#include "ConfigurationGUIs.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../GUI.h"

#include "../ChildWindows/ChildWindow.h"

#include "../ChildWindows/ChildWindows.h"

#include "../MainMenus/MainMenus.h"

#include "Panels/Artwork/ArtworkPanelColumnRecord.h"

#include "Panels/KeyboardShortcuts/KeyboardShortcutsPanelColumnRecord.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <filesystem>

#include <glibmm.h>

#include <gtkmm/applicationwindow.h>

#include <gtkmm/button.h>

#include <gtkmm/textbuffer.h>





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

ConfigurationGUIs::ConfigurationGUIs(Base& base)

// Inherited Class

: GUIElementList(base)



// Flags

, disable_functions_(false)

, unsaved_changes_(false)



// Artwork

, filename_liststore_column_record_(new ArtworkPanelColumnRecord)

, filename_liststore_(Gtk::ListStore::create(*filename_liststore_column_record_))



// Keyboard Shortcuts

, keyboard_shortcuts_liststore_column_record_(new KeyboardShortcutsPanelColumnRecord)

, keyboard_shortcuts_liststore_(Gtk::ListStore::create(*keyboard_shortcuts_liststore_column_record_))



// Metadata

, error_log_textbuffer_(Gtk::TextBuffer::create())

{

  // 
  string errors_log_file_str = base . config_directory_c_str();



  // 
  errors_log_file_str += "/errors.md";



  // 
  if(filesystem::exists(errors_log_file_str))
  {

    // 
    Glib::ustring errors_log_ustr = Glib::file_get_contents(errors_log_file_str); 



    // 
    error_log_textbuffer_ -> insert_markup
      (error_log_textbuffer_ -> begin(), errors_log_ustr);

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

ConfigurationGUIs::~ConfigurationGUIs()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void ConfigurationGUIs::Apply_Saved_Values()
{

  // Disables ConfigGUI functions.
  set_disable_functions(true);



  // 
  Mark_Unsaved_Changes(false);

  // Loops through the ConfigGUI list to call Apply_Saved_Values in each.
  for(auto it : (*this)())
  {

    // Calls Apply_Saved_Values of the current ConfigGUI to apply saved values.
    it -> Apply_Saved_Values(); 

  }

  // 
  Mark_Unsaved_Changes(false);



  // Enables ConfigGUI functions.
  set_disable_functions(false);

}

void ConfigurationGUIs::Load_Default_Values()
{

  // Loads the default values of the configuration.
  config() . Load_Default_Values();



  // 
  Apply_Saved_Values();



  // 
  config() . set("default_values", true);

}

void ConfigurationGUIs::Mark_Unsaved_Changes(bool new_value)
{


  // 
  unsaved_changes_ = new_value;



  // 
  for(auto config_guis_it : (*this)())
  {

    // 
    config_guis_it -> Mark_Unsaved_Changes(new_value);

  }

}

void ConfigurationGUIs::Open_Configuration()
{

  // Creates a temporary ConfigGUI pointer.
  ConfigurationGUI* new_config_gui;

  // Assigns a new ConfigGUI object to the pointer.
  new_config_gui = new ConfigurationGUI(base(), (*this));



  // 
  bool unsaved_changes = this -> unsaved_changes();



  // Binds the ConfigGUI's Destroy function a std::function pointer.
  std::function<void(void)> new_destroy_func_ptr
    = std::bind(&ConfigurationGUI::Destroy, new_config_gui);

  // Creates of new window using the Create_New_Window function.
  ChildWindow* new_child_window
    = windows() . Create("Configuration", new_destroy_func_ptr);



  // Sets the flag to disable ConfigGUI functions while applying the values
  // to the new ConfigGUI.
  config_guis() . set_disable_functions(true);

  // Calls the new ConfigGUI's function to apply the saved values to it.
  new_config_gui -> Apply_Saved_Values();



  // 
  this -> Mark_Unsaved_Changes(unsaved_changes);



  // Sets the flag to enable ConfigGUI functions after applying the values
  // to the new ConfigGUI.
  config_guis() . set_disable_functions(false);



  // Adds the ConfigGUI to the new window. 
  new_child_window -> box() . pack_start(new_config_gui -> box(),
                                         Gtk::PACK_EXPAND_WIDGET);

  // 
  new_child_window -> window() . set_default_size(700, 500);

  // Displays the new window.
  new_child_window -> Show();

}

void ConfigurationGUIs::Save_Changes()
{

  debug("At the beginning of Save Changes in ConfigGUIs.");



  // True if ConfigGUI functions are disabled.
  if(disable_functions_)
  {

    // Ends the function early.
    return;

  }



  // Disables ConfigGUI functions.
  set_disable_functions(true);



  debug("Before iterating through ConfigGUI list.");



  debug("Before config write");



  config() . set("default_values", false);



  // Writes the configuration to file.
  config() . write_file();



  // 
  Mark_Unsaved_Changes(false);



  debug("After config write");



  // Enables ConfigGUI functions.
  set_disable_functions(false);

}

void ConfigurationGUIs::Undo_Changes()
{

  debug("Undo_Changes occurring.");



  // Disables all ConfigGUI functions.
  if(disable_functions())
  {

    // Returns, ending Undo_Changes.
    return;

  }



  // Disables ConfigGUI functions.
  set_disable_functions(true);



  //
  Glib::ustring temp_active_panel_name_ustr
    = config() . get("gui.configuration.active_panel");



  // Reads the configuration file to get saved values.
  config() . read_file();



  // 
  config()
    . set("gui.configuration.active_panel", temp_active_panel_name_ustr);



  // Applies saved values to all ConfigGUIs.
  Apply_Saved_Values();



  // 
  main_menus() . Apply_Saved_Values();



  // Enables ConfigGUI functions.
  set_disable_functions(false);

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

//         //
// general ////////////////////////////////////////////////////////////////////
//         //

bool ConfigurationGUIs::disable_functions()
{

  return disable_functions_;

}

atomic<bool>& ConfigurationGUIs::unsaved_changes()
{

  return unsaved_changes_;

}





//         //
// Artwork ////////////////////////////////////////////////////////////////////
//         //

Glib::RefPtr<Gtk::ListStore> ConfigurationGUIs::filename_liststore()
{

  return filename_liststore_;

}





//                    //
// Keyboard Shortcuts /////////////////////////////////////////////////////////
//                    //

Glib::RefPtr<Gtk::ListStore> ConfigurationGUIs::keyboard_shortcuts_liststore()
{

  return keyboard_shortcuts_liststore_;

}





//          //
// Metadata ////////////////////////////////////////////////////////////////////
//          //

Glib::RefPtr<Gtk::TextBuffer> ConfigurationGUIs::error_log_textbuffer()
{

  return error_log_textbuffer_;

}




//         //
//         //
// Setters //////////////////////////////////////////////////////////////////
//         //
//         //

void ConfigurationGUIs::set_disable_functions(bool new_setting)
{

  disable_functions_ = new_setting;

}
