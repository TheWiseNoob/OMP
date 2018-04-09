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

#include "About.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "Abouts.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/frame.h>

#include <gtkmm/image.h>

#include <gtkmm/label.h>

#include <gtkmm/textview.h>





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

About::About(Base& base_ref, Abouts& abouts_ref)

// Inherited Class

: GUIElement(base_ref, abouts_ref())

{

  // 
  abouts_ref() . push_front(this);



  // 
  box() . set_orientation(Gtk::ORIENTATION_VERTICAL);



  // 
  auto omp_pixbuf 
    = Gdk::Pixbuf::create_from_file("/usr/share/pixmaps/OMP_Icon_128.png", 
                                    128, 128, true);

  // 
  Gtk::Image* omp_logo = Gtk::manage(new Gtk::Image(omp_pixbuf));

  // 
  box() . pack_start(*omp_logo, Gtk::PACK_EXPAND_WIDGET);



  // 
  Gtk::Label* version_label = Gtk::manage(new Gtk::Label);

  // 
  version_label -> set_markup("<b>OMP Version 0.0.8 (Pre-Alpha)</b>");

  // 
  box() . pack_start(*version_label, Gtk::PACK_EXPAND_PADDING);



  // 
  Gtk::Frame* contributors_text_view_frame = Gtk::manage(new Gtk::Frame);

  // 
  Gtk::TextView* contributors_text_view = Gtk::manage(new Gtk::TextView);

  // 
  contributors_text_view -> set_editable(false);

  // 
  box() . pack_start(*contributors_text_view_frame, Gtk::PACK_EXPAND_WIDGET);

  // 
  contributors_text_view_frame -> add(*contributors_text_view);



  //
  auto contributors_text_buffer = Gtk::TextBuffer::create();

  // 
  contributors_text_buffer
    -> set_text("Contributors:\n\n" \
                " Lead Developer: \n" \
                "    DJ Griffin\n\n" \
                " GIT Contributors: \n" \
                "    aladar42\n" \
                "    AxelSilverdew\n" \
                "    FabioLolix\n" \
                "    mmetak");

  // 
  contributors_text_view -> set_buffer(contributors_text_buffer);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //


About::~About()
{

}
