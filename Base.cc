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





#include "Base.h"

#include "Configuration/Configuration.h"
#include "GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h"
#include "GUI/GUI.h"
#include "Playback/Playback.h"
#include "Metadata/Metadata.h"
#include "Scrobbling/Scrobbling.h"
#include "TimeConversion.h"

#include <string>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>


Base::Base(int argc, char *argv[])
{

//  bool arg_used[argc] = {false};

  struct passwd *pw = getpwuid(getuid());

  const char *homedir = pw->pw_dir;

  std::string directory_str = homedir;

  directory_str += "/.omp";


  // 
  struct stat st; 



  // 
  if(!(stat(directory_str . c_str(),&st) == 0))
  {

    string mkdir_str = "mkdir " + directory_str;

    system(mkdir_str . c_str());

  }



  string config_str = directory_str + "/configuration.conf";

  //
  config_
    = new Configuration(argc, argv, config_str);



//  config().add_default("general.debug_level", 0);


  list<string> front_names_str_list;

  front_names_str_list . push_back(string("Cover.png"));

  front_names_str_list . push_back(string("cover.png"));

  front_names_str_list . push_back(string("Cover.jpg"));

  front_names_str_list . push_back(string("cover.jpg"));

  front_names_str_list . push_back(string("Front.png"));

  front_names_str_list . push_back(string("front.png"));

  front_names_str_list . push_back(string("Front.jpg"));

  front_names_str_list . push_back(string("front.jpg"));

  front_names_str_list . push_back(string("Folder.png"));

  front_names_str_list . push_back(string("folder.png"));

  front_names_str_list . push_back(string("Folder.jpg"));

  front_names_str_list . push_back(string("folder.jpg"));

  config() . add_default("gui.artwork.front_names", front_names_str_list);




  //
  config() . add_default("default_values", true);

  // 
  config() . add_default("last_folder", "");

  //
  config() . add_default("gui.hide_duplicates", false);

  //
  config() . add_default("gui.hide_header_bar", false);

  //
  config() . add_default("gui.hide_status_bar", false);

  //
  config() . add_default("gui.playlist.empty_space_row_deselects", false);

  //
  config() . add_default("gui.playlist.empty_space_playlist_deselects", false);

  //
  config() . add_default("playback.cursor_follows_playback", true);

  //
  config() . add_default("playback.playback_follows_cursor", false);

  //
  config() . add_default("playback.looping", "none");

  //
  config() . add_default("playback.order", "normal");

  //
  config() . add_default("playback.queue", true);

  //
  config() . add_default("playback.queue_saved", true);

  //
  config() . add_default("playback.buffer_time", 60);

  //
  config() . add_default("playback.volume", 100.00);

  //
  config() . add_default("playback.start_at_pregap", true);

  //
  config() . add_default("output.buffer_time", 30000);


  config() . add_default("output.sink", string("autoaudiosink"));


  config() . add_default("scrobbling.lastfm_enabled", false);


  config() . add_default("scrobbling.lastfm_username", "Username");


  config() . add_default("scrobbling.lastfm_password", "Password");


  config() . add_default("scrobbling.percent", 51);


  config() . add_default("replaygain.limiter", true);


  config() . add_default("replaygain.enabled", true);


  config() . add_default("replaygain.pre_amp", -3.000);


  config() . add_default("replaygain.fallback_gain", -3.000);

  //
  config() . add_default("replaygain.gain_type", string("album"));

//  config().add_default("playback.minimum_prepared_track_time", 60);

  quitting_ = false;

  time_converter_ = new TimeConversion;



  metadata_ = (new Metadata(*this));

  playback_ = (new Playback(*this, argc, argv));



  scrobbling_ = (new Scrobbling(*this));

  gui_ = (new GUI(*this));




  gui_ -> config_guis().Apply_Saved_Values();


}





Base::~Base()
{


  delete scrobbling_;

  delete metadata_;

  delete config_;

}





GUI& Base::gui()
{

  return *gui_;

}





Playback& Base::playback()
{

  return *playback_;

}





Configuration& Base::config()
{

  return *config_;

}





void Base::Quit()
{

  delete playback_;

}
