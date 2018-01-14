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





#ifndef PANEL_TREEVIEW_COLUMN_RECORD_H
#define PANEL_TREEVIEW_COLUMN_RECORD_H





#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>





//Configuration TreeView Model Class 
class PanelTreeViewColumnRecord : public Gtk::TreeModel::ColumnRecord
{
                                                                          
  public:
 
    PanelTreeViewColumnRecord()
    {

      add(name_);
      add(type_);
      add(box_);

    }

    Gtk::TreeModelColumn<Glib::ustring> name_;
    Gtk::TreeModelColumn<Glib::ustring> type_;
    Gtk::TreeModelColumn<Gtk::Box*> box_;

}; 





#endif
