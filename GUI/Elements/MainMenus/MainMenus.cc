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

#include "MainMenus.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Configuration/Configuration.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //





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

MainMenus::MainMenus(Base& base_ref)

// Inherited Class

: GUIElementList(base_ref)



// General

, disable_menubar_functions_flag_(false)

, system_menu_(new MainMenu(base_ref, *this, true))

{

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

MainMenus::~MainMenus()
{

}




//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void MainMenus::Apply_Saved_Values()
{

  // 
  for(auto main_menus_it : (*this)())
  {

    // 
    main_menus_it -> Apply_Saved_Values();

  }

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

bool MainMenus::disable_menubar_functions_flag()
{

  return disable_menubar_functions_flag_;

}

MainMenu& MainMenus::system_menu()
{

  // 
  return *system_menu_;

}





//         //
//         //
// Setters ////////////////////////////////////////////////////////////////////
//         //
//         //

void MainMenus::set_disable_menubar_functions_flag(bool new_setting)
{ 

  disable_menubar_functions_flag_ = new_setting; 

}
