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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef TRACK_H
#define TRACK_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "Tag.h"

#include "TrackType.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <string>

#include <vector>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

namespace Glib
{

  class ustring;

}





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Track
{

  //              //
  //              //
  // Constructors /////////////////////////////////////////////////////////////
  //              //
  //              //

  public:

    Track();

    Track(const Track& new_track);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~Track();





  //                    //
  //                    //
  // Operator Overloads ///////////////////////////////////////////////////////
  //                    //
  //                    //

  public:

    void operator =(const Track& copied_track);

    bool operator !=(const Track& compared_track) const;

    bool operator ==(const Track& compared_track) const;





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    bool Cue() const;

    bool Cue_Embedded() const;

    bool Is_Empty() const;

  protected:
    
    Glib::ustring* 
        Multiple_Values_To_Single_Value(std::vector<Glib::ustring*> &values);

  public:

    bool Normal() const;

    bool Pregap() const;

    void Print();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    TrackType type() const;

    Glib::ustring &filename() const;

    Glib::ustring &cue_filename() const;

    Glib::ustring &pregap_filename() const;

    Glib::ustring &album() const;

    Glib::ustring &title() const;

    Glib::ustring &length() const;

    int date() const;

    Glib::ustring &comment() const;

    Glib::ustring &disc_id() const;

    Glib::ustring &catalog() const;

    Glib::ustring &isrc() const;

    Glib::ustring& codec() const;

    Glib::ustring& mime() const;

    std::vector<Glib::ustring*>& album_artists();
    std::vector<Glib::ustring*>& album_artists() const;
    Glib::ustring* album_artists_string();

    std::vector<Glib::ustring*> &artists();
    std::vector<Glib::ustring*> &artists() const;
    Glib::ustring* artists_string();

    std::vector<Glib::ustring*> &genres();
    std::vector<Glib::ustring*> &genres() const;
    Glib::ustring *genres_string();

    std::vector<Glib::ustring*> &flags();
    std::vector<Glib::ustring*> &flags() const;

    int track_number() const;

    int track_total() const;

    int disc_number() const;

    int disc_total() const; 

    int bit_rate() const;

    int bit_depth() const;

    int sample_rate() const;

    int channels() const;

    long long start() const;

    long long pregap_start() const;

    long long end() const;

    std::vector<long long> &subindices();
    std::vector<long long> &subindices() const;

    double replaygain_album_gain() const;

    double replaygain_album_peak() const;

    double replaygain_track_gain() const;

    double replaygain_track_peak() const;

    long long duration() const;

    std::vector<Tag> &tags();
    std::vector<Tag> &tags() const;





  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    void set_type(const TrackType new_type);

    void set_filename(const char* new_filename);
    void set_filename(const std::string &new_filename);
    void set_filename(const Glib::ustring &new_filename);
    void set_filename(Glib::ustring* new_filename);

    void set_cue_filename(const char* new_filename);
    void set_cue_filename(const std::string &new_filename);
    void set_cue_filename(const Glib::ustring &new_filename);
    void set_cue_filename(Glib::ustring* new_filename);

    void set_pregap_filename(const char* new_filename);
    void set_pregap_filename(const std::string &new_filename);
    void set_pregap_filename(const Glib::ustring &new_filename);
    void set_pregap_filename(Glib::ustring* new_filename);

    void set_album(const char* new_album);
    void set_album(const std::string &new_album);
    void set_album(const Glib::ustring &new_album);
    void set_album(Glib::ustring* new_album);

    void set_title(const char* new_title);
    void set_title(const std::string &new_title);
    void set_title(const Glib::ustring &new_title);
    void set_title(Glib::ustring* new_title);

    void set_length(const char* new_setting);
    void set_length(const std::string &new_setting);
    void set_length(const Glib::ustring &new_setting);
    void set_length(Glib::ustring* new_setting);

    void set_comment(const char* new_comment);
    void set_comment(const std::string &new_comment);
    void set_comment(const Glib::ustring &new_comment);
    void set_comment(Glib::ustring* new_comment);

    void set_disc_id(const char* new_setting);
    void set_disc_id(const std::string &new_setting);
    void set_disc_id(const Glib::ustring &new_setting);
    void set_disc_id(Glib::ustring* new_setting);

    void set_catalog(const char* new_setting);
    void set_catalog(const std::string &new_setting);
    void set_catalog(const Glib::ustring &new_setting);
    void set_catalog(Glib::ustring* new_setting);

    void set_isrc(const char* new_setting);
    void set_isrc(const std::string &new_setting);
    void set_isrc(const Glib::ustring &new_setting);
    void set_isrc(Glib::ustring* new_setting);

    void set_codec(const char* new_codec);
    void set_codec(const std::string &new_codec);
    void set_codec(const Glib::ustring &new_codec);
    void set_codec(Glib::ustring* new_codec);

    void set_mime(const char* new_mime);
    void set_mime(const std::string &new_mime);
    void set_mime(const Glib::ustring &new_mime);
    void set_mime(Glib::ustring* new_mime);

    void set_album_artists(std::vector<Glib::ustring*> *new_album_artists);
    void add_album_artist(const char* new_album_artist);
    void add_album_artist(const std::string &new_album_artist);
    void add_album_artist(const Glib::ustring &new_album_artist);
    void add_album_artist(Glib::ustring* new_album_artist);
    void clear_album_artists();

    void set_artists(std::vector<Glib::ustring*> *new_artists);
    void add_artist(const char* new_artist);
    void add_artist(const std::string &new_artist);
    void add_artist(const Glib::ustring &new_artist);
    void add_artist(Glib::ustring* new_artist);
    void clear_artists();

    void set_genres(std::vector<Glib::ustring*> *new_genres);
    void add_genre(const char* new_genre);
    void add_genre(const std::string &new_genre);
    void add_genre(const Glib::ustring &new_genre);
    void add_genre(Glib::ustring* new_genre);
    void clear_genres();

    void set_flags(std::vector<Glib::ustring*> *new_flags);
    void add_flag(const char* new_flag);
    void add_flag(const std::string &new_flag);
    void add_flag(const Glib::ustring &new_flag);
    void add_flag(Glib::ustring* new_flag);
    void clear_flags();

    void set_track_number(int new_track_number);
    void set_track_number(const std::string &new_track_number);

    void set_track_total(int new_track_total);
    void set_track_total(const std::string &new_track_total);

    void set_date(int new_date);
    void set_date(const std::string &new_date);

    void set_disc_number(int new_disc_number);
    void set_disc_number(const std::string &new_disc_number);

    void set_disc_total(int new_disc_total);
    void set_disc_total(const std::string &new_disc_total);

    void set_bit_rate(int new_bit_rate);

    void set_bit_depth(int new_bit_rate);

    void set_sample_rate(int new_sample_rate);

    void set_channels(int new_channels);

    void set_start(long long new_setting);

    void set_pregap_start(long long new_setting);

    void set_end(long long new_setting);

    void set_replaygain_album_gain(double new_setting);

    void set_replaygain_album_peak(double new_setting);

    void set_replaygain_track_gain(double new_setting);

    void set_replaygain_track_peak(double new_setting);

    void set_duration(long long duration);





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  private:    

    TrackType type_;

    Glib::ustring *filename_,
                  *cue_filename_,
                  *pregap_filename_,
                  *album_,
                  *title_,
                  *length_,
                  *comment_,
                  *disc_id_,
                  *catalog_,
                  *isrc_,
                  *codec_,
                  *mime_;
 
    std::vector<Glib::ustring*>* album_artists_,
                               * artists_,
                               * genres_,
                               * flags_;

    int track_number_,
        track_total_,
        date_,
        disc_number_,
        disc_total_,
        bit_rate_,
        bit_depth_,
        sample_rate_,
        channels_;
 
    long long start_,
              pregap_start_,
           end_;

    std::vector<long long> *subindices_;

    double replaygain_album_gain_,
           replaygain_album_peak_,
           replaygain_track_gain_,
           replaygain_track_peak_;

    std::vector<Tag> *tags_;

    long long duration_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
