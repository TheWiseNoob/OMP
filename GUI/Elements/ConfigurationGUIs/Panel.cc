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

#include "Panel.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Configuration/Configuration.h"

#include "../../GUI.h"

#include "ConfigurationGUI.h"

#include "PanelTreeViewColumnRecord.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/box.h>

#include <gtkmm/treeiter.h>

#include <gtkmm/treestore.h>

#include <iostream>





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

Panel::Panel(Base& base_ref, ConfigurationGUI& config_gui_ref,
             const char* panel_name, Gtk::TreeRow* parent_row_ptr, bool debug)

// Inherited Class
: Parts(base_ref, debug)



// Other

, config_gui_(config_gui_ref)

{

  // True if the row doesn't have a parent.
  if(parent_row_ptr == nullptr)
  {

    // Appends a new row and assigns it to the row_ variable.
    row_  = *(config_gui_ . panel_treeview_treestore() -> append());



    // Adds a name to the row.
    row_[config_gui() . panel_treeview_column_record() . name_] = panel_name;

    // Adds whether the row is for a main part of the program or a plugin.
    row_[config_gui() . panel_treeview_column_record() . type_] = "Main";

    // Adds a copy of the panel box to the row data.
    row_[config_gui() . panel_treeview_column_record() . box_] = this;

  }

  // True if the row has a parent.
  else
  {

    // Appends a new row in the parent and assigns it to the row_ variable.
    row_  = *(config_gui_ . panel_treeview_treestore() 
                              -> append(parent_row_ptr -> children()));



    // Adds a name to the row.
    row_[config_gui() . panel_treeview_column_record() . name_] = panel_name;

    // Adds whether the row is for a main part of the program or a plugin.
    row_[config_gui() . panel_treeview_column_record() . type_] = "Main";

    // Adds a copy of the panel box to the row data.
    row_[config_gui() . panel_treeview_column_record() . box_] = this;

  }



  // Sets the orientation of the box to vertical.
  set_orientation(Gtk::Orientation::VERTICAL);

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

Panel::~Panel()
{

}





//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

Gtk::Box &Panel::box()
{

  return *this;

}

ConfigurationGUI &Panel::config_gui()
{

  return config_gui_;

}

Gtk::TreeRow &Panel::row()
{

  return row_;

}
