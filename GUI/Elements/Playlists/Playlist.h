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

  class Overlay;

  class ProgressBar;

  class ScrolledWindow;

  class TreePath;

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

    Playlist(Base& base_ref, Playlists& playlists_ref,
             const char* new_playlist_view_name);





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

    bool on_drag_failed
      (const Glib::RefPtr< Gdk::DragContext>& context, Gtk::DragResult result);

    void on_drag_end
      (const Glib::RefPtr< Gdk::DragContext>& context);





  //                 //
  // Mouse Callbacks //////////////////////////////////////////////////////////
  //                 //

  protected:

    virtual bool on_button_press_event(GdkEventButton *event) override final;

    void On_Button_Press_Event_Column_Header();

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

    void Lock();

    void On_Columns_Changed();

    void On_Selection_Changed();





  //                  //
  // Row Manipulators /////////////////////////////////////////////////////////
  //                  //

  public:

    void Copy_Selected_Rows(bool cut = false);

    void Cut_Selected_Rows();

    void Delete_Selected_Rows();

    void Edit();

    void Paste_Clipboard_Rows();

    void Queue_Rows();

    void Select_All_Rows();





  //                  //
  // State Indicators /////////////////////////////////////////////////////////
  //                  //

  public:

    bool Locked();





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  public:

    Glib::ustring active_playlist_name();

    PlaylistMenu& menu();

    PlaylistColumnRecord& playlist_column_record();

    Glib::RefPtr<Gtk::TreeSelection>& playlist_treeselection();

    Glib::RefPtr<PlaylistTreeStore> playlist_treestore();

    std::list<Glib::RefPtr<PlaylistTreeStore>>::iterator 
      playlist_treestore_it();

    const char* playlist_view_name();





  //            //
  // Status Bar ///////////////////////////////////////////////////////////////
  //            //

  public:

    Gtk::Label& name_label();

    Gtk::ProgressBar& progress_bar();

    Gtk::Label& row_count_label();





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

  //         //
  // General //////////////////////////////////////////////////////////////////
  //         //

  private:

    const char* playlist_view_name_;

  public:

    sigc::connection selection_conn_;

    // 
    Gtk::TreePath* right_click_row_tree_path_;

    // Will hold a pointer to the column of the row the mouse is over.
    Gtk::TreeViewColumn* right_click_row_column_;

    // 
    int right_click_x_cell_;

    // 
    int right_click_y_cell_;

    // 
    int right_click_x_;

    // 
    int right_click_y_;






  //       //
  // Flags ////////////////////////////////////////////////////////////////////
  //       //

  private:

    //
    std::atomic<bool> clipboard_event_;

    //
    std::atomic<bool> constructed_;

    //
    std::atomic<bool> selecting_flag_;





  //           //
  // GUI Parts ////////////////////////////////////////////////////////////////
  //           //

  private:

    //
    PlaylistMenu* menu_;

    // 
    Gtk::Box* playlist_box_;

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





  //            //
  // Status Bar ///////////////////////////////////////////////////////////////
  //            //

  public:

    // 
    Gtk::ProgressBar* copy_progress_bar_;

  private:

    // 
    Gtk::Label* name_label_;

    // 
    Gtk::ProgressBar* progress_bar_;

    // 
    Gtk::Box* progress_bars_box_;

    // 
    Gtk::Label* row_count_label_;
 
    //
    Gtk::Box* status_box_;

    //
    Gtk::Frame* status_frame_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
