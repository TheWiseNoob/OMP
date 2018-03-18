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

#ifndef ARTWORK_H
#define ARTWORK_H





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

#include <gtkmm/drawingarea.h>





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <mutex>

#include <string>

#include <thread>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

namespace Gdk
{

  class Pixbuf;

}

namespace Gtk
{

  class AspectFrame;

  class Box;

}





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Artwork : public Gtk::DrawingArea, public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    Artwork(Base& base_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~Artwork();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //                                 //
  // Overloaded Base Class Functions //////////////////////////////////////////
  //                                 //

  public:

    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);





  //        //
  // Normal ///////////////////////////////////////////////////////////////////
  //        //

  public:

    void Resize_Image();    

    void Resize_Loop();

    void Set_Image_Filename(std::string new_image_filename);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::Box& box();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //            //
  // Dimensions ///////////////////////////////////////////////////////////////
  //            //

  private:

    int current_box_height_;

    int current_box_width_;

    int saved_box_height_;

    int saved_box_width_;





  //       //
  // Flags ////////////////////////////////////////////////////////////////////
  //       //

  public:

    bool finished_first_draw_;

    bool first_iteration_;

    std::atomic<bool> resizing_image_;

    bool set_unscaled_image_;

    std::atomic<bool> setting_image_filename_;





  //           //
  // GUI Parts ////////////////////////////////////////////////////////////////
  //           //

  public:

    Gtk::Box* art_box_;

    Gtk::AspectFrame* art_aspect_frame_;

    Glib::RefPtr<Gdk::Pixbuf> art_pixbuf_;

    Gtk::Box* inner_art_box_;





  //                 //
  // Image Filenames //////////////////////////////////////////////////////////
  //                 //

  public:

    std::string cover_file_;

    std::string saved_cover_file_;





  //              //
  // Thread Parts /////////////////////////////////////////////////////////////
  //              //

  public:

    std::thread* art_thread_;

    std::mutex resize_mutex_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
