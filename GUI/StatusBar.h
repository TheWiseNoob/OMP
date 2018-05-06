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

#ifndef STATUS_BAR_H
#define STATUS_BAR_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                 //
//                 //
// Inherited Class ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Parts.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <functional>

#include <list>

#include <vector>

#include <memory>

#include <mutex>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

//         //
//         //
// Classes ////////////////////////////////////////////////////////////////////
//         //
//         //

class Base;

namespace Gtk
{

  class Box;

  class Button;

  class EventBox;

  class Frame;

  class Image;

  class Label;

  class VolumeButton;

}





//         //
//         //
// Structs ////////////////////////////////////////////////////////////////////
//         //
//         //

//     //
// Gdk ////////////////////////////////////////////////////////////////////////
//     //

struct _GdkEventKey;
typedef struct _GdkEventKey GdkEventKey;

struct _GdkEventCrossing;
typedef struct _GdkEventCrossing GdkEventCrossing;

struct _GdkEventButton;
typedef struct _GdkEventButton GdkEventButton;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class StatusBar : public Parts
{  

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:  
    //
    //   Creates the class.
    //
    //
    //
    // Arguments: 
    //
    //   base: passes a reference to the Base of the program.
    //
    ///////// /////////////////////////////////////////////////////////////// */
    StatusBar(Base& base_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Cleans up anything that needs it when the GUI is being destroyed.
    //
    //
    //
    // Arguments: 
    //
    //   None
    //
    //////////////////////////////////////////////////////////////////////// */
    virtual ~StatusBar();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Formats the position and time and sets it as the value of time_label_.
    //
    // 
    //
    // Arguments: 
    //
    //   timeout_number: Time in milliseconds to recall the function after
    //   finishing.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Display_Time(int timeout_number);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Function run when the value of the volume spin button is changed.
    //
    // 
    //
    // Arguments: 
    //
    //   new_volume: New value of the volume changer.
    //
    //////////////////////////////////////////////////////////////////////// */
    void On_Signal_Value_Changed_Main_Volume_Button(double new_volume);

    void Playback_Status_Paused();

    void Playback_Status_Playing();

    void Playback_Status_Stopped();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Changes the color of the StatusBar and selects the current playing
    //   track if double-clicked.
    //
    // 
    //
    // Arguments: 
    //
    //   event: Event data structure.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Status_Bar_Event_Box_Button_Press(GdkEventButton* event);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Changes the color of the StatusBar and selects upon mouse button
    //   release.
    //
    // 
    //
    // Arguments: 
    //
    //   event: Event data structure.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Status_Bar_Event_Box_Button_Release(GdkEventButton* event);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Changes the color of the StatusBar and selects upon mouse button
    //   release.
    //
    // 
    //
    // Arguments: 
    //
    //   event: Event data structure.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Status_Bar_Event_Box_Enter(GdkEventCrossing* event);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Changes the color of the StatusBar and selects upon mouse button
    //   release.
    //   
    // 
    //
    // Arguments: 
    //
    //   event: Event data structure.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool Status_Bar_Event_Box_Leave(GdkEventCrossing* event);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::Frame& status_bar_frame();





  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Used to set the label of the selected rows count.
    //
    //
    //
    // Arguments: 
    //
    //   new_rows_count: Amount of rows selected.
    //
    //////////////////////////////////////////////////////////////////////// */
    void set_selected_rows_count_label(int new_rows_count);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Used to set the label of the selected tracks time sum.
    //
    // 
    //
    // Arguments: 
    //
    //   new_time: String that is the time sum of the selected tracks.
    //
    //////////////////////////////////////////////////////////////////////// */
    void set_selected_time_label(const char* new_time);





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //         //
  // General ////////////////////////////////////////////////////////////////// 
  //         //

  private:

    Gtk::Box* inner_box_;    

    Gtk::EventBox* event_box_;

    Gtk::Frame* status_bar_frame_;





   //                 //
   // Playback Status /////////////////////////////////////////////////////////
   //                 //

   private:

    Gtk::Image* playback_status_image_;

    Gtk::Label* playback_status_label_;




   //                    //
   // Playing Track Time //////////////////////////////////////////////////////
   //                    //

   private:

    Gtk::Box* time_label_box_;

    Gtk::Label* time_left_label_;

    Gtk::Label* time_right_label_;





   //                    //
   // Selected Rows Time //////////////////////////////////////////////////////
   //                    //

   private:

    Gtk::Label* selected_rows_count_label_;

    Gtk::Label* selected_time_label_;





   //        //
   // Volume //////////////////////////////////////////////////////////////////
   //        //

   private:

    Gtk::VolumeButton* main_volume_button_;

    Gtk::Box* main_volume_button_box_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
