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

#include "Base.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "Configuration/Configuration.h"

#include "Errors/Errors.h"

#include "GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "GUI/Elements/Playlists/Playlists.h"

#include "GUI/Elements/Playlists/PlaylistTreeStore.h"

#include "GUI/GUI.h"

#include "KeyboardShortcuts/KeyboardShortcuts.h"

#include "Metadata/Metadata.h"

#include "Playback/Playback.h"

#include "Scrobbling/Scrobbling.h"

#include "Metadata/TimeConversion.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

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

// 

: quitting_(false)

{ 

  //                                           //
  // Finds Home Directory and Config Directory ////////////////////////////////
  //                                           //

  // 
  struct passwd* pw = getpwuid(getuid());

  // 
  const char* homedir = pw -> pw_dir;

  // Stores the home directory as and std::string.
  std::string directory_str = homedir;

  // Adds the .omp folder to the folder path to make the full directory path.
  directory_str += "/.omp";

  // Saves the full directory path as a c string.
  char* directory_str_c_str = const_cast<char*>(directory_str . c_str());

  // Copies the .omp directory path to the member variable for storing it.
  strcpy(config_directory_c_str_, directory_str_c_str);
 


  // Used for determining if a folder exists. 
  struct stat st; 

  // True if the .omp directory does not exist yet.
  if(!(stat(directory_str . c_str(), &st) == 0))
  {  

    // 
    mkdir(directory_str . c_str(), S_IWUSR);

  }





  //                          //
  // Opens Main Configuration /////////////////////////////////////////////////
  //                          //

  // Creates a directory string to the main configuration file.
  string config_str = directory_str + "/configuration.conf";



  // Creates the configuration class.
  config_  = new Configuration(argc, argv, config_str);





  //                     //
  // Sets Default Values //////////////////////////////////////////////////////
  //                     //

  // Sets the default value for determining if the default values have changed.
  config() . add_default("default_values", true);

  // 
  config() . add_default("last_folder", "");



  // Cover Art ////////////////////////////////////////////////////////////////

  // 
  list<string> front_names_str_list
    {"Cover.png", "cover.png", "Cover.jpg", "cover.jpg", "Front.png",
     "front.png", "Front.jpg", "front.jpg", "Folder.png", "folder.png",
     "Folder.jpg", "folder.jpg"};

  // 
  config() . add_default("gui.artwork.front_names", front_names_str_list);



  // Configuration GUI ////////////////////////////////////////////////////////

  // 
  config() . add_default("gui.configuration.active_panel", "Playback");



  // GUI General //////////////////////////////////////////////////////////////

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
  config() . add_default("gui.tabs.hide", false);

  //
  config() . add_default("gui.tabs.active", "Main Content");

  // 
  config() . add_default("gui.main_content_paned_position", 420);

  // 
  config() . add_default("gui.double_playlist_paned_position", 500);

  // 
  config() . add_default("gui.file_chooser_playlist_paned_position", 500);



  // Playlists ////////////////////////////////////////////////////////////////

  //
  config() . add_default("gui.playlist.empty_space_row_deselects", false);

  //
  config() . add_default("gui.playlist.view.main_content.locked", false);



  // 
  list<string> view_names
    {"main_content", "full", "double_left", "double_right", "file_chooser"};

  // 
  list<string> column_names
    {"track_number", "title", "artists", "album_artists", "album", "genres",
     "length", "date", "track_total", "bit_rate", "bit_depth", "sample_rate",
     "channels", "codec", "mime"};



  // 
  for(auto column_names_it : column_names)
  {

    string column_name_config_str
      = "gui.playlist.columns." + column_names_it + ".title";



    // 
    if(column_names_it == "album")
    {

      // 
      config() . add_default(column_name_config_str, "Album");

    }

    // 
    else if(column_names_it == "album_artists")
    {

      // 
      config() . add_default(column_name_config_str, "Albums Artist(s)");

    }

    // 
    else if(column_names_it == "artists")
    {

      // 
      config() . add_default(column_name_config_str, "Artist(s)");

    }

    // 
    else if(column_names_it == "bit_depth")
    {

      // 
      config() . add_default(column_name_config_str, "Bit Depth");

    }

    // 
    else if(column_names_it == "bit_rate")
    {

      // 
      config() . add_default(column_name_config_str, "Bitrate");

    }

    // 
    else if(column_names_it == "channels")
    {

      // 
      config() . add_default(column_name_config_str, "Channels");

    }

    // 
    else if(column_names_it == "codec")
    {

      // 
      config() . add_default(column_name_config_str, "Codec");

    }

    // 
    else if(column_names_it == "date")
    {

      // 
      config() . add_default(column_name_config_str, "Date");

    }

    // 
    else if(column_names_it == "genres")
    {

      // 
      config() . add_default(column_name_config_str, "Genre(s)");

    }

    // 
    else if(column_names_it == "length")
    {

      // 
      config() . add_default(column_name_config_str, "Length");

    }

    // 
    else if(column_names_it == "mime")
    {

      // 
      config() . add_default(column_name_config_str, "Mime");

    }

    // 
    else if(column_names_it == "sample_rate")
    {

      // 
      config() . add_default(column_name_config_str, "Sample Rate");

    }

    // 
    else if(column_names_it == "title")
    {

      // 
      config() . add_default(column_name_config_str, "Title");

    }

    // 
    else if(column_names_it == "track_number")
    {

      // 
      config() . add_default(column_name_config_str, "#");

    }

    // 
    else if(column_names_it == "track_total")
    {

      // 
      config() . add_default(column_name_config_str, "Track Total");

    }

  }



  // 
  for(auto view_names_it : view_names)
  {

    // 
    string config_str = "gui.playlist.view.";

    // 
    config_str += view_names_it;
    


    // 
    string playlist_view_config_str = config_str + ".locked";

    // 
    config() . add_default(playlist_view_config_str, false);



    // 
    playlist_view_config_str = config_str + ".active";

    // 
    config() . add_default(playlist_view_config_str, "Library");



    // 
    playlist_view_config_str = config_str + ".column_order";

    // 
    config() . add_default(playlist_view_config_str, column_names);



    // 
    for(auto column_names_it : column_names)
    {

      // 
      string column_position_config_str
        = config_str + ".columns." + column_names_it + ".size";



      // 
      config() . add_default(column_position_config_str, 80);

    }

  }



  //
  config() . add_default("gui.playlist.empty_space_playlist_deselects", false);

  //
  config() . add_default("gui.playlist_combobox.active", "Library");



  // Keyboard Shortcuts ///////////////////////////////////////////////////////

  // 
  config()
    . add_default("keyboard_shortcuts.keys.add_files", "<Primary>o");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.close_secondary", "Escape");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.configuration", "<Primary>j");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.copy_rows", "<Primary>c");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.create_playlist", "<Primary>n");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.cut_rows", "<Primary>x");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.delete_playlist", "<Shift>Delete");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.delete_rows", "Delete");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.fullscreen", "F11");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.next_track", "n");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.paste_rows", "<Primary>v");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.pause", "p");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.play", "s");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.select_all_rows", "<Primary>a");

  // 
  config()
    . add_default("keyboard_shortcuts.keys.system_menu", "<Primary>m");

  // 
  config()  . add_default("keyboard_shortcuts.keys.stop", "q");



  // Playback /////////////////////////////////////////////////////////////////

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
  config() . add_default("playback.selected_playlist_only", true);

  //
  config() . add_default("playback.start_at_pregap", true);



  // Output ///////////////////////////////////////////////////////////////////

  //
  config() . add_default("output.buffer_time", 30000);

  // 
  config() . add_default("output.sink", string("autoaudiosink"));



  // ReplayGain ///////////////////////////////////////////////////////////////

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



  // Scrobbling ///////////////////////////////////////////////////////////////

  // 
  config() . add_default("scrobbling.lastfm_enabled", false);

  // 
  config() . add_default("scrobbling.lastfm_username", "Username");

  // 
  config() . add_default("scrobbling.lastfm_password", "Password");

  // 
  config() . add_default("scrobbling.percent", 51);





  //                     //
  // Main Parts Creation //////////////////////////////////////////////////////
  //                     //

  // 
  errors_ = new Errors(*this);

  // 
  time_converter_ = new TimeConversion;

  // 
  metadata_ = new Metadata(*this);

  // 
  playback_ = (new Playback(*this, argc, argv));

  // 
  scrobbling_ = (new Scrobbling(*this));

  // 
  keyboard_shortcuts_ = new KeyboardShortcuts(*this);

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

  quitting_ = true;

  delete gui_;

  delete keyboard_shortcuts_;

  delete playback_;

  // delete scrobbling_;

  delete metadata_;

  delete config_;

  delete errors_;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Configuration& Base::config()
{

  return *config_;

}

const char* Base::config_directory_c_str()
{

  return config_directory_c_str_;

}

Errors& Base::errors()
{

  return *errors_;

}

GUI& Base::gui()
{

  return *gui_;

}

KeyboardShortcuts& Base::keyboard_shortcuts()
{

  return *keyboard_shortcuts_;

}

Metadata& Base::metadata()
{ 

  return *metadata_; 

}

Playback& Base::playback()
{

  return *playback_;

}

atomic<bool>& Base::quitting()
{

  return quitting_;

}

Scrobbling& Base::scrobbling()
{ 

  return *scrobbling_; 

}

TimeConversion& Base::time_converter()
{ 

  return *time_converter_; 

}
