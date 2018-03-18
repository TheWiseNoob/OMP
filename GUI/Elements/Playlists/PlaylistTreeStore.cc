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





#include "PlaylistTreeStore.h"





#include "PlaylistColumnRecord.h"
#include "Playlists.h"
#include "../../../Base.h"



#include <iostream>

#include <gtkmm/treepath.h>





PlaylistTreeStore::PlaylistTreeStore(Base& base, const PlaylistColumnRecord& columns)
: Parts(base)
{

  set_column_types(columns);

  signal_rows_reordered()
    .connect(sigc::mem_fun(*this, 
                           &PlaylistTreeStore::On_Signal_Rows_Reordered));
  signal_row_changed()
    .connect(sigc::mem_fun(*this, 
                           &PlaylistTreeStore::On_Signal_Row_Inserted));

}





PlaylistTreeStore::~PlaylistTreeStore()
{



}





Glib::RefPtr<PlaylistTreeStore>
  PlaylistTreeStore::create(Base& base, const PlaylistColumnRecord& columns)
{

  return Glib::RefPtr<PlaylistTreeStore>(new PlaylistTreeStore(base, columns));

}





void PlaylistTreeStore::
  On_Signal_Rows_Reordered(const TreeModel::Path& path,
                           const TreeModel::iterator& iter,
                           int* new_order)
{


}





void PlaylistTreeStore::
  On_Signal_Row_Inserted(const TreeModel::Path& path,
                         const TreeModel::iterator& iter)
{


}




bool PlaylistTreeStore::row_drop_possible_vfunc
  (const Gtk::TreeModel::Path& dest,
   const Gtk::SelectionData& selection_data) const
{

  std::string path_string = dest.to_string();


//  std::cout << "\n\n" << path_string << "\n\n";



  if(!dest)
  {

    std::cout << "\n\nDestination does not exist!\n\n";


    return false;

  }
 
 
//  std::cout << "\n\nReturning true!\n\n"; 

  

  return true;

}





bool PlaylistTreeStore::drag_data_received_vfunc
  (const Gtk::TreeModel::Path& dest,
   const Gtk::SelectionData& selection_data)
{

  return true;

}
