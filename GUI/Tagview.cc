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

#include "Tagview.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Metadata/Track.h"

#include "GUI.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gdkmm/rgba.h>

#include <gtkmm/box.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <list>

#include <pangomm.h>

#include <string>





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

Tagview::Tagview(Base& base_ref)

// Inherited Class

: Parts(base_ref)



//  

, box_(Gtk::manage(new Gtk::Box))

, tag_frame_(Gtk::manage(new Gtk::Frame))

, tag_frame_label_(Gtk::manage(new Gtk::Label))

, tag_label_center_(new Gtk::Label)

, tag_label_end_left_(Gtk::manage(new Gtk::Label))

, tag_label_end_right_(Gtk::manage(new Gtk::Label))

, tag_label_middle_left_(Gtk::manage(new Gtk::Label))

, tag_label_middle_right_(Gtk::manage(new Gtk::Label))

, tag_label_start_left_(Gtk::manage(new Gtk::Label))

, tag_label_start_right_(Gtk::manage(new Gtk::Label))

, tag_labels_box_(Gtk::manage(new Gtk::Box))

, tag_frame_rgba_(new Gdk::RGBA)

, tag_frame_label_fontdescription_(new Pango::FontDescription)

{

  // 
  box() . pack_start(*tag_frame_, Gtk::PACK_EXPAND_WIDGET);



  // 
  box() . set_margin_left(5);

  // 
  box() . set_margin_right(5);

  // 
  box() . set_margin_top(5);

  // 
  box() . set_margin_bottom(5);



  //
  box() . set_homogeneous(false);



  // 
  tag_frame_ -> set_label_widget(*tag_frame_label_);

  // 
  tag_frame_label_ -> set_text(" Selected : ");

  // 
  tag_frame_ 
    -> override_background_color(*tag_frame_rgba_, Gtk::STATE_FLAG_NORMAL);

  // 
  tag_frame_label_fontdescription_ -> set_weight(Pango::Weight::WEIGHT_BOLD);

  // 
  tag_frame_label_ -> override_font(*tag_frame_label_fontdescription_);



  // 
  tag_frame_ -> add(*tag_labels_box_);

  //
  tag_labels_box_ -> set_homogeneous(false);

  // 
  tag_labels_box_ -> set_center_widget(*tag_label_center_);

  // 
  tag_labels_box_ -> pack_start(*tag_label_start_left_, true, true, 0);

  // 
  tag_labels_box_ -> pack_start(*tag_label_start_right_, true, true, 0);

  // 
  tag_labels_box_ -> pack_start(*tag_label_middle_left_, true, true, 0);

  // 
  tag_labels_box_ -> pack_start(*tag_label_middle_right_, true, true, 0);

  // 
  tag_labels_box_ -> pack_start(*tag_label_end_left_, true, true, 0);

  // 
  tag_labels_box_ -> pack_start(*tag_label_end_right_, true, true, 0);



  // 
  tag_frame_ -> set_tooltip_text("Displays various metadata and " \
                                 "technical details about the active track.");

  // 
  tag_frame_label_ -> set_tooltip_text
    ("Displays whether the active track is the selected " \
     "track, the playing track, or both.");



  // 
  tag_labels_box_ -> set_margin_top(10);



  // 
  tag_label_center_ -> set_text("No track is selected!\n"),



  // 
  tag_label_center_ -> set_ellipsize(Pango::ELLIPSIZE_END);

  // 
  tag_label_end_right_ -> set_ellipsize(Pango::ELLIPSIZE_END);

  // 
  tag_label_middle_right_ -> set_ellipsize(Pango::ELLIPSIZE_END);

  // 
  tag_label_start_right_ -> set_ellipsize(Pango::ELLIPSIZE_END);



  // 
  tag_label_end_left_ -> set_xalign(1);

  // 
  tag_label_end_right_ -> set_xalign(0);

  // 
  tag_label_middle_left_ -> set_xalign(1);

  // 
  tag_label_middle_right_ -> set_xalign(0);

  // 
  tag_label_start_left_ -> set_xalign(1);

  // 
  tag_label_start_right_ -> set_xalign(0);



  // 
  tag_label_end_left_ -> set_justify(Gtk::JUSTIFY_RIGHT);

  // 
  tag_label_middle_left_ -> set_justify(Gtk::JUSTIFY_RIGHT);

  // 
  tag_label_start_left_ -> set_justify(Gtk::JUSTIFY_RIGHT);



  // 
  tag_label_start_left_ -> set_margin_left(10);

  // 
  tag_label_start_right_ -> set_margin_right(20);

  // 
  tag_label_middle_right_ -> set_margin_right(20);

  // 
  tag_label_end_right_ -> set_margin_right(10);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Tagview::~Tagview()
{

  // 
  delete tag_frame_rgba_;

  // 
  delete tag_frame_label_fontdescription_;

  // 
  delete tag_label_center_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void Tagview::Update_Tags(const char *new_frame_label_name, Track& new_track)
{

  // 
  Glib::ustring* artists = new_track . artists_string();

  // 
  Glib::ustring* genres = new_track . genres_string();

  // 
  int date = new_track . date();

  // 
  string date_str = "-";

  //
  int track_number = new_track . track_number();

  // 
  string track_number_str = "-";

  //
  int track_total = new_track . track_total();

  // 
  string track_total_str = "-";

  //
  int disc_number = new_track . disc_number();

  // 
  string disc_number_str = "-";

  //
  int disc_total = new_track . disc_total();

  // 
  string disc_total_str = "-";



  // 
  if((*artists) == "")
  {

    // 
    *artists = "-";

  }

  // 
  if((*genres) == "")
  {

    // 
    *genres = "-";

  }

  // 
  if(date > 0)
  {

    // 
    date_str = to_string(date);

  }

  // 
  if(track_number > 0)
  {

    // 
    track_number_str = to_string(track_number);

  }

  // 
  if(track_total > 0)
  {

    // 
    track_total_str = to_string(track_total);

  } 

  // 
  if(disc_number > 0)
  {

    // 
    disc_number_str = to_string(disc_number);

  }

  // 
  if(disc_total > 0)
  {

    // 
    disc_total_str = to_string(disc_total);

  } 



  // 
  for(list<Tagview*>::iterator tagviews_it = gui() . tagviews() . begin();
      tagviews_it != gui().tagviews().end();
      tagviews_it++)
  { 

    // 
    (*tagviews_it) -> tag_frame_label()
      . set_label(" " + string(new_frame_label_name) + " : ");



    // 
    if(new_track . Is_Empty())
    {

      //
      (*tagviews_it) -> tag_labels_box()
        . set_center_widget(*tag_label_center_);



      //
      (*tagviews_it) -> tag_label_start_left() . set_label("");

      //
      (*tagviews_it) -> tag_label_start_right() . set_label("");



      //
      (*tagviews_it) -> tag_label_middle_left() . set_label("");

      //
      (*tagviews_it) -> tag_label_middle_right() . set_label("");



      //
      (*tagviews_it) -> tag_label_end_left() . set_label("");

      //
      (*tagviews_it) -> tag_label_end_right() . set_label("");

    }

    // 
    else
    {

      //
      (*tagviews_it) -> tag_labels_box() . unset_center_widget();



      // 
      (*tagviews_it) -> tag_label_start_left() . set_markup
        ("<u>Track Number</u> :   \n" \
         "<u>Track Total</u> :   \n" \
         "<u>Disc Number</u> :   \n" \
         "<u>Disc Total</u> :   \n");

      // 
      (*tagviews_it) -> tag_label_start_right() . set_markup
        (track_number_str + "\n" +
         track_total_str + "\n" +
         disc_number_str + "\n" +
         disc_total_str + "\n"); 



      // 
      (*tagviews_it) -> tag_label_middle_left() . set_markup
        ("<u>Title</u> :   \n" \
         "<u>Artist(s)</u> :   \n" \
         "<u>Album</u> :   \n" \
         "<u>Genre(s)</u> :   \n");

      // 
      (*tagviews_it) -> tag_label_middle_right() . set_markup
        (new_track . title() + "\n" +
         *artists + "\n" +
         new_track . album() + "\n" +
         *genres + "\n"); 



      // 
      (*tagviews_it) -> tag_label_end_left() . set_markup
        ("<u>Length</u> :   \n" \
         "<u>Codec</u> :   \n" \
         "<u>Bitrate</u> :   \n" \
         "<u>Date</u> :   \n");

      // 
      (*tagviews_it) -> tag_label_end_right() . set_markup
        (new_track . length() + "\n" +
         new_track . codec() + "\n" +
         to_string(new_track . bit_rate()) + "\n" +
         date_str + "\n"); 

    }

  }



  // 
  delete artists;

  // 
  delete genres;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::Box& Tagview::box()
{

  return *box_;

}

Gtk::Label& Tagview::tag_frame_label()
{

  return *tag_frame_label_;

}

Gtk::Label& Tagview::tag_label_center()
{

  return *tag_label_center_;

}

Gtk::Label& Tagview::tag_label_end_left()
{ 

  return *tag_label_end_left_;

}

Gtk::Label& Tagview::tag_label_end_right()
{ 

  return *tag_label_end_right_;

} 

Gtk::Label& Tagview::tag_label_middle_left()
{

  return *tag_label_middle_left_;

}

Gtk::Label& Tagview::tag_label_middle_right()
{

  return *tag_label_middle_right_;

} 

Gtk::Label& Tagview::tag_label_start_left()
{  

  return *tag_label_start_left_;

}

Gtk::Label& Tagview::tag_label_start_right()
{  

  return *tag_label_start_right_;

}

Gtk::Box& Tagview::tag_labels_box()
{

  return *tag_labels_box_;

}
