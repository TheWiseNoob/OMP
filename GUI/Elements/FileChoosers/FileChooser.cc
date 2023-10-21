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

#include "FileChooser.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../../Errors/Errors.h"

#include "../../../Metadata/Metadata.h"

#include "../../GUI.h"

#include "../ChildWindows/ChildWindow.h"

#include "../Playlists/PlaylistColumnRecord.h"

#include "../Playlists/Playlists.h"

#include "../Playlists/PlaylistsDatabase.h"

#include "../Playlists/PlaylistTreeStore.h"

#include "FileChoosers.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <filesystem>

#include <gdkmm/device.h>

#include <gdkmm/devicemanager.h>

#include <gdkmm/display.h>

#include <gdkmm/window.h>

#include <glibmm/main.h>

#include <glibmm/refptr.h>

#include <gtkmm/box.h>

#include <gtkmm/filechooserwidget.h>

#include <gtkmm/filefilter.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/progressbar.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/scrolledwindow.h>

#include <gtkmm/treeiter.h>

#include <gtkmm/treepath.h>

#include <gtkmm/treerowreference.h>

#include <gtkmm/treeselection.h>

#include <pangomm/fontdescription.h>

#include <iostream>

#include <thread>

#include <vector>





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

FileChooser::FileChooser
  (Base& base_ref, FileChoosers& file_choosers_ref,
   bool appending, bool open_from_command, vector<string>* new_filenames)

// Inherited Class

: GUIElement(base_ref, file_choosers_ref())



// General

, action_box_(Gtk::manage(new Gtk::Box))

, cancel_button_(Gtk::manage(new Gtk::Button("Cancel")))

, close_upon_used_(open_from_command)

, file_chooser_(Gtk::manage(new Gtk::FileChooserWidget))

, okay_button_(new Gtk::Button("Add Selected"))

, progress_bar_(new Gtk::ProgressBar())

, quitting_(false)

, thread_quit_(false)

{

  if(new_filenames != nullptr)
  {

    // 
    filenames_ = *new_filenames;

  }



  // Adds the new FileChooser object to the FileChoosers list.
  file_choosers_ref() . push_front(this);

  // Adds the new FileChooser's to the iterator to it's it storage variable.
  set_gui_elements_it(file_choosers_ref() . begin());



  // 
  box() . set_orientation(Gtk::Orientation::VERTICAL);



  // 
  if(!open_from_command)
  {

    // 
    box() . append(*file_chooser_, Gtk::PACK_EXPAND_WIDGET);

  }

  // 
  box() . append(*progress_bar_, Gtk::PACK_SHRINK);



  // 
  file_chooser_ -> set_extra_widget(*action_box_);



  // 
  file_chooser_ -> set_select_multiple(true);

  // 
  file_chooser_ -> set_create_folders(true);



  // 
  file_chooser_ -> set_action(Gtk::FILE_CHOOSER_ACTION_OPEN);

  // 
  file_chooser_ -> set_use_preview_label(true);



  // 
  string last_folder_str = config() . get("last_folder");

  // 
  if(last_folder_str != "")
  { 

    // 
    file_chooser_ -> set_current_folder(last_folder_str);

  }



  // 
  action_box_ -> set_hexpand(true);



  // 
  action_box_ -> set_orientation(Gtk::Orientation::HORIZONTAL);



  // 
  action_box_ -> append(*okay_button_, true, true, 0);



  // 
  okay_button_ -> signal_clicked()
    . connect(sigc::mem_fun(*this, &FileChooser::Use_Selected));

  // 
  file_chooser_ -> signal_current_folder_changed()
    . connect(sigc::mem_fun
        (*this, &FileChooser::On_Current_Folder_Changed_Signal));

  // 
  file_chooser_ -> signal_file_activated()
    . connect(sigc::mem_fun(*this, &FileChooser::Use_Selected));





  //                 //
  // Filter Creation //////////////////////////////////////////////////////////
  //                 //

  // 
  Glib::RefPtr<Gtk::FileFilter> temp_file_filter;

  // 
  temp_file_filter = Gtk::FileFilter::create();

  // 
  temp_file_filter -> set_name
    ("All Compatible File Types (.ape, .cue, .flac .m2a, .oga, .ogg, .wv)");

  // 
  temp_file_filter -> add_mime_type("audio/x-ape");

  // 
  temp_file_filter -> add_mime_type("application/x-cue");

  // 
  temp_file_filter -> add_mime_type("audio/flac");

  // 
  temp_file_filter -> add_mime_type("audio/mp4");

  // 
  temp_file_filter -> add_mime_type("audio/mpeg");

  // 
  temp_file_filter -> add_mime_type("audio/ogg");

  // 
  temp_file_filter -> add_mime_type("audio/x-wavpack");

  // 
  file_chooser_ -> add_filter(temp_file_filter);



  // 
  temp_file_filter = Gtk::FileFilter::create();

  // 
  temp_file_filter -> set_name(".cue - (CUESHEET)");

  // 
  temp_file_filter -> add_mime_type("application/x-cue");

  // 
  file_chooser_ -> add_filter(temp_file_filter);



  // 
  temp_file_filter = Gtk::FileFilter::create();

  // 
  temp_file_filter -> set_name(".ape - (Monkey's Audio)");

  // 
  temp_file_filter -> add_mime_type("audio/x-ape");

  // 
  file_chooser_ -> add_filter(temp_file_filter);



  // 
  temp_file_filter = Gtk::FileFilter::create();

  // 
  temp_file_filter -> set_name(".flac - (FLAC)");

  // 
  temp_file_filter -> add_mime_type("audio/flac");

  // 
  file_chooser_ -> add_filter(temp_file_filter);



  // 
  temp_file_filter = Gtk::FileFilter::create();

  // 
  temp_file_filter -> set_name(".m4a - (AAC or ALAC)");

  // 
  temp_file_filter -> add_mime_type("audio/mp4");

  // 
  file_chooser_ -> add_filter(temp_file_filter);



  // 
  temp_file_filter = Gtk::FileFilter::create();

  // 
  temp_file_filter -> set_name(".mp3 - (MP3)");

  // 
  temp_file_filter -> add_mime_type("audio/mpeg");

  // 
  file_chooser_ -> add_filter(temp_file_filter);



  // 
  temp_file_filter = Gtk::FileFilter::create();

  // 
  temp_file_filter -> set_name(".ogg | .oga - (Ogg Vorbis)");

  // 
  temp_file_filter -> add_mime_type("audio/ogg");

  // 
  file_chooser_ -> add_filter(temp_file_filter);



  // 
  temp_file_filter = Gtk::FileFilter::create();

  // 
  temp_file_filter -> set_name(".wv - (WavPack)");

  // 
  temp_file_filter -> add_mime_type("audio/x-wavpack");

  // 
  file_chooser_ -> add_filter(temp_file_filter);



  // 
  progress_bar_ -> set_show_text(true);

  // 
  progress_bar_ -> set_fraction(1);

  // 
  progress_bar_ -> set_text("No File Actions Occurring");

  // 
  progress_bar_ -> set_ellipsize(Pango::ELLIPSIZE_END);



  // 
  progress_bar_ -> set_margin_left(3);

  // 
  progress_bar_ -> set_margin_right(3);



  // 
  if(appending)
  {

    // 
    okay_button_ -> set_sensitive(false);

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

FileChooser::~FileChooser()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void FileChooser::Destroy()
{

  // 
  quitting_ = true;

  // 
  quitting_finished() = false;

}

void FileChooser::Enable_Cancel_Button(ChildWindow* child_window_ptr)
{

  // 
  action_box_ -> pack_end(*cancel_button_, true, true, 0);



  // 
  cancel_button_ -> set_margin_left(6);



  // 
  cancel_button_ -> signal_clicked()
    . connect(sigc::bind(sigc::mem_fun
        (*this, &FileChooser::Quit), child_window_ptr));

 }

void FileChooser::On_Current_Folder_Changed_Signal()
{

  // 
  filenames_ . clear();



  // 
  config() . set("last_folder", file_chooser_ -> get_current_folder());



  // 
  config() . write_file();

}

void FileChooser::Quit(ChildWindow* child_window_ref)
{

  // 
  child_window_ref -> On_Irregular_Quit(NULL);

}

void FileChooser::Use_Selected()
{

  // 
  okay_button_ -> set_sensitive(false);



  // 
  static vector<string> selected_filenames;

  // 
  if(filenames_ . empty())
  {

    // 
    selected_filenames = file_chooser_ -> get_filenames();

  }

  // 
  else
  {

    // 
    selected_filenames = filenames_;

  }



  // 
  static string selected_filename;

  // 
  selected_filename . clear();



  // 
  static vector<string> all_filenames;

  all_filenames . clear();



  // 
  static Glib::RefPtr<PlaylistTreeStore> selected_playlist_treestore;

  // 
  selected_playlist_treestore = playlists() . selected_playlist_treestore();



  // 
  int playlist_size;

  // 
  playlist_size
    = selected_playlist_treestore -> children() . size();



  // 
  static atomic<int> new_tracks_size;

  // 
  new_tracks_size = 0;



  // 
  static atomic<bool> getting_all_files;

  // 
  getting_all_files = true;



  // 
  static atomic<bool> appending;

  // 
  appending = false;


  // 
  static string active_filename_str;

  // 
  active_filename_str = "";



  // 
  static mutex active_filename_str_mutex;



  // 
  static atomic<int> reading_files_count;

  // 
  reading_files_count = 0;

  // 
  static atomic<int> reading_files_total;

  // 
  reading_files_total = 0;



  // 
  static atomic<bool> thread_finished = false;

  // 
  thread_finished = false;



  // 
  thread* file_reading_thread(new thread(
  [this, playlist_size]()
  {

    // 
    Gtk::TreeModel::Children selected_playlist_treestore_children
      = selected_playlist_treestore -> children();



    // 
    if(selected_filenames . empty())
    {

      //
      selected_filename = string(config() . get("last_folder"));



      // 
      metadata() . All_Files_In_All_Folders
                     (selected_filename, all_filenames,
                      active_filename_str, active_filename_str_mutex);

    }

    // 
    else
    {

      // 
      selected_filename = selected_filenames[0];



      // 
      for(auto selected_filename_it : selected_filenames)
      {

        // 
        if(quitting_)
        {

          // 
          thread_quit_ = true;



          // 
          return;

        }



        // 
        filesystem::path selected_filename_path = selected_filename_it;



        // 
        if(filesystem::is_directory(selected_filename_path))
        {

          // 
          metadata() . All_Files_In_All_Folders
                         (selected_filename_it, all_filenames,
                          active_filename_str, active_filename_str_mutex);

        }

        // 
        else
        {

          // 
          all_filenames . push_back(selected_filename_it);

        }

      }

    }



    // 
    double id = 1;



    // 
    if(playlist_size > 0)
    {

      // 
      Gtk::TreeRow last_row
        = *(selected_playlist_treestore_children[playlist_size - 1]);

      // 
      id = double(last_row[playlists() . playlist_column_record() . id_]);

    }



    // 
    getting_all_files = false;



    // 
    reading_files_total = all_filenames . size();

    // 
    auto track_ptrs_ptr = metadata() . Filenames_To_Tracks
      (all_filenames, active_filename_str,
       active_filename_str_mutex, reading_files_count, quitting_);



    // 
    for(auto track_ptr : *track_ptrs_ptr)
    { 

      // 
      if(quitting_)
      {

        // 
        delete track_ptr;



        // 
        continue;

      }



      // 
      shared_ptr<Track> track_sptr(track_ptr);

      // 
      id++;



      // 
      selected_playlist_treestore -> add_track_queue()
        . push_back(make_pair(int(id), track_sptr));

    }



    // 
    if(quitting_)
    {

      // 
      thread_quit_ = false;

    }



    // 
    new_tracks_size = track_ptrs_ptr -> size();



    // 
    thread_finished = true;

  }));



  // 
  static double database_saving_pos = 0.00;

  // 
  database_saving_pos = 0.00;

  // 
  static bool database_saving_inverted = false;

  // 
  database_saving_inverted = false;

  // 
  static bool database_saving_decreasing = false;

  // 
  database_saving_decreasing = false;



  // 
  sigc::connection program_conn = Glib::signal_timeout() . connect
  (

    // 
    [this, file_reading_thread]() -> bool
    { 

      // 
      if(quitting_)
      {

        // 
        if(!thread_quit_)
        {

          // 
          return true;

        }

        // 
        else
        {

          // 
          quitting_finished() = true;



          // 
          delete *gui_elements_it();



          // 
          return false;

        }

      }

      // 
      else if(!thread_finished)
      {

        // 
        if(getting_all_files)
        {

          // 
          if(database_saving_pos >= double(1.000))
          {

            //
            database_saving_inverted = !database_saving_inverted;



            // 
            database_saving_decreasing = true;



            // 
            database_saving_pos = 1;

          }

          // 
          else if(database_saving_pos <= double(0.000))
          {

            // 
            database_saving_decreasing = false;



            // 
            database_saving_pos = 0;

          }




          // 
          progress_bar_
            -> set_inverted(database_saving_inverted);



          // 
          active_filename_str_mutex . lock();

          // 
          string status_str = "Finding All Files: " + active_filename_str;

          // 
          active_filename_str_mutex . unlock();



          // 
          progress_bar_ -> set_text(status_str);



          // 
          progress_bar_ -> set_fraction(database_saving_pos);




          // 
          if(database_saving_decreasing)
          {

            // 
            database_saving_pos -= 0.05;

          }

          // 
          else
          {

            // 
            database_saving_pos += 0.05;

          }



          // 
          return true;

        }

        

        // 
        progress_bar_ -> set_inverted(false);



        // 
        double progress_fraction
          = reading_files_count / double(reading_files_total);

        // 
        progress_bar_ -> set_fraction(progress_fraction);



        // 
        active_filename_str_mutex . lock();

        // 
        progress_bar_
          -> set_text("Reading Music Files and Removing Non Music Files: " + active_filename_str);

        // 
        active_filename_str_mutex . unlock();



        // 
        return true;

      }

      // 
      else if(appending)
      {

        // 
        if(selected_playlist_treestore -> appending())
        {

          // 
          return true;

        }



        // 
        okay_button_ -> set_sensitive(true);



        // 
        if(close_upon_used_)
        {

          // 
          Quit(child_window_);

        }



        // 
        return false;

      }



      // 
      appending = true;



      // 
      file_reading_thread -> join();

      // 
      delete file_reading_thread;



      // 
      errors() . Display_Errors();



      // 
      if(new_tracks_size > 0)
      {

        // 
        playlists() . rebuild_databases() = true;

        // 
        selected_playlist_treestore -> rebuild_database() = true;

        // 
        selected_playlist_treestore -> restart_changes() = true;



        // 
        if(!(playlists() . database_extraction_complete())
             && !(selected_playlist_treestore -> extraction_complete()))
        {

          // 
          selected_playlist_treestore -> appending_rows_total()
            += new_tracks_size;

        }

        // 
        else
        {

          // 
          selected_playlist_treestore -> appending_rows_done() = 0;

          // 
          selected_playlist_treestore -> appending_rows_total()
            = int(new_tracks_size);



          // 
          playlists() . Append_Rows(selected_playlist_treestore);

        }

      }



      // 
      progress_bar_ -> set_text("No File Actions Occurring");

      // 
      progress_bar_ -> set_fraction(1);



      // 
      return true;

    },



    // 
    3, Glib::PRIORITY_HIGH_IDLE

  );

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::Button& FileChooser::okay_button()
{

  return *okay_button_;

}





//         //
//         //
// Setters ////////////////////////////////////////////////////////////////////
//         //
//         //

void FileChooser::set_child_window(ChildWindow* new_child_window)
{

  // 
  child_window_ = new_child_window;

}
