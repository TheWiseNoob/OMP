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

#include "GUIPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../../../Configuration/Configuration.h"

#include "../../../../../GUI/GUI.h"

#include "../../../../../GUI/MenuBar.h"

#include "../../../../../GUI/StatusBar.h"

#include "../../../PlaylistComboBoxes/PlaylistComboBox.h"

#include "../../../PlaylistComboBoxes/PlaylistComboBoxes.h"

#include "../../ConfigurationGUI.h"

#include "../../ConfigurationGUIs.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/box.h>

#include <gtkmm/checkbutton.h>

#include <gtkmm/checkmenuitem.h>

#include <gtkmm/frame.h>

#include <gtkmm/notebook.h>





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

GUIPanel::GUIPanel(Base& base_ref, ConfigurationGUI& config_gui_ref)

// Inherited Class

: Panel(base_ref, config_gui_ref, "GUI")



// General

, selection_box_(Gtk::manage(new Gtk::Box))

, hide_duplicates_check_button_(Gtk::manage(new Gtk::CheckButton))

, hide_header_bar_check_button_(Gtk::manage(new Gtk::CheckButton))

, hide_status_bar_check_button_(Gtk::manage(new Gtk::CheckButton))

, hide_tabs_check_button_(Gtk::manage(new Gtk::CheckButton))

{

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  // 
  box() . set_center_widget(*selection_box_);

  // 
  selection_box_ -> pack_start(*hide_duplicates_check_button_,
                               Gtk::PACK_EXPAND_PADDING);

  // 
  selection_box_ -> pack_start(*hide_header_bar_check_button_,
                               Gtk::PACK_EXPAND_PADDING);

  // 
  selection_box_ -> pack_start(*hide_status_bar_check_button_,
                               Gtk::PACK_EXPAND_PADDING);

  // 
  selection_box_ -> pack_start(*hide_tabs_check_button_,
                               Gtk::PACK_EXPAND_PADDING);



  // 
  box() . set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  // 
  selection_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // 
  box() . set_hexpand(true);

  // 
  box() . set_vexpand(true);

  // 
  selection_box_ -> set_hexpand(true);

  // 
  selection_box_ -> set_vexpand(true);



  //
  hide_duplicates_check_button_
    -> set_label("Hide the duplicate menu bar and playlist combobox.");

  //
  hide_header_bar_check_button_
    -> set_label("Hide the header bar. (Requires a restart.)");

  //
  hide_status_bar_check_button_ -> set_label("Hide the status bar.");

  //
  hide_tabs_check_button_ -> set_label("Hide the tabs.");



  // 
  hide_duplicates_check_button_
    -> set_tooltip_text("Hides the duplicate menu bar and playlist combobox " \
                        "under the cover art in the Main Content tab.");

  // 
  hide_header_bar_check_button_
    -> set_tooltip_text("Hides the header bar. Requires OMP to be restarted " \
                        "in order to take effect.");

  // 
  hide_status_bar_check_button_
    -> set_tooltip_text("Hides the status bar at the bottom " \
                        "of the main window.");

  // 
  hide_tabs_check_button_
    -> set_tooltip_text("Hides the tabs of OMP.");



  // 
  hide_duplicates_check_button_ -> set_margin_left(2);

  // 
  hide_duplicates_check_button_ -> set_margin_right(2);

  // 
  hide_header_bar_check_button_ -> set_margin_left(2);

  // 
  hide_header_bar_check_button_ -> set_margin_right(2);

  // 
  hide_status_bar_check_button_ -> set_margin_left(2);

  // 
  hide_status_bar_check_button_ -> set_margin_right(2);

  // 
  hide_tabs_check_button_ -> set_margin_left(2);

  // 
  hide_tabs_check_button_ -> set_margin_right(2);



  //
  hide_duplicates_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &GUIPanel::On_Hide_Duplicates_Check_Button_Toggled_Signal));

  //
  hide_header_bar_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &GUIPanel::On_Hide_Header_Bar_Check_Button_Toggled_Signal));

  //
  hide_status_bar_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, 
         &GUIPanel::On_Hide_Status_Bar_Check_Button_Toggled_Signal));

  //
  hide_tabs_check_button_ -> signal_toggled()
    . connect(sigc::mem_fun(*this, &GUIPanel::Hide_Tabs));





  //       //
  // Other ////////////////////////////////////////////////////////////////////
  //       //

  // Adds the Apply_Saved_Value function to the ConfigGUI's list.
  config_gui_ref
    . Add_Apply_Saved_Value_Function(*this, &Panel::Apply_Saved_Values);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

GUIPanel::~GUIPanel()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void GUIPanel::Apply_Saved_Values()
{

  // 
  bool active;





  //                 //
  // Hide Duplicates //////////////////////////////////////////////////////////
  //                 //

  // 
  active = config() . get("gui.hide_duplicates");



  // 
  hide_duplicates_check_button_ -> set_active(active);



  // 
  if(active)
  {

    // 
    gui() . menubars() . back() -> box() . hide();

    // 
    playlist_comboboxes()() . back() -> box() . hide();

  }

  // 
  else
  { 

    // 
    gui() . menubars() . back() -> box() . show();

    // 
    playlist_comboboxes()() . back() -> box() . show();

  }



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_duplicates_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);





  //                 //
  // Hide Header Bar //////////////////////////////////////////////////////////
  //                 //

  // 
  active = config() . get("gui.hide_header_bar");



  // 
  hide_header_bar_check_button_ -> set_active(active);



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_header_bar_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);





  //                 //
  // Hide Status Bar //////////////////////////////////////////////////////////
  //                 //

  // 
  active = config() . get("gui.hide_status_bar");



  // 
  hide_status_bar_check_button_ -> set_active(active);



  // 
  if(active)
  {

    // 
    status_bar() . status_bar_frame() . hide();

  }

  //
  else
  {

    // 
    status_bar() . status_bar_frame() . show();

  }



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_status_bar_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);





  //           //
  // Hide Tabs ////////////////////////////////////////////////////////////////
  //           //

  // 
  active = config() . get("gui.tabs.hide");



  // 
  hide_tabs_check_button_ -> set_active(active);



  // 
  gui() . main_window_notebook() . set_show_tabs(!active);



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_tabs_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);

}

void GUIPanel::Hide_Tabs()
{

  // 
  if(config_guis() . disable_functions())
  {

    return;

  }



  // 
  bool active = hide_tabs_check_button_ -> get_active();



  // 
  gui() . main_window_notebook() . set_show_tabs(!active);



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // 
  for(auto it : config_guis()())
  {

    // 
    it -> gui_panel() . hide_tabs_check_button()
      . set_active(active);

  }



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_tabs_check_menu_item() . set_active(active);

  } 

  // 
  gui() . set_disable_menubar_functions_flag(false);




  // 
  config() . set("gui.tabs.hide", active); 

}

void GUIPanel::On_Hide_Duplicates_Check_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    return;

  }



  // 
  bool active = hide_duplicates_check_button_ -> get_active();



  // 
  if(active)
  {

    // 
    gui() . menubars() . back() -> box() . hide();

    // 
    playlist_comboboxes()() . back() -> box() . hide();

  }

  else
  {

    // 
    gui() . menubars() . back() -> box() . show();

    // 
    playlist_comboboxes()() . back() -> box() . show();

  }



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // 
  for(auto it : config_guis()())
  {

    // 
    it -> gui_panel() . hide_duplicates_check_button()
      . set_active(active);

  }



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_duplicates_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);




  // 
  config() . set("gui.hide_duplicates", active); 

}

void GUIPanel::On_Hide_Header_Bar_Check_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    return;

  }



  // 
  bool active = hide_header_bar_check_button_ -> get_active();



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // 
  for(auto it : config_guis()())
  {

    // 
    it -> gui_panel() . hide_header_bar_check_button()
      . set_active(active);

  }



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_header_bar_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);




  // 
  config() . set("gui.hide_header_bar", active); 



}

void GUIPanel::On_Hide_Status_Bar_Check_Button_Toggled_Signal()
{

  // 
  if(config_guis() . disable_functions())
  {

    return;

  }



  // 
  bool active = hide_status_bar_check_button_ -> get_active();



  // 
  if(active)
  {

    // 
    status_bar() . status_bar_frame() . hide();

  }

  //
  else
  {

    // 
    status_bar() . status_bar_frame() . show();

  }



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // 
  for(auto it : config_guis()())
  {

    // 
    it -> gui_panel() . hide_status_bar_check_button()
      . set_active(active);

  }



  // 
  gui() . set_disable_menubar_functions_flag(true);

  // 
  for(auto menubars_it : gui() . menubars())
  {

    // 
    menubars_it -> hide_status_bar_check_menu_item() . set_active(active);

  }

  // 
  gui() . set_disable_menubar_functions_flag(false);




  // 
  config() . set("gui.hide_status_bar", active); 

}




 
//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::CheckButton& GUIPanel::hide_duplicates_check_button()
{

  // 
  return *hide_duplicates_check_button_;

}

Gtk::CheckButton& GUIPanel::hide_header_bar_check_button()
{

  // 
  return *hide_header_bar_check_button_;

}

Gtk::CheckButton& GUIPanel::hide_status_bar_check_button()
{

  // 
  return *hide_status_bar_check_button_;

}

Gtk::CheckButton& GUIPanel::hide_tabs_check_button()
{

  // 
  return *hide_tabs_check_button_;

}
