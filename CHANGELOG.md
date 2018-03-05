# Changelog
All notable changes to this project will be documented in this file.



## [0.0.6] - 2018-03-04

### Added
- Configurable keyboard shortcuts.

### Changed
- Windows now use the GUIElement base class and have had their code cleaned up.



## [0.0.5] - 2018-02-18

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



## [0.0.4] - 2018-01-28

### Added
- Database for tracks that failed to scrobble.
- Button to reattempt failed scrobbles.
- Button to clear failed scrobbles.

### Changed
- Playlist views now save what playlist was used last.
- Lock status of playlist is now saved.
- Playlist ComboBox position is now saved.
- Scrobbling now confirmed as fully disableable.

### Removed
- Boost build requirement removed.



## [0.0.3] - 2018-01-21

### Added
- Detects if the currently loaded cover image has been deleted and continues
  searching for a new one if it has been deleted.
- Added underlines to the names of the metadata in the TagView.
- Added support for the album artist tag and the date tag.
- About window with version number and contributors.

### Changed
- "Base" changed to "OMP" in MenuBar for Quit MenuItem and About MenuItem.
- Fixed crash after changing the output sink and then restarting playback fixed.
- Fixed the cause of an occasional crash when artwork is resizing or first
  rendering.
- Fixed bug with Playback Follow Cursor. This bug caused playback to still 
  follow the last selected row that isn't the playing track after
  clicking the status bar to select the playing row.
- TagView ellipsizes a line if it's too long to display in its allocated space.
- General code clean-up, sylization, and code commenting.

### Removed
- No longer possible to open multiple instances of OMP.
- Removed the & from the File Chooser & Playlist tab.



## [0.0.2] - 2018-01-14

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
- Copy, Cut, Delete, and Paste menu items in the playlist menu now work.
- Changed old name to new name in copyright statement at the top of files.
- General code clean-up and stylization.

### Removed
- MAKEFILE_COMPILE removed.



## [0.0.1] - 2017-12-31

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
