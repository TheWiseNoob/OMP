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

#ifndef PLAYLISTS_DATABASE_H
#define PLAYLISTS_DATABASE_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Parts.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <glibmm/refptr.h>

#include <mutex>

#include <string>

#include <vector>





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Base;

namespace Glib
{

  class ustring;

}

class PlaylistTreeStore;

class sqlite3;

class Track;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class PlaylistsDatabase : public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Creates the PlaylistsDatabase class.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    PlaylistsDatabase(Base& base_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Used to destroy any data that needs it when PlaylistsDatabase ends.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    ~PlaylistsDatabase();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    bool Add_Tracks(Glib::RefPtr<PlaylistTreeStore> playlist_treestore);

    bool Add_Column
      (const char* playlist_name, const char* column_name, const char* type);

    bool Cleanup_Database();

    bool Clear_Playlist(const char* playlist_name);

    std::string Convert(std::string raw_str);

    bool Create_Playlist(const char* playlist_name);

    bool Delete_Playlist(const char* playlist_name);

    bool Delete_Rows(const char* playlist_name, std::vector<int> ids);

    bool Drop_Table(const char* playlist_name);

    bool Extract_Tracks(Glib::RefPtr<PlaylistTreeStore> playlist_treestore);

    static int Extract_Tracks_Callback
      (void* tracks_and_ids_vptr, int argc, char **argv, char **column_name);

    Glib::ustring* Multiple_Values_Tag_Encode(std::vector<Glib::ustring*>& tag);

    bool Playlist_Names(std::vector<std::string>& playlist_names);

    static int Playlist_Names_Callback
      (void* names, int argc, char **argv, char **azColName);

    bool Rebuild_Database();

    bool Rename_Playlist
      (const char* playlist_name, const char* new_playlist_name);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    std::atomic<bool>& quit_rebuilding();
  




  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  private:

    sqlite3* database_;

    Glib::ustring* database_ustr_ptr_;

    std::mutex mutex_;

    std::atomic<bool> quit_rebuilding_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
