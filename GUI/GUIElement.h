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

#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H





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
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <gtkmm/box.h>





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

template<class datatype>
class GUIElement : public Parts 
{ 

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  protected:

    GUIElement(Base& base, typename std::list<datatype*>& new_gui_element_list,
               bool debug = false)

    // 

    : Parts(base, debug)



    // 

    , box_(new Gtk::Box)

    , gui_element_list_(new_gui_element_list)

    , quitting_finished_(true)

    { 

    }





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  protected:

    virtual ~GUIElement()
    {

      gui_element_list_ . erase(gui_elements_it_);



      // 
      delete box_;

    }





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    virtual void Destroy()
    {

      // 
      if(!quitting_finished_)
      {

        // 
        return;

      }




      // 
      delete *gui_elements_it_;

    }





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:
 
    virtual Gtk::Box& box()
    {

      return *box_;

    }

    virtual typename std::list<datatype*>::iterator& gui_elements_it() final
    { 

      return gui_elements_it_; 

    }

    virtual std::atomic<bool>& quitting_finished()
    {

      return quitting_finished_;

    }





  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:
 
    virtual void set_gui_elements_it
      (typename std::list<datatype*>::iterator new_gui_elements_it) final
    { 

      gui_elements_it_ = new_gui_elements_it; 

    }





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //
  
  private:

    Gtk::Box* box_;

    typename std::list<datatype*>::iterator gui_elements_it_;

    typename std::list<datatype*>& gui_element_list_;

    std::atomic<bool> quitting_finished_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
