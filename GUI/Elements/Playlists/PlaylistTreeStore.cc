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

#include "PlaylistTreeStore.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "PlaylistColumnRecord.h"

#include "Playlists.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/treepath.h>

#include <iostream>

#include <memory>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

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

PlaylistTreeStore::PlaylistTreeStore
  (Base& base, const PlaylistColumnRecord& columns)

// Inhereted Class

: Parts(base)



// Flags

, deleting_(false)

, drag_occurring_(false)

, pause_appending_(false)

, pause_changes_(false)

, rebuild_database_(false)

, rebuild_finished_(true)

, rebuild_scheduled_(false)

, rebuilding_database_(false)

, restart_changes_(false)

{

  // 
  set_column_types(columns);

  // 
  signal_rows_reordered()
    .connect(sigc::mem_fun(*this, 
                           &PlaylistTreeStore::On_Signal_Rows_Reordered));

  // 
  signal_row_changed()
    .connect(sigc::mem_fun(*this, 
                           &PlaylistTreeStore::On_Signal_Row_Inserted));

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaylistTreeStore::~PlaylistTreeStore()
{



}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

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

  // 
  return true;

} 

bool PlaylistTreeStore::drag_data_received_vfunc
  (const Gtk::TreeModel::Path& dest,
   const Gtk::SelectionData& selection_data)
{

  // 
  if(!dest)
  {

    // 
    return false;

  }

  

  // 
  return true;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

std::list<std::pair<int, shared_ptr<Track>>>&
  PlaylistTreeStore::add_track_queue()
{

  return add_track_queue_;

}

std::atomic<bool>& PlaylistTreeStore::cancel_changes()
{ 

  return cancel_changes_;

}

std::atomic<bool>& PlaylistTreeStore::deleting()
{

  return deleting_;

}

std::atomic<bool>& PlaylistTreeStore::drag_occurring()
{

  return drag_occurring_;

}

std::mutex& PlaylistTreeStore::mutex()
{

  return mutex_;

}

Gtk::TreeRowReference& PlaylistTreeStore::new_rows_end_ref()
{

  return new_rows_end_ref_;

}

Gtk::TreeRowReference& PlaylistTreeStore::new_rows_start_it()
{

  return new_rows_start_it_;

}

std::atomic<bool>& PlaylistTreeStore::pause_appending()
{ 

  return pause_appending_;

}

std::atomic<bool>& PlaylistTreeStore::pause_changes()
{

  return pause_changes_;

}

std::atomic<bool>& PlaylistTreeStore::rebuild_database()
{

  return rebuild_database_;

}

std::atomic<bool>& PlaylistTreeStore::rebuild_scheduled()
{

  return rebuild_scheduled_;

}

std::atomic<bool>& PlaylistTreeStore::rebuilding_database()
{

  return rebuilding_database_;

}

std::atomic<bool>& PlaylistTreeStore::restart_changes()
{

  return restart_changes_;

}
