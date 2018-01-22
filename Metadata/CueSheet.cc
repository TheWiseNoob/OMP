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

#include "CueSheet.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Metadata/Track.h"

#include "../TimeConversion.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/ustring.h>

#include <iostream>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

using namespace std;

using namespace Glib;





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

CueSheet::CueSheet()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

std::vector<Track*>* CueSheet::operator()(Glib::ustring* cue_sheet,
                                          Glib::ustring& filename_beginning)
{

  // A pointer to a new Track pointer queue object.
  std::vector<Track*> *cue_sheet_tracks = new std::vector<Track*>;



  // 
  if(!(cue_sheet -> size() > 1))
  {

    // 
    return cue_sheet_tracks;

  }



  // Holds the genres of the release.
  std::vector<Glib::ustring*>* genres = nullptr;

  // Holds the artists of the release.
  std::vector<Glib::ustring*>* artists = nullptr;

  // Holds the album name.
  Glib::ustring* album = nullptr;

  // Holds the date the album was releasted.
  int date = 0;

  // Holds the MusicBrainz disc id.
  Glib::ustring* disc_id = nullptr;

  // Holds the comment for the release.
  Glib::ustring* comment = nullptr;

  // Holds the catalog id for the album.
  Glib::ustring* catalog = nullptr;

  // Holds the filename for the single-file album.
  Glib::ustring* filename = nullptr;

  // Used to count how many tracks there are in the release.
  int track_count = 0;

  // 
  int filename_count = 0;

  // Holds the ReplayGain album gain of the release.
  double replaygain_album_gain = -1.000;

  // Holds the ReplayGain album peak of the release.
  double replaygain_album_peak = -1.000;



  // Holds the TrackType of the release/album.
  TrackType type;

  // Indicates whether the type of Cue Sheet has been determined.
  bool type_locked = false;

  // 
  bool first_track = true;



  // 
  Track* new_track = new Track;



  // 
  vector<ustring*> filenames_vector;



  // Sets the global locale to US UTF8 English.
  std::locale::global(std::locale(""));



  // Loops through the cue sheet.
  for(auto it = (cue_sheet -> raw() . begin());
      it != (cue_sheet -> raw() . end());
      Increment_It_To_Next_Line(it, cue_sheet))
  {

    // REM top section
    if(*it == 'R')
    {

      //
      if(!Advance_It(it, 4, cue_sheet))
      {

        //
        break;

      }



      //
      if(char(*it) == 'G')
      {

        //
        if(!Advance_It(it, 6, cue_sheet))
        {

          //
          break;

        }



        //
        Glib::ustring * genre_ustr = new ustring;



        // 
        if(*it == '\"')
        {


          //
          if(!Advance_It(it, 1, cue_sheet))
          {

            //
            break;

          }



          // 
          while((*it != '\"') && (*it != '\r') && (*it != '\n')
                && (it != cue_sheet -> raw() . end()))
          {

            //
            genre_ustr -> push_back(char(*it));



            //
            it++;

          }

        }

        // 
        else if(*it == '\n')
        {

          // 
          continue;

        }

        // 
        else if(*it == '\r')
        {

          // 
          continue;

        }

        // 
        else
        {

          Extract_Characters_Until_Line_Break(it, genre_ustr, cue_sheet);

        } 


        //
        genres = new vector<ustring*>;

        //
        genres -> push_back(genre_ustr);

      }

      //
      else if(char(*it) == 'D')
      {

        //
        if(!Advance_It(it, 1, cue_sheet))
        {

          //
          break;

        }



        //
        if(*it == 'A')
        {

          //
          if(!Advance_It(it, 4, cue_sheet))
          {

            //
            break;

          }



          // 
          if(*it == '\n')
          {

            // 
            continue;

          }

          // 
          else if(*it == '\r')
          {

            // 
            continue;

          }



          //
          Glib::ustring temp_date;

          //
          Extract_Characters_Until_Line_Break(it, &temp_date, cue_sheet);



          date = stoi(temp_date . raw());

        }

        //
        else if(*it == 'I')
        {

          //
          if(!Advance_It(it, 6, cue_sheet))
          {

            //
            break;

          }



          //
          disc_id = new Glib::ustring;

          //
          Extract_Characters_Until_Line_Break(it, disc_id, cue_sheet);

        }

      }

      //
      else if(*it == 'C')
      {

        //
        if(!Advance_It(it, 1, cue_sheet))
        {

          //
          break;

        }



        //
        if(*it == 'O')
        {

          //
          if(!Advance_It(it, 7, cue_sheet))
          {

            //
            break;

          }



          //
          comment = new Glib::ustring;

          //
          Extract_Characters_Until_Line_Break(it, comment, cue_sheet);

         }

       }

      //
      else if(*it == 'R')
      {

        //
        if(!Advance_It(it, 17, cue_sheet))
        {

          //
          break;

        }



        //
        if(*it == 'G')
        {

          //
          if(!Advance_It(it, 5, cue_sheet))
          {

            //
            break;

          }



          //
          ustring* gain_ustr = new ustring;



          //
          while((*it != ' ') && (*it != 'd')
                  && (it != cue_sheet -> raw() . end()))
          {

            //
            gain_ustr -> push_back(*it);



            //
            it++;

          }



          //
          try
          {

            //
            replaygain_album_gain = stod(*gain_ustr);

          }

          //
          catch(const std::invalid_argument& argument)
          {

          }

          //
          catch(const std::out_of_range& argument)
          {

          }



          //
          delete gain_ustr;

        }

        //
        else if(*it == 'P')
        {

          //
          if(!Advance_It(it, 5, cue_sheet))
          {

            //
            break;

          }



          //
          ustring* peak_ustr = new ustring;



          //
          Extract_Characters_Until_Line_Break(it, peak_ustr, cue_sheet);



          //
          try
          {

            //
            replaygain_album_peak = stod(*peak_ustr);

          }

          //
          catch(const std::invalid_argument& argument)
          {

          }

          //
          catch(const std::out_of_range& argument)
          {

          }



          //
          delete peak_ustr;

        }

      }

    }

    // Catalog parser
    else if(*it == 'C')
    {

      //
      if(!Advance_It(it, 1, cue_sheet))
      {

        //
        break;

      }



      //
      if(*it == 'A')
      {

        //
        if(!Advance_It(it, 7, cue_sheet))
        {

          //
          break;

        }



        //
        catalog = new Glib::ustring;



        //
        Extract_Characters_Until_Line_Break(it, catalog, cue_sheet);

      }

    }

    // Album performer parser
    else if(*it == 'P')
    {

      //
      if(!Advance_It(it, 11, cue_sheet))
      {

        //
        break;

      }



      //
      artists = new vector<ustring*>;

      //
      ustring* artist_ustr = new ustring;



      //
      while((*it != '\"') && (*it != '\r') && (*it != '\n')
              && (it != cue_sheet -> raw() . end()))
      {

        //
        artist_ustr -> push_back(char(*it));



        //
        it++;

      }



      //
      artists -> push_back(artist_ustr);

    } 

    //
    else if(*it == 'T')
    {

      //
      if(!Advance_It(it, 7, cue_sheet))
      {

        //
        break;

      }



      //
      album = new Glib::ustring;

      //
      while((*it != '\"') && (*it != '\r') && (*it != '\n')
              && (it != cue_sheet -> raw() . end()))
      {

        //
        album -> push_back(char(*it));



        //
        it++;

      }

    }

    // Parse FILE
    else if(char(*it) == 'F')
    {

      //
      if(!Advance_It(it, 6, cue_sheet))
      {

        //
        break;

      }



      //
      filename = new Glib::ustring(filename_beginning);

      //
      while((*it != '\"') && (*it != '\r') && (*it != '\n')
              && (it != cue_sheet -> raw() . end()))
      {

        //
        filename -> push_back(char(*it));



        //
        it++;

      }




      // 
      filenames_vector . push_back(filename);



      // 
      filename_count++;

    }

    //
    else if(char(*it) == ' ')
    {

      //
      if(!Advance_It(it, 2, cue_sheet))
      {

        //
        break;

      }



      // 
      if(*it == 'T')
      {

        // 
        track_count++;



        // 
        if(type_locked)
        {

        }

        // 
        else if(filename_count > 1)
        {

          // 
          if(filename_count < track_count)
          {

            // 
            type = TrackType::MULTIPLE_FILE_NONCOMPLIANT;



            // 
            type_locked = true;

          }

          // 
          else
          {

            //
            type = TrackType::MULTIPLE_FILE_GAP_PREPENDED;

          }

        }

        // 
        else
        {

          // 
          type = TrackType::SINGLE_FILE;

        }



        // 
        if(first_track)
        {

          first_track = false;

        }

        // 
        else
        {

          // 
          cue_sheet_tracks -> push_back(new_track);



          // 
          new_track = new Track;

        }



        // 
        new_track -> set_track_number(track_count);

      }

      // 
      else if(*it == ' ')
      {

        //
        if(!Advance_It(it, 2, cue_sheet))
        {

          //
          break;

        }



        // Title
        if(*it == 'T')
        {

          //
          if(!Advance_It(it, 7, cue_sheet))
          {

            //
            break;

          }



          //
          ustring* track_title_ustr = new Glib::ustring;

          //
          while((*it != '\"') && (*it != '\r') && (*it != '\n')
                  && (it != cue_sheet -> raw() . end()))
          {

            //
            track_title_ustr -> push_back(*it);



            //
            it++;

          }



          // 
          new_track -> set_title(track_title_ustr);

        }

        // 
        else if(*it == 'P')
        {

          //
          if(!Advance_It(it, 1, cue_sheet))
          {

            //
            break;

          }



          // Album artist
          if(*it == 'E')
          { 

            //
            if(!Advance_It(it, 10, cue_sheet))
            {

              //
              break;

            }



            //
            ustring* album_artist_ustr = new ustring;

            // 
            vector<ustring*>* album_artists_vector = new vector<ustring*>;



            //
            while((*it != '\"') && (*it != '\r') && (*it != '\n')
                    && (it != cue_sheet -> raw() . end()))
            {

              //
              album_artist_ustr -> push_back(char(*it));



              //
              it++;

            }



            // 
            album_artists_vector -> push_back(album_artist_ustr);

            // 
            new_track -> set_album_artists(album_artists_vector);

          }

          // 
          else if(*it == 'R')
          {

            // 
            type = TrackType::MULTIPLE_FILE_GAP_LEFT_OUT;



            // 
            type_locked = true;
            
          }

        }

        // Parses ISRC if first if is true or INDE XX if else if is true.
        else if(*it == 'I')
        {

          //
          if(!Advance_It(it, 1, cue_sheet))
          {

            //
            break;

          }



          // 
          if(*it == 'S')
          { 

            //
            if(!Advance_It(it, 4, cue_sheet))
            {

              //
              break;

            }



            // 
            ustring* isrc_ustr = new Glib::ustring;



            //
            Extract_Characters_Until_Line_Break(it, isrc_ustr, cue_sheet);



            // 
            new_track -> set_isrc(isrc_ustr);

          }

          // 
          else if(*it == 'N')
          {

            //
            if(!Advance_It(it, 5, cue_sheet))
            {

              //
              break;

            }



            // 
            long long index_time = 0LL;



            //
            if(*it == '0')
            {

              //
              if(!Advance_It(it, 1, cue_sheet))
              {

                //
                break;

              }



              //
              if(*it == '0')
              {

                //
                if(!Advance_It(it, 2, cue_sheet))
                {

                  //
                  break;

                }



                // 
                Extract_Time(it, index_time, cue_sheet);



                // 
                new_track -> set_pregap_start(index_time);

              }

              //
              else if(*it == '1')
              {

                //
                if(!Advance_It(it, 2, cue_sheet))
                {

                  //
                  break;

                }



                // 
                Extract_Time(it, index_time, cue_sheet);



                // 
                new_track -> set_start(index_time);

              }

            }

          } 

        }

        // Parses REM of tracks.
        else if(*it == 'R')
        {

          //
          if(!Advance_It(it, 4, cue_sheet))
          {

            //
            break;

          }



          // Genres
          if(*it == 'G')
          {

            //
            if(!Advance_It(it, 6, cue_sheet))
            {

              //
              break;

            }



            //
            Glib::ustring * genre_ustr = new ustring;

            //
            Extract_Characters_Until_Line_Break(it, genre_ustr, cue_sheet);



            //
            auto genres_vector = new vector<ustring*>;

            // 
            genres_vector -> push_back(genre_ustr);

            //
            new_track -> set_genres(genres_vector);

          }

          // ReplayGain
          else if(*it == 'R')
          {

            //
            if(!Advance_It(it, 17, cue_sheet))
            {

              //
              break;

            }



            //
            if(*it == 'G')
            {

              //
              if(!Advance_It(it, 5, cue_sheet))
              {

                //
                break;

              }



              //
              ustring* gain_ustr = new ustring;



              //
              while((*it != ' ') && (*it != 'd')
                      && (it != cue_sheet -> raw() . end()))
              {

                //
                gain_ustr -> push_back(*it);



                //
                it++;

              }



              //
              try
              {

                //
                new_track -> set_replaygain_track_gain(stod(*gain_ustr));

              }

              //
              catch(const std::invalid_argument& argument)
              {

              }

              //
              catch(const std::out_of_range& argument)
              {

              }



              //
              delete gain_ustr;

            }

            //
            else if(*it == 'P')
            {

              //
              if(!Advance_It(it, 5, cue_sheet))
              {

                //
                break;

              }



              //
              ustring* peak_ustr = new ustring;



              //
              Extract_Characters_Until_Line_Break(it, peak_ustr, cue_sheet);



              //
              try
              {

                //
                new_track -> set_replaygain_track_peak(stod(*peak_ustr));

              }

              //
              catch(const std::invalid_argument& argument)
              {

              }

              //
              catch(const std::out_of_range& argument)
              {

              }



              //
              delete peak_ustr;

            }

          }

        }

        // Parses FLAGS
        else if(*it == 'F')
        {

          //
          if(!Advance_It(it, 6, cue_sheet))
          {

            //
            break;

          }



          // 
          auto* flags = new vector<Glib::ustring*>;

          // 
          ustring* flag_ustr = new Glib::ustring;



          // 
          while((*it != '\n') && (*it != '\r')
                  && (it != (cue_sheet -> raw() . end())))
          {

            // 
            if(*it == ' ')
            {

              // 
              flags -> emplace_back(flag_ustr);



              // 
              flag_ustr = nullptr;

            }

            // 
            else
            {

              //
              if(flag_ustr == nullptr)
              {

                // 
                flag_ustr = new ustring;

              }



              // 
              flag_ustr -> push_back(char(*it));

            }



            // 
            it++;

          }



          // 
          if(flag_ustr != nullptr)
          {

            flags -> emplace_back(flag_ustr);

          }



          // 
          new_track -> set_flags(flags);

        }

      }

    }

  }



  // 
  cue_sheet_tracks -> push_back(new_track);



  // 
  if(type == TrackType::SINGLE_FILE)
  {

    // 
    for(auto track_it : *cue_sheet_tracks)
    {

      // 
      track_it -> set_filename(new ustring(*filename));

     }


    // 
    delete filename;

  }

  else
  {

    // 
    int count = 0;

    // 
    int filenames_vector_size = filenames_vector . size();



    // 
    for(auto track_it : *cue_sheet_tracks)
    {

      // 
      if(count == filenames_vector_size)
      {

        // 
        break;

      }



      // 
      track_it -> set_filename(*(filenames_vector[count]));



      // 
      count++;

    }

  }



  //
  for(int i = 0; i < (int(cue_sheet_tracks -> size()) - 1); i++)
  {

    (*cue_sheet_tracks)[i] -> set_track_total(track_count);

    (*cue_sheet_tracks)[i] -> set_type(type);

    if(artists != nullptr)
    {

      auto artists_copy = new vector<Glib::ustring*>;

      for(auto it : *artists)
      {

        artists_copy -> push_back(new Glib::ustring(*it));

       }

      (*cue_sheet_tracks)[i] -> set_artists(artists_copy);

    } 

    if(album != nullptr)
    {

      (*cue_sheet_tracks)[i] -> set_album(*album);

    }

    if(genres != nullptr)
    {

      auto genres_copy = new vector<Glib::ustring*>;

      for(auto it : *genres)
      {

        genres_copy -> push_back(new Glib::ustring(*it));

      }

      (*cue_sheet_tracks)[i] -> set_genres(genres_copy);

    }

    (*cue_sheet_tracks)[i] -> set_date(date);

    if(disc_id != nullptr)
    {

      (*cue_sheet_tracks)[i] -> set_disc_id(*disc_id);

    }

    if(catalog != nullptr)
    {
      (*cue_sheet_tracks)[i] -> set_catalog(*catalog);
    }

    if(comment != nullptr)
    {

      (*cue_sheet_tracks)[i] -> set_comment(*comment);

    }



    // 
    if(type == TrackType::SINGLE_FILE)
    {

      // 
      if(((*cue_sheet_tracks)[i+1] -> pregap_start()) == -1)
      {

        (*cue_sheet_tracks)[i] -> set_end((*cue_sheet_tracks)[i+1] -> start() - 1);

      }

      // 
      else
      {

        (*cue_sheet_tracks)[i] -> set_end(((*cue_sheet_tracks)[i+1] -> pregap_start()) - 1);

      }

    }



/*
    // 
    else if(type == TrackType::MULTIPLE_FILE_NONCOMPLIANT)
    {

      if(((*cue_sheet_tracks)[i+1] -> pregap_start()) != -1)
      {

        (*cue_sheet_tracks)[i] -> set_end((*cue_sheet_tracks)[i+1] -> pregap_start());

      }

    }
*/



    // 
    (*cue_sheet_tracks)[i] -> set_replaygain_album_gain(replaygain_album_gain);

    // 
    (*cue_sheet_tracks)[i] -> set_replaygain_album_peak(replaygain_album_peak);

  }



  //
  cue_sheet_tracks -> back() -> set_type(type);

  //
  cue_sheet_tracks -> back() -> set_track_total(track_count);

  //
  cue_sheet_tracks -> back() -> set_replaygain_album_gain(replaygain_album_gain);

  //
  cue_sheet_tracks -> back() -> set_replaygain_album_peak(replaygain_album_peak);

  //
  cue_sheet_tracks -> back() -> set_end(-1);



  //
  if(artists != nullptr)
  {

    cue_sheet_tracks -> back() -> set_artists(artists);

  }

  //
  if(album != nullptr)
  {

    cue_sheet_tracks -> back() -> set_album(album);

  }

  //
  if(genres != nullptr)
  {

    cue_sheet_tracks -> back() -> set_genres(genres);

  }

  cue_sheet_tracks -> back() -> set_date(date);

  //
  if(disc_id != nullptr)
  {

    cue_sheet_tracks -> back() -> set_disc_id(disc_id);

  }

  //
  if(catalog != nullptr)
  {

    cue_sheet_tracks -> back() -> set_catalog(catalog);

  }

  //
  if(comment != nullptr)
  {

    cue_sheet_tracks -> back() -> set_comment(comment);

  }



  //
  return cue_sheet_tracks;

}

bool CueSheet::Advance_It
  (auto& it, int length, Glib::ustring* cue_sheet)
{

  //
  int count = 0;



  //
  while((count < length))
  {

    //
    it++;



    //
    if(it == (cue_sheet -> raw() . end()))
    {

      //
      return false;

    }



    //
    count++;

  }



  //
  return true;

}

inline bool CueSheet::Extract_Characters_Until_Line_Break
  (auto& it, Glib::ustring* ustring_ptr, Glib::ustring* cue_sheet)
{

  //
  do
  {

    //
    if(it == (cue_sheet -> raw() . end()))
    {

      //
      return false;

    }



    //
    ustring_ptr -> push_back(char(*it));



    //
    it++;

  }

  //
  while((*it != '\n') && (*it != '\r'));



  //
  return true;

}

inline bool CueSheet::Extract_Time
  (auto& it, long long& time, Glib::ustring* cue_sheet)
{

  //
  string temp_string = "";

  //
  static TimeConversion temp_time_conversion;



  //
  if(it == (cue_sheet -> raw() . end()))
  {

    //
    return false;

  }

  //
  temp_string += char(*it);



  //
  it++;

  //
  if(it == (cue_sheet -> raw() . end()))
  {

    //
    return false;

  }

  //
  temp_string += char(*it);



  //
  it++;

  //
  if(it == (cue_sheet -> raw() . end()))
  {

    //
    return false;

  }

  //
  temp_string += ':';


  //
  it++;

  //
  if(it == (cue_sheet -> raw() . end()))
  {

    //
    return false;

  }

  //
  temp_string += char(*it);



  //
  it++;

  //
  if(it == (cue_sheet -> raw() . end()))
  {

    //
    return false;

  }

  //
  temp_string += char(*it);



  //
  it++;

  //
  if(it == (cue_sheet -> raw() . end()))
  {

    //
    return false;

  }

  //
  temp_string += ':';



  //
  it++;

  //
  if(it == (cue_sheet -> raw() . end()))
  {

    //
    return false;

  }

  //
  temp_string += char(*it);



  //
  it++;

  //
  if(it == (cue_sheet -> raw() . end()))
  {

    //
    return false;

  }

  //
  temp_string += char(*it);



  // 
  time = temp_time_conversion.Time_Format_To_Nano_Seconds(temp_string, true);



  //
  return true;

}

inline void CueSheet::Increment_It_To_Next_Line
  (auto& it, Glib::ustring* cue_sheet)
{

  //
  while((*it != '\n') && (*it != '\r') && (it != (cue_sheet -> raw() . end())))
  {

    //
    it++;

  }



  //
  while(((*it == '\n') || (*it == '\r'))
          && (it != (cue_sheet -> raw() . end())))
  {

    //
    it++;

  }

}
