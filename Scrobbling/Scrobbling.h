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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef SCROBBLING_H
#define SCROBBLING_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                   //
//                   //
// Inherited Headers //////////////////////////////////////////////////////////
//                   //
//                   //

#include "../Parts.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <list>

#include <memory>

#include <thread>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class Base;

class Configuration;

class FailedScrobblesDatabase;

class Track;

class Playback;

class Signals;





//                    //
//                    //
//                    //
// Struct Declaration /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

struct ScrobblingThread
{

  ScrobblingThread()
  {

  }

  ~ScrobblingThread()
  { 

  }



  std::list<ScrobblingThread>::iterator scrobbling_threads_it_;

  std::shared_ptr<std::thread> thread_;

};





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Scrobbling : public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    Scrobbling(Base& base_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~Scrobbling();





  //       //
  //       //
  // Enums ////////////////////////////////////////////////////////////////////
  //       //
  //       //

  public:

    enum Action
    {

      LOGIN = 0,

      UPDATE,

      SCROBBLE,

      LOVE

    };





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  protected:

    void Login(Track temp_track, 
               ScrobblingThread* scrobbling_thread,
               std::string temp_username, 
               std::string temp_password,
               std::shared_ptr<bool> thread_finished,
               std::shared_ptr<bool> successful);

  public:

    void Login_Lastfm();

  protected:

    void Scrobble(Track temp_track, 
                  ScrobblingThread *scrobbling_thread,
                  std::string temp_username, 
                  std::string temp_password,
                  std::shared_ptr<bool> thread_finished,
                  std::shared_ptr<bool> successful);

  public:

    void Scrobble_Playing_Track_Lastfm();

    void Track_Action(Scrobbling::Action action,
                      std::shared_ptr<bool> thread_finished,
                      std::shared_ptr<bool> successful);

    void Track_Action(Scrobbling::Action action, Track& new_track,
                      std::shared_ptr<bool> thread_finished,
                      std::shared_ptr<bool> successful);

  protected:

    void Update(Track temp_track,
               ScrobblingThread* scrobbling_thread,
               std::string temp_username, 
               std::string temp_password,
               int song_duration_seconds,
               std::shared_ptr<bool> thread_finished,
               std::shared_ptr<bool> successful);

  public:

    void Update_Playing_Track_Lastfm();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    bool restart();

    std::list<ScrobblingThread>& scrobbling_threads();
 
    std::atomic<bool>& scrobbling_threads_active();





  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    void set_restart(bool new_value);

    void set_scrobbling_loop_paused(bool new_value);

    void set_scrobbling_threads_active(bool new_setting);




 
  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //
   
  private:

    FailedScrobblesDatabase* database_;

    Track* playing_scrobble_track_;

    bool restart_;

    bool scrobbling_loop_paused_;

    std::list<ScrobblingThread> scrobbling_threads_;

    std::atomic<bool> scrobbling_threads_active_;
 
};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
