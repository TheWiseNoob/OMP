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





#ifndef FILE_CHOOSER_H
#define FILE_CHOOSER_H





#include "../../GUIElement.h"





class FileChoosers;



namespace Gtk
{

  class FileChooserWidget;
  class Box;
  class Button;

}





class FileChooser : public GUIElement<FileChooser>
{

  public:

    FileChooser(Base& base, FileChoosers& file_choosers_ref);

    virtual ~FileChooser();

  //Public Member Functions

    void Use_Selected();

    void On_Current_Folder_Changed_Signal();

  //Getters and Setters

    Gtk::FileChooserWidget& file_chooser()
    {

      return *file_chooser_;

    }

  private:

    Gtk::FileChooserWidget* file_chooser_;

    Gtk::Box* action_box_;

    Gtk::Button* choose_button_;



};






#endif
