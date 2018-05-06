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

#include "PlaylistsDatabase.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Metadata/Track.h"

#include "PlaylistColumnRecord.h"

#include "Playlists.h"

#include "PlaylistTreeStore.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <iostream>

#include <memory>

#include <pwd.h>

#include <sqlite3.h>

#include <sys/stat.h>

#include <sys/types.h>

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

PlaylistsDatabase::PlaylistsDatabase(Base& base_ref)

// Inherited Class

: Parts(base_ref)

{

  // 
  int result_code;



  // 
  struct passwd *pw = getpwuid(getuid());

  // 
  const char *homedir = pw->pw_dir;

  // 
  std::string directory_str = homedir;

  // 
  directory_str += "/.omp";



  // 
  struct stat st; 

  // 
  if(!(stat(directory_str . c_str(),&st) == 0))
  {

    string mkdir_str = "mkdir " + directory_str;

    system(mkdir_str . c_str());

  }



  // 
  string db_str = directory_str + "/playlists.db";



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
  Create_Playlist("Queue");

  // 
  Create_Playlist("Library");

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaylistsDatabase::~PlaylistsDatabase()
{

  sqlite3_close(database_);

}





//                  //
//                  //
// Member Functions ////////////////////////////////////////////////////////////
//                  //
//                  //

bool PlaylistsDatabase::Add_Tracks
  (const char* playlist_name,
   Glib::RefPtr<PlaylistTreeStore> playlist_treestore)
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql = "";

  // 
  int id = 0;



  // 
  for(auto it : playlist_treestore -> children())
  {

    // 
    Gtk::TreeRow temp_treerow = it;

    // 
    shared_ptr<Track> track_sptr
      = temp_treerow[playlists() . playlist_column_record() . track_col];



    // 
    temp_treerow[playlists() . playlist_column_record() . id_] = id;



    // 
    Glib::ustring* artists_str_ptr
      = Multiple_Values_Tag_Encode(track_sptr -> artists());

    // 
    Glib::ustring* album_artists_str_ptr
      = Multiple_Values_Tag_Encode(track_sptr -> album_artists());

    // 
    Glib::ustring* genres_str_ptr
      = Multiple_Values_Tag_Encode(track_sptr -> genres());



    // 
    sql += "INSERT INTO '";

    // 
    sql += Convert(playlist_name);

    // 
    sql += "' (ID, ALBUM, ALBUM_ARTIST, ARTIST, BIT_DEPTH, BIT_RATE, CHANNELS, CODEC, " \
           "DATE, DURATION, END, FILE_NAME, GENRE, LENGTH_CS, MIME, PREGAP_START," \
           "REPLAY_GAIN_ALBUM_GAIN, REPLAY_GAIN_ALBUM_PEAK, " \
           "REPLAY_GAIN_TRACK_GAIN, REPLAY_GAIN_TRACK_PEAK, " \
           "SAMPLE_RATE, START, TITLE, TRACK_NUMBER, TRACK_TOTAL, " \
           "TYPE) " \
           "VALUES (" + to_string(id) + ", '"
             + Convert(track_sptr -> album()) + "', '"
             + Convert(album_artists_str_ptr -> raw()) + "', '"
             + Convert(artists_str_ptr -> raw()) + "', "
             + to_string(track_sptr -> bit_depth()) + ", "
             + to_string(track_sptr -> bit_rate()) + ", "
             + to_string(track_sptr -> channels()) + ", '"
             + Convert(track_sptr -> codec()) + "', "
             + to_string(track_sptr -> date()) + ", "
             + to_string(track_sptr -> duration()) + ", "
             + to_string(track_sptr -> end()) + ", '"
             + Convert(track_sptr -> filename()) + "', '"
             + Convert(genres_str_ptr -> raw()) + "', '"
             + Convert(track_sptr -> length()) + "', '"
             + Convert(track_sptr -> mime()) + "', "
             + to_string(track_sptr -> pregap_start()) + ", "
             + to_string(track_sptr -> replaygain_album_gain()) + ", "
             + to_string(track_sptr -> replaygain_album_peak()) + ", "
             + to_string(track_sptr -> replaygain_track_gain()) + ", "
             + to_string(track_sptr -> replaygain_track_peak()) + ", "
             + to_string(track_sptr -> sample_rate()) + ", "
             + to_string(track_sptr -> start()) + ", '"
             + Convert(track_sptr -> title()) + "', "
             + to_string(track_sptr -> track_number()) + ", "
             + to_string(track_sptr -> track_total()) + ", "
             + to_string(int(track_sptr -> type())) + ");";



    // 
    delete artists_str_ptr;

    // 
    delete genres_str_ptr;



    // 
    id++;

  }



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

bool PlaylistsDatabase::Add_Column
  (const char* playlist_name, const char* column_name, const char* type)
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql;  /* Create SQL statement */



  // 
  sql = "ALTER TABLE ";

  // 
  sql += Convert(playlist_name);

  // 
  sql += " ADD COLUMN ";

  // 
  sql += column_name;

  // 
  sql += " ";

  // 
  sql += type;

  // 
  sql += ";";



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

bool PlaylistsDatabase::Create_Playlist(const char* playlist_name)
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql;  /* Create SQL statement */



  // 
  sql = "CREATE TABLE '";

  // 
  sql += Convert(playlist_name);

  // 
  sql += "'("  \
         "ID                       INT PRIMARY KEY   NOT NULL," \
         "ALBUM                    TEXT," \
         "ALBUM_ARTIST             TEXT," \
         "ARTIST                   TEXT," \
         "BIT_DEPTH                INT               NOT NULL," \
         "BIT_RATE                 INT               NOT NULL," \
         "DATE                     INT               NOT NULL," \
         "DURATION                 INT               NOT NULL," \
         "CHANNELS                 INT               NOT NULL," \
         "CODEC                    TEXT              NOT NULL," \
         "END                      INT               NOT NULL," \
         "FILE_NAME                TEXT              NOT NULL," \
         "GENRE                    TEXT," \
         "LENGTH_CS                TEXT              NOT NULL," \
         "MIME                     TEXT              NOT NULL," \
         "PREGAP_START             INT               NOT NULL," \
         "REPLAY_GAIN_ALBUM_GAIN   REAL," \
         "REPLAY_GAIN_ALBUM_PEAK   REAL," \
         "REPLAY_GAIN_TRACK_GAIN   REAL," \
         "REPLAY_GAIN_TRACK_PEAK   REAL," \
         "SAMPLE_RATE              INT               NOT NULL," \
         "START                    INT               NOT NULL," \
         "TITLE                    TEXT," \
         "TRACK_NUMBER             INT," \
         "TRACK_TOTAL              INT," \
         "TYPE                     INT               NOT NULL);";



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

bool PlaylistsDatabase::Cleanup_Database()
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql = "";



  // 
  sql = "VACUUM;";



  // 
  result_code = sqlite3_exec(database_, sql . c_str(), 0, 0, &error_message);



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

std::string PlaylistsDatabase::Convert(std::string raw_str)
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

bool PlaylistsDatabase::Clear_Playlist(const char* playlist_name)
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql = "";



  // 
  sql = "DELETE FROM '";
  // 
  sql += Convert(playlist_name);

  sql += "';";



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

bool PlaylistsDatabase::Delete_Playlist(const char* playlist_name)
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql = "";



  // 
  sql = "DROP TABLE '";

  // 
  sql += Convert(playlist_name);

  sql += "'; VACUUM;";



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

bool PlaylistsDatabase::Delete_Rows
  (const char* playlist_name, std::vector<int> ids)
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql = "";



  for(auto it : ids)
  {

  // 
  sql += "DELETE from '";

  // 
  sql += Convert(playlist_name);

  // 
  sql += "' where ID=";

  sql += to_string(it) + ";";

  }

  


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

bool PlaylistsDatabase::Extract_Tracks
  (const char* playlist_name, vector<Track*>* tracks, vector<int>* ids)
{ 

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql;



  // 
  sql = "SELECT * from '";

  // 
  sql += Convert(playlist_name);

  // 
  sql+= "';";



  // 
  pair<vector<Track*>*, vector<int>*> tracks_and_ids;

  // 
  tracks_and_ids . first = tracks;

  // 
  tracks_and_ids . second = ids;



  // 
  result_code = sqlite3_exec(database_, sql . c_str(), Extract_Tracks_Callback,
                             &tracks_and_ids, &error_message);



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

int PlaylistsDatabase::Extract_Tracks_Callback
  (void* tracks_and_ids_vptr, int argc, char **argv, char **column_name)
{

  // 
  auto tracks_and_ids_ptr
    = (pair<vector<Track*>*, vector<int>*>*)(tracks_and_ids_vptr);

  // 
  auto tracks_ptr = tracks_and_ids_ptr -> first;

  // 
  auto ids_ptr = tracks_and_ids_ptr -> second;



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
    else if(strcmp(column_name[i], "ALBUM_ARTIST") == 0)
    {

      // 
      Glib::ustring album_artists_ustr = argv[i];



      // 
      new_track_ptr -> set_album_artists(new_track_ptr -> Multiple_Values_Tag_Decode(album_artists_ustr));

    }

    // 
    else if(strcmp(column_name[i], "ARTIST") == 0)
    {

      // 
      Glib::ustring artists_ustr = argv[i];



      // 
      new_track_ptr -> set_artists(new_track_ptr -> Multiple_Values_Tag_Decode(artists_ustr));

    } 

    // 
    else if(strcmp(column_name[i], "BIT_DEPTH") == 0)
    {

      // 
      new_track_ptr -> set_bit_depth(atoi(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "BIT_RATE") == 0)
    { 

      // 
      new_track_ptr -> set_bit_rate(atoi(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "CHANNELS") == 0)
    { 

      // 
      new_track_ptr -> set_channels(atoi(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "CODEC") == 0)
    {

      // 
      new_track_ptr -> set_codec(argv[i]);

    }

    // 
    else if(strcmp(column_name[i], "DATE") == 0)
    {

      // 
      new_track_ptr -> set_date(atoi(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "DURATION") == 0)
    {

      // 
      new_track_ptr -> set_duration(atol(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "END") == 0)
    { 

      // 
      new_track_ptr -> set_end(atol(argv[i]));

    }

    // 
    if(strcmp(column_name[i], "FILE_NAME") == 0)
    {

      // 
      new_track_ptr -> set_filename(argv[i]);

    }

    // 
    else if(strcmp(column_name[i], "GENRE") == 0)
    {

      // 
      Glib::ustring genres_ustr = argv[i];



      // 
      new_track_ptr -> set_genres(new_track_ptr -> Multiple_Values_Tag_Decode(genres_ustr));

    }

    // 
    else if(strcmp(column_name[i], "LENGTH_CS") == 0)
    {

      // 
      new_track_ptr -> set_length(argv[i]);

    }

    // 
    else if(strcmp(column_name[i], "MIME") == 0)
    {

      // 
      new_track_ptr -> set_mime(argv[i]);

    }

    // 
    else if(strcmp(column_name[i], "PREGAP_START") == 0)
    { 

      // 
      new_track_ptr -> set_pregap_start(atol(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "REPLAY_GAIN_ALBUM_GAIN") == 0)
    { 

      // 
      new_track_ptr -> set_replaygain_album_gain(atof(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "REPLAY_GAIN_ALBUM_PEAK") == 0)
    { 

      // 
      new_track_ptr -> set_replaygain_album_peak(atof(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "REPLAY_GAIN_TRACK_GAIN") == 0)
    { 

      // 
      new_track_ptr -> set_replaygain_track_gain(atof(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "REPLAY_GAIN_TRACK_PEAK") == 0)
    { 

      // 
      new_track_ptr -> set_replaygain_track_peak(atof(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "SAMPLE_RATE") == 0)
    { 

      // 
      new_track_ptr -> set_sample_rate(atoi(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "START") == 0)
    { 

      // 
      new_track_ptr -> set_start(atol(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "TITLE") == 0)
    {

      // 
      new_track_ptr -> set_title(argv[i]);

    }

    // 
    else if(strcmp(column_name[i], "TYPE") == 0)
    {

      // 
      TrackType type = TrackType(atoi(argv[i]));



      // 
      new_track_ptr -> set_type(type);

    }

    // 
    else if(strcmp(column_name[i], "TRACK_NUMBER") == 0)
    {

      // 
      new_track_ptr -> set_track_number(atoi(argv[i]));

    }

    // 
    else if(strcmp(column_name[i], "TRACK_TOTAL") == 0)
    {

      // 
      new_track_ptr -> set_track_total(atoi(argv[i]));

    }

  }



  // 
  tracks_ptr -> push_back(new_track_ptr);



  // 
  return 0;

}

Glib::ustring* PlaylistsDatabase::Multiple_Values_Tag_Encode
  (vector<Glib::ustring*>& tag)
{

  // 
  Glib::ustring* tags_ustr_ptr = new Glib::ustring;

  // 
  int count = 0;



  // 
  for(auto tag_it : tag)
  {

    // 
    if(count > 0)
    {

      // 
      (*tags_ustr_ptr) += "; ";

    }



    // 
    for(auto tag_it_char : *tag_it)
    {

      // 
      if(tag_it_char == ';')
      {

        // 
        (*tags_ustr_ptr) += "\\;";

      }

      // 
      else
      {

        // 
        (*tags_ustr_ptr) += tag_it_char;

      }

    }



    // 
    count++;

  }



  // 
  return tags_ustr_ptr;

}

bool PlaylistsDatabase::Playlist_Names(std::vector<std::string>& playlist_names)
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  char* sql;  // Create SQL statement



  // Create merged SQL statement 
  sql = "SELECT name FROM sqlite_master WHERE type='table';";



  // Execute SQL statement
  result_code = sqlite3_exec(database_, sql, Playlist_Names_Callback,
                             &playlist_names, &error_message);



  //    
  if(result_code != SQLITE_OK )
  {

//    fprintf(stderr, "SQL error: %s\n", error_message);

    sqlite3_free(error_message);

    return false;

  }

  // 
  else
  {


    return true;

  }
  

}

int PlaylistsDatabase::Playlist_Names_Callback
  (void* names, int argc, char **argv, char **azColName)
{

  int i;



  vector<string>* names_ptr = (vector<string>*)(names);


  for(i = 0; i < argc; i++)
  {

//      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

    names_ptr -> push_back(string(argv[i]));

  }


  return 0;

}

bool PlaylistsDatabase::Rename_Playlist
  (const char* playlist_name, const char* new_playlist_name)
{

  // 
  char* error_message = 0;

  // 
  int result_code;

  // 
  string sql = "";



  // 
  sql = "ALTER TABLE \'";

  // 
  sql += Convert(playlist_name);

  // 
  sql += "\' RENAME TO \'";

  sql += Convert(new_playlist_name);

  sql += "\';";



  // 
  result_code
  = sqlite3_exec(database_, sql . c_str(), 0, 0, &error_message);



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
