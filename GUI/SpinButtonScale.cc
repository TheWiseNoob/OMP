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

#include "SpinButtonScale.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Configuration/Configuration.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/box.h>

#include <gtkmm/scale.h>

#include <gtkmm/spinbutton.h>

#include <iostream>

#include <string>





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

SpinButtonScale::SpinButtonScale(double begin, double end, int accuracy)

: box_(Gtk::manage(new Gtk::Box))

, sbs_spinbutton_(Gtk::manage(new Gtk::SpinButton))

, sbs_scale_(Gtk::manage(new Gtk::Scale))

, sbs_scale_box_(Gtk::manage(new Gtk::Box))

, sbs_spinbutton_box_(Gtk::manage(new Gtk::Box))

, scale_first_(true)

{

  // 
  sbs_scale_ -> set_range(begin, end);

  // 
  sbs_spinbutton_ -> set_range(begin, end);



  // 
  sbs_spinbutton_ -> set_vexpand(false);



  // 
  disable_update_ = false;



  // 
  box_ -> pack_start(*sbs_scale_box_, Gtk::PACK_EXPAND_WIDGET);

  // 
  box_ -> pack_end(*sbs_spinbutton_box_, Gtk::PACK_SHRINK);



  // 
  sbs_scale_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // 
  sbs_scale_box_ -> pack_start(*sbs_scale_, Gtk::PACK_EXPAND_WIDGET);

  // 
  sbs_spinbutton_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // 
  sbs_spinbutton_box_
    -> pack_start(*sbs_spinbutton_, Gtk::PACK_EXPAND_PADDING);



  // 
  sbs_scale_ -> set_draw_value(false);

  // 
  sbs_scale_ -> set_digits(3);

  // 
  sbs_scale_ -> set_value(begin);



  // Sets the increments of the SpinButtonScale for buffer time.
  sbs_spinbutton_ -> set_increments(1, 0.1);



  // 
  sbs_spinbutton_ -> set_margin_right(2);

  // 
  sbs_spinbutton_ -> set_margin_left(4);

  // 
  sbs_spinbutton_ -> set_margin_top(2);

  // 
  sbs_spinbutton_ -> set_margin_bottom(2);

  // 
  sbs_spinbutton_ -> set_update_policy(Gtk::UPDATE_IF_VALID);



  // 
  sbs_spinbutton_ -> set_digits(accuracy);

  // 
  sbs_spinbutton_ -> set_value(begin);



  // 
  sbs_scale_ -> signal_value_changed().connect(sigc::mem_fun(*this,
      &SpinButtonScale::On_Signal_Value_Changed_SBS_Scale));

  // 
  sbs_spinbutton_ -> signal_value_changed().connect(sigc::mem_fun(*this,
      &SpinButtonScale::On_Signal_Value_Changed_SBS_SpinButton));



  // 
  box_ -> show_all_children();

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//           //
// Callbacks //////////////////////////////////////////////////////////////////
//           //

void SpinButtonScale::On_Signal_Value_Changed_SBS_Scale()
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
  double value = sbs_scale_ -> get_value();

  // 
  sbs_spinbutton_ -> set_value(value);



  // 
  disable_update_ = false;

} 

void SpinButtonScale::On_Signal_Value_Changed_SBS_SpinButton()
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
  double value = sbs_spinbutton_ -> get_value();

  // 
  sbs_scale_ -> set_value(value);



  // 
  disable_update_ = false;

}





//         //
// General ////////////////////////////////////////////////////////////////////
//         //

double SpinButtonScale::get_value()
{

  return sbs_spinbutton_ -> get_value();

}

void SpinButtonScale::set_value(double new_value)
{

  sbs_spinbutton_ -> set_value(new_value);

}

void SpinButtonScale::set_range(double begin, double end)
{

  sbs_scale_ -> set_range(begin * 1000.000, end * 1000.000);
  sbs_spinbutton_ -> set_range(begin, end);

}

void SpinButtonScale::Flip()
{

  box_ -> remove(*sbs_spinbutton_box_);
  box_ -> remove(*sbs_scale_box_);

  if(scale_first_)
  {

    box_ -> pack_end(*sbs_scale_box_, Gtk::PACK_EXPAND_WIDGET);
    box_ -> pack_start(*sbs_spinbutton_box_, Gtk::PACK_SHRINK);

    scale_first_ = false;

  }
  else
  {

    box_ -> pack_start(*sbs_scale_box_, Gtk::PACK_EXPAND_WIDGET);
    box_ -> pack_end(*sbs_spinbutton_box_, Gtk::PACK_SHRINK);

    scale_first_ = true;

  }

}

void SpinButtonScale::Set_Sensitive(bool new_value)
{

  sbs_scale_ -> set_sensitive(new_value);

  sbs_spinbutton_ -> set_sensitive(new_value);

}
