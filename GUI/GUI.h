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

#ifndef GUI_H
#define GUI_H





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//                 //
//                 //
// Inherited Class ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Parts.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <functional>

#include <list>

#include <vector>

#include <memory>

#include <mutex>

#include <string>





//                      //
//                      //
//                      //
// Forward Declarations ///////////////////////////////////////////////////////
//                      //
//                      //
//                      //

//         //
//         //
// Classes ////////////////////////////////////////////////////////////////////
//         //
//         //

class Abouts;

class Artwork;

class Base;

class ChildWindow;

class ChildWindows;

class Configuration;

class ConfigurationGUIs;

class FileChooser;

class FileChoosers;

class Flags;

namespace Glib
{

  class ustring;

}

namespace Gtk
{

  class Box;

  class Button;

  class EventBox;

  class Frame;

  class HeaderBar;

  class Label;

  class Notebook;

  class Paned;

  class SpinButton;

  class StackSwitcher;

  class VolumeButton;

  class Window;

}

class MainMenus;

class Metadata;

class Playback;

class PlaybackController;

class PlaybackControllers;

class PlaylistComboBoxes;

class Playlists;

class Seekbar;

class StatusBar;

class Tagview;

class Track;





//         //
//         //
// Structs ////////////////////////////////////////////////////////////////////
//         //
//         //

//           //
// GStreamer //////////////////////////////////////////////////////////////////
//           //

struct _GdkEventConfigure;
typedef struct _GdkEventConfigure GdkEventConfigure;

struct _GdkEventConfigure;
typedef struct _GdkEventConfigure GdkEventConfigure;

struct _GdkEventKey;
typedef struct _GdkEventKey GdkEventKey;

struct _GdkEventCrossing;
typedef struct _GdkEventCrossing GdkEventCrossing;

struct _GdkEventButton;
typedef struct _GdkEventButton GdkEventButton;

struct _GdkEventAny;
typedef struct _GdkEventAny GdkEventAny;





//                   //
//                   //
//                   //
// Class Declaration //////////////////////////////////////////////////////////
//                   //
//                   //
//                   //

class GUI : public Parts
{

  //             //
  //             //
  // Constructor //////////////////////////////////////////////////////////////
  //             //
  //             //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:  
    //
    //   Creates the class.
    //
    //
    //
    // Arguments: 
    //
    //   base: passes a reference to the Base of the program.
    //
    //////////////////////////////////////////////////////////////////////// */
    GUI(Base& base_ref);





  //            //
  //            //
  // Destructor ///////////////////////////////////////////////////////////////
  //            //
  //            //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Cleans up anything that needs it when the GUI is being destroyed.
    //
    //
    //
    // Arguments: 
    //
    //   None
    //
    //////////////////////////////////////////////////////////////////////// */
    virtual ~GUI();





  //                  //
  //                  //
  // Member Functions /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //        //
  // Normal ///////////////////////////////////////////////////////////////////
  //        //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose:
    //
    //   Returns whether the file provided exists or not.
    //
    //
    //
    // Arguments: 
    //
    //   filename_ref: Holds the filename and path of the file being checked.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool File_Exists(std::string& filename_ref);

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Called when the main window is closed. Cleans up program.
    // 
    //
    //
    // Arguments: 
    //
    //   event: Holds the event type and GDK window.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool On_GUI_Window_Signal_Delete_Event(GdkEventAny* event);

    void On_Main_Window_Check_Resize_Signal();

    bool On_Double_Playlist_Paned_Button_Release_Event
      (GdkEventButton* release_event);

    bool On_File_Chooser_Playlist_Paned_Button_Release_Event
      (GdkEventButton* release_event);

    bool On_Main_Content_Paned_Button_Release_Event
      (GdkEventButton* release_event);

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Called by the window delete function when the main window is closed. 
    //   Cleans up program.
    //
    // 
    //
    // Arguments: 
    //
    //   None.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Quit(bool release_twice = false);





  //          //
  // Artworks /////////////////////////////////////////////////////////////////
  //          //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Sets a new cover art for the artworks.
    //
    // 
    //
    // Arguments: 
    //
    //   filename_ref: String that contains the full filename and path of the
    //   music file that is active.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Load_Cover_Art(std::string& filename_ref);





  //         //
  // Tagview //////////////////////////////////////////////////////////////////
  //         //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Updates the TagViews.
    //
    // 
    //
    // Arguments: 
    //
    //   tag_frame_label_name: String to update the TagView's frame label.
    //
    //   new_track: The Track used to update the TrackViews.
    //
    //////////////////////////////////////////////////////////////////////// */
    void Update_Tagview(const char* tag_frame_label_name, Track& new_track);





  //         //
  //         //
  // Getters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //                //
  // Normal Getters ///////////////////////////////////////////////////////////
  //                //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Used for disabling the Menubar's functions.
    //
    //////////////////////////////////////////////////////////////////////// */
    bool& disable_menubar_functions_flag();

    Gtk::Box& duplicates_box();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Returns the recorded duration of the file.
    //
    //////////////////////////////////////////////////////////////////////// */
    long long duration();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Returns the recorded position of the file.
    //
    //////////////////////////////////////////////////////////////////////// */
    long long position();





  //                      //
  // GUIElements Getters //////////////////////////////////////////////////////
  //                      //

  public:

    Abouts& abouts();

    ChildWindows& windows();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   GUIElementList for the ConfigurationGUIs.
    //
    //////////////////////////////////////////////////////////////////////// */
    ConfigurationGUIs& config_guis();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   GUIElementList for the FileChoosers.
    //
    //////////////////////////////////////////////////////////////////////// */
    FileChoosers& file_choosers();

    MainMenus& main_menus();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   GUIElementList for the Playlists.
    //
    //////////////////////////////////////////////////////////////////////// */
    Playlists& playlists();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   GUIElementList for the Playlists.
    //
    //////////////////////////////////////////////////////////////////////// */
    PlaylistComboBoxes& playlist_comboboxes();





  //                               //
  // Future GUIElementList Getters ////////////////////////////////////////////
  //                               //

  public:

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Seekbar for the program.
    //
    //////////////////////////////////////////////////////////////////////// */
    Seekbar& seekbar();

    StatusBar& status_bar();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Returns reference to the list of the TagViews.
    //
    //////////////////////////////////////////////////////////////////////// */
    std::list<Tagview*>& tagviews();





  //          //
  // Notebook /////////////////////////////////////////////////////////////////
  //          //

  public:

    Gtk::Notebook& main_window_notebook();





  //         //
  // Windows //////////////////////////////////////////////////////////////////
  //         //

  public:

    bool fullscreen();

    /* ////////////////////////////////////////////////////////////////////////
    //
    // Purpose: 
    //
    //   Returns a new shared_ptr to the main window.
    //
    //////////////////////////////////////////////////////////////////////// */
    ChildWindow* main_window();





  //         //
  //         //
  // Setters //////////////////////////////////////////////////////////////////
  //         //
  //         //

  //         //
  // Windows //////////////////////////////////////////////////////////////////
  //         //

  public:

    void set_fullscreen(bool new_value);





  //                  //
  //                  //
  // Member Variables /////////////////////////////////////////////////////////
  //                  //
  //                  //

  //                 //
  // GUIElementLists //////////////////////////////////////////////////////////
  //                 //

  private:

    Abouts* abouts_;

    ChildWindows* windows_;

    ConfigurationGUIs* config_guis_;

    FileChoosers* file_choosers_;

    MainMenus* main_menus_;

    Playlists* playlists_;

    PlaybackControllers* playback_controllers_;

    PlaylistComboBoxes* playlist_comboboxes_;





  //                        //
  // Future GUIElementLists ///////////////////////////////////////////////////
  //                        //

  private:

    std::list<Artwork*> artworks_;

    Seekbar* seekbar_;

    std::list<Tagview*> tagviews_;





  //          //
  // Artworks /////////////////////////////////////////////////////////////////
  //          //

  private:

    std::string cover_file_;

    std::string default_cover_file_;





  //             //
  // Header Bars //////////////////////////////////////////////////////////////
  //             //

  private:

    Gtk::HeaderBar* header_bar_;





  //          //
  // Notebook /////////////////////////////////////////////////////////////////
  //          //

  private:

    Gtk::Notebook* main_window_notebook_;





  //         //
  // Windows //////////////////////////////////////////////////////////////////
  //         //

  private:

    ChildWindow* main_window_;

    bool main_window_maximized_;

    int window_size_x_;

    int window_size_y_;





  //            //
  // Status Bar ///////////////////////////////////////////////////////////////
  //            //

  public:

    StatusBar* status_bar_;





  //         //
  // Top Bar //////////////////////////////////////////////////////////////////
  //         //

  private:

    PlaybackController* main_playback_controller_;





  //              //
  // Main Content /////////////////////////////////////////////////////////////
  //              //

  private:

    bool fullscreen_;

    Gtk::Box* duplicates_box_;

    Gtk::Box* left_main_content_paned_box_;

    Gtk::Box* main_content_page_box_;

    Gtk::Paned* main_content_paned_;

    Gtk::Box* top_box_;





  //                 //
  // Double Playlist //////////////////////////////////////////////////////////
  //                 //

  private:

    Gtk::Paned* double_playlist_paned_;





  //                       //
  // File Chooser Playlist ////////////////////////////////////////////////////
  //                       //

  private:

    Gtk::Paned* file_chooser_playlist_paned_;

};





//                  //
//                  //
//                  //
// Header Guard End ///////////////////////////////////////////////////////////
//                  //
//                  //
//                  //

#endif
