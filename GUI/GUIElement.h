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





#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H



//Base Class Headers

#include "../Parts.h"



#include "../Base.h"



#include <gtkmm/box.h>




template<class datatype>
class GUIElement : public Parts 
{ 

  protected:

    GUIElement(Base& base, 
               typename std::list<datatype*>& new_gui_element_list, bool debug = false)
    : Parts(base, debug)
    , gui_element_list_(new_gui_element_list)
    , box_(Gtk::manage(new Gtk::Box))
    {

      box_ = Gtk::manage(new Gtk::Box);

    }

    virtual ~GUIElement()
    {

      gui_element_list_ . erase(gui_elements_it_);

    }

  public:
 
  //
  //Public Member Functions
  //

    virtual void Destroy()
    {

      delete *gui_elements_it_;

    }


  //
  //Getters and Setters
  //

    virtual Gtk::Box& box()
    {

      return *box_;

    }

    virtual typename std::list<datatype*>::iterator& gui_elements_it() final
    { 

      return gui_elements_it_; 

    }

    virtual void set_gui_elements_it(typename std::list<datatype*>::iterator
                                       new_gui_elements_it) final
    { 

      gui_elements_it_ = new_gui_elements_it; 

    }

  private:

    typename std::list<datatype*>& gui_element_list_;

    Gtk::Box* box_;

    typename std::list<datatype*>::iterator gui_elements_it_;


};





#endif
