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

#ifndef CONFIGURATION_GUI_H
#define CONFIGURATION_GUI_H





//                   //
//                   //
// Inherited Headers //////////////////////////////////////////////////////////
//                   //
//                   //

#include "../../GUIElement.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <functional>

#include <list>

#include <glibmm/refptr.h>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

// ConfigGUI Parts

class PanelTreeViewColumnRecord;

class Panel;



// gtkmm

namespace Glib
{

  class ustring;

}

namespace Gtk
{

  class Box;

  class Button;

  class Frame;

  class Label;

  class Revealer;

  class ScrolledWindow;

  class TreePath;

  class TreeStore;

  class TreeView;

  class TreeViewColumn;

}



// Panels

class ArtworkPanel;

class GUIPanel;

class KeyboardShortcutsPanel;

class MetadataPanel;

class OutputPanel;

class PlaybackPanel;

class PlaylistPanel;

class ReplayGainPanel;

class ScrobblingPanel;



// Parts

class ConfigurationGUIs;

class Configuration;

class Base;

class GUI;

class Flags;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class ConfigurationGUI : public GUIElement<ConfigurationGUI>
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    ConfigurationGUI(Base& new_main,
                     ConfigurationGUIs& new_gui_element_list);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    virtual ~ConfigurationGUI();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    template <typename ClassType>
    void Add_Apply_Saved_Value_Function
           (ClassType& new_class, void (Panel::*new_func_ptr)())
    {

      std::function<void(void)> temp_func_ptr
        = std::bind(new_func_ptr, &new_class);

      apply_saved_values_list() . push_back(temp_func_ptr);

    }

    void Apply_Saved_Values();

    void Mark_Unsaved_Changes(bool new_value);

  private:

    void Panel_Chooser(const Gtk::TreePath& path, Gtk::TreeViewColumn* column);

    void Panel_Cursor_Changed();






  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  public:

    std::list<std::function<void(void)>>& apply_saved_values_list();





  //                        //
  // Panel TreeView Objects ///////////////////////////////////////////////////
  //                        //

  public:

    PanelTreeViewColumnRecord& panel_treeview_column_record();

    Glib::RefPtr<Gtk::TreeStore>& panel_treeview_treestore();





  //        //
  // Panels ///////////////////////////////////////////////////////////////////
  //        //

  public:

    GUIPanel& gui_panel();

    KeyboardShortcutsPanel& keyboard_shortcuts_panel();

    MetadataPanel& metadata_panel();

    OutputPanel& output_panel();

    PlaybackPanel& playback_panel();

    PlaylistPanel& playlist_panel();

    ReplayGainPanel& replay_gain_panel();

    ScrobblingPanel& scrobbling_panel();





  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  private:

    std::list<std::function<void(void)>> apply_saved_values_list_;





  //                         //
  // Actions Buttons Objects //////////////////////////////////////////////////
  //                         //

  private:

    Gtk::Box* action_buttons_box_;

    Gtk::Frame* action_buttons_frame_;

    Gtk::Button* load_default_values_button_;

    Gtk::Button* save_changes_button_;

    Gtk::Button* undo_changes_button_;





  //        //
  // Panels /////////////////////////////////////////////////////////////////
  //        //

  public:

    ArtworkPanel* artwork_panel_;

    GUIPanel* gui_panel_;

    KeyboardShortcutsPanel* keyboard_shortcuts_panel_;

    MetadataPanel* metadata_panel_;

    OutputPanel* output_panel_;

    PlaybackPanel* playback_panel_;

    PlaylistPanel* playlist_panel_;

    ReplayGainPanel* replay_gain_panel_;

    ScrobblingPanel* scrobbling_panel_;





  //               //
  // Panel Objects //////////////////////////////////////////////////////////
  //               //

  public:

    Gtk::Box* active_panel_box_;

    Glib::ustring* active_panel_name_ustr_;

    Gtk::Box* panel_box_;

    Gtk::Frame* panel_frame_;

    Gtk::Label* panel_frame_label_;

    Gtk::Revealer* panel_revealer_;





  //                        //
  // Panel TreeView Objects /////////////////////////////////////////////////
  //                        //

  public:

    Gtk::TreeView* panel_treeview_;

    Gtk::Box* panel_treeview_box_;

    PanelTreeViewColumnRecord* panel_treeview_column_record_;

    Gtk::Frame* panel_treeview_frame_;

    Gtk::ScrolledWindow* panel_treeview_scrolledwindow_;

    Glib::RefPtr<Gtk::TreeStore> panel_treeview_treestore_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
