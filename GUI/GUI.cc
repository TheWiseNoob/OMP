




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

#include "GUI.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"

#include "../Configuration/Configuration.h"

#include "../Metadata/Metadata.h"

#include "../Metadata/TimeConversion.h"

#include "../Metadata/Track.h"

#include "../Playback/Playback.h"

#include "../Scrobbling/Scrobbling.h"

#include "Artwork.h"

#include "Elements/Abouts/Abouts.h"

#include "Elements/ChildWindows/ChildWindow.h"

#include "Elements/ChildWindows/ChildWindows.h"

#include "Elements/ConfigurationGUIs/ConfigurationGUIs.h"

#include "Elements/MainMenus/MainMenu.h"

#include "Elements/MainMenus/MainMenus.h"

#include "Elements/PlaybackControllers/PlaybackController.h"

#include "Elements/PlaybackControllers/PlaybackControllers.h"

#include "Elements/PlaylistComboBoxes/PlaylistComboBoxes.h"

#include "Elements/Playlists/PlaylistColumnRecord.h"

#include "Elements/Playlists/Playlists.h"

#include "Elements/Playlists/PlaylistsDatabase.h"

#include "Elements/Playlists/PlaylistTreeStore.h"

#include "Elements/FileChoosers/FileChooser.h"

#include "Elements/FileChoosers/FileChoosers.h"

#include "../KeyboardShortcuts/KeyboardShortcuts.h"

#include "Seekbar.h"

#include "StatusBar.h"

#include "Tagview.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <cstdlib>

#include <fstream>

#include <functional>

#include <gdkmm/rectangle.h>

#include <gdkmm/rgba.h>

#include <glibmm/main.h>

#include <gtkmm/applicationwindow.h>

#include <gtkmm/box.h>

#include <gtkmm/button.h>

#include <gtkmm/combobox.h>

#include <gtkmm/eventbox.h>

#include <gtkmm/frame.h>

#include <gtkmm/headerbar.h>

#include <gtkmm/icontheme.h>

#include <gtkmm/label.h>

#include <gtkmm/main.h>

#include <gtkmm/notebook.h>

#include <gtkmm/paned.h>

#include <gtkmm/progressbar.h>

#include <gtkmm/scale.h>

#include <gtkmm/separator.h>

#include <gtkmm/spinbutton.h>

#include <gtkmm/stackswitcher.h>

#include <gtkmm/treerowreference.h>

#include <gtkmm/volumebutton.h>

#include <iostream>

#include <pwd.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>

#include <vector>





//            //
//            //
//            //
// Namespaces /////////////////////////////////////////////////////////////////
//            //
//            //
//            //

using namespace std;





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

GUI::GUI(Base& base_ref)

// Inherited Class

: Parts(base_ref)



// GUI Elements

, abouts_(new Abouts(base_ref))

, windows_(new ChildWindows(base_ref))

, config_guis_(new ConfigurationGUIs(base_ref))

, main_menus_(new MainMenus(base_ref))

, playlists_(new Playlists(base_ref))

, playback_controllers_(new PlaybackControllers(base_ref))

, playlist_comboboxes_(new PlaylistComboBoxes(base_ref, *playlists_))

, file_choosers_(new FileChoosers(base_ref))



// Unorganized GUI Parts

, duplicates_box_(Gtk::manage(new Gtk::Box))

, header_bar_(Gtk::manage(new Gtk::HeaderBar))

, left_main_content_paned_box_(Gtk::manage(new Gtk::Box))

, main_window_notebook_(Gtk::manage(new Gtk::Notebook))

, main_content_page_box_(Gtk::manage(new Gtk::Box))

, main_content_paned_(Gtk::manage(new Gtk::Paned))

, seekbar_(new Seekbar(base_ref, 0, 10))

, top_box_(Gtk::manage(new Gtk::Box))



// Status Bar 

, status_bar_(new StatusBar(base_ref))



// Double Playlist

, double_playlist_paned_(Gtk::manage(new Gtk::Paned))



// File Chooser Playlist

, file_chooser_playlist_paned_(Gtk::manage(new Gtk::Paned))

{

  //                   //
  // Quitting Function ////////////////////////////////////////////////////////
  //                   //

  // Function run when the program closes.
  std::function<void(void)> temp_func_ptr = std::bind(&GUI::Quit, this, false);





  //                      //
  // Main Window Creation /////////////////////////////////////////////////////
  //                      //

  // Sets main_window equal to a new shared_ptr<ChildWindow> object that will
  // be the program's main window.
  main_window_
    = new ChildWindow("OMP", base_ref, *windows_, temp_func_ptr, true);



  // Overrides the default event signal handler.
  main_window() -> window() . add_events(Gdk::KEY_PRESS_MASK);



  // 
  int window_size_x_ = config() . get("gui.window_size_x");

  // 
  int window_size_y_ = config() . get("gui.window_size_y");

  // Sets default window size.
  main_window() -> window() . set_default_size(window_size_x_, window_size_y_);



  // 
  bool maximized = config() . get("gui.window_maximized");

  // 
  if(maximized)
  { 

    // 
    main_window() -> window() . maximize();

  }

  // 
  main_window_maximized_ = maximized;



  // Sets the main window's orientation to vertical.
  main_window() -> box() . set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Overrides the function for keypresses to allow custom shortcuts.
  main_window() -> window() . signal_key_press_event()
    . connect(sigc::mem_fun(base_ref . keyboard_shortcuts(),
                            &KeyboardShortcuts::On_Key_Press_Event));

  // Event function for when the window irregularly closes.
  main_window() -> window() . signal_delete_event()
    . connect(sigc::mem_fun(*this, &GUI::On_GUI_Window_Signal_Delete_Event));

  // Adds top_box_ to the start of the main window's box.
  main_window() -> box().pack_start(*top_box_, Gtk::PACK_SHRINK);

  // Sets top_box's orientation to vertical.
  top_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);



#if defined(FLATPAK)
  string dir_head = "/app";

#else
  string dir_head = "";

#endif



  // 
  vector<Glib::RefPtr<Gdk::Pixbuf>> icon_pixbufs;

  // 
  string icon_16p_filename
    = dir_head + "/usr/share/icons/hicolor/16x16/apps/com.openmusicplayer.OMP.png";

  cout << "\n\n16 icon: " << icon_16p_filename << "\n\n";

  // 
  Glib::RefPtr<Gdk::Pixbuf> icon_16p_pixbuf
    = Gdk::Pixbuf::create_from_file(icon_16p_filename);

  // 
  icon_pixbufs . push_back(icon_16p_pixbuf);

  // 
  string icon_32p_filename
    = dir_head + "/usr/share/icons/hicolor/32x32/apps/com.openmusicplayer.OMP.png";

  // 
  Glib::RefPtr<Gdk::Pixbuf> icon_32p_pixbuf
    = Gdk::Pixbuf::create_from_file(icon_32p_filename);

  // 
  icon_pixbufs . push_back(icon_32p_pixbuf);

  // 
  string icon_48p_filename
    = dir_head + "/usr/share/icons/hicolor/48x48/apps/com.openmusicplayer.OMP.png";

  // 
  Glib::RefPtr<Gdk::Pixbuf> icon_48p_pixbuf
    = Gdk::Pixbuf::create_from_file(icon_48p_filename);

  // 
  icon_pixbufs . push_back(icon_48p_pixbuf);

  // 
  string icon_64p_filename
    = dir_head + "/usr/share/icons/hicolor/64x64/apps/com.openmusicplayer.OMP.png";

  // 
  Glib::RefPtr<Gdk::Pixbuf> icon_64p_pixbuf
    = Gdk::Pixbuf::create_from_file(icon_64p_filename);

  // 
  icon_pixbufs . push_back(icon_64p_pixbuf);

  // 
  string icon_128p_filename
    = dir_head + "/usr/share/icons/hicolor/128x128/apps/com.openmusicplayer.OMP.png";

  // 
  Glib::RefPtr<Gdk::Pixbuf> icon_128p_pixbuf
    = Gdk::Pixbuf::create_from_file(icon_128p_filename);

  // 
  icon_pixbufs . push_back(icon_128p_pixbuf);

  // 
  main_window() -> window() . set_default_icon_list(icon_pixbufs);



  // Sets the name of the header bar. 
  main_window() -> window() . set_title("OMP");



  // 
  main_window() -> window() . add_events(Gdk::STRUCTURE_MASK);

  // 
  main_window() -> window() . signal_check_resize()
    . connect(sigc::mem_fun(*this,
                            &GUI::On_Main_Window_Check_Resize_Signal));





  //                 //
  // Header Creation //////////////////////////////////////////////////////////
  //                 //

  if(!(config() . get("gui.hide_header_bar")))
  {

    // Sets the name of the header bar. 
    header_bar_ -> set_title("OMP");

    // Sets the header bar's subtitle to nothing.
    header_bar_ -> set_subtitle("");

    // Adds the header bar to the main window.
    main_window() -> window() . set_titlebar(*header_bar_);

    // Displays the close button on the header bar.
    header_bar_ -> set_show_close_button();

    // Enables the menu, minimize, maximize and close buttons on the header bar.
    header_bar_ -> set_decoration_layout("menu:minimize,maximize,close");

  }

  // 
  else
  {

  }





  //                  //
  // Top Box Creation /////////////////////////////////////////////////////////
  //                  //

  // Seekbar Creation /////////////////////////////////////////////////////////

  // Adds the seekbar to the top of the main window's box.
  top_box_ -> pack_end(seekbar() . box());

  // Disables the displayed value of the seekbar.
  seekbar() . scale() . set_draw_value(false);

  // Sets the range of values used by the seekbar.
  seekbar() . scale() . set_range(0, 100);

  // Sets the seekbar to 0.
  seekbar() . scale() . set_value(0);

  // Makes the seekbar insensitive to grabbing. 
  seekbar() . scale() . set_sensitive(false);



  // Header MainMenu Creation /////////////////////////////////////////////////

  // Creates two MainMenu pointers.
  MainMenu* header_bar_menubar
    = new MainMenu(base_ref, *main_menus_);

  // Adds the menubar to the header.
  header_bar_ -> pack_start(header_bar_menubar -> box());



  // 
  PlaylistComboBox* header_bar_playlist_combobox_ptr
    = new PlaylistComboBox(base_ref, *playlist_comboboxes_);

  

  // Adds a playlist combobox from the beginning of the playlist_comboboxes_.
  header_bar_ -> pack_end(header_bar_playlist_combobox_ptr -> box());



  // Playback Buttons Creation ////////////////////////////////////////////////

  // 
  main_playback_controller_
    = new PlaybackController(base(), *playback_controllers_);

  // Adds the playback_buttons_stack_switcher to header_bar_.
  top_box_ -> pack_start(main_playback_controller_ -> box(), Gtk::PACK_SHRINK);





  //                     //
  // Notebook Assembling //////////////////////////////////////////////////////
  //                     //

  // Adds the notebook to main_window's box.
  main_window() -> box()
    . pack_start(*main_window_notebook_, Gtk::PACK_EXPAND_WIDGET);



  // Sets the tab position of the notebook at the top.
  main_window_notebook_ -> set_tab_pos(Gtk::POS_TOP);



  // Sets the notebook tabs as visual.
  main_window_notebook_ -> set_show_tabs(true);



  // Sets the notebook tabs to be scrollable.
  main_window_notebook_ -> set_scrollable(true);



  // Sets the notebook tabs to show the border.
  main_window_notebook_ -> set_show_border(true);



  //
  main_window_notebook_ -> set_show_tabs(!config() . get("gui.tabs.hide"));





  //                         //
  // Main Content Assembling //////////////////////////////////////////////////
  //                         //

  // 
  Gtk::Label main_content_page_label("Main Content");

  // 
  main_content_page_label . set_hexpand(true);



  // Sets the orientation of the box containing the Main Content tab's 
  // contents to vertical.
  main_content_page_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);



  // Adds main_content_paned_ to main_content_page_box_.
  main_content_page_box_ 
    -> pack_start(*main_content_paned_, Gtk::PACK_EXPAND_WIDGET);

  // 
  int main_content_paned_position
    = config() . get("gui.main_content_paned_position");


  // Sets the current position of main_content_paned_.
  main_content_paned_ -> set_position(main_content_paned_position);



  // Adds the Main Content page to the notebook.
  main_window_notebook_ -> append_page(*main_content_page_box_,
                                       main_content_page_label);

  // Sets the orientation of main_content_paned_.
  main_content_paned_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  // Adds the left box to the left paned side.
  main_content_paned_ -> pack1(*left_main_content_paned_box_, Gtk::EXPAND);



  //
  main_content_paned_ -> signal_button_release_event()
    . connect(sigc::mem_fun
        (*this, &GUI::On_Main_Content_Paned_Button_Release_Event));



  // 
  Playlist* main_content_playlist
    = new Playlist(base_ref, (*playlists_), "main_content");

  // Adds the right box to the right paned side.
  main_content_paned_ 
    -> pack2(main_content_playlist -> box(), Gtk::EXPAND);

  // Sets the orientation of the left paned box.
  left_main_content_paned_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);
 
  // Sets the left margin of the playlist in Main Content.
  main_content_playlist -> box() . set_margin_left(3);

  // Sets the right margin of the playlist in Main Content.
  main_content_playlist -> box() . set_margin_right(3);

  // Sets the upper margin of the playlist in Main Content.
  main_content_playlist -> box() . set_margin_top(3);

  // 
  playlists_ -> set_selected_playlist(*main_content_playlist);



  // Tagview Creation /////////////////////////////////////////////////////////

  // Creates a temporary pointer object for a new Tagview.
  Tagview *temp_tagview;

  // Creates a new Tagview and assigns it to the pointer.
  temp_tagview = new Tagview(base_ref);

  // Adds the Tagview to the Tagview list.
  tagviews_ . push_front(temp_tagview);

  // Adds the new Tagview to the left pane's box.
  left_main_content_paned_box_
    -> pack_start(temp_tagview -> box(), Gtk::PACK_SHRINK);



  // Artworks Creation ////////////////////////////////////////////////////////

  // 
  struct passwd* pw = getpwuid(getuid());

  // 
  const char* homedir = pw -> pw_dir;

  // 
  string directory_str = homedir;

  // 
  directory_str += "/.omp";



  // 
  cover_file_ = dir_head + "/usr/share/OMP/No_Cover.svg";

  // 
  default_cover_file_ = cover_file_;



  // Creates two temporary Artwork pointers.
  Artwork *artwork_1,
          *artwork_2;

  //Adds the first artwork to artworks_.
  artworks_ . push_back(artwork_1);

  //Adds the second artwork to artworks_.
  artworks_ . push_back(artwork_2);

  // Gets an iterator to the beginning of artworks_.
  list<Artwork*>::iterator artwork_it = artworks_ . begin();

  // Creates an Artwork object for each pointer entry in the list.
  while(artwork_it != artworks_ . end())
  {

    // Creates a new Artwork class.
    (*artwork_it) = Gtk::manage(new Artwork(base_ref));

    // Increments the artworks_ iterator.
    artwork_it++;

  }

  // Makes a reference to the first Artwork instance.
  Gtk::Box* temp_box_ptr = &((*(artworks_ . begin())) -> box());



  // 
  Gtk::Label artwork_page_label("Artwork");

  // 
  artwork_page_label . set_hexpand(true);



  // Adds the Artwork tab with the second Artwork instances.
  main_window_notebook_ -> append_page(*temp_box_ptr, artwork_page_label);

  // Adds the first Artwork instance to the left pain of the Main Content tab.
  left_main_content_paned_box_ 
    -> pack_start(((*(artworks_.rbegin())) -> box()),
                  Gtk::PACK_EXPAND_WIDGET);

  // Sets the left margin of the Main Content's Artwork box.
  ((*(artworks_.rbegin())) -> box()).set_margin_left(5);

  // Sets the right margin of the Main Content's Artwork box.
  ((*(artworks_.rbegin())) -> box()).set_margin_right(5);

  // Sets the top margin of the Main Content's Artwork box.
  ((*(artworks_.rbegin())) -> box()).set_margin_top(5);

  // Sets the bottom margin of the Main Content's Artwork box.
  ((*(artworks_.rbegin())) -> box()).set_margin_bottom(5);



  // Main Content MainMenu Creation ///////////////////////////////////////////

  // Adds a MainMenu to the left pane in Main Content.
  left_main_content_paned_box_ -> pack_end(*duplicates_box_, Gtk::PACK_SHRINK);



  // Creates two MainMenu pointers.
  MainMenu* main_content_menubar
    = new MainMenu(base_ref, *main_menus_);



  // Adds a MainMenu to the left pane in Main Content.
  duplicates_box_
    -> pack_start(main_content_menubar -> box(), Gtk::PACK_SHRINK);



  // Main Content Playlist ComboBox Creation //////////////////////////////////

  // Adds a MainMenu to the left pane in Main Content.
  duplicates_box_
    -> pack_start(*Gtk::manage(new Gtk::Separator), false, false, 10);



  // 
  PlaylistComboBox* duplicates_playlist_combobox_ptr
    = new PlaylistComboBox(base_ref, *playlist_comboboxes_);

  // Adds a PlaylistCombobox to the the left pane in Main Content.
  duplicates_box_ -> pack_end(duplicates_playlist_combobox_ptr -> box(), 
                             true, true, 0);

  // Adds a PlaylistCombobox to the the left pane in Main Content.
  duplicates_playlist_combobox_ptr -> box() . set_hexpand(true);

  // Adds a PlaylistCombobox to the the left pane in Main Content.
  duplicates_playlist_combobox_ptr -> playlist_combobox_entry()
    . set_hexpand(true);

  // Adds a PlaylistCombobox to the the left pane in Main Content.
  duplicates_playlist_combobox_ptr -> playlist_combobox() . set_hexpand(true);

  // Adds a PlaylistCombobox to the the left pane in Main Content.
  duplicates_playlist_combobox_ptr -> box() . set_hexpand_set(true);

  // Adds a PlaylistCombobox to the the left pane in Main Content.
  duplicates_playlist_combobox_ptr -> playlist_combobox_entry()
    . set_hexpand_set(true);

  // Adds a PlaylistCombobox to the the left pane in Main Content.
  duplicates_playlist_combobox_ptr -> playlist_combobox()
    . set_hexpand_set(true);





  //               //
  // Playlist Page ////////////////////////////////////////////////////////////
  //               //

  // Creates labels for the four pages in main_content_notebook_.
  Gtk::Label playlist_page_label("Full Playlist");

  // 
  playlist_page_label . set_hexpand(true);


  // 
  Playlist* full_playlist = new Playlist(base_ref, (*playlists_), "full");

  // Adds the Playlist tab to the notebook.
  main_window_notebook_
    -> append_page(full_playlist -> box(), playlist_page_label);





  //                 //
  // Double Playlist //////////////////////////////////////////////////////////
  //                 //

  // Creates labels for the four pages in main_content_notebook_.
  Gtk::Label double_playlist_page_label("Double Playlist");

  double_playlist_page_label . set_hexpand(true);



  // 
  Playlist* left_double_playlist
    = new Playlist(base_ref, (*playlists_), "double_left");

  // 
  Playlist* right_double_playlist
    = new Playlist(base_ref, (*playlists_), "double_right");



  // 
  int double_playlist_paned_position
    = config() . get("gui.double_playlist_paned_position");


  // Sets the current position of main_content_paned_.
  double_playlist_paned_ -> set_position(double_playlist_paned_position);



  //
  double_playlist_paned_ -> signal_button_release_event()
    . connect(sigc::mem_fun
        (*this, &GUI::On_Double_Playlist_Paned_Button_Release_Event));



  // 
  double_playlist_paned_
    -> pack1(left_double_playlist -> box(), Gtk::EXPAND);

  // 
  double_playlist_paned_
    -> pack2(right_double_playlist -> box(), Gtk::EXPAND);



  // Adds the Playlist tab to the notebook.
  main_window_notebook_
    -> append_page(*double_playlist_paned_, double_playlist_page_label);





  //                         //
  // File Chooser & Playlist //////////////////////////////////////////////////
  //                         //

  // Creates labels for the four pages in main_content_notebook_.
  Gtk::Label file_chooser_playlist_page_label("File Chooser Playlist");

  file_chooser_playlist_page_label . set_hexpand(true);



  // 
  Playlist* file_chooser_playlist
    = new Playlist(base_ref, (*playlists_), "file_chooser");

  // 
  FileChooser* playlist_file_chooser
    = new FileChooser(base_ref, (*file_choosers_));



  // 
  int file_chooser_playlist_paned_position
    = config() . get("gui.file_chooser_playlist_paned_position");


  // Sets the current position of main_content_paned_.
  file_chooser_playlist_paned_
    -> set_position(file_chooser_playlist_paned_position);



  //
  file_chooser_playlist_paned_ -> signal_button_release_event()
    . connect(sigc::mem_fun
        (*this, &GUI::On_File_Chooser_Playlist_Paned_Button_Release_Event));



  // 
  file_chooser_playlist_paned_
    -> pack2(file_chooser_playlist -> box(), Gtk::EXPAND);

  // 
  file_chooser_playlist_paned_
    -> pack1(playlist_file_chooser -> box(), Gtk::EXPAND);



  // Adds the Playlist tab to the notebook.
  main_window_notebook_
    -> append_page(*file_chooser_playlist_paned_,
                   file_chooser_playlist_page_label);





  //                        //
  // Configuration Creation ///////////////////////////////////////////////////
  //                        //

  // Creates the label for the configuration notebook page.
  Gtk::Label configuration_page_label("Configuration");

  // 
  configuration_page_label . set_hexpand(true);



  // 
  ConfigurationGUI* new_config_gui_ptr
    = new ConfigurationGUI(base_ref, *config_guis_);

  // Appends the ConfigGUI page to the notebook.
  main_window_notebook_ -> append_page(new_config_gui_ptr -> box(),
                                       configuration_page_label);





  //                     //
  //                     //
  // Status Bar Creation //////////////////////////////////////////////////////
  //                     //
  //                     //

  // Adds the status_bar_frame to the bottom of the main_window's box.
  main_window() -> box() . pack_end(status_bar_ -> status_bar_frame(),
                                    Gtk::PACK_SHRINK);





  //                       //
  //                       //
  // Finalize GUI Creation ////////////////////////////////////////////////////
  //                       //
  //                       //

  // Displays the main window and all of its child objects.
  main_window() -> window() . show_all_children();



  // 
  for(auto playlists_it : playlists()())
  {

    // 
    playlists_it -> copy_progress_bar() . hide();

  }



  // Sets the visibility of the Status Bar based on the
  if(config() . get("gui.hide_duplicates"))
  {

    // 
    duplicates_box_ -> hide();

  }

  // 
  else
  {

    // 
    duplicates_box_ -> show();

  }





  // 
  string active_tab_str = config() . get("gui.tabs.active");

  // 
  for(auto tabs_it : main_window_notebook_ -> get_children())
  {

    // 
    if((main_window_notebook_ -> get_tab_label_text(*tabs_it))
         == active_tab_str)
    {

      // 
      main_window_notebook_
        -> set_current_page(main_window_notebook_ -> page_num(*tabs_it));

    }

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

GUI::~GUI()
{

  // 
  delete main_menus_;

  // Deletes the instance of the Playlists class.
  delete playlists_;

  // Deletes the instance of the PlaylistComboboxes class.
  delete playlist_comboboxes_;

  // Deletes the instance of the ConfigGUIs class.
  delete config_guis_;

  // 
  delete status_bar_;

  // Deletes the instance of the seekbar.
  delete seekbar_;



  // 
  int current_page_num = main_window_notebook_ -> get_current_page();

  // 
  auto active_page
    = (main_window_notebook_ -> get_nth_page(current_page_num));

  // 
  string active_page_name
    = main_window_notebook_ -> get_tab_label_text(*active_page);

  // 
  config() . set("gui.tabs.active", active_page_name);

  // 
  config() . write_file();



  // 
  delete windows_;

}






//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//        //
// Normal /////////////////////////////////////////////////////////////////////
//        //

bool GUI::File_Exists(string& filename_ref)
{

  // Returns true if the file exists when attempting to open an ifstream
  // with its filename.
  return bool(std::ifstream(filename_ref . c_str()));

}

bool GUI::On_Double_Playlist_Paned_Button_Release_Event
  (GdkEventButton* release_event)
{

  // 
  int double_playlist_paned_position
    = double_playlist_paned_ -> get_position();

  // 
  config() . set("gui.double_playlist_paned_position",
                 double_playlist_paned_position);



  // 
  config() . write_file();



  // 
  return false;

} 

bool GUI::On_File_Chooser_Playlist_Paned_Button_Release_Event
  (GdkEventButton* release_event)
{

  // 
  int file_chooser_playlist_paned_position
    = file_chooser_playlist_paned_ -> get_position();

  // 
  config() . set("gui.file_chooser_playlist_paned_position",
                 file_chooser_playlist_paned_position);



  // 
  config() . write_file();



  // 
  return false;

}

bool GUI::On_GUI_Window_Signal_Delete_Event(GdkEventAny* event)
{

  // 
  if(playlists() . rebuild_databases() || playlists() . rebuilding_databases())
  {

    // 
    main_window() -> Show();



    // 
    playlists() . Open_Changes_Cancel_Dialog();



    // 
    return true;

  }

  // 
  else
  {

    // 
    base() . quitting() = true;

  }



  // Ends the function.
  return true;

}

bool GUI::On_Main_Content_Paned_Button_Release_Event
  (GdkEventButton* release_event)
{

  // 
  int main_content_paned_position = main_content_paned_ -> get_position();

  // 
  config() . set("gui.main_content_paned_position",
                 main_content_paned_position);



  // 
  config() . write_file();



  // 
  return false;

}

void GUI::On_Main_Window_Check_Resize_Signal()
{

  // 
  int x = 0;

  // 
  int y = 0;



  // 
  main_window() -> window() . get_size(x, y);



  // 
  bool maximized = main_window() -> window() . is_maximized();



  // 
  if((x != window_size_x_) || (y != window_size_y_)
       || (maximized != main_window_maximized_))
  { 

    // 
    config() . set("gui.window_size_x", x);

    // 
    config() . set("gui.window_size_y", y);



    // 
    config() . set("gui.window_maximized", maximized);

    // 
    main_window_maximized_ = maximized;



    // 
    int main_content_paned_position = main_content_paned_ -> get_position();

    // 
    config() . set("gui.main_content_paned_position",
                   main_content_paned_position);



    // 
    int double_playlist_paned_position
      = double_playlist_paned_ -> get_position();

    // 
    config() . set("gui.double_playlist_paned_position",
                   double_playlist_paned_position);



    // 
    int file_chooser_playlist_paned_position
      = file_chooser_playlist_paned_ -> get_position();

    // 
    config() . set("gui.file_chooser_playlist_paned_position",
                   file_chooser_playlist_paned_position);



    // 
    config() . write_file();

  }



  // 
  window_size_x_ = x;

  // 
  window_size_y_ = y;

}

void GUI::Quit(bool release_twice)
{ 

  // 
  if(playlists() . rebuild_databases() || playlists() . rebuilding_databases())
  {

    // 
    playlists() . Open_Changes_Cancel_Dialog(release_twice);



    // 
    return;

  }

  // 
  else
  {

    // 
    base() . quitting() = true;

  }



  // 
  playback() . Stop();



  // 
  playback() . Quit();



  // 
  if(!(config() . get("playback.queue_saved")))
  {

    // 
    playlists() . database() . Clear_Playlist("Queue");

  }



  //
  playlists() . database() . Cleanup_Database();



  // 
  if(release_twice)
  {

    // Releases the application window.
    main_window() -> window() . get_application() -> release();

  }

  // Releases the application window.
  main_window() -> window() . get_application() -> release();

}





//          //
// Artworks ///////////////////////////////////////////////////////////////////
//          //

void GUI::Load_Cover_Art(string& filename_ref)
{

  // Makes a copy of the song's filename.
  string filename_copy = filename_ref;

  // Used to create a copy of the filename's path name.
  string file_folder_path = "";

  // Stores the current position in the filename string in order to iterate
  // through it to find the end of the file's path.
  int filename_pos = filename_copy.size() - 1;

  // Finds the last instance of a forward slash in the filename.
  for(int i = filename_pos - 1; filename_copy[i] != '/'; i--)
  {

    // The final value is the position of the start of the workign directory.
    filename_pos = i;

  }

  // Stores all of the file path.
  for(int i = 0; i < filename_pos; i++)
  {

    // Uses filename_copy to get the path the music file is in.
    file_folder_path = file_folder_path + filename_copy[i];

  }


  // 
  list<string> cover_art_filename_str_list;

  // 
  config() . get_list("gui.artwork.front_names", cover_art_filename_str_list);



  // 
  for(auto filename_it : cover_art_filename_str_list)
  { 

    // Set file_folder_path to final cover image filename.
    std::string new_cover_filename = file_folder_path + filename_it;

    // Does nothing if true because the the cover file is the same as the cover
    // file already being used.
    if(cover_file_ == new_cover_filename)
    { 

      // 
      if(!File_Exists(cover_file_))
      {

        continue;

      }

      // 
      else
      {

        // 
        return;

      }

    }

    // Sets the new cover image.
    else if(File_Exists(new_cover_filename))
    {

      // Gets an iterator to the beginning of the artworks_ list.
      list<Artwork*>::iterator artwork_it = artworks_.begin();

      // Sets all of the Artworks to the default cover image.
      while(artwork_it != (artworks_.end()))
      {

        // Sets the iterator's current Artwork image filename to the default.
        (*artwork_it) -> Set_Image_Filename(new_cover_filename);

        // Increments the artworks_ iterator.
        artwork_it++;

      }

      // Sets cover_file_ to new cover_filename_.
      cover_file_ = new_cover_filename;



      // 
      return;

    }

  }



#if defined(FLATPAK)
  string dir_head = "/app";

#else
  string dir_head = "";

#endif



  // Gets an iterator to the beginning of the artworks_ list.
  list<Artwork*>::iterator artwork_it = artworks_.begin();

  // Sets all of the Artworks to the default cover image.
  while(artwork_it != (artworks_.end()))
  {

    // Sets the iterator's current Artwork image filename to the default.
    (*artwork_it) -> Set_Image_Filename(dir_head + "/usr/share/OMP/No_Cover.svg");
 
    // Increments the iterator.
    artwork_it++;

  }


  

  // Sets cover_file_ to the default cover image.
  cover_file_ = default_cover_file_;

}





//         //
// Tagview ////////////////////////////////////////////////////////////////////
//         //

void GUI::Update_Tagview(const char* tag_frame_label_name, Track& new_track)
{

  // Nothing happens if no TagViews exist.
  if(!(tagviews_.empty()))
  {

    // Updates the Tagview with the frame label and track.
    (*(tagviews_.begin())) -> Update_Tags(tag_frame_label_name, new_track);

  }

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

//                //
// Normal Getters /////////////////////////////////////////////////////////////
//                //

Gtk::Box& GUI::duplicates_box()
{

  return *duplicates_box_;

}

long long GUI::duration()
{

  // Returns Playback's duration variable.
  return playback() . duration();

}

long long GUI::position()
{

  // Returns Playback's position variable.
  return playback() . position();

}





//                      //
// GUIElements Getters ///////////////////////////////////////////////////////
//                      //

Abouts& GUI::abouts()
{

  return *abouts_;

}

ChildWindows& GUI::windows()
{  

  return *windows_; 

}

ConfigurationGUIs& GUI::config_guis()
{  

  return *config_guis_; 

}

FileChoosers& GUI::file_choosers()
{

  return *file_choosers_;

}

MainMenus& GUI::main_menus()
{ 

  return *main_menus_;

}

Playlists& GUI::playlists()
{

  return *playlists_;

}

PlaylistComboBoxes& GUI::playlist_comboboxes()
{

  return *playlist_comboboxes_;

}





//                               //
// Future GUIElementList Getters //////////////////////////////////////////////
//                               //

Seekbar& GUI::seekbar()
{ 

  return *seekbar_; 

}

StatusBar& GUI::status_bar()
{

  return *status_bar_;

}

std::list<Tagview*>& GUI::tagviews()
{ 

  return tagviews_; 

}





//          //
// Notebook ///////////////////////////////////////////////////////////////////
//          //

Gtk::Notebook& GUI::main_window_notebook()
{ 

  return *main_window_notebook_;

}





//         //
// Windows ////////////////////////////////////////////////////////////////////
//         //

ChildWindow* GUI::main_window()
{ 

  return main_window_;

}
