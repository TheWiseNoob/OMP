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

#include "PlaybackController.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../../../Base.h"

#include "../../../Playback/Playback.h"

#include "../Playlists/Playlists.h"

#include "PlaybackControllers.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <gtkmm/button.h>

#include <gtkmm/stackswitcher.h>

#include <gtkmm/treerowreference.h>





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

PlaybackController::PlaybackController
  (Base& base_ref, PlaybackControllers& playback_controllers_ref)

// Inherited Class

: GUIElement(base_ref, playback_controllers_ref())



// Playback Buttons

, playback_buttons_stack_switcher_(Gtk::manage(new Gtk::StackSwitcher))

, next_button_(Gtk::manage(new Gtk::Button))

, pause_button_(Gtk::manage(new Gtk::Button))

, play_button_(Gtk::manage(new Gtk::Button))

, previous_button_(Gtk::manage(new Gtk::Button))

, stop_button_(Gtk::manage(new Gtk::Button))

{

  // 
  playback_controllers_ref() . push_back(this);

  // Adds the new FileChooser's to the iterator to it's it storage variable.
  set_gui_elements_it(playback_controllers_ref() . begin());



  // 
  box() . set_center_widget(*playback_buttons_stack_switcher_);



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
         Gtk::TreeRowReference(), false, 0LL));

  // Connects the click of the stop button to its function.
  stop_button_ -> signal_clicked().connect(sigc::mem_fun(playback(),
                                           &Playback::Stop));



  // Makes the previous button unclickable.
  previous_button_ -> set_sensitive(false);





}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

PlaybackController::~PlaybackController()
{

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//         //
//         //
// Getters //////////////////////////////////////////////////////////////////
//         //
//         //

//         //
//         //
// Setters //////////////////////////////////////////////////////////////////
//         //
//         //
