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





#ifndef SCROBBLING_H
#define SCROBBLING_H





#include "../Parts.h"



#include <atomic>
#include <list>
#include <memory>
#include <thread>



class Track;
class Base;
class Playback;
class Signals;
class Configuration;





struct ScrobblingThread
{



  ScrobblingThread()
  {}

  ~ScrobblingThread()
  { 

    //thread_ -> detach(); 

  }

  std::shared_ptr<std::thread> thread_;

  std::list<ScrobblingThread>::iterator scrobbling_threads_it_;



};





class Scrobbling : public Parts
{

  public:

    enum Action
    {

      LOGIN = 0,
      UPDATE,
      SCROBBLE,
      LOVE

    };



    Scrobbling(Base& base);

    ~Scrobbling();

    void Login_Lastfm();

    void Update_Playing_Track_Lastfm();

    void Scrobble_Playing_Track_Lastfm();

    bool Timeout_Scrobbling(int timeout_number);

    void Update_Track(Track& track_ref);

    void Scrobble_Track(Track& track_ref);

    void Track_Action(Scrobbling::Action action,
                      std::shared_ptr<bool> thread_finished,
                      std::shared_ptr<bool> successful);

    void Track_Action(Scrobbling::Action action,
                      Track& new_track,
                      std::shared_ptr<bool> thread_finished,
                      std::shared_ptr<bool> successful);

  protected:

    void Login(Track temp_track, 
               ScrobblingThread* scrobbling_thread,
               std::string temp_username, 
               std::string temp_password,
               std::shared_ptr<bool> thread_finished,
               std::shared_ptr<bool> successful);

    void Update(Track temp_track,
               ScrobblingThread* scrobbling_thread,
               std::string temp_username, 
               std::string temp_password,
               int song_duration_seconds,
               std::shared_ptr<bool> thread_finished,
               std::shared_ptr<bool> successful);

    void Scrobble(Track temp_track, 
                  ScrobblingThread *scrobbling_thread,
                  std::string temp_username, 
                  std::string temp_password,
                  std::shared_ptr<bool> thread_finished,
                  std::shared_ptr<bool> successful);

    std::list<ScrobblingThread>& scrobbling_threads();
 
    std::atomic<bool>& scrobbling_threads_active()
    {

      return scrobbling_threads_active_;  

     }

    void set_scrobbling_threads_active(bool new_setting)
    {

      scrobbling_threads_active_.store(new_setting, std::memory_order_relaxed);

    }

  public:

    void set_scrobbling_loop_paused(bool new_value)
    {

      scrobbling_loop_paused_ = new_value;

    }

    bool restart()
    {

      return restart_;

    }

    void set_restart(bool new_value)
    {

      restart_ = new_value;

    }
 
  private:

    std::list<ScrobblingThread> scrobbling_threads_;

    std::list<Track*> failed_scrobbles_;

    std::atomic<bool> scrobbling_threads_active_;
 
    std::atomic<bool> failed_scrobbles_active_;

    Track* playing_scrobble_track_;

    bool scrobbling_loop_paused_;

    bool restart_;

    Track* scrobbled_track_stage_;



};





#endif
