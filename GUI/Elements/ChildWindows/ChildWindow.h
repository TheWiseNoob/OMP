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

#ifndef CHILD_WINDOW_H
#define CHILD_WINDOW_H





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

#include "../../GUIElement.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <functional>

#include <list>

#include <memory>





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

//                            //
//                            //
// Class Forward Declarations /////////////////////////////////////////////////
//                            //
//                            //

class ChildWindows;

class Flags;

namespace Gtk
{

  class ApplicationWindow;

  class Box;

}

class GUI;





//                             //
//                             //
// Struct Forward Declarations ////////////////////////////////////////////////
//                             //
//                             //

struct _GdkEventAny;
typedef struct _GdkEventAny GdkEventAny;

struct _GdkEventButton;
typedef struct _GdkEventButton GdkEventButton;

struct _GdkEventKey;
typedef struct _GdkEventKey GdkEventKey;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class ChildWindow : public GUIElement<ChildWindow>
{ 

  //              //
  //              //
  // Constructors //////////////////////////////////////////////////////////////
  //              //
  //              //

  public:

    ChildWindow
      (const char* new_title, Base& base_ref, ChildWindows& child_windows_ref,
       std::function<void(void)> new_child_class_destroy_function,
       bool set_main_window = false);

    ChildWindow
      (const char* window_name, 
       std::function<void(void)> new_child_class_destroy_function);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    virtual ~ChildWindow();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    bool On_Button_Press_Event(GdkEventButton* event);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Called when a keyboard press occurs. Used to assign keyboard 
    //   shortcuts to various functions.
    //
    // 
    //
    // Arguments: 
    //
    //   event: Holds the event type and GDK window.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool On_Key_Press_Event(GdkEventKey* event);

    bool On_Irregular_Quit(GdkEventAny* event);                        

    void Show();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::Box& box();

    bool fullscreen();

    Gtk::ApplicationWindow& window();





  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    void set_default_size(int width, int height);

    void set_fullscreen(bool new_fullscreen);

    void set_title(const char* new_title);





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  private:

    Gtk::Box* box_;

    std::function<void(void)> child_class_destroy_function_;

    bool fullscreen_;

    bool main_window_;

    Gtk::ApplicationWindow* window_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
