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

#ifndef METADATA_H
#define METADATA_H





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
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "TrackType.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <list>

#include <string>

#include <vector>





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

namespace Glib
{

  class ustring;

} 

namespace TagLib
{

  class AudioProperties;

  class PropertyMap;

}

class TimeConversion;

class Track;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Metadata : public Parts
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
    //   Creates the metadata class.
    //
    //
    //
    // Arguments: 
    //
    //   base: A reference to the Base class of the program in order to set up
    //         the quick access references in the inherited parts class.
    //
    //////////////////////////////////////////////////////////////////////// */
    Metadata(Base& base);





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
    //   Used to destroy any data that needs it when the metadata class ends.
    //
    //
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    ~Metadata();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //         //
  // Reading //////////////////////////////////////////////////////////////////
  //         //

  public:

    bool Determine_Codec_If_Supported(TagLib::AudioProperties& audio_prop,
                                      Track& new_track);

    void Determine_Mime_Type(const char* filename, std::string& mime_type);

    void Extract_File_Path(const std::string& filename, 
                           Glib::ustring& file_path);

    std::vector<Track*>*
      Filenames_To_Tracks(std::vector<std::string>& filenames);

  private:

    std::vector<Track*>* Interpret_Cue_Sheet(const std::string& filename);

    std::vector<Track*>* Interpret_Metadata(const std::string& filename);

    void Interpret_Multiple_Value_Tag(const char* tag_name, 
                                      TrackType type, 
                                      TagLib::PropertyMap &prop_map, 
                                      Track &track,
                                      std::vector<Glib::ustring*>& (Track::*getter)() const,
                                      void (Track::*setter)(Glib::ustring*),
                                      void (Track::*clear)());

    std::vector<Track*>* Interpret_Properties(const std::string& filename,
                                              std::vector<Track*>* new_tracks);

    void Interpret_Single_Value_Tag(const char* tag_name, 
                                    TrackType type, 
                                    TagLib::PropertyMap &prop_map, 
                                    Track &track,
                                    Glib::ustring& (Track::*getter)() const,
                                    void (Track::*setter)(Glib::ustring*));

    std::vector<Track*>* Merge(std::vector<Track*>* vector1, 
                               std::vector<Track*>* vector2);

  public:

    bool Print_Tags(const char* song_filename);

    void Print_Properties(const char* filename);





  //         //
  // Writing //////////////////////////////////////////////////////////////////
  //         //

  public:

    bool Write_Tag(int argc, char* argv[]); 





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //   

  private:

    // Used for converting time to different formats.
    TimeConversion* time_converter_;

 };





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
