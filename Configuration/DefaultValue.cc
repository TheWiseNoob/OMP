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





#include "DefaultValue.h"





DefaultValue::DefaultValue(string new_name)
: name(new_name), int_value(NULL), double_value(NULL), bool_value(NULL), string_value(NULL)
{ 

}





DefaultValue::DefaultValue(string new_name, int new_value) 
: DefaultValue(new_name)
{

  delete int_value;

  int_value = new int(new_value);

}





DefaultValue::DefaultValue(string new_name, double new_value) 
: DefaultValue(new_name)
{

  delete double_value;

  double_value = new double(new_value);

}





DefaultValue::DefaultValue(string new_name, bool new_value) 
: DefaultValue(new_name)
{

  delete bool_value;

  bool_value = new bool(new_value);

}





DefaultValue::DefaultValue(string new_name, list<string>& new_values)
: DefaultValue(new_name)
{ 

  list_value_ . clear();

  for(auto new_value_it : new_values)
  {

    list_value_ . push_back(new_value_it);

  }

}




DefaultValue::DefaultValue(string new_name, string new_value) 
: DefaultValue(new_name)
{

  delete string_value;

  string_value = new string(new_value);


}





DefaultValue::DefaultValue(string new_name, const char *new_value) 
: DefaultValue(new_name)
{

  delete string_value;

  string_value = new string(new_value);


}





DefaultValue::~DefaultValue()
{

  if(string_value != NULL)
  {

    delete string_value;

    return;

  }
  if(int_value != NULL)
  {

    delete int_value;

    return;

  }
  if(double_value != NULL)
  {

    delete double_value;

    return;

  }
  if(bool_value != NULL)
  {

    delete bool_value;

    return;

  }

}





//GETTERS//
string &DefaultValue::get_name()
{ 

  return name; 

}





bool DefaultValue::get(int& retriever)
{ 
 
  if(int_value == NULL)
  {

    return false;

  }
  else
  {

    retriever = *int_value;        

    return true;

  }

 }





bool DefaultValue::get(double& retriever)
{ 
 
  if(double_value == NULL)
  {

    return false;

  }
  else
  {

    retriever = *double_value;

    return true;

  }

}





bool DefaultValue::get(bool& retriever)
{ 

  if(bool_value == NULL)
  {

    return false;

  }
  else
  {

    retriever = *bool_value;  

    return true;

  }

}





bool DefaultValue::get(list<string>& retriever)
{ 

  for(auto list_value_it : list_value_)
  {
 
    retriever . push_back(list_value_it); 

  }

  return true;

}





bool DefaultValue::get(string& retriever)
{ 
 
  if(string_value == NULL)
  {


    return false;

  }
  else
  {


    retriever = *string_value;        

    return true;

  }

}
