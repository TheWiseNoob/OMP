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

#include "Seekbar.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Configuration/Configuration.h"

#include "../Playback/Playback.h"

#include "../Metadata/TimeConversion.h"

#include "../Metadata/Track.h"

#include "Elements/Playlists/Playlists.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/main.h>

#include <gtkmm/entry.h>

#include <gtkmm/scale.h>

#include <gtkmm/treerowreference.h>

#include <iostream>

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

Seekbar::Seekbar(Base& base, int begin, int end)

// Inherited Class

: Parts(base)



// General

, seekbar_movement_(true)

, disable_update_(false)

{

  debug("Beginning of Seekbar constructor.");



  // 
  entry_ = Gtk::manage(new Gtk::Entry);

  // 
  scale_ = Gtk::manage(new Gtk::Scale);



  // 
  scale_ -> set_range(begin, end); 



  // 
  pack_end(entry_box_, Gtk::PACK_SHRINK);

  // 
  pack_start(scale_box_, Gtk::PACK_EXPAND_WIDGET);

 

  // 
  scale_box_ . set_orientation(Gtk::ORIENTATION_VERTICAL);

  // 
  scale_box_ . pack_start(*scale_, Gtk::PACK_EXPAND_WIDGET);

  // 
  entry_box_ . set_orientation(Gtk::ORIENTATION_VERTICAL);

  // 
  entry_box_ . set_center_widget(*entry_);



  // 
  scale_ -> set_draw_value(false);                                         

  // 
  scale_ -> set_digits(0);    

  // 
  scale_ -> set_value(begin); 



  // 
  entry_ -> set_margin_right(2);  

  // 
  entry_ -> set_margin_left(4);

  // 
  entry_ -> set_margin_top(2);                                    

  // 
  entry_ -> set_margin_bottom(2);                                    

  // 
  entry_ -> set_sensitive(false);




  // 
  entry_ -> set_placeholder_text("00:00.00 / 00:00.00");

  // 
  entry_ -> set_alignment(0.5);



  // 
  scale_ -> set_tooltip_text("Drag to seek the currently active track.");

  //
  entry_ -> set_tooltip_text
    ("Position in Playing Track / Time Left in Playing Track");



  // 
  scale_ -> signal_value_changed()
    . connect(sigc::mem_fun(*this, &Seekbar::On_Signal_Value_Changed_Scale));

  // 
  entry_ -> signal_changed()
    . connect(sigc::mem_fun(*this, &Seekbar::On_Signal_Value_Changed_Entry));            
 
  // 
  scale() . signal_button_press_event()
    . connect(sigc::mem_fun(*this, &Seekbar::Stop_Seekbar_Movement), false);

  // 
  scale() . signal_button_release_event()
    . connect(sigc::mem_fun(*this, &Seekbar::Start_Seekbar_Movement), false);



  // 
  show_all_children();



  // 
  sigc::slot<bool> seekbar_slot 
    = sigc::bind(sigc::mem_fun(*this, &Seekbar::Automation), 0);

  // This is where we connect the slot to the Glib::signal_timeout()
  sigc::connection seekbar_connection 
    = Glib::signal_timeout() . connect(seekbar_slot, 20, Glib::PRIORITY_HIGH);

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

bool Seekbar::Automation(int timeout_number)
{

  // 
  if((playback() . Stopped()))
  {

    // 
    scale() . set_value(0);

    // 
    entry() . set_text("00:00:00 / 00:00.00");



    // 
    scale() . set_sensitive(false);

    // 
    entry() . set_sensitive(false);



    // 
    scale() . set_range(0,10);

  } 

  // Checks if seekbar_HScale is sensitive and sets it if so.
  else if(playlists() . changing_track())
  {

    debug("Changing tracks in Seekbar.");



    // 
    scale() . set_value(0);

    // 
    entry() . set_text("00:00:00 / 00:00.00");



    // 
    scale() . set_sensitive(false);

    // 
    entry() . set_sensitive(false);



    // 
    scale() . set_range(0,10);

  } 

  // 
  else if(seekbar_movement_)
  {

    // 
    scale().set_sensitive(true);

    // 
    entry().set_sensitive(true);



    // 
    long long position_temp = (playback() . Position());

    // 
    long long duration_temp = (playback() . Duration());



    // 
    auto playing_track_copy = playback().playing_track();



    // 
    if(duration_temp <= 0LL)
    {

      // 
      scale() . set_range(0, 100); 

    }

    // 
    else if(playing_track_copy . Pregap())
    {

      // 
      long long track_start_ll 
        = playing_track_copy.pregap_start() - playing_track_copy.start();




      // 
      scale() . set_range(double(track_start_ll / 10000000.000000),
                          double((duration_temp) / 10000000.000000));

    }

    // 
    else
    {

      scale() . set_range(0, double((duration_temp) / 10000000.0000000));

    }



    // 
    scale() . set_value((position_temp) / 10000000);

   }



  // 
  return true;

}

void Seekbar::On_Signal_Value_Changed_Entry() 
{                                                                               
                                                            
  if(disable_update_)                                                            
  {                                                                             
                                                                                
    return;                                                                     
                                                                                
  }                                                                             

 /*
                                                                                
  disable_update = true;                                                        

  int value = entry_ -> get_value();
                                                                                
  scale_ -> set_value(value);                                              
                                                                                
  cout << "\n\nentry_.value: " << entry_ -> get_value()
       << "\n" << "Scale value: " << scale_ -> get_value() << "\n\n";               
                                                                                
  disable_update = false;  
*/

}    

void Seekbar::On_Signal_Value_Changed_Scale()
{

  // 
  if(disable_update_)
  {

    // 
    return;

  }

  // 
  disable_update_ = true;



  // 
  long long pos_ll = (10000000 * (scale_ -> get_value()));

  //
  long long time_left_ll = (playback() . Duration()) - pos_ll;



  // 
  std::string* pos_str 
    = time_converter() . Nanoseconds_To_Time_Format(pos_ll);

  // 
  std::string* time_left_str 
    = time_converter() . Nanoseconds_To_Time_Format(time_left_ll);



  //   
  string final_time_str = (*pos_str) + " / " + (*time_left_str);


  // 
  entry_ -> set_text(final_time_str);



  // 
  delete pos_str;

  // 
  delete time_left_str;



  // 
  disable_update_ = false;

}

void Seekbar::Set_Range(double begin, double end)
{

  //  
  scale_ -> set_range(begin, end); 

}

bool Seekbar::Start_Seekbar_Movement(GdkEventButton* event)
{ 

  // 
  if(disable_update_)
  {

    // 
    return false;

   }

  // 
  disable_update_ = true;



  // 
  long long new_value = (10000000 * (scale_ -> get_value()));

  // 
  std::string *temp_time 
    = time_converter().Nanoseconds_To_Time_Format(new_value);

  // 
  entry_ -> set_text(*temp_time);

  // 
  delete temp_time;



  // 
  long long new_position = 0LL;

  // 
  auto playing_track_copy = playback().playing_track();



  // 
  if(playing_track_copy.Normal())
  { 

    // 
    new_position = ((scale().get_value()) * (10000000));

  }

  // 
  else if(playing_track_copy.type() == TrackType::SINGLE_FILE)
  { 

    // 
    new_position 
      = ((scale().get_value()) * (10000000)) + (playing_track_copy.start());

  }



  // 
  playback() . Update_Track_Position(new_position);



  // 
  disable_update_ = false;

  // 
  seekbar_movement_ = true;



  // 
  return false;

}

bool Seekbar::Stop_Seekbar_Movement(GdkEventButton* event)
{

  // 
  seekbar_movement_ = false;



  // 
  return false;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::Entry& Seekbar::entry()
{

  return *entry_;

}

Gtk::Scale& Seekbar::scale()
{

  return *scale_;

}
