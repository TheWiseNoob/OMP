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

#ifndef PLAYLISTS_H
#define PLAYLISTS_H





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

#include "Playlist.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "PlaylistColumn.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/refptr.h>

#include <gtkmm/treestore.h>

#include <list>

#include <map>

#include <memory>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

namespace Gtk
{

  class TreeRowReference;

}

class PlaylistChangesCancelDialog;

class PlaylistCreateDialog;

class PlaylistDeleteDialog;

class PlaylistColumnRecord;

class PlaylistsDatabase;

class PlaylistTreeStore;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class Playlists : public GUIElementList<Playlist>
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    Playlists(Base& base);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    virtual ~Playlists();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  public:

    bool Add_Playlist(const char* name);

    void Append_Rows(Glib::RefPtr<PlaylistTreeStore> playlist_treestore,
                     bool database_extraction = false);

    void Change_Track(); 

    void Delete_Current_Playlist
      (bool delete_playlist_combobox_playlist = false);

    bool Editable_Column(const char* column_name);

    void Fill_Row
      (Gtk::TreeRow& new_tree_row, std::shared_ptr<Track> new_track_sptr);

    std::string Find_Column_Name(std::string& column_title);

    std::string Find_Column_Title(const char* column_name);

    std::string Find_Column_Title(std::string& column_name);

    void Flush_Playback_Queue();

    void Open_Changes_Cancel_Dialog(bool release_twice = false);

    void Open_Create_Playlist_Dialog();

    void Open_Delete_Playlist_Dialog
      (bool delete_playlist_combobox_playlist = false);

    void Scroll_To_Row(Gtk::TreeRowReference desired_row_ref,
                       bool only_use_selected_playlist_view = false);

    void Select_Row(Gtk::TreeRowReference desired_row_ref,
                    bool only_use_selected_playlist_view = false);

    void Unselect_Row(Gtk::TreeRowReference desired_row_ref);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //           //
  // Clipboard ////////////////////////////////////////////////////////////////
  //           //

  public:

    std::list<std::shared_ptr<Track>>& clipboard_tracks();





  //         //
  // Columns //////////////////////////////////////////////////////////////////
  //         //

  public:

    std::list<PlaylistColumn>& columns();

    PlaylistColumnRecord& playlist_column_record();





  //          //
  // Database /////////////////////////////////////////////////////////////////
  //          //

  public:

    PlaylistsDatabase& database();





  //       //
  // Flags ////////////////////////////////////////////////////////////////////
  //       //

  public:

    std::atomic<bool>& changing_track();

    std::atomic<bool>& database_extraction_complete();

    bool disable_on_selection_changed();

    bool inserting();

    std::atomic<bool>& new_selection();

    std::atomic<bool>& rebuild_databases();

    std::atomic<bool>& rebuilding_databases();

    bool skip_button_press();





  //           //
  // Playlists ////////////////////////////////////////////////////////////////
  //           //

  public:

    Playlist& selected_playlist();





  //        //
  // Tracks ///////////////////////////////////////////////////////////////////
  //        //

  public:

    Track& empty_track();

    Track& next_track();

    Track& playing_track();

    Track& previous_track();

    Track& queue_track();

    Track& selected_track();





  //                   //
  // TreeRowReferences ////////////////////////////////////////////////////////
  //                   //

  public:

    Gtk::TreeRowReference last_non_queue_row_ref();

    Gtk::TreeRowReference next_playing_row_ref();

    Gtk::TreeRowReference playing_row_ref();

    Gtk::TreeRowReference selected_row_ref();





  //            //
  // TreeStores ///////////////////////////////////////////////////////////////
  //            //

  public:

    Glib::RefPtr<PlaylistTreeStore> hovering_playlist_treestore();

    Glib::RefPtr<PlaylistTreeStore> library_playlist_treestore();
        
    Glib::RefPtr<PlaylistTreeStore> playing_playlist_treestore();

    std::list<Glib::RefPtr<PlaylistTreeStore>>& playlist_treestores();

    Glib::RefPtr<PlaylistTreeStore> queue_playlist_treestore();

    Glib::RefPtr<PlaylistTreeStore> selected_playlist_treestore();
    




  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //       //
  // Flags ////////////////////////////////////////////////////////////////////
  //       //

  public:

    void set_disable_on_selection_changed(bool new_value);

    void set_inserting(bool new_value);

    void set_skip_button_press(bool new_value);





  //           //
  // Playlists ////////////////////////////////////////////////////////////////
  //           //

  public:

    void set_selected_playlist(Playlist& new_selected_playlist);





  //                   //
  // TreeRowReferences ////////////////////////////////////////////////////////
  //                   //

  public:

    void set_last_non_queue_row_ref(Gtk::TreeRowReference new_row_ref);

    void set_selected_row_ref(Gtk::TreeRowReference new_row_ref);





  //            //
  // TreeStores ///////////////////////////////////////////////////////////////
  //            //

  public:

    void set_hovering_playlist_treestore
      (const Glib::RefPtr<PlaylistTreeStore>& new_treestore);





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //           //
  // Clipboard ////////////////////////////////////////////////////////////////
  //           //

  private:

    std::list<std::shared_ptr<Track>> clipboard_tracks_;





  //         //
  // Columns //////////////////////////////////////////////////////////////////
  //         //

  private:

    std::list<PlaylistColumn> columns_;

    PlaylistColumnRecord* playlist_column_record_;





  //          //
  // Datebase ////////////////////////////////////////////////////////////////////
  //          //

  private:

    PlaylistsDatabase* database_;





  //       //
  // Flags ////////////////////////////////////////////////////////////////////
  //       //

  private:

    std::atomic<bool> changing_track_;

    std::atomic<bool> database_extraction_complete_;

    bool disable_on_selection_changed_;

  public:

    bool drag_occurred_;

  private:

    bool inserting_;

    std::atomic<bool> new_selection_;

    std::atomic<bool> rebuild_databases_;

    std::atomic<bool> rebuilding_databases_;

    bool skip_button_press_;





  //                   //
  // Playlist Creation ////////////////////////////////////////////////////////
  //                   //

  private:

    PlaylistChangesCancelDialog* playlist_changes_cancel_dialog_;

    PlaylistCreateDialog* playlist_create_dialog_;

    PlaylistDeleteDialog* playlist_delete_dialog_;





  //           //
  // Playlists ////////////////////////////////////////////////////////////////
  //           //

  private:

    Playlist* selected_playlist_;





  //             //
  // Status Bars //////////////////////////////////////////////////////////////
  //             //

  private:

    // 
    int extracting_playlists_;

    // 
    std::string playlist_status_str_;

    // 
    std::mutex playlist_status_str_mutex_;





  //        //
  // Tracks ///////////////////////////////////////////////////////////////////
  //        //

  private:

    Track* empty_track_;





  //                   //
  // TreeRowReferences ////////////////////////////////////////////////////////
  //                   //

  private:

    Gtk::TreeRowReference* last_non_queue_row_ref_;

    Gtk::TreeRowReference* selected_row_ref_;





  //            //
  // TreeStores ///////////////////////////////////////////////////////////////
  //            //

  private:

    Glib::RefPtr<PlaylistTreeStore> hovering_playlist_treestore_;

    Glib::RefPtr<PlaylistTreeStore> library_playlist_treestore_;

    std::list<Glib::RefPtr<PlaylistTreeStore>> playlist_treestores_;
    
    Glib::RefPtr<PlaylistTreeStore> queue_playlist_treestore_;

    Glib::RefPtr<PlaylistTreeStore> selected_playlist_treestore_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif 
