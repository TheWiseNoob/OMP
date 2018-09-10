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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef PLAYLIST_COLUMN_H
#define PLAYLIST_COLUMN_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <string>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

namespace Gtk
{

  class TreeViewColumn;

}





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class PlaylistColumn
{ 

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    PlaylistColumn()
    : editable_(false)
    {

    }





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    virtual ~PlaylistColumn()
    {

    }





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    bool editable_;

    std::string name_;

    std::string title_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif 
