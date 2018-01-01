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

#ifndef PLAYBACK_PANEL_H
#define PLAYBACK_PANEL_H





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

  class CheckButton;

  class Frame;

  class Label;

  class RadioButton;

}





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class PlaybackPanel : public Panel
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    PlaybackPanel(Base& base_ref, ConfigurationGUI& new_config_gui);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~PlaybackPanel();





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




  //         //
  // Looping //////////////////////////////////////////////////////////////////
  //         //

  public:

    void On_None_Looping_Radio_Button_Toggled_Signal();

    void On_Playlist_Looping_Radio_Button_Toggled_Signal();





  //         //
  // Options //////////////////////////////////////////////////////////////////
  //         //

  public:

    void On_Playback_Follows_Cursor_Check_Button_Toggled_Signal();

    void On_Cursor_Follows_Playback_Check_Button_Toggled_Signal();

    void On_Queue_Check_Button_Toggled_Signal();

    void On_Queue_Saved_Check_Button_Toggled_Signal();

    void On_Start_At_Pregap_Check_Button_Toggled_Signal();

    void On_Stop_After_Current_Track_Check_Button_Toggled_Signal();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //         //
  // Looping //////////////////////////////////////////////////////////////////
  //         //

  public:

    // 
    Gtk::RadioButton& none_looping_radio_button();

    // 
    Gtk::RadioButton& playlist_looping_radio_button();





  //         //
  // Options //////////////////////////////////////////////////////////////////
  //         //

  public:

    // 
    Gtk::CheckButton& cursor_follows_playback_check_button();

    // 
    Gtk::CheckButton& playback_follows_cursor_check_button();

    // 
    Gtk::CheckButton& queue_check_button();

    // 
    Gtk::CheckButton& queue_saved_check_button();

    // 
    Gtk::CheckButton& start_at_pregap_check_button();

    // 
    Gtk::CheckButton& stop_after_current_track_check_button();





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
    Gtk::Box* looping_and_order_box_;





  //         //
  // Looping //////////////////////////////////////////////////////////////////
  //         //

  private:

    // 
    Gtk::Box* looping_box_;

    // 
    Gtk::Frame* looping_frame_;

    // 
    Gtk::RadioButton* none_looping_radio_button_;

    // 
    Gtk::RadioButton* track_looping_radio_button_;

    // 
    Gtk::RadioButton* playlist_looping_radio_button_;





  //         //
  // Options //////////////////////////////////////////////////////////////////
  //         //

  private:

    // 
    Gtk::Box* options_box_;

    //
    Gtk::Box* left_options_box_;

    //
    Gtk::Box* middle_options_box_;

    //
    Gtk::Box* right_options_box_;

    //
    Gtk::CheckButton* cursor_follows_playback_check_button_;

    //
    Gtk::CheckButton* playback_follows_cursor_check_button_;

    //
    Gtk::CheckButton* queue_check_button_;

    //
    Gtk::CheckButton* queue_saved_check_button_;

    //
    Gtk::CheckButton* stop_after_current_track_check_button_;

    //
    Gtk::CheckButton* start_at_pregap_check_button_;





  //       //
  // Order ////////////////////////////////////////////////////////////////////
  //       //

  private:

    // 
    Gtk::Box* order_box_;

    // 
    Gtk::Frame* order_frame_;

    // 
    Gtk::RadioButton* normal_order_radio_button_;

    // 
    Gtk::RadioButton* shuffle_tracks_order_radio_button_;

    // 
    Gtk::RadioButton* random_tracks_order_radio_button_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif

