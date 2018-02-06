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

#include "FailedScrobblesDatabase.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"

#include "../Metadata/Track.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/ustring.h>

#include <iostream>

#include <memory>

#include <pwd.h>

#include <stdio.h>

#include <sqlite3.h>

#include <sstream>

#include <string.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <tuple>

#include <unistd.h>





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

FailedScrobblesDatabase::FailedScrobblesDatabase(Base& base_ref)

// Inherited Class

: Parts(base_ref, true)



//
, database_size_(0)

{

  // 
  int result_code;



  // 
  struct passwd* pw = getpwuid(getuid());

  // 
  const char *homedir = pw -> pw_dir;

  // 
  std::string directory_str = homedir;

  // 
  directory_str += "/.omp";



  // 
  struct stat st; 

  // 
  if(!(stat(directory_str . c_str(), &st) == 0))
  {

    string mkdir_str = "mkdir " + directory_str;

    system(mkdir_str . c_str());

  }



  // 
  string db_str = directory_str + "/failedscrobbles.db";



  // 
  result_code = sqlite3_open(db_str . c_str(), &database_);

  // 
  if(result_code)
  { 

    stringstream debug_ss;

    debug_ss << "Can't open database: " << sqlite3_errmsg(database_);

    debug(debug_ss . str() . c_str());

  } 

  // 
  else
  {

    debug("Opened database successfully!");

  }



  // 
  if(Create_Table())
  {

  }

  // 
  else
  {

    // 
    vector<Track*> unscrobbled_tracks;

    // 
    vector<int> unscrobbled_track_ids;

    // 
    vector<long> unscrobbled_track_times;



    // 
    Extract_Tracks(&unscrobbled_tracks, &unscrobbled_track_ids, 
                   &unscrobbled_track_times);



    // 
    int count = 0;

    // 
    for(auto it : unscrobbled_tracks)
    {

      count++;

    }



    // 
    database_size_ = count;

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

FailedScrobblesDatabase::~FailedScrobblesDatabase()
{

  sqlite3_close(database_);

}





//                  //
//                  //
// Member Functions ////////////////////////////////////////////////////////////
//                  //
//                  //

bool FailedScrobblesDatabase::Add_Track(Track& new_track, long time)
{

  // 
  lock_guard<mutex> database_mutex_lock(database_mutex_);



  stringstream debug_ss;

  debug_ss << "\n\ndatabase size: " << database_size_ << "\n\n";

  debug(debug_ss . str() . c_str());



  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql = "";



  // 
  Glib::ustring* artists_str_ptr = new_track . artists_string();

  // 
  sql += "INSERT INTO FailedScrobbles " \
         "(ID, ALBUM, ARTIST, TITLE, TIME) " \
           "VALUES (" + to_string(database_size_) + ", '"
             + Convert(new_track . album() . raw()) + "', '"
             + Convert(artists_str_ptr -> raw()) + "', '"
             + Convert(new_track . title() . raw()) + "', "
             + to_string(time) + ");";



  // 
  database_size_++;



  // 
  delete artists_str_ptr;



  // 
  result_code = sqlite3_exec(database_, sql . c_str(), 0,
                             0, &error_message);



  //    
  if(result_code != SQLITE_OK )
  {

    stringstream debug_ss;

    debug_ss << "SQL error: " << error_message;

    debug(debug_ss . str() . c_str());



    // 
    sqlite3_free(error_message);

    // 
    return false;

  } 

  // 
  else
  {

    // 
    return true;

  } 



  // 
  return false;

}

bool FailedScrobblesDatabase::Clear()
{

  // 
  lock_guard<mutex> database_mutex_lock(database_mutex_);



  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql = "";



  // 
  sql = "DELETE FROM FailedScrobbles; VACUUM;";



  // 
  result_code = sqlite3_exec(database_, sql . c_str(), 0,
                             0, &error_message);



  //    
  if(result_code != SQLITE_OK )
  {

    stringstream debug_ss;

    debug_ss << "SQL error: " << error_message;

    debug(debug_ss . str() . c_str());



    // 
    sqlite3_free(error_message);



    // 
    return false;

  }

  // 
  else
  {

    // 
    database_size_ = 0;



    // 
    return true;

  }

}     

std::string FailedScrobblesDatabase::Convert(std::string raw_str)
{

  // 
  string new_str;



  // 
  for(auto it : raw_str)
  {

    // 
    if(it == '\'')
    {

      // 
      new_str += "''";

    }

    // 
    else
    {

      // 
      new_str += it;

    }

  }



  // 
  return new_str;

}

bool FailedScrobblesDatabase::Create_Table()
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql;



  // 
  sql = "CREATE TABLE FailedScrobbles " \
         "("  \
         "ID                       INT PRIMARY KEY   NOT NULL," \
         "ALBUM                    TEXT              NOT NULL," \
         "ARTIST                   TEXT              NOT NULL," \
         "TITLE                    TEXT              NOT NULL," \
         "TIME                     INT               NOT NULL);";



  // 
  result_code = sqlite3_exec(database_, sql . c_str(), 0,
                             0, &error_message);



  //    
  if(result_code != SQLITE_OK )
  {

    stringstream debug_ss;

    debug_ss << "SQL error: " << error_message;

    debug(debug_ss . str() . c_str());



    // 
    sqlite3_free(error_message);

    // 
    return false;

  }

  // 
  else
  {

    // 
    return true;

  }

}      

bool FailedScrobblesDatabase::Delete_Track(int track_id)
{ 

  // 
  lock_guard<mutex> database_mutex_lock(database_mutex_);



  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql
    = "DELETE from FailedScrobbles where ID=" + to_string(track_id) + ";";



  // 
  result_code = sqlite3_exec(database_, sql . c_str(), 0,
                             0, &error_message);



  //    
  if(result_code != SQLITE_OK )
  {

    stringstream debug_ss;

    debug_ss << "SQL error: " << error_message;

    debug(debug_ss . str() . c_str());



    // 
    sqlite3_free(error_message);

    // 
    return false;

  }

  // 
  else
  {

    // 
    database_size_--;



    // 
    return true;

  }

}

bool FailedScrobblesDatabase::Extract_Tracks
  (vector<Track*>* tracks, vector<int>* ids, vector<long>* times)
{

  // 
  lock_guard<mutex> database_mutex_lock(database_mutex_);



  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql;



  // 
  sql = "SELECT * from FailedScrobbles;";



  // 
  tuple<vector<Track*>*, vector<int>*, vector<long>*> tracks_ids_times_tuple
    = make_tuple(tracks, ids, times);



  // 
  result_code = sqlite3_exec(database_, sql . c_str(), Extract_Tracks_Callback,
                             &tracks_ids_times_tuple, &error_message);



  //    
  if(result_code != SQLITE_OK )
  { 

    stringstream debug_ss;

    debug_ss << "SQL error: " << error_message;

    debug(debug_ss . str() . c_str());



    // 
    sqlite3_free(error_message);



    // 
    return false;

  }

  // 
  else
  {

    // 
    return true;

  }

}

int FailedScrobblesDatabase::Extract_Tracks_Callback
  (void* tracks_ids_times_tuple_vptr, int argc, char **argv,
   char **column_name)
{

  // 
  auto tracks_ids_times_tuple_ref
    = *(tuple<vector<Track*>*, vector<int>*, vector<long>*>*)
        (tracks_ids_times_tuple_vptr);

  // 
  auto tracks_ptr = get<0>(tracks_ids_times_tuple_ref);

  // 
  auto ids_ptr = get<1>(tracks_ids_times_tuple_ref);

  // 
  auto times_ptr = get<2>(tracks_ids_times_tuple_ref);



  // 
  Track* new_track_ptr = new Track;



  // 
  for(int i = 0; i < argc; i++)
  {

    // 
    if(strcmp(column_name[i], "ID") == 0)
    {

      // 
      ids_ptr -> push_back(atoi(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "ALBUM") == 0)
    {

      // 
      new_track_ptr -> set_album(argv[i]);

    }

    // 
    else if(strcmp(column_name[i], "ARTIST") == 0)
    {

      // 
      new_track_ptr -> add_artist(argv[i]);

    }

    // 
    else if(strcmp(column_name[i], "TITLE") == 0)
    {

      // 
      new_track_ptr -> set_title(argv[i]);

    }

    // 
    else if(strcmp(column_name[i], "TIME") == 0)
    { 

      // 
      times_ptr -> push_back(atoi(argv[i]));

    }

  }



  // 
  tracks_ptr -> push_back(new_track_ptr);



  // 
  return 0;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

int FailedScrobblesDatabase::database_size()
{

  // 
  lock_guard<mutex> database_mutex_lock(database_mutex_);



  // 
  return database_size_;

}





//         //
//         //
// Setters ////////////////////////////////////////////////////////////////////
//         //
//         //

void FailedScrobblesDatabase::set_database_size(int new_database_size)
{

  // 
  lock_guard<mutex> database_mutex_lock(database_mutex_);



  // 
  database_size_ = new_database_size;

}
