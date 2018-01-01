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





#include "Parts.h"





#include "Base.h"
#include "Configuration/Configuration.h"
#include "GUI/GUI.h"
#include "Playback/Playback.h"
#include "Metadata/Metadata.h"
#include "GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h"
#include "GUI/Elements/Playlists/Playlists.h"
#include "GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h"





Parts::Parts(Base& new_base, bool debug_value)
: base_(new_base)

, debug_(debug_value)
{  

}





Base& Parts::base()
{

  return base_;

}





Configuration& Parts::config()
{ 

  return base_.config(); 

}





GUI& Parts::gui()
{  

  return base_.gui(); 

}





Playback& Parts::playback()
{

  return base_.playback();

}





Metadata& Parts::metadata()
{

  return base_.metadata();

}





int Parts::debug(char* debug_message)
{

  if(debug_)
  {

    cout << "\n\n" << debug_message << "\n\n";

  }

  else
  {

  }


  return debug_;

}





int Parts::debug(const char* debug_message)
{

  return debug(const_cast<char*>(debug_message));

}





ConfigurationGUIs& Parts::config_guis()
{

  return base_.gui().config_guis();

}





Playlists& Parts::playlists()
{

  return base_.gui().playlists();

}





PlaylistComboBoxes& Parts::playlist_comboboxes()
{

  return base_.gui().playlist_comboboxes();

}





FileChoosers& Parts::file_choosers()
{

  return base_.gui().file_choosers();

}





TimeConversion& Parts::time_converter()
{

  return base_.time_converter();

}

