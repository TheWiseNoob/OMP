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





#include "ScrobblingPanel.h"



#include "../../../../../Base.h"
#include "../../ConfigurationGUIs.h"
#include "../../../../../Scrobbling/Scrobbling.h"
#include "../../../../../Configuration/Configuration.h"
#include "../../../../SpinButtonScale.h"
#include "../../../../GUI.h"



#include <glibmm/checksum.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/switch.h>
#include <gtkmm/button.h>
#include <gtkmm/spinner.h>
#include <gtkmm/spinbutton.h>
#include <iostream>
#include <glibmm/main.h>



namespace sigc
{

  SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE

}

using namespace std;





ScrobblingPanel::ScrobblingPanel(Base& base_ref, 
                                 ConfigurationGUI& new_config_gui)
: Panel(base_ref, new_config_gui, "Scrobbling")
{

  scrobble_percent_label_ 
    = Gtk::manage(new Gtk::Label("Scrobble Percent:", 0, 0));
  scrobble_percent_spinbuttonscale_ = new SpinButtonScale(10, 95, 0);
 

 
  lastfm_frame_ = Gtk::manage(new Gtk::Frame);
  lastfm_box_ = Gtk::manage(new Gtk::Box);
  lastfm_sub_box_ = Gtk::manage(new Gtk::Box);
  lastfm_username_entry_ = Gtk::manage(new Gtk::Entry);
  lastfm_password_entry_ = Gtk::manage(new Gtk::Entry);
  lastfm_switch_ = Gtk::manage(new Gtk::Switch); 
  lastfm_login_button_ = Gtk::manage(new Gtk::Button);
  lastfm_spinner_ = Gtk::manage(new Gtk::Spinner); 

  scrobble_percent_label_ -> set_margin_left(6);

  box().pack_start(*scrobble_percent_label_, Gtk::PACK_SHRINK);
  box().pack_start(scrobble_percent_spinbuttonscale_ -> box(), 
                   Gtk::PACK_EXPAND_WIDGET);
  box().pack_start(*lastfm_frame_, Gtk::PACK_EXPAND_WIDGET);
 
  lastfm_frame_ -> add(*lastfm_box_); 
 
  lastfm_box_ -> pack_start(*lastfm_username_entry_, Gtk::PACK_SHRINK);
  lastfm_box_ -> set_center_widget(*lastfm_password_entry_);
  lastfm_password_entry_ -> set_vexpand(true);
  lastfm_box_ -> pack_end(*lastfm_sub_box_, Gtk::PACK_SHRINK);
  lastfm_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);
 
  lastfm_sub_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);
  lastfm_sub_box_ -> pack_start(*lastfm_switch_, Gtk::PACK_SHRINK);
  lastfm_sub_box_ -> pack_end(*lastfm_login_button_, Gtk::PACK_EXPAND_WIDGET);
 
 
  
  box().set_spacing(5);
  box().set_margin_left(5);
  box().set_margin_right(5);
  box().set_margin_top(10);
  box().set_margin_bottom(5);
 
  lastfm_frame_ -> set_label("last.fm");
 
  lastfm_box_ -> set_spacing(5);
  lastfm_box_ -> set_margin_left(5);
  lastfm_box_ -> set_margin_right(5);
  lastfm_box_ -> set_margin_top(10);
  lastfm_box_ -> set_margin_bottom(5);


  lastfm_sub_box_ -> set_spacing(5);

  lastfm_username_entry_ -> set_text("Username");

  lastfm_password_entry_ -> set_text("Password");


  lastfm_login_button_ -> set_label("Test Login Credentials");
  lastfm_login_button_ -> set_image(*lastfm_spinner_);
  lastfm_login_button_ -> set_always_show_image(true);



  //
  scrobble_percent_spinbuttonscale_ -> box()
    . set_tooltip_text("Minimum percent of the track that must be played " \
                       "before the track is scrobbled.");

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
  lastfm_switch_
    -> set_tooltip_text("Enables or disables scrobbling.");



  scrobble_percent_spinbuttonscale_ -> sbs_spinbutton().signal_changed()
    .connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Scrobble_Percent_SpinButtonScale_Signal_Changed));
  lastfm_username_entry_ -> signal_focus_in_event()
    .connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Lastfm_Username_Entry_Signal_Focus_In_Event));
  lastfm_username_entry_ -> signal_focus_out_event()
    .connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Lastfm_Username_Entry_Signal_Focus_Out_Event));
  lastfm_username_entry_ -> signal_changed()
    .connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Lastfm_Username_Entry_Signal_Changed));
  lastfm_password_entry_ -> signal_focus_in_event()
    .connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Lastfm_Password_Entry_Signal_Focus_In_Event));
  lastfm_password_entry_ -> signal_focus_out_event()
    .connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Lastfm_Password_Entry_Signal_Focus_Out_Event));
  lastfm_password_entry_ -> signal_changed()
    .connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Lastfm_Password_Entry_Signal_Changed));
  lastfm_switch_ 
    -> property_active().signal_changed().connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Lastfm_Switch_Signal_Changed));
  lastfm_login_button_
    -> signal_clicked().connect(sigc::mem_fun(*this,
        &ScrobblingPanel::Lastfm_Login_Button_Signal_Clicked));

  new_config_gui.Add_Apply_Saved_Value_Function(*this,
                                                &Panel::Apply_Saved_Values);

}





ScrobblingPanel::~ScrobblingPanel()
{

  delete scrobble_percent_spinbuttonscale_; 

}





void ScrobblingPanel::Apply_Saved_Values()
{

  scrobble_percent_spinbuttonscale_ 
      -> set_value(int(config().get("scrobbling.percent")));


  bool lastfm_active = config().get("scrobbling.lastfm_enabled");

  lastfm_switch_ -> set_active(lastfm_active);

  lastfm_username_entry_ -> set_sensitive(lastfm_active);

  lastfm_password_entry_ -> set_sensitive(lastfm_active);

  lastfm_login_button_ -> set_sensitive(lastfm_active);



  if(((config().get("scrobbling.lastfm_username")) != "")
       ||
     ((config().get("scrobbling.lastfm_username")) != "Username"))
  {

    lastfm_username_entry_ 
        -> set_text(config().get("scrobbling.lastfm_username"));

  }
  if(((lastfm_password_entry_ -> get_text()) != "")
       ||
     ((lastfm_password_entry_ -> get_text()) != "Password"))
  {

    lastfm_password_entry_ -> set_visibility(false);

    lastfm_password_entry_ 
        -> set_text(config().get("scrobbling.lastfm_password"));

  }

}

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

bool ScrobblingPanel::Lastfm_Username_Entry_Signal_Focus_In_Event(
       GdkEventFocus* event)
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

bool ScrobblingPanel::Lastfm_Username_Entry_Signal_Focus_Out_Event(
       GdkEventFocus* event)
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
    
void ScrobblingPanel::Lastfm_Login_Button_Signal_Clicked()
{



  lastfm_login_button_ -> set_label("Logging In...");

  lastfm_spinner_ -> show();

  lastfm_spinner_ -> start();

  lastfm_login_button_ -> set_sensitive(false);



  shared_ptr<bool> thread_finished(new bool(false));

  shared_ptr<bool> successful(new bool (false));

  base().scrobbling().Track_Action(Scrobbling::Action::LOGIN,
                                   thread_finished, successful);



  sigc::connection program_conn 
    = Glib::signal_timeout().connect
  (

    [thread_finished, successful, this]() -> bool
    {
   
      if(!(*thread_finished))
      {

        return true;

      }
 

      this -> lastfm_spinner().stop();

      if(*successful)
      {
 
        this -> lastfm_login_button().set_label("Login Succeeded!");

      }
      else
      {

        this -> lastfm_login_button().set_label("Login Failed!");

      }

      sigc::connection program_conn 
        = Glib::signal_timeout().connect
      (
    
        [this]() -> bool
        {
       
    
          this -> lastfm_login_button().set_label("Test Login Credentials");

          if((this -> lastfm_switch().get_active()))
          {        
 
            this -> lastfm_login_button().set_sensitive(true);
 
          }

          return false;
    
        }, 
    
        1000
    
      );

      return false;

    }, 

    100

  );

}





Gtk::Entry &ScrobblingPanel::lastfm_username_entry()
{

  return *lastfm_username_entry_;

}

Gtk::Entry &ScrobblingPanel::lastfm_password_entry()
{

  return *lastfm_password_entry_;

}

Gtk::Switch &ScrobblingPanel::lastfm_switch()
{

  return *lastfm_switch_;

}

Gtk::Button &ScrobblingPanel::lastfm_login_button()
{

  return *lastfm_login_button_;

}
