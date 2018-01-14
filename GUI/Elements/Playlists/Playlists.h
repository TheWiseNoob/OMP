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
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <glibmm/refptr.h>

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

namespace Gtk
{

  class TreeRowReference;

}

class PlaylistCreateDialog;

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

    void Append_Rows(std::vector<Track*>* tracks,
                     Glib::RefPtr<PlaylistTreeStore> playlist_treestore,
                     bool append_to_database = true,
                     std::vector<int>* ids = nullptr);

    void Change_Track(); 

    void Fill_Row
      (Gtk::TreeRow& new_tree_row, std::shared_ptr<Track> new_track_sptr);

    void Flush_Playback_Queue();

    void Open_Create_Playlist_Dialog();

    void Scroll_To_Row(Gtk::TreeRowReference desired_row_ref);

    void Select_Row(Gtk::TreeRowReference desired_row_ref);

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

    bool disable_on_selection_changed();

    bool drag_occurring();

    bool inserting();

    std::atomic<bool>& new_selection();

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

    void set_drag_occurring(bool new_value);

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

    PlaylistColumnRecord* playlist_column_record_;





  //       //
  // Flags ////////////////////////////////////////////////////////////////////
  //       //

  private:

    PlaylistsDatabase* database_;





  //       //
  // Flags ////////////////////////////////////////////////////////////////////
  //       //

  private:

    std::atomic<bool> changing_track_;

    bool disable_on_selection_changed_;

    bool drag_occurring_;

    bool inserting_;

    std::atomic<bool> new_selection_;

    bool skip_button_press_;





  //                   //
  // Playlist Creation ////////////////////////////////////////////////////////
  //                   //

  private:

    PlaylistCreateDialog* playlist_create_dialog_;





  //           //
  // Playlists ////////////////////////////////////////////////////////////////
  //           //

  private:

    Playlist* selected_playlist_;





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
