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





//                    //
//                    //
//                    //
// Header Guard Start /////////////////////////////////////////////////////////
//                    //
//                    //
//                    //

#ifndef PLAYLIST_H
#define PLAYLIST_H





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

#include "gtkmm/treeview.h"

#include "../../GUIElement.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <atomic>

#include <iostream>

#include <string>

#include <vector>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

// Playlist Parts

class PlaylistColumnRecord;

class PlaylistMenu;

class PlaylistTreeStore;



// gtkmm

namespace Gtk
{

  class Box;

  class Frame;

  class Label;

  class ScrolledWindow;

  class TreeRowReference;

  class TreeSelection;

  class TreeStore;

  class TreeViewColumn;

}



// Parts

class Base;

class GUI;

class Metadata;

class Playback;

class Track;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Playlist : public GUIElement<Playlist>, public Gtk::TreeView
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    Playlist(Base& base_ref, Playlists& playlists_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    virtual ~Playlist();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //                //
  // Drag Callbacks ///////////////////////////////////////////////////////////
  //                //

  protected:

    virtual void on_drag_begin
      (const Glib::RefPtr<Gdk::DragContext>& context) override final;

    void on_drag_data_received_signal
      (const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
       const Gtk::SelectionData& selection_data, guint info, guint time);

    virtual bool on_drag_drop
      (const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
       guint time) override final;

    virtual void on_drag_end
      (const Glib::RefPtr< Gdk::DragContext >& context) override final;





  //               //
  // Key Callbacks ////////////////////////////////////////////////////////////
  //               //

  protected:

    virtual bool on_key_press_event(GdkEventKey* event) override final;

    virtual bool on_key_release_event(GdkEventKey* event) override final;





  //                 //
  // Mouse Callbacks //////////////////////////////////////////////////////////
  //                 //

  protected:

    virtual bool on_button_press_event(GdkEventButton *event) override final;

    virtual bool on_button_release_event(GdkEventButton* event) override final;

    virtual bool on_motion_notify_event
      (GdkEventMotion* motion_event) override final;





  //                  //
  // Playlist Actions /////////////////////////////////////////////////////////
  //                  //

  public:

    void Add_Selected_Tracks_Times();

    void Change_Playlist();

    void Header_Clicked();

    void On_Selection_Changed();





  //                  //
  // Row Manipulators /////////////////////////////////////////////////////////
  //                  //

  public:

    void Copy_Selected_Rows();

    void Cut_Selected_Rows();

    void Delete_Selected_Rows();

    void Paste_Clipboard_Rows();

    void Queue_Rows();





  //                  //
  // State Indicators /////////////////////////////////////////////////////////
  //                  //

  public:

    bool Is_Playing_Track();

    bool Locked();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Glib::ustring active_playlist();

    Gtk::Label& filename_label();

    PlaylistMenu& menu();

    PlaylistColumnRecord& playlist_column_record();

    Glib::RefPtr<Gtk::TreeSelection>& playlist_treeselection();

    Glib::RefPtr<PlaylistTreeStore> playlist_treestore();





  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    void set_playlist_treestore(Glib::RefPtr<PlaylistTreeStore> new_treestore);





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //       //
  // Flags ////////////////////////////////////////////////////////////////////
  //       //

  private:

    //
    std::atomic<bool> clipboard_event_;

    //
    std::atomic<bool> constructed_;

    //
    std::atomic<bool> deleting_;

    //
    std::atomic<bool> selecting_flag_;





  //           //
  // GUI Parts ////////////////////////////////////////////////////////////////
  //           //

  private:

    //
    Gtk::Box* filename_box_;

    //
    Gtk::Frame* filename_frame_;

    //
    Gtk::Label* filename_label_;

    //
    PlaylistMenu* menu_;

    //
    Gtk::Frame* playlist_frame_;

    //
    Gtk::ScrolledWindow* playlist_scrolled_window_;

    //
    Glib::RefPtr<Gtk::TreeSelection> playlist_treeselection_;

    //
    Glib::RefPtr<PlaylistTreeStore> playlist_treestore_;





  //               //
  // Drag and Drop ////////////////////////////////////////////////////////////
  //               //

  private:

    //
    bool cut_tracks_for_drag_;

    //
    unsigned int dest_row_drop_position_;

    //
    Gtk::TreeRowReference* dest_tree_row_ref_;

    //
    Gtk::TreeRowReference* drag_tree_row_ref_;

    //
    bool handle_multirow_drag_;

    //
    long int previous_button_press_event_time_;

    //
    int x_click_;

    //
    int y_click_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
