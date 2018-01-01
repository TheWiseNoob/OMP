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
//  The developer(s) of the Moyσikh audio player hereby grant(s) permission
//  for non-GPL compatible GStreamer plugins to be used and distributed
//  together with GStreamer and Moyσikh. This permission is above and beyond
//  the permissions granted by the GPL license by which Moyσikh is covered.
//  If you modify this code, you may extend this exception to your version
//  of the code, but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version.
//
//  Libraries used by Moyσikh:
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
#include "../Playback/Playback.h"
#include "../TimeConversion.h"
#include "../Metadata/Track.h"
#include "../Scrobbling/Scrobbling.h"
#include "Artwork.h"
#include "ChildWindow.h"
#include "Elements/ConfigurationGUIs/ConfigurationGUIs.h"
#include "Elements/Playlists/PlaylistTreeStore.h"
#include "Elements/Playlists/PlaylistColumnRecord.h"
#include "Elements/Playlists/Playlists.h"
#include "Elements/Playlists/PlaylistsDatabase.h"
#include "Elements/PlaylistComboBoxes/PlaylistComboBoxes.h"
#include "Elements/FileChoosers/FileChoosers.h"
#include "Elements/FileChoosers/FileChooser.h"
#include "MenuBar.h"
#include "Seekbar.h"
#include "Tagview.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <pwd.h>

#include <fstream>
#include <functional>
#include <gdkmm/rgba.h>
#include <gtkmm/main.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/stackswitcher.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/label.h>
#include <gtkmm/volumebutton.h>
#include <gtkmm/separator.h>
#include <iostream>
#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>






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



// Member Variables

, fullscreen_(false)



// GUI Elements

, config_guis_(new ConfigurationGUIs(base_ref))
, playlists_(new Playlists(base_ref))
, playlist_comboboxes_(new PlaylistComboBoxes(base_ref, *playlists_))
, file_choosers_(new FileChoosers(base_ref))



// Unorganized GUI Parts

, header_bar_(Gtk::manage(new Gtk::HeaderBar))
, left_main_content_paned_box_(Gtk::manage(new Gtk::Box))
, main_window_notebook_(Gtk::manage(new Gtk::Notebook))
, main_content_page_box_(Gtk::manage(new Gtk::Box))
, main_content_paned_(Gtk::manage(new Gtk::Paned))
, seekbar_(new Seekbar(base_ref, 0, 10))
, top_box_(Gtk::manage(new Gtk::Box))



// MenuBars

, disable_menubar_functions_flag_(false)


// Status Bar

, status_bar_frame_(Gtk::manage(new Gtk::Frame))
, status_bar_event_box_(Gtk::manage(new Gtk::EventBox))
, status_bar_box_(Gtk::manage(new Gtk::Box))
, time_label_box_(Gtk::manage(new Gtk::Box))
, time_label_(Gtk::manage(new Gtk::Label("00:00.00 / 00:00.00", 
                                         0.5, 1.0, false)))
, selected_time_label_(Gtk::manage(new Gtk::Label()))
, playback_status_label_(Gtk::manage(new Gtk::Label("⏹ Stopped")))



// Playback Buttons

, playback_buttons_stack_switcher_(Gtk::manage(new Gtk::StackSwitcher))
, next_button_(Gtk::manage(new Gtk::Button))
, pause_button_(Gtk::manage(new Gtk::Button))
, play_button_(Gtk::manage(new Gtk::Button))
, previous_button_(Gtk::manage(new Gtk::Button))
, stop_button_(Gtk::manage(new Gtk::Button))



// Volume

, main_volume_button_(Gtk::manage(new Gtk::VolumeButton))
, main_volume_button_box_(Gtk::manage(new Gtk::Box))

{

  //                   //
  //                   //
  // Quitting Function ////////////////////////////////////////////////////////
  //                   //
  //                   //

  // Function run when the program closes.
  std::function<void(void)> temp_func_ptr = std::bind(&GUI::Quit, this);



  //                      //
  //                      //
  // Main Window Creation /////////////////////////////////////////////////////
  //                      //
  //                      //

  // Sets main_window equal to a new shared_ptr<ChildWindow> object that will
  // be the program's main window.
  main_window_
    = make_shared<ChildWindow>("OMP", base_ref, temp_func_ptr, true);

  // Adds the main window to the windows data structure. 
  windows_.push_front(main_window_);

  // Overrides the default event signal handler.
  main_window() -> window().add_events(Gdk::KEY_PRESS_MASK);

  // Overrides the function for keypresses to allow custom shortcuts.
  main_window() -> window().signal_key_press_event()
    .connect(sigc::mem_fun(*this, &GUI::On_Key_Press_Event));

  // Event function for when the window irregularly closes.
  main_window() -> window().signal_delete_event()
    .connect(sigc::mem_fun(*this, &GUI::On_GUI_Window_Signal_Delete_Event));

  // Sets the list location of the window for easy access.
  main_window() -> set_location(windows_.begin());

  // Sets default window size.
  main_window() -> window().set_default_size(1200, 700);

  // Adds top_box_ to the start of the main window's box.
  main_window() -> box().pack_start(*top_box_, Gtk::PACK_SHRINK);

  // Sets top_box's orientation to vertical.
  top_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  // 
  main_window() -> window() . set_icon_from_file("/usr/share/pixmaps/omp.png");
  // Sets the name of the header bar. 
  main_window() -> window() . set_title("OMP");





  //                 //
  //                 //
  // Header Creation //////////////////////////////////////////////////////////
  //                 //
  //                 //

  if(!(config() . get("gui.hide_header_bar")))
  {

    // Sets the name of the header bar. 
    header_bar_ -> set_title("OMP");

    // Sets the header bar's subtitle to nothing.
    header_bar_ -> set_subtitle("");

    // Adds the header bar to the main window.
    main_window() -> window().set_titlebar(*header_bar_);

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
  //                  //
  // Seekbar Creation /////////////////////////////////////////////////////////
  //                  //
  //                  //

  // Adds the seekbar to the top of the main window's box.
  top_box_ -> pack_end(seekbar());

  // Disables the displayed value of the seekbar.
  seekbar().scale().set_draw_value(false);

  // Sets the range of values used by the seekbar.
  seekbar().scale().set_range(0, 100);

  // Sets the seekbar to 0.
  seekbar().scale().set_value(0);

  // Makes the seekbar insensitive to grabbing. 
  seekbar().scale().set_sensitive(false);



  //                    //
  //                    //
  // Various Assembling ///////////////////////////////////////////////////////
  //                    //
  //                    //

  // Adds the notebook to main_window's box.
  main_window() -> box()
    .pack_start(*main_window_notebook_, Gtk::PACK_EXPAND_WIDGET);

  // Sets the main window's orientation to vertical.
  main_window() -> box().set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Sets the tab position of the notebook at the top.
  main_window_notebook_ -> set_tab_pos(Gtk::POS_TOP);

  // Sets the notebook tabs as visual.
  main_window_notebook_ -> set_show_tabs(true);

  // Sets the notebook tabs to be scrollable.
  main_window_notebook_ -> set_scrollable(true);

  // Sets the notebook tabs to show the border.
  main_window_notebook_ -> set_show_border(true);

  // Sets the orientation of the box containing the Main Content tab's 
  // contents to vertical.
  main_content_page_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Adds main_content_paned_ to main_content_page_box_.
  main_content_page_box_ 
    -> pack_start(*main_content_paned_, Gtk::PACK_EXPAND_WIDGET);

  // Sets the current position of main_content_paned_.
  main_content_paned_ -> set_position(420);

  // Creates labels for the four pages in main_content_notebook_.
  Gtk::Label artwork_page_label("Artwork"),
             main_content_page_label("Main Content"),
             playlist_page_label("Full Playlist");

  // Adds the Main Content page to the notebook.
  main_window_notebook_ -> append_page(*main_content_page_box_,
                                       main_content_page_label);

  // Sets the Main Content tab as reorderable.
//  main_window_notebook_ -> set_tab_reorderable(*main_content_page_box_, true);

  // Makes a shortcut to the box of the playlist for the Playlist tab.  
  Gtk::Box* temp_box_ptr = &((*(playlists()().rbegin())) -> box());

  // Adds the Playlist tab to the notebook.
  main_window_notebook_ -> append_page(*temp_box_ptr,
                                       playlist_page_label);

  // Sets the Playlist tab as detachable.
//  main_window_notebook_ -> set_tab_detachable(*temp_box_ptr, true);

  // Sets the Playlist tab as reorderable.
//  main_window_notebook_ -> set_tab_reorderable(*temp_box_ptr, true);

  // Sets the orientation of main_content_paned_.
  main_content_paned_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);

  // Adds the left box to the left paned side.
  main_content_paned_ -> pack1(*left_main_content_paned_box_, Gtk::EXPAND);

  // Adds the right box to the right paned side.
  main_content_paned_ 
    -> pack2(((*(playlists()().begin())) -> box()), Gtk::EXPAND);

  // Sets the orientation of the left paned box.
  left_main_content_paned_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);
 
  // Sets the left margin of the playlist in Main Content.
  ((*(playlists()().begin())) -> box()).set_margin_left(3);

  // Sets the right margin of the playlist in Main Content.
  ((*(playlists()().begin())) -> box()).set_margin_right(3);

  // Sets the upper margin of the playlist in Main Content.
  ((*(playlists()().begin())) -> box()).set_margin_top(3);



  //                 //
  //                 //
  // Volume Creation //////////////////////////////////////////////////////////
  //                 //
  //                 //

  // Adds the volume box to the header bar.
  status_bar_box_ -> pack_start(*main_volume_button_box_, Gtk::PACK_SHRINK);

  // Creates a Gtk::Sepratator pointer.
  Gtk::Separator* separator;

  // Creates the Gtk::Separator object.
  separator = Gtk::manage(new Gtk::Separator);

  // Sets the separator orientation to vertical.
  separator -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Adds the separator to the start of the header bar.
  status_bar_box_ -> pack_start(*separator, Gtk::PACK_SHRINK);

  // Sets the left margin of the main_volume_button_box_.
  main_volume_button_box_ -> set_margin_left(5);

  // Sets the right margin of the main_volume_button_box_.
  main_volume_button_box_ -> set_margin_right(5);

  // Adds main_volume_button_ to main_volume_button_box_.
  main_volume_button_box_ -> set_center_widget(*main_volume_button_);

  // Sets the value of the main_volume_button_.
  main_volume_button_ -> set_value(base().config().get("playback.volume"));

  // Attachs the function called when the value of the button changes.
  main_volume_button_ -> signal_value_changed()
    .connect(sigc::mem_fun(*this, 
                           &GUI::On_Signal_Value_Changed_Main_Volume_Button));



  //                         //
  //                         //
  // Header MenuBar Creation //////////////////////////////////////////////////
  //                         //
  //                         //

  // Creates two MenuBar pointers.
  MenuBar *temp_menubar1,
          *temp_menubar2;

  // Puts the first menubars in the menubars_ list.
  menubars_.push_back(temp_menubar1);

  // Puts the second menubars in the menubars_ list.
  menubars_.push_back(temp_menubar2);

  // Creates an iterator to the start of the menubars_ list.
  list<MenuBar*>::iterator menubars_it = menubars_.begin();

  // Creates a MenuBar object for each MenuBar pointer in the menubars_ list.
  while(menubars_it != menubars_.end())
  {  

    // Creates a menubar object and sets the current iterator location to it.
    (*menubars_it) = new MenuBar(base_ref);

    // Increments the iterator.
    menubars_it++;

  }

  // Adds the menubar to the header.
  header_bar_ -> pack_start((*(menubars_.begin())) -> box());



  //                                   //
  //                                   //
  // Playlist ComboBox Header Creation ////////////////////////////////////////
  //                                   //
  //                                   //

  // Adds a playlist combobox from the beginning of the playlist_comboboxes_.
  header_bar_ -> pack_end((*(playlist_comboboxes()().begin())) -> box());



  //                           //
  //                           //
  // Playback Buttons Creation ////////////////////////////////////////////////
  //                           //
  //                           //

  // Creates the Gtk::Separator object.
  separator = Gtk::manage(new Gtk::Separator);

  // Sets the separator orientation to vertical.
  separator -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Adds the playback_buttons_stack_switcher to header_bar_.
  top_box_ -> pack_start(*playback_buttons_stack_switcher_, Gtk::PACK_SHRINK);


  // Sets the margin on the end of the switcher to 10.
  playback_buttons_stack_switcher_ -> set_margin_end(10);

  // Sets the margin on the start of the switcher to 3.
  playback_buttons_stack_switcher_ -> set_margin_start(3);

  // Sets the margin on the top of the switcher to 3.
  playback_buttons_stack_switcher_ -> set_margin_top(3);

  // Sets the margin on the bottom of the switcher to 3.
  playback_buttons_stack_switcher_ -> set_margin_bottom(3);



  // Adds the previous button to the playback_button_stack_switcher_.
  playback_buttons_stack_switcher_
    -> pack_start(*previous_button_, Gtk::PACK_SHRINK, 0);

  // Adds the stop button to the playback_button_stack_switcher_.
  playback_buttons_stack_switcher_
    -> pack_start(*stop_button_, Gtk::PACK_SHRINK, 0);

  // Adds the pause button to the playback_button_stack_switcher_.
  playback_buttons_stack_switcher_
    -> pack_start(*pause_button_, Gtk::PACK_SHRINK, 0);

  // Adds the play button to the playback_button_stack_switcher_.
  playback_buttons_stack_switcher_
    -> pack_start(*play_button_, Gtk::PACK_SHRINK, 0);

  // Adds the play button to the playback_button_stack_switcher_.
  playback_buttons_stack_switcher_
    -> pack_start(*next_button_, Gtk::PACK_SHRINK, 0);



  // Sets the icon of the play button.
  previous_button_ 
    -> set_image_from_icon_name("media-skip-backward-symbolic",
                                Gtk::ICON_SIZE_DND);

  // Sets the icon of the play button.
  play_button_ -> set_image_from_icon_name("media-playback-start-symbolic",
                                           Gtk::ICON_SIZE_DND);

  // Sets the icon of the pause button
  pause_button_ -> set_image_from_icon_name("media-playback-pause-symbolic",
                                            Gtk::ICON_SIZE_DND);

  // Sets the icon of the stop button.
  stop_button_ -> set_image_from_icon_name("media-playback-stop-symbolic",
                                           Gtk::ICON_SIZE_DND);

  // Sets the icon of the next button.
  next_button_ -> set_image_from_icon_name("media-skip-forward-symbolic",
                                           Gtk::ICON_SIZE_DND);



  // Connects the click of the pause button to its function.
  next_button_ -> signal_clicked().connect(sigc::mem_fun(playback(),
                                           &Playback::Next_Track));

  // Connects the click of the pause button to its function.
  pause_button_ -> signal_clicked().connect(sigc::mem_fun(playback(),
                                            &Playback::Pause));

  // Connects the click of the play button to its function.
  play_button_ -> signal_clicked()
    . connect(sigc::bind<Gtk::TreeRowReference, bool, long long>
        (sigc::mem_fun(playback(), &Playback::Play), 
         playlists() . selected_row_ref(), false, 0LL));

  // Connects the click of the stop button to its function.
  stop_button_ -> signal_clicked().connect(sigc::mem_fun(playback(),
                                           &Playback::Stop));



  // Makes the previous button unclickable.
  previous_button_ -> set_sensitive(false);



  //                  //
  //                  //
  // Tagview Creation ////////////////////////////////////////////////////////////
  //                  //
  //                  //

  // Creates a temporary pointer object for a new Tagview.
  Tagview *temp_tagview;

  // Creates a new Tagview and assigns it to the pointer.
  temp_tagview = new Tagview(*this);

  // Adds the Tagview to the Tagview list.
  tagviews_.push_front(temp_tagview);

  // Adds the new Tagview to the left pane's box.
  left_main_content_paned_box_ -> pack_start(*temp_tagview, Gtk::PACK_SHRINK);

  

  //                               //
  //                               //
  // Main Content MenuBar Creation ////////////////////////////////////////////
  //                               //
  //                               //

  // Adds a MenuBar to the left pane in Main Content.
  left_main_content_paned_box_ 
    -> pack_end(((*(menubars_.rbegin())) -> box()),Gtk::PACK_SHRINK);

  //                                         //
  //                                         //
  // Main Content Playlist ComboBox Creation //////////////////////////////////
  //                                         //
  //                                         //

  // Adds a PlaylistCombobox to the the left pane in Main Content.
  left_main_content_paned_box_ 
    -> pack_end(((*(playlist_comboboxes()().rbegin())) -> box()), 
                Gtk::PACK_SHRINK);



  //                     //
  //                     //
  // Status Bar Creation //////////////////////////////////////////////////////
  //                     //
  //                     //

  // Adds the Status Bar's event box to the status_bar_frame_.
  status_bar_frame_ -> add(*status_bar_event_box_);

  // Adds the status_bar_frame to the bottom of the main_window's box.
  main_window() -> box().pack_end(*status_bar_frame_, Gtk::PACK_SHRINK);

  // Adds a box to the status_bar_event_box_.
  status_bar_event_box_ -> add(*status_bar_box_);

  // Sets the orientation of the inner status_bar_box_.
  status_bar_box_ -> set_orientation(Gtk::ORIENTATION_HORIZONTAL);



  // Sets the upper margin of the status_bar_box_.
  status_bar_box_ ->set_margin_top(0);

  // Sets the lower margin of the status_bar_box_.
  status_bar_box_ -> set_margin_bottom(0);

  // Sets the end margin of the status_bar_box_.
  status_bar_box_ -> set_margin_end(0);

  // Sets the start margin of the status_bar_box_.
  status_bar_box_ -> set_margin_start(0);



  // Adds the playback status label to status_bar_box_.
  status_bar_box_ -> pack_start(*playback_status_label_, Gtk::PACK_SHRINK);

  // Sets the padding of the playback_status_label_. 
  playback_status_label_ -> set_padding(15,0);



  // Sets the orientation of the time_label_box_.
  time_label_box_ -> set_orientation(Gtk::ORIENTATION_VERTICAL);

  // Adds a label to the time_label_box_.
  time_label_box_ -> set_center_widget(*time_label_);

  // Adds the time_label_box_ to the Status Bar.
  status_bar_box_ -> set_center_widget(*time_label_box_);



  // Adds the selected tracks total time display label to status_bar_box_.
  status_bar_box_ -> pack_end(*selected_time_label_, Gtk::PACK_SHRINK);

  // Sets the padding of the selected_time_label_. 
  selected_time_label_ -> set_padding(5,5);

  // Sets the default time of the selected_time_label_.
  selected_time_label_ -> set_label("0:00.00");



  // Creates a slot for the time display.
  sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,
      &GUI::Display_Time), 0);

  // Uses the slot to make a Glib::signal_timeout.
  sigc::connection conn
    = Glib::signal_timeout().connect(my_slot, 20, Glib::PRIORITY_HIGH);



  // Enables overriding of the enter event for the status_bar_event_box_.
  status_bar_event_box_ -> set_events(Gdk::ENTER_NOTIFY_MASK);

  // Enables overriding of the leave event for the status_bar_event_box_.
  status_bar_event_box_ -> set_events(Gdk::LEAVE_NOTIFY_MASK);

  // Enables overriding of the button press event for the 
  // status_bar_event_box_.
  status_bar_event_box_ -> set_events(Gdk::BUTTON_PRESS_MASK);

  // Enables overriding of the button release event for the 
  // status_bar_event_box_.
  status_bar_event_box_ -> set_events(Gdk::BUTTON_RELEASE_MASK);



  // Overrides the enter event for the status_bar_event_box_.
  status_bar_event_box_ -> signal_enter_notify_event()
    .connect(sigc::mem_fun(*this, &GUI::Status_Bar_Event_Box_Enter));

  // Overrides the leave event for the status_bar_event_box_.
  status_bar_event_box_ -> signal_leave_notify_event()
    .connect(sigc::mem_fun(*this, &GUI::Status_Bar_Event_Box_Leave));

  // Overrides the button press event for the status_bar_event_box_.
  status_bar_event_box_ -> signal_button_press_event()
    .connect(sigc::mem_fun(*this, &GUI::Status_Bar_Event_Box_Button_Press));

  // Overrides the button release event for the status_bar_event_box_.
  status_bar_event_box_ -> signal_button_release_event()
    .connect(sigc::mem_fun(*this, &GUI::Status_Bar_Event_Box_Button_Release));





  //                   //
  //                   //
  // Artworks Creation /////////////////////////////////////////////////////////
  //                   //
  //                   //

  // 
  struct passwd* pw = getpwuid(getuid());

  // 
  const char* homedir = pw -> pw_dir;

  // 
  string directory_str = homedir;

  // 
  directory_str += "/.omp";



  cover_file_ = directory_str + "/No_Cover.svg"; 

  default_cover_file_ = cover_file_;


  // Creates two temporary Artwork pointers.
  Artwork *artwork_1,
          *artwork_2;

  //Adds the first artwork to artworks_.
  artworks_.push_back(artwork_1);

  //Adds the second artwork to artworks_.
  artworks_.push_back(artwork_2);

  // Gets an iterator to the beginning of artworks_.
  list<Artwork*>::iterator artwork_it = artworks_.begin();

  // Creates an Artwork object for each pointer entry in the list.
  while(artwork_it != artworks_.end())
  {

    // Creates a new Artwork class.
    (*artwork_it) = Gtk::manage(new Artwork(base_ref));

    // Increments the artworks_ iterator.
    artwork_it++;

  }

  // Makes a reference to the first Artwork instance.
  temp_box_ptr = &((*(artworks_.begin())) -> get_art_Box());

  // Adds the Artwork tab with the second Artwork instances.
  main_window_notebook_ -> append_page(*temp_box_ptr, artwork_page_label);

  // Adds the first Artwork instance to the left pain of the Main Content tab.
  left_main_content_paned_box_ 
    -> pack_start(((*(artworks_.rbegin())) -> get_art_Box()),
                  Gtk::PACK_EXPAND_WIDGET);

  // Sets the left margin of the Main Content's Artwork box.
  ((*(artworks_.rbegin())) -> get_art_Box()).set_margin_left(5);

  // Sets the right margin of the Main Content's Artwork box.
  ((*(artworks_.rbegin())) -> get_art_Box()).set_margin_right(5);

  // Sets the top margin of the Main Content's Artwork box.
  ((*(artworks_.rbegin())) -> get_art_Box()).set_margin_top(5);

  // Sets the bottom margin of the Main Content's Artwork box.
  ((*(artworks_.rbegin())) -> get_art_Box()).set_margin_bottom(5);





  //                        //
  //                        //
  // Configuration Creation ///////////////////////////////////////////////////
  //                        //
  //                        //

  // Creates the label for the configuration notebook page.
  Gtk::Label configuration_page_label("Configuration");

  // Creates a reference to the config_guis' box.
  Gtk::Box& config_gui_box_ref = (config_guis()().front()) -> box();

  // Appends the ConfigGUI page to the notebook.
  main_window_notebook_ -> append_page(config_gui_box_ref,
                                       configuration_page_label);

  // Makes the ConfigGUI tab detachable.
//  main_window_notebook_ -> set_tab_detachable(config_gui_box_ref, true);

  // Makes the ConfigGUI tab reorderable.
//  main_window_notebook_ -> set_tab_reorderable(config_gui_box_ref, true);





  //                       //
  //                       //
  // Finalize GUI Creation ////////////////////////////////////////////////////
  //                       //
  //                       //

  // Displays the main window and all of its child objects.
  main_window() -> window().show_all_children();



  // Sets the visibility of the Status Bar based on the
  if(config() . get("gui.hide_duplicates"))
  {

    // 
    menubars_ . back() -> box() . hide();

    // 
    playlist_comboboxes()() . back() -> box() . hide();

  }

  // 
  else
  {

    // 
    menubars_ . back() -> box() . show();

    // 
    playlist_comboboxes()() . back() -> box() . show();

  }




  // Sets the visibility of the Status Bar based on the
  if(config() . get("gui.hide_status_bar"))
  {

    // 
    status_bar_frame_ -> hide();

  }

  // 
  else
  {

    // 
    status_bar_frame_ -> show();

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

GUI::~GUI()
{

  // Deletes the instance of the Playlists class.
  delete playlists_;

  // Deletes the instance of the PlaylistComboboxes class.
  delete playlist_comboboxes_;

  // Deletes the instance of the ConfigGUIs class.
  delete config_guis_;

  // Deletes the instance of the seekbar.
  delete seekbar_;

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
  return bool(std::ifstream(filename_ref.c_str()));

}

bool GUI::On_GUI_Window_Signal_Delete_Event(GdkEventAny* event)
{

  // Releases the application window.
  main_window() -> window().get_application() -> release();


  // Ends the function.
  return true;

}

bool GUI::On_Key_Press_Event(GdkEventKey* event)
{

  // Opens file chooser dialog for adding files to the currently selected
  // playlists.
  if((event -> keyval == GDK_KEY_o) && (event->state & GDK_CONTROL_MASK))
  {

    // Opens the FileChooser.
    Add_File();

    // End the key press function.
    return true;

  }

  // Opens a new configuration window.
  else if((event -> keyval == GDK_KEY_j) && (event->state & GDK_CONTROL_MASK))
  {

    // Calls the function for opening a new ConfigurationGUI window.
    config_guis() . Open_Configuration();



    return true;

  }

  // Is true if the space bar was pressed.
  else if((event -> keyval == GDK_KEY_space))
  {

    // Pauses playback.
    playback() . Pause();

    return true;

  } 

  // Is true if the escape key is pressed.
  else if((event -> keyval == GDK_KEY_Escape))
  {

    if(!(playback() . Stopped()))
    {

      // Stops playback.
      playback().Stop();

    }



    // 
    return true;

  }

  // Is true if F11 is pressed.
  else if((event -> keyval == GDK_KEY_F11))
  {

    // True the fullscreen_ variable is true.
    if(fullscreen_)
    {

      // Unfullscreens the window.
      windows_.front() -> window().unfullscreen();

      // Sets the fullscreen variable to false.
      fullscreen_ = false;

    }
    else
    {

      // Fullscreens the window.
      windows_.front() -> window().fullscreen();

      // Sets the fullscreen variable to true.
      fullscreen_ = true;

    }

    // Ends the function.
    return true;

  }



  // Allows normal keyboard event propagation.
  return false;

}

void GUI::Quit()
{ 

  // Releases the application window.
  main_window() -> window().get_application() -> release();

  playback().Stop();

  playback().Quit();



  // 
  if(!(config() . get("playback.queue_saved")))
  {

    // 
    playlists() . database() . Clear_Playlist("Queue");

  }



  //
  playlists() . database() . Cleanup_Database();

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



  list<string> cover_art_filename_str_list;

  config() . get_list("gui.artwork.front_names", cover_art_filename_str_list);



  for(auto filename_it : cover_art_filename_str_list)
  {

    // Set file_folder_path to final cover image filename.
    std::string new_cover_filename = file_folder_path + filename_it;

    // Does nothing if true because the the cover file is the same as the cover
    // file already being used.
    if((cover_file_ == new_cover_filename))
    { 

      return;

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
        (*artwork_it) -> set_image_filename(new_cover_filename);

        // Increments the artworks_ iterator.
        artwork_it++;

      }

      // Sets cover_file_ to new cover_filename_.
      cover_file_ = new_cover_filename;



      // 
      return;

    }

  }



  // Gets an iterator to the beginning of the artworks_ list.
  list<Artwork*>::iterator artwork_it = artworks_.begin();

  // Sets all of the Artworks to the default cover image.
  while(artwork_it != (artworks_.end()))
  {

    // Sets the iterator's current Artwork image filename to the default.
    (*artwork_it) -> set_image_filename("Images/No_Cover.svg");
 
    // Increments the iterator.
    artwork_it++;

  }


  

  // Sets cover_file_ to the default cover image.
  cover_file_ = default_cover_file_;

}





//              //
// FileChoosers ///////////////////////////////////////////////////////////////
//              //

void GUI::Add_File()
{

  // Creates of new FileChooser pointer.
  FileChooser* temp_file_chooser;

  // Assigns a new FileChooser object to the pointer.
  temp_file_chooser = Create_New_File_Chooser();

  // Creates and std function pointer to the destroy function of the new
  // FileChooser.
  std::function<void(void)> temp_func_ptr
    = std::bind(&FileChooser::Destroy, temp_file_chooser);

  // Creates a new shared_ptr<ChildWindow> to hold the FileChooser 
  shared_ptr<ChildWindow> temp_window;

  // Creates the new ChildWindow.
  temp_window = Create_New_Window("Add File(s)", temp_func_ptr);

  // Adds the new FileChooser to the new ChildWindow.
  temp_window -> box().pack_start(temp_file_chooser -> box(),
                                  Gtk::PACK_EXPAND_WIDGET);

  // Displays the new ChildWindow and its contents.
  temp_window -> show();

}

FileChooser* GUI::Create_New_File_Chooser()
{

  // Creates of new FileChooser pointer.
  FileChooser* temp_file_chooser;

  // Assigns a new FileChooser object to the pointer.
  temp_file_chooser = new FileChooser(base(), file_choosers());

  // Adds the new FileChooser object to the FileChoosers list.
  file_choosers()().push_front(temp_file_chooser);

  // Adds the new FileChooser's to the iterator to it's it storage variable.
  temp_file_chooser -> set_gui_elements_it(file_choosers()().begin());

  // Returns the new FileChooser.
  return temp_file_chooser;

}





//             //
// Status Bars ////////////////////////////////////////////////////////////////
//             //

bool GUI::Display_Time(int timeout_number)
{

  // Ends the Glib::signal_timeout if the program is closing.
  if(base().playback().backend_quitting())
  {

    // Ends the Glib::signal_timeout.
    return false;

  }




  // Sets the StatusBar time if music is playing.
  if((!(playback().Stopped())))
  {

    // Creates position string pointer and assigns it a new object of the 
    // current position.
    std::string* position_string_ptr
      = base().time_converter()
                 .Nanoseconds_To_Time_Format(playback().Position());

    // Creates duration string pointer and assigns it a new object of the 
    // current duration.
    std::string* duration_string_ptr
      = base().time_converter()
                 .Nanoseconds_To_Time_Format(playback().Duration());

    // Combines the two strings into the standard time output.
    Glib::ustring final_time_string
      = *position_string_ptr + " / " + *duration_string_ptr;

    // Deletes the position string memeory.
    delete position_string_ptr;

    // Deletes the duration string memeory.
    delete duration_string_ptr;

    // Sets time_label_ with final_time_string.
    time_label_ -> set_label(final_time_string);

  }

  else if(playlists() . changing_track())
  {

    debug("hanging track");

  }

  // Is true if playback is stopped.
  else
  { 

    // Sets the time label to the zero value.
    time_label_ -> set_label("00:00.00 / 00:00.00");

    // Lets the timeout propagate.
    return true;

  }


  // Lets the timeout propagate.
  return true;


}

void GUI::Set_Selected_Time_Label(const char* new_time)
{ 

  // Sets the label used for displaying the summed time of the selected tracks.
  selected_time_label_ -> set_label(new_time); 

}

bool GUI::Status_Bar_Event_Box_Button_Press(GdkEventButton* event)
{

  // Makes a new Gdk::RGBA.
  Gdk::RGBA status_bar_box_rgba;

  // Sets the RGBA to black and 24% opacity.
  status_bar_box_rgba.set_rgba(0.0,0.0,0.0,0.24);

  // Sets the status_bar_box_ to be the color of the new RGBA.
  status_bar_box_ -> override_background_color(status_bar_box_rgba,
                                               Gtk::STATE_FLAG_NORMAL);



  // Makes the function do nothing if nothing is playing.
  if(base().playback().playing_track().Is_Empty())
  { 

    return false;

  }



  // Is true if the StatusBar was double-clicked.
  if((event -> type) == GDK_2BUTTON_PRESS)
  { 

    // Selects the playing track.
    playlists() . Select_Row(playlists() . playing_row_ref());

    // Scrolls to the playing track.
    playlists() . Scroll_To_Row(playlists() . playing_row_ref());

    // Gets the filename of the playing track.
    string temp_string = playlists() . playing_track() . filename();



    // Updates the Tagviews as Playing.
    if(playback().Playing())
    {

      // Updates the Tagviews as Playing while including the playing track.
      Update_Tagview("Playing", playback().playing_track());

    }

    // Updates the Tagviews as Paused.
    else if(playback().Paused())
    {

      // Updates the Tagviews as Paused while including the paused track.
      Update_Tagview("Paused", playback().playing_track());

    }

    // Updates the Tagviews as Idle.
    else
    {

      // Sets the Tagviews as Idle.
      Update_Tagview("Idle", playback().empty_track());

    }



    // Sets the cover art.
    Load_Cover_Art(temp_string);



    // Sets the selected time label.
    playlists()() . front() -> Add_Selected_Tracks_Times();


    // 
    for(auto playlists_it : playlists()())
    {

      // 
      if(playlists_it -> playlist_treestore()
           == playlists() . playing_playlist_treestore())
      {

        // Sets the filename label to the newly selected track.
        playlists_it -> filename_label()
          . set_label(playlists() . playing_track() . filename());

      }

    }

  }



  // Ends the function call.
  return false;

}

bool GUI::Status_Bar_Event_Box_Button_Release(GdkEventButton *event)
{

  // Makes a new Gdk::RGBA.
  Gdk::RGBA status_bar_box_rgba;

  // Sets the RGBA to black and 12% opacity.
  status_bar_box_rgba.set_rgba(0.0,0.0,0.0,0.12);

  // Sets the status_bar_box_ to be the color of the new RGBA.
  status_bar_box_ -> override_background_color(status_bar_box_rgba,
                                               Gtk::STATE_FLAG_NORMAL);

  // Ends the function call.
  return false;

}

bool GUI::Status_Bar_Event_Box_Enter(GdkEventCrossing* event)
{

  // Makes a new Gdk::RGBA.
  Gdk::RGBA status_bar_box_rgba;

  // Sets the RGBA to black and 12% opacity.
  status_bar_box_rgba.set_rgba(0.0,0.0,0.0,0.12);

  // Sets the status_bar_box_ to be the color of the new RGBA.
  status_bar_box_ -> override_background_color(status_bar_box_rgba,
                                               Gtk::STATE_FLAG_NORMAL);

  // Ends the function call.
  return false;

}

bool GUI::Status_Bar_Event_Box_Leave(GdkEventCrossing* event)
{

  // Makes a new Gdk::RGBA.
  Gdk::RGBA status_bar_box_rgba;

  // Sets the RGBA to black and 0% opacity.
  status_bar_box_rgba.set_rgba(0.0,0.0,0.0,0.00);

  // Sets the status_bar_box_ to be the color of the new RGBA.
  status_bar_box_ -> override_background_color(status_bar_box_rgba,
                                               Gtk::STATE_FLAG_NORMAL);

  // Ends the function call.
  return false;

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
    (*(tagviews_.begin())) -> update_tags(tag_frame_label_name, new_track);

  }

}





//                   //
// Volume Spinbutton //////////////////////////////////////////////////////////
//                   //

void GUI::On_Signal_Value_Changed_Main_Volume_Button(double new_volume)
{

  // Sets the new volume level in the configuration
  base().config().set("playback.volume", new_volume);

  // Writes the configuration to the file.
  base().config().write_file();



  // Sets volume level of the volume element in the playback pipeline.
  g_object_set (G_OBJECT(playback().volume()),
                "volume", gdouble(config().get("playback.volume")),
                NULL);

}





//         //
// Windows ////////////////////////////////////////////////////////////////////
//         //

shared_ptr<ChildWindow> GUI::Create_New_Window
  (const char* window_name, auto temp_destroy_func_ptr)
{

  // Creates a window for the new ConfigGUI.
  shared_ptr<ChildWindow> temp_window(nullptr);
  temp_window = make_shared<ChildWindow>(window_name,
                                         base(), temp_destroy_func_ptr);



  // Adds the new window to the windows_ list.
  windows_.push_front(temp_window);

  // Sets the location of the new window in the windows_ list.
  temp_window -> set_location(windows_.begin());



  // Returns the new shared_ptr<ChildWindow> to whatever called this function.
  return temp_window;

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

//                //
// Normal Getters /////////////////////////////////////////////////////////////
//                //

bool& GUI::disable_menubar_functions_flag()
{ 

  return disable_menubar_functions_flag_;

}

long long GUI::duration()
{

  // Returns Playback's duration variable.
  return (base().playback().duration());

}

long long GUI::position()
{

  // Returns Playback's position variable.
  return (base().playback().position());

}





//                      //
// GUIElements Getters ///////////////////////////////////////////////////////
//                      //

ConfigurationGUIs& GUI::config_guis()
{  

  return *config_guis_; 

}

FileChoosers& GUI::file_choosers()
{

  return *file_choosers_;

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

std::list<MenuBar*>& GUI::menubars()
{ 

  return menubars_; 

}

Seekbar& GUI::seekbar()
{ 

  return *seekbar_; 

}

std::list<Tagview*>& GUI::tagviews()
{ 

  return tagviews_; 

}

std::list<std::shared_ptr<ChildWindow>>& GUI::windows()
{  

  return windows_; 

}





//            //
// Status Bar /////////////////////////////////////////////////////////////////
//            //

Gtk::Frame& GUI::status_bar_frame()
{

  return *status_bar_frame_;

 }





//         //
// Windows ////////////////////////////////////////////////////////////////////
//         //

std::shared_ptr<ChildWindow> GUI::main_window()
{ 

  return main_window_;

}





//         //
//         //
// Setters ////////////////////////////////////////////////////////////////////
//         //
//         //

//          //
// MenuBars ///////////////////////////////////////////////////////////////////
//          //

void GUI::set_disable_menubar_functions_flag(bool new_setting)
{ 

  disable_menubar_functions_flag_ = new_setting; 

}





//             //
// Status Bars ////////////////////////////////////////////////////////////////
//             //

void GUI::set_playback_status_label(const char* new_label)
{

  // Sets the playback status label with the new status.
  playback_status_label_ -> set_label(new_label);

}
