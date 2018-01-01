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
//  The developer(s) of the Moyσikh audio player hereby grant(s) permission
//  for non-GPL compatible GStreamer plugins to be used and distributed
//  together with GStreamer and Moyσikh. This permission is above and beyond
//  the permissions granted by the GPL license by which Moyσikh is covered.
//  If you modify this code, you may extend this exception to your version
//  of the code, but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version.
//
//  Libraries used by Moyσikh:
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





#ifndef ARTWORK_H
#define ARTWORK_H





#include "../Parts.h"

#include <gtkmm.h>
#include <gdkmm/pixbuf.h>
#include <atomic>
#include <string>
#include <thread>





class Artwork : public Gtk::DrawingArea, public Parts
{

  public:

    Artwork(Base& base_ref);

    ~Artwork()
    {

      art_thread -> detach();

    }

    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

    void set_image_filename(std::string new_image_filename);

    void resize_loop();

    void resize_image();    

    Gtk::Box &get_art_Box()
    { return art_Box; }


  private:

    int saved_Box_width,
        current_Box_width,
        saved_Box_height,
        current_Box_height;


    bool set_unscaled_image,
         first_iteration,
         finished_first_draw;


    std::atomic<bool> resizing_image,
                      setting_image_filename;

    std::string cover_file,
                saved_cover_file;

    Gtk::Box art_Box,
             inner_art_Box;


    std::thread *art_thread;


    std::string directory_str_;

    Gtk::AspectFrame art_AspectFrame;


    Glib::RefPtr<Gdk::Pixbuf> art_Pixbuf;

    Gtk::Allocation art_Box_Allocation;

};





#endif

