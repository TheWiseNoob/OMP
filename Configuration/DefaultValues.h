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





#ifndef DEFAULT_VALUES_H
#define DEFAULT_VALUES_H





#include "DefaultValue.h"

#include <list>



using namespace std;





class DefaultValues
{

  public:

    enum
    {

      No_Default_Found_Exception

    };

    DefaultValues();

    bool exists(string &setting_name);

    list<DefaultValue>::iterator find(string &setting_name);


    int get_int(string &setting_name);

    double get_double(string &setting_name);

    bool get_bool(string &setting_name);

    void get_list(std::string &setting_name, std::list<std::string>& setting_list);

    string get_str(string &setting_name);


    void print_defaults();



    int get_size()
    { 

      return defaults_ . size(); 

    }

    list<DefaultValue>& defaults()
    {  

      return defaults_;

    }

    list<DefaultValue>::iterator it_begin()
    {  

      return defaults_ . begin(); 

    }

    list<DefaultValue>::iterator it_end()
    {  

      return defaults_ . end(); 

    }

  private:

    list<DefaultValue> defaults_;

};





#endif
