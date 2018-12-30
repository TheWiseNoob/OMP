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

#include "ChildWindow.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../../KeyboardShortcuts/KeyboardShortcuts.h"

#include "../../GUI.h"

#include "../Playlists/Playlists.h"

#include "../MainMenus/MainMenu.h"

#include "../MainMenus/MainMenus.h"

#include "ChildWindows.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/applicationwindow.h>

#include <gtkmm/box.h>

#include <gtkmm/menu.h>

#include <iostream>





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

//              //
//              //
// Constructors ////////////////////////////////////////////////////////////////
//              //
//              //

ChildWindow::ChildWindow
  (const char* new_title, Base& base_ref, ChildWindows& child_windows_ref,
   std::function<void(void)> new_child_class_destroy_function,
   bool make_main_window)

// Inherited Class

: GUIElement(base_ref, child_windows_ref())



// Member Variables

, box_(Gtk::manage(new Gtk::Box))

, fullscreen_(false)

, window_(new Gtk::ApplicationWindow)

{ 

  // 
  child_windows_ref() . push_front(this);

  // Adds the new FileChooser's to the iterator to it's it storage variable.
  set_gui_elements_it(child_windows_ref() . begin());



  // 
  main_window_ = make_main_window;

  // 
  if(!main_window_)
  { 

    // Overrides the function for keypresses to allow custom shortcuts.
    window_ -> signal_key_press_event()
      . connect(sigc::mem_fun(base_ref . keyboard_shortcuts(),
                              &KeyboardShortcuts::On_Key_Press_Event));

   }



  // 
  window_ -> signal_button_press_event()
    . connect(sigc::mem_fun(*this, &ChildWindow::On_Button_Press_Event));

  // 
  window_ -> signal_key_press_event()
    . connect(sigc::mem_fun(*this, &ChildWindow::On_Key_Press_Event));



  // 
  window_ -> set_title(new_title);

  // 
  window_ -> set_position(Gtk::WIN_POS_CENTER);

  // 
  window_ -> set_default_size(650,400);  



  debug("ChildWindow shown");



  // 
  window_ -> signal_delete_event()
    . connect(sigc::mem_fun(*this, &ChildWindow::On_Irregular_Quit));



  // 
  child_class_destroy_function_ = new_child_class_destroy_function;



  // 
  window_ -> add(*box_);

}

ChildWindow::ChildWindow
  (const char* window_name, 
   std::function<void(void)> new_child_class_destroy_function)

: ChildWindow(window_name, base(), windows(), new_child_class_destroy_function)

{

  

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

ChildWindow::~ChildWindow()
{

  // 
  if(!main_window_)
  {

    // 
    if(!(base() . quitting()))
    {

      // 
      delete window_;

    }

  }

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

bool ChildWindow::On_Button_Press_Event(GdkEventButton* event)
{

  // 
  if((event -> type == GDK_BUTTON_PRESS) && (event -> button == 3))
  {

    // 
    main_menus() . system_menu() . menu() . popup_at_pointer(nullptr);

  }



  // 
  return false;

}

bool ChildWindow::On_Key_Press_Event(GdkEventKey* event)
{

  // 
  if(main_window_)
  {

    // 
    keyboard_shortcuts() . set_main_window(true);

  }

  // 
  else if(main_window_)
  {

    // 
    keyboard_shortcuts() . set_main_window(false);

  }



  if(keyboard_shortcuts() . skip_window_key_callback())
  {

    // 
    keyboard_shortcuts() . set_skip_window_key_callback(false);



    // 
    return false;

  }



  // 
  guint keyval = event -> keyval;

  // 
  GdkModifierType state = (GdkModifierType)(event -> state);

  // 
  string name = gtk_accelerator_name(keyval, state);

  // 
  string label = gtk_accelerator_get_label(keyval, state);



  // 
  if(name == config() . get_str("keyboard_shortcuts.keys.close_secondary"))
  {

    // 
    if(!main_window_)
    {

      // 
      On_Irregular_Quit(NULL);



      // 
      return true;

    }

  }

  // 
  if(name == config() . get_str("keyboard_shortcuts.keys.fullscreen"))
  {

    // 
    if(fullscreen_)
    {

      // 
      fullscreen_ = false;

      // 
      window() . unfullscreen();

    }

    // 
    else
    {

      // 
      fullscreen_ = true;

      // 
      window() . fullscreen();

    }

  }



  // Allows normal keyboard event propagation.
  return false;

} 

bool ChildWindow::On_Irregular_Quit(GdkEventAny* event)                          
{                                                                                
                                                                                
  // 
  if(child_class_destroy_function_)
  {

    // 
    child_class_destroy_function_();

  } 



  // 
  if(main_window_)
  {

    // 
    if(playlists() . rebuild_databases() || playlists() . rebuilding_databases())
    {

      // 
      playlists() . Open_Changes_Cancel_Dialog(true);



      // 
      return true;

    }

    // 
    else
    {

      // 
      base() . quitting() = true;

    }

  }



  // 
  window_ -> hide();



  // 
  delete *gui_elements_it();



  // 
  return true;

}

void ChildWindow::Show()
{

  window_ -> show();

  window_ -> show_all_children();

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::Box& ChildWindow::box()
{

  return *box_; 

}

bool ChildWindow::fullscreen()
{

  return fullscreen_;

}

Gtk::ApplicationWindow& ChildWindow::window()
{

  return *window_;

}





//         //
//         //
// Setters //////////////////////////////////////////////////////////////////
//         //
//         //

void ChildWindow::set_default_size(int width, int height)
{ 

  window_ -> set_default_size(width, height); 

}

void ChildWindow::set_fullscreen(bool new_fullscreen)
{

  fullscreen_ = new_fullscreen;

}

void ChildWindow::set_title(const char* new_title)
{

  window_ -> set_title(new_title); 

}
