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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef BASE_H
#define BASE_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class Configuration;

class Errors;

namespace Gio
{

  class ApplicationCommandLine;

}

class GUI;

namespace Gtk
{

  class Application;

}

class Metadata;

class Playback;

class Scrobbling;

class TimeConversion;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Base
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    Base(int argc, char* argv[]);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~Base();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    const char* config_directory_c_str();

    GUI& gui();

    Metadata& metadata();

    Playback& playback();

    Scrobbling& scrobbling();

    TimeConversion& time_converter();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  private:

    // 
    Configuration* config_;

    // 
    char config_directory_c_str_[400];

    // 
    Errors* errors_;

    // 
    GUI* gui_;

    // 
    Metadata* metadata_;

    // 
    Playback* playback_;

    // 
    std::atomic<bool> quitting_;

    // 
    Scrobbling* scrobbling_;

    // 
    TimeConversion* time_converter_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
