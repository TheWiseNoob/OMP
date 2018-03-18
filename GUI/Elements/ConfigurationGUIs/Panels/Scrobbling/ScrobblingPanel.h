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

#ifndef SCROBBLING_PANEL_H
#define SCROBBLING_PANEL_H





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

  class Box;

  class Button;

  class Entry;

  class Frame;

  class Label;

  class Spinner;

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

class ScrobblingPanel : public Panel
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public: 

    ScrobblingPanel(Base& base_ref, ConfigurationGUI& new_config_gui);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    virtual ~ScrobblingPanel();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  public:

    virtual void Apply_Saved_Values() final override;





  //                     //
  // Account Information //////////////////////////////////////////////////////
  //                     //

  protected:

    void Lastfm_Login_Button_Signal_Clicked();

    void Lastfm_Password_Entry_Signal_Changed();

    bool Lastfm_Password_Entry_Signal_Focus_In_Event(GdkEventFocus* event);

    bool Lastfm_Password_Entry_Signal_Focus_Out_Event(GdkEventFocus* event);

    void Lastfm_Username_Entry_Signal_Changed();
 
    bool Lastfm_Username_Entry_Signal_Focus_In_Event(GdkEventFocus* event);
    
    bool Lastfm_Username_Entry_Signal_Focus_Out_Event(GdkEventFocus* event);

    void Lastfm_Switch_Signal_Changed();





  //                  //
  // Failed Scrobbles /////////////////////////////////////////////////////////
  //                  //

  protected:

    void Clear_Scrobbles_Button_Signal_Clicked();

    void Reattempt_Scrobbles_Button_Signal_Clicked();





  //                  //
  // Scrobble Percent /////////////////////////////////////////////////////////
  //                  //

  protected:

    void Scrobble_Percent_SpinButtonScale_Signal_Changed();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //                     //
  // Account Information //////////////////////////////////////////////////////
  //                     //

  public:

    // 
    Gtk::Button& lastfm_login_button();

    // 
    Gtk::Entry& lastfm_password_entry();

    // 
    Gtk::Spinner& lastfm_spinner();

    // 
    Gtk::Switch& lastfm_switch();

    // 
    Gtk::Entry& lastfm_username_entry();





  //                  //
  // Failed Scrobbles /////////////////////////////////////////////////////////
  //                  //

  public:

    // 
    Gtk::Button& clear_failed_scrobbles_button();

    // 
    Gtk::Label& failed_scrobbles_count_label();

    // 
    Gtk::Button& reattempt_failed_scrobbles_button();

    // 
    Gtk::Spinner& reattempt_scrobbles_spinner();





  //                  //
  // Scrobble Percent /////////////////////////////////////////////////////////
  //                  //

  public:

    // 
    SpinButtonScale& scrobble_percent_spinbuttonscale();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //                     //
  // Account Information //////////////////////////////////////////////////////
  //                     //

  private:

    // 
    Gtk::Box* lastfm_box_;

    // 
    Gtk::Frame* lastfm_frame_;

    // 
    Gtk::Button* lastfm_login_button_;

    // 
    Gtk::Entry* lastfm_password_entry_;

    // 
    Gtk::Spinner* lastfm_spinner_;

    // 
    Gtk::Box* lastfm_sub_box_;

    // 
    Gtk::Switch* lastfm_switch_;

    // 
    Gtk::Entry* lastfm_username_entry_;





  //                  //
  // Failed Scrobbles /////////////////////////////////////////////////////////
  //                  //

  private:

    // 
    Gtk::Button* clear_failed_scrobbles_button_;

    // 
    Gtk::Box* failed_scrobbles_box_;

    // 
    Gtk::Label* failed_scrobbles_count_label_;

    // 
    Gtk::Label* failed_scrobbles_label_;

    // 
    Gtk::Button* reattempt_failed_scrobbles_button_;

    // 
    Gtk::Spinner* reattempt_scrobbles_spinner_;





  //                  //
  // Scrobble Percent /////////////////////////////////////////////////////////
  //                  //

  private:

    // 
    Gtk::Label* scrobble_percent_label_;

    // 
    SpinButtonScale* scrobble_percent_spinbuttonscale_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
