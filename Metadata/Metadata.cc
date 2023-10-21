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

#include "Metadata.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Configuration/Configuration.h"

#include "../Errors/Errors.h"

#include "TimeConversion.h"

#include "Track.h"

#include "CueSheet.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

//#include <apeproperties.h>

#include <filesystem>

#include <fstream>

#include <gio/gio.h>

#include <glibmm.h>

#include <iostream>

#include <locale>

#include <mp4file.h>

#include <mp4properties.h>

#include <oggflacfile.h>

#include <vorbisfile.h>

#include <taglib/audioproperties.h>

#include <taglib/fileref.h>

#include <taglib/tpropertymap.h>

#include <taglib/tstring.h>

#include <sstream>

#include <stdio.h>

#include <string.h>

#include <wavpackproperties.h>





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

Metadata::Metadata(Base& base)

// Inherited Class

: Parts(base)



// Member Variables

, time_converter_(new TimeConversion)

{

} 





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Metadata::~Metadata()
{

  // Deletes the memory for the time conversion class instance.
  delete time_converter_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//         //
// Reading ////////////////////////////////////////////////////////////////////
//         //

void Metadata::All_Files_In_All_Folders
  (string& folder_str_ref, vector<string>& files,
   string& active_filename_str, mutex& active_filename_str_mutex)
{

  // Holds the filenames that haven't been sorted.
  vector<string> unsorted_files;

  // Holds the filenames that have been sorted.
  vector<string> sorted_folders;

  // Creates a C++ path variable.
  filesystem::path folder_path(folder_str_ref);



  // Gets the contents of the current folder.
  auto folder_contents = filesystem::directory_iterator(folder_path);

  // Iterates through all of the files of the current folder.
  for(auto file : folder_contents)
  {

    // Current filename.
    string path_string = file . path() . string();



    // Pushes the current filename to the back of the unsorted files vector.
    unsorted_files . push_back(path_string);

  }



  // Sorts the unsorted files.
  sort(unsorted_files . begin(), unsorted_files . end());

  // Iterates through the now sorted files and deparates files from folders.
  for(auto sorted_file : unsorted_files)
  {

    // True if the current filename is to a folder.
    if(filesystem::is_directory(sorted_file))
    { 

      // Adds the folder to the sorted folders vector.
      sorted_folders . push_back(sorted_file);

    } 

    // True if the current filename is a file.
    else
    { 

      // Adds the file to the files list.
      files . push_back(sorted_file);



      // Locks the mutex for the active filename.
      active_filename_str_mutex . lock();

      // Sets the active filename.
      active_filename_str = sorted_file;

      // Unlocks the mutex for the active filename.
      active_filename_str_mutex . unlock();

    }

  } 



  // Marks the end of the files group for a folder.
  files . push_back("END_GROUP");



  // Recursive call to the current function for all folders.
  for(auto sorted_folder : sorted_folders)
  { 

    // Calls this function again for one of the folders in the current folder.
    All_Files_In_All_Folders(sorted_folder, files, 
                             active_filename_str, active_filename_str_mutex);

  }

}

bool Metadata::Determine_If_Codec_Supported
  (TagLib::AudioProperties& audio_prop, Track& new_track)
 {

  // Holds the mime type of the track.
  string mime_type;

  // Determines the mime type and stores it in mime_type
  Determine_Mime_Type(new_track . filename() . data(), mime_type);

  // Sets the mime type of the new track.
  new_track . set_mime(mime_type);



  // True if the mime type is for FLAC.
  if(mime_type == "audio/flac")
  {

    // Retrieves the Properties of the new track.
    TagLib::FLAC::Properties& flac_prop_map
      = dynamic_cast<TagLib::FLAC::Properties&>(audio_prop);



    // Stores the determined bit depth.
    new_track . set_bit_depth(flac_prop_map . bitsPerSample());



    // Sets the codec of the new track.
    new_track . set_codec("FLAC");

  }
/*
  // True if the mime type is for Monkey's audio.
  else if((mime_type == "audio/x-ape") || (mime_type == "audio/ape"))
  {

    // Retrieves the Properties of the new track.
    TagLib::APE::Properties& ape_prop_map
      = dynamic_cast<TagLib::APE::Properties&>(audio_prop);



    // Stores the determined bit depth.
    new_track . set_bit_depth(ape_prop_map . bitsPerSample());



    // Sets the codec of the new track.
    new_track . set_codec("Monkey's Audio");

  }
*/
  // True if the mime type is for WavPack.
  else if(mime_type == "audio/x-wavpack")
  {

    // Retrieves the Properties of the new track.
    TagLib::WavPack::Properties& wavpack_prop_map
      = dynamic_cast<TagLib::WavPack::Properties&>(audio_prop);



    // Stores the determined bit depth.
    new_track . set_bit_depth(wavpack_prop_map . bitsPerSample());



    // Sets the codec of the new track.
    new_track . set_codec("WavPack");

  }

  // True if the mime type is for ALAC or AAC.
  else if(mime_type == "audio/mp4")
  { 

    // Retrieves the Properties of the new track.
    TagLib::MP4::Properties& mp4_prop_map
      = dynamic_cast<TagLib::MP4::Properties&>(audio_prop);



    // Stores the determined bit depth.
    new_track . set_bit_depth(mp4_prop_map . bitsPerSample());



    // Holds an int that indicates what codec the new track is.
    int codec_int = mp4_prop_map . codec();



    // True if the codec is AAC.
    if(codec_int == 1)
    { 

      debug("AAC");



      // Sets the codec of the new track.
      new_track . set_codec("AAC");

    }

    // True if the codec is ALAC.
    else if(codec_int == 2)
    {  

      debug("ALAC");



      // Sets the codec of the new track.
      new_track . set_codec("ALAC");

    }

    // True if no valid codec was determined.
    else
    {

      // A string to hold the error message.
      string error_message_str = "Failed to identify the codec of ";

      // Adds to the error message.
      error_message_str += new_track . filename();

      // Adds to the error message.
      error_message_str += ".";



      // Writes to the error message log.
      errors() . Write_Error(error_message_str . c_str());



      // Returns false because a valid codec was unable to be determined.
      return false;

     }

  }

  // True if the mime type is for mp3.
  else if(mime_type == "audio/mpeg")
  { 

    // Sets the codec of the new track.
    new_track . set_codec("MP3");

  }

  // True if the mime type is for AAC.
  else if(mime_type == "audio/aac")
  { 

    // Creates an MP4 File to see if it is pure AAC or in an mp4 container.
    TagLib::MP4::File mp4_file(new_track . filename() . c_str());



    // True if the file is determined to have an mp4 container.
    if(mp4_file .  isValid())
    {

      // Sets the mime type as mp4.
      new_track . set_mime("audio/mp4");

    }



    // Sets the codec of the new track.
    new_track . set_codec("AAC");

  }

  // True if the mime type is for ogg.
  else if((mime_type == "audio/ogg") || (mime_type == "audio/oga"))
  { 

    // Creates an Ogg FLAC file to help determine the codec.
    TagLib::Ogg::FLAC::File ogg_flac_file(new_track . filename() . c_str());

    // Creates an Ogg Vorbis file to help determine the codec.
    TagLib::Ogg::Vorbis::File ogg_vorbis_file
      (new_track . filename() . c_str());



    // True if the file is Ogg FLAC.
    if(ogg_flac_file . isValid())
    {

      // Sets the codec of the new track.
      new_track . set_codec("FLAC");



      // Stores the determined bit depth.
      new_track
        . set_bit_depth(ogg_flac_file . audioProperties() -> bitsPerSample());

    }

    // True if the file is Ogg Vorbis.
    else if(ogg_vorbis_file . isValid())
    { 

      // Sets the codec of the new track.
      new_track . set_codec("Vorbis");

    } 

    // True if the Ogg file is not a valid codec.
    else
    {

      // A string to hold the error message.
      string error_message_str = "Failed to identify a valid codec for ";

      // Adds to the error message.
      error_message_str += new_track . filename();

      // Adds to the error message.
      error_message_str += ".";



      // Writes to the error message log.
      errors() . Write_Error(error_message_str . c_str());



      // Returns false because a valid codec was unable to be determined.
      return false;

    }

  } 

  // True if the mime type is not of a not supported codec.
  else
  {

    debug("Error occurred or not a valid mime type!");



    // A string to hold the error message.
    string error_message_str
      = "Invalid or unsupported mime type for filename ";

    // Adds to the error message.
    error_message_str += new_track . filename();

    // Adds to the error message.
    error_message_str += ".";



    // Writes to the error message log.
    errors() . Write_Error(error_message_str . c_str());



    // Returns false because a valid codec was unable to be determined.
    return false;

  } 



  // Indicates the determination of the track's codec was successful.
  return true;

}

bool Metadata::Determine_If_Mime_Supported(const string& mime)
{

  // True if the mime type is for FLAC.
  if(mime == "audio/flac")
  {

    // Returns true because the mime type is supported.
    return true;

  }

  // True if the mime type is for a cuesheet file.
  else if((mime == "application/x-cue") || (mime == "application/cue"))
  { 

    // Returns true because the mime type is supported.
    return true;

  }

  // True if the mime type is for Monkey's audio.
  else if((mime == "audio/x-ape") || (mime == "audio/ape"))
  {

    // Returns true because the mime type is supported.
    return true;

  }

  // True if the mime type is for WavPack.
  else if(mime == "audio/x-wavpack")
  {

    // Returns true because the mime type is supported.
    return true;

  }

  // True if the mime type is for ALAC or AAC.
  else if(mime == "audio/mp4")
  {

    // Returns true because the mime type is supported.
    return true;

  }

  // True if the mime type is for mp3.
  else if(mime == "audio/mpeg")
  {

    // Returns true because the mime type is supported.
    return true;

  }

  // True if the mime type is for AAC.
  else if(mime == "audio/aac")
  {

    // Returns true because the mime type is supported.
    return true;

  }

  // True if the mime type is for ogg.
  else if((mime == "audio/ogg") || (mime == "audio/oga"))
  {

    // Returns true because the mime type is supported.
    return true;

  }

  // 
  else
  {

    // Returns false because the mime type is not supported.
    return false;

  }

}

void Metadata::Determine_Mime_Type(const char* filename, string& mime_type)
{

  // Stores whether the guessed content type is certain.
  gboolean is_certain = false;

  // Guesses the content type of the file.
  char* content_type = g_content_type_guess(filename, NULL, 0, &is_certain);



  // Char pointer that will point to the determined mime type.
  char* mime_type_temp;

  // Uses the guessed content type to determine the mime type and assigns it 
  // to a pointer.
  mime_type_temp = g_content_type_get_mime_type(content_type);

  // Assigns the mime type to the referenced std::string.
  mime_type = mime_type_temp;



  // Frees the memory of the guessed content type.
  g_free(content_type);

  // Frees the memory of the mime type char pointer.
  g_free(mime_type_temp);

}

void Metadata::Extract_File_Path
  (const string& filename, Glib::ustring& file_path)
{

  // Will hold the last index of the end of the cuesheet's path.
  int end_index;

  // Holds the length of the cuesheet's filename.
  int length = filename . length();



  // Finds the index of the the last character of the cuesheet's filepath.
  for(end_index = length - 5; filename[end_index] != '/'; end_index--);


         
  // Stores the path of the cuesheet.
  for(int i = 0; i <= end_index; i++)
  {

    // Stores the current character of the cuesheet path.
    file_path.push_back(filename[i]);

  }

}

vector<Track*>* Metadata::Filenames_To_Tracks
  (vector<string>& filenames, string& active_filename_str,
   mutex& active_filename_str_mutex, atomic<int>& reading_files_count,
   atomic<bool>& quitting)
{

  // New-created vector pointer of new-created Track pointers.
  vector<Track*>* tracks = new vector<Track*>;

  // Holds the tracks not created from a cuesheet.
  vector<pair<Track*, int>> single_tracks;

  // Holds the tracks created from an external cuesheet.
  vector<vector<pair<Track*, int>>> external_cue_tracks_tracks;

  // Holds the tracks created from an embedded cuesheet.
  vector<vector<pair<Track*, int>>> embedded_cue_tracks_tracks;

  // Holds the track count for each group of files.
  int group_files_count = 0;



  // Iterates through the vector of filenames to create a Track or Tracks for
  // each file.
  for(auto filename : filenames)
  {

    // True if the the function needs to quit.
    if(quitting)
    {  

      // Iterates through all of the single tracks.
      for(auto single_track_pair : single_tracks)
      {

        // Deletes the Track.
        delete (single_track_pair . first);

      }



      // Iterates through all of the embedded cue tracks vectors.
      for(auto embedded_cue_tracks : embedded_cue_tracks_tracks)
      {

        // Iterates through the current embedded tracks.
        for(auto embedded_cue_track_pair : embedded_cue_tracks)
        {

          // Deletes the Track.
          delete (embedded_cue_track_pair . first);

        }

      }



      // Iterates through all of the external cue tracks vectors.
      for(auto external_cue_tracks : external_cue_tracks_tracks)
      {

        // Iterates through the current external tracks.
        for(auto external_cue_track_pair : external_cue_tracks)
        {

          // Deletes the Track.
          delete (external_cue_track_pair . first);

        }

      }



      // Returns the tracks vector.
      return tracks;

    }



    // Skips folders.
    if(filename . back() == '/')
    {

      // Continues to the next iteration of the loop.
      continue;

    }

    // End of all music files in a folder.
    else if(filename == "END_GROUP")
    {

      // Removes any duplicates files in the three vectors and combines them.
      Remove_Duplicates(tracks, single_tracks, external_cue_tracks_tracks,
                        embedded_cue_tracks_tracks, (group_files_count + 1));



      // Clears the single tracks vector.
      single_tracks . clear();

      // Clears the external cuesheet tracks vector.
      external_cue_tracks_tracks . clear();

      // Clears the embedded cuesheet tracks vector.
      embedded_cue_tracks_tracks . clear();

      // Resets the group file count.
      group_files_count = 0;



      // Continues to the next file.
      continue;

    }



    // Locks the mutex for the active filename.
    active_filename_str_mutex . lock();

    // Sets the active filename.
    active_filename_str = filename;

    // Unlocks the mutex for the active filename.
    active_filename_str_mutex . unlock();



    // Holds the newly created tracks.
    vector<Track*>* new_tracks;

    // Attempts to open the file.
    try
    {

      // Interprets the metadata of the file provided.
      new_tracks = Interpret_Metadata(filename);

    }

    // True if an exception occurs while attempting
    // to read the file's metadata.
    catch(...)
    {

      // Sets the new tracks to nullptr.
      new_tracks = nullptr;



      debug("Exception caught in NORMAL track metadata retrieval");



      // Adds to the error message.
      string error_message_str = "Failed to open filename ";

      // Adds to the error message.
      error_message_str += filename;

      // Adds to the error message.
      error_message_str += ".";

      // Writes the error message.
      errors() . Write_Error(error_message_str . c_str());

    }



    // True if the was no Track vector created. Nothing is done.
    if(new_tracks == nullptr)
    {

    }

    // True if an empty Track vector was created.
    else if(new_tracks -> empty())
    {

      // Deletes the empty Track vector.
      delete new_tracks;

    }

    // 
    else
    {

      // True if the Track vector was created from a cuesheet.
      if(new_tracks -> front() -> Cue())
      {

        // Creates a new vector for Track ptrs and an order int.
        vector<pair<Track*, int>> temp_track_pairs;



        // Iterates through the Track pointers.
        for(auto new_track_ptr : *new_tracks)
        {

          // Adds the new tracks Track ptr to the new ordered Track* vector.
          temp_track_pairs . emplace_back(new_track_ptr, group_files_count++);



          // Increases the group file count.
          group_files_count++;

        }



        // True if the tracks were created from an embedded cuesheet.
        if(new_tracks -> front() -> Cue_Embedded())
        {

          // Adds the Track vector to the embedded cuesheet tracks vector.
          embedded_cue_tracks_tracks . push_back(temp_track_pairs);

        }

        // True if the tracks were created from an external cuesheet.
        else
        {

          // Adds the Track vector to the external cuesheet tracks vector.
          external_cue_tracks_tracks . push_back(temp_track_pairs);

        }

      }

      // True if the Track vector was created from a normal music file.
      else
      {

        // Iterates through the new tracks Track vector.
        for(auto new_track_ptr : *new_tracks)
        {

          // Adds the new Track to the single tracks vector with an order.
          single_tracks . emplace_back(new_track_ptr, group_files_count);



          // Increases the group file count.
          group_files_count++;

        }

      }

    }



    // Increases the reading files count.
    reading_files_count++;

  }



  // Removes duplicates from the final files added.
  Remove_Duplicates(tracks, single_tracks, external_cue_tracks_tracks,
                    embedded_cue_tracks_tracks, group_files_count + 1);



  // Returns the new-created vector of tracks.
  return tracks;

 }

std::vector<Track*>* Metadata::Interpret_Cue_Sheet(const std::string& filename)
{

  // Holds the CUE SHEET interpreter created only once using static.
  static CueSheet cue_sheet_interpreter;

  // Sets the global locale to the computer's default.
  setlocale(LC_ALL, "en_US.UTF-8");



  // Gets the contents of the file of the filename provided.
  Glib::ustring temp_string = Glib::file_get_contents(filename); 

  // Used to store the converted to UTF CUESHEET string.
  Glib::ustring temp_cue_sheet;



  debug("After file_get_contents");



  // Attempted to convert the CUE SHEET string to a ustring.
  try
  { 

    // Converts the CUE SHEET to a ustring.
    temp_cue_sheet = Glib::locale_to_utf8(temp_string . data());

  }

  // Catches the error if conversion fails.
  catch(Glib::ConvertError error)
  { 

    // Switches through the error types to determine which one occurred.
    switch(error . code())
    { 

      // True if the conversion between character sets is not supported.
      case Glib::ConvertError::Code::NO_CONVERSION:
      {

        debug("No conversion possible between these formats!");



        // Exits the switch.
        break;

      }

      // True if the encoding is possibly wrong.
      case Glib::ConvertError::Code::ILLEGAL_SEQUENCE:
      {

        debug("Illegal sequence!");



        // Sets the initial codeset to LATIN-1.
        string codeset_str = "LATIN-1";

        // Iterates through all of the LATIN codesets until the correct one is
        // found or it fails to find the correct one.
        while(true)
        {

          // Attempts using the current LATIN codeset.
          try
          {

            // Converts from LATIN-X to UTF-8.
            temp_cue_sheet
              = Glib::convert(temp_string, "UTF-8", codeset_str . c_str());



            // Breaks from the loop because the correct codeset was found.
            break;

          }

          // Occurs if the codeset conversion failed.
          catch(...)
          {

            // True if none of the cuesheet conversion was none of the
            // LATIN codesets.
            if(codeset_str == "LATIN-9")
            {

              // A string to hold the error message.
              string error_message_str = "Could not read the cuesheet ";

              // Adds to the error message.
              error_message_str += filename;

              // Adds to the error message.
              error_message_str 
                += ".\n     Charset conversation could not achieved.";



              // Writes to the error message log.
              errors() . Write_Error(error_message_str . c_str());



              // Breaks from the loop.
              break;

            }
   
            // True if the last attempted codeset conversion was LATIN-7.
            else if(codeset_str == "LATIN-7")
            {

              // Sets the codeset.
              codeset_str = "LATIN-9";

            }

            // True if the last attempted codeset conversion was LATIN-5.
            else if(codeset_str == "LATIN-5")
            {

              // Sets the codeset.
              codeset_str = "LATIN-7";

            }

            // True if the last attempted codeset conversion was LATIN-3.
            else if(codeset_str == "LATIN-3")
            {

              // Sets the codeset.
              codeset_str = "LATIN-5";

            }

            // True if the last attempted codeset conversion was LATIN-2.
            else if(codeset_str == "LATIN-2")
            {

              // Sets the codeset.
              codeset_str = "LATIN-3";

            }

            // True if the last attempted codeset conversion was LATIN-1.
            else if(codeset_str == "LATIN-1")
            {

              // Sets the codeset.
              codeset_str = "LATIN-2";

            }

          }

        }



        // Exits the switch.
        break;

      }

      // 
      default:
      { 

        debug("Conversion not identified!");



        // A string to hold the error message.
        string error_message_str = "Could not read the cuesheet ";

        // Adds to the error message.
        error_message_str += filename;

        // Adds to the error message.
        error_message_str 
          += ".\n     The reason could not be determined.";



        // Writes to the error message log.
        errors() . Write_Error(error_message_str . c_str());



        // Return an new empty Track pointer vector ptr.
        return new vector<Track*>;

      }

    }

  }



  debug("After locale to utf8");



  // Stores the path of the cuesheet. 
  Glib::ustring file_path;

  // Extracts the file path.
  Extract_File_Path(filename, file_path);



  // Interprets the cue sheet.
  return cue_sheet_interpreter(&temp_cue_sheet, file_path, filename);

}

std::vector<Track*> *Metadata::Interpret_Metadata(const std::string& filename)
{

  // Holds the mime type.
  string mime;

  // Determines the mime type and stores it in mime.
  Determine_Mime_Type(filename . c_str(), mime);



  // True if the mime type is determined to be NOT supported.
  if(!Determine_If_Mime_Supported(mime))
  {

    // Returns a nullptr and exits the function.
    return nullptr;

  }



  // String that will contain the mime type.
  string mime_type;

  // Determines the mime type and assigns it to the string.
  Determine_Mime_Type(filename . c_str(), mime_type);
 


  // True if the file is a CUE SHEET file.
  if((mime_type == "application/x-cue") || (mime_type == "application/cue"))
  {

    // Interprets the CUE SHEET and assigns adds the Tracks to a new vector. 
    vector<Track*>* cue_sheet_tracks = Interpret_Cue_Sheet(filename);



    // Interprets the audio properties of the CUE SHEET tracks.
    Interpret_Properties(string(""), cue_sheet_tracks);



    // Returns the tracks created from interpreting the CUE SHEET.
    return cue_sheet_tracks;

  }



  // Interprets the properties of a NORMAL track.
  return Interpret_Properties(filename, NULL);

}

void Metadata::Interpret_Multiple_Value_Tag
  (const char* tag_name, TrackType type, TagLib::PropertyMap& prop_map, 
   Track& track, vector<Glib::ustring*>& (Track::*getter)() const,
   void (Track::*setter)(Glib::ustring*), void (Track::*clear)() = NULL)
{

  // Checks if that tag provided is present in the metadata.
  if(!(prop_map . contains(TagLib::String(tag_name))))
  { 

    debug("Tag is not present. Returning!");



    // Ends interpreting of the current tag.
    return;

  }



  // Interprets the tag normally if the track is of type NORMAL.
  if(type == TrackType::NORMAL)
  {

    // Iterates through the multiple indices of the tag if present.
    for(auto it : prop_map[TagLib::String(tag_name)])
    {

      // Adds the tag to the list of entries in the multiple-value tag.
      (track.*setter)(new Glib::ustring(it . to8Bit(true)));

     }

  } 

  // True if the track was created from a cuesheet.
  else
  {

    // True if the property is not empty.
    if(((prop_map[TagLib::String(tag_name)] . size() > 1))
         || 
       ((track.*getter)() . empty()))
     {

      // True if the clear function is NULL.
      if(clear == NULL)
      { 

        // Return the multiple value track tag reading function.
        return;

      }



      // Clears the multiple value Track tag.
      (track.*clear)(); 



      debug("Multiple values!");



      // Iterates through the new Track tag.
      for(auto it : prop_map[TagLib::String(tag_name)])
      {

        // Adds the new track tag to the new Track instance.
        (track.*setter)(new Glib::ustring(it . to8Bit(true)));

       }

    }

  } 



  // Erases the track tag from the property map.
  prop_map . erase(TagLib::String(tag_name));

}

std::vector<Track*> *Metadata::Interpret_Properties
  (const std::string& filename = "", vector<Track*>* new_tracks = nullptr)
{ 

  // Sets the track type to normal until determined otherwise.
  TrackType type = TrackType::NORMAL;

  // Holds the property map of the file, which contains metadata.
  TagLib::PropertyMap temp_prop_map;



  // True if the file provided is not a cuesheet that has already been read. 
  if(new_tracks == nullptr)
  { 

    // Creates a new vector for holding Track pointers.
    new_tracks = new vector<Track*>;



    // Used for handling files.
    TagLib::FileRef file_ref(filename . c_str());

    // Determines if the file-ref is NULL or not.
    if(file_ref . isNull())
    {

      debug("File ref null");



      // Stores the mime type.
      string mime_type;

      // Determines and stores the mime type in mime_type.
      Determine_Mime_Type(filename . c_str(), mime_type);



      // Determines the mime type of the file.
      if(!Determine_If_Mime_Supported(mime_type))
      {

        // A string to hold the error message.
        string error_message_str = "Failed to open ";

        // Adds to the error message.
        error_message_str += filename;

        // Adds to the error message.
        error_message_str += ".";

        // Adds to the error message.
        error_message_str += " \n     The mime type is not supported or invalid.";



        // Writes to the error message log.
        errors() . Write_Error(error_message_str . c_str());



        // Returns an empty new-created vector.
        return new_tracks;

      }



      // True if the mime type and file extension is .aac.
      if(mime_type == "audio/aac")
      {

        // Creates an mp4 TagLib File.
        TagLib::MP4::File mp4_file(filename . c_str());



        // Pointer for the new track file.
        Track* new_track;



        // If the mp4 file is valid, the aac file has the wrong extension.
        if(mp4_file .  isValid())
        {

          debug("Mislabeled MP4 AAC as AAC (.aac).");



          // A string to hold the error message.
          string error_message_str = "The file ";

          // Adds to the error message.
          error_message_str += new_track -> filename();

          // Adds to the error message.
          error_message_str
            += " is labelled as pure AAC, but it is AAC in an mp4 container." \
               "\n     Rename the extension to .mp4 to make it work.";



          // Writes to the error message log.
          errors() . Write_Error(error_message_str . c_str());



          // Returns the empty Track pointer vector pointer.
          return new_tracks;

        }

        // The file has been determined to be pure AAC.
        else
        {

          // Creates a new Track instance.
          new_track = new Track;

          // Pushes a new empty track into the vector.
          new_tracks -> push_back(new_track);

  

          // Sets the mime type.
          new_track -> set_mime("audio/aac");

          // Sets the codec.
          new_track -> set_codec("AAC");

          // Sets the filename.
          new_track -> set_filename(filename);



          // Returns the new Track pointer vector pointer.
          return new_tracks;

        }

      }

    } 

    // True if the FileRef is valid.
    else
    {

      // Extracts the property map from the file ref.
      temp_prop_map = (file_ref . file() -> properties());
   
  
  
      // Checks if the property map contains a cuesheet.
      if(temp_prop_map . contains(TagLib::String("CUESHEET")))
      {
  
        debug("Property map contains a cuesheet!");
  
  
  
        // Creates a static cuesheet interpreter so it only needs created once.
        static CueSheet cue_sheet_interpreter;
  
  
  
        // Holds the file path to be used by the cue sheet interpreter.
        Glib::ustring file_path;
  
        // Extracts the file path.
        Extract_File_Path(filename, file_path);
  
  
  
        stringstream debug_ss;
  
        debug_ss << "filename beginning: " << file_path;
  
        debug(debug_ss . str() . c_str());
  
  
  
        // Creates a new-created ustring pointer of the cuesheet.
        Glib::ustring* temp_ustring 
          = new Glib::ustring(temp_prop_map[TagLib::String("CUESHEET")]
                                              . toString() . to8Bit(true));
  
        // Extracts the track data from the cuesheet.
        new_tracks = cue_sheet_interpreter(temp_ustring, file_path, "");
  
        // Destroys the cue sheet ustring.
        delete temp_ustring;



        // Sets the track type to what the cue sheet interpreter identified.
        type = new_tracks -> front() -> type();

      } 
  
      // True if the file provided does not have a cue sheet. 
      else
      {
  
        // Pushes a new empty track into the vector.
        new_tracks -> push_back(new Track);
  
      }

    }

  }  

  // True if there was a cuesheet already interpreted.
  else
  {

    // Sets the TrackType to what the cuesheet interpreter identified.
    type = new_tracks -> front() -> type();


  }



  // Finds and stores the tags of a NORMAL track.
  if(type == TrackType::NORMAL)
  {

    // Interprets the properties of the NORMAL Track.
    return Interpret_Properties_Normal_Track
             (filename, new_tracks, type, temp_prop_map);

  }

  // Handles the reading of files for a SINGLE_FILE cue sheet.
  else if(type == TrackType::SINGLE_FILE)
  {

    // 
    string filename = new_tracks -> front() -> filename();



    // 
    if(!filesystem::exists(filename))
    {

      debug("Audioproperties NULL! Unable to open file!");



      // A string to hold the error message.
      string error_message_str = "Failed to open ";

      // Adds to the error message.
      error_message_str += filename;

      // Adds to the error message.
      error_message_str += ".";

      // 
      error_message_str += "\n     The file does not exist.";



      debug("Debug Write Error");



      // Writes to the error message log.
      errors() . Write_Error(error_message_str . c_str());



      debug("After Write Error");



      // 
      if(new_tracks != nullptr)
      {

        // 
        for(auto track_it : *new_tracks)
        {

          // 
          delete track_it;

        }



        // 
        new_tracks -> clear();



        // 
        return new_tracks;

      }

    }



    // 
    TagLib::FileRef file_ref(filename . c_str()); 

    // 
    temp_prop_map = (file_ref.file() -> properties());



    //   
    if((file_ref . audioProperties()) == NULL)
    {

      debug("Audioproperties NULL! Unable to open file!");



      // A string to hold the error message.
      string error_message_str = "Failed to open ";

      // Adds to the error message.
      error_message_str += filename;



      // 
      if((new_tracks -> front() -> mime()) != "")
      {

        // Adds to the error message.
        error_message_str += ".";

        // Adds to the error message.
        error_message_str += "\n     It has a mime type of ";

        // Adds to the error message.
        error_message_str += new_tracks -> front() -> mime();

      }



      // Adds to the error message.
      error_message_str += ".";

      // 
      error_message_str += "\n     The file is damaged or has an invalid extension.";



      // Writes to the error message log.
      errors() . Write_Error(error_message_str . c_str());



      // 
      if(new_tracks != nullptr)
      {

        // 
        for(auto track_it : *new_tracks)
        {

          // 
          delete track_it;

        }



        // 
        new_tracks -> clear();



        // 
        return new_tracks;

       }

     }



    // 
    TagLib::AudioProperties &audio_properties = *(file_ref.audioProperties());

    // 
    int bit_rate = audio_properties . bitrate(),
        sample_rate = audio_properties . sampleRate(),
        channels = audio_properties . channels();

    // 
    Track& new_track = *(new_tracks -> front());



    // Determines the codec if supported.
    if(!Determine_If_Codec_Supported(audio_properties, new_track))
    {

      // 
      new_tracks -> clear();



      // 
      return new_tracks;

    }



    // 
    Glib::ustring codec = new_track . codec(); 

    // 
    Glib::ustring mime = new_track . mime();



    // 
    for(auto it : *new_tracks)
    { 

      // 
      it -> set_bit_rate(bit_rate);

      // 
      it -> set_sample_rate(sample_rate);

      // 
      it -> set_channels(channels);

      // 
      it -> set_codec(codec);

      // 
      it -> set_mime(mime);

      // 
      it -> set_bit_depth(new_track . bit_depth());

    }



    // 
    long long file_length
      = audio_properties . lengthInMilliseconds() * 1000000LL;

    // 
    int track_total = (*new_tracks)[0] -> track_total();

    // 
    long long temp_duration = 0LL;

    // 
    int disc_number = 0;

    // 
    int disc_total = 0;



    // 
    if(temp_prop_map . contains(TagLib::String("DISCNUMBER")))
    { 

      debug("Metadata: Interpret_Properties: NORMAL: DISCNUMBER");



      // 
      disc_number = stoi(temp_prop_map[TagLib::String("DISCNUMBER")]
                                         . toString() . to8Bit(true));

    }

    // 
    if(temp_prop_map . contains(TagLib::String("DISCTOTAL")))
    {

      debug("Metadata: Interpret_Properties: NORMAL: DISCTOTAL");



      // 
      disc_total = stoi(temp_prop_map[TagLib::String("DISCTOTAL")]
                                        . toString().to8Bit(true));
  
    } 



    // 
    for(int i = 0; i < track_total - 1; i++)
    {

      // 
      temp_duration
        = ((*new_tracks)[i] -> end()) - ((*new_tracks)[i] -> start());

      // 
      std::string* time_string_ptr
        = time_converter_ -> Nanoseconds_To_Time_Format(temp_duration);



      // 
      (*new_tracks)[i] -> set_length(*time_string_ptr);

      // 
      (*new_tracks)[i] -> set_duration(temp_duration);

      // 
      (*new_tracks)[i] -> set_disc_number(disc_number);

      // 
      (*new_tracks)[i] -> set_disc_total(disc_total);



      // 
      delete time_string_ptr;

    }



    // 
    temp_duration = file_length - ((*new_tracks)[track_total - 1] -> start());

    // 
    std::string* time_string_ptr
    = time_converter_ -> Nanoseconds_To_Time_Format(temp_duration);

    // 
    (*new_tracks)[track_total - 1] -> set_length(*time_string_ptr);

    // 
    (*new_tracks)[track_total - 1] -> set_duration(temp_duration);

    // 
    (*new_tracks)[track_total - 1] -> set_disc_number(disc_number);

    // 
    (*new_tracks)[track_total - 1] -> set_disc_total(disc_total);




    
    // 
    delete time_string_ptr;



    // 
    for(auto it = temp_prop_map.begin(); it != temp_prop_map.end(); it++)
    { 

      stringstream debug_ss;

      debug_ss << "\nTag: \"" << (it->first).to8Bit(true) << "\"\n";

      debug(debug_ss . str() . c_str());



      // 
      Glib::ustring tag_name = (it -> first) . to8Bit(true);



      // 
      if(tag_name == "CUESHEET")
      { 

        debug("single-file Cue track identified");

       }

    }

  } 

  //
  else
  {

    debug("MULTIPLE_FILES!");



    // 
    for(auto track_it : *new_tracks)
    {

      // 
      TagLib::FileRef file_ref(track_it -> filename() . c_str()); 



      // 
      if((file_ref . audioProperties()) == NULL)
      { 

        debug("Audioproperties NULL!");



        // A string to hold the error message.
        string error_message_str = "Failed to open ";

        // Adds to the error message.
        error_message_str += track_it -> filename();

        // Adds to the error message.
        error_message_str += ".";



        // Writes to the error message log.
        errors() . Write_Error(error_message_str . c_str());



        // 
        for(auto new_tracks_delete_it : *new_tracks)
        {

          // 
          delete new_tracks_delete_it;

        }



        // 
        new_tracks -> clear();



        // 
        return new_tracks;

      }



      // 
      TagLib::AudioProperties &audio_properties
        = *(file_ref . audioProperties());



      // Determines the codec if supported.
      if(!Determine_If_Codec_Supported(audio_properties, *track_it))
      {

        // 
        new_tracks -> clear();



        // 
        return new_tracks;

      }



      // 
      int bit_rate = audio_properties . bitrate();

      // 
      long long file_length
        = audio_properties . lengthInMilliseconds() * 1000000LL;



      // 
      track_it -> set_bit_rate(bit_rate);

      track_it -> set_sample_rate(audio_properties . sampleRate());

      track_it -> set_channels(audio_properties . channels());

      std::string* time_string_ptr
        = time_converter_ -> Nanoseconds_To_Time_Format(file_length);

      track_it -> set_length(*time_string_ptr);

      track_it -> set_duration(file_length);

      delete time_string_ptr;



      // 
      temp_prop_map = (file_ref . file() -> properties());
  


      // 
      Interpret_Single_Value_Tag("TITLE", type, temp_prop_map, *track_it, 
                                 &Track::title, &Track::set_title);

      // 
      Interpret_Multiple_Value_Tag
        ("ARTIST", type, temp_prop_map, *track_it, &Track::artists,
         &Track::add_artist, &Track::clear_artists);

      // 
      Interpret_Single_Value_Tag
        ("ALBUM", type, temp_prop_map, *track_it, 
         &Track::album, &Track::set_album);

      // 
      Interpret_Multiple_Value_Tag
        ("GENRE", type, temp_prop_map, *track_it, 
        &Track::genres, &Track::add_genre, &Track::clear_genres);



      // 
      temp_prop_map . erase(TagLib::String("TRACKNUMBER"));

      // 
      temp_prop_map . erase(TagLib::String("TRACKTOTAL"));



      // 
      if(temp_prop_map . contains(TagLib::String("DISCNUMBER")))
      {

        // 
        try
        {

          // 
          int disc_number
            = stoi(temp_prop_map[TagLib::String("DISCNUMBER")]
                                   . toString() . to8Bit(true));


          // 
          track_it -> set_disc_number(disc_number);

        }

        // 
        catch(...)
        {

        }

      }

      // 
      if(temp_prop_map.contains(TagLib::String("DISCTOTAL")))
      {

        try
        {

          // 
          int disc_total
            = stoi(temp_prop_map[TagLib::String("DISCTOTAL")]
                                   . toString() . to8Bit(true));


          // 
          track_it -> set_disc_number(disc_total);

        }

        catch(...)
        {

        }

      }


    
      //Finds individuals tags for the current track file
      for(auto it = temp_prop_map.begin(); it != temp_prop_map.end(); it++)
      {

        stringstream debug_ss;

        debug_ss << "TAG: " << ((it->first).to8Bit(true));
   
        debug(debug_ss . str() . c_str());



        // 
        track_it -> tags() . push_back
          (Tag(((it -> first) . to8Bit(true) . c_str()), 
               ((it -> second) . toString() . to8Bit(true))));

      }

    }

  }



  //  
  return new_tracks;

}

vector<Track*>* Metadata::Interpret_Properties_Normal_Track
  (const std::string& filename, std::vector<Track*>* new_tracks,
   TrackType& type, TagLib::PropertyMap& temp_prop_map)
{

  stringstream debug_ss;

  debug_ss << "filename: " << filename;

  debug(debug_ss . str() . c_str());



  // Creates a file ref for the track.
  TagLib::FileRef file_ref(filename . c_str()); 



  // True if the FileRef is NULL or its audioProperties is NULL.
  if((file_ref . isNull()) || ((file_ref . audioProperties()) == NULL))
  {

    debug("File ref NULL! Unable to open file!");



    // Iterates through all of new_tracks.
    for(auto new_tracks_it : *new_tracks)
    {

      // Deletes the Track.
      delete new_tracks_it;

    }



    // Clears the pointers in new_tracks.
    new_tracks -> clear();



    // Returns the empty new_tracks.
    return new_tracks;


  }  



  // Creates a reference to the Track having data added to it.
  Track& new_track = *(new_tracks -> front());



  // Sets the filename of the new track.
  new_track . set_filename(filename);    



  debug("Before file_ref");



  // Makes a reference to the file's audioProperties.
  auto audio_properties  = (file_ref . audioProperties());



  // Determines the codec if supported.
  if(!Determine_If_Codec_Supported(*audio_properties, new_track))
  {
 
    // Iterates through all of new_tracks.
    for(auto new_tracks_it : *new_tracks)
    {

      // Deletes the Track.
      delete new_tracks_it;

    }



    // Clears the pointers in new_tracks.
    new_tracks -> clear();



    // Returns the empty new_tracks.
    return new_tracks;

  }



  // Stores the bitrate of the audio file.
  new_track . set_bit_rate(audio_properties -> bitrate());

  // Stores the sample rate of the audio file.
  new_track . set_sample_rate(audio_properties -> sampleRate());

  // Stores the channel count of the audio file.
  new_track . set_channels(audio_properties -> channels());



  // Gets the duration of the audio file in milliseconds and converts it to
  // nanoseconds.
  long long duration = 1e6 * (audio_properties -> lengthInMilliseconds());

  // Stores the duration of the audio file.
  new_track . set_duration(duration);



  // Stores the time as a formatted string.
  std::string* temp_time_string;

  // Converts the duration to a time format string in centiseconds.
  temp_time_string
    = time_converter_ -> Nanoseconds_To_Time_Format(duration);



  // Sets the Track's length using time format string.
  new_track . set_length(*temp_time_string);

  // Deletes the time format string.
  delete temp_time_string;



  // Retrieves the title of the track. 
  Interpret_Single_Value_Tag
    ("TITLE", type, temp_prop_map, new_track,
     &Track::title, &Track::set_title);

  // Retireves the artists of the track.
  Interpret_Multiple_Value_Tag
    ("ALBUMARTIST", type, temp_prop_map, new_track, &Track::album_artists,
     &Track::add_album_artist, &Track::clear_album_artists);

  // Retireves the artists of the track.
  Interpret_Multiple_Value_Tag
    ("ARTIST", type, temp_prop_map, new_track, &Track::artists,
     &Track::add_artist, &Track::clear_artists);

  // Retrieves the album name of the track.
  Interpret_Single_Value_Tag
    ("ALBUM", type, temp_prop_map, new_track, &Track::album,
     &Track::set_album);

  // Retrieves the genres of the track.
  Interpret_Multiple_Value_Tag
    ("GENRE", type, temp_prop_map, new_track, &Track::genres,
     &Track::add_genre, &Track::clear_genres);



  // True if the track has a track number.
  if(temp_prop_map . contains(TagLib::String("TRACKNUMBER")))
  {

    debug("Metadata: Interpret_Properties: NORMAL: TRACKNUMBER");



    //  Saved the value of the tag to a string.
    string track_num_str
      = temp_prop_map[TagLib::String("TRACKNUMBER")]
          . toString() . to8Bit(true);



    // Sets the track number of new_track.
    new_track . set_track_number(track_num_str);



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("TRACKNUMBER"));

  }

  // True if the track has a track total.
  if(temp_prop_map . contains(TagLib::String("TRACKTOTAL")))
  {

    debug("Metadata: Interpret_Properties: NORMAL: TRACKTOTAL");



    //  Saved the value of the tag to a string.
    string track_total_str
      = temp_prop_map[TagLib::String("TRACKTOTAL")]
          . toString() . to8Bit(true);



    // Sets the track total of new_track.
    new_track . set_track_total(track_total_str);



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("TRACKTOTAL"));

  }

  // True if the disc has a disc number.
  if(temp_prop_map . contains(TagLib::String("DISCNUMBER")))
  {

    debug("Metadata: Interpret_Properties: NORMAL: DISCNUMBER");



    //  Saved the value of the tag to a string.
    string disc_num_str
      = temp_prop_map[TagLib::String("DISCNUMBER")]
          . toString() . to8Bit(true);



    // Sets the disc number of new_track.
    new_track . set_disc_number(stoi(disc_num_str));



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("DISCNUMBER"));

  }

  // True if the track has a disc total.
  if(temp_prop_map . contains(TagLib::String("DISCTOTAL")))
  { 

    debug("Metadata: Interpret_Properties: NORMAL: DISCTOTAL");



    //  Saves the value of the tag to a string.
    string disc_total_str
      = temp_prop_map[TagLib::String("DISCTOTAL")]
          . toString() . to8Bit(true);



    // Sets the disc total of new_track.
    new_track . set_disc_total(stoi(disc_total_str));



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("DISCTOTAL"));

  }

  // True if the track has a date.
  if(temp_prop_map . contains(TagLib::String("DATE")))
  { 

    debug("Metadata: Interpret_Properties: NORMAL: DATE");



    // Sets the date of new_track.
    new_track . set_date(temp_prop_map[TagLib::String("DATE")]
                                         . toString() . to8Bit(true));



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("DATE"));

  }

  // True if the track has a ReplayGain Album Gain.
  if(temp_prop_map . contains(TagLib::String("REPLAYGAIN_ALBUM_GAIN")))
  {

    debug("Metadata: Interpret_Properties: NORMAL: REPLAYGAIN_ALBUM_GAIN");



    // Sets the value of the ReplayGain Album Gain.
    new_track . set_replaygain_album_gain
      (stod(temp_prop_map[TagLib::String("REPLAYGAIN_ALBUM_GAIN")] 
                            . toString() . to8Bit(true)));



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("REPLAYGAIN_ALBUM_GAIN"));

  }

  // True if the track has a ReplayGain Album Peak.
  if(temp_prop_map . contains(TagLib::String("REPLAYGAIN_ALBUM_PEAK")))
  {

    debug("Metadata: Interpret_Properties: NORMAL: REPLAYGAIN_ABLUM_PEAK");



    // Sets the value of the ReplayGain Album Peak.
    new_track . set_replaygain_album_peak
      (stod(temp_prop_map[TagLib::String("REPLAYGAIN_ALBUM_PEAK")]
                            . toString() . to8Bit(true)));



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("REPLAYGAIN_ALBUM_PEAK"));

  }

  // True if the track has a ReplayGain Track Gain.
  if(temp_prop_map . contains(TagLib::String("REPLAYGAIN_TRACK_GAIN")))
  {

    debug("Metadata: Interpret_Properties: NORMAL: REPLAYGAIN_TRACK_GAIN");



    // Sets the value of the ReplayGain Track Gain.
    new_track . set_replaygain_track_gain
      (stod(temp_prop_map[TagLib::String("REPLAYGAIN_TRACK_GAIN")]
                            . toString() . to8Bit(true)));



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("REPLAYGAIN_TRACK_GAIN"));

  } 

  // True if the track has a ReplayGain Track Peak.
  if(temp_prop_map . contains(TagLib::String("REPLAYGAIN_TRACK_PEAK")))
  {

    debug("Metadata: Interpret_Properties: NORMAL: REPLAYGAIN_TRACK_PEAK");



    // Sets the value of the ReplayGain Track Peak.
    new_track . set_replaygain_track_peak
      (stod(temp_prop_map[TagLib::String("REPLAYGAIN_TRACK_PEAK")]
                            . toString() . to8Bit(true)));



    // Erases the tag from the PropertyMap.
    temp_prop_map . erase(TagLib::String("REPLAYGAIN_TRACK_PEAK"));

  }



  // 
  if(config() . get("metadata.guess_metadata"))
  {

    // 
    if(new_track . album_artists() . empty())
    {

      // 
      if(!(new_track . artists() . empty()))
      {

        // 
        

      }

    }

    // 
    

  }



  // Iterates through all of the unused tags.
  for(auto it = temp_prop_map . begin(); it != temp_prop_map . end(); it++)
  {

    debug_ss . str("");

    debug_ss << "Custom Tag: \"" << (it -> first) . to8Bit(true) << "\"";

    debug(debug_ss . str() . c_str());



    // True if the tag is empty.
    if(((it -> first) . to8Bit(true)) == "")
    {

    }

  }



  // Returns the finished new_tracks Track data vector.
  return new_tracks;

}

void Metadata::Interpret_Single_Value_Tag
                 (const char* tag_name, 
                  TrackType type, 
                  TagLib::PropertyMap &prop_map, 
                  Track &track,
                  Glib::ustring& (Track::*getter)() const,
                  void (Track::*setter)(Glib::ustring*))
{

  // 
  if(!(prop_map . contains(TagLib::String(tag_name))))
  {

    // 
    return;

  }



  // 
  if(type == TrackType::NORMAL)
  {

    // 
    (track.*setter)
    (

      // 
      new Glib::ustring
      (

        // 
        prop_map[TagLib::String(tag_name)] . toString() . to8Bit(true)

      )

    ); 



    // 
    prop_map . erase(TagLib::String(tag_name));

  }

  // 
  else
  {

    // 
    if((track.*getter)() . empty())
    {

      // 
      (track.*setter)
      (

        new Glib::ustring
        (

          prop_map[TagLib::String(tag_name)] . toString() . to8Bit(true)

        )


      ); 

    }

    prop_map . erase(TagLib::String(tag_name));

  }

}

std::vector<Track*>* Metadata::Merge(std::vector<Track*>* vector1, 
                                     std::vector<Track*>* vector2)
{

  // 
  for(auto it : *vector2)
  {

    vector1 -> push_back(it);

  }



  // 
  delete vector2;



  // 
  return vector1;

}

bool Metadata::Print_Tags(const char* song_filename)
{

  // 
  TagLib::FileRef file_ref(song_filename);

  TagLib::PropertyMap temp_prop_map = (file_ref.file() -> properties());

  for(auto it = temp_prop_map.begin(); it != temp_prop_map.end(); it++)
  {

    stringstream debug_ss;

    debug_ss << "Tag Name: \"" << (it->first).to8Bit(true) << "\"";

    debug(debug_ss . str() . c_str());



    for(auto value_it : (it->second))
    {

      debug_ss << "Value: \""<< value_it.to8Bit(true) << "\"";

    debug(debug_ss . str() . c_str());

    }

    debug("");

  }

  return true;

}

void Metadata::Print_Properties(const char *filename)
{

  stringstream debug_ss;

  TagLib::FileRef file(filename); 
                                                                                
  TagLib::PropertyMap temp_prop_map = (file.file() -> properties());  

  for(auto it = temp_prop_map.begin(); it != temp_prop_map.end(); it++)
  {

    debug_ss << "\nTag: \"" << (it->first).to8Bit(true) << "\n";

    for(auto new_it = (it->second).begin(); new_it != (it->second).end(); new_it++)
    {

      debug_ss << "Value: " << new_it -> to8Bit(true) << "\n"; 

    }

  }


  debug(debug_ss . str() . c_str());

}

void Metadata::Remove_Duplicates
  (auto tracks, auto& single_tracks, auto& external_cue_tracks_tracks,
   auto& embedded_cue_tracks_tracks, int size)
{

  stringstream debug_ss;

  debug("Beginning of remove duplicates.");



  // 
  auto sorted_tracks = new vector<Track*>(size, nullptr);

  // 
  string cue_type_preference
    = config() . get("metadata.cuesheet_type_preference");



  // True if there are Tracks created from an embedded cue sheet.
  if(!(embedded_cue_tracks_tracks . empty()))
  {

    // 
    debug("Embedded cue conditional in Remove_Duplicates");



    // 
    auto embedded_cue_tracks_it = embedded_cue_tracks_tracks . begin();

    // 
    while((embedded_cue_tracks_it != embedded_cue_tracks_tracks . end())
            && ((embedded_cue_tracks_tracks . size()) > 0)) 
    {

      // 
      auto current_embedded_cue_tracks_it
        = embedded_cue_tracks_it;

      // 
      embedded_cue_tracks_it++;



      // 
      if(!(external_cue_tracks_tracks . empty()))
      { 

        // 
        auto external_cue_tracks_it
          = external_cue_tracks_tracks . begin();



        // 
        while(external_cue_tracks_it != external_cue_tracks_tracks . end()
	        && ((external_cue_tracks_tracks . size()) > 0)) 
        {

          // 
          auto current_external_cue_tracks_it
            = external_cue_tracks_it;

          // 
          external_cue_tracks_it++;



          // 
          Glib::ustring& external_filename
            = current_external_cue_tracks_it -> front() . first -> filename();

          // 
          Glib::ustring& embedded_filename
            = current_embedded_cue_tracks_it -> front() . first -> filename();



          debug_ss . str("");



          // 
          if(external_filename == embedded_filename)
          { 

            // 
            string cue_type_preference = "embedded";

            // 
            if(cue_type_preference == "embedded")
            {

              // 
              for(auto track_pair : *current_external_cue_tracks_it)
              {

                // 
                delete track_pair . first;

              } 

              // 
              external_cue_tracks_tracks . erase(current_external_cue_tracks_it);



              // 
              if(current_external_cue_tracks_it
                   == (external_cue_tracks_tracks . end()))
              {

                // 
                external_cue_tracks_it = external_cue_tracks_tracks . end();

              }

            }

            // 
            else
            {

              // 
              for(auto track_pair : *current_embedded_cue_tracks_it)
              {

                // 
                delete track_pair . first;

              } 

              // 
              embedded_cue_tracks_tracks . erase(current_embedded_cue_tracks_it);



              // 
              if(current_embedded_cue_tracks_it
                   == (embedded_cue_tracks_tracks . end()))
              {

                // 
                embedded_cue_tracks_it = embedded_cue_tracks_tracks . end();

              }

            }




            // 
            break;

          }

        }

      }

    } 



    // True if there are still external cue sheets.
    if(!(external_cue_tracks_tracks . empty()))
    {

      // Removes duplicates from external cue sheets.
      Remove_Duplicates_External_Cue
        (tracks, single_tracks, external_cue_tracks_tracks);

    }

  } 

  // True if there are Tracks created from an embedded cue sheet.
  else if(!(external_cue_tracks_tracks . empty()))
  {

    // Removes duplicates between single tracks and external cue sheets.
    Remove_Duplicates_External_Cue
      (tracks, single_tracks, external_cue_tracks_tracks);

  }



  // True if there are still Tracks created from an embedded cue sheet.
  if(!(embedded_cue_tracks_tracks . empty()))
  {

    // 
    for(auto embedded_cue_tracks : embedded_cue_tracks_tracks)
    {

      // 
      for(auto embedded_cue_track_pair : embedded_cue_tracks)
      {

        // 
        (*sorted_tracks)[embedded_cue_track_pair . second]
          = embedded_cue_track_pair . first;

      }

    }

  }

  // True if there are still Tracks created from an external cue sheet.
  if(!(external_cue_tracks_tracks . empty()))
  {

    // 
    for(auto external_cue_tracks : external_cue_tracks_tracks)
    {

      // 
      for(auto external_cue_track_pair : external_cue_tracks)
      {

        // 
        (*sorted_tracks)[external_cue_track_pair . second]
          = external_cue_track_pair . first;

      }

    }

  }

  // True if there are still Tracks created from an embedded cue sheet.
  if(!(single_tracks . empty()))
  {

    // 
    for(auto single_tracks_pair : single_tracks)
    {

      // 
      (*sorted_tracks)[single_tracks_pair . second]
        = single_tracks_pair . first;

    }

  }



  // 
  auto shrunk_sorted_tracks = new vector<Track*>;



  // 
  for(auto sorted_track : *sorted_tracks)
  {

    // 
    if(sorted_track != nullptr)
    {

      // 
      shrunk_sorted_tracks -> push_back(sorted_track);

    }

  }



  // 
  delete sorted_tracks;



  // 
  Merge(tracks, shrunk_sorted_tracks);

}

void Metadata::Remove_Duplicates_External_Cue
  (auto tracks, auto& single_tracks, auto& external_cue_tracks_tracks)
{

  stringstream debug_ss;



  // 
  auto external_cue_tracks_it = external_cue_tracks_tracks . begin();

  // Loops through the list of external cue files.
  while((external_cue_tracks_it != external_cue_tracks_tracks . end())
          &&
        (external_cue_tracks_tracks . size() > 0))
  {

    debug_ss . str("");

    debug_ss << "External cue size: " << external_cue_tracks_tracks . size();

    debug(debug_ss . str() . c_str());



    // True if there are non-cuesheet tracks generated from individual
    // track files being read.
    if(!(single_tracks . empty()))
    {

      debug("Entered single tracks conditional.");



      // 
      if(external_cue_tracks_it -> front() . first -> type()
           == TrackType::SINGLE_FILE)
      { 

        debug("SINGLE TRACKS CUE");



        // Holds a reference to the music filename of the external cue sheet.
        Glib::ustring& external_filename
          = external_cue_tracks_it -> front() . first -> filename();



        // Gets an iterator to the beginning of the single tracks.
        auto single_track_it = single_tracks . begin();

        // Loops through the single tracks looking for the files of any
        // external cue sheets so they aren't added to the playlist.
        while((single_track_it != (single_tracks . end()))
                && ((single_tracks . size()) > 0))
        {

          // Holds a pointer to the current single track it before
          // incrementing single_track_it so the single_track_it isn't
          // invalidated after erasing the current track.
          auto current_single_track_it = single_track_it;

          // Increments single_track_it.
          single_track_it++;



          // True if there the single file Track generated is the same as
          // music file for the cuesheet tracks.
          if(external_filename
               == (current_single_track_it -> first -> filename()))
          {

            debug("Deleting duplicate single file music file.");

            debug_ss . str("");

            debug_ss << "Single tracks: " << single_tracks . size();

            debug(debug_ss . str() . c_str());



            // Deletes the single file's track data.
            delete (current_single_track_it -> first);

            // Erasing the single file from the single tracks.
            single_tracks . erase(current_single_track_it);



            debug_ss . str("");

            debug_ss << "Single tracks: " << single_tracks . size();

            debug(debug_ss . str() . c_str());



            break;

          }

        }

      }

      // 
      else
      {

        debug("MULTIPLE TRACKS CUE");



        // 
        string files_or_cuesheet = config() . get("metadata.files_or_cuesheet");



        // 
        auto external_cue_track_pair_it = external_cue_tracks_it -> begin();

        // 
        while((external_cue_track_pair_it != (external_cue_tracks_it -> end()))
                && ((external_cue_tracks_it -> size()) > 0))
        {

          // 
          auto current_external_cue_track_pair_it = external_cue_track_pair_it;

          // 
          external_cue_track_pair_it++;



          // Holds a reference to the music filename of the external cue sheet.
          Glib::ustring& external_filename
            = current_external_cue_track_pair_it -> first -> filename();



          // Gets an iterator to the beginning of the single tracks.
          auto single_track_it = single_tracks . begin();

          // Loops through the single tracks looking for the files of any
          // external cue sheets so they aren't added to the playlist.
          while((single_track_it != (single_tracks . end()))
                  && ((single_tracks . size()) > 0))
          {

            // Holds a pointer to the current single track it before
            // incrementing single_track_it so the single_track_it isn't
            // invalidated after erasing the current track.
            auto current_single_track_it = single_track_it;

            // Increments single_track_it.
            single_track_it++;



            Glib::ustring& single_filename
              = current_single_track_it -> first -> filename();



            // True if there the single file Track generated is the same as
            // music file for the cuesheet tracks.
            if(external_filename == single_filename)
            {

              debug("Deleting duplicate single file music file.");

              debug_ss . str("");

              debug_ss << "Single tracks: " << single_tracks . size();

              debug(debug_ss . str() . c_str());



              // 
              if(files_or_cuesheet == "cuesheet")
              {

                // Deletes the single file's track data.
                delete (current_single_track_it -> first);

                // Erasing the single file from the single tracks.
                single_tracks . erase(current_single_track_it);

              }

              // 
              else
              {

                // Deletes the single file's track data.
                delete (current_external_cue_track_pair_it -> first);

                // Erasing the single file from the single tracks.
                single_tracks . erase(current_external_cue_track_pair_it);

              }




              debug_ss . str("");

              debug_ss << "Single tracks: " << single_tracks . size();

              debug(debug_ss . str() . c_str());



              // 
              break;

            }

          }



          // 
          if(current_external_cue_track_pair_it
               == (external_cue_tracks_it -> end()))
          {

            // 
            break;

          }

        }

      }

    }



    // 
    external_cue_tracks_it++;

  }

}





//         //
// Writing ////////////////////////////////////////////////////////////////////
//         //

bool Metadata::Write_Tag(const char* tag_name, shared_ptr<Track> track_sptr)
{

  // 
  const char* filename = track_sptr -> filename() . c_str();



  // 
  TagLib::FileRef file_ref(filename);

  // 
  TagLib::PropertyMap temp_prop_map = (file_ref . file() -> properties());



  // 
  string tag_name_str = tag_name;



  // 
  if(tag_name_str == "album")
  {

    // 
    temp_prop_map . erase(TagLib::String("ALBUM"));



    // 
    TagLib::String tag_str(track_sptr -> album() . raw(), TagLib::String::Type::UTF8);



    // 
    if(temp_prop_map . insert(TagLib::String("ALBUM"), tag_str))
    {

      debug("Successfully inserted!");

    }

    // 
    else
    {

      debug("Insertion failed!");



      // 
      return false;

    }
 
  } 

  // 
  else if(tag_name_str == "album_artists")
  {
 
    // 
    temp_prop_map . erase(TagLib::String("ALBUMARTIST"));



    // 
    for(auto tag_value : track_sptr -> album_artists())
    { 

      // 
      TagLib::StringList tag_str_list
        (TagLib::String(tag_value -> raw(), TagLib::String::Type::UTF8));



      // 
      if(temp_prop_map . insert(TagLib::String("ALBUMARTIST"), tag_str_list))
      {

        debug("Successfully inserted!");

      }

      // 
      else
      {

        debug("Insertion failed!");



        // 
        return false;

      }
 
    }

  } 

  // 
  else if(tag_name_str == "artists")
  { 

    // 
    temp_prop_map . erase(TagLib::String("ARTIST"));



    // 
    for(auto tag_value : track_sptr -> artists())
    { 

      // 
      TagLib::StringList tag_str_list
        (TagLib::String(tag_value -> raw(), TagLib::String::Type::UTF8));



      // 
      if(temp_prop_map . insert(TagLib::String("ARTIST"), tag_str_list))
      {

        debug("Successfully inserted!");

      }

      // 
      else
      {

        debug("Insertion failed!");



        // 
        return false;

      }

    }
 
  }

  // 
  else if(tag_name_str == "date")
  {

    // 
    temp_prop_map . erase(TagLib::String("DATE"));



    // 
    Glib::ustring tag_str;

    // 
    if(track_sptr -> date() == 0)
    {

      tag_str = "";

    }

    // 
    else
    {

      // 
      tag_str = to_string(track_sptr -> date());

    }



    // 
    TagLib::String taglib_str(tag_str, TagLib::String::Type::UTF8);



    // 
    if(temp_prop_map . insert(TagLib::String("DATE"), taglib_str))
    {

      debug("Successfully inserted!");

    }

    // 
    else
    {

      debug("Insertion failed!");



      // 
      return false;

    }
 
  }  

  // 
  else if(tag_name_str == "disc_number")
  {

    // 
    temp_prop_map . erase(TagLib::String("DISCNUMBER"));



    // 
    Glib::ustring tag_str;

    // 
    if(track_sptr -> disc_number() <= 0)
    {

      tag_str = "";

    }

    // 
    else
    {

      // 
      tag_str = to_string(track_sptr -> disc_number());

    }



    // 
    TagLib::String taglib_str(tag_str, TagLib::String::Type::UTF8);



    // 
    if(temp_prop_map . insert(TagLib::String("DISCNUMBER"), taglib_str))
    {

      debug("Successfully inserted!");

    }

    // 
    else
    {

      debug("Insertion failed!");



      // 
      return false;

    }
 
  }   

  // 
  else if(tag_name_str == "disc_total")
  {

    // 
    temp_prop_map . erase(TagLib::String("DISCTOTAL"));



    // 
    Glib::ustring tag_str;

    // 
    if(track_sptr -> disc_total() == 0)
    {

      tag_str = "";

    }

    // 
    else
    {

      // 
      tag_str = to_string(track_sptr -> disc_total());

    }



    // 
    TagLib::String taglib_str(tag_str, TagLib::String::Type::UTF8);



    // 
    if(temp_prop_map . insert(TagLib::String("DISCTOTAL"), taglib_str))
    {

      debug("Successfully inserted!");

    }

    // 
    else
    {

      debug("Insertion failed!");



      // 
      return false;

    }
 
  }  

  // 
  else if(tag_name_str == "genres")
  { 

    // 
    temp_prop_map . erase(TagLib::String("GENRE"));



    // 
    for(auto tag_value : track_sptr -> genres())
    { 

      // 
      TagLib::StringList tag_str_list
        (TagLib::String(tag_value -> raw(), TagLib::String::Type::UTF8));



      // 
      if(temp_prop_map . insert(TagLib::String("GENRE"), tag_str_list))
      {

        debug("Successfully inserted!");

      }

      // 
      else
      {

        debug("Insertion failed!");



        // 
        return false;

      }
 
    }

  } 

  // 
  else if(tag_name_str == "title")
  { 

    // 
    temp_prop_map . erase(TagLib::String("TITLE"));



    // 
    TagLib::String tag_str(track_sptr -> title() . raw(),
                           TagLib::String::Type::UTF8);



    // 
    if(temp_prop_map . insert(TagLib::String("TITLE"), tag_str))
    {

      debug("Successfully inserted!");

    }

    // 
    else
    {

      debug("Insertion failed!");



      // 
      return false;

    }
 
  } 

  // 
  else if(tag_name_str == "track_number")
  {

    // 
    temp_prop_map . erase(TagLib::String("TRACKNUMBER"));



    // 
    Glib::ustring tag_str;

    // 
    if(track_sptr -> track_number() == 0)
    {

      tag_str = "";

    }

    // 
    else
    {

      // 
      tag_str = to_string(track_sptr -> track_number());

    }



    // 
    TagLib::String taglib_str(tag_str, TagLib::String::Type::UTF8);



    // 
    if(temp_prop_map . insert(TagLib::String("TRACKNUMBER"), taglib_str))
    {

      debug("Successfully inserted!");

    }

    // 
    else
    {

      debug("Insertion failed!");



      // 
      return false;

    }
 
  } 

  // 
  else if(tag_name_str == "track_total")
  {

    // 
    temp_prop_map . erase(TagLib::String("TRACKTOTAL"));



    // 
    Glib::ustring tag_str;

    // 
    if(track_sptr -> track_total() == 0)
    {

      tag_str = "";

    }

    // 
    else
    {

      // 
      tag_str = to_string(track_sptr -> track_total());

    }



    // 
    TagLib::String taglib_str(tag_str, TagLib::String::Type::UTF8);



    // 
    if(temp_prop_map . insert(TagLib::String("TRACKTOTAL"), taglib_str))
    {

      debug("Successfully inserted!");

    }

    // 
    else
    {

      debug("Insertion failed!");



      // 
      return false;

    }
 
  } 



  // 
  file_ref . file() -> setProperties(temp_prop_map);

  // 
  file_ref . file() -> save();



  // 
  return true;

}
