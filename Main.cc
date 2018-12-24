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

//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "Base.h"

#include "GUI/Elements/ChildWindows/ChildWindow.h"

#include "GUI/Elements/ChildWindows/ChildWindows.h"

#include "GUI/GUI.h"



//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm.h>

#include <gtkmm.h>

#include <gtkmm/application.h>

#include <gtkmm/applicationwindow.h>

#include <glibmm/refptr.h>

#include <iostream>





//      //
//      //
//      //
// Main ///////////////////////////////////////////////////////////////////////
//      //
//      //
//      //

int main (int argc, char *argv[])
{  

  // Created the Gtk::Application instance that is OMP's GUI.
  Glib::RefPtr<Gtk::Application> application 
    = Gtk::Application::create
        ("com.openmusicplayer.OMP"/*,    // Uncomment this for multiple OMP instances.
         Gio::APPLICATION_NON_UNIQUE*/);



  // The base class of OMP.
  Base base(argc, argv, application);



  // 
  application -> signal_command_line()
    . connect(sigc::mem_fun(base, &Base::New_Command), false);

  // 
  application -> signal_startup()
    . connect(sigc::mem_fun(base, &Base::OMP_Started));



  // Prevents application from closing after hidden. use release() to close
  application -> hold();



  // Shows the window and returns when it is closed.
  return application -> run(base . gui() . windows()() . front() -> window(), argc, argv);

}
