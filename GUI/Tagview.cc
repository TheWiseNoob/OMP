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





#include "Tagview.h"

#include "GUI.h"
#include "../Metadata/Track.h"

#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gdkmm/rgba.h>
#include <pangomm.h>
#include <string>
#include <list>




using namespace std;






Tagview::Tagview(GUI &new_gui)
: gui_(new_gui)
{

  tag_frame_ = Gtk::manage(new Gtk::Frame);
  tag_frame_label_ = Gtk::manage(new Gtk::Label);
  tag_label_ = Gtk::manage(new Gtk::Label);
  tag_frame_rgba_ = new Gdk::RGBA;
  tag_frame_label_fontdescription_ = new Pango::FontDescription;


  pack_start(*tag_frame_, Gtk::PACK_EXPAND_WIDGET);
  tag_frame_ -> set_label_widget(*tag_frame_label_);

  tag_frame_label_ -> set_text(" Selected : ");

  tag_frame_ -> add(*tag_label_);
  
  tag_label_ 
    -> set_text("No track is selected!\n"),

//  tag_frame_rgba_ -> set_rgba(1.0,1.0,1.0,1.0);

  tag_frame_ 
    -> override_background_color(*tag_frame_rgba_, Gtk::STATE_FLAG_NORMAL);

  tag_frame_label_fontdescription_ -> set_weight(Pango::Weight::WEIGHT_BOLD);

  tag_frame_label_ -> override_font(*tag_frame_label_fontdescription_);
   
  set_margin_left(5);
  set_margin_right(5);
  set_margin_top(5);
  set_margin_bottom(5);

}





Tagview::~Tagview()
{




}





void Tagview::update_tags(const char *new_frame_label_name, Track& new_track)
{

  list<Tagview*>::iterator tagviews_it = gui().tagviews().begin();

  Glib::ustring* artists = new_track.artists_string();

  for(list<Tagview*>::iterator tagviews_it = gui().tagviews().begin();
      tagviews_it != gui().tagviews().end();
      tagviews_it++)
  { 

      (*tagviews_it) -> tag_frame_label()
        . set_label(" " + string(new_frame_label_name) + " : ");



    // 
    if(new_track.Is_Empty())
    {

      //
      (*tagviews_it) -> tag_label() . set_label("No track is selected!\n");

    }

    // 
    else
    {

      (*tagviews_it) -> tag_label() 
        . set_label("Title: " + new_track.title() + "\n" +
                    "Track #: " + to_string(new_track.track_number()) + "\n" +
                    "Artist: " + *artists + "\n" +
                    "Album: " + new_track.album() + "\n"); 

    }

  }



  // 
  delete artists;

}
