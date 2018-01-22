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

#include "Tag.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/ustring.h>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

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
