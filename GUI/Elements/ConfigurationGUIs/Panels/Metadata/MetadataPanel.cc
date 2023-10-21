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

#include "MetadataPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../../../Configuration/Configuration.h"

#include "../../../../../Errors/Errors.h"

#include "../../../../GUI.h"

#include "../../ConfigurationGUI.h"

#include "../../ConfigurationGUIs.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/ustring.h>

#include <gtkmm/box.h>

#include <gtkmm/checkbutton.h>

#include <gtkmm/frame.h>

#include <gtkmm/liststore.h>

#include <gtkmm/textview.h>

#include <gtkmm/treeview.h>

#include <gtkmm/radiobutton.h>

#include <gtkmm/scrolledwindow.h>

#include <map>





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

MetadataPanel::MetadataPanel
  (Base& base_ref, ConfigurationGUI& new_config_gui,
   ConfigurationGUIs& config_guis_ref)

// Inherited Class

: Panel(base_ref, new_config_gui, "Metadata")



// General 

, errors_log_display_box_(Gtk::manage(new Gtk::Box))

, errors_log_display_inner_box_(Gtk::manage(new Gtk::Box))



// Cue Sheets

, cuesheet_box_
    (Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 20)))

, cuesheet_radiobutton_
    (Gtk::manage(new Gtk::RadioButton("Use Cuesheet Metadata")))

, cuesheet_type_preference_box_
    (Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 5)))

, cuesheet_type_preference_frame_(Gtk::manage(new Gtk::Frame))

, cuesheet_type_preference_frame_box_
    (Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 5)))

, cuesheet_type_preference_label_(Gtk::manage(new Gtk::Label()))

, embedded_cuesheet_radiobutton_
    (Gtk::manage(new Gtk::RadioButton("Embedded Cuesheet Preferred")))

, external_cuesheet_radiobutton_
    (Gtk::manage(new Gtk::RadioButton("External Cuesheet Preferred")))

, files_radiobutton_
    (Gtk::manage(new Gtk::RadioButton("Use File Metadata")))

, files_or_cuesheet_box_
    (Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 5)))

, files_or_cuesheet_frame_(Gtk::manage(new Gtk::Frame))

, files_or_cuesheet_frame_box_
    (Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 5)))

, files_or_cuesheet_label_(Gtk::manage(new Gtk::Label()))



// Error Log

, clear_key_button_(Gtk::manage(new Gtk::Button("Clear Errors Log")))

, errors_log_box_(Gtk::manage(new Gtk::Box))

, errors_log_frame_(Gtk::manage(new Gtk::Frame))

, errors_log_scrolled_window_(Gtk::manage(new Gtk::ScrolledWindow))

, error_textview_(Gtk::manage(new Gtk::TextView))



// Guess Metadata

, guess_metadata_checkbutton_(Gtk::make_managed<Gtk::CheckButton>())

{

  //           //
  // Cue Sheet ////////////////////////////////////////////////////////////////
  //           //

  // Adds the output
  box() . append(*cuesheet_box_, Gtk::PACK_SHRINK);



  // 
  cuesheet_box_ -> set_margin_start(5);

  // 
  cuesheet_box_ -> set_margin_end(5);

  // 
  cuesheet_box_ -> set_margin_top(5);

  // 
  cuesheet_box_ -> set_margin_bottom(5);



  // 
  cuesheet_box_ -> pack_start
    (*cuesheet_type_preference_box_, Gtk::PACK_EXPAND_PADDING);

  // 
  cuesheet_box_ -> pack_end
    (*files_or_cuesheet_box_, Gtk::PACK_EXPAND_PADDING);



  // 
  cuesheet_box_ -> set_margin_top(10);



  // 
  files_or_cuesheet_box_ -> set_center_widget(*files_or_cuesheet_frame_);

  // 
  cuesheet_type_preference_box_ -> set_center_widget(*cuesheet_type_preference_frame_);



  // 
  files_or_cuesheet_frame_ -> set_label_widget(*files_or_cuesheet_label_);

  // 
  files_or_cuesheet_frame_ -> add(*files_or_cuesheet_frame_box_);



  // 
  files_or_cuesheet_frame_box_ -> append(*files_radiobutton_, Gtk::PACK_SHRINK);

  // 
  files_or_cuesheet_frame_box_ -> pack_end(*cuesheet_radiobutton_, Gtk::PACK_SHRINK);



  // 
  cuesheet_type_preference_frame_
    -> set_label_widget(*cuesheet_type_preference_label_);

  // 
  cuesheet_type_preference_frame_ -> add(*cuesheet_type_preference_frame_box_);



  // 
  cuesheet_type_preference_frame_box_
    -> append(*external_cuesheet_radiobutton_, Gtk::PACK_SHRINK);

  // 
  cuesheet_type_preference_frame_box_
    -> pack_end(*embedded_cuesheet_radiobutton_, Gtk::PACK_SHRINK);



  // 
  files_or_cuesheet_label_ -> set_markup
    ("<b>File or Cuesheet Preference</b>");



  // 
  cuesheet_type_preference_label_ -> set_markup
    ("<b>External or Embedded Preference</b>");



  // 
  files_radiobutton_ -> set_margin_start(5);

  // 
  files_radiobutton_ -> set_margin_end(5);

  // 
  files_radiobutton_ -> set_margin_top(5);

  // 
  files_radiobutton_ -> set_margin_bottom(5);



  // 
  cuesheet_radiobutton_ -> set_margin_start(5);

  // 
  cuesheet_radiobutton_ -> set_margin_end(5);

  // 
  cuesheet_radiobutton_ -> set_margin_top(5);

  // 
  cuesheet_radiobutton_ -> set_margin_bottom(5);



  // 
  external_cuesheet_radiobutton_ -> set_margin_start(5);

  // 
  external_cuesheet_radiobutton_ -> set_margin_end(5);

  // 
  external_cuesheet_radiobutton_ -> set_margin_top(5);

  // 
  external_cuesheet_radiobutton_ -> set_margin_bottom(5);



  // 
  embedded_cuesheet_radiobutton_ -> set_margin_start(5);

  // 
  embedded_cuesheet_radiobutton_ -> set_margin_end(5);

  // 
  embedded_cuesheet_radiobutton_ -> set_margin_top(5);

  // 
  embedded_cuesheet_radiobutton_ -> set_margin_bottom(5);



  // 
  cuesheet_radiobutton_ -> join_group(*files_radiobutton_);



  // 
  embedded_cuesheet_radiobutton_
    -> join_group(*external_cuesheet_radiobutton_);



  // 
  files_or_cuesheet_frame_ -> set_tooltip_text
    ("Determines whether to prefer the metadata obtained from the files " \
     "directly or the metadata obtained from the multi-file cuesheet.");

  // 
  cuesheet_type_preference_frame_ -> set_tooltip_text
    ("Determines whether metadata obtained from a cuesheet embedded in a " \
     "single-file release is preferred or metadata obtained from an " \
     "external cuesheet is preferred.");



  //
  string files_or_cuesheet
    = config() . get("metadata.files_or_cuesheet");

  // 
  if(files_or_cuesheet == "cuesheet")
  {

    // 
    cuesheet_radiobutton_ -> set_active(true);

  }

  // 
  else
  {

    // 
    files_radiobutton_ -> set_active(true);

  }



  //
  string cuesheet_type_preference
    = config() . get("metadata.cuesheet_type_preference");

  // 
  if(cuesheet_type_preference == "embedded")
  {

    // 
    embedded_cuesheet_radiobutton_ -> set_active(true);

  }

  // 
  else
  {

    // 
    external_cuesheet_radiobutton_ -> set_active(true);

  }



  // 
  cuesheet_radiobutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &MetadataPanel::Files_Or_Cuesheet_Preference_Changed));

  // 
  files_radiobutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &MetadataPanel::Files_Or_Cuesheet_Preference_Changed));

  // 
  external_cuesheet_radiobutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &MetadataPanel::Cuesheet_Type_Preference_Changed));

  // 
  embedded_cuesheet_radiobutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &MetadataPanel::Cuesheet_Type_Preference_Changed));





  //           //
  // Error Log ////////////////////////////////////////////////////////////////
  //           //

  // Adds the output sink's box to the panel's box.
  box() . append(*errors_log_box_);

  // Adds the output sink's box to the panel's box.
  errors_log_box_ -> pack_start
    (*errors_log_frame_);

  // Adds the output sink's box to the panel's box.
  errors_log_frame_ -> add(*errors_log_scrolled_window_);

  // Adds the output sink's box to the panel's box.
  errors_log_scrolled_window_ -> add(*error_textview_);

  // 
  error_textview_ -> set_buffer(config_guis_ref . error_log_textbuffer());

  // 
  error_textview_ -> set_editable(false);



  // Sets the margin of the errors_log treeview box.
  errors_log_box_ -> set_margin_top(2);

  // Sets the margin of the errors_log treeview box.
  errors_log_box_ -> set_margin_bottom(2);

  // Sets the margin of the errors_log treeview box.
  errors_log_box_ -> set_margin_start(2);

  // Sets the margin of the errors_log treeview box.
  errors_log_box_ -> set_margin_end(2);



  // Sets the behaviour of the scrobbable window of the errors_log treeview to 
  // appear automatically when needed.
  errors_log_scrolled_window_ -> set_policy(Gtk::POLICY_AUTOMATIC,
                                            Gtk::POLICY_AUTOMATIC);


  // Adds the output sink's box to the panel's box.
  box() . pack_end(*errors_log_display_box_, Gtk::PACK_SHRINK);



  // 
  errors_log_display_box_
    -> set_orientation(Gtk::Orientation::HORIZONTAL);



  // 
  errors_log_display_box_ -> pack_end
    (*errors_log_display_inner_box_);

  // 
  errors_log_display_inner_box_ -> set_hexpand(true);





  // 
  errors_log_display_inner_box_
    -> set_orientation(Gtk::Orientation::HORIZONTAL);



  // 
  errors_log_display_inner_box_
    -> append(*clear_key_button_);



  // 
  clear_key_button_ -> set_margin_start(3);

  // 
  clear_key_button_ -> set_margin_end(3);

  // 
  clear_key_button_ -> set_margin_bottom(3);



  // 
  clear_key_button_ -> set_hexpand(true);



  // 
  errors_log_frame_ -> set_tooltip_text
    ("A log of all of the music files that failed to be read.");

  // 
  clear_key_button_ -> set_tooltip_text
    ("Clears all errors in the error log.");



  // 
  clear_key_button_ -> signal_clicked()
    . connect(sigc::mem_fun(*this, &MetadataPanel::Clear_Errors_Log));





  //                //
  // Guess Metadata ///////////////////////////////////////////////////////////
  //                //

  // 
//  cuesheet_box_ -> set_center_widget(*guess_metadata_checkbutton_);



  // 
  guess_metadata_checkbutton_ -> set_label("Guess Metadata");

  // 
  guess_metadata_checkbutton_ -> set_tooltip_text("");



  // 
  guess_metadata_checkbutton_
    -> set_active(config() . get("metadata.guess_metadata"));



  // 
  guess_metadata_checkbutton_ -> set_tooltip_text
    ("Guesses the correct value for certain metadata if missing.\n" \
     "Metadata tags that can be guessed are album artist(s), \n" \
     "artist(s), track total, disc number, and disc total.");



  // 
  guess_metadata_checkbutton_ -> signal_toggled()
    . connect(sigc::mem_fun(*this, &MetadataPanel::Guess_Metadata_Toggled));





  //       //
  // Other ////////////////////////////////////////////////////////////////////
  //       //

  // Adds the Apply_Saved_Value function to the ConfigGUI's list.
  new_config_gui . Add_Apply_Saved_Value_Function(*this,
                                                  &Panel::Apply_Saved_Values);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

MetadataPanel::~MetadataPanel()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void MetadataPanel::Apply_Saved_Values()
{

  //                          //
  // Cuesheet Type Preference //
  //                          //

  //
  string files_or_cuesheet
    = config() . get("metadata.files_or_cuesheet");

  // 
  if(files_or_cuesheet == "cuesheet")
  {

    // 
    cuesheet_radiobutton_ -> set_active(true);

  }

  // 
  else
  {

    // 
    files_radiobutton_ -> set_active(true);

  }



  //                              //
  // Files or Cuesheet Preference //
  //                              //

  //
  string cuesheet_type_preference
    = config() . get("metadata.cuesheet_type_preference");

  // 
  if(cuesheet_type_preference == "embedded")
  {

    // 
    embedded_cuesheet_radiobutton_ -> set_active(true);

  }

  // 
  else
  {

    // 
    external_cuesheet_radiobutton_ -> set_active(true);

  }



  //                //
  // Guess Metadata //
  //                //

  // 
  guess_metadata_checkbutton_
    -> set_active(config() . get("metadata.guess_metadata"));

}

void MetadataPanel::Clear_Errors_Log()
{

  // 
  errors() . Clear_Errors();



  // 
  error_textview_ -> get_buffer() -> set_text("");

}

void MetadataPanel::Cuesheet_Type_Preference_Changed()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  // 
  if(embedded_cuesheet_radiobutton_ -> get_active())
  {

    // 
    config() . set("metadata.cuesheet_type_preference", "embedded");

  }

  // 
  else if(external_cuesheet_radiobutton_ -> get_active())
  {

    // 
    config() . set("metadata.cuesheet_type_preference", "external");

  }



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    if(embedded_cuesheet_radiobutton_ -> get_active())
    { 

      // 
      config_guis_it -> metadata_panel() . embedded_cuesheet_radiobutton()
        . set_active(true);

    }

    // 
    else if(external_cuesheet_radiobutton_ -> get_active())
    { 

      // 
      config_guis_it -> metadata_panel() . external_cuesheet_radiobutton()
        . set_active(true);

    }

  }

  // 
  config_guis() . set_disable_functions(false);

}

void MetadataPanel::Files_Or_Cuesheet_Preference_Changed()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  // 
  if(cuesheet_radiobutton_ -> get_active())
  {

    // 
    config() . set("metadata.files_or_cuesheet", "cuesheet");

  }

  // 
  else if(files_radiobutton_ -> get_active())
  {

    // 
    config() . set("metadata.files_or_cuesheet", "files");

  }



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    if(cuesheet_radiobutton_ -> get_active())
    {

      // 
      config_guis_it -> metadata_panel() . cuesheet_radiobutton()
        . set_active(true);

    }

    // 
    else if(files_radiobutton_ -> get_active())
    {

      // 
      config_guis_it -> metadata_panel() . files_radiobutton()
        . set_active(true);

    }

  }

  // 
  config_guis() . set_disable_functions(false);

}

void MetadataPanel::Guess_Metadata_Toggled()
{

  // 
  if(config_guis() . disable_functions())
  {

    // 
    return;

  }



  // 
  bool active = guess_metadata_checkbutton_ -> get_active();  



  // 
  config() . set("metadata.guess_metadata", active);



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis() . set_disable_functions(true);

  // 
  for(auto config_gui_ptr : config_guis()())
  {

    // 
    config_gui_ptr -> metadata_panel() . guess_metadata_checkbutton()
      . set_active(active);

  }

  // 
  config_guis() . set_disable_functions(false);
}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //
    
Gtk::RadioButton& MetadataPanel::cuesheet_radiobutton()
{

  return *cuesheet_radiobutton_;

}

Gtk::RadioButton& MetadataPanel::embedded_cuesheet_radiobutton()
{ 

  return *embedded_cuesheet_radiobutton_;

}

Gtk::RadioButton& MetadataPanel::external_cuesheet_radiobutton()
{

  return *external_cuesheet_radiobutton_;

}

Gtk::RadioButton& MetadataPanel::files_radiobutton()
{

  return *files_radiobutton_;

}

Gtk::CheckButton& MetadataPanel::guess_metadata_checkbutton()
{

  return *guess_metadata_checkbutton_;

}
