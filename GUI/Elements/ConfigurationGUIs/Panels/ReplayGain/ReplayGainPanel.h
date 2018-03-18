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

#ifndef REPLAY_GAIN_PANEL_H
#define REPLAY_GAIN_PANEL_H





//                   //
//                   //
// Inherited Headers //////////////////////////////////////////////////////////
//                   //
//                   //

#include "../../Panel.h"





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

  class CheckButton;

  class Frame;

  class RadioButton;

  class Switch;

}

class SpinButtonScale;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class ReplayGainPanel : public Panel
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    ReplayGainPanel(Base& base_ref, ConfigurationGUI& new_config_gui,
                    Gtk::TreeRow* parent_row_ptr);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~ReplayGainPanel();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    virtual void Apply_Saved_Values() final override;

    void On_Enable_Switch_Changed_Signal();

    void On_Fallback_Gain_SBS_Value_Changed_Signal();

    void On_Gain_Type_Radiobutton_Toggled_Signal();

    void On_Limiter_Checkbutton_Toggled_Signal();

    void On_Pre_Amp_SBS_Value_Changed_Signal();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::RadioButton& album_gain_radiobutton();

    Gtk::Switch& enable_switch();

    SpinButtonScale& fallback_gain_sbs();

    Gtk::CheckButton& limiter_checkbutton();

    SpinButtonScale& pre_amp_sbs();

    Gtk::RadioButton& track_gain_radiobutton();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  private:

    // 
    Gtk::Box* rg_options_box_;





  //               //
  // Enable Switch ////////////////////////////////////////////////////////////
  //               //

  private:

    // 
    Gtk::Switch* enable_switch_;

    // 
    Gtk::Box* enable_switch_box_;




 
  //               //
  // Fallback Gain ////////////////////////////////////////////////////////////
  //               //

  private:

    // 
    SpinButtonScale* fallback_gain_sbs_;

    //
    Gtk::Frame* fallback_gain_sbs_frame_;





  //           //
  // Gain Type ////////////////////////////////////////////////////////////////
  //           //

  private:

    // 
    Gtk::RadioButton* album_gain_radiobutton_;

    // 
    Gtk::Box* gain_type_box_;

    // 
    Gtk::RadioButton* track_gain_radiobutton_;





  //         //
  // Limiter //////////////////////////////////////////////////////////////////
  //         //

  private:

    // 
    Gtk::Box* limiter_box_;

    // 
    Gtk::CheckButton* limiter_checkbutton_;





  //         //
  // Pre-Amp //////////////////////////////////////////////////////////////////
  //         //

  private:

    // 
    SpinButtonScale* pre_amp_sbs_;

    //
    Gtk::Frame* pre_amp_sbs_frame_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
