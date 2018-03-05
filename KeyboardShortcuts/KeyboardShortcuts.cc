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

#include "KeyboardShortcuts.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"

#include "../Configuration/Configuration.h"

#include "../GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "../GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanel.h"

#include "../GUI/Elements/FileChoosers/FileChoosers.h"

#include "../GUI/Elements/Playlists/Playlists.h"

#include "../GUI/GUI.h"

#include "../Playback/Playback.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gdk/gdk.h>

#include <gtkmm/treerowreference.h>





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

KeyboardShortcuts::KeyboardShortcuts(Base& base_ref)

// Inherited Class

: Parts(base_ref)



// 

, main_window_(true)

, skip_window_key_callback_(false)

, updating_binding_(false)

{

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

KeyboardShortcuts::~KeyboardShortcuts()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

bool KeyboardShortcuts::On_Key_Press_Event(GdkEventKey* event)
{

  // 
  guint keyval = event -> keyval;

  // 
  GdkModifierType state = (GdkModifierType)(event -> state);

  // 
  string name = gtk_accelerator_name(keyval, state);

  // 
  string label = gtk_accelerator_get_label(keyval, state);



  //
  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> keyboard_shortcuts_panel()
      . keyboard_shortcuts_key_label() . set_text(label);



    // 
    if(updating_binding_)
    {

      // 
      config_guis_it -> keyboard_shortcuts_panel()
        . Update_Key(name . c_str(), label . c_str());



      // 
      return false;

    }

  }



  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.add_files")))
  {

    // Calls the function for opening a new ConfigurationGUI window.
    file_choosers() . Add_Files();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.close_secondary")))
  { 

    //
    if(!main_window_)
    {

      // 
      return false;

    }

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.configuration")))
  {

    // Calls the function for opening a new ConfigurationGUI window.
    config_guis() . Open_Configuration();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.copy")))
  {

    // 
    playlists() . selected_playlist() . Copy_Selected_Rows();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.cut")))
  {

    // 
    playlists() . selected_playlist() . Cut_Selected_Rows();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.delete")))
  {

    // 
    playlists() . selected_playlist() . Delete_Selected_Rows();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.next")))
  {

    // 
    playback() . Next_Track();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.paste")))
  {

    // 
    playlists() . selected_playlist() . Paste_Clipboard_Rows();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.pause")))
  {

    // 
    playback() . Pause();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.play")))
  {

    // 
    playback() . Play(Gtk::TreeRowReference());

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.select_all")))
  {

    // 
    playlists() . selected_playlist() . Select_All_Rows();

  }

  // 
  if(name == string(config() . get("keyboard_shortcuts.keys.stop")))
  {

    // Calls the function for stopping music playback.
    playback() . Stop();

  }



  // Allows normal keyboard event propagation.
  return false;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

bool KeyboardShortcuts::main_window()
{

  return main_window_;

}

bool KeyboardShortcuts::updating_binding()
{

  return updating_binding_;

}

bool KeyboardShortcuts::skip_window_key_callback()
{

  return skip_window_key_callback_;

}





//         //
//         //
// Setters ////////////////////////////////////////////////////////////////////
//         //
//         //

void KeyboardShortcuts::set_main_window(bool new_value)
{

  main_window_ = new_value;

}

void KeyboardShortcuts::set_updating_binding(bool new_value)
{

  updating_binding_ = new_value;

}

void KeyboardShortcuts::set_skip_window_key_callback(bool new_value)
{

  skip_window_key_callback_ = new_value;

}
