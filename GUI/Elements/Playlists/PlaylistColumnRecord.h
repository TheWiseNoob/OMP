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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef PLAYLIST_COLUMN_RECORD_H
#define PLAYLIST_COLUMN_RECORD_H 





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Metadata/Track.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/treemodel.h>

#include <gtkmm/treemodelcolumn.h>

#include <memory>

#include <string>





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class PlaylistColumnRecord : public Gtk::TreeModel::ColumnRecord
{ 

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    PlaylistColumnRecord()
    {

      // 
      add(album_col); 

      // 
      add(artist_col);

      // 
      add(bit_depth_col); 

      // 
      add(bit_rate_col); 

      // 
      add(channels_col); 

      // 
      add(codec_col); 

      // 
      add(filename_col); 

      //
      add(genre_col); 

      //
      add(id_); 

      //
      add(length_col);

      //
      add(mime_col);

      // 
      add(sample_rate_col); 

      // 
      add(title_col); 

      // 
      add(track_col);

      // 
      add(track_num_col); 

      // 
      add(track_total_col); 

      // 
      add(track_num_int_col); 

      // 
      add(track_total_int_col); 

    }





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //   

  public:

    // 
    Gtk::TreeModelColumn<Glib::ustring> album_col;

    // 
    Gtk::TreeModelColumn<Glib::ustring> artist_col;

    //
    Gtk::TreeModelColumn<Glib::ustring> bit_depth_col;

    //
    Gtk::TreeModelColumn<unsigned int> bit_rate_col;

    //
    Gtk::TreeModelColumn<unsigned int> channels_col;

    // 
    Gtk::TreeModelColumn<Glib::ustring> codec_col;

    //
    Gtk::TreeModelColumn<std::string> filename_col;

    // 
    Gtk::TreeModelColumn<Glib::ustring> genre_col;

    //
    Gtk::TreeModelColumn<unsigned int> id_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> length_col;

    // 
    Gtk::TreeModelColumn<Glib::ustring> mime_col;

    //
    Gtk::TreeModelColumn<unsigned int> sample_rate_col;

    // 
    Gtk::TreeModelColumn<Glib::ustring> title_col;

    //
    Gtk::TreeModelColumn<std::shared_ptr<Track>> track_col;

    // 
    Gtk::TreeModelColumn<Glib::ustring> track_num_col;

    // 
    Gtk::TreeModelColumn<unsigned int> track_num_int_col;

    // 
    Gtk::TreeModelColumn<Glib::ustring> track_total_col;

    // 
    Gtk::TreeModelColumn<unsigned int> track_total_int_col;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
