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





#include "Track.h"





#include <string.h>
#include <glibmm/ustring.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>





using namespace std;
using namespace Glib;





Tag::Tag(const char* new_name, const char* new_value)
: name_(new ustring(new_name))
, values_(new vector<Glib::ustring*>) 
{ 

  values_ -> push_back(new Glib::ustring(new_value));

}





Tag::Tag(const char* new_name, const std::string& new_value)
: name_(new ustring(new_name))
, values_(new vector<Glib::ustring*>) 
{ 

  values_ -> push_back(new Glib::ustring(new_value));

}





Tag::Tag(const char* new_name, const Glib::ustring& new_value)
: name_(new ustring(new_name))
,  values_(new vector<Glib::ustring*>)
{ 

  values_ -> push_back(new Glib::ustring(new_value));

} 





Tag::Tag(const char* new_name, std::vector<Glib::ustring*>* new_values)
: name_(new ustring(new_name))
, values_(new_values)
{ 

}





//Overladed assignment operator
void Tag::operator =(const Tag& copied_tag)
{

  delete name_;
  name_ = new ustring(copied_tag.name());

  for(auto it : *values_)
  { 

    delete it;

  }
  values_ -> clear();
  for(auto it : copied_tag.values())
  {

    values_ -> push_back(new Glib::ustring(*it));

  }

}





//Overladed not equal to operator
bool Tag::operator !=(const Tag& compared_tag) const
{

  if(*name_ == (compared_tag.name()))
  {

    for(int i = 0; i < int(compared_tag.values().size()); i++)
    {

      if(((*values_)[i]) != (compared_tag.values()[i]))
      {

        return true;

      }
 
    }

    return false;

  }
  else
  { 

    return true;

  }

}





//Overladed equal to operator
bool Tag::operator ==(const Tag& compared_tag) const
{

  if(*name_ == (compared_tag.name()))
  {

    for(int i = 0; i < int(compared_tag.values().size()); i++)
    {

      if(((*values_)[i]) != (compared_tag.values()[i]))
      {

        return false;

       }
 
     }

    return true;

   }
  else
  {

    return false;

  }

 }





Track::Track()
{

  type_ = TrackType::NORMAL;

  filename_ = new ustring("");

  cue_filename_ = new ustring("");

  pregap_filename_ = new ustring("");

  album_ = new ustring;
  *album_ = "";

  title_ = new ustring;
  *title_ = "";

  length_ = new ustring;
  *length_ = "";

  date_ = new ustring;
  *date_ = "";

  comment_ = new ustring;
  *comment_ = "";

  disc_id_ = new ustring;
  *disc_id_ = "";

  catalog_ = new ustring;
  *catalog_ = "";

  isrc_ = new ustring;
  *isrc_ = "";

  codec_ = new ustring;
  *codec_ = "";

  mime_ = new ustring;
  *mime_ = "";

  artists_ = new vector<ustring*>;
  
  genres_ = new vector<ustring*>;

  flags_ = new vector<ustring*>;

  track_number_ = 0;

  track_total_ = 0;

  disc_number_ = 0;

  disc_total_ = 0;

  bit_depth_ = 0;

  bit_rate_ = 0;

  sample_rate_ = 0;

  channels_ = 0;

  start_ = 0;

  pregap_start_ = -1;

  end_ = -1;

  subindices_ = new vector<gint64>;
 
  replaygain_album_gain_ = -1000;
  replaygain_album_peak_ = -1000;
  replaygain_track_gain_ = -1000;
  replaygain_track_peak_ = -1000;

  duration_ = 0LL;

  tags_ = new vector<Tag>;

}





//Copy Constructor
Track::Track(const Track& new_track)
{

  type_ = new_track.type();

  filename_ = new ustring(new_track.filename());

  cue_filename_ = new ustring(new_track.filename());

  pregap_filename_ = new ustring(new_track.filename());

  album_ = new ustring(new_track.album());

  title_ = new ustring (new_track.title());

  length_ = new ustring(new_track.length());

  date_ = new ustring(new_track.date());

  comment_ = new ustring(new_track.comment());

  artists_ = new vector<ustring*>;
  for(auto it : new_track.artists())
  {

    artists_ -> push_back(new ustring(*it));

  }

  genres_ = new vector<ustring*>;
  for(auto it : new_track.genres())
  {

    genres_ -> push_back(new ustring(*it));

  }

  flags_ = new vector<ustring*>;
  for(auto it : new_track.flags())
  {

    flags_ -> push_back(new ustring(*it));

  }

  track_number_ = new_track.track_number();

  track_total_ = new_track.track_total();

  disc_number_ = new_track.disc_number();

  disc_total_ = new_track.disc_total();

  bit_rate_ = new_track.bit_rate();

  bit_depth_ = new_track.bit_depth();

  sample_rate_ = new_track.sample_rate();

  channels_ = new_track.channels();

  disc_id_ = new ustring(new_track.disc_id());

  catalog_ = new ustring(new_track.catalog());

  isrc_ = new ustring(new_track.isrc());

  codec_ = new ustring(new_track.codec());

  mime_ = new ustring(new_track.mime());

  start_ = new_track.start();

  pregap_start_ = new_track.pregap_start();

  end_ = new_track.end();

  subindices_ = new vector<gint64>(new_track.subindices());

  replaygain_album_gain_ = new_track.replaygain_album_gain();

  replaygain_album_peak_ = new_track.replaygain_album_peak();

  replaygain_track_gain_ = new_track.replaygain_track_gain();

  replaygain_track_peak_ = new_track.replaygain_track_peak();

  duration_ = new_track.duration();
  
  tags_ = new vector<Tag>(new_track.tags());

}





Track::~Track()
{

  delete filename_;

  delete cue_filename_;

  delete pregap_filename_;

  delete album_;

  delete title_;

  delete length_;

  delete date_;

  delete comment_;

  delete disc_id_;

  delete catalog_;

  delete isrc_;

  delete codec_;

  delete mime_;

  for(auto it : *artists_)
  {

    delete it;

  }
  delete artists_;

  for(auto it : *genres_)
  {

    delete it;

  }
  delete genres_;

  for(auto it : *flags_)
  {

    delete it;

  }
  delete flags_;

  delete subindices_;

  delete tags_;

}





//Overladed assignment operator
void Track::operator =(const Track& copied_track)
{

  type_ = copied_track.type();

  *filename_ = copied_track.filename();

  *cue_filename_ = copied_track.cue_filename();

  *pregap_filename_ = copied_track.pregap_filename();

  *album_ = copied_track.album();

  *title_ = copied_track.title();

  *length_ = copied_track.length();

  *date_ = copied_track.date();

  *comment_ = copied_track.comment();

  *disc_id_ = copied_track.disc_id();

  *catalog_ = copied_track.catalog();

  *isrc_ = copied_track.isrc();

  *codec_ = copied_track.codec();

  *mime_ = copied_track.mime();

  for(auto it : *artists_)
  {

    delete it;

  }
  artists_ -> clear();
  for(auto it : copied_track.artists())
  {

    artists_ -> push_back(new ustring(*it));

  }

  for(auto it : *genres_)
  {

    delete it;

  }
  genres_ -> clear();
  for(auto it : copied_track.genres())
  {

    genres_ -> push_back(new ustring(*it));

  }

  for(auto it : *flags_)
  {

    delete it;

  }
  flags_ -> clear();
  for(auto it : copied_track.flags())
  {

    flags_ -> push_back(new ustring(*it));

  }

  track_number_ = copied_track.track_number();

  track_total_ = copied_track.track_total();

  disc_number_ = copied_track.disc_number();

  disc_total_ = copied_track.disc_total();

  bit_rate_ = copied_track.bit_rate();

  bit_depth_ = copied_track.bit_depth();

  sample_rate_ = copied_track.sample_rate();

  channels_ = copied_track.channels();

  start_ = copied_track.start();

  pregap_start_ = copied_track.start();

  end_ = copied_track.end();

  replaygain_album_peak_ = copied_track.replaygain_album_gain();

  replaygain_album_peak_ = copied_track.replaygain_album_peak();

  replaygain_track_gain_ = copied_track.replaygain_track_gain();

  replaygain_track_peak_ = copied_track.replaygain_track_peak();

  duration_ = copied_track.duration();

  *tags_ = copied_track.tags();
 
}





//Overladed not equal to operator
bool Track::operator !=(const Track& compared_track) const
{

  if((track_number_ == compared_track.track_number())
       && 
     (*filename_ == compared_track.filename())
    )
  {

    return false;

  }
  else
  {

    return true;

  }

}





//Overladed equal to operator
bool Track::operator ==(const Track& compared_track) const
{

  if((track_number_ == compared_track.track_number())
       && 
     (*filename_ == compared_track.filename())
    )
  {

    return true;

  }
  else
  {

    return false;

  }

}





bool Track::Cue() const
{ 

  if(type_ != TrackType::NORMAL)
  {

    return true;

  }
  else
  {

    return false;

  }

}





bool Track::Cue_Embedded() const
{

  if(type_ != TrackType::NORMAL)
  {

    if(cue_filename_ -> empty())
    {

      return true;

    }

  }

  return false;

}





bool Track::Is_Empty() const
{  

  if(filename_ -> empty())
  {

    return true;

  }
  else
  {

    return false;

  }

}





bool Track::Normal() const
{

  switch(type_)
  {

    case TrackType::NORMAL:
    {

      return true;

    }
    case TrackType::SINGLE_FILE:
    {

      return false;

    }
    case TrackType::MULTIPLE_FILE_GAP_PREPENDED:
    {

    }
    case TrackType::MULTIPLE_FILE_GAP_LEFT_OUT:
    {

    }
    case TrackType::MULTIPLE_FILE_NONCOMPLIANT:
    {

      if(pregap_start_ == -1)
      {

        return true;

      }
      else
      {

        return false;

      }

    }   
 
  }

  return false;

}





bool Track::Pregap() const
{

  return (pregap_start_ != -1);

}





void Track::Print()
{

  cout << "\n"
       << "Title: \"" << *title_ << "\"\n"
       << "Artist(s): " 
       << (((artists_ -> size()) != 0) ? (artists_ -> front() -> c_str()) : (""));
  for(int i = 1; i < int(artists_ -> size()); i++)
  {
  
    cout << "; " << *(artists_ -> at(i));

  }
  cout << "\n"
       << "Album: \"" << album() << "\"\n"
       << "Filename: \"" << filename() << "\"\n";

  if(!(cue_filename().empty()))  
  {

    cout << "Cue Sheet Filename: \"" << cue_filename() << "\"\n";

  }

  if(!(pregap_filename().empty()))  
  {

    cout << "Pregap Filename: \"" << pregap_filename() << "\"\n";

  }
  
  cout << "Genre(s): " 
       << (((genres_ -> size()) != 0) ? (genres_ -> front() -> c_str()) : (""));
  for(int i = 1; i < int(genres_ -> size()); i++)
  {
  
    cout << "; " << *(genres_ -> at(i));

  }
  cout << "\n"
       << "\x1b[1m" << "Length:" << "\x1b[0m" << " \"" <<  length() << "\"\n"
       << "\x1b[1m" << "Track Number:" << "\x1b[0m" << " \"" << track_number() << "\"\n"
       << "\x1b[1m" << "Total Tracks: " << "\x1b[0m" << "\"" << track_total() << "\"\n"
       << "\x1b[1m" << "Disc Number: " << "\x1b[0m" << "\"" << disc_number() << "\"\n"
       << "\x1b[1m" << "Total Discs: " << "\x1b[0m" << "\"" << disc_total() << "\"\n"
       << "\x1b[1m" << "Bit Rate: " << "\x1b[0m" << bit_rate() << " kb/s\n"
       << "\x1b[1m" << "Sample Rate: " << "\x1b[0m" << sample_rate() << " Hz\n"
       << "\x1b[1m" << "Channels: " << "\x1b[0m" << channels() << "\n"
       << "\x1b[1m" << "Date: " << "\x1b[0m" << date() << "\n"
       << "\x1b[1m" << "Disc ID: " << "\x1b[0m" << disc_id() << "\n"
       << "\x1b[1m" << "Catalog Number: " << "\x1b[0m" << catalog() << "\n"
       << "\x1b[1m" << "ISRC: " << "\x1b[0m" << isrc() << "\n"
       << "\x1b[1m" << "Flag(s): " << "\x1b[0m"  
       << (((flags_ -> size()) != 0) ? (flags_ -> front() -> c_str()) : (""));
  for(int i = 1; i < int(flags_ -> size()); i++)
  {
  
    cout << "; " << *(flags_ -> at(i));

  }
  cout << "\n"
       << "\x1b[1m" << "Comment: " << "\x1b[0m" << comment() << "\n"
       << "Is Empty: \"" << (Is_Empty() ? "true" : "false") << "\"\n"
       << "Cue Sheet: \"" << (Cue() ? "true" : "false")  << "\"\n"
       << "Normal: \"" << (Normal() ? "true" : "false")  << "\"\n"
       << "Start(pregap): \"" << pregap_start() << "\"\n"
       << "Start(normal): \"" << start() << "\"\n"
       << "End: \"" << end() << "\"\n";
  if(!(subindices_ -> empty()))
  {

    cout  << "Subindices: \n";

    int index_count = 1;

    for(auto it : *subindices_)
    {

      cout << "|- Subindex " << index_count << ": " << it << "\n";

      index_count++;

    }

  }

  cout << "ReplayGain Album Gain: " 
           << fixed << setprecision(6) << showpos 
           << replaygain_album_gain() << " dB\n"
       << "ReplayGain Album Peak: " << noshowpos 
           << replaygain_album_peak() << "\n"
       << "ReplayGain Track Gain: " 
           << showpos << replaygain_track_gain() << " dB\n"
       << "ReplayGain Track Peak: " << noshowpos 
           << replaygain_track_peak() << "\n";

  (tags_ -> empty()) ? cout << "" : cout << "Tags: \n";
  for(auto it : *tags_)
  {

    cout << "  Tag Name: \"" << it.name() << "\"\n"; 
    for(auto value_it : it.values())
    { 

      cout << "  Value: \"" << *value_it << "\"\n";

    }

  }

  (tags_ -> empty()) ? cout << "" : cout << "\n";

  cout << "Type: ";
  switch(type_)
  {

    case TrackType::NORMAL:
    {

      cout << "NORMAL";

      break;

    }

    case TrackType::SINGLE_FILE:
    {

      cout << "SINGLE_FILE";

      break;

    }

    case TrackType::MULTIPLE_FILE_GAP_PREPENDED:
    {

      cout << "MULTIPLE_FILE_GAP_PREPENDED";

      break;

    }

    case TrackType::MULTIPLE_FILE_GAP_LEFT_OUT:
    {

      cout << "MULTIPLE_FILE_GAP_LEFT_OUT";

      break;

    }

    case TrackType::MULTIPLE_FILE_NONCOMPLIANT:
    {

      cout << "MULTIPLE_FILE_NONCOMPLIANT";

      break;

    }

    default:
    {

      break;

    }

  }

  cout << "\n\n";

}





TrackType Track::type() const
{

  return type_;

}





ustring &Track::filename() const
{

  return *filename_;

}





ustring &Track::cue_filename() const
{

  return *cue_filename_;

}





ustring &Track::pregap_filename() const
{

  return *pregap_filename_;

}





ustring &Track::album() const
{

 return *album_;

}





ustring &Track::title() const
{

  return *title_;

}





ustring &Track::length() const
{

  return *length_;

}





ustring &Track::date() const
{

  return *date_;

}





ustring &Track::comment() const
{

  return *comment_;

}





Glib::ustring &Track::disc_id() const
{

  return *disc_id_;

}





Glib::ustring &Track::catalog() const
{

  return *catalog_;

}





Glib::ustring &Track::isrc() const
{

  return *isrc_;

}





Glib::ustring &Track::codec() const
{

  return *codec_;

}





Glib::ustring &Track::mime() const
{

  return *mime_;

}





std::vector<Glib::ustring*> &Track::artists()
{

  return *artists_;

}





std::vector<Glib::ustring*> &Track::artists() const
{

  return *artists_;

}





Glib::ustring *Track::artists_string()
{

  return Multiple_Values_To_Single_Value(*artists_); 

}





std::vector<Glib::ustring*> &Track::genres()
{

  return *genres_;

}





std::vector<Glib::ustring*> &Track::genres() const
{

  return *genres_;

}





Glib::ustring *Track::genres_string()
{

  return Multiple_Values_To_Single_Value(*genres_); 

}





std::vector<Glib::ustring*> &Track::flags()
{

  return *genres_;

}





std::vector<Glib::ustring*> &Track::flags() const
{

  return *genres_;

}





int Track::track_number() const
{

  return track_number_;

}





int Track::track_total() const
{

  return track_total_;

} 





int Track::disc_number() const
{

  return disc_number_;

}





int Track::disc_total() const
{

  return disc_total_;

}





int Track::bit_rate() const
{

  return bit_rate_;

}





int Track::bit_depth() const
{

  return bit_depth_;

}





int Track::sample_rate() const
{

  return sample_rate_;

}





int Track::channels() const
{

  return channels_;

}





gint64 Track::start() const
{ 

  return start_;

}





gint64 Track::pregap_start() const
{ 

  return pregap_start_;

}





gint64 Track::end() const
{ 

  return end_; 

}





std::vector<gint64> &Track::subindices()
{

  return *subindices_;

}





std::vector<gint64> &Track::subindices() const
{

  return *subindices_;

}





double Track::replaygain_album_gain() const
{

  return replaygain_album_gain_;

}





double Track::replaygain_album_peak() const
{

  return replaygain_album_peak_;

}





double Track::replaygain_track_gain() const
{

  return replaygain_track_gain_;

}





double Track::replaygain_track_peak() const
{

  return replaygain_track_peak_;

}





long long Track::duration() const
{

  return duration_;

}





std::vector<Tag> &Track::tags() const
{

  return *tags_; 

}





std::vector<Tag> &Track::tags()
{

  return *tags_; 

}





//Setters
void Track::set_type(const TrackType new_type)
{

  type_ = new_type;

}





void Track::set_filename(const char* new_filename)
{

  *filename_ = new_filename;

}





void Track::set_filename(const string &new_filename)
{ 

  *filename_ = new_filename;

}





void Track::set_filename(const ustring &new_filename)
{ 

  *filename_ = new_filename;

}





void Track::set_filename(ustring* new_filename)
{ 

  delete filename_;
  filename_ = new_filename;

}





void Track::set_cue_filename(const char* new_filename)
{

  *cue_filename_ = new_filename;

}





void Track::set_cue_filename(const string &new_filename)
{ 

  *cue_filename_ = new_filename;

}





void Track::set_cue_filename(const ustring &new_filename)
{ 

  *cue_filename_ = new_filename;

}





void Track::set_cue_filename(ustring* new_filename)
{ 

  delete cue_filename_;
  cue_filename_ = new_filename;

}





void Track::set_pregap_filename(const char* new_pregap_filename)
{

  *pregap_filename_ = new_pregap_filename;

}





void Track::set_pregap_filename(const string &new_pregap_filename)
{ 

  *pregap_filename_ = new_pregap_filename;

}





void Track::set_pregap_filename(const ustring &new_pregap_filename)
{ 

  *pregap_filename_ = new_pregap_filename;

}





void Track::set_pregap_filename(ustring* new_pregap_filename)
{ 

  delete pregap_filename_;
  pregap_filename_ = new_pregap_filename;

}





void Track::set_album(const char* new_album)
{

  *album_ = new_album;

} 





void Track::set_album(const string &new_album)
{

  *album_ = new_album;

} 





void Track::set_album(const ustring &new_album)
{

  *album_ = new_album;

} 





void Track::set_album(ustring* new_album)
{

  delete album_;
  album_ = new_album;

} 





void Track::set_title(const char* new_title)
{

  *title_ = new_title;

}





void Track::set_title(const string &new_title)
{

  *title_ = new_title;

}





void Track::set_title(const ustring &new_title)
{

  *title_ = new_title;

}





void Track::set_title(ustring* new_title)
{

  delete title_;
  title_ = new_title;

}





void Track::set_length(const char* new_setting)
{

  *length_ = new_setting;

}





void Track::set_length(const string &new_setting)
{

  *length_ = new_setting;

}





void Track::set_length(const ustring &new_setting)
{

  *length_ = new_setting;

}





void Track::set_length(ustring* new_setting)
{

  delete length_;
  length_ = new_setting;

}





void Track::set_date(const char* new_date)
{

  *date_ = new_date;

}





void Track::set_date(const std::string &new_date)
{

  *date_ = new_date;

}





void Track::set_date(const Glib::ustring &new_date)
{

  *date_ = new_date;

}





void Track::set_date(Glib::ustring* new_date)
{

  delete date_;
  date_ = new_date;

}





void Track::set_comment(const char* new_setting)
{

  *comment_ = new_setting;

}





void Track::set_comment(const string &new_setting)
{

  *comment_ = new_setting;

}





void Track::set_comment(const ustring &new_setting)
{

  *comment_ = new_setting;

}





void Track::set_comment(ustring* new_setting)
{

  delete comment_;
  comment_ = new_setting;

}





void Track::set_disc_id(const char* new_setting)
{

  *disc_id_ = new_setting;  

}





void Track::set_disc_id(const std::string &new_setting)
{

  *disc_id_ = new_setting;  

}





void Track::set_disc_id(const Glib::ustring &new_setting)
{

  *disc_id_ = new_setting;  

}





void Track::set_disc_id(Glib::ustring* new_setting)
{

  delete disc_id_;
  disc_id_ = new_setting;  

}





void Track::set_catalog(const char* new_setting)
{

  *catalog_ = new_setting;  

}





void Track::set_catalog(const std::string &new_setting)
{

  *catalog_ = new_setting;  

}





void Track::set_catalog(const Glib::ustring &new_setting)
{

  *catalog_ = new_setting;  

}





void Track::set_catalog(Glib::ustring* new_setting)
{

  delete catalog_;
  catalog_ = new_setting;  

}





void Track::set_isrc(const char* new_setting)
{

  *isrc_ = new_setting;  

}





void Track::set_isrc(const std::string &new_setting)
{

  *isrc_ = new_setting;  

}





void Track::set_isrc(const Glib::ustring &new_setting)
{

  *isrc_ = new_setting;  

}





void Track::set_isrc(Glib::ustring* new_setting)
{

  delete isrc_;
  isrc_ = new_setting;  

}





void Track::set_codec(const char* new_setting)
{

  *codec_ = new_setting;  

}





void Track::set_codec(const std::string &new_setting)
{

  *codec_ = new_setting;  

}





void Track::set_codec(const Glib::ustring &new_setting)
{

  *codec_ = new_setting;  

}





void Track::set_codec(Glib::ustring* new_setting)
{

  delete codec_;
  codec_ = new_setting;  

}





void Track::set_mime(const char* new_setting)
{

  *mime_ = new_setting;  

}





void Track::set_mime(const std::string &new_setting)
{

  *mime_ = new_setting;  

}





void Track::set_mime(const Glib::ustring &new_setting)
{

  *mime_ = new_setting;  

}





void Track::set_mime(Glib::ustring* new_setting)
{

  delete mime_;
  mime_ = new_setting;  

}





void Track::set_artists(std::vector<Glib::ustring*> *new_artists)
{

  delete artists_;
  artists_ = new_artists;

}





void Track::add_artist(const char* new_artist)
{

  artists_ -> emplace_back(new ustring(new_artist));

}





void Track::add_artist(const std::string &new_artist)
{

  artists_ -> emplace_back(new ustring(new_artist));

}





void Track::add_artist(const Glib::ustring &new_artist)
{

  artists_ -> emplace_back(new ustring(new_artist));

}





void Track::add_artist(Glib::ustring* new_artist)
{

  artists_ -> emplace_back(new_artist);

}





void Track::clear_artists()
{

  for(auto it : *artists_)
  {

    delete it;

  }

  artists_ -> clear();

}





void Track::set_genres(std::vector<Glib::ustring*> *new_genres)
{

  delete genres_;
  genres_ = new_genres;

}





void Track::add_genre(const char* new_genre)
{

  genres_ -> emplace_back(new ustring(new_genre));

}





void Track::add_genre(const std::string &new_genre)
{

  genres_ -> emplace_back(new ustring(new_genre));

}





void Track::add_genre(const Glib::ustring &new_genre)
{

  genres_ -> emplace_back(new ustring(new_genre));

}





void Track::add_genre(Glib::ustring* new_genre)
{

  genres_ -> emplace_back(new_genre);

}





void Track::clear_genres()
{

  for(auto it : *genres_)
  {

    delete it;

  }

  genres_ -> clear();

}





void Track::set_flags(std::vector<Glib::ustring*> *new_flags)
{

  delete flags_;
  flags_ = new_flags;

}





void Track::add_flag(const char* new_flag)
{

  flags_ -> emplace_back(new ustring(new_flag));

}





void Track::add_flag(const std::string &new_flag)
{

  flags_ -> emplace_back(new ustring(new_flag));

}





void Track::add_flag(const Glib::ustring &new_flag)
{

  flags_ -> emplace_back(new ustring(new_flag));

}





void Track::add_flag(Glib::ustring* new_flag)
{

  flags_ -> emplace_back(new_flag);

}





void Track::clear_flags()
{

  for(auto it : *flags_)
  {

    delete it;

  }

  flags_ -> clear();

}





void Track::set_track_number(int new_track_number)
{ 

  track_number_ = new_track_number; 

}





void Track::set_track_number(const std::string &new_track_number)
{

  track_number_ = stoi(new_track_number);

}





void Track::set_track_total(int new_track_total)
{ 

  track_total_ = new_track_total; 

}





void Track::set_track_total(const std::string &new_track_total)
{

  track_total_ = stoi(new_track_total);

}





void Track::set_disc_number(int new_disc_number)
{ 

  disc_number_ = new_disc_number; 

}





void Track::set_disc_number(const std::string &new_disc_number)
{

  disc_number_ = stoi(new_disc_number);

}





void Track::set_disc_total(int new_disc_total)
{ 

  disc_total_ = new_disc_total; 

}





void Track::set_disc_total(const std::string &new_disc_total)
{

  disc_total_ = stoi(new_disc_total);

}





void Track::set_bit_rate(int new_bit_rate)
{ 

  bit_rate_ = new_bit_rate;

}





void Track::set_bit_depth(int new_bit_depth)
{ 

  bit_depth_ = new_bit_depth;

}





void Track::set_sample_rate(int new_sample_rate)
{ 

  sample_rate_ = new_sample_rate;

}





void Track::set_channels(int new_channels)
{ 

  channels_ = new_channels;

}





void Track::set_start(gint64 new_setting)
{

  start_ = new_setting;

}





void Track::set_pregap_start(gint64 new_setting)
{

  pregap_start_ = new_setting;

}





void Track::set_end(gint64 new_setting)
{

  end_ = new_setting;

}





void Track::set_replaygain_album_gain(double new_setting)
{

  replaygain_album_gain_ = new_setting;

}





void Track::set_replaygain_album_peak(double new_setting)
{

  replaygain_album_peak_ = new_setting;

}





void Track::set_replaygain_track_gain(double new_setting)
{

  replaygain_track_gain_ = new_setting;

}





void Track::set_replaygain_track_peak(double new_setting)
{

  replaygain_track_peak_ = new_setting;

}





void Track::set_duration(long long new_duration)
{

  duration_ = new_duration;

}





Glib::ustring* Track::Multiple_Values_To_Single_Value
                   (std::vector<Glib::ustring*> &values)
{

  Glib::ustring *values_string = new Glib::ustring;

  if(!(values.empty()))
  {

    *values_string = *values[0];

    for(int i = 1; i < int(values.size()); i++)
    {

      *values_string += "; " + *values[i];

    }

  }


  return values_string;

}
