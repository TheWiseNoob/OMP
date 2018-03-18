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

#ifndef ARTWORK_PANEL_H
#define ARTWORK_PANEL_H





//                   //
//                   //
// Inherited Headers //////////////////////////////////////////////////////////
//                   //
//                   //

#include "../../Panel.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/refptr.h>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class ArtworkPanelColumnRecord;

class ConfigurationGUI;

namespace Gtk
{

  class Box;

  class Button;

  class CellEditable;

  class Frame;

  class ListStore;

  class ScrolledWindow;

  class TreeIter;

  class TreePath;

  class TreeView;

}





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class ArtworkPanel : public Panel
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    ArtworkPanel(Base& base_ref, ConfigurationGUI& new_config_gui,
                 Gtk::TreeRow* parent_row_ptr,
                 Glib::RefPtr<Gtk::ListStore> new_filename_liststore);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~ArtworkPanel();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    void Add_Filename();

    virtual void Apply_Saved_Values() final override;

    void On_Cell_Editable_Editing_Done_Signal();

    void On_Cell_Renderer_Editing_Started_Signal
      (Gtk::CellEditable* editable, const Glib::ustring& path);

    void On_Filename_TreeView_Row_Drag_End_Signal
      (const Glib::RefPtr<Gdk::DragContext>& context);

    void On_List_Store_Row_Changed_Signal
      (const Gtk::TreePath& path, const Gtk::TreeIter& iter);

    void Remove_Filename();




  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //                  //
  // Filename Buttons /////////////////////////////////////////////////////////
  //                  //

  Gtk::Button* add_filename_button_;

  Gtk::Box* filename_buttons_box_;

  Gtk::Button* remove_filename_button_;





  //                   //
  // Filename TreeView ////////////////////////////////////////////////////////
  //                   //

  Glib::RefPtr<Gtk::ListStore> filename_liststore_;

  Gtk::TreeView* filename_treeview_;

  Gtk::Box* filename_treeview_box_;

  ArtworkPanelColumnRecord* filename_treeview_columnrecord_;

  Gtk::Frame* filename_treeview_frame_;

  Gtk::ScrolledWindow* filename_treeview_scrolled_window_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
