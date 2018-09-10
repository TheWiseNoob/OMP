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
      add(album_); 

      // 
      add(album_artists_); 

      // 
      add(artists_);

      // 
      add(bit_depth_); 

      // 
      add(bit_rate_); 

      // 
      add(channels_); 

      // 
      add(codec_); 

      // 
      add(date_);

      // 
      add(date_int_);

      // 
      add(disc_number_);

      // 
      add(disc_number_int_);

      // 
      add(disc_total_);

      // 
      add(disc_total_int_);

      // 
      add(filename_); 

      //
      add(genres_); 

      //
      add(id_); 

      //
      add(length_);

      //
      add(mime_);

      // 
      add(sample_rate_); 

      // 
      add(title_); 

      // 
      add(track_);

      // 
      add(track_number_); 

      // 
      add(track_total_); 

      // 
      add(track_number_int_); 

      // 
      add(track_total_int_); 

    }





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //   

  public:

    // 
    Gtk::TreeModelColumn<Glib::ustring> album_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> album_artists_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> artists_;

    //
    Gtk::TreeModelColumn<Glib::ustring> bit_depth_;

    //
    Gtk::TreeModelColumn<unsigned int> bit_rate_;

    //
    Gtk::TreeModelColumn<unsigned int> channels_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> codec_;

    //
    Gtk::TreeModelColumn<Glib::ustring> date_;

    //
    Gtk::TreeModelColumn<unsigned int> date_int_;

    //
    Gtk::TreeModelColumn<Glib::ustring> disc_number_;

    //
    Gtk::TreeModelColumn<unsigned int> disc_number_int_;

    //
    Gtk::TreeModelColumn<Glib::ustring> disc_total_;

    //
    Gtk::TreeModelColumn<unsigned int> disc_total_int_;

    //
    Gtk::TreeModelColumn<std::string> filename_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> genres_;

    //
    Gtk::TreeModelColumn<unsigned int> id_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> length_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> mime_;

    //
    Gtk::TreeModelColumn<unsigned int> sample_rate_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> title_;

    //
    Gtk::TreeModelColumn<std::shared_ptr<Track>> track_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> track_number_;

    // 
    Gtk::TreeModelColumn<unsigned int> track_number_int_;

    // 
    Gtk::TreeModelColumn<Glib::ustring> track_total_;

    // 
    Gtk::TreeModelColumn<unsigned int> track_total_int_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
