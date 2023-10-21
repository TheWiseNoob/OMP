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

#include "OutputPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../../../Configuration/Configuration.h"

#include "../../../../../Playback/Playback.h"

#include "../../../../SpinButtonScale.h"

#include "../../ConfigurationGUI.h"

#include "../../ConfigurationGUIs.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/comboboxtext.h>

#include <gtkmm/box.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/spinbutton.h>

#include <gtkmm/treeiter.h>

#include <iostream>





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

OutputPanel::OutputPanel(Base& new_main, 
                         ConfigurationGUI& new_config_gui)

// Inherited Class

: Panel(new_main, new_config_gui, "Output")



// Queue Buffer

, buffer_box_(Gtk::manage(new Gtk::Box))

, buffer_frame_(Gtk::manage(new Gtk::Frame))

, buffer_time_spinbuttonscale_(new SpinButtonScale(30000, 3600000, 0))

, buffer_time_spinbuttonscale_label_(Gtk::manage(new Gtk::Label))

, buffer_time_spinbuttonscale_label_box_(Gtk::manage(new Gtk::Box))

, buffer_time_spinbuttonscale_label_center_box_(Gtk::manage(new Gtk::Box))



// Sink 

, output_sink_box_(Gtk::manage(new Gtk::Box))

, output_sink_combobox_(Gtk::manage(new Gtk::ComboBoxText))

, output_sink_combobox_box_(Gtk::manage(new Gtk::Box))

, output_sink_combobox_name_label_(Gtk::manage(new Gtk::Label))

{

  //             //
  // Output Sink //////////////////////////////////////////////////////////////
  //             //

  // Adds the output sink's box to the panel's box.
  box() . append(*output_sink_box_, Gtk::PACK_EXPAND_WIDGET);

  // Adds the output sink combobox to the output sink box.
  output_sink_box_ -> append(*output_sink_combobox_box_, 
                                 Gtk::PACK_EXPAND_WIDGET);

  // Adds the sink combobox label to output_sink_combobox_box_.
  output_sink_combobox_box_ 
    -> append(*output_sink_combobox_name_label_, Gtk::PACK_SHRINK);

  // Sets the sink combobox as the center widget of output_sink_combobox_box_.
  output_sink_combobox_box_ -> set_center_widget(*output_sink_combobox_);



  // Sets the output sink box to expand horizontally. 
  output_sink_box_ -> set_hexpand(true);



  // Sets the left margin of output_sink_box_.
  output_sink_box_ -> set_margin_left(3);

  // Sets the right margin of output_sink_box_.
  output_sink_box_ -> set_margin_right(3);



  // Sets the output sink's label text.
  output_sink_combobox_name_label_ -> set_label("Output Sink:");



  // Makes the label align to the left.
  output_sink_combobox_name_label_ -> set_alignment(0.00,0.00);



  // Sets the margin of the left side of the sink combobox label.
  output_sink_combobox_name_label_ -> set_margin_left(6);

  // Sets the margin of the left side of the sink combobox label.
  output_sink_combobox_name_label_ -> set_margin_top(10);



  // Sets the orietnation of the output sink box.
  output_sink_box_ -> set_orientation(Gtk::Orientation::VERTICAL);

  // Sets the orietnation of the output sink combobox box.
  output_sink_combobox_box_ -> set_orientation(Gtk::Orientation::VERTICAL);



  // Adds a row called Automatic to the output_sink_combobox_
  output_sink_combobox_ -> append("Automatic");

  // Adds a row called ALSA to the output_sink_combobox_
  output_sink_combobox_ -> append("ALSA");

  // Adds a row called PulseAudio to the output_sink_combobox_
  output_sink_combobox_ -> append("PulseAudio");

  // Adds a row called JACK to the output_sink_combobox_
  output_sink_combobox_ -> append("JACK");



  // 
  output_sink_box_
    -> set_tooltip_text("Sets the output source of the audio to either " \
                        "ALSA, PulseAudio, JACK, or automatic. Does not " \
                        "take effect until after playback is stopped if OMP " \
                        "is playing when this setting is changed.");



  // Connects the output_sink_combobox_ to its callback.
  output_sink_combobox_ -> signal_changed()
    . connect(
        sigc::mem_fun
          (*this, &OutputPanel::On_Signal_Changed_Output_Sink_Combobox));





  //        //
  // Buffer ///////////////////////////////////////////////////////////////////
  //        //

  // Adds the buffer frame to the GUIElement box.
  box() . pack_end(*buffer_frame_, Gtk::PACK_EXPAND_WIDGET);

  // Adds the buffer box to the buffer frame.
  buffer_frame_ -> add(*buffer_box_);

  // Adds the buffer time SpinButtonScale box to the buffer box.
  buffer_box_
    -> pack_end(buffer_time_spinbuttonscale_ -> box(), Gtk::PACK_SHRINK);

  // Adds a box to center the buffer time.
  buffer_box_
    -> append(*buffer_time_spinbuttonscale_label_center_box_,
                  Gtk::PACK_SHRINK);

  // Adds a box for time format conversion to the buffer time SpinButtonScale.
  buffer_time_spinbuttonscale_label_center_box_
    -> set_center_widget(*buffer_time_spinbuttonscale_label_box_);

  // Adds a label for seconds conversion to the buffer time buffer time box.
  buffer_time_spinbuttonscale_label_box_
    -> pack_end(*buffer_time_spinbuttonscale_label_, Gtk::PACK_SHRINK);

  // Removes the SpinButton from the SpinButtonScale box.
  buffer_time_spinbuttonscale_ -> box()
    . remove(buffer_time_spinbuttonscale_ -> sbs_spinbutton_box());

  // Adds the SpinButton to buffer_time_spinbuttonscale_label_box_.
  buffer_time_spinbuttonscale_label_box_
    -> append(buffer_time_spinbuttonscale_ -> sbs_spinbutton_box(),
                  Gtk::PACK_SHRINK);



  // Sets the buffer box's orientation as vertical.
  buffer_box_ -> set_orientation(Gtk::Orientation::VERTICAL);

  // Sets the orientation of the buffer time SpinButtonScale as horizontal.
  buffer_time_spinbuttonscale_ -> sbs_spinbutton_box()
    . set_orientation(Gtk::Orientation::HORIZONTAL);



  // Sets the top margin of the buffer box. 
  buffer_box_ -> set_margin_top(5);

  // Sets the bottom margin of the buffer box. 
  buffer_box_ -> set_margin_bottom(5);

  // Sets the left margin of the buffer box. 
  buffer_box_ -> set_margin_left(5);

  // Sets the right margin of the buffer box. 
  buffer_box_ -> set_margin_right(5);



  // 
  buffer_box_
    -> set_tooltip_text("The minimum amount of total music time OMP will " \
                        "prepare to be played next. The lowest minimum is " \
                        "30 seconds and the highest minumum is 60 minutes.");



  // Sets the left margin of the buffer frame. 
  buffer_frame_ -> set_margin_left(4);

  // Sets the right margin of the buffer box. 
  buffer_frame_ -> set_margin_right(4);

  // Sets the top margin of the buffer box. 
  buffer_frame_ -> set_margin_top(4);

  // Sets the bottom margin of the buffer box. 
  buffer_frame_ -> set_margin_bottom(4);



  // Sets the increments of the SpinButtonScale for buffer time.
  buffer_time_spinbuttonscale_ -> sbs_spinbutton()
    . set_increments(15000, 0.1);



  // Sets the left margin of the buffer frame. 
  buffer_time_spinbuttonscale_label_ -> set_margin_left(3);



  // Sets the label of the buffer frame label.
  buffer_frame_ -> set_label("Buffer");



  // Sets the callback of the buffer SpinButtonScale.
  buffer_time_spinbuttonscale_ -> sbs_spinbutton() . signal_changed()
    . connect(sigc::mem_fun
        (*this, &OutputPanel::On_Signal_Changed_Buffer_Time_SpinButtonScale));





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

OutputPanel::~OutputPanel()
{

  // Destroys the buffer time SpinButtonScale during destruction of the class.
  delete buffer_time_spinbuttonscale_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void OutputPanel::Apply_Saved_Values()
{

  //      //
  // Sink /////////////////////////////////////////////////////////////////////
  //      //

  string sink_str  = config() . get("output.sink");



  stringstream debug_ss;

  debug_ss << "Output sink applied: " << sink_str;

  debug(debug_ss . str() . c_str());



  // Once assigned, it is used to select the appropriate row in the combobox.
  int row_number;

  // True if the new sink is Automatic.
  if(sink_str == "autoaudiosink")
  {

    // Assigns the appropriate row number.
    row_number = 0;

  }

  // True if the new sink is Automatic.
  else if(sink_str == "alsasink")
  {

    // Assigns the appropriate row number.
    row_number = 1;

  }

  // True if the new sink is Automatic.
  else if(sink_str == "pulsesink")
  {

    // Assigns the appropriate row number.
    row_number = 2;

  }

  // True if the new sink is Automatic.
  else if(sink_str == "jackaudiosink")
  {

    // Assigns the appropriate row number.
    row_number = 3;

  }

  // True if there is no setting. 
  else
  {

    // Assigns the first row.
    row_number = 0;



    // Adds a new setting for the Automatic sink.
    config() . set("output.sink", "autoaudiosink");

  }



  // Sets the appropriate row as active.
  output_sink_combobox_ -> set_active(row_number);



  //        //
  // Buffer ///////////////////////////////////////////////////////////////////
  //        //

  // Sets the value of the buffer SpinButtonScale using the configuration.
  buffer_time_spinbuttonscale_ 
    -> set_value(int(config() . get("output.buffer_time")));



  // Creates a string to hold the buffer time label text.
  string label_text = "";

  // Gets the 
  int buffer_time_milliseconds
    = buffer_time_spinbuttonscale_ -> sbs_spinbutton() . get_value();

  // Holds the seconds leftover amount of the buffer queue.
  int seconds = (buffer_time_milliseconds % 60000) / 1000;



  // True if the buffer time is greater than or equal to 1 minute.
  if(buffer_time_milliseconds >= 60000)
  {

    // Retrieves only the minutes from the time.
    int minutes = (buffer_time_milliseconds) / 60000;



    // Combines text for the buffer time label using minutes and seconds.
    label_text = "milliseconds (" + to_string(minutes) + " minutes " 
                   + to_string(seconds) + " seconds)";

    // Sets the new buffer time label.
    buffer_time_spinbuttonscale_label_ -> set_text(label_text);

  }

  // 
  else
  {

    // Combines text for the buffer time label using seconds.
    label_text = "milliseconds (0 minutes " 
                   + to_string(seconds) + " seconds)";

    // Sets the new buffer time label.
    buffer_time_spinbuttonscale_label_ -> set_text(label_text);

  }

}

void OutputPanel::On_Signal_Changed_Output_Sink_Combobox()
{

  // 
  if(config_guis().disable_functions())
  {

    return;

  }

  // 
  config_guis().set_disable_functions(true);



  // 
  Glib::ustring sink_factory_name;

  // 
  int row_number = output_sink_combobox_ -> get_active_row_number();

  // 
  switch(row_number)
  {

    case 0:
    {

      sink_factory_name = "autoaudiosink"; 

      break;

    }
    case 1:
    {

      sink_factory_name = "alsasink";

      break;

    }
    case 2:
    {

      sink_factory_name = "pulsesink";

      break;

    }
    case 3:
    {

      sink_factory_name = "jackaudiosink";

      break;

    }

  }



  // 
  config() . set("output.sink", sink_factory_name);



  // 
  playback() . change_sink() = true;



  // 
  config_guis() . Mark_Unsaved_Changes(true);



  // 
  config_guis().set_disable_functions(true);

  // 
  for(auto it : config_guis()())
  {

    // 
    it -> output_panel().output_sink_combobox().set_active(row_number);

  }



  // 
  config_guis().set_disable_functions(false);

} 

void OutputPanel::On_Signal_Changed_Buffer_Time_SpinButtonScale()
{

  // True
  if(config_guis().disable_functions())
  {

    return;

  }

  config_guis().set_disable_functions(true);

  int new_value = buffer_time_spinbuttonscale_ -> get_value();





  config().set("output.buffer_time", new_value);

  int temp_seconds = new_value / 1000;



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // 
  for(auto it : config_guis()())
  { 

    it -> output_panel().buffer_time_spinbuttonscale().set_value(new_value);



    string label_text;



    if(new_value > 60000)
    {

      // 
      int seconds = (new_value % 60000) / 1000;

      // 
      int minutes = (new_value - seconds) / 60000;



      // 
      label_text = "milliseconds (" + to_string(minutes) + " minutes " + to_string(seconds) + " seconds)";



      it -> output_panel().buffer_time_spinbuttonscale_label()
        .set_text(label_text);

    }
    else
    {

      it -> output_panel().buffer_time_spinbuttonscale_label()
        .set_text("milliseconds (0 minutes " + to_string(temp_seconds) + " seconds)");

    } 

  }

  // 
  config_guis().set_disable_functions(false);

}
