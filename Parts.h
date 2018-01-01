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





#ifndef PARTS_H
#define PARTS_H





#include <list>



class Base;
class Configuration;
class Flags;
class GUI;
class Playback;
class Metadata;
class ConfigurationGUIs;
class Playlists;
class PlaylistComboBoxes;
class FileChoosers;
class TimeConversion;





class Parts
{

  protected:

    Parts(Base& base, bool debug_value = false);

  public:

    virtual ~Parts() = 0;

    virtual Base& base() final;

    virtual Configuration& config() final;

    virtual GUI& gui() final;

    virtual Playback& playback() final;

    virtual Metadata& metadata() final;

    virtual int debug(char* debug_message) final;

    virtual int debug(const char* debug_message) final;

    virtual ConfigurationGUIs& config_guis();

    virtual Playlists& playlists();

    virtual PlaylistComboBoxes& playlist_comboboxes();

    virtual FileChoosers& file_choosers();

    virtual TimeConversion& time_converter();
    
  private:

    Base& base_;

    bool debug_;

};






inline Parts::~Parts()
{

}





#endif
