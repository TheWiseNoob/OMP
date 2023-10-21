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

#include "ScrobblingPanel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../../../Base.h"

#include "../../../../../Configuration/Configuration.h"

#include "../../../../../Scrobbling/FailedScrobblesDatabase.h"

#include "../../../../../Scrobbling/Scrobbling.h"

#include "../../../../GUI.h"

#include "../../../../SpinButtonScale.h"

#include "../../ConfigurationGUIs.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/checksum.h>

#include <glibmm/main.h>

#include <gtkmm/box.h>

#include <gtkmm/button.h>

#include <gtkmm/entry.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/scale.h>

#include <gtkmm/spinbutton.h>

#include <gtkmm/spinner.h>

#include <gtkmm/switch.h>

#include <iostream>





//        //
//        //
//        //
// Macros /////////////////////////////////////////////////////////////////////
//        //
//        //
//        //

namespace sigc
{ 

  SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE

}





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

ScrobblingPanel::ScrobblingPanel(Base& base_ref, 
                                 ConfigurationGUI& new_config_gui)

// Inherited Class

: Panel(base_ref, new_config_gui, "Scrobbling")



// Account Information

, lastfm_box_(Gtk::manage(new Gtk::Box))

, lastfm_frame_(Gtk::manage(new Gtk::Frame))

, lastfm_login_button_(Gtk::manage(new Gtk::Button))

, lastfm_password_entry_(Gtk::manage(new Gtk::Entry))

, lastfm_spinner_(Gtk::manage(new Gtk::Spinner))

, lastfm_sub_box_(Gtk::manage(new Gtk::Box))

, lastfm_switch_(Gtk::manage(new Gtk::Switch))

, lastfm_username_entry_(Gtk::manage(new Gtk::Entry))



// Failed Scrobbles

, clear_failed_scrobbles_button_
    (Gtk::manage(new Gtk::Button("Clear Failed Scrobbles")))
 
, failed_scrobbles_box_(Gtk::manage(new Gtk::Box))
 
, failed_scrobbles_count_label_
    (Gtk::manage(new Gtk::Label("0 Failed Scrobbles")))
 
, failed_scrobbles_label_
    (Gtk::manage(new Gtk::Label("", 0 , 0)))

, reattempt_failed_scrobbles_button_
    (Gtk::manage(new Gtk::Button("Reattempt Failed Scrobbles")))
 
, reattempt_scrobbles_spinner_(Gtk::manage(new Gtk::Spinner))



// Scrobble Percent

, scrobble_percent_label_
    (Gtk::manage(new Gtk::Label("", 0, 0)))

, scrobble_percent_spinbuttonscale_(new SpinButtonScale(10, 95, 0))
 
{

  // 
  box() . set_spacing(5);



  // 
  box() . set_margin_left(5);

  // 
  box() . set_margin_right(5);

  // 
  box() . set_margin_top(10);

  // 
  box() . set_margin_bottom(5);





  //                  //
  // Scrobble Percent /////////////////////////////////////////////////////////
  //                  //

  // 
  box() . append(*scrobble_percent_label_, Gtk::PACK_SHRINK);

  // 
  box() . append(scrobble_percent_spinbuttonscale_ -> box(), 
                     Gtk::PACK_EXPAND_WIDGET);



  // 
  scrobble_percent_label_ -> set_markup("<u>Scrobble Percent</u>:");



  //  
  scrobble_percent_label_ -> set_margin_left(6);



  //
  scrobble_percent_spinbuttonscale_ -> box()
    . set_tooltip_text("Minimum percent of the track that must be played " \
                       "before the track is scrobbled.");



  // 
  scrobble_percent_spinbuttonscale_ -> sbs_scale() . signal_value_changed()
    . connect(sigc::mem_fun
        (*this,
         &ScrobblingPanel::Scrobble_Percent_SpinButtonScale_Signal_Changed));





  //                  //
  // Failed Scrobbles /////////////////////////////////////////////////////////
  //                  //

  // 
  box() . append(*failed_scrobbles_label_, Gtk::PACK_SHRINK);

  // 
  box() . append(*failed_scrobbles_box_, Gtk::PACK_EXPAND_WIDGET);



  // 
  failed_scrobbles_label_ -> set_markup("<u>Failed Scrobbles</u>:");



  // 
  failed_scrobbles_box_ -> set_orientation(Gtk::Orientation::HORIZONTAL);



  // 
  failed_scrobbles_label_ -> set_margin_left(6);



  // 
  Gtk::Box* clear_failed_scrobbles_button_box_ = Gtk::manage(new Gtk::Box);

  // 
  Gtk::Box* reattempt_failed_scrobbles_button_box_ = Gtk::manage(new Gtk::Box);



  // 
  failed_scrobbles_box_
    -> append(*clear_failed_scrobbles_button_box_,
                  Gtk::PACK_EXPAND_WIDGET);

  // 
  failed_scrobbles_box_
    -> append(*reattempt_failed_scrobbles_button_box_,
                  Gtk::PACK_EXPAND_WIDGET);

  // 
  failed_scrobbles_box_
    -> pack_end(*failed_scrobbles_count_label_, Gtk::PACK_EXPAND_WIDGET);



  // 
  clear_failed_scrobbles_button_box_
    -> set_orientation(Gtk::Orientation::VERTICAL);



  // 
  clear_failed_scrobbles_button_box_
    -> set_center_widget(*clear_failed_scrobbles_button_);



  // 
  clear_failed_scrobbles_button_ -> set_vexpand(false);



  // 
  reattempt_failed_scrobbles_button_box_
    -> set_orientation(Gtk::Orientation::VERTICAL);



  // 
  reattempt_failed_scrobbles_button_box_
    -> set_center_widget(*reattempt_failed_scrobbles_button_);

  // 
  reattempt_failed_scrobbles_button_
    -> set_image(*reattempt_scrobbles_spinner_);



  // 
  reattempt_failed_scrobbles_button_ -> set_vexpand(false);



  // 
  reattempt_failed_scrobbles_button_ -> set_margin_left(6);



  // 
  int failed_scrobbles_amount
    = scrobbling() . failed_scrobbles_database() . database_size();

  // 
  failed_scrobbles_count_label_
    -> set_text(to_string(failed_scrobbles_amount) + " Failed Scrobbles");

  // 
  if(failed_scrobbles_amount < 1)
  {

    // 
    clear_failed_scrobbles_button_ -> set_sensitive(false);

    // 
    reattempt_failed_scrobbles_button_ -> set_sensitive(false);

  }



  // 
  failed_scrobbles_count_label_ -> set_margin_left(6);



  // 
  clear_failed_scrobbles_button_box_ -> set_tooltip_text
    ("Clears the tracks in the failed scrobbles database.");

  // 
  reattempt_failed_scrobbles_button_box_ -> set_tooltip_text
    ("Reattempts to scrobble the tracks that failed to scrobble.");

  // 
  failed_scrobbles_count_label_ -> set_tooltip_text
    ("The amount of tracks in the failed scrobbles database.");



  // 
  clear_failed_scrobbles_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &ScrobblingPanel::Clear_Scrobbles_Button_Signal_Clicked));

  // 
  reattempt_failed_scrobbles_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &ScrobblingPanel::Reattempt_Scrobbles_Button_Signal_Clicked));





  //                     //
  // Account Information //////////////////////////////////////////////////////
  //                     //

  // 
  box() . append(*lastfm_frame_, Gtk::PACK_EXPAND_WIDGET);



  // 
  lastfm_frame_ -> add(*lastfm_box_); 



  //  
  lastfm_frame_ -> set_label("last.fm");



  // 
  lastfm_box_ -> append(*lastfm_username_entry_, Gtk::PACK_SHRINK);

  // 
  lastfm_box_ -> set_center_widget(*lastfm_password_entry_);

  // 
  lastfm_box_ -> pack_end(*lastfm_sub_box_, Gtk::PACK_SHRINK);



  // 
  lastfm_box_ -> set_orientation(Gtk::Orientation::VERTICAL);



  // 
  lastfm_box_ -> set_spacing(5);



  // 
  lastfm_box_ -> set_margin_left(5);

  // 
  lastfm_box_ -> set_margin_right(5);

  // 
  lastfm_box_ -> set_margin_top(10);

  // 
  lastfm_box_ -> set_margin_bottom(5);



  // 
  lastfm_sub_box_ -> append(*lastfm_switch_, Gtk::PACK_SHRINK);

  // 
  lastfm_sub_box_ -> pack_end(*lastfm_login_button_, Gtk::PACK_EXPAND_WIDGET);



  //  
  lastfm_sub_box_ -> set_orientation(Gtk::Orientation::HORIZONTAL);



  // 
  lastfm_sub_box_ -> set_spacing(5);



  // 
  lastfm_username_entry_ -> set_text("Username");



  // 
  lastfm_password_entry_ -> set_vexpand(true);

  // 
  lastfm_password_entry_ -> set_text("Password");



  // 
  lastfm_login_button_ -> set_label("Test Login Credentials");

  // 
  lastfm_login_button_ -> set_image(*lastfm_spinner_);

  // 
  lastfm_login_button_ -> set_always_show_image(true);



  // 
  lastfm_username_entry_
    -> set_tooltip_text("Enter your last.fm username here.");

  // 
  lastfm_password_entry_
    -> set_tooltip_text("Enter your last.fm password here.");

  // 
  lastfm_login_button_
    -> set_tooltip_text("Tests whether the login credentials provided " \
                        "are correct.");

  // 
  lastfm_switch_ -> set_tooltip_text("Enables or disables scrobbling.");



  // 
  lastfm_login_button_ -> signal_clicked()
    . connect(sigc::mem_fun
        (*this, &ScrobblingPanel::Lastfm_Login_Button_Signal_Clicked));

  // 
  lastfm_username_entry_ -> signal_changed()
    . connect(sigc::mem_fun
        (*this, &ScrobblingPanel::Lastfm_Username_Entry_Signal_Changed));

  // 
  lastfm_username_entry_ -> signal_focus_in_event()
    . connect(sigc::mem_fun
        (*this, 
         &ScrobblingPanel::Lastfm_Username_Entry_Signal_Focus_In_Event));

  // 
  lastfm_username_entry_ -> signal_focus_out_event()
    . connect(sigc::mem_fun
        (*this,
         &ScrobblingPanel::Lastfm_Username_Entry_Signal_Focus_Out_Event));

  // 
  lastfm_password_entry_ -> signal_changed()
    . connect(sigc::mem_fun
        (*this, &ScrobblingPanel::Lastfm_Password_Entry_Signal_Changed));

  // 
  lastfm_password_entry_ -> signal_focus_in_event()
    . connect(sigc::mem_fun
        (*this,
         &ScrobblingPanel::Lastfm_Password_Entry_Signal_Focus_In_Event));

  // 
  lastfm_password_entry_ -> signal_focus_out_event()
    . connect(sigc::mem_fun
        (*this,
         &ScrobblingPanel::Lastfm_Password_Entry_Signal_Focus_Out_Event));

  // 
  lastfm_switch_ -> property_active() . signal_changed()
    . connect(sigc::mem_fun
        (*this,
         &ScrobblingPanel::Lastfm_Switch_Signal_Changed));





  //       //
  // Other ////////////////////////////////////////////////////////////////////
  //       //

  // 
  new_config_gui
    . Add_Apply_Saved_Value_Function(*this, &Panel::Apply_Saved_Values);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

ScrobblingPanel::~ScrobblingPanel()
{

  // 
  delete scrobble_percent_spinbuttonscale_; 

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//         //
// General ////////////////////////////////////////////////////////////////////
//         //

void ScrobblingPanel::Apply_Saved_Values()
{

  // 
  scrobble_percent_spinbuttonscale_ 
      -> set_value(int(config() . get("scrobbling.percent")));



  // 
  bool lastfm_active = config() . get("scrobbling.lastfm_enabled");

  // 
  lastfm_switch_ -> set_active(lastfm_active);

  // 
  lastfm_username_entry_ -> set_sensitive(lastfm_active);

  // 
  lastfm_password_entry_ -> set_sensitive(lastfm_active);

  // 
  lastfm_login_button_ -> set_sensitive(lastfm_active);



  // 
  string username_str = config() . get("scrobbling.lastfm_username");

  // 
  if((username_str != "")
       ||
     (username_str != "Username"))
  {

    // 
    lastfm_username_entry_ -> set_text(username_str); 

  }



  // 
  if(((lastfm_password_entry_ -> get_text()) != "")
       ||
     ((lastfm_password_entry_ -> get_text()) != "Password"))
  {

    lastfm_password_entry_ -> set_visibility(false);

    lastfm_password_entry_ 
        -> set_text(config().get("scrobbling.lastfm_password"));

  }

}





//                     //
// Account Information ////////////////////////////////////////////////////////
//                     //

void ScrobblingPanel::Lastfm_Login_Button_Signal_Clicked()
{

  // 
  lastfm_login_button_ -> set_label("Logging In...");



  // 
  lastfm_spinner_ -> show();

  // 
  lastfm_spinner_ -> start();



  // 
  lastfm_login_button_ -> set_sensitive(false);



  // 
  shared_ptr<bool> thread_finished(new bool(false));

  // 
  shared_ptr<bool> successful(new bool (false));



  //
  scrobbling() . Track_Action(Scrobbling::Action::LOGIN,
                              thread_finished, successful);



  // 
  sigc::connection program_conn = Glib::signal_timeout() . connect
  (

    // 
    [thread_finished, successful, this]() -> bool
    { 

      // 
      if(!(*thread_finished))
      { 

        // 
        return true;

      }
 


      // 
      this -> lastfm_spinner() . stop();



      // 
      if(*successful)
      {
 
        // 
        this -> lastfm_login_button() . set_label("Login Succeeded!");

      } 

      // 
      else
      {

        // 
        this -> lastfm_login_button() . set_label("Login Failed!");

      } 



      // 
      sigc::connection program_conn = Glib::signal_timeout() . connect
      (

        // 
        [this]() -> bool
        { 
       
          // 
          this -> lastfm_login_button() . set_label("Test Login Credentials");



          //  
          if((this -> lastfm_switch().get_active()))
          {

            // 
            this -> lastfm_login_button().set_sensitive(true);
 
          }



          // 
          return false;
    
        }, 
    


        // 
        1000
    
      );



      // 
      return false;

    },



    // 
    100

  );

}

bool ScrobblingPanel::Lastfm_Password_Entry_Signal_Focus_In_Event
  (GdkEventFocus* event)
{

  // 
  if(config_guis() . disable_functions())
  {

     // 
     return true;
 
  }



  // 
  if(lastfm_password_entry_ -> get_text() == "Password")
  {

    // 
    lastfm_password_entry_ -> set_text("");

  } 



  // 
  return true;

}

bool ScrobblingPanel::Lastfm_Password_Entry_Signal_Focus_Out_Event
  (GdkEventFocus* event)
{

  // 
  if(config_guis().disable_functions())
  {

     // 
     return true;
 
  }  



  // 
  if(((lastfm_password_entry_ -> get_text()) == "") 
       ||
     ((lastfm_password_entry_ -> get_text()) == "Password"))
  {

    // 
    config_guis() . set_disable_functions(true);

    // 
    for(auto config_guis_it : config_guis()()) 
    {

      // 
      config_guis_it -> scrobbling_panel() . lastfm_password_entry()
        . set_text("Password");
  
      config_guis_it -> scrobbling_panel() . lastfm_password_entry() 
        . set_visibility(true);
  
    } 
 
    //  
    config_guis() . set_disable_functions(false);

  }



  // 
  return true;

}

void ScrobblingPanel::Lastfm_Password_Entry_Signal_Changed()
{

  // 
  if(config_guis() . disable_functions())
  { 

    // 
    return;

  }




  // 
  string password_str = lastfm_password_entry_ -> get_text();

  //
  Glib::Checksum temp_checksum;



  // 
  string md5_password_str 
    = temp_checksum . compute_checksum
                        (Glib::Checksum::ChecksumType::CHECKSUM_MD5, 
                         password_str);



  // 
  config() . set("scrobbling.lastfm_password", (md5_password_str));



  // 
  config_guis() . set_disable_functions(true);

  // 
  config_guis() . Mark_Unsaved_Changes(true);

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    if(lastfm_password_entry_ -> get_text() != "Password")
    { 

      config_guis_it -> scrobbling_panel().lastfm_password_entry()
          . set_visibility(false);    
   
    }



    // 
    config_guis_it -> scrobbling_panel() . lastfm_password_entry()
      . set_text(lastfm_password_entry_ -> get_text());    

  }

  // 
  config_guis() . set_disable_functions(false);

}

void ScrobblingPanel::Lastfm_Username_Entry_Signal_Changed()
{

  if(config_guis().disable_functions())
  {
 
     return;
 
  }

  config_guis().set_disable_functions(true);



  config().set("scrobbling.lastfm_username",
               lastfm_username_entry_ -> get_text());
  
  list<ConfigurationGUI*>::iterator config_guis_it; 
  
  // 
  config_guis() . Mark_Unsaved_Changes(true);

  for(config_guis_it = config_guis()().begin();
      config_guis_it != config_guis()().end();
      config_guis_it++)
  {

    (*config_guis_it) -> scrobbling_panel().lastfm_username_entry()
        .set_text(lastfm_username_entry_ -> get_text());
  
  }
  
  config_guis().set_disable_functions(false);



}

bool ScrobblingPanel::Lastfm_Username_Entry_Signal_Focus_In_Event
  (GdkEventFocus* event)
{

  if(config_guis().disable_functions())
  {
 
     return true;
 
  }

  if(lastfm_username_entry_ -> get_text() == "Username")
  {

    lastfm_username_entry_ -> set_text("");

  }

  return true;

}

bool ScrobblingPanel::Lastfm_Username_Entry_Signal_Focus_Out_Event
  (GdkEventFocus* event)
{

  if(config_guis().disable_functions())
  {
 
     return true;
 
  }

  if(lastfm_username_entry_ -> get_text() == "")
  {


    config_guis().set_disable_functions(true);
  
    list<ConfigurationGUI*>::iterator config_guis_it; 
  
    for(config_guis_it = config_guis()().begin();
        config_guis_it != config_guis()().end();
        config_guis_it++)
    {
  
      (*config_guis_it) -> scrobbling_panel().lastfm_username_entry()
          .set_text("Username");
  
    }
  
    config_guis().set_disable_functions(false);

  }

  return true;

}

void ScrobblingPanel::Lastfm_Switch_Signal_Changed()
{

  if(config_guis().disable_functions())
  {

    return;

  }

  config_guis().set_disable_functions(true);

  config().set("scrobbling.lastfm_enabled",
               (lastfm_switch_ -> get_active()));

  list<ConfigurationGUI*>::iterator config_guis_it; 

  // 
  config_guis() . Mark_Unsaved_Changes(true);

  for(config_guis_it = config_guis()().begin();
      config_guis_it != config_guis()().end();
      config_guis_it++)
  {

    (*config_guis_it) -> scrobbling_panel().lastfm_username_entry() 
        .set_sensitive(lastfm_switch_ -> get_active());
    (*config_guis_it) -> scrobbling_panel().lastfm_password_entry() 
        .set_sensitive(lastfm_switch_ -> get_active());
    (*config_guis_it) -> scrobbling_panel().lastfm_login_button() 
        .set_sensitive(lastfm_switch_ -> get_active());
    (*config_guis_it) -> scrobbling_panel().lastfm_switch() 
        .set_active(lastfm_switch_ -> get_active());

  }

  config_guis().set_disable_functions(false);

}




    
//                  //
// Failed Scrobbles ///////////////////////////////////////////////////////////
//                  //

void ScrobblingPanel::Clear_Scrobbles_Button_Signal_Clicked()
{

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> scrobbling_panel() . clear_failed_scrobbles_button()
      . set_sensitive(false);

    // 
    config_guis_it -> scrobbling_panel() . failed_scrobbles_count_label()
      . set_text("0 Failed Scrobbles");

    // 
    config_guis_it -> scrobbling_panel() . reattempt_failed_scrobbles_button()
      . set_sensitive(false);



    // 
    scrobbling() . failed_scrobbles_database() . Clear();

  }

}

void ScrobblingPanel::Reattempt_Scrobbles_Button_Signal_Clicked()
{

  // 
  for(auto config_guis_it : config_guis()())
  {

    // 
    config_guis_it -> scrobbling_panel()
      . clear_failed_scrobbles_button() . set_sensitive(false);

    // 
    config_guis_it -> scrobbling_panel()
      . reattempt_failed_scrobbles_button() . set_sensitive(false);



    // 
    config_guis_it -> scrobbling_panel()
      . reattempt_scrobbles_spinner() . show();

    // 
    config_guis_it -> scrobbling_panel()
      . reattempt_scrobbles_spinner() . start();

  }



  // 
  shared_ptr<bool> thread_finished(new bool(false));



  //
  scrobbling() . Rescrobble_Failed_Scrobbles(thread_finished);



  // 
  sigc::connection program_conn = Glib::signal_timeout() . connect
  (

    // 
    [thread_finished, this]() -> bool
    { 

      // 
      int database_size
        = scrobbling() . failed_scrobbles_database() . database_size();



      // 
      if(!(*thread_finished))
      { 

        // 
        for(auto config_guis_it : config_guis()())
        {
    
          // 
          config_guis_it -> scrobbling_panel() . failed_scrobbles_count_label()
            . set_text(to_string(database_size) + " Failed Scrobbles");

        }



        // 
        return true;

      }

        

      // 
      database_size
        = scrobbling() . failed_scrobbles_database() . database_size();



      // 
      for(auto config_guis_it : config_guis()())
      {
    
        // 
        config_guis_it -> scrobbling_panel() . failed_scrobbles_count_label()
          . set_text(to_string(database_size) + " Failed Scrobbles");



        // 
        config_guis_it -> scrobbling_panel()
          . reattempt_scrobbles_spinner() . stop();


 
        // 
        if(database_size > 0)
        {

          // 
          config_guis_it -> scrobbling_panel()
            . clear_failed_scrobbles_button() . set_sensitive(true);
    
          // 
          config_guis_it -> scrobbling_panel()
            . reattempt_failed_scrobbles_button() . set_sensitive(true);
    
        }

      }



      // 
      return false;

    },



    // 
    100

  );

}





//                  //
// Scrobble Percent ///////////////////////////////////////////////////////////
//                  //

void ScrobblingPanel::Scrobble_Percent_SpinButtonScale_Signal_Changed()
{


  if(config_guis().disable_functions())
  {
 
    return;
 
  }

  config_guis().set_disable_functions(true);

  list<ConfigurationGUI*>::iterator config_guis_it; 


  config().set("scrobbling.percent", 
               int(scrobble_percent_spinbuttonscale_ -> get_value()));




  // 
  config_guis() . Mark_Unsaved_Changes(true);

  for(auto it : config_guis()())
  { 


    it -> scrobbling_panel().scrobble_percent_spinbuttonscale()
      .sbs_spinbutton().set_value(
        scrobble_percent_spinbuttonscale_ -> sbs_spinbutton().get_value());

  }

  config_guis().set_disable_functions(false);


}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

//                     //
// Account Information ////////////////////////////////////////////////////////
//                     //

Gtk::Button& ScrobblingPanel::lastfm_login_button()
{

  return *lastfm_login_button_;

}

Gtk::Entry& ScrobblingPanel::lastfm_password_entry()
{

  return *lastfm_password_entry_;

}

Gtk::Spinner& ScrobblingPanel::lastfm_spinner()
{ 

  return *lastfm_spinner_; 

}

Gtk::Switch& ScrobblingPanel::lastfm_switch()
{

  return *lastfm_switch_;

}

Gtk::Entry& ScrobblingPanel::lastfm_username_entry()
{

  return *lastfm_username_entry_;

}




    
//                  //
// Failed Scrobbles ///////////////////////////////////////////////////////////
//                  //

Gtk::Label& ScrobblingPanel::failed_scrobbles_count_label()
{

  return *failed_scrobbles_count_label_;

}

Gtk::Button& ScrobblingPanel::clear_failed_scrobbles_button()
{

  return *clear_failed_scrobbles_button_;

}

Gtk::Button& ScrobblingPanel::reattempt_failed_scrobbles_button()
{

  return *reattempt_failed_scrobbles_button_;

}

Gtk::Spinner& ScrobblingPanel::reattempt_scrobbles_spinner()
{

  return *reattempt_scrobbles_spinner_;

}





//                  //
// Scrobble Percent ///////////////////////////////////////////////////////////
//                  //

SpinButtonScale& ScrobblingPanel::scrobble_percent_spinbuttonscale()
{ 

  return *scrobble_percent_spinbuttonscale_; 

} 
