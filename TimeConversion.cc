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





#include "TimeConversion.h"





#include <stdlib.h>
#include <iostream>



using namespace std;



TimeConversion::TimeConversion()
{



}





std::string* TimeConversion::Nanoseconds_To_Time_Format(long long temp_nanoseconds)
{

  //Creates final_time string pointer, which returns before the function ends.

  std::string *final_time = new std::string("");



  //Determines the sign or returns 0 time format if 0.

  bool positive;

  if(temp_nanoseconds > 0LL)
  {

    positive = true;

  }
  else if(temp_nanoseconds < 0LL)
  {

    positive = false;

  }
  else
  {

    *final_time = "00:00.00";

    return final_time;

  }



  //Determine centiseconds of time and assigned it to final time string.
  
  int temp_centiseconds;

  temp_centiseconds = abs(((temp_nanoseconds) % 1000000000) / 10000000);

  if(temp_centiseconds < 10)
  {

    *final_time = "0" + std::to_string(temp_centiseconds);

  }
  else
  {

    *final_time = std::to_string(temp_centiseconds);

  }



  //Determines if temp_nanoseconds is less than a second. Returns if true;

  if(labs(temp_nanoseconds) < 1000000000LL)
  {

    *final_time = "00:00." + *final_time;

    //Adds negative sigh if negative time
    if(positive)
    {

    }
    else
    {

      *final_time = "-" + *final_time;

    }

    return final_time;

  }



  //Determine seconds of time and assigned it to final time string.

  int temp_seconds = abs(((temp_nanoseconds) % 60000000000) / 1000000000);

  if(temp_seconds < 10)
  {

    *final_time = "0" + std::to_string(temp_seconds) + "." + *final_time;

  }
  else
  {

    *final_time = std::to_string(temp_seconds) + "." + *final_time;

  }



  //Determines if temp_nanoseconds is less than a minute. Returns if true;

  if(labs(temp_nanoseconds) < 60000000000)
  {

    *final_time = "00:" + *final_time;

    //Adds negative sigh if negative time
    if(positive)
    {

    }
    else
    {

      *final_time = "-" + *final_time;

    }

    return final_time;

  }



  //Determines minutes of from temp_nanoseconds assigns it 
  //to final_time string.
  	
  int temp_minutes;

  temp_minutes = abs((temp_nanoseconds) / 60000000000);

  if(temp_minutes < 10)
  {

    *final_time = "0" + std::to_string(temp_minutes) + ":" + *final_time;

  }
  else
  {

    *final_time = std::to_string(temp_minutes) + ":" + *final_time;

  }



  //Adds negative sigh if negative time
  if(positive)
  {

  }
  else
  {

    *final_time = "-" + *final_time;

  }



  return final_time;

}





long long TimeConversion::Time_Format_To_Nano_Seconds(const std::string& time,
                                                      bool milliseconds)
{

  // Gets the size of the time string.
  int time_string_length = time.size();

  // Used to store the position in the time string. Starts at the end.
  int pos = time_string_length - 1;

  // The final time to be returned.
  long long converted_time = 0LL;

  // Stores the chara
  std::string temp_num_string = "";



  // 
  temp_num_string = time[pos];

  // 
  pos--;

  temp_num_string = time[pos] + temp_num_string;

  // 
  pos -= 2;

  // 
  if(milliseconds)
  {

    converted_time = stoi(temp_num_string) * 1e7;

  }

  //
  else
  {

    converted_time = stoi(temp_num_string) * 1e9;

  }

  

  // 
  temp_num_string = time[pos];

  //
  pos--;

  if(pos < 0)
  {

    // 
    if(milliseconds)
    {

      converted_time += stoi(temp_num_string) * 1e9;


    }

    //
    else
    {

      converted_time += stoi(temp_num_string) * 1e9 * 60LL;

    }



    // 
    return converted_time;

  }

  //
  temp_num_string = time[pos] + temp_num_string;

  // 
  if(milliseconds)
  {

    converted_time += stoi(temp_num_string) * 1e9;

  }

  //
  else
  {

    converted_time += stoi(temp_num_string) * 1e9 * 60LL;

  }



  // 
  pos--;

  // 
  if(pos < 0)
  {

    // 
    return converted_time;

  }

  // 
  temp_num_string = "";

  //
  pos--;

  //
  while(pos > -1)
  {

    //
    temp_num_string = time[pos] + temp_num_string;

    //
    pos--;

  }

  // 
  if(milliseconds)
  {

    converted_time += stoi(temp_num_string) * 1e9 * 60LL;


  }

  //
  else
  {

    // 
    converted_time += stoi(temp_num_string) * 1e9 * 3600L;

  }



  //
  return converted_time;

}





std::string* TimeConversion::Seconds_To_Time_Format(int temp_seconds)
{

  std::string* time_string = new std::string("");

  int minutes = ((temp_seconds / 60) - ((temp_seconds % 60) / 60)),
      seconds = (temp_seconds % 60);

  if(minutes < 10)
  {

    (*time_string) += "0";

  }

  (*time_string) = std::to_string(minutes);
  (*time_string) += ":";

  if(seconds < 10)
  {

    (*time_string) += "0";

  }

  (*time_string) += std::to_string(seconds);

  return time_string;

}





int TimeConversion::Time_Format_To_Seconds(std::string& time)
{

  int seconds = 0,
      minutes = 0;

  int total_seconds = 0;

  int time_length = int(time.size());

  int seconds_start_position = time_length - 2;

  int minutes_length = time_length - 3; 
 
  seconds = stoi(time.substr(seconds_start_position, 2));

  minutes = stoi(time.substr(0, minutes_length));

  total_seconds += seconds;

  total_seconds += (minutes * 60); 

  return total_seconds;

}
