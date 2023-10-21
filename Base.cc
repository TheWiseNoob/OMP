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

#include "Base.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "GUI/GUI.h"

#include "Metadata/Metadata.h"

#include "Playback/Playback.h"

#include "Scrobbling/Scrobbling.h"

#include "Metadata/TimeConversion.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <gtkmm/filechooserwidget.h>

#include <gtkmm/messagedialog.h>

#include <pwd.h>

#include <string>

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>





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

Base::Base(int argc, char *argv[])

// 

// : application_(new_application)

// , quitting_(false)

{


}





//                  //
//                  //
// Member Functions /////////////////////////////////////////////////////////
//                  //
//                  //





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Base::~Base()
{

  quitting_ = true;

  delete gui_;

  delete playback_;

  // delete scrobbling_;

  delete metadata_;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

const char* Base::config_directory_c_str()
{

  return config_directory_c_str_;

}

GUI& Base::gui()
{

  return *gui_;

}

Metadata& Base::metadata()
{ 

  return *metadata_; 

}

Playback& Base::playback()
{

  return *playback_;

}

atomic<bool>& Base::quitting()
{

  return quitting_;

}

Scrobbling& Base::scrobbling()
{ 

  return *scrobbling_; 

}

TimeConversion& Base::time_converter()
{ 

  return *time_converter_; 

}
