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

#include "ChildWindow.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "GUI.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/applicationwindow.h>

#include <gtkmm/box.h>

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

ChildWindow::ChildWindow
  (const char* new_title, Base& new_main, std::function<void(void)> 
   new_child_class_destroy_function, bool make_main_window)

// Inherited Class

: Parts(new_main)



// Member Variables

, box_(Gtk::manage(new Gtk::Box))

, window_(new Gtk::ApplicationWindow)

{ 

  // 
  main_window_ = make_main_window;

  // 
  if(!main_window_)
  { 

    // 
    window_ -> signal_key_press_event()
      . connect(sigc::mem_fun(*this, &ChildWindow::On_Key_Press_Event));

  }



  // 
  window_ -> set_title(new_title);

  // 
  window_ -> set_position(Gtk::WIN_POS_CENTER);

  // 
  window_ -> set_default_size(650,400);  



  debug("ChildWindow shown");



  // 
  window_ -> signal_delete_event()
    . connect(sigc::mem_fun(*this, &ChildWindow::On_Irregular_Quit));



  // 
  child_class_destroy_function_ = new_child_class_destroy_function;



  // 
  window_ -> add(*box_);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

ChildWindow::~ChildWindow()
{

  if(!main_window_)
  {

    gui() . windows() . erase(windows_it_);

  }



  // 
  delete window_;

 }





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

bool ChildWindow::On_Key_Press_Event(GdkEventKey* event)
{

  // Is true if the escape key is pressed.
  if((event -> keyval == GDK_KEY_Escape))
  {

    // 
    On_Irregular_Quit(NULL);



    // 
    return true;

  }



  // Allows normal keyboard event propagation.
  return false;

} 

bool ChildWindow::On_Irregular_Quit(GdkEventAny* event)                          
{                                                                                
                                                                                
  // 
  if(child_class_destroy_function_)
  {

    // 
    child_class_destroy_function_();

  } 



  // 
  window_ -> hide();



  // 
  return true;

}

void ChildWindow::Show()
{

  window_ -> show();

  window_ -> show_all_children();

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::Box& ChildWindow::box()
{

  return *box_; 

}

Gtk::ApplicationWindow& ChildWindow::window()
{

  return *window_;

}





//         //
//         //
// Setters //////////////////////////////////////////////////////////////////
//         //
//         //

void ChildWindow::set_default_size(int width, int height)
{ 

  window_ -> set_default_size(width, height); 

}

void ChildWindow::set_location(std::list<std::shared_ptr<ChildWindow>>::iterator new_it)
{ 

  windows_it_ = new_it; 

}

void ChildWindow::set_title(const char* new_title)
{

  window_ -> set_title(new_title); 

}
