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

#include "KeyboardShortcutsPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../ConfigurationGUI.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/eventbox.h>

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

//             //
//             //
// Constructor ////////////////////////////////////////////////////////////////
//             //
//             //

KeyboardShortcutsPanel::KeyboardShortcutsPanel
  (Base& base_ref, ConfigurationGUI& new_config_gui)

// Inherited Class

: Panel(base_ref, new_config_gui, "Keyboard Shortcuts")



// 

, main_event_box_(Gtk::manage(new Gtk::EventBox))

{

  //                 //
  // KeyboardShortcuts Sink //////////////////////////////////////////////////////////////
  //             //

  // Adds the output sink's box to the panel's box.
  box() . pack_start(*main_event_box_, Gtk::PACK_EXPAND_WIDGET);





  //       //
  // Other ////////////////////////////////////////////////////////////////////
  //       //

  // Adds the Apply_Saved_Value function to the ConfigGUI's list.
  new_config_gui . Add_Apply_Saved_Value_Function(*this,
                                                  &Panel::Apply_Saved_Values);




  // 
  main_event_box_ -> set_events(Gdk::KEY_PRESS_MASK);

  // Overrides the function for keypresses to allow custom shortcuts.
  main_event_box_ -> signal_key_press_event()
    . connect(sigc::mem_fun(*this, 
                            &KeyboardShortcutsPanel::On_Key_Press_Event));

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

KeyboardShortcutsPanel::~KeyboardShortcutsPanel()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void KeyboardShortcutsPanel::Apply_Saved_Values()
{

}

bool KeyboardShortcutsPanel::On_Key_Press_Event(GdkEventKey* event)
{

  // 
  cout << "\n\nDerp\n\n";



/*
  // Opens file chooser dialog for adding files to the currently selected
  // playlists.
  if((event -> keyval == GDK_KEY_o) && (event->state & GDK_CONTROL_MASK))
  {

    // Opens the FileChooser.
    Add_File();

    // End the key press function.
    return true;

  }

  // Opens a new configuration window.
  else if((event -> keyval == GDK_KEY_j) && (event->state & GDK_CONTROL_MASK))
  {

    // Calls the function for opening a new ConfigurationGUI window.
    config_guis() . Open_Configuration();



    return true;

  }

  // Is true if the space bar was pressed.
  else if((event -> keyval == GDK_KEY_space))
  {

    // Pauses playback.
    playback() . Pause();

    return true;

  } 

  // Is true if the escape key is pressed.
  else if((event -> keyval == GDK_KEY_Escape))
  {

    if(!(playback() . Stopped()))
    {

      // Stops playback.
      playback().Stop();

    }



    // 
    return true;

  }

  // Is true if F11 is pressed.
  else if((event -> keyval == GDK_KEY_F11))
  {

    // True the fullscreen_ variable is true.
    if(fullscreen_)
    {

      // Unfullscreens the window.
      windows_.front() -> window().unfullscreen();

      // Sets the fullscreen variable to false.
      fullscreen_ = false;

    }
    else
    {

      // Fullscreens the window.
      windows_.front() -> window().fullscreen();

      // Sets the fullscreen variable to true.
      fullscreen_ = true;

    }

    // Ends the function.
    return true;

  }


*/
  // Allows normal keyboard event propagation.
  return false;
 
}
