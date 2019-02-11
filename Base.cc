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

#include "GUI/Elements/ChildWindows/ChildWindow.h"

#include "GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "GUI/Elements/FileChoosers/FileChoosers.h"

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

#include <giomm/applicationcommandline.h>

#include <glibmm.h>

#include <gtkmm/applicationwindow.h>

#include <gtkmm/filechooserwidget.h>

#include <gtkmm/messagedialog.h>

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

Base::Base(int argc, char *argv[],
           Glib::RefPtr<Gtk::Application> new_application)

// 

: application_(new_application)

, quitting_(false)

{

  //                                           //
  // Finds Home Directory and Config Directory ////////////////////////////////
  //                                           //

  // Stores user account information.
  struct passwd* uai = getpwuid(getuid());

  // Stores the home directory.
  const char* homedir = uai -> pw_dir;

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

    // Makes the .omp directory if it does not exist.
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

  // Sets default value.
  config() . add_default("default_values", true);

  // Sets default value.
  config() . add_default("last_folder", "");



  // Cover Art ////////////////////////////////////////////////////////////////

  // Stores all of the default front cover art names.
  list<string> front_names_str_list
    {"Cover.png", "cover.png", "Cover.jpg", "cover.jpg", "Front.png",
     "front.png", "Front.jpg", "front.jpg", "Folder.png", "folder.png",
     "Folder.jpg", "folder.jpg"};

  // Sets the default values for the front cover art.
  config() . add_default("gui.artwork.front_names", front_names_str_list);



  // Configuration GUI ////////////////////////////////////////////////////////

  // Sets default value.
  config() . add_default("gui.configuration.active_panel", "Playback");



  // GUI General //////////////////////////////////////////////////////////////

  // Sets default value.
  config() . add_default("gui.window_maximized", false);

  // Sets default value.
  config() . add_default("gui.window_size_x", 1200);

  // Sets default value.
  config() . add_default("gui.window_size_y", 700);

  // Sets default value.
  config() . add_default("gui.hide_duplicates", false);

  // Sets default value.
  config() . add_default("gui.hide_header_bar", false);

  // Sets default value.
  config() . add_default("gui.hide_status_bar", false);

  // Sets default value.
  config() . add_default("gui.tabs.hide", false);

  // Sets default value.
  config() . add_default("gui.tabs.active", "Main Content");

  // Sets default value.
  config() . add_default("gui.main_content_paned_position", 420);

  // Sets default value.
  config() . add_default("gui.double_playlist_paned_position", 500);

  // Sets default value.
  config() . add_default("gui.file_chooser_playlist_paned_position", 500);



  // Metadata /////////////////////////////////////////////////////////////////

  // Sets default value.
  config() . add_default("metadata.cuesheet_type_preference", "embedded");

  // Sets default value.
  config() . add_default("metadata.files_or_cuesheet", "cuesheet");

  // Sets default value.
  config() . add_default("metadata.guess_metadata", "true");



  // Playlists ////////////////////////////////////////////////////////////////

  // Sets default value.
  config() . add_default("gui.playlist.empty_space_row_deselects", false);

  // Sets default value.
  config() . add_default("gui.playlist.view.main_content.locked", false);



  // 
  list<string> view_names
    {"main_content", "full", "double_left", "double_right", "file_chooser"};

  // 
  list<string> column_names
    {"album", "album_artists", "artists", "bit_rate", "bit_depth", "channels",
     "codec", "date", "disc_number", "disc_total", "genres", "mime", "length",
     "sample_rate", "title", "track_number", "track_total"};

  // 
  list<string> column_order_names
    {"track_number", "track_total", "disc_number", "disc_total", "title",
     "artists", "album_artists", "album", "date", "length", "genres",
     "bit_rate", "codec", "bit_depth", "channels", "mime", "sample_rate"};



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
      config() . add_default(column_name_config_str, "Album Artist(s)");

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
    else if(column_names_it == "disc_number")
    {

      // 
      config() . add_default(column_name_config_str, "Disc Number");

    }

    // 
    else if(column_names_it == "disc_total")
    { 

      // 
      config() . add_default(column_name_config_str, "Disc Total");

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
    config() . add_default(playlist_view_config_str, column_order_names);



    // 
    for(auto column_name : column_names)
    {

      // 
      string column_position_config_str
        = config_str + ".columns." + column_name + ".size";



      // 
      if(column_name == "album")
      {

        // 
        config() . add_default(column_position_config_str, 167);

      }

      // 
      else if(column_name == "album_artists")
      {

        // 
        config() . add_default(column_position_config_str, 122);

      }

      // 
      else if(column_name == "artists")
      {

        // 
        config() . add_default(column_position_config_str, 137);

      }

      // 
      else if(column_name == "bit_rate")
      {

        // 
        config() . add_default(column_position_config_str, 49);

      }

      // 
      else if(column_name == "bit_depth")
      {

        // 
        config() . add_default(column_position_config_str, 32);

      }

      // 
      else if(column_name == "channels")
      {

        // 
        config() . add_default(column_position_config_str, 25);

      }

      // 
      else if(column_name == "codec")
      {

        // 
        config() . add_default(column_position_config_str, 55);

      }

      // 
      else if(column_name == "date")
      {

        // 
        config() . add_default(column_position_config_str, 46);

      }

      // 
      else if(column_name == "disc_number")
      {

        // 
        config() . add_default(column_position_config_str, 25);

      }

      // 
      else if(column_name == "disc_total")
      {

        // 
        config() . add_default(column_position_config_str, 25);

      }

      // 
      else if(column_name == "genres")
      {

        // 
        config() . add_default(column_position_config_str, 438);

      }

      // 
      else if(column_name == "length")
      {

        // 
        config() . add_default(column_position_config_str, 80);

      }

      // 
      else if(column_name == "mime")
      {

        // 
        config() . add_default(column_position_config_str, 80);

      }

      // 
      else if(column_name == "sample_rate")
      {

        // 
        config() . add_default(column_position_config_str, 58);

      }

      // 
      else if(column_name == "title")
      {

        // 
        config() . add_default(column_position_config_str, 284);

      }

      // 
      else if(column_name == "track_number")
      {

        // 
        config() . add_default(column_position_config_str, 42);

      }

      // 
      else if(column_name == "track_total")
      {

        // 
        config() . add_default(column_position_config_str, 25);

      }

    }

  }



  // Sets default value.
  config() . add_default("gui.playlist.empty_space_playlist_deselects", false);

  // Sets default value.
  config() . add_default("gui.playlist_combobox.active", "Library");



  // Keyboard Shortcuts ///////////////////////////////////////////////////////

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.add_files", "<Primary>o");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.close_secondary", "Escape");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.configuration", "<Primary>j");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.copy_rows", "<Primary>c");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.create_playlist", "<Primary>n");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.cut_rows", "<Primary>x");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.delete_playlist", "<Shift>Delete");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.delete_rows", "Delete");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.edit_row", "e");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.fullscreen", "F11");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.next_track", "n");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.paste_rows", "<Primary>v");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.pause", "p");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.play", "s");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.rename_playlist", "<Primary>r");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.select_all_rows", "<Primary>a");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.stop_after_current", "<Shift>s");

  // Sets default value.
  config()
    . add_default("keyboard_shortcuts.keys.system_menu", "<Primary>m");

  // Sets default value.
  config()  . add_default("keyboard_shortcuts.keys.stop", "q");



  // Playback /////////////////////////////////////////////////////////////////

  // Sets default value.
  config() . add_default("playback.cursor_follows_playback", true);

  // Sets default value.
  config() . add_default("playback.playback_follows_cursor", false);

  // Sets default value.
  config() . add_default("playback.looping", "none");

  // Sets default value.
  config() . add_default("playback.order", "normal");

  // Sets default value.
  config() . add_default("playback.queue", true);

  // Sets default value.
  config() . add_default("playback.queue_saved", true);

  // Sets default value.
  config() . add_default("playback.buffer_time", 60);

  // Sets default value.
  config() . add_default("playback.volume", 1.000);

  // Sets default value.
  config() . add_default("playback.selected_playlist_only", true);

  // Sets default value.
  config() . add_default("playback.start_at_pregap", true);



  // Output ///////////////////////////////////////////////////////////////////

  // Sets default value.
  config() . add_default("output.buffer_time", 30000);

  // Sets default value.
  config() . add_default("output.sink", string("autoaudiosink"));



  // ReplayGain ///////////////////////////////////////////////////////////////

  // Sets default value.
  config() . add_default("replaygain.limiter", true);

  // Sets default value.
  config() . add_default("replaygain.enabled", true);

  // Sets default value.
  config() . add_default("replaygain.pre_amp", -3.000);

  // Sets default value.
  config() . add_default("replaygain.fallback_gain", -3.000);

  // Sets default value.
  config() . add_default("replaygain.gain_type", string("album"));



  // Scrobbling ///////////////////////////////////////////////////////////////

  // Sets default value.
  config() . add_default("scrobbling.lastfm_enabled", false);

  // Sets default value.
  config() . add_default("scrobbling.lastfm_username", "Username");

  // Sets default value.
  config() . add_default("scrobbling.lastfm_password", "Password");

  // Sets default value.
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





//                  //
//                  //
// Member Functions /////////////////////////////////////////////////////////
//                  //
//                  //

int Base::New_Command
  (const Glib::RefPtr<Gio::ApplicationCommandLine>& commands)
{

  // 
  static mutex command_mutex;

  // 
  while(!(command_mutex . try_lock()));



  // 
  static mutex main_command_mutex;

  // 
  bool main_command = false;

  // 
  static vector<string> filenames;



  // 
  static chrono::time_point<chrono::system_clock> last_command_time;

  // 
  static chrono::time_point<chrono::system_clock> new_time;



  // 
  if(main_command_mutex . try_lock())
  {

    // 
    main_command = true;

  }



  // 
  last_command_time = chrono::system_clock::now();



  // 
  int argc = 0;

  // 
  char** argv = commands -> get_arguments(argc);



  // 
  for(int i = 1; i < argc; i++)
  {

    // 
    filenames . push_back(string(argv[i]));

  }



  // 
  if(main_command)
  {

    // 
    sigc::connection program_conn = Glib::signal_timeout() . connect
    (

      // 
      [this]() -> bool
      { 

        // 
        chrono::duration<double> last_command_elapsed_seconds;



        // 
        new_time = chrono::system_clock::now();



        // 
        last_command_elapsed_seconds = new_time - last_command_time;



        // 
        if((last_command_elapsed_seconds . count()) < 0.1)
        {

          // 
          return true;

        }



        // 
        if(!(filenames . empty()))
        {

          // 
          sort(filenames . begin(), filenames . end());



          // 
          auto file_chooser = gui_ -> file_choosers() . Create(true, &filenames);

          // 
          file_chooser -> Use_Selected();



          // 
          filenames . clear();



          // 
          gui() . main_window() -> window() . grab_focus();

          // 
          gui() . main_window() -> window() . present();

        }

 

        // 
        main_command_mutex . unlock();



        // 
        return false;

      },



      // 
      3, Glib::PRIORITY_HIGH_IDLE

    );

  }



  // 
  application_ -> activate();



  // 
  command_mutex . unlock();



  // 
  return EXIT_SUCCESS;

}

void Base::OMP_Started()
{

  // 
//  application_ -> activate();



  // 
 // auto new_file_chooser = gui_ -> file_choosers() . Create(true);

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
