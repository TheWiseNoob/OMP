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

#include "ParserAndDecoder.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Metadata/Track.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/ustring.h>

#include <gst/gst.h>

#include <iostream>

#include <sstream>

#include <string.h>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

using namespace std;





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

ParserAndDecoder::ParserAndDecoder(Base& base, Track& track_ref)

// Inherited Class
: Parts(base)

// Elements
, decoder_(nullptr)
, demuxer_(nullptr)
, parser_(nullptr)

// Plugin Names
, decoder_plugin_("")
, demuxer_plugin_("")
, parser_plugin_("")

{

  // Stores decoder's name.
  char* decoder_name;

  // Stores the parser or demuxer's name.
  char* demuxer_name;

  // Stores the parser or demuxer's name.
  char* parser_name;



  // True if the mime type is for FLAC.
  if(track_ref . mime() == "audio/flac")
  {

    debug("Inside FLAC assignemnt");



    // Assigns parser_plugin_ to flacparse.
    parser_plugin_ = "flacparse";

    // Assigns the parser a name.
    parser_name = "flac-parser";

    // Assigns decoder_plugin_ to flacdec.
    decoder_plugin_ = "flacdec";

    // Assigns the decoder a name.
    decoder_name = "flac-decoder";

  }

  // True if the mime type is for Monkey's audio.
  else if(track_ref . mime() == "audio/x-ape" 
          || track_ref . mime() == "audio/ape")
  {

    debug("Inside APE assignemnt");



    // Assigns demuxer_plugin_ to avdemux_ape.
    demuxer_plugin_ = "avdemux_ape";

    // Assigns the demuxer a name.
    demuxer_name = "ape-parser";

    // Assigns decoder_plugin_ to avdec_ape.
    decoder_plugin_ = "avdec_ape";

    // Assigns the decoder a name.
    decoder_name = "ape-decoder";

  }

  // True if the mime type is for WavPack.
  else if(track_ref . mime() == "audio/x-wavpack")
  {

    debug("Inside wavpack assignemnt");



    // Assigns parser_plugin_ to wavpackparse.
    parser_plugin_ =  "wavpackparse";

    // Assigns the parser a name.
    parser_name = "wavpack-parser";

    // Assigns decoder_plugin_ to wavpackdec.
    decoder_plugin_ = "wavpackdec";

    // Assigns the decoder a name.
    decoder_name = "wavpack-decoder";

  }

  // True if the mime type is for ALAC or AAC.
  else if(track_ref . mime() == "audio/mp4")
  {

    debug("Inside ALAC or AAC assignemnt");



    // Assigns demuxer_plugin_ to qt_demux.
    demuxer_plugin_ = "qtdemux";

    // Assigns the demuxer a name.
    demuxer_name = "mp4-demuxer";



    // True if the codec is ALAC.
    if(track_ref . codec() == "ALAC")
    {

      // Assigns decoder_plugin_ to avdec_alac.
      decoder_plugin_ = "avdec_alac";

      // Assigns the decoder a name.
      decoder_name = "alac-decoder";

    }

    // True if the codec is AAC.
    else if(track_ref . codec() == "AAC")
    {

      // Assigns decoder_plugin_ to faad.
      decoder_plugin_ = "faad";

      // Assigns the decoder a name.
      decoder_name = "aac-decoder";

    } 

  }

  // True if the mime type is for mp3.
  else if(track_ref . mime() == "audio/mpeg")
  {

    debug("Inside mp3 assignemnt");



    // Assigns parser_plugin_ to mpegaudioparse.
    parser_plugin_ = "mpegaudioparse";

    // Assigns the parser a name.
    parser_name = "mp3-parser";

    // Assigns decoder_plugin_ to mpg123audiodec.
    decoder_plugin_ = "mpg123audiodec";

    // Assigns the decoder a name.
    decoder_name = "mp3-decoder";

  }

  // True if the mime type is for ogg.
  else if(track_ref . mime() == "audio/ogg")
  {

    debug("Inside ogg assignemnt");



    // Assigns demuxer_plugin_ to oggdemux.
    demuxer_plugin_ = "oggdemux";

    // Assigns the demuxer a name.
    demuxer_name = "ogg-demuxer";



    // True if the codec is Vorbis.
    if(track_ref . codec() == "Vorbis")
    {

      // Assigns decoder_plugin_ to vorbisdec.
      decoder_plugin_ = "vorbisdec";

      // Assigns the decoder a name.
      decoder_name = "ogg-decoder";

    }

    // True if the codec is FLAC.
    else if(track_ref . codec() == "FLAC")
    {

      // Assigns parser_plugin_ to flacparse.
      parser_plugin_ = "flacparse";

      // Assigns the parser a name.
      parser_name = "flac-parser";

      // Assigns decoder_plugin_ to flacdec.
      decoder_plugin_ = "flacdec";

      // Assigns the decoder a name.
      decoder_name = "flac-decoder";

    }

  }

  // True if the mime type is not of a not supported codec.
  else
  {

    debug("Error occurred or not a valid mime type!");

  }



  stringstream debug_ss;

  debug_ss << "Decoder plugin: " << decoder_plugin_
           << "\nDecoder name: " << decoder_name;

  debug(debug_ss . str() . c_str());



  // Creates the decoder element.
  decoder_ = gst_element_factory_make (decoder_plugin_, decoder_name);

  // True if the decoder creation failed.
  if(!decoder_)
  {

    debug("Decoder assignment and creation failed!");

  }



  // Checks if a demuxer was needed.
  if(strcmp(demuxer_plugin_, "") != 0)
  {

    // Creates the demuxer element.
    demuxer_ = gst_element_factory_make (demuxer_plugin_,  demuxer_name);

    // True if the demuxer creation failed.
    if(!demuxer_)
    {

      debug("Demuxer assignment and creation failed!");

    }

  }



  // Checks if a parser was needed.
  if(strcmp(parser_plugin_, "") != 0)
  {

    // Creates the parser element.
    parser_ = gst_element_factory_make (parser_plugin_,  parser_name);

    // True if the parser creation failed.
    if(!parser_)
    {

      debug("Parser assignment and creation failed!");

    }

  }



}
