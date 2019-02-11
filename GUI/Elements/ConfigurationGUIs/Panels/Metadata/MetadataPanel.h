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

#ifndef METADATA_PANEL_H
#define METADATA_PANEL_H





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

#include <string>





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

  class CheckButton;

  class Frame;

  class RadioButton;

  class ScrolledWindow;

  class TextView;

}





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class MetadataPanel : public Panel
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    MetadataPanel(Base& base_ref, ConfigurationGUI& new_config_gui,
                  ConfigurationGUIs& config_guis_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~MetadataPanel();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    virtual void Apply_Saved_Values() final override;

    void Clear_Errors_Log();

    void Cuesheet_Type_Preference_Changed();

    void Files_Or_Cuesheet_Preference_Changed();

    void Guess_Metadata_Toggled();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::RadioButton& cuesheet_radiobutton();

    Gtk::RadioButton& embedded_cuesheet_radiobutton();

    Gtk::RadioButton& external_cuesheet_radiobutton();

    Gtk::RadioButton& files_radiobutton();

    Gtk::CheckButton& guess_metadata_checkbutton();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  private:

    Gtk::Box* errors_log_display_box_;

    Gtk::Box* errors_log_display_inner_box_;





  //            //
  // Cue Sheets ///////////////////////////////////////////////////////////////
  //            //

  private:

    Gtk::Box* cuesheet_box_;

    Gtk::RadioButton* cuesheet_radiobutton_;

    Gtk::Box* cuesheet_type_preference_box_;

    Gtk::Frame* cuesheet_type_preference_frame_;

    Gtk::Box* cuesheet_type_preference_frame_box_;

    Gtk::Label* cuesheet_type_preference_label_;

    Gtk::RadioButton* embedded_cuesheet_radiobutton_;

    Gtk::RadioButton* external_cuesheet_radiobutton_;

    Gtk::RadioButton* files_radiobutton_;

    Gtk::Box* files_or_cuesheet_box_;

    Gtk::Frame* files_or_cuesheet_frame_;

    Gtk::Box* files_or_cuesheet_frame_box_;

    Gtk::Label* files_or_cuesheet_label_;





  //           //
  // Error Log ////////////////////////////////////////////////////////////////
  //           //

  private:

    Gtk::Button* clear_key_button_;

    Gtk::Box* errors_log_box_;

    Gtk::Frame* errors_log_frame_;

    Gtk::ScrolledWindow* errors_log_scrolled_window_;

    Gtk::TextView* error_textview_;





  //                // 
  // Guess Metadata ///////////////////////////////////////////////////////////
  //                //

  Gtk::CheckButton* guess_metadata_checkbutton_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
