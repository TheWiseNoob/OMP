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

#ifndef CONFIGURATION_GUIS_H
#define CONFIGURATION_GUIS_H





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

#include "../../GUIElementList.h"

#include "ConfigurationGUI.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <glibmm/refptr.h>

#include <gtkmm/liststore.h>

#include <gtkmm/textbuffer.h>

#include <gtkmm/treestore.h>

#include <list>

#include <memory>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

class ArtworkPanelColumnRecord;

class KeyboardShortcutsPanelColumnRecord;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class ConfigurationGUIs : public GUIElementList<ConfigurationGUI>
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    ConfigurationGUIs(Base& base);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    virtual ~ConfigurationGUIs();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    void Apply_Saved_Values();

    void Load_Default_Values();

    void Mark_Unsaved_Changes(bool new_value);

    void Open_Configuration();

    void Save_Changes();

    void Undo_Changes();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  public:

    bool disable_functions();

    std::atomic<bool>& unsaved_changes();





  //         //
  // Artwork //////////////////////////////////////////////////////////////////
  //         //

  public:

    Glib::RefPtr<Gtk::ListStore> filename_liststore();





  //                    //
  // Keyboard Shortcuts ///////////////////////////////////////////////////////
  //                    //

  public:

    Glib::RefPtr<Gtk::ListStore> keyboard_shortcuts_liststore();





  //          //
  // Metadata /////////////////////////////////////////////////////////////////
  //          //

  public:

    Glib::RefPtr<Gtk::TextBuffer> error_log_textbuffer();





  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    void set_disable_functions(bool new_setting);





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  private:

    bool disable_functions_;

    std::atomic<bool> unsaved_changes_;





  //         //
  // Artwork //////////////////////////////////////////////////////////////////
  //         //

  private:

    ArtworkPanelColumnRecord* filename_liststore_column_record_;

    Glib::RefPtr<Gtk::ListStore> filename_liststore_;





  //                    //
  // Keyboard Shortcuts ///////////////////////////////////////////////////////
  //                    //

  private:

    KeyboardShortcutsPanelColumnRecord* keyboard_shortcuts_liststore_column_record_;

    Glib::RefPtr<Gtk::ListStore> keyboard_shortcuts_liststore_;





  //          //
  // Metadata /////////////////////////////////////////////////////////////////
  //          //

  private:

    Glib::RefPtr<Gtk::TextBuffer> error_log_textbuffer_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
