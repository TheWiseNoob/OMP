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

#ifndef GUI_H
#define GUI_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                 //
//                 //
// Inherited Class ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Parts.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <functional>

#include <list>

#include <vector>

#include <memory>

#include <mutex>

#include <string>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

//         //
//         //
// Classes ////////////////////////////////////////////////////////////////////
//         //
//         //

class Abouts;

class Artwork;

class Base;

class ChildWindow;

class ChildWindows;

class Configuration;

class ConfigurationGUIs;

class FileChooser;

class FileChoosers;

class Flags;

namespace Glib
{

  class ustring;

}

namespace Gtk
{

  class Box;

  class Button;

  class EventBox;

  class Frame;

  class HeaderBar;

  class Label;

  class Notebook;

  class Paned;

  class SpinButton;

  class StackSwitcher;

  class VolumeButton;

  class Window;

}

class MainMenus;

class Metadata;

class Playback;

class PlaybackController;

class PlaybackControllers;

class PlaylistComboBoxes;

class Playlists;

class Seekbar;

class StatusBar;

class Tagview;

class Track;





//         //
//         //
// Structs ////////////////////////////////////////////////////////////////////
//         //
//         //

//           //
// GStreamer //////////////////////////////////////////////////////////////////
//           //

struct _GdkEventConfigure;
typedef struct _GdkEventConfigure GdkEventConfigure;

struct _GdkEventConfigure;
typedef struct _GdkEventConfigure GdkEventConfigure;

struct _GdkEventKey;
typedef struct _GdkEventKey GdkEventKey;

struct _GdkEventCrossing;
typedef struct _GdkEventCrossing GdkEventCrossing;

struct _GdkEventButton;
typedef struct _GdkEventButton GdkEventButton;

struct _GdkEventAny;
typedef struct _GdkEventAny GdkEventAny;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class GUI : public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:  
    //
    //   Creates the class.
    //
    //
    //
    // Arguments: 
    //
    //   base: passes a reference to the Base of the program.
    //
    //////////////////////////////////////////////////////////////////////// */
    GUI(Base& base_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Cleans up anything that needs it when the GUI is being destroyed.
    //
    //
    //
    // Arguments: 
    //
    //   None
    //
    //////////////////////////////////////////////////////////////////////// */
    virtual ~GUI();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //        //
  // Normal ///////////////////////////////////////////////////////////////////
  //        //

  public:


};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
