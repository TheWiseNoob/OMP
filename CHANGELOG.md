# Changelog
All notable changes to this project will be documented in this file.



## [0.0.20] - November 25th, 2018

### Added
- Files can now be opened via command line.

### Fixed
- Files open in OMP when opened in a file browser. Before, nothing happened.



## [0.0.19.1] - November 23rd, 2018

### Fixed
- Icons improved so they are no longer pixelated.



## [0.0.19] - November 4th, 2018

### Added
- Dialog for renaming a playlist. Useful for when the Playlist ComboBox is not
  using the same playlist as the active one in the playlist view.

### Changed
- Playlist create dialog code cleaned up.
- Better design of playlist create dialog.



## [0.0.18] - October 21st, 2018

### Added
- Support for mass-adding files without locking up OMP.
- Support for adding by a folder or folders recursively. That means all music
  files in all folders will be read.

### Changed
- General code clean-up.
- Both files and folders can be added now.
- Playlists are no longer deletable while changes are occurring to the playlist
  until a segfault is fixed.



## [0.0.17] - September 9th, 2018

### Added
- Simple meta editing for 10 fields of tracks that don't use a cuesheet.
  Those fields are artist(s), genre(s), album artist(s), disc number,
  disc total, track number, track total, title, album, and date.
  The three fields that allow multiple values can use a semicolon to separate
  values when editing in order to insert multiple values! The only metadata
  field for files added using a cue sheet are disc number and disc total.
  Those metadata fields are written to music file in the DISCNUMBER and
  DISCTOTAL metadata fields and not the cue sheet.
- Confirmation dialog for when OMP is closed when the database is still being
  updated. It will help ensure changes are not lost.
- Disc number and disc total metadata fields
- Keyboard shortcuts for stop after current track and editing.

### Changed
- General code clean-up.
- Playlists are now backed up to help ensure no losses occur
  when force closing or something bad happens.

### Fixed
- Default column sizes now vary in a way that makes sense.
- Default column order is now logical.



## [0.0.16] - August 26th, 2018

### Added
- Improvements for larger playlists with cutting, deleting, and dragging
  operations.

### Changed
- General code clean-up.

### Fixed
- Deleting no longer stops working.



## [0.0.15] - August 5th, 2018

### Added
- Added tooltips to various parts missing them.

  Parts now with tooltips:
    - copy progress bar 
    - normal progress bar
    - row count label
    - playlist name in the playlist status bar
    - artwork
    - tagview data
    - tagview label
    - all 5 sections of the main menu bar menu item

### Changed
- General code clean-up.

### Fixed
- Tabs now expand to fill empty space.
- Metadata in the Tagview is now properly centered.



## [0.0.14] - July 22nd, 2018

### Added
- Copying large amounts of tracks no longer locks up OMP.
- Improvements to pasting tracks, adding new tracks, dragging tracks, and
  deleting tracks in a large playlist. These improvements are good, but I'm
  still working on finishing them.
- Row count in the playlist status bar.

### Fixed
- Genres and album artists column tabs can now be clicked to sort.



## [0.0.13.1] - July 8st, 2018

### Fixed
- Duplicate Main Menu Bar and Playlist Combobox weren't hiding properly.
- The Playlist Combobox in the header bar was not present.



## [0.0.13] - July 1st, 2018

### Added
- Improvements to make OMP more usable with larger playlists.
- Playlist status bar.
- Playlist name in the playlist status bar.

### Changed
- Track filename is no longer shown at the bottom of the playlist.

### Fixed
- Clicking play a second time on the playing track now restarts the playing or
  plays a new track if it is not the playing row.
- Fixed values in the cue sheet with quotes causing parsing issues.
- Tags with multiple values, like artist, album artists, and genres, were not
  being read properly.



## [0.0.12] - June 3rd, 2018

### Added
- The Selected Playlist View Only option now allows only the selected Playlist
  View to have it's next track selected when the track changes and Cursor
  Follows Playback is on.

### Changed
- Start At Pregap is also present in the System Menu and Menu Bars now.
- Double-clicking the Status Bar now only selects the playing track in the
  selected Playlist View if the playing playlist is active.
- Triple-clicking the Status Bar will select the playing track in all playlist
  views with the playing playlist active.
- Code clean-up.

### Fixed
- The Undo Changes button in the Configuration GUI did not undo the changes 
  made to the System Menu and Menu Bars. That has been fixed.



## [0.0.11] - May 20th, 2018

### Added
- A system menu! Right click most parts of OMP to display a system menu! It
  has the exact options a system menu bar has. You can also press the keyboard
  shortcut for the system menu. The default is control + m.

### Changed
- Less space between the seekbar and the next track button.
- System menubars and the system menu now fully have all of the filler options
  removed and all have tooltips.

### Fixed
- No longer possible to delete the Library or Queue playlist using the delete
  playlist keyboard shortcut.
- Bug causing the use of the playback track queue to crash OMP when using
  playback follows cursor has been fixed.
- The artwork display is now far more stable. Some images uses to vibrate,
  infinitely attempting to resize. This is fixed.



## [0.0.10] - May 6th, 2018

### Added
- Add Files button has been added to the FileChooser. If the FileChooser is in
  a window, a Cancel button also shows that closes the window.

### Changed
- The times shown with the seekbar and the status bar for the playing track
  are now designed different. Insensitive entries are used instead of labels.
- The entry of the playlist combobox of the duplicate content under the album
  art in Main Content now expands.
- The playback status symbol in the status bar is now an image instead
  of the text symbol for play, pause, or stop.
- General code clean-up.

### Fixed
- Multiple album artists, artists, and genres now display and store properly
  for normal track files. Tracks with multi-value metadata fields and use a 
  cuesheet are not confirmed working yet. They will be working by the next 
  release.
- Switching between None and Playlist Repeat while playing or paused no longer
  breaks OMP.



## [0.0.9] - April 22nd, 2018

### Added
- A confirmation dialog for deleting a playlist is now displayed so a playlist
  is not accidentally deleted.
- Tooltips have been added to most of the parts of the program the might be
  useful.
- Time left in track is now visible in the upper right part of OMP next to
  the seekbar.

### Changed
- The tagview is now designed slightly different and has more information.
- Many placeholder buttons and parts for future features have been removed
  until their feature is implemented.
- The create and delete playlist buttons of the playlist combobox are now
  attached to the playlist combobox.
- General code clean-up.

### Fixed
- The right-click playlist menu delete option is no longer clickable when the
  playlist is Library or Queue.
- Info bar telling user that a playlist already exists disappears properly now.
- The spinbutton of 4 parts of the configuration GUI can now be used.



## [0.0.8] - April 8th, 2018

### Added
- Added a Fixed section to the CHANGELOG.
- Add and remove playlist buttons are not in a playlist's right-click menu.

### Changed
- CHANGELOG date format was changed for clarification.
- Removed the section of the CHANGELOG was removed. Things removed will now be
  listed in the Changed section.
- Removed the useless Edit button in the playlist right-click menu until 
  advanced editing is added starting with version 0.3.0.
- All bug fixes listed in the CHANGELOG have been moved to a new section called
  Fixed.
- Reorganized the playlist right-click menu.
- Cleaned up the over 3 years-old code for playlist removal in order to
  make it possible to remove a playlist from something other than a playlist
  combobox.
- The initial time listed in the status bar upon opening OMP is now formatted
  properly. 

### Fixed
- Fixed a bug that caused a playlist combobox to not saved its position after
  a playlist is removed.
- Renaming a playlist with a playlist combobox is now working again.



## [0.0.7] - March 18th, 2018

### Added
- Error logging for files that fail to open with OMP.
- Add playlist right-click menu option. 

### Changed
- Makefile organized.
- General code clean-up.
- All mentions of Boost removed.
- Other minor text changes.

### Fixed
- A crash caused by OMP trying to convert an empty string to an int has
  been fixed. It happened when a metadata field exists, is empty, and is 
  an integer. This produced an exception that is now caught too. 
  Thanks to aladar42 on GitHub helping me find and solve this bug!



## [0.0.6] - March 4th, 2018

### Added
- Configurable keyboard shortcuts.

### Changed
- Windows now use the GUIElement base class and have had their code cleaned up.



## [0.0.5] - February 18th, 2018 

### Added
- Playlist column orders now saved.
- Playlist column positions now saved and are fixed to where user sets them.
- Tab position now saved.
- Last chosen configuration GUI column now saved.
- Tabs are now hideable via the View menu or the GUI section of the
  configuration GUI.

### Changed
- Queue bug that caused a crash is now fixed.
- Fixed blank space of a track row being clicked or drug not working as 
  expected.



## [0.0.4] - January 28th, 2018

### Added
- Database for tracks that failed to scrobble.
- Button to reattempt failed scrobbles.
- Button to clear failed scrobbles.

### Changed
- Playlist views now save what playlist was used last.
- Lock status of playlist is now saved.
- Playlist ComboBox position is now saved.
- Boost build requirement removed.

### Fixed
- Scrobbling now confirmed as fully disableable.


## [0.0.3] - January 21st, 2018

### Added
- Detects if the currently loaded cover image has been deleted and continues
  searching for a new one if it has been deleted.
- Added underlines to the names of the metadata in the TagView.
- Added support for the album artist tag and the date tag.
- About window with version number and contributors.

### Changed
- "Base" changed to "OMP" in MenuBar for Quit MenuItem and About MenuItem.
- TagView ellipsizes a line if it's too long to display in its allocated space.
- General code clean-up, sylization, and code commenting.
- No longer possible to open multiple instances of OMP.
- Removed the & from the File Chooser & Playlist tab.

### Fixed
- A crash after changing the output sink and then restarting playback fixed.
- The cause of an occasional crash when artwork is resizing or first
  rendering.
- Abug caused playback to still follow the last selected row that isn't 
  the playing track after clicking the status bar to select the playing row 
  when Playback Follows Cursor is on.



## [0.0.2] - January 14th, 2018

### Added
- CHANGELOG.md added.
- Search of columns using Ctrl+f.
- Window size remembered.
- Tab with two side-by-side playlists added.
- Tab with a file chooser and a playlist side-by-side added.
- Pane positions remembered.
- Added a number to the status bar indicating that number of selected rows
  in all playlists views of the program.

### Changed
- Fix crash if CUESHEET has extra MODE/2xxx track.
- MAKEFILE_COMPILE install elements moved to install section of Makefile.
- Prevent extra newline in genre metadata if GENRE in the CUESHEET is empty.
- Prevent extra newline in date metadata if DATE in the CUESHEET is empty.
- Changed old name to new name in copyright statement at the top of files.
- General code clean-up and stylization.
- MAKEFILE_COMPILE removed.

### Fixed
- OMP no longer crashes if a CUESHEET has extra an MODE/2xxx track.
- Copy, Cut, Delete, and Paste menu items in the playlist menu now work.



## [0.0.1] - December 31st, 2017

### Added
- First public source and build release.
- Plays mp3, FLAC, Ogg, Ogg FLAC, ALAC, APE, and AAC(m4a).
- Gapless playback.
- CUESHEET support (embedded and external)
- Playlists saved in SQLite3 database.
- Multi-row drag and drop.
- Copy, cut, and pasting of rows. (No clipboard history)
- Random track next and track shuffling.
- Playlist and track looping.
- Stop after current track.
- Playback follows the cursor(selected track).
- Cursor does or doesn't follow the cursor.
- ALSA, Pulseaudio, JACK, or automatic audio sink selection.
- Last.fm scrobbling and track updating.
- Front cover image support.
- Playback of a track can start at the pregap.
