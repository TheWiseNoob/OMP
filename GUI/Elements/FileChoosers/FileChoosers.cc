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

#include "FileChoosers.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../GUI.h"

#include "../ChildWindows/ChildWindow.h"

#include "../ChildWindows/ChildWindows.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

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

FileChoosers::FileChoosers(Base& base)
: GUIElementList(base)
{


}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

FileChoosers::~FileChoosers()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void FileChoosers::Add_Files()
{

  // Creates of new FileChooser pointer.
  FileChooser* temp_file_chooser = new FileChooser(base(), file_choosers());



  // Creates and std function pointer to the destroy function of the new
  // FileChooser.
  std::function<void(void)> temp_func_ptr
    = std::bind(&FileChooser::Destroy, temp_file_chooser);



  // Creates a new shared_ptr<ChildWindow> to hold the FileChooser 
  ChildWindow* new_window;

  // Creates the new ChildWindow.
  new_window = windows() . Create_New_Window("Add File(s)", temp_func_ptr);



  // Adds a cancel button to the FileChooser window.
  temp_file_chooser -> Enable_Cancel_Button(new_window);



  // Adds the new FileChooser to the new ChildWindow.
  new_window -> box() . pack_start(temp_file_chooser -> box(),
                                   Gtk::PACK_EXPAND_WIDGET);



  // Displays the new ChildWindow and its contents.
  new_window -> Show();

}
