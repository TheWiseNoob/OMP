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





#ifndef DEFAULT_VALUE_H
#define DEFAULT_VALUE_H




#include<list>

#include <string>
#include <iostream>



using namespace std;





class DefaultValue
{

  public:

    DefaultValue(string new_name);

    DefaultValue(string new_name, int new_value); 
    DefaultValue(string new_name, double new_value); 
    DefaultValue(string new_name, bool new_value);
    DefaultValue(string new_name, string new_value); 
    DefaultValue(string new_name, const char *new_value);
    DefaultValue(string new_name, std::list<std::string>& new_values);

    ~DefaultValue();


    //GETTERS//

    std::string &get_name();


    bool get(int& retriever);
    bool get(double& retriever);
    bool get(bool& retriever);
    bool get(std::list<std::string>& retriever);
    bool get(string& retriever);


    //SETTERS//

    void set_name(std::string new_name)
    { 

      name = new_name; 

    }

    void set_value(int new_value)
    { 

      DefaultValue(name, new_value); 

    }

    void set_value(double new_value)
    { 

      DefaultValue(name, new_value); 

    }

    void set_value(bool new_value)
    { 

      DefaultValue(name, new_value); 

    }

    void set_value(std::list<std::string>& new_values)
    {  

      DefaultValue(name, new_values); 

    }

    void set_value(string new_value)
    { 

      DefaultValue(name, new_value); 

    }

  private:

    string name;

    int *int_value;
    double *double_value;
    bool *bool_value;
    string *string_value;    
    std::list<string> list_value_;

};





#endif
