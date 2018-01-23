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





#include "DefaultValues.h"

#include <iostream>



DefaultValues::DefaultValues()
: for_default_it(defaults.end()), rev_default_it(defaults.rend())
{

}





bool DefaultValues::exists(string &setting_name)
{

  find(setting_name);

  if(for_default_it != (defaults.end()))
  {

    return true;

  }

  if(rev_default_it != (defaults.rend()))
  {

    return true;

  }

  return false;

}





void DefaultValues::find(string &setting_name)
{

  for_default_it = defaults.begin();

  rev_default_it = defaults.rbegin();

  int half_length;

  if(((defaults.size()) % 2) == 1)
  {

     half_length = ((defaults.size()) / 2 ) + 1;

  }
  else
  {

    half_length = ((defaults.size()) / 2);

  }


  for(int i = 0; i < half_length; i++)
  {

    if(((*for_default_it).get_name()) == setting_name )
    {

      rev_default_it = defaults.rend(); 


      return;

    }

    if(((*rev_default_it).get_name()) == setting_name)
    {

      for_default_it = defaults.end(); 

      clog << "\n\nfind() setting name: " << rev_default_it -> get_name() << "\n\n";

      return;

    }

    for_default_it++;

    rev_default_it++;

  }

  for_default_it = defaults.end(); 

  rev_default_it = defaults.rend();

}





int DefaultValues::get_int(string &setting_name)
{

  find(setting_name);

  int default_setting;

  if(for_default_it != (defaults.end()))
  {

    ((*for_default_it).get(default_setting));


    return default_setting;

  }

  if(rev_default_it != (defaults.rend()))
  {

    ((*rev_default_it).get(default_setting));


    return default_setting;

  }

  throw No_Default_Found_Exception;

}





double DefaultValues::get_double(string &setting_name)
{

  find(setting_name);

  double default_setting;

  if(for_default_it != (defaults.end()))
  {

    ((*for_default_it).get(default_setting));


    return default_setting;

  }

  if(rev_default_it != (defaults.rend()))
  {

    ((*rev_default_it).get(default_setting));


    return default_setting;

  }

  throw No_Default_Found_Exception;

}





bool DefaultValues::get_bool(string &setting_name)
{

  find(setting_name);

  bool default_setting;

  if(for_default_it != (defaults.end()))
  {

    ((*for_default_it).get(default_setting));


    return default_setting;

  }

  if(rev_default_it != (defaults.rend()))
  {

    ((*rev_default_it).get(default_setting));


    return default_setting;

  }

  throw No_Default_Found_Exception;

}





void DefaultValues::get_list(string &setting_name, std::list<string>& setting_list)
{

  find(setting_name);

  if(for_default_it != (defaults.end()))
  {

    ((*for_default_it).get(setting_list));


  }

  else if(rev_default_it != (defaults.rend()))
  {

    ((*for_default_it).get(setting_list));

  }

  else
  {

    throw No_Default_Found_Exception;

  }

}





string DefaultValues::get_str(string &setting_name)
{

  find(setting_name);

  string default_setting;

  if(for_default_it != (defaults.end()))
  {


    ((*for_default_it).get(default_setting));


    return default_setting;

  }

  if(rev_default_it != (defaults.rend()))
  {

    ((*rev_default_it).get(default_setting));


    return default_setting;

  }

  throw No_Default_Found_Exception;

}





void DefaultValues::print_defaults()
{

  list<DefaultValue>::iterator defaults_it = defaults.begin();

  while(defaults_it != (defaults.end()))
  {

    clog << "\nDefaultValue Name: \"" << defaults_it -> get_name() << "\"\n";
//         << "DefaultValue Value: \"" << defaults_it -> get_value() << "\"\n";

    defaults_it++;

  }

}
