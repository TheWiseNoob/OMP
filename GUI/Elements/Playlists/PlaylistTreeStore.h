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





#ifndef PLAYLIST_TREESTORE_H
#define PLAYLIST_TREESTORE_H





#include <gtkmm/treestore.h>
#include "../../../Parts.h"





class PlaylistColumnRecord;





class PlaylistTreeStore : public Gtk::TreeStore, public Parts
{

  protected:

    PlaylistTreeStore(Base& base, 
                      const PlaylistColumnRecord& columns);

    ~PlaylistTreeStore();

  public:

    static Glib::RefPtr<PlaylistTreeStore> 
      create(Base& base, const PlaylistColumnRecord& columns);

    void On_Signal_Rows_Reordered(const TreeModel::Path& path,
                                  const TreeModel::iterator& iter,
                                  int* new_order);

    void On_Signal_Row_Inserted(const TreeModel::Path& path,
                                const TreeModel::iterator& iter);

  protected:

//    virtual bool drag_data_get_vfunc(const Gtk::TreeModel::Path& dest,
//                                         const Gtk::SelectionData& selection_data) const;

    virtual bool row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest,
                                         const Gtk::SelectionData& selection_data) const;

    virtual bool drag_data_received_vfunc(const Gtk::TreeModel::Path& dest,
                                          const Gtk::SelectionData& selection_data);

  private:



};





#endif
