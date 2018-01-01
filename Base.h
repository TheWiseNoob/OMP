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





#ifndef BASE_H
#define BASE_H





#include <atomic>



class Configuration;
class Scrobbling;
class Metadata;
class Playback;
class GUI;
class TimeConversion;

namespace Gtk
{

  class Application;

}





class Base
{

  public:

    Base(int argc, char* argv[]);

    ~Base();

    Configuration& config();

    Scrobbling& scrobbling()
    { 

      return *scrobbling_; 

    }

    Metadata& metadata()
    { 

      return *metadata_; 

    }

    Base& main()
    { 

      return *this; 

    }

    GUI& gui();

    Playback& playback();

    TimeConversion& time_converter()
    { 

      return *time_converter_; 

    }

    std::atomic<bool>& quitting()
    { 

      return quitting_;

    }

    void set_quitting(bool new_setting)
    { 

      quitting_.store(new_setting, std::memory_order_relaxed); 

    }

    void Quit();

  private:

    bool* arg_used_;

    Configuration* config_;

    Metadata* metadata_;

    std::atomic<bool> quitting_;

    Scrobbling* scrobbling_;

    Playback* playback_;

    GUI* gui_;

    TimeConversion* time_converter_;

};





#endif
