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

#include "Parts.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "Base.h"

#include "Configuration/Configuration.h"

#include "GUI/Elements/Abouts/Abouts.h"

#include "GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h"

#include "GUI/Elements/Playlists/Playlists.h"

#include "GUI/GUI.h"

#include "Metadata/Metadata.h"

#include "Playback/Playback.h"

#include "Scrobbling/Scrobbling.h"





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

Parts::Parts(Base& new_base, bool debug_value)

// Inherited Class

: base_(new_base)



// Other

, debug_(debug_value)

{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

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





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

Abouts& Parts::abouts()
{

  return base_ . gui() . abouts();

}

Base& Parts::base()
{

  return base_;

}

Configuration& Parts::config()
{ 

  return base_.config(); 

}

ConfigurationGUIs& Parts::config_guis()
{

  return base_.gui().config_guis();

}

FileChoosers& Parts::file_choosers()
{

  return base_.gui().file_choosers();

}

GUI& Parts::gui()
{  

  return base_.gui(); 

}

Metadata& Parts::metadata()
{

  return base_.metadata();

}

Playback& Parts::playback()
{

  return base_.playback();

}

PlaylistComboBoxes& Parts::playlist_comboboxes()
{

  return base_.gui().playlist_comboboxes();

}

Playlists& Parts::playlists()
{

  return base_.gui().playlists();

}

Scrobbling& Parts::scrobbling()
{

  return base_ . scrobbling();

}

TimeConversion& Parts::time_converter()
{

  return base_.time_converter();

}
