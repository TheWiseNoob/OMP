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

#ifndef KEYBOARD_SHORTCUTS_PANEL_H
#define KEYBOARD_SHORTCUTS_PANEL_H





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

#include <string>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class KeyboardShortcutsPanelColumnRecord;

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

  class TreeViewColumn;

}





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class KeyboardShortcutsPanel : public Panel
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    KeyboardShortcutsPanel(Base& base_ref, ConfigurationGUI& new_config_gui,
                 Glib::RefPtr<Gtk::ListStore> new_keyboard_shortcuts_liststore);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    ~KeyboardShortcutsPanel();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    virtual void Apply_Saved_Values() final override;

    void Clear_Key();

    void Load_Default();

    void Update_Key(const char* name, const char* label);

    void Row_Activated(const Gtk::TreePath& path, Gtk::TreeViewColumn* column);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Gtk::Label& keyboard_shortcuts_key_label();





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  Gtk::Button* clear_key_button_;

  Gtk::Box* keyboard_shortcuts_display_box_;

  Gtk::Box* keyboard_shortcuts_display_inner_box_;

  Gtk::Label* keyboard_shortcuts_key_label_;

  Gtk::Label* keyboard_shortcuts_label_;

  Gtk::Button* load_default_button_;





  //               //
  // Keys TreeView ////////////////////////////////////////////////////////////
  //               //

  std::string editing_path_;

  Glib::RefPtr<Gtk::ListStore> keyboard_shortcuts_liststore_;

  Gtk::TreeView* keyboard_shortcuts_treeview_;

  Gtk::Box* keyboard_shortcuts_treeview_box_;

  KeyboardShortcutsPanelColumnRecord* keyboard_shortcuts_treeview_columnrecord_;

  Gtk::Frame* keyboard_shortcuts_treeview_frame_;

  Gtk::ScrolledWindow* keyboard_shortcuts_treeview_scrolled_window_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
