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





#include "FileChooser.h"





#include "FileChoosers.h"

#include "../../../Configuration/Configuration.h"
#include "../../../Base.h"
#include "../../../Metadata/Metadata.h"
#include "../Playlists/Playlists.h"
#include "../Playlists/PlaylistTreeStore.h"



#include <gtkmm/filechooserwidget.h>
#include <gtkmm/filefilter.h>

#include <glibmm/refptr.h>

#include <iostream>
#include <vector>



using namespace std;




FileChooser::FileChooser(Base& base, 
                         FileChoosers& file_choosers_ref)
: GUIElement(base, file_choosers_ref())
, file_chooser_(new Gtk::FileChooserWidget)
, action_box_(new Gtk::Box)
{

  // Adds the new FileChooser object to the FileChoosers list.
  file_choosers_ref() . push_front(this);

  // Adds the new FileChooser's to the iterator to it's it storage variable.
  set_gui_elements_it(file_choosers_ref() . begin());



  box().set_orientation(Gtk::ORIENTATION_VERTICAL);

  box().pack_start(*file_chooser_, Gtk::PACK_EXPAND_WIDGET);

  box().pack_end(*action_box_, Gtk::PACK_SHRINK);

  file_chooser_ -> set_select_multiple(true);

  file_chooser_ -> set_create_folders(true);

  file_chooser_ -> set_action(Gtk::FILE_CHOOSER_ACTION_OPEN);

  file_chooser_ -> set_use_preview_label(true);

  file_chooser_ -> signal_file_activated()
    .connect(sigc::mem_fun(*this, &FileChooser::Use_Selected));



  //Filter Creation
  //  
  Glib::RefPtr<Gtk::FileFilter> temp_file_filter;


  temp_file_filter = Gtk::FileFilter::create();

  temp_file_filter
    -> set_name("All Compatible File Types " 
                "(.cue, .ape, .flac .m4a, .oga, .ogg, .wv)");

  temp_file_filter -> add_mime_type("application/x-cue");

  temp_file_filter -> add_mime_type("audio/x-ape");

  temp_file_filter -> add_mime_type("audio/flac");

  temp_file_filter -> add_mime_type("audio/mp4");

  temp_file_filter -> add_mime_type("audio/mpeg");

  temp_file_filter -> add_mime_type("audio/ogg");

  temp_file_filter -> add_mime_type("audio/x-wavpack");

  file_chooser_ -> add_filter(temp_file_filter);


  temp_file_filter = Gtk::FileFilter::create();

  temp_file_filter -> set_name(".cue - (CUESHEET)");

  temp_file_filter -> add_mime_type("application/x-cue");

  file_chooser_ -> add_filter(temp_file_filter);


  temp_file_filter = Gtk::FileFilter::create();

  temp_file_filter -> set_name(".ape - (Monkey's Audio)");

  temp_file_filter -> add_mime_type("audio/x-ape");

  file_chooser_ -> add_filter(temp_file_filter);


  temp_file_filter = Gtk::FileFilter::create();

  temp_file_filter -> set_name(".flac - (FLAC)");

  temp_file_filter -> add_mime_type("audio/flac");

  file_chooser_ -> add_filter(temp_file_filter);


  temp_file_filter = Gtk::FileFilter::create();

  temp_file_filter -> set_name(".m4a - (AAC or ALAC)");

  temp_file_filter -> add_mime_type("audio/mp4");

  file_chooser_ -> add_filter(temp_file_filter);


  temp_file_filter = Gtk::FileFilter::create();

  temp_file_filter -> set_name(".mp3 - (MP3)");

  temp_file_filter -> add_mime_type("audio/mpeg");

  file_chooser_ -> add_filter(temp_file_filter);


  temp_file_filter = Gtk::FileFilter::create();

  temp_file_filter -> set_name(".ogg | .oga - (Ogg Vorbis)");

  temp_file_filter -> add_mime_type("audio/ogg");

  file_chooser_ -> add_filter(temp_file_filter);


  temp_file_filter = Gtk::FileFilter::create();

  temp_file_filter -> set_name(".wv - (WavPack)");

  temp_file_filter -> add_mime_type("audio/x-wavpack");

  file_chooser_ -> add_filter(temp_file_filter);



  string last_folder_str = config() . get("last_folder");

  if(last_folder_str != "")
  {

    file_chooser_ -> set_current_folder(last_folder_str);

  }


  file_chooser_ -> signal_current_folder_changed()
    . connect(sigc::mem_fun
        (*this, &FileChooser::On_Current_Folder_Changed_Signal));

}





FileChooser::~FileChooser()
{



}





void FileChooser::On_Current_Folder_Changed_Signal()
{

  // 
  config() . set("last_folder", file_chooser_ -> get_current_folder());



  // 
  config() . write_file();

}





void FileChooser::Use_Selected()
{



  vector<string> filenames = file_chooser_ -> get_filenames();


  playlists().Append_Rows(metadata().Filenames_To_Tracks(filenames),
                          playlists() . selected_playlist_treestore());



}
