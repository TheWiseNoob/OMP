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





#include "DefaultValues.h"

#include <iostream>



DefaultValues::DefaultValues()
{

}





bool DefaultValues::exists(string &setting_name)
{

  // 
  auto defaults_it = find(setting_name);



  // 
  if(defaults_it != (defaults_ . end()))
  {

    // 
    return true;

  }



  // 
  return false;

}

list<DefaultValue>::iterator DefaultValues::find(string& setting_name)
{

  // 
  for(auto defaults_it = (defaults_ . begin());
      (defaults_it != (defaults_ . end()));
       defaults_it++)
  {

    // 
    if((defaults_it -> get_name()) == setting_name)
    {

      // 
      return defaults_it;

    }

  }



  // 
  return defaults_ . end();

}

int DefaultValues::get_int(string &setting_name)
{

  // 
  auto defaults_it = find(setting_name);

  // 
  int default_setting;



  // 
  if(defaults_it != (defaults_ . end()))
  {

    //
    defaults_it -> get(default_setting);



    // 
    return default_setting;

  }



  // 
  throw No_Default_Found_Exception;

}

double DefaultValues::get_double(string &setting_name)
{

  // 
  auto defaults_it = find(setting_name);

  // 
  double default_setting;



  // 
  if(defaults_it != (defaults_ . end()))
  {

    // 
    defaults_it -> get(default_setting);



    // 
    return default_setting;

  }



  // 
  throw No_Default_Found_Exception;

}

bool DefaultValues::get_bool(string &setting_name)
{

  // 
  auto defaults_it = find(setting_name);

  // 
  bool default_setting;



  // 
  if(defaults_it != (defaults_ . end()))
  {

    // 
    defaults_it -> get(default_setting);



    // 
    return default_setting;

  }



  // 
  throw No_Default_Found_Exception;

}

void DefaultValues::get_list
  (string &setting_name, std::list<string>& setting_list)
{

  // 
  auto defaults_it = find(setting_name);



  // 
  if(defaults_it != (defaults_ . end()))
  {

    // 
    defaults_it -> get(setting_list);



    // 
    return;

  }



  // 
  throw No_Default_Found_Exception;

}

string DefaultValues::get_str(string &setting_name)
{

  // 
  auto defaults_it = find(setting_name);

  // 
  string default_setting;



  // 
  if(defaults_it != (defaults_ . end()))
  {

    // 
    defaults_it -> get(default_setting);



    // 
    return default_setting;

  }



  // 
  throw No_Default_Found_Exception;

}

void DefaultValues::print_defaults()
{

  // 
  for(auto defaults_it : defaults_)
  {

    clog << "\nDefaultValue Name: \"" << defaults_it . get_name() << "\"\n";

  }

}
