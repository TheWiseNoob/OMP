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

#include "Configuration.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <algorithm>

#include <iostream>

#include <fstream>

#include <libconfig.h++>

#include <string>

#include <string.h>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

using namespace libconfig;

using namespace std;





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

Configuration::Configuration(int& argc, char *argv[100], string new_filename_)

: config_(new Config)

, filename_(new_filename_)

{

  read_file();

  write_file();


}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

void Configuration::create_groups(string& group_name)
{

  Setting &root = config_ -> getRoot();


  string root_group_str = "";

  size_t rfound = 0,
         found = 0,
         pos = 0;      

  bool multiple = false;

  do
  {

    found = (group_name.substr((pos),200)).find('.');

    if(found == string::npos)
    {

      if(config_ -> exists(group_name))
      {

      }
      else
      {


        if(!multiple)
        {

          Setting& final_group = root.add(group_name, Setting::TypeGroup);

        }
        else
        {

          rfound = group_name.rfind('.');

          root_group_str = (group_name.substr(0, rfound));


          Setting& root_group = config_ -> lookup(root_group_str);

          string final_group_name = group_name.substr((rfound + 1), 
                                                      (group_name.length() - rfound));

          Setting&final_group = root_group.add(final_group_name, Setting::TypeGroup);

        }

      }

      break;

    }
    else
    {

      multiple = true;

      pos += found;

      if(config_ -> exists((group_name.substr(0,pos))))
      {

      }
      else
      {

        try
        {

          root_group_str = (group_name.substr(0,pos));

          rfound = (group_name.substr(0,pos)).rfind('.');

          if(rfound == string::npos)
          {

            Setting &new_group = root.add((group_name.substr(0, pos)), 
                                          Setting::TypeGroup);

          } 
          else
          {

            root_group_str = (group_name.substr(0,(rfound)));

            Setting &root_group = config_ -> lookup(root_group_str);

            Setting &new_group 
              = root_group.add((group_name.substr((rfound + 1), 
                                                  (pos - (rfound + 1)))), 
                               Setting::TypeGroup);

          }

        }
        catch(const SettingNameException &exception)
        {

       //   cerr << "\n\nSettingNameException occurred!\n\n";

        } 
        catch(const SettingNotFoundException &exception)
        {

          //cerr << "\n\nSettingNotFoundException occurred!\n\n";

        }

      }

      pos++;

    }

  }
  while(true);

}

bool Configuration::file_exists(string &filename_)
{

  return bool(std::ifstream(filename_.c_str()));                                
                                                                                
}

ReturnProxy Configuration::get(string setting_name)
{ 

  return ReturnProxy(*this, setting_name);

}

bool Configuration::get_bool(string& setting_name) 
{

  lock_guard<mutex> config_mutex__lock(config_mutex_);


  if((config_ -> exists(setting_name)))
  {

//    d ? clog << "\n\nSetting exists!\n\n" : false;

    return(bool(config_ -> lookup(setting_name)));

  }
  else
  {

    if(default_values_.exists(setting_name))
    {


      bool setting = default_values_.get_bool(setting_name);

      set(setting_name, setting, true);

      write_file(true);

      return setting;

    }
    else
    {


      return false;

    }

  }

}

const char *Configuration::get_c_str(const char *setting_name)
{

  return get_str(setting_name).c_str(); 

}

double Configuration::get_double(string& setting_name) 
{

  lock_guard<mutex> config_mutex__lock(config_mutex_);

  Setting &root = config_ -> getRoot();


  if((config_ -> exists(setting_name)))
  {


    return(double(config_ -> lookup(setting_name)));

  }
  else
  {

    if(default_values_.exists(setting_name))
    {


      double setting = default_values_.get_double(setting_name);

      set(setting_name, setting, true);

      write_file(true);

      return setting;

    }
    else
    {


      return 420.69;

    }

  }

}

int Configuration::get_int(string& setting_name) 
{

  lock_guard<mutex> config_mutex__lock(config_mutex_);




  if((config_ -> exists(setting_name)))
  {



    return(int(config_ -> lookup(setting_name)));

  }
  else
  {

    if(default_values_.exists(setting_name))
    {


      int setting = default_values_.get_int(setting_name);

      set(setting_name, setting, true);

      write_file(true);

      return setting;

    }
    else
    {


      return 420;

    }

  }

}

void Configuration::get_list(const char* setting_name, list<string>& setting_list)
{

  string setting_name_str = setting_name;

  get_list(setting_name_str, setting_list);

}

void Configuration::get_list(string& setting_name, list<string>& setting_list)
{

  lock_guard<mutex> config_mutex_lock(config_mutex_);

  Setting& root = config_ -> getRoot();


  if((config_ -> exists(setting_name)))
  {


    Setting& setting = config_ -> lookup(setting_name);

    int setting_length = setting . getLength();

    for(int i = 0; i < setting_length; i++)
    {

      setting_list . push_back(setting[i]);

    } 


  }
  else
  {

    // 
    if(default_values_ . exists(setting_name))
    {

      // 
      default_values_ . get_list(setting_name, setting_list);



      // 
      set(setting_name, setting_list, true);



      // 
      write_file(true);

    }

    // 
    else
    {

    }

  }

} 

string Configuration::get_str(const char *setting_name)
{

  string new_setting_name = setting_name;

  return (get_str(new_setting_name));

}

string Configuration::get_str(string& setting_name) 
{ 

  lock_guard<mutex> config_mutex__lock(config_mutex_);

  Setting &root = config_ -> getRoot();


  if((config_ -> exists(setting_name)))
  {


    return(string(config_ -> lookup(setting_name).c_str()));

  }
  else
  {

    if(default_values_.exists(setting_name))
    {


      string setting = default_values_.get_str(setting_name);

      set(setting_name, setting, true);

      write_file(true);

      return setting;

    }
    else
    {


      return "YOU FUCKED UP!";

    }

  }

} 

void Configuration::Load_Default_Value(const char* value_name)
{

  // 
  string value_name_str = value_name;



  // 
  set(value_name, default_values_ . get_str(value_name_str));

}

void Configuration::Load_Default_Values()
{

  lock_guard<mutex> config_mutex_lock(config_mutex_);


  delete config_;

  config_ = new Config;


  write_file(true);

}

void Configuration::output_default_values_size()
{  


}

void Configuration::print_defaults()
{ 

  default_values_ . print_defaults();

} 

void Configuration::read_file()
{

  if(file_exists(filename_))
  {


    try
    {

      lock_guard<mutex> config_mutex__lock(config_mutex_);

      config_ -> readFile(filename_.c_str());


    }
    catch(const FileIOException &fioex)
    {

      //cerr << "I/O error while reading file." << endl;

    }
    catch(const ParseException &pex)
    {

      //cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
        //        << " - " << pex.getError() << endl;

    }

  }
  else
  {

    Load_Default_Values();

    write_file();    

  }

}

void Configuration::set(const char* setting_name, bool setting, bool ignore_lock)
{

  string new_setting_name = setting_name; 

  set(new_setting_name, setting, ignore_lock); 

}

void Configuration::set(const char* setting_name, const char* setting, bool ignore_lock)
{ 

  string new_setting_name = setting_name,
         new_setting = setting; 

  set(new_setting_name, new_setting, ignore_lock);

}

void Configuration::set(const char* setting_name, double setting, bool ignore_lock)
{

  string new_setting_name = setting_name; 

  set(new_setting_name, setting, ignore_lock);

}

void Configuration::set(const char* setting_name, int setting, bool ignore_lock)
{ 

  string new_setting_name = setting_name; 

  set(new_setting_name, setting, ignore_lock); 

}

void Configuration::set(const char* setting_name, list<string>& setting_list, bool ignore_lock)
{

  string setting_name_str = setting_name;



  set(setting_name_str, setting_list, ignore_lock);

}

void Configuration::set(const char* setting_name, Glib::ustring setting, bool ignore_lock)
{ 

  string new_setting_name = setting_name,
         new_setting = setting; 

  set(new_setting_name, new_setting, ignore_lock); 

}

void Configuration::set(string& setting_name, bool setting, bool ignore_lock) 
{

  if(!ignore_lock)
  {

    config_mutex_ . lock();

  }


  //Stores the possition of the last '.' or string::npos if no '.' is present
  size_t found = setting_name.substr(0, 200).rfind('.');

  Setting &root = config_ -> getRoot();

  if(found != string::npos)
  {

    //Retrived the group name using a substring and found.
    string group_names = setting_name.substr(0,found);

    //Creates the group or group or does nothing if it exists.
    create_groups(group_names);

    //Creates a libconfig_::Setting ref variable to the most embedded group.
    Setting& groups = config_ -> lookup(group_names);


    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting &retrieved_setting = config_ -> lookup(setting_name) = setting;

    }
    else
    {
      Setting &new_setting = groups.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeBoolean)
        = setting;

    }


  }
  else
  {

    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting &retrieved_setting = config_ -> lookup(setting_name) = setting;

    }
    else
    {
      Setting &new_setting = root.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeBoolean)
        = setting;

    }

  }



  if(!ignore_lock)
  {

    config_mutex_ . unlock();

  }

}

void Configuration::set(string& setting_name, double setting, bool ignore_lock) 
{

  if(!ignore_lock)
  {

    config_mutex_ . lock();

  }

  //Stores the possition of the last '.' or string::npos if no '.' is present
  size_t found = setting_name.substr(0, 200).rfind('.');

  Setting &root = config_ -> getRoot();



  if(found != string::npos)
  {

    //Retrived the group name using a substring and found.
    string group_names = setting_name.substr(0,found);

    //Creates the group or group or does nothing if it exists.
    create_groups(group_names);

    //Creates a libconfig_::Setting ref variable to the most embedded group.
    Setting& groups = config_ -> lookup(group_names);


    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting &retrieved_setting = config_ -> lookup(setting_name) = setting;

    }
    else
    {
      Setting &new_setting = groups.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeFloat)
        = setting;

    }


  }
  else
  {

    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting &retrieved_setting = config_ -> lookup(setting_name) = setting;

    }
    else
    {
      Setting &new_setting = root.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeFloat)
        = setting;

    }

  }



  if(!ignore_lock)
  {

    config_mutex_ . unlock();

  }

}

void Configuration::set(string& setting_name, int setting, bool ignore_lock) 
{

  if(!ignore_lock)
  {

    config_mutex_ . lock();

  }

  //Stores the possition of the last '.' or string::npos if no '.' is present
  size_t found = setting_name.substr(0, 200).rfind('.');

  Setting &root = config_ -> getRoot();



  if(found != string::npos)
  {

    //Retrived the group name using a substring and found.
    string group_names = setting_name.substr(0,found);

    //Creates the group or group or does nothing if it exists.
    create_groups(group_names);

    //Creates a libconfig_::Setting ref variable to the most embedded group.
    Setting& groups = config_ -> lookup(group_names);


    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting &retrieved_setting = config_ -> lookup(setting_name) = setting;

    }
    else
    {
      Setting &new_setting = groups.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeInt)
        = setting;

    }


  }
  else
  {

    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting &retrieved_setting = config_ -> lookup(setting_name) = setting;

    }
    else
    {
      Setting &new_setting = root.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeInt)
        = setting;

    }

  }

  if(!ignore_lock)
  {

    config_mutex_ . unlock();

  }

}

void Configuration::set(string& setting_name, list<string>& setting_list, bool ignore_lock)
{

  if(!ignore_lock)
  {

    config_mutex_ . lock();

  }


  //Stores the possition of the last '.' or string::npos if no '.' is present
  size_t found = setting_name.substr(0, 200).rfind('.');

  Setting &root = config_ -> getRoot();

  if(found != string::npos)
  {

    //Retrived the group name using a substring and found.
    string group_names = setting_name.substr(0,found);

    //Creates the group or group or does nothing if it exists.
    create_groups(group_names);

    //Creates a libconfig_::Setting ref variable to the most embedded group.
    Setting& groups = config_ -> lookup(group_names);


    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting& retrieved_setting = config_ -> lookup(setting_name);


      int retrieved_setting_length = retrieved_setting . getLength();

      for(int index = retrieved_setting_length - 1; index >= 0; index--)
      {

        retrieved_setting . remove(index);

      }


      for(auto setting_list_it : setting_list)
      {

        retrieved_setting . add(Setting::TypeString) = setting_list_it;

      }

    }
    else
    {

      Setting& new_setting = groups.add(setting_name.substr((found+1), 200), 
                                        Setting::TypeArray);


      for(auto setting_list_it : setting_list)
      {

        new_setting . add(Setting::TypeString) = setting_list_it;



      }

    }


  }
  else
  {

    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting& retrieved_setting = config_ -> lookup(setting_name);


      int retrieved_setting_length = retrieved_setting . getLength();

      for(int index = retrieved_setting_length - 1; index >= 0; index--)
      {

        retrieved_setting . remove(index);

      }


      for(auto setting_list_it : setting_list)
      {

        retrieved_setting . add(Setting::TypeString) = setting_list_it;

      }

    }
    else
    {

      int index = 0;


      Setting& new_setting = root.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeArray);



      for(auto setting_list_it : setting_list)
      {

        new_setting[index] = setting_list_it;


        index++;

      }

    }

  }


  if(!ignore_lock)
  {

    config_mutex_ . unlock();

  }

}

void Configuration::set(string& setting_name, string& setting, bool ignore_lock) 
{

  if(!ignore_lock)
  {

    config_mutex_ . lock();

  }

  //Stores the possition of the last '.' or string::npos if no '.' is present
  size_t found = setting_name.substr(0, 200).rfind('.');

  Setting &root = config_ -> getRoot();



  if(found != string::npos)
  {

    //Retrived the group name using a substring and found.
    string group_names = setting_name.substr(0,found);

    //Creates the group or group or does nothing if it exists.
    create_groups(group_names);

    //Creates a libconfig_::Setting ref variable to the most embedded group.
    Setting& groups = config_ -> lookup(group_names);


    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting &retrieved_setting = config_ -> lookup(setting_name) = setting;

    }
    else
    {
      Setting &new_setting = groups.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeString)
        = setting;

    }


  }
  else
  {

    //Creates new setting and sets the value.
    if(config_ -> exists(setting_name))
    {

      Setting &retrieved_setting = config_ -> lookup(setting_name) = setting;

    }
    else
    {
      Setting &new_setting = root.add(setting_name.substr((found+1), 200), 
                                      Setting::TypeString)
        = setting;

    }

  }

  if(!ignore_lock)
  {

    config_mutex_ . unlock();

  }

}

void Configuration::write_file(bool ignore_lock)
{

  if(ignore_lock)
  {

    // Write out the updated config_.ration.
    try
    {

      config_ -> writeFile(filename_.data());
//      cerr << "Updated configuration successfully written to: " << filename_
//           << endl;

    }
    catch(const FileIOException &fioex)
    {

//      cerr << "I/O error while writing file: " << filename_ << endl;

    }

  }

  else
  {

    lock_guard<mutex> config_mutex__lock(config_mutex_);

    // Write out the updated config_.ration.
    try
    {

      config_ -> writeFile(filename_.data());
      //cerr << "Updated config_uration successfully written to: " << filename_
        //   << endl;

    }
    catch(const FileIOException &fioex)
    {

      //cerr << "I/O error while writing file: " << filename_ << endl;

    }

  }

}
