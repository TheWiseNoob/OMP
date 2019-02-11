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

#include "Abouts.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../ChildWindows/ChildWindow.h"

#include "../ChildWindows/ChildWindows.h"

#include "../../GUI.h"






//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <functional>

#include <gtkmm/applicationwindow.h>

#include <memory>






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

Abouts::Abouts(Base& base_ref)

// Inherited Class

: GUIElementList(base_ref)

{

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Abouts::~Abouts()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void Abouts::Open_About()
{

  // Creates a temporary ConfigGUI pointer.
  About* new_about;

  // Assigns a new ConfigGUI object to the pointer.
  new_about = new About(base(), (*this));



  // Binds the ConfigGUI's Destroy function a std::function pointer.
  std::function<void(void)> new_destroy_func_ptr
    = std::bind(&About::Destroy, new_about);

  // Creates of new window using the Create_New_Window function.
  ChildWindow* new_child_window
    = windows() . Create("About OMP", new_destroy_func_ptr);



  // Adds the ConfigGUI to the new window. 
  new_child_window -> box() . pack_start(new_about -> box(),
                                         Gtk::PACK_EXPAND_WIDGET);



  // 
  new_child_window -> window() . set_default_size(360, 420);

  // 
  new_child_window -> window() . set_resizable(false);



  // Displays the new window.
  new_child_window -> Show();

}
