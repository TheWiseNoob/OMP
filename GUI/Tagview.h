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

#ifndef TAGVIEW_H
#define TAGVIEW_H





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





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class Base;

namespace Gdk
{

  class RGBA;

}

class GUI;

namespace Gtk
{

  class Box;

  class Frame;

  class Label;

}

namespace Pango
{

  class FontDescription;

}

class Track;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Tagview : public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    Tagview() = delete;

    Tagview(Base& base_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~Tagview();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    void Update_Tags(const char* new_frame_label_name, Track& new_track);




 
  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::Box& box();

    Gtk::Label& tag_frame_label();

    Gtk::Label& tag_label_center();

    Gtk::Label& tag_label_end_left();

    Gtk::Label& tag_label_middle_right();

    Gtk::Label& tag_label_middle_left();

    Gtk::Label& tag_label_end_right();

    Gtk::Label& tag_label_start_left();

    Gtk::Label& tag_label_start_right();

    Gtk::Box& tag_labels_box();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  private:

    Gtk::Box* box_;

    Gtk::Frame* tag_frame_;

    Gtk::Label* tag_frame_label_;

    Gtk::Label* tag_label_center_;

    Gtk::Label* tag_label_end_left_;

    Gtk::Label* tag_label_end_right_;

    Gtk::Label* tag_label_middle_left_;

    Gtk::Label* tag_label_middle_right_;

    Gtk::Label* tag_label_start_left_;

    Gtk::Label* tag_label_start_right_;

    Gtk::Box* tag_labels_box_;

    Gdk::RGBA* tag_frame_rgba_;

    Pango::FontDescription* tag_frame_label_fontdescription_; 

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
