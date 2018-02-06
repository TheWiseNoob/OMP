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

#include "Base.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "Configuration/Configuration.h"

#include "GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "GUI/Elements/Playlists/Playlists.h"

#include "GUI/Elements/Playlists/PlaylistTreeStore.h"

#include "GUI/GUI.h"

#include "Metadata/Metadata.h"

#include "Playback/Playback.h"

#include "Scrobbling/Scrobbling.h"

#include "TimeConversion.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <pwd.h>

#include <string>

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>





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

Base::Base(int argc, char *argv[])
{

  // 
  struct passwd* pw = getpwuid(getuid());

  // 
  const char* homedir = pw -> pw_dir;

  // 
  std::string directory_str = homedir;

  // 
  directory_str += "/.omp";



  // 
  struct stat st; 

  // 
  if(!(stat(directory_str . c_str(), &st) == 0))
  {

    // 
    mkdir(directory_str . c_str(), S_IWUSR);

  }



  // 
  string config_str = directory_str + "/configuration.conf";



  //
  config_  = new Configuration(argc, argv, config_str);



  //
  config() . add_default("default_values", true);

  // 
  config() . add_default("last_folder", "");



  // 
  list<string> front_names_str_list
    {"Cover.png", "cover.png", "Cover.jpg", "cover.jpg", "Front.png",
     "front.png", "Front.jpg", "front.jpg", "Folder.png", "folder.png",
     "Folder.jpg", "folder.jpg"};

  // 
  config() . add_default("gui.artwork.front_names", front_names_str_list);



  // 
  config() . add_default("gui.window_maximized", false);

  // 
  config() . add_default("gui.window_size_x", 1200);

  // 
  config() . add_default("gui.window_size_y", 700);

  //
  config() . add_default("gui.hide_duplicates", false);

  //
  config() . add_default("gui.hide_header_bar", false);

  //
  config() . add_default("gui.hide_status_bar", false);

  // 
  config() . add_default("gui.main_content_paned_position", 420);

  // 
  config() . add_default("gui.double_playlist_paned_position", 500);

  // 
  config() . add_default("gui.file_chooser_playlist_paned_position", 500);

  //
  config() . add_default("gui.playlist.empty_space_row_deselects", false);

  //
  config() . add_default("gui.playlist.view.main_content.locked", false);



  // 
  list<string> view_names
    {"main_content", "full", "double_left", "double_right", "file_chooser"};

  for(auto view_names_it : view_names)
  {

    // 
    string config_str = "gui.playlist.view";

    // 
    config_str += view_names_it;
    


    // 
    string playlist_view_config_str = view_names_it + ".locked";

    // 
    config() . add_default(playlist_view_config_str, false);



    // 
    playlist_view_config_str = view_names_it + ".active";

    // 
    config() . add_default(playlist_view_config_str, "Library");



    // 
    list<string> column_names
      {"track_number", "title", "artists", "album_artists", "album", "genre",
       "length", "date", "track_total", "bit_rate", "bit_depth", "sample_rate",
       "channels", "codec", "mime"};

  }



  //
  config() . add_default("gui.playlist.empty_space_playlist_deselects", false);

  //
  config() . add_default("gui.playlist_combobox.active", "Library");



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
  config() . add_default("playback.volume", 1.000);

  //
  config() . add_default("playback.start_at_pregap", true);



  //
  config() . add_default("output.buffer_time", 30000);

  // 
  config() . add_default("output.sink", string("autoaudiosink"));



  // 
  config() . add_default("replaygain.limiter", true);

  // 
  config() . add_default("replaygain.enabled", true);

  // 
  config() . add_default("replaygain.pre_amp", -3.000);

  // 
  config() . add_default("replaygain.fallback_gain", -3.000);

  //
  config() . add_default("replaygain.gain_type", string("album"));



  // 
  config() . add_default("scrobbling.lastfm_enabled", false);

  // 
  config() . add_default("scrobbling.lastfm_username", "Username");

  // 
  config() . add_default("scrobbling.lastfm_password", "Password");

  // 
  config() . add_default("scrobbling.percent", 51);





  // 
  time_converter_ = new TimeConversion;

  // 
  metadata_ = new Metadata(*this);

  // 
  playback_ = (new Playback(*this, argc, argv));

  // 
  scrobbling_ = (new Scrobbling(*this));

  // 
  gui_ = (new GUI(*this));



  // 
  gui_ -> config_guis() . Apply_Saved_Values();

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Base::~Base()
{

  delete playback_;

  delete scrobbling_;

  delete metadata_;

  delete config_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

Configuration& Base::config()
{

  return *config_;

}

GUI& Base::gui()
{

  return *gui_;

}

Metadata& Base::metadata()
{ 

  return *metadata_; 

}

Playback& Base::playback()
{

  return *playback_;

}

Scrobbling& Base::scrobbling()
{ 

  return *scrobbling_; 

}

TimeConversion& Base::time_converter()
{ 

  return *time_converter_; 

}
