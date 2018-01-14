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





#include "Artwork.h"





#include "../Base.h"

#include <iostream>
#include <pwd.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>






using namespace std;





Artwork::Artwork(Base& base_ref)
: Parts(base_ref)
{

  set_unscaled_image = true;

  first_iteration = true;

  finished_first_draw = false;

  resizing_image.store(false, memory_order_relaxed);
  setting_image_filename.store(false, memory_order_relaxed);

  current_Box_width = 420;

  saved_Box_width = 420; 

  current_Box_height = 420;

  saved_Box_height = 420;  



  // 
  struct passwd* pw = getpwuid(getuid());

  // 
  const char* homedir = pw -> pw_dir;

  // 
  directory_str_ = homedir;

  // 
  directory_str_ += "/.omp";



  cover_file = "/usr/share/pixmaps/no_cover.png"; 

             
  saved_cover_file = cover_file; 

  art_AspectFrame.add(inner_art_Box); 

  inner_art_Box.pack_start(*this, Gtk::PACK_EXPAND_WIDGET);


  art_Box.pack_start(art_AspectFrame, Gtk::PACK_EXPAND_WIDGET);
  art_Box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  art_Box.set_baseline_position(Gtk::BASELINE_POSITION_BOTTOM);


  set_size_request(20, 
                   20);


  art_AspectFrame.set_margin_top(0);
  art_AspectFrame.set_margin_start(0);
 
  art_AspectFrame.unset_label();

  inner_art_Box.set_vexpand(true);
  inner_art_Box.set_hexpand(true);


  art_Box_Allocation = inner_art_Box.get_allocation();                      
                                                                                
  current_Box_width = art_Box_Allocation.get_width();                       
                                                                                
  current_Box_height = art_Box_Allocation.get_height(); 

  saved_Box_width = current_Box_width;                                        
                                                                                
  saved_Box_height = current_Box_height;                                      
                                                                                
                                                                                
  art_Pixbuf = Gdk::Pixbuf::create_from_file(cover_file,                      
                                             (saved_Box_width),               
                                             (saved_Box_height),              
                                             true);                           
                                                          

  
  debug("Before Artwork thread creation");
  
  
  art_thread = new thread(&Artwork::resize_loop, this);

  queue_draw();

  show();

}





bool Artwork::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{

  if(!art_Pixbuf)
  {

    return false;

  }

  while((resizing_image.load()))                                                
  {                                                                             
                                                                                
                                                                                
                                                                                
  }                                                                             
                                                                                
  setting_image_filename.store(true, memory_order_relaxed);                     

  art_AspectFrame.set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
                      (float(art_Pixbuf -> get_width())                             
                        / float(art_Pixbuf -> get_height())),                       
                      false);   
                                                                                
 /* 
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
*/
  // Draw the image in the middle of the drawing area, or (if the image is
  // larger than the drawing area) draw the middle part of the image.
  Gdk::Cairo::set_source_pixbuf(cr, 
                                art_Pixbuf,
                                (0), 
                                (0));


  cr -> paint();
   

//  queue_resize();

  setting_image_filename.store(false, memory_order_relaxed);

  if(!finished_first_draw)
  {

    finished_first_draw = true;

  }

  return true;

}






void Artwork::set_image_filename(string new_image_filename)
{

  debug("Beginning of set_image_filename");

  while((resizing_image.load()))
  {

    

  }

  setting_image_filename.store(true, memory_order_relaxed);

  cover_file = new_image_filename;

  set_unscaled_image = true;  

  setting_image_filename.store(false, memory_order_relaxed);



  debug("End of set_image_filename");

}





void Artwork::resize_loop()
{

  for (int i = 0; ; ++i)
  {

    //Ensures fast as possible loading on first iteration.
    if(first_iteration)
    {

      first_iteration = false;

    }
    else
    {

      Glib::usleep(20000);

    }

    if(setting_image_filename.load())
    {

    }
    else if(!((*this).is_visible()))
    {
   
    }
    else if(!finished_first_draw)
    {

      

    }
    else
    {

      resizing_image.store(true, memory_order_relaxed);

      art_Box_Allocation = inner_art_Box.get_allocation();

      current_Box_width = art_Box_Allocation.get_width(); 

      current_Box_height = art_Box_Allocation.get_height();

      resize_image();

      resizing_image.store(false, memory_order_relaxed);;

    }

  }

}





void Artwork::resize_image()
{

  if(saved_cover_file != cover_file)
  {

    set_unscaled_image = true; 

  }
  else if(
          (saved_Box_width != current_Box_width)
            ||
          (saved_Box_height != current_Box_height)
         )
  {

    
    saved_Box_width = current_Box_width;

    saved_Box_height = current_Box_height;


    art_Pixbuf = (art_Pixbuf -> scale_simple((saved_Box_width),
                                             (saved_Box_height),
                                             (Gdk::INTERP_HYPER)));



    debug("scaled!");

    queue_draw();
/*
    art_AspectFrame.set(Gtk::ALIGN_START, Gtk::ALIGN_START,
                        ((get_allocated_width())                                      
                          /                                                     
                        float(get_allocated_height())), true);

*/

    if(cover_file == "/usr/share/pixmaps/no_cover.png")
    {

//      set(art_Pixbuf);

    }
    else
    {

  //    set(art_Pixbuf);

    }

    if(!set_unscaled_image)
    {

      set_unscaled_image = true;

    }

    return;

  }

  if(set_unscaled_image)
  {

    saved_cover_file = cover_file;

    debug("in settings unscaled image");

    art_Box_Allocation = inner_art_Box.get_allocation();                    
                                                                                
    current_Box_width = art_Box_Allocation.get_width();                       
                                                                                
    current_Box_height = art_Box_Allocation.get_height(); 

    saved_Box_width = current_Box_width;

    saved_Box_height = current_Box_height;

    debug("Before setting unscaled image");


    art_Pixbuf = Gdk::Pixbuf::create_from_file(cover_file,(saved_Box_width),
                                               (saved_Box_height), true);



    stringstream debug_ss;

    debug_ss << "art Pixbuf width: " << art_Pixbuf -> get_width()              
             << "\nart Pixbuf height: " << art_Pixbuf -> get_height(); 

    debug(debug_ss . str() . c_str());



    queue_draw();



    debug_ss << "art Pixbuf width: " << art_Pixbuf -> get_width()               
             << "\nart Pixbuf height: " << art_Pixbuf -> get_height();

    debug(debug_ss . str() . c_str());



    debug_ss << "art_AspectFrame width: " << art_AspectFrame.get_width()               
             << "\nart_AspectFrame height: " << art_AspectFrame.get_height();

    debug(debug_ss . str() . c_str());



    debug_ss << "get_allocated_width(): " << get_allocated_width()
             << "\nget_allocated_height(): " << get_allocated_height();

    debug(debug_ss . str() . c_str());



    debug_ss << "inner_art_Box width: " << inner_art_Box.get_allocated_width()              
             << "\ninner_art_Box height: " << inner_art_Box.get_allocated_height();

    debug(debug_ss . str() . c_str());



    debug("After setting unscaled image");



    set_unscaled_image = false;



    debug("End of settings unscaled");

  }  

}
