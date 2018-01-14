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





#ifndef CHILD_WINDOW_H
#define CHILD_WINDOW_H





#include "../Parts.h"



#include <gtkmm/box.h>
#include <gtkmm/applicationwindow.h>
#include <list>
#include <memory>



class Windows;
class Flags;
class GUI;

using namespace std;





class ChildWindow : public Parts
{ 

  public:


    ChildWindow(const char* new_title,
                Base& new_main,
                std::function<void(void)> 
                    new_child_class_destroy_function, 
                bool set_main_window = false);

    ~ChildWindow();

    void show();


    bool on_irregular_quit(GdkEventAny* event);                        

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


    void set_default_size(int width, int height)
    { 

      window_.set_default_size(width, height); 

    }

    void set_title(const char* new_title)
    { 

      window_.set_title(new_title); 

    }

    void set_location(list<shared_ptr<ChildWindow>>::iterator new_it)
    { 

      windows_it_ = new_it; 

    }

    Gtk::ApplicationWindow& window()
    {

      return window_;

    }

    Gtk::Box& box()
    { 

      return box_; 

    }

  private:

    bool main_window_;

    std::function <void(void)> child_class_destroy_function_;

    Gtk::Box box_;

    Gtk::ApplicationWindow window_;

    list<shared_ptr<ChildWindow>>::iterator windows_it_;

};





#endif
