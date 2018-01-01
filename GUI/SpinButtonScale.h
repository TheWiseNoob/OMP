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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef SPINBUTTONSCALE_H
#define SPINBUTTONSCALE_H





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

//                              //
//                              //
// Class Forward Declarations /////////////////////////////////////////////////
//                              //
//                              //

class Audiophile;

class Configuration;

namespace Gtk
{

  class Box;

  class Scale;

  class SpinButton;

}





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class SpinButtonScale
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    SpinButtonScale(double begin, double end, int accuracy);





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //           //
  // Callbacks ////////////////////////////////////////////////////////////////
  //           //

  public:

    void On_Signal_Value_Changed_SBS_Scale();

    void On_Signal_Value_Changed_SBS_SpinButton();





  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  public:

    double get_value();

    void set_value(double new_value);

    void set_range(double begin, double end);

    void Flip();

    void Set_Sensitive(bool new_value);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    //Getters
    Gtk::Box& box()
    {

      return *box_;

    }

    Gtk::Box& sbs_spinbutton_box()
    { 

      return *sbs_spinbutton_box_;

    }

    Gtk::Box& sbs_scale_box()
    { 

      return *sbs_scale_box_;

    }

    Gtk::SpinButton& sbs_spinbutton()
    { 

      return *sbs_spinbutton_; 

    }

    Gtk::Scale& sbs_scale()                                       
    { 

      return *sbs_scale_; 

    }





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //
   
  private:

    Gtk::Box* box_;

    Gtk::SpinButton* sbs_spinbutton_;
    Gtk::Scale* sbs_scale_;

    Gtk::Box* sbs_scale_box_;
    Gtk::Box* sbs_spinbutton_box_;

    bool disable_update_;

    bool scale_first_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
