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

#ifndef PARSER_AND_DECODER_H
#define PARSER_AND_DECODER_H





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





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

//                            //
//                            //
// Class Forward Declarations /////////////////////////////////////////////////
//                            //
//                            //

class Track;





//                             //
//                             //
// Struct Forward Declarations ////////////////////////////////////////////////
//                             //
//                             //

//           //
// GStreamer //////////////////////////////////////////////////////////////////
//           //

struct _GstElement;
typedef struct _GstElement GstElement;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class ParserAndDecoder : public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Creates the parser and decoder depending on the mime type provided.
    //
    //
    //
    // Arguments: 
    //
    //   base: A reference to the Base class of the program in order to set up
    //         the quick access references in the inherited parts class.
    //
    //   mime_type: The mime type that determines the parser and decoder.
    //
    //////////////////////////////////////////////////////////////////////// */
    ParserAndDecoder(Base& base, Track& track_ref);





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //          //
  // Elements /////////////////////////////////////////////////////////////////
  //          //

  public:

    // A pointer to the decoder of the track.
    GstElement* decoder_;

    // A pointer to the demuxer of the track.
    GstElement* demuxer_;

    // A pointer to the parser of the track.
    GstElement* parser_;





  //              //
  // Plugin Names /////////////////////////////////////////////////////////////
  //              //

  public:

    // A char pointer to the decoder plugin's name.
    char* decoder_plugin_;

    // A char pointer to the parser or demuxer plugin's name.
    char* demuxer_plugin_;

    // A char pointer to the parser or demuxer plugin's name.
    char* parser_plugin_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //


#endif
