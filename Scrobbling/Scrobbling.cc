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





#include "Scrobbling.h"



#include "../Configuration/Configuration.h"
#include "../Base.h"
#include "../Playback/Playback.h"
#include "../Metadata/Track.h"



#include <iomanip>
#include <thread>
#include <mutex>
#include <list>
#include <chrono>

#include <glibmm.h>





//Includes the C API library for using various last.fm scrobbling features
extern "C"
{

  #include "clastfm.h"

}

namespace sigc
{

  SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE

}



using namespace std;






Scrobbling::Scrobbling(Base& base)
: Parts(base)
, restart_(false)
{

  playing_scrobble_track_ = nullptr;

  scrobbled_track_stage_ = nullptr;

  scrobbling_threads_active_.store(false, memory_order_relaxed);

}





Scrobbling::~Scrobbling()
{

  

}





void Scrobbling::Login_Lastfm()
{

  debug("Logging in!");

  Track temp_useless_track;

  Track_Action(Scrobbling::Action::LOGIN, temp_useless_track, 
               shared_ptr<bool>(new bool(false)), 
               shared_ptr<bool>(new bool(false)));

}





void Scrobbling::Update_Playing_Track_Lastfm()
{

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





void Scrobbling::Scrobble_Playing_Track_Lastfm()
{

  scrobbling_loop_paused_ = false;

  restart_ = true;

  sigc::connection program_conn = Glib::signal_timeout().connect
  (

    [this]() -> bool
    {


      static chrono::time_point<chrono::system_clock> start;

      static chrono::time_point<chrono::system_clock> end;

      static long long saved_elapsed_nanoseconds = 0LL;

      long long total_elapsed_nanoseconds = 0LL;

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

      end = chrono::system_clock::now();

      chrono::duration<double> elapsed_seconds;

      elapsed_seconds = end - start;

      total_elapsed_nanoseconds 
        = saved_elapsed_nanoseconds 
            + (1000000000LL * (elapsed_seconds.count()));

      int calculated_percent 
        = (int(100 * (total_elapsed_nanoseconds / double(playback().Duration()))));


      if(calculated_percent >= int(config().get("scrobbling.percent")))
      {

        if(bool(config() . get("scrobbling.lastfm_enabled")))
        {

          Track_Action(Scrobbling::Action::SCROBBLE, *playing_scrobble_track_,
                       shared_ptr<bool>(new bool(false)), 
                       shared_ptr<bool>(new bool(false)));

        }

        auto temp_playing_scrobble_track_ = playing_scrobble_track_;

        playing_scrobble_track_ = nullptr;

        scrobbled_track_stage_ = temp_playing_scrobble_track_;

        saved_elapsed_nanoseconds = 0LL;

        return false;

      }
      else
      {

        return true;

      }

    }, 
    100 

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





void Scrobbling::Scrobble(Track temp_track, 
                          ScrobblingThread *scrobbling_thread, 
                          string temp_username,
                          string temp_password,
                          shared_ptr<bool> thread_finished,
                          shared_ptr<bool> successful)
{                        

  LASTFM_SESSION *lastfm_session;

  lastfm_session = LASTFM_init("41940cc3bb675fba6283b0891b9de7f3",              
                               "f18ff7cee0603cdf532e73297bc5a2db");


//  try
//  {

  char* username = const_cast<char*>(temp_username.c_str());
  char* password = const_cast<char*>(temp_password.c_str());

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


  char title[100],
       album[100],
       artist[100];

  strcpy(title, const_cast<char*>((temp_track.title()).c_str()));

  strcpy(album, const_cast<char*>((temp_track.album()).c_str()));

  Glib::ustring *temp_artists = temp_track.artists_string();

  strcpy(artist, const_cast<char*>(temp_artists -> c_str()));

  delete temp_artists;


  time_t started;
         time(&started);
         started -= 1;

  LASTFM_track_scrobble(lastfm_session, title, album, artist,
                        started, 1000, 0, 0, NULL);

  LASTFM_dinit(lastfm_session);

  temp_track.Print();

  debug("track track scrobbled!!");

  *thread_finished = true;

  delete scrobbling_thread;



}






list<ScrobblingThread> &Scrobbling::scrobbling_threads()
{ 

  while(scrobbling_threads_active().load())
  { 

     debug("scrobbling thread sleeping while waiting for access");

       Glib::usleep(25000);

  }

  set_scrobbling_threads_active(true);

  return scrobbling_threads_;

}
