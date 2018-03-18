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

#ifndef OUTPUT_PANEL_H
#define OUTPUT_PANEL_H





//                   //
//                   //
// Inherited Headers //////////////////////////////////////////////////////////
//                   //
//                   //

#include "../../Panel.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/refptr.h>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class ConfigurationGUI;

namespace Gtk
{

  class Box;

  class ComboBoxText;

  class Frame;

  class Label;

}

class SpinButtonScale;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class OutputPanel : public Panel
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    OutputPanel(Base& new_main, ConfigurationGUI& new_config_gui);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~OutputPanel();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    virtual void Apply_Saved_Values() final override;

  protected:
    
    void On_Signal_Changed_Output_Sink_Combobox();

    void On_Signal_Changed_Buffer_Time_SpinButtonScale();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::ComboBoxText& output_sink_combobox()
    {

      return *output_sink_combobox_;

    }

    SpinButtonScale& buffer_time_spinbuttonscale()
    {

      return *buffer_time_spinbuttonscale_;

    }

    Gtk::Label& buffer_time_spinbuttonscale_label()
    {

      return *buffer_time_spinbuttonscale_label_;

     }





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //              //
  // Queue Buffer /////////////////////////////////////////////////////////////
  //              //

  private:

    Gtk::Box* buffer_box_;

    Gtk::Frame* buffer_frame_;

    SpinButtonScale* buffer_time_spinbuttonscale_;

    Gtk::Label* buffer_time_spinbuttonscale_label_;

    Gtk::Box* buffer_time_spinbuttonscale_label_box_;

    Gtk::Box* buffer_time_spinbuttonscale_label_center_box_;





  //      //
  // Sink /////////////////////////////////////////////////////////////////////
  //      //

  private:

    Gtk::Box* output_sink_box_;

    Gtk::ComboBoxText* output_sink_combobox_;

    Gtk::Box* output_sink_combobox_box_;

    Gtk::Label* output_sink_combobox_name_label_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
