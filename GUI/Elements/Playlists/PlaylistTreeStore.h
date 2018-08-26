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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef PLAYLIST_TREESTORE_H
#define PLAYLIST_TREESTORE_H





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

#include "../../../Parts.h"

#include <gtkmm/treestore.h>





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <gtkmm/treerowreference.h>

#include <mutex>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class PlaylistColumnRecord;

class Track;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class PlaylistTreeStore : public Gtk::TreeStore, public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  protected:

    PlaylistTreeStore(Base& base, 
                      const PlaylistColumnRecord& columns);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  protected:

    ~PlaylistTreeStore();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    static Glib::RefPtr<PlaylistTreeStore> 
      create(Base& base, const PlaylistColumnRecord& columns);

    void On_Signal_Row_Inserted(const TreeModel::Path& path,
                                const TreeModel::iterator& iter);

    void On_Signal_Rows_Reordered(const TreeModel::Path& path,
                                  const TreeModel::iterator& iter,
                                  int* new_order);

  protected:

    virtual bool row_drop_possible_vfunc
      (const Gtk::TreeModel::Path& dest,
       const Gtk::SelectionData& selection_data) const;

    virtual bool drag_data_received_vfunc
      (const Gtk::TreeModel::Path& dest,
       const Gtk::SelectionData& selection_data);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    std::list<std::pair<int, std::shared_ptr<Track>>>& add_track_queue();

    std::atomic<bool>& cancel_changes();

    std::atomic<bool>& deleting();

    std::atomic<bool>& drag_occurring();

    std::mutex& mutex();

    Gtk::TreeRowReference& new_rows_end_ref();

    Gtk::TreeRowReference& new_rows_start_it();

    std::atomic<bool>& pause_appending();

    std::atomic<bool>& pause_changes();

    std::atomic<bool>& rebuild_database();

    std::atomic<bool>& rebuild_scheduled();

    std::atomic<bool>& rebuilding_database();

    std::atomic<bool>& restart_changes();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  private:

    // 
    std::list<std::pair<int, std::shared_ptr<Track>>> add_track_queue_;

    // 
    std::atomic<bool> cancel_changes_;

    // 
    std::atomic<bool> deleting_;

    // 
    std::atomic<bool> drag_occurring_;

    // 
    std::mutex mutex_;

    // 
    Gtk::TreeRowReference new_rows_end_ref_;

    // 
    Gtk::TreeRowReference new_rows_start_it_;

    // 
    std::atomic<bool> pause_appending_;

    // 
    std::atomic<bool> pause_changes_;

    // 
    std::atomic<bool> rebuild_database_;

    // 
    std::atomic<bool> rebuild_scheduled_;

    // 
    std::atomic<bool> rebuilding_database_;

    // 
    std::atomic<bool> restart_changes_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
