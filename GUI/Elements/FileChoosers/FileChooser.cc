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

#include "../../../Metadata/Metadata.h"

#include "../ChildWindows/ChildWindow.h"

#include "../Playlists/Playlists.h"

#include "../Playlists/PlaylistTreeStore.h"

#include "FileChoosers.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <iostream>

#include <gdkmm/device.h>

#include <gdkmm/devicemanager.h>

#include <gdkmm/display.h>

#include <gdkmm/window.h>

#include <glibmm/refptr.h>

#include <gtkmm/box.h>

#include <gtkmm/filechooserwidget.h>

#include <gtkmm/filefilter.h>

#include <gtkmm/frame.h>

#include <gtkmm/label.h>

#include <gtkmm/radiomenuitem.h>

#include <gtkmm/scrolledwindow.h>

#include <gtkmm/treepath.h>

#include <gtkmm/treerowreference.h>

#include <gtkmm/treeselection.h>

#include <pangomm/fontdescription.h>

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

FileChooser::FileChooser(Base& base_ref, FileChoosers& file_choosers_ref)

// Inherited Class

: GUIElement(base_ref, file_choosers_ref())



// General

, action_box_(new Gtk::Box)

, cancel_button_(new Gtk::Button("Cancel"))

, file_chooser_(new Gtk::FileChooserWidget)

, file_mode_radio_button_(Gtk::manage(new Gtk::RadioButton("File Mode")))

, folder_mode_radio_button_(Gtk::manage(new Gtk::RadioButton("Folder Mode")))

, mode_box_(Gtk::manage(new Gtk::Box))

, okay_button_(new Gtk::Button("Add File(s)"))

{

  // Adds the new FileChooser object to the FileChoosers list.
  file_choosers_ref() . push_front(this);

  // Adds the new FileChooser's to the iterator to it's it storage variable.
  set_gui_elements_it(file_choosers_ref() . begin());



  // 
  box() . set_orientation(Gtk::ORIENTATION_VERTICAL);

  // 
  box() . pack_start(*file_chooser_, Gtk::PACK_EXPAND_WIDGET);



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
  action_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);



  // 
//  action_box_ -> pack_start(*mode_box_, true, true, 0);

  // 
  action_box_ -> pack_start(*okay_button_, true, true, 0);



  // 
  mode_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);



  // 
  mode_box_ -> pack_start(*file_mode_radio_button_, true, true, 0);

  // 
  mode_box_ -> pack_start(*folder_mode_radio_button_, true, true, 0);



  // 
  file_mode_radio_button_ -> join_group(*folder_mode_radio_button_);



  // 
  file_mode_radio_button_ -> set_active(true);



  // 
  folder_mode_radio_button_ -> set_margin_right(3);



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

  // 
  file_mode_radio_button_ -> signal_toggled()
    . connect(sigc::mem_fun(*this, &FileChooser::Change_Mode));

  // 
  folder_mode_radio_button_ -> signal_toggled()
    . connect(sigc::mem_fun(*this, &FileChooser::Change_Mode));





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

void FileChooser::Change_Mode()
{

  // 
  if(folder_mode_radio_button_ -> get_active())
  {

    // 
    okay_button_ -> set_label("Add Folder");

  }

  // 
  else
  {

    // 
    okay_button_ -> set_label("Add File(s)");

  }

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
  vector<string> filenames = file_chooser_ -> get_filenames();



  // 
  for(auto filenames_it : filenames)
  {

//    cout << "\n\nFilename: " << filenames_it << "\n\n";

  }



  // 
  playlists() . Append_Rows(metadata() . Filenames_To_Tracks(filenames), 
                            playlists() . selected_playlist_treestore());

}
