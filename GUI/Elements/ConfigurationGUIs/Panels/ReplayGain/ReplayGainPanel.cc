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

#include "ReplayGainPanel.h"





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

#include <gst/gst.h>

#include <gtkmm/checkbutton.h>

#include <gtkmm/frame.h>

#include <gtkmm/radiobutton.h>

#include <gtkmm/radiobuttongroup.h>

#include <gtkmm/spinbutton.h>

#include <gtkmm/switch.h>





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

ReplayGainPanel::ReplayGainPanel(Base& base_ref, 
                                 ConfigurationGUI& new_config_gui,
                                 Gtk::TreeRow* parent_row_ptr)

// Inherited Class

: Panel(base_ref, new_config_gui, "Replay Gain", parent_row_ptr)



// General

, rg_options_box_(Gtk::manage(new Gtk::Box))



// Enable Switch

, enable_switch_(Gtk::manage(new Gtk::Switch))

, enable_switch_box_(Gtk::manage(new Gtk::Box))



// Fallback Gain

, fallback_gain_sbs_(new SpinButtonScale(-60, 60, 3))

, fallback_gain_sbs_frame_(Gtk::manage(new Gtk::Frame))



// Gain Type

, album_gain_radiobutton_(Gtk::manage(new Gtk::RadioButton("Album Mode")))

, gain_type_box_(Gtk::manage(new Gtk::Box))

, track_gain_radiobutton_(Gtk::manage(new Gtk::RadioButton("Track Mode")))



// Limiter

, limiter_box_(Gtk::manage(new Gtk::Box))

, limiter_checkbutton_(Gtk::manage(new Gtk::CheckButton))



// Pre-Amp

, pre_amp_sbs_(new SpinButtonScale(-60, 60, 3))

, pre_amp_sbs_frame_(Gtk::manage(new Gtk::Frame))

{

  // Adds the pre-amp sbs frame to the start of the GUIElement box.
  box() . pack_start(*pre_amp_sbs_frame_, Gtk::PACK_EXPAND_WIDGET);

  // Adds the fallback gain sbs frame to the start of the GUIElement box.
  box() . pack_start(*fallback_gain_sbs_frame_, Gtk::PACK_EXPAND_WIDGET);

  // Adds rg_options_box_ to the end of the GUIElement box.
  box() . pack_end(*rg_options_box_, Gtk::PACK_EXPAND_WIDGET);



  // Adds enable_switch_box_ frame to the end of rg_options_box.
//  rg_options_box_ -> pack_start(*enable_switch_box_, Gtk::PACK_EXPAND_PADDING);

  // Adds limter_box_ frame to the end of rg_options_box.
  rg_options_box_ -> pack_end(*limiter_box_, Gtk::PACK_EXPAND_PADDING);

  // Adds enable_switch_box_ frame to the end of rg_options_box.
  rg_options_box_ -> pack_end(*gain_type_box_, Gtk::PACK_EXPAND_PADDING);



  // Adds the Apply_Saved_Value function to the ConfigGUI's list.
  new_config_gui . Add_Apply_Saved_Value_Function(*this,
                                                  &Panel::Apply_Saved_Values);





  //               //
  // Enable Switch ////////////////////////////////////////////////////////////
  //               //

  // Sets enable_switch_ as the center of enable_switch_box_.
  enable_switch_box_ -> set_center_widget(*enable_switch_);



  // Sets the orientation of enable_switch_box_ as vertical.
  enable_switch_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // Connects the switch's changed signal to a callback function.
  enable_switch_ -> property_active() . signal_changed()
    . connect(sigc::mem_fun
        (*this, &ReplayGainPanel::On_Enable_Switch_Changed_Signal));




 
  //               //
  // Fallback Gain ////////////////////////////////////////////////////////////
  //               //

  // Adds fallback_gain_sbs_ to its frame.
  fallback_gain_sbs_frame_ -> add(fallback_gain_sbs_ -> box());



  // Sets the text of the frame's label.
  fallback_gain_sbs_frame_ -> set_label("Fallback Gain");



  // Sets fallback_gain_sbs_frame's top margin as 3.
  fallback_gain_sbs_frame_ -> set_margin_top(3);

  // Sets fallback_gain_sbs_frame's bottom margin as 3.
  fallback_gain_sbs_frame_ -> set_margin_bottom(3);

  // Sets fallback_gain_sbs_frame's left margin as 3.
  fallback_gain_sbs_frame_ -> set_margin_left(3);

  // Sets fallback_gain_sbs_frame's right margin as 3.
  fallback_gain_sbs_frame_ -> set_margin_right(3);



  // 
  fallback_gain_sbs_ -> box()
    . set_tooltip_text("The fallback gain amount of tracks without " \
                        "ReplayGain data.");



  // Connects the fallback_gain_sbs_ to its value changed callback.
  fallback_gain_sbs_ -> sbs_spinbutton() . signal_value_changed()
    . connect(sigc::mem_fun
        (*this, &ReplayGainPanel::On_Fallback_Gain_SBS_Value_Changed_Signal));





  //           //
  // Gain Type ////////////////////////////////////////////////////////////////
  //           //

  // Adds the album gain RadioButton to gain_type_box_.
  gain_type_box_
    -> pack_start(*album_gain_radiobutton_, Gtk::PACK_EXPAND_PADDING);

  // Adds the track gain RadioButton to gain_type_box_.
  gain_type_box_
    -> pack_end(*track_gain_radiobutton_, Gtk::PACK_EXPAND_PADDING);



  // Sets the orientation of gain_type_box_ to vertical.
  gain_type_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // Adds the track gain radiobutton to the same group as the album one.
  track_gain_radiobutton_ -> join_group(*album_gain_radiobutton_);



  // 
  album_gain_radiobutton_
    -> set_tooltip_text("Sets OMP to use the track's Album Gain.");

  // 
  track_gain_radiobutton_
    -> set_tooltip_text("Sets OMP to use a track's Track Gain");



  // Attaches the album gain RadioButton to its toggled signal.
  album_gain_radiobutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &ReplayGainPanel::On_Gain_Type_Radiobutton_Toggled_Signal));

  // Attaches the track gain RadioButton to its toggled signal.
  track_gain_radiobutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &ReplayGainPanel::On_Gain_Type_Radiobutton_Toggled_Signal));





  //         //
  // Limiter //////////////////////////////////////////////////////////////////
  //         //

  // Adds the limiter checkbutton to its box.
  limiter_box_ -> pack_end(*limiter_checkbutton_, Gtk::PACK_EXPAND_PADDING);



  // Sets the orietnation of limiter_box_ to vertical.
  limiter_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // Sets the label of the limiter CheckButton.
  limiter_checkbutton_ -> set_label("Limiter");


  // 
  limiter_checkbutton_
    -> set_tooltip_text("DSP to help prevent or reduce clipping even if it " \
                        "was not caused by ReplayGain.");




  // Connects the limiter to its toggle signal callback.
  limiter_checkbutton_ -> signal_toggled()
    . connect(sigc::mem_fun
        (*this, &ReplayGainPanel::On_Limiter_Checkbutton_Toggled_Signal));





  //         //
  // Pre-Amp //////////////////////////////////////////////////////////////////
  //         //

  // Adds the box of pre_amp_sbs_ to its frame.
  pre_amp_sbs_frame_ -> add(pre_amp_sbs_ -> box());



  // Sets the label of the pre-amp frame.
  pre_amp_sbs_frame_ -> set_label("Pre-Amp");



  // Sets the pre-amp sbs frame to have a top margin of 3.
  pre_amp_sbs_frame_ -> set_margin_top(3);

  // Sets the pre-amp sbs frame to have a bottom margin of 3.
  pre_amp_sbs_frame_ -> set_margin_bottom(3);

  // Sets the pre-amp sbs frame to have a left margin of 3.
  pre_amp_sbs_frame_ -> set_margin_left(3);

  // Sets the pre-amp sbs frame to have a right margin of 3.
  pre_amp_sbs_frame_ -> set_margin_right(3);



  // 
  pre_amp_sbs_ -> box()
    . set_tooltip_text("The pre-amp gain amount of tracks with" \
                        "ReplayGain data.");




  // Connects the pre_amp_sbs_ to its value changed callback.
  pre_amp_sbs_ -> sbs_spinbutton() . signal_value_changed()
    . connect(sigc::mem_fun
        (*this, &ReplayGainPanel::On_Pre_Amp_SBS_Value_Changed_Signal));

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

ReplayGainPanel::~ReplayGainPanel()
{

  // Deletes the fallback gain sbs.
  delete fallback_gain_sbs_;

  // Deletes the pre-amp sbs.
  delete pre_amp_sbs_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void ReplayGainPanel::Apply_Saved_Values()
{

  //                //
  // Enabled Switch ///////////////////////////////////////////////////////////
  //                //

  // Stores whether ReplayGain is enabled or not.
  bool enabled = config() . get("replaygain.enabled");



  // Sets whether the enable switch is active. 
  enable_switch_ -> set_active(enabled);



  // Sets whether the fallback gain SpinButtonScale is sensitive.
  fallback_gain_sbs_ -> Set_Sensitive(enabled);

  // Sets whether the pre-amp SpinButtonScale is sensitive.
  pre_amp_sbs_ -> Set_Sensitive(enabled);

  // Sets whether album gain RadioButton is sensitive.
  album_gain_radiobutton_ -> set_sensitive(enabled);

    // Sets whether track gain RadioButton is sensitive.
  track_gain_radiobutton_ -> set_sensitive(enabled);

  // Sets whether limiter CheckButton is sensitive.
  limiter_checkbutton_ -> set_sensitive(enabled);





  //                   //
  // Fallback Gain SBS ////////////////////////////////////////////////////////
  //                   //

  fallback_gain_sbs() . set_value(config() . get("replaygain.fallback_gain"));





  //           //
  // Gain Type ////////////////////////////////////////////////////////////////
  //           //

  // 
  string saved_gain_type = config() . get("replaygain.gain_type");

  // 
  if(saved_gain_type == "album")
  {

    // 
    album_gain_radiobutton_ -> set_active(true);

  }

  // 
  else
  {

    // 
    track_gain_radiobutton_ -> set_active(true);

  }





  //         //
  // Limiter //////////////////////////////////////////////////////////////////
  //         //

  limiter_checkbutton_
    -> set_active(config() . get("replaygain.limiter"));





  //                  //
  // Pre-Amp Gain SBS /////////////////////////////////////////////////////////
  //                  //

  pre_amp_sbs() . set_value(config() . get("replaygain.pre_amp"));

}

void ReplayGainPanel::On_Enable_Switch_Changed_Signal()
{

  // True if ConfigGUI functions are disabled.
  if(config_guis() . disable_functions())
  {

    // Ends the function.
    return;

  }

  // Disables ConfigGUI functions.
  config_guis() . set_disable_functions(true);




  // Stores whether the enable switch is on or not.
  bool enabled = enable_switch_ -> get_active();

  

  // Sets the configuration setting.
  config() . set("replaygain.enabled", enabled);



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // Iterates through all of the ConfigGUIs.
  for(auto it : (config_guis()()))
   {

    // Sets whether the enable switch is active. 
    it -> replay_gain_panel() . enable_switch()
      . set_active(enabled);



    // Sets whether the fallback gain SpinButtonScale is sensitive.
    it -> replay_gain_panel() . fallback_gain_sbs()
      . Set_Sensitive(enabled);

    // Sets whether the pre-amp SpinButtonScale is sensitive.
    it -> replay_gain_panel() . pre_amp_sbs()
      . Set_Sensitive(enabled);

    // Sets whether album gain RadioButton is sensitive.
    it -> replay_gain_panel() . album_gain_radiobutton()
      . set_sensitive(enabled);

    // Sets whether track gain RadioButton is sensitive.
    it -> replay_gain_panel() . track_gain_radiobutton()
      . set_sensitive(enabled);

    // Sets whether limiter CheckButton is sensitive.
    it -> replay_gain_panel() . limiter_checkbutton()
      . set_sensitive(enabled);

  }



  // Enables ConfigGUI functions.
  config_guis() . set_disable_functions(false);



} 

void ReplayGainPanel::On_Fallback_Gain_SBS_Value_Changed_Signal()
{

  // True if ConfigGUI functions are disabled.
  if(config_guis() . disable_functions())
  {

    // Ends the function.
    return;

  }

  // Disables ConfigGUI functions.
  config_guis() . set_disable_functions(true);



  // Retrieves the new value.
  double new_value = fallback_gain_sbs_ -> sbs_spinbutton() . get_value();



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // Iterates through all of the ConfigGUIs.
  for(auto it : config_guis()())
  {

    // Sets the new value in the current ConfigGUI.
    it -> replay_gain_panel() . fallback_gain_sbs() . set_value(new_value);

  }



  // Assigns the correct pre-amp value to the rg pre-amp.
  g_object_set(G_OBJECT(playback() . replay_gain()),
               "fallback-gain", new_value, NULL);



  // Sets the new value in the configuration.
  config() . set("replaygain.fallback_gain", new_value);



  // Enables ConfigGUI functions.
  config_guis() . set_disable_functions(false);

}

void ReplayGainPanel::On_Gain_Type_Radiobutton_Toggled_Signal()
{

  // True if ConfigGUI functions are disabled.
  if(config_guis() . disable_functions())
  {

    // Ends the function.
    return;

  }

  // Disables ConfigGUI functions.
  config_guis() . set_disable_functions(true);



  // Retrieves the new value.
  string new_gain_type_str = "";

  // True if the album gain RadioButton is active.
  if(album_gain_radiobutton_ -> get_active())
  {

    // Sets new_gain_type_str.
    new_gain_type_str = "album";

  }

  // True if the track gain RadioButton is active.
  else
  {

    // Sets new_gain_type_str.
    new_gain_type_str = "track";

  }



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // Iterates through all of the ConfigGUIs.
  for(auto it : config_guis()())
  {

    // True if the gain type is album.
    if(new_gain_type_str == "album")
    {

      // Sets the new value in the current ConfigGUI.
      it -> replay_gain_panel() . album_gain_radiobutton() . set_active(true);

    }

    // True if the gain type is track.
    else
    {

      // Sets the new value in the current ConfigGUI.
      it -> replay_gain_panel() . track_gain_radiobutton() . set_active(true);

    }

  }



  // True if album mode.
  if(new_gain_type_str == "album")
  {

    // Sets the ReplayGain type as album.
    g_object_set(G_OBJECT(playback() . replay_gain()), "album-mode", TRUE, NULL);

  }

  // True if track mode.
  else
  {

    // Sets the ReplayGain type as track.
    g_object_set(G_OBJECT(playback() . replay_gain()), "album-mode", FALSE, NULL);

  }



  // Sets the new value in the configuration.
  config() . set("replaygain.gain_type", new_gain_type_str);



  // Enables ConfigGUI functions.
  config_guis() . set_disable_functions(false);

}

void ReplayGainPanel::On_Limiter_Checkbutton_Toggled_Signal()
{

  // True if ConfigGUI functions are disabled.
  if(config_guis() . disable_functions())
  {

    // Ends the function.
    return;

  }

  // Disables ConfigGUI functions.
  config_guis() . set_disable_functions(true);



  // Retrieves the new value.
  bool new_value = limiter_checkbutton_ -> get_active();



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // Iterates through all of the ConfigGUIs.
  for(auto it : config_guis()())
  {

    // Sets the new value in the current ConfigGUI.
    it -> replay_gain_panel() . limiter_checkbutton() . set_active(new_value);

  }



  // True if the ReplayGain limiter is enabled.
  if(new_value)
  {

    // Sets the ReplayGain limiter as enabled.
    g_object_set(G_OBJECT(playback() . replay_gain_limiter()),
                 "enabled", TRUE, NULL);

  }

  // True if the ReplayGain limiter is disabled.
  else
  {

    // Sets the ReplayGain limiter as disabled.
    g_object_set(G_OBJECT(playback() . replay_gain_limiter()),
                 "enabled", FALSE, NULL);

  }



  // Sets the new value in the configuration.
  config() . set("replaygain.limiter", new_value);



  // Enables ConfigGUI functions.
  config_guis() . set_disable_functions(false);

}

void ReplayGainPanel::On_Pre_Amp_SBS_Value_Changed_Signal()
{

  // True if ConfigGUI functions are disabled.
  if(config_guis() . disable_functions())
  {

    // Ends the function.
    return;

  }

  // Disables ConfigGUI functions.
  config_guis() . set_disable_functions(true);



  // Retrieves the new value.
  double new_value = pre_amp_sbs_ -> sbs_spinbutton() . get_value();



  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // Iterates through all of the ConfigGUIs.
  for(auto it : config_guis()())
  {

    // Sets the new value in the current ConfigGUI.
    it -> replay_gain_panel() . pre_amp_sbs() . set_value(new_value);

  }



  // Assigns the correct pre-amp value to the rg pre-amp.
  g_object_set(G_OBJECT(playback() . replay_gain()), "pre-amp",
               new_value, NULL);



  // Sets the new value in the configuration.
  config() . set("replaygain.pre_amp", new_value);



  // Enables ConfigGUI functions.
  config_guis() . set_disable_functions(false);

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::RadioButton& ReplayGainPanel::album_gain_radiobutton()
{

  return *album_gain_radiobutton_;

}

Gtk::Switch& ReplayGainPanel::enable_switch()
{

  return *enable_switch_;

}

SpinButtonScale& ReplayGainPanel::fallback_gain_sbs()
{

  return *fallback_gain_sbs_;

}

Gtk::CheckButton& ReplayGainPanel::limiter_checkbutton()
{

  return *limiter_checkbutton_;

}

SpinButtonScale& ReplayGainPanel::pre_amp_sbs()
{

  return *pre_amp_sbs_;

}

Gtk::RadioButton& ReplayGainPanel::track_gain_radiobutton()
{

  return *track_gain_radiobutton_;

}
