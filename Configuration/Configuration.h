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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                   //
//                   //
// Inherited Headers //////////////////////////////////////////////////////////
//                   //
//                   //

#include "DefaultValue.h"

#include "DefaultValues.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/ustring.h>

#include <list>

#include <mutex>

#include <stdio.h>

#include <string.h>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

//                            //
//                            //
// Class Forward Declarations /////////////////////////////////////////////////
//                            //
//                            //

namespace libconfig
{

  class Config;

}

class ReturnProxy;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Configuration
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    //Configuration Constructor
    Configuration(int& argc, char *argv[100], std::string new_filename);





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    template <typename DV_Type>
    void add_default(string setting_name, DV_Type new_default)
    { 

      default_values_ . get_defaults()
        . emplace_back(setting_name, new_default); 

    }

    void create_groups(std::string &group_name);

    bool file_exists(std::string &filename); 

    ReturnProxy get(std::string setting_name);

    bool get_bool(std::string& setting_name);

    const char* get_c_str(const char* setting_name);

    double get_double(std::string& setting_name); 

    int get_int(std::string& setting_name);

    void get_list(const char* setting_name, std::list<std::string>& setting_list);

    void get_list(std::string& setting_name, std::list<std::string>& setting_list);

    string get_str(const char* setting_name);

    string get_str(std::string& setting_name);

    void Load_Default_Values();

    void output_default_values_size();
    
    void print_defaults();

    void read_file();

    void set(const char* setting_name, bool setting, bool ignore_lock = false);

    void set(const char* setting_name, const char* setting, bool ignore_lock = false);

    void set(const char* setting_name, double setting, bool ignore_lock = false);

    void set(const char* setting_name, int setting, bool ignore_lock = false);

    void set(const char*, std::list<string>& setting_list, bool ignore_lock = false);

    void set(const char* setting_name, Glib::ustring setting, bool ignore_lock = false);

    void set(std::string& setting_name, bool setting, bool ignore_lock = false);

    void set(std::string& setting_name, double setting, bool ignore_lock = false);

    void set(std::string& setting_name, int setting, bool ignore_lock = false);

    void set(string& setting_name, std::list<string>& setting_list, bool ignore_lock = false);

    void set(std::string& setting_name, std::string& setting, bool ignore_lock = false);

    void write_file(bool ignore_lock = false);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    DefaultValues &default_values()
    {  

      return default_values_; 

    }





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //
   
  private:

    libconfig::Config* config_;

    std::mutex config_mutex_;

    DefaultValues default_values_;

    std::string filename_;

 };





class ReturnProxy
{

  public:

    ReturnProxy(Configuration& config_ptr, std::string& passed_setting_name) 
    : config_(config_ptr)
    , setting_name_(passed_setting_name)
    {}

    operator int() 
    {

      return config_.get_int(setting_name_);

    }

    operator double() 
    {

      return config_.get_double(setting_name_);

    }

    operator bool()
    {

      return config_.get_bool(setting_name_);

    }
 
    operator std::string()
    {

      return string(config_.get_str(setting_name_));

    }

    operator Glib::ustring()
    {

      return string(config_.get_str(setting_name_));

    }

    operator const char*()
    {

      string temp_string = string(config_.get_str(setting_name_));
 
      char *temp_char_ptr = const_cast<char*>(temp_string.c_str());

      return temp_char_ptr;

    }



  private:

    Configuration& config_;
    std::string& setting_name_;

}; 





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
