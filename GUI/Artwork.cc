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

#include "Artwork.h"





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

#include <gtkmm.h>

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

Artwork::Artwork(Base& base_ref)

// Inherited Class

: Parts(base_ref)



// Dimensions

, current_box_height_(420)

, current_box_width_(420)

, saved_box_height_(420)

, saved_box_width_(420)



// Flags

, finished_first_draw_(false)

, first_iteration_(true)

, resizing_image_(false)

, set_unscaled_image_(true)

, setting_image_filename_(false)



// GUI Parts

, art_box_(Gtk::manage(new Gtk::Box))

, art_aspect_frame_(Gtk::manage(new Gtk::AspectFrame))

, inner_art_box_(Gtk::manage(new Gtk::Box))

{

  // 
  cover_file_ = "/usr/share/pixmaps/No_Cover.png";

  // 
  saved_cover_file_ = cover_file_;



  // 
  art_aspect_frame_ -> add(*inner_art_box_);

  // 
  inner_art_box_ -> pack_start(*this, Gtk::PACK_EXPAND_WIDGET);



  // 
  art_box_ -> pack_start(*art_aspect_frame_, Gtk::PACK_EXPAND_WIDGET);

  // 
  art_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  // 
  inner_art_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // 
  art_box_ -> set_baseline_position(Gtk::BASELINE_POSITION_BOTTOM);



  // 
  set_size_request(20, 20);




  // 
  art_aspect_frame_ -> set_margin_top(0);

  // 
  art_aspect_frame_ -> set_margin_start(0);

  // 
  art_aspect_frame_ -> unset_label();



  // 
  inner_art_box_ -> set_vexpand(true);

  // 
  art_box_ -> set_hexpand(true);



  // 
  current_box_width_
    = art_aspect_frame_ -> get_allocation() . get_width();

  // 
  current_box_height_
    = art_aspect_frame_ -> get_allocation() .  get_height();

  // 
  saved_box_width_ = current_box_width_;

  // 
  saved_box_height_ = current_box_height_;



  // 
  art_pixbuf_
    = Gdk::Pixbuf::create_from_file(cover_file_, saved_box_width_,
                                    saved_box_height_, true);



  debug("Before Artwork thread creation");



  // 
  art_thread_ = new thread(&Artwork::Resize_Loop, this);



  // 
  queue_draw();



  // 
  show();

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Artwork::~Artwork()
{

  art_thread_ -> detach();

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//                                 //
// Overloaded Base Class Functions ////////////////////////////////////////////
//                                 //

bool Artwork::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{

  // 
  if(!art_pixbuf_)
  { 

    return false;

  }



  // 
  while(resizing_image_)
  {

  }



  // 
  setting_image_filename_ = true;



  // Draw the image in the middle of the drawing area, or (if the image is
  // larger than the drawing area) draw the middle part of the image.
  Gdk::Cairo::set_source_pixbuf(cr, art_pixbuf_, 0, 0);



  // 
  cr -> paint();



  // 
  setting_image_filename_ = false;



  // 
  if(!finished_first_draw_)
  {

    // 
    finished_first_draw_ = true;

  }



  // 
  return true;

}





//        //
// Normal /////////////////////////////////////////////////////////////////////
//        //

void Artwork::Resize_Image()
{

  // 
  if(saved_cover_file_ != cover_file_)
  {

    set_unscaled_image_ = true;

  }

  // 
  else if((saved_box_width_ != current_box_width_)
            ||
          (saved_box_height_ != current_box_height_))
  {

    // 
    saved_box_height_ = current_box_height_;

    // 
    saved_box_width_ = current_box_width_;



    // 
    if(!set_unscaled_image_)
    {

      // 
      set_unscaled_image_ = true;

    }



    // 
    return;

  }



  // 
  if(set_unscaled_image_)
  {

    // 
    saved_cover_file_ = cover_file_;



    debug("In setting unscaled image code!");



    // 
    current_box_width_
      = art_aspect_frame_ -> get_allocation() . get_width();

    // 
    current_box_height_
      = art_aspect_frame_ -> get_allocation() . get_height();

    // 
    saved_box_width_ = current_box_width_;

    // 
    saved_box_height_ = current_box_height_;



    // 
    debug("Before setting unscaled image");



    // 
    art_pixbuf_ = Gdk::Pixbuf::create_from_file(cover_file_, saved_box_width_,
                                               saved_box_height_, true);



    // 
    art_aspect_frame_
      -> set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
             (float(art_pixbuf_ -> get_width())
                / float(art_pixbuf_ -> get_height())), false);



    stringstream debug_ss;

    debug_ss << "art_pixbuf_ width: " << art_pixbuf_ -> get_width()
             << "\nart_pixbuf_ height: " << art_pixbuf_ -> get_height();

    debug(debug_ss . str() . c_str());



    // 
    queue_draw();



    debug_ss << "art_pixbuf_ width: " << art_pixbuf_ -> get_width()
             << "\nart_pixbuf_ height: " << art_pixbuf_ -> get_height();

    debug(debug_ss . str() . c_str());



    debug_ss << "art_aspect_frame_ width: " 
             << art_aspect_frame_ -> get_width()
             << "\nart_aspect_frame_ height: "
             << art_aspect_frame_ -> get_height();

    debug(debug_ss . str() . c_str());



    debug_ss << "get_allocated_width(): " << get_allocated_width()
             << "\nget_allocated_height(): " << get_allocated_height();

    debug(debug_ss . str() . c_str());



    debug_ss << "inner_art_box_ width: "
             << inner_art_box_ -> get_allocated_width()
             << "\ninner_art_box_ height: "
             << inner_art_box_ -> get_allocated_height();

    debug(debug_ss . str() . c_str());



    debug("After setting unscaled image");



    set_unscaled_image_ = false;



    debug("End of settings unscaled");

  }

}

void Artwork::Resize_Loop()
{

  // 
  while(true)
  {

    // Ensures fast as possible loading on first iteration.
    if(first_iteration_)
    {

      // 
      first_iteration_ = false;

    }

    // 
    else
    {

      Glib::usleep(20000);

    }



    if(setting_image_filename_)
    {

    }

    // 
    else if(!((*this) . is_visible()))
    {

    }

    // 
    else if(!finished_first_draw_)
    {


    }

    // 
    else
    {

      // 
      lock_guard<mutex> resize_mutex_lock_guard(resize_mutex_);



      // 
      resizing_image_ = true;



      // 
      current_box_width_
        = art_aspect_frame_ -> get_allocation() . get_width();

      // 
      current_box_height_
        = art_aspect_frame_ -> get_allocation() . get_height();



      // 
      Resize_Image();



      // 
      resizing_image_ = false;

    }

  }

}

void Artwork::Set_Image_Filename(string new_image_filename)
{

  debug("Beginning of Set_Image_Filename");



  // 
  while(resizing_image_)
  {

  }



  // 
  lock_guard<mutex> resize_mutex_lock_guard(resize_mutex_);



  // 
  setting_image_filename_ = true;



  // 
  cover_file_ = new_image_filename;

  // 
  set_unscaled_image_ = true;



  // 
  setting_image_filename_ = false;



  debug("End of Set_Image_Filename");

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::Box& Artwork::box()
{

  return *art_box_;

}
