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





#ifndef SCROBBLING_PANEL_H
#define SCROBBLING_PANEL_H





#include "../../Panel.h"



class ConfigurationGUI;
class SpinButtonScale;



namespace Gtk
{

  class Frame;
  class Box;
  class Label;
  class Entry;
  class Switch;
  class Button;
  class Spinner;

}





class ScrobblingPanel : public Panel
{

  public: 

    ScrobblingPanel(Base& base_ref, ConfigurationGUI& new_config_gui);

    virtual ~ScrobblingPanel();

    virtual void Apply_Saved_Values() final override;

  protected:

    void Scrobble_Percent_SpinButtonScale_Signal_Changed();

    bool Lastfm_Username_Entry_Signal_Focus_In_Event(GdkEventFocus* event);
    
    bool Lastfm_Username_Entry_Signal_Focus_Out_Event(GdkEventFocus* event);

    void Lastfm_Username_Entry_Signal_Changed();
 
    bool Lastfm_Password_Entry_Signal_Focus_In_Event(GdkEventFocus* event);

    bool Lastfm_Password_Entry_Signal_Focus_Out_Event(GdkEventFocus* event);

    void Lastfm_Password_Entry_Signal_Changed();

    void Lastfm_Switch_Signal_Changed();

    void Lastfm_Login_Button_Signal_Clicked();

  public:
  
    SpinButtonScale &scrobble_percent_spinbuttonscale()
    { 

      return *scrobble_percent_spinbuttonscale_; 

    }
 
    Gtk::Entry &lastfm_username_entry();

    Gtk::Entry &lastfm_password_entry();

    Gtk::Switch &lastfm_switch();

    Gtk::Button &lastfm_login_button();

    Gtk::Spinner &lastfm_spinner()
    { 

      return *lastfm_spinner_; 

    }

  private:

    Gtk::Frame *lastfm_frame_;

    Gtk::Box *lastfm_box_,
             *lastfm_sub_box_;

    Gtk::Label *scrobble_percent_label_;

    SpinButtonScale *scrobble_percent_spinbuttonscale_;

    Gtk::Entry *lastfm_username_entry_;
    
    Gtk::Entry *lastfm_password_entry_;

    Gtk::Switch *lastfm_switch_;

    Gtk::Button *lastfm_login_button_;

    Gtk::Spinner *lastfm_spinner_;

};





#endif
