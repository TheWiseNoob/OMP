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

#include "ConfigurationGUIs.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../ChildWindow.h"

#include "../../GUI.h"

#include "Panels/Artwork/ArtworkPanelColumnRecord.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/button.h>





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

{

  // Creates a new ConfigurationGUI.
  ConfigurationGUI* temp_config_gui = new ConfigurationGUI(base, *this);

  // Adds the ConfigGUI to the gui_elements() list.
  gui_elements() . push_front(temp_config_gui);



  // Sets the iterator location inside the gui_elements list.
  temp_config_gui -> set_gui_elements_it(gui_elements() . begin());

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

  // Adds the new ConfigGUI to the ConfigGUIs list.
  (*this)() . push_front(new_config_gui);

  // Sets the new ConfigGUI's location in ConfigGUIs.
  new_config_gui -> set_gui_elements_it((*this)() . begin());



  bool unsaved_changes = this -> unsaved_changes();



  // Binds the ConfigGUI's Destroy function a std::function pointer.
  std::function<void(void)> new_destroy_func_ptr
    = std::bind(&ConfigurationGUI::Destroy, new_config_gui);

  // Creates of new window using the Create_New_Window function.
  shared_ptr<ChildWindow> new_child_window
    = gui() . Create_New_Window("Configuration", new_destroy_func_ptr);



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
  new_child_window -> window() . set_default_size(600, 400);

  // Displays the new window.
  new_child_window -> show();

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



  // Reads the configuration file to get saved values.
  config() . read_file();



  // Applies saved values to all ConfigGUIs.
  Apply_Saved_Values();



  // Enables ConfigGUI functions.
  set_disable_functions(false);

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

bool ConfigurationGUIs::disable_functions()
{

  return disable_functions_;

}

atomic<bool>& ConfigurationGUIs::unsaved_changes()
{

  return unsaved_changes_;

}

Glib::RefPtr<Gtk::ListStore> ConfigurationGUIs::filename_liststore()
{

  return filename_liststore_;

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
