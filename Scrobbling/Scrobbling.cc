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

#include "Scrobbling.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"

#include "../Configuration/Configuration.h"

#include "../Metadata/Track.h"

#include "../Playback/Playback.h"

#include "FailedScrobblesDatabase.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <chrono>

extern "C"
{

  #include "clastfm.h"

}

#include <glibmm.h>

#include <iomanip>

#include <list>

#include <thread>

#include <mutex>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

using namespace std;





//        //
//        //
//        //
// Macros /////////////////////////////////////////////////////////////////////
//        //
//        //
//        //

namespace sigc
{

  SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE

}





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

Scrobbling::Scrobbling(Base& base_ref)

// Inherited Class

: Parts(base_ref)



// Member Variables

, failed_scrobbles_database_(new FailedScrobblesDatabase(base_ref))

, playing_scrobble_track_(nullptr)

, restart_(false)

, scrobbling_threads_active_(false)

{ 

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Scrobbling::~Scrobbling()
{ 

  // 
  delete failed_scrobbles_database_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void Scrobbling::Login(Track temp_track, 
                       ScrobblingThread *scrobbling_thread,
                       string temp_username,
                       string temp_password,
                       shared_ptr<bool> thread_finished, 
                       shared_ptr<bool> successful)
{

  LASTFM_SESSION *lastfm_session;

  lastfm_session = LASTFM_init("41940cc3bb675fba6283b0891b9de7f3",              
                               "f18ff7cee0603cdf532e73297bc5a2db");

  char* username = const_cast<char*>(temp_username.c_str());
  char* password = const_cast<char*>(temp_password.c_str());


  int login_info
    = LASTFM_login_MD5(lastfm_session, username, password);


  if(login_info != 0)
  {

    debug("An error occurred when logining in!");


  }
  else
  {

    debug("Login succcesful");

    *successful = true;

  }

  LASTFM_dinit(lastfm_session);

  *thread_finished = true;

  delete scrobbling_thread;

  debug("After delete scrobbling_thread");

}

void Scrobbling::Login_Lastfm()
{

  debug("Logging in!");



  // 
  Track temp_useless_track;



  // 
  Track_Action(Scrobbling::Action::LOGIN, temp_useless_track, 
               shared_ptr<bool>(new bool(false)), 
               shared_ptr<bool>(new bool(false)));

}

void Scrobbling::Rescrobble_Failed_Scrobbles(shared_ptr<bool> thread_finished)
{

  // Thread to scrobble the failed scrobbles in.
  thread rescrobble_thread([this, thread_finished]
  {

    // Holds all of the unscrobbled tracks.
    vector<Track*> unscrobbled_tracks;

    // Holds the ids of the unscrobbled tracks.
    vector<int> unscrobbled_track_ids;

    // Holds the times
    vector<long> unscrobbled_track_times;



    // Extracts new-created Tracks of the tracks that failed to scrobble.
    failed_scrobbles_database_
      -> Extract_Tracks(&unscrobbled_tracks, &unscrobbled_track_ids, 
                        &unscrobbled_track_times);



    // Keeps count of the tracks attempted to rescrobble.
    int count = 0;

    // Loops through all of the tracks that need a scrobble reattempt.
    for(auto unscrobbled_track_ptr : unscrobbled_tracks)
    {

      // Holds the LASTFM_SESSION, a clastfm variable.
      LASTFM_SESSION *lastfm_session;

      // Initializes the LASTFM_SESSION.
      lastfm_session = LASTFM_init("41940cc3bb675fba6283b0891b9de7f3",              
                                   "f18ff7cee0603cdf532e73297bc5a2db");



      // Gets the username from the config.
      string username_str = config() . get("scrobbling.lastfm_username");

      // Gets the password from the config.
      string password_str = config() . get("scrobbling.lastfm_password");

      // Converts the username into a c_str.
      char* username_c_str = const_cast<char*>(username_str . c_str());

      // Converts the password into a c_str.
      char* password_c_str = const_cast<char*>(password_str . c_str());



      // Acquires the time the track was originally attempted to be scrobbled.
      time_t started = unscrobbled_track_times[count];



      // Holds the result of loginning in.
      int login_info
        = LASTFM_login_MD5(lastfm_session, username_c_str, password_c_str);

      // True if unable to login to last.fm.
      if(login_info != 0)
      {  

        debug("An error occurred when logining in!");

      }

      // 
      else
      {

        debug("Login succcesful!");



        // Holds the title to be sent to last.fm.
        char title[100];

        // Holds the album name to be sent to last.fm.
        char album[100];

        // Holds the artist name to be sent to last.fm.
        char artist[100];

        // Temporary Glib::ustring used to extract the data from the Track.
        Glib::ustring* temp_artists;




        // True if unscrobbled Track has no title.
        if((unscrobbled_track_ptr -> title()) == "")
        {

          // Deletes the track from the database.
          failed_scrobbles_database_ -> Delete_Track(count);


          // Delete the new-created Track.
          delete unscrobbled_track_ptr;



          // Continues the loop.
          continue;

        }

        // True if the is now album artist(s).
        else if(unscrobbled_track_ptr -> album_artists() . empty())
        {

          // True if there is also no artist.
          if(unscrobbled_track_ptr -> artists() . empty())
          {

            // Deletes the track from the database.
            failed_scrobbles_database_ -> Delete_Track(count);



            // Delete the new-created Track.
            delete unscrobbled_track_ptr;



            // Continues the loop.
            continue;

          }

          // 
          else
          {

            // Users the artists metadata to send the artist to last.fm.
            temp_artists = unscrobbled_track_ptr -> artists_string();

          }

        }

        // 
        else
        {

          // Users the album artists metadata to send the artist to last.fm.
          temp_artists = unscrobbled_track_ptr -> album_artists_string();

        }





        // Copies the title into the title variable usable by LASTFM_SESSION.
        strcpy(title, const_cast<char*>((unscrobbled_track_ptr -> title()) . c_str()));

        // Copies the album into the album variable usable by LASTFM_SESSION.
        strcpy(album, const_cast<char*>((unscrobbled_track_ptr -> album()) . c_str()));

        // Copies the artist into the artist variable usable by LASTFM_SESSION.
        strcpy(artist, const_cast<char*>(temp_artists -> c_str()));



        // Deletes the temporary artist new-created string.
        delete temp_artists;



        // Attempts to scrobble the track.
        int result
          = LASTFM_track_scrobble(lastfm_session, title, album, artist,
                                  started, 1000, 0, 0, NULL);



        // True if the scrobbled failed.
        if(result != 0)
        {

          debug("Track failed to scrobble!");

        }

        // True if the scrobble succeeded.
        else
        {

          debug("Track scrobbled!!");



          // Deletes the track from the database.
          failed_scrobbles_database_ -> Delete_Track(count);

        } 

      }



      // Unitializes the LASTFM_SESSION.
      LASTFM_dinit(lastfm_session);



      // Increases the count of the tracks attempted to be scrobbled.
      count++;

    }



    // Sets the atomic flag indicating the thread is finished to true.
    *thread_finished = true;

  });  



  // Detaches the rescrobbling thread.
  rescrobble_thread . detach();

}

void Scrobbling::Scrobble(Track temp_track, 
                          ScrobblingThread *scrobbling_thread, 
                          string temp_username,
                          string temp_password,
                          shared_ptr<bool> thread_finished,
                          shared_ptr<bool> successful)
{                        

  // 
  LASTFM_SESSION *lastfm_session;

  //
  lastfm_session = LASTFM_init("41940cc3bb675fba6283b0891b9de7f3",              
                               "f18ff7cee0603cdf532e73297bc5a2db");



  // 
  char* username = const_cast<char*>(temp_username.c_str());

  // 
  char* password = const_cast<char*>(temp_password.c_str());



  // 
  time_t started;
         time(&started);
         started -= 1;



  // 
  int login_info = LASTFM_login_MD5(lastfm_session, username, password);

  // 
  if(login_info != 0)
  {

    debug("An error occurred when logining in!");



    // 
    failed_scrobbles_database_ -> Add_Track(temp_track, long(started));

  }

  // 
  else
  {

    debug("Login succcesful!");



    // Holds the title to be sent to last.fm.
    char title[100];

    // Holds the album name to be sent to last.fm.
    char album[100];

    // Holds the artist name to be sent to last.fm.
    char artist[100];

    // Temporary Glib::ustring used to extract the data from the Track.
    Glib::ustring* temp_artists;




    // True if unscrobbled Track has no title.
    if((temp_track . title()) == "")
    {

      // 
      LASTFM_dinit(lastfm_session);



      // 
      *thread_finished = true;



      //
      delete scrobbling_thread;



      // 
      return;

    }

    // True if the is now album artist(s).
    else if(temp_track . album_artists() . empty())
    {

      // True if there is also no artist.
      if(temp_track . artists() . empty())
      {

        // 
        LASTFM_dinit(lastfm_session);



        // 
        *thread_finished = true;



        //
        delete scrobbling_thread;



        // 
        return;

      }

      // 
      else
      {

        // Users the artists metadata to send the artist to last.fm.
        temp_artists = temp_track . artists_string();

      }

    }

    // 
    else
    {

      // Users the album artists metadata to send the artist to last.fm.
      temp_artists = temp_track . album_artists_string();

    }





    // Copies the title into the title variable usable by LASTFM_SESSION.
    strcpy(title, const_cast<char*>((temp_track . title()) . c_str()));

    // Copies the album into the album variable usable by LASTFM_SESSION.
    strcpy(album, const_cast<char*>((temp_track . album()) . c_str()));

    // Copies the artist into the artist variable usable by LASTFM_SESSION.
    strcpy(artist, const_cast<char*>(temp_artists -> c_str()));



    // Deletes the temporary artist new-created string.
    delete temp_artists;



    // 
    int result = LASTFM_track_scrobble(lastfm_session, title, album, artist,
                                       started, 1000, 0, 0, NULL);



    // 
    if(result != 0)
    {

      // 
      failed_scrobbles_database_ -> Add_Track(temp_track, long(started));

    }

    // 
    else
    {

      debug("track track scrobbled!!");

    }

  }



  // 
  LASTFM_dinit(lastfm_session);



  // 
  *thread_finished = true;



  //
  delete scrobbling_thread;

} 

void Scrobbling::Scrobble_Playing_Track_Lastfm()
{

  // 
  scrobbling_loop_paused_ = false;

  // 
  restart_ = true;

  // 
  sigc::connection program_conn = Glib::signal_timeout().connect
  (

    // 
    [this]() -> bool
    {

      // 
      static chrono::time_point<chrono::system_clock> start;

      // 
      static chrono::time_point<chrono::system_clock> end;

      // 
      static long long saved_elapsed_nanoseconds = 0LL;

      // 
      long long total_elapsed_nanoseconds = 0LL;



      // 
      if(scrobbling_loop_paused_)
      {

        end = chrono::system_clock::now();

        chrono::duration<double> elapsed_seconds;

        elapsed_seconds = end - start;

        saved_elapsed_nanoseconds += 1000000000LL * elapsed_seconds.count();

        return false;

      }

      if(restart_)
      {


        start = chrono::system_clock::now();

        restart_ = false;

      }


      if(playing_scrobble_track_ == nullptr)
      {

        playing_scrobble_track_ = new Track(playback().playing_track());

        saved_elapsed_nanoseconds = 0LL;

      }
      else if(*playing_scrobble_track_ != playback().playing_track())
      {

        if(playback().playing_track().Is_Empty())
        {

          auto temp_playing_scrobble_track_ = playing_scrobble_track_;

          playing_scrobble_track_ = nullptr;

          delete temp_playing_scrobble_track_;

          return false;

        }
        else
        {

          *playing_scrobble_track_ = playback().playing_track();

          saved_elapsed_nanoseconds = 0LL;

          restart_ = true;

          return false;

        }       

      }

      if((playback().Paused()))
      {

        return true;

      }
      else if(playback().Stopped())
      {

        return false;

      }

      if(playback().Duration() == 0LL)
      {

        return true;

      }



      // 
      end = chrono::system_clock::now();



      // 
      chrono::duration<double> elapsed_seconds;

      // 
      elapsed_seconds = end - start;

      // 
      total_elapsed_nanoseconds 
        = saved_elapsed_nanoseconds 
            + (1000000000LL * (elapsed_seconds.count()));

      // 
      int calculated_percent 
        = (int(100 * (total_elapsed_nanoseconds / double(playback().Duration()))));



      // 
      if(calculated_percent >= int(config().get("scrobbling.percent")))
      { 

        // 
        if(bool(config() . get("scrobbling.lastfm_enabled")))
        {

          Track_Action(Scrobbling::Action::SCROBBLE, *playing_scrobble_track_,
                       shared_ptr<bool>(new bool(false)), 
                       shared_ptr<bool>(new bool(false)));

        } 



        // 
        playing_scrobble_track_ = nullptr;

        //
        saved_elapsed_nanoseconds = 0LL;



        // 
        return false;

      }

      // 
      else
      {

        return true;

      }

     }, 100 

  );
  


  debug("Scrobbling playing track end!");

}

void Scrobbling::Track_Action(Scrobbling::Action action,
                              shared_ptr<bool> thread_finished,
                              shared_ptr<bool> successful)
{ 

  Track temp_useless_track;

  Track_Action(action, temp_useless_track, thread_finished, successful); 

}

void Scrobbling::Track_Action(Scrobbling::Action action,
                              Track &new_track,
                              shared_ptr<bool> thread_finished,
                              shared_ptr<bool> successful)
{

  ScrobblingThread *new_scrobbling_thread = new ScrobblingThread;

  string temp_username = config().get("scrobbling.lastfm_username");
  string temp_password = config().get("scrobbling.lastfm_password");


  *thread_finished = false;
  *successful = false;

  switch(action)
  {

    case LOGIN: //LOGIN Scrobbling enum
    {


      debug("Making LOGIN ScrobblingThread");

      new_scrobbling_thread -> thread_
        = make_shared<thread>(&Scrobbling::Login, 
                              this, 
                              new_track, 
                              new_scrobbling_thread,
                              temp_username,
                              temp_password,
                              thread_finished,
                              successful);


      break;

    }
    case UPDATE: //UPDATE Scrobbling enum
    { 


      new_scrobbling_thread -> thread_
        = make_shared<thread>(&Scrobbling::Update, 
                              this, 
                              new_track, 
                              new_scrobbling_thread,
                              temp_username,
                              temp_password,
                              ((playback().Duration()) 
                                - (playback().Position())) / 1000000000,
                              thread_finished,
                              successful); 
      
      break;

    }
    case SCROBBLE: //SCROBBLE Scrobbling enum
    { 

      new_scrobbling_thread -> thread_
        = make_shared<thread>(&Scrobbling::Scrobble, 
                              this, 
                              new_track, 
                              new_scrobbling_thread,
                              temp_username,
                              temp_password,
                              thread_finished,
                              successful);

      break; 

    }
    case LOVE: //LOVE Scrobbling enum
    {

      break;

    }
    default:
    {

    }

  }

  new_scrobbling_thread -> thread_ -> detach();

}

void Scrobbling::Update(Track temp_track, 
                        ScrobblingThread *scrobbling_thread,
                        string temp_username,
                        string temp_password,
                        int song_duration_seconds,
                        shared_ptr<bool> thread_finished,
                        shared_ptr<bool> successful)
{

  LASTFM_SESSION *lastfm_session;

  lastfm_session = LASTFM_init("41940cc3bb675fba6283b0891b9de7f3",              
                               "f18ff7cee0603cdf532e73297bc5a2db");

//  Base &Main = base();

  char* username = const_cast<char*>(temp_username.c_str());
  char* password = const_cast<char*>(temp_password.c_str());

  debug("Lastfm initalized!");

//  try
//  {

  int login_info = LASTFM_login_MD5(lastfm_session, username, password);

  if(login_info != 0)
  {

    debug("An error occurred when logining in!");

    return;

  }
  else
  {

    debug("Login succcesful!");

  }

  const char *status = NULL;

  char title[100],
       album[100],
       artist[100];

  strcpy(title, const_cast<char*>((temp_track.title()).c_str()));

  strcpy(album, const_cast<char*>((temp_track.album()).c_str()));

  Glib::ustring *temp_artists = temp_track.artists_string();

  strcpy(artist, const_cast<char*>(temp_artists -> c_str()));

  delete temp_artists;



  LASTFM_status(lastfm_session, &status, NULL, NULL);

  int result = LASTFM_track_update_now_playing(lastfm_session, title, album, 
                                               artist, song_duration_seconds, 
                                               1, 0, NULL);

  stringstream debug_ss;

  debug_ss << "Update lastfm result " << result;

  debug(debug_ss . str() . c_str());


//  LASTFM_print_session(stdout,lastfm_session);

  LASTFM_dinit(lastfm_session);


  *thread_finished = true;

  delete scrobbling_thread;

}

void Scrobbling::Update_Playing_Track_Lastfm()
{

  // 
  if(!(config() . get("scrobbling.lastfm_enabled")))
  {

    // 
    return;

  }



  // 
  sigc::connection program_conn = Glib::signal_timeout().connect
  (

    [this]() -> bool
    {

      if(playback().Duration() != 0LL)
      {


        debug("Updating track");

        Track_Action(Scrobbling::Action::UPDATE, playback().playing_track(),
                     shared_ptr<bool>(new bool(false)), 
                     shared_ptr<bool>(new bool(false)));


        return false;

      }
      else
      {

        return true;

      }

    }, 
    100 

  );

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

FailedScrobblesDatabase& Scrobbling::failed_scrobbles_database()
{

  return *failed_scrobbles_database_;

}

bool Scrobbling::restart()
{

  return restart_;

}

list<ScrobblingThread>& Scrobbling::scrobbling_threads()
{ 

  while(scrobbling_threads_active() . load())
  { 

     debug("scrobbling thread sleeping while waiting for access");

       Glib::usleep(25000);

  }

  set_scrobbling_threads_active(true);

  return scrobbling_threads_;

}

std::atomic<bool>& Scrobbling::scrobbling_threads_active()
{

  return scrobbling_threads_active_;  

}





//         //
//         //
// Setters ////////////////////////////////////////////////////////////////////
//         //
//         //

void Scrobbling::set_restart(bool new_value)
{

  restart_ = new_value;

}

void Scrobbling::set_scrobbling_threads_active(bool new_setting)
{

  scrobbling_threads_active_.store(new_setting, std::memory_order_relaxed);

}

void Scrobbling::set_scrobbling_loop_paused(bool new_value)
{

  scrobbling_loop_paused_ = new_value;

}
