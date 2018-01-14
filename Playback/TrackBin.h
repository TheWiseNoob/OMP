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

#ifndef TRACK_BIN_H
#define TRACK_BIN_H





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





//                  //
//                  //
// Required Headers ///////////////////////////////////////////////////////////
//                  //
//                  //

#include <atomic>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

//                              //
//                              //
// Class Forward Declarations /////////////////////////////////////////////////
//                              //
//                              //

class Base;

namespace Gtk
{

  class TreeRowReference;

}

class Track;





//                              //
//                              //
// Struct Forward Declarations ////////////////////////////////////////////////
//                              //
//                              //

struct _GstElement;
typedef struct _GstElement GstElement;

struct _GstPad;
typedef struct _GstPad GstPad;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class TrackBin : public Parts
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
    //   The constructor for a TrackBin. It takes all of the necessary data to
    //   make a TrackBin. 
    //
    //
    //
    // Arguments: 
    //
    //   base: A reference to the Base class of the program in order to set up
    //         the quick access references in the inherited Parts class.
    //
    //
    //   new_track: A pointer with a new-created object that will be assigned
    //              to the TrackBin's track_ variable.  
    //
    //   new_track_bin: A bin created that converts the track file to data
    //                  usable by the playback pipeline.
    //
    //   new_track_row_ref: A TreeRowReference to the row of the track. 
    //
    //   new_source: The source element of the TrackBin. It gets the file.
    //
    //   new_parser: Parsers the file to be usable by the decoder if necessary.
    //
    //   new_decoder: Decodes the track.
    //
    //   new_demuxer: Demuxes the file if necessary.
    //
    //   new_conv: Converts the decoded track to an audio format usable by the
    //             playback pipeline only if it is not already.
    //
    //   new_queue: Used to accumulate data ready to be sent to the pipeline.
    //
    //////////////////////////////////////////////////////////////////////// */
    TrackBin(Base& base, 
             Track* new_track, 
             GstElement* new_track_bin, 
             Gtk::TreeRowReference new_track_row_ref,
             GstElement* new_source, 
             GstElement* new_parser, 
             GstElement* new_decoder, 
             GstElement* new_demuxer,
             GstElement* new_conv,
             GstElement* new_queue);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Deletes data in the TrackBin that needs it when the class is ending.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    virtual ~TrackBin();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //                       //
  // Pipeline Manipulators ////////////////////////////////////////////////////
  //                       //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Attaches the TrackBin to the playback pipeline.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Attach();

    /* /////// /////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Detaches the TrackBin from the playback pipeline.
    //
    //
    //
    // Arguments: 
    //
    //  None. 
    //
    //////////////////////////////////////////////////////////////////////// */
    void Detach();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //           //
  // Bin Parts ////////////////////////////////////////////////////////////////
  //           //

  public:

    // Returns the audioconverter element of the TrackBin.
    GstElement* conv();

    // Returns the decoder of the TrackBin.
    GstElement* decoder();

    // Returns the demuxer element of the TrackBin.
    GstElement* demuxer();

    // Returns the parser element of the TrackBin.
    GstElement* parser();

    // Returns the queue element of the TrackBin.
    GstElement* queue();

    // Returns the source element of the TrackBin.
    GstElement* source();

    // Returns the track GstBin element of the TrackBin.
    GstElement* track_bin();





  //      //
  // Pads /////////////////////////////////////////////////////////////////////
  //      //

  public:

    // Gets the pointer to the connected sinkpad of the concat element.
    GstPad* concat_request_sinkpad();





  //                  //
  // State Indicators /////////////////////////////////////////////////////////
  //                  //

  public:

    // Returns the seeked state indicator.
    std::atomic<bool>& seeked();





  //            //
  // Track Data ///////////////////////////////////////////////////////////////
  //            //

  public:

    // Returns the row iterator to the track in the playlist.
    Gtk::TreeRowReference& row_ref();

    // Returns the Track information data structure.
    Track& track();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //           //
  // Bin Parts ////////////////////////////////////////////////////////////////
  //           //

  private:

    // Pointer to the connected sinkpad of the concat element if attached.
    GstPad* concat_request_sinkpad_;

    // Pointer to the converter of the TrackBin.
    GstElement* conv_;

    // Pointer to the decoder of the TrackBin.
    GstElement* decoder_;

    // Pointer to the parser of the TrackBin.
    GstElement* demuxer_;

    // Pointer to the parser of the TrackBin.
    GstElement* parser_;

    // Pointer to the queue of the TrackBin.
    GstElement* queue_;

    // Pointer to the source of the TrackBin.
    GstElement* source_;

    // Pointer to the bin of the TrackBin.
    GstElement* track_bin_;





  //                  //
  // State Indicators /////////////////////////////////////////////////////////
  //                  //

  private:

    // Stores whether the track has been seeked or not if it needs it.
    std::atomic<bool> seeked_;





  //            //
  // Track Data ///////////////////////////////////////////////////////////////
  //            //
   
  private:

    // Pointer to an iterator for the track's row.
    Gtk::TreeRowReference* row_ref_;

    // Pointer to the information data structure.
    Track* track_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
