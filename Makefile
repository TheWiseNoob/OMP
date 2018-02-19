OBJS = Main.o About.o Abouts.o Base.o Parts.o GUI.o SpinButtonScale.o \
	Seekbar.o PlaylistComboBox.o PlaylistComboBoxes.o MenuBar.o Tagview.o \
	Artwork.o PlaylistsDatabase.o Playlists.o Playlist.o \
	PlaylistCreateDialog.o PlaylistTreeStore.o PlaylistMenu.o \
	FileChoosers.o FileChooser.o PlaybackController.o \
	PlaybackControllers.o ConfigurationGUI.o \
	ConfigurationGUIs.o ArtworkPanel.o GUIPanel.o \
	KeyboardShortcutsPanel.o OutputPanel.o PlaybackPanel.o \
	PlaylistPanel.o ReplayGainPanel.o ScrobblingPanel.o Panel.o \
	ChildWindow.o Playback.o ParserAndDecoder.o TrackBin.o Metadata.o \
	CueSheet.o Scrobbling.o FailedScrobblesDatabase.o TimeConversion.o \
	Tag.o Track.o Configuration.o DefaultValues.o DefaultValue.o

CFLAGS = -std=c++14 -Wno-deprecated-declarations

DESTDIR = ''

all: base 

base: $(OBJS) Makefile 
	g++ -g -Wall -pipe $(CFLAGS) -o omp $(OBJS) \
	`pkg-config --cflags --libs gtkmm-3.0` \
	`pkg-config --cflags --libs gstreamer-1.0` \
	`pkg-config --libs --cflags taglib` \
	-pthread -lclastfm -lconfig++ -l sqlite3

Main.o: Main.cc Base.h GUI/GUI.h GUI/ChildWindow.h
	g++ -g -Wall -pipe $(CFLAGS) -c Main.cc \
	-pthread `pkg-config --cflags --libs gtkmm-3.0`

About.o: GUI/Elements/Abouts/About.cc GUI/Elements/Abouts/About.h \
	GUI/GUIElement.h GUI/Elements/Abouts/Abouts.h GUI/ChildWindow.h \
	GUI/GUI.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/Elements/Abouts/About.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \

Abouts.o: GUI/Elements/Abouts/Abouts.cc GUI/Elements/Abouts/Abouts.h \
	GUI/Elements/Abouts/About.h GUI/GUIElementList.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/Elements/Abouts/Abouts.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \

Base.o: Base.cc Base.h Configuration/Configuration.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h \
	GUI/GUI.h Playback/Playback.h Metadata/Metadata.h \
	Scrobbling/Scrobbling.h TimeConversion.h
	g++ -g -Wall -pipe $(CFLAGS) -c Base.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4 giomm-2.4` \
	-pthread

Parts.o: Parts.cc Parts.h Base.h Configuration/Configuration.h \
	GUI/GUI.h Playback/Playback.h Metadata/Metadata.h \
	GUI/Elements/**/*.h Scrobbling/Scrobbling.h TimeConversion.h
	g++ -g -Wall -pipe $(CFLAGS) -c Parts.cc \
	`pkg-config --cflags --libs glibmm-2.4 gtkmm-3.0` \
	-pthread

GUI.o: Parts.h GUI/GUI.cc GUI/GUI.h Base.h Configuration/Configuration.h \
	Metadata/Metadata.h Playback/Playback.h TimeConversion.h \
	Metadata/Track.h Scrobbling/Scrobbling.h GUI/Artwork.h GUI/ChildWindow.h \
	GUI/Elements/**/*.h GUI/Tagview.h GUI/MenuBar.h GUI/SpinButtonScale.h \
	GUI/Seekbar.h GUI/Tagview.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/GUI.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread -Wno-reorder

PlaylistComboBox.o: GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.cc \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.h GUI/GUIElement.h \
	Parts.h Base.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxColumnRecord.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistsDatabase.h \
	GUI/Elements/Playlists/PlaylistMenu.h \
	Configuration/Configuration.h GUI/GUI.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

PlaylistComboBoxes.o: GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.cc \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/Playlist.h \
	GUI/GUI.h GUI/GUIElement.h GUI/GUIElementList.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

MenuBar.o: GUI/MenuBar.cc GUI/MenuBar.h Parts.h GUI/GUI.h Base.h \
	Configuration/Configuration.h Playback/Playback.h \
	GUI/ChildWindow.h GUI/Elements/Abouts/Abouts.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.h \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/MenuBar.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread -Wno-reorder

Tagview.o: GUI/Tagview.cc GUI/Tagview.h GUI/GUI.h Metadata/Track.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/Tagview.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread -Wno-unused-but-set-variable


Artwork.o: GUI/Artwork.cc GUI/Artwork.h Base.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/Artwork.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

PlaylistsDatabase.o: GUI/Elements/Playlists/PlaylistsDatabase.cc \
	GUI/Elements/Playlists/PlaylistsDatabase.h Parts.h \
	Base.h Metadata/Track.h GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h
	g++ -g -Wall -pipe $(CFLAGS) -c -Wno-write-strings \
	GUI/Elements/Playlists/PlaylistsDatabase.cc -l sqlite3 \
	`pkg-config --cflags --libs gtkmm-3.0`

Playlists.o: GUI/Elements/Playlists/Playlists.cc \
	GUI/Elements/Playlists/Playlists.h GUI/GUIElementList.h \
	GUI/Elements/Playlists/Playlist.h Parts.h \
	GUI/Elements/Playlists/PlaylistColumn.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h \
	GUI/Elements/Playlists/PlaylistsDatabase.h \
	GUI/Elements/Playlists/PlaylistCreateDialog.h \
	GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/PlaylistMenu.h \
	GUI/Elements/Playlists/PlaylistsDatabase.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h \
	GUI/GUI.h Metadata/Track.h Playback/Playback.h Playback/TrackBin.h \
	Scrobbling/Scrobbling.h Configuration/Configuration.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/Elements/Playlists/Playlists.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

Playlist.o: GUI/Elements/Playlists/Playlist.cc \
	GUI/Elements/Playlists/Playlist.h \
	GUI/GUIElement.h Parts.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h \
	GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/PlaylistMenu.h Base.h GUI/GUI.h \
	Configuration/Configuration.h \
	Metadata/Metadata.h Playback/Playback.h Playback/TrackBin.h \
	Metadata/Track.h GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.h \
	GUI/Elements/Playlists/Playlists.h TimeConversion.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/Elements/Playlists/Playlist.cc \
	-pthread `pkg-config --cflags --libs gtkmm-3.0`

PlaylistCreateDialog.o: GUI/Elements/Playlists/PlaylistCreateDialog.cc \
	GUI/Elements/Playlists/PlaylistCreateDialog.h \
	Parts.h GUI/Elements/Playlists/Playlists.h GUI/GUI.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/Playlists/PlaylistCreateDialog.cc \
	-pthread `pkg-config --cflags --libs gtkmm-3.0 gdk-3.0`

PlaylistTreeStore.o: GUI/Elements/Playlists/PlaylistTreeStore.cc \
	GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h Parts.h
	g++ -g -Wall -pipe $(CFLAGS) \
	-c GUI/Elements/Playlists/PlaylistTreeStore.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

PlaylistMenu.o: GUI/Elements/Playlists/PlaylistMenu.cc \
	GUI/Elements/Playlists/PlaylistMenu.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h Parts.h \
	GUI/Elements/Playlists/Playlist.h GUI/Elements/Playlists/Playlists.h \
	Playback/Playback.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/Playlists/PlaylistMenu.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

FileChoosers.o: GUI/Elements/FileChoosers/FileChoosers.cc \
	GUI/Elements/FileChoosers/FileChoosers.h \
	GUI/GUIElementList.h GUI/Elements/FileChoosers/FileChooser.h \
	Base.h Parts.h GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h Metadata/Metadata.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/FileChoosers/FileChoosers.cc \
	`pkg-config --cflags --libs gtkmm-3.0`
	
FileChooser.o: GUI/Elements/FileChoosers/FileChooser.cc \
	GUI/Elements/FileChoosers/FileChooser.h \
	GUI/GUIElement.h GUI/Elements/FileChoosers/FileChoosers.h \
	Metadata/Metadata.h GUI/Elements GUI/Elements/Playlists/Playlists.h \
	Base.h Configuration/Configuration.h Parts.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/FileChoosers/FileChooser.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

SpinButtonScale.o: GUI/SpinButtonScale.cc GUI/SpinButtonScale.h \
	GUI/GUI.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/SpinButtonScale.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

Seekbar.o: GUI/Seekbar.cc GUI/Seekbar.h Playback/Playback.h TimeConversion.h \
	Metadata/Track.h Configuration/Configuration.h \
	GUI/Elements/Playlists/Playlists.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/Seekbar.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4` \
	-pthread

PlaybackControllers.o: \
	GUI/Elements/PlaybackControllers/PlaybackControllers.cc \
	GUI/Elements/PlaybackControllers/PlaybackControllers.h \
	GUI/GUIElementList.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/PlaybackControllers/PlaybackControllers.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4` \
	-pthread

PlaybackController.o: GUI/Elements/PlaybackControllers/PlaybackController.cc \
	GUI/Elements/PlaybackControllers/PlaybackController.h \
	GUI/Elements/PlaybackControllers/PlaybackControllers.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/GUIElement.h Playback/Playback.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/PlaybackControllers/PlaybackController.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4` \
	-pthread

ConfigurationGUI.o: GUI/Elements/ConfigurationGUIs/ConfigurationGUI.cc \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h\
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h GUI/GUIElement.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h Base.h \
	GUI/SpinButtonScale.h GUI/GUI.h \
	GUI/Elements/ConfigurationGUIs/PanelTreeViewColumnRecord.h \
	GUI/Elements/ConfigurationGUIs/Panels/**/*.h 
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

ConfigurationGUIs.o: GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.cc \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	GUI/GUIElementList.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	Base.h Configuration/Configuration.h GUI/ChildWindow.h GUI/GUI.h \
	GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanelColumnRecord.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

GUIPanel.o: GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h GUI/GUI.h GUI/MenuBar.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

ArtworkPanel.o: GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h \
	GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanelColumnRecord.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

PlaylistPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/Playlist/PlaylistPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Playlist/PlaylistPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Playlist/PlaylistPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

KeyboardShortcutsPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

OutputPanel.o: GUI/Elements/ConfigurationGUIs/Panels/Output/OutputPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Output/OutputPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h GUI/SpinButtonScale.h \
	Playback/Playback.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Output/OutputPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

PlaybackPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h GUI/GUI.h GUI/MenuBar.h \
	Playback/Playback.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

ReplayGainPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/ReplayGain/ReplayGainPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/ReplayGain/ReplayGainPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Playback/Playback.h Configuration/Configuration.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/ReplayGain/ReplayGainPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0 gstreamer-1.0` \
	-pthread

ScrobblingPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/Scrobbling/ScrobblingPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Scrobbling/ScrobblingPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h Base.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	Scrobbling/Scrobbling.h Scrobbling/FailedScrobblesDatabase.h \
	Configuration/Configuration.h GUI/SpinButtonScale.h 
	g++ -g -Wall -pipe $(CFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Scrobbling/ScrobblingPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

Panel.o: GUI/Elements/ConfigurationGUIs/Panel.cc \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	Base.h Configuration/Configuration.h \
	GUI/GUI.h GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/Elements/ConfigurationGUIs/Panel.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread
	
ChildWindow.o: GUI/ChildWindow.cc GUI/ChildWindow.h \
	Parts.h GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/GUI.h
	g++ -g -Wall -pipe $(CFLAGS) -c GUI/ChildWindow.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

Playback.o: Parts.h Playback/Playback.cc Playback/Playback.h \
	Base.h Configuration/Configuration.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistsDatabase.h \
	GUI/Elements/Playlists/PlaylistColumnRecord.h GUI/GUI.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h GUI/GUI.h GUI/MenuBar.h \
	GUI/Seekbar.h Metadata/Metadata.h Scrobbling/Scrobbling.h \
	TimeConversion.h Metadata/Track.h Playback/ParserAndDecoder.h \
	Playback/TrackBin.h 
	g++ -g -Wno-write-strings -pipe $(CFLAGS) -c Playback/Playback.cc \
	`pkg-config --cflags --libs gstreamer-1.0` \
	`pkg-config --cflags --libs glibmm-2.4 giomm-2.4` \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-pthread

TrackBin.o: Playback/TrackBin.h Playback/TrackBin.cc Playback/Playback.h \
	Metadata/Track.h Base.h Parts.h
	g++ -g -Wall -pipe $(CFLAGS) -c Playback/TrackBin.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	`pkg-config --cflags --libs gstreamer-1.0` -pthread

ParserAndDecoder.o: Playback/ParserAndDecoder.h Playback/ParserAndDecoder.cc \
	Parts.h Metadata/Track.h
	g++ -g -Wno-write-strings -pipe $(CFLAGS) -c Playback/ParserAndDecoder.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	`pkg-config --cflags --libs gstreamer-1.0` -pthread

Metadata.o: Metadata/Metadata.h Metadata/Metadata.cc Parts.h \
	Metadata/TrackType.h Metadata/Track.h TimeConversion.h \
	Metadata/CueSheet.h
	g++ -g -Wall -pipe $(CFLAGS) -c \
	Metadata/Metadata.cc \
	`pkg-config --libs --cflags taglib` \
	`pkg-config --cflags --libs glibmm-2.4 giomm-2.4` \
	-pthread

CueSheet.o: Metadata/CueSheet.cc Metadata/CueSheet.h Metadata/Track.h TimeConversion.h
	g++ -g -Wall -pipe $(CFLAGS) -c Metadata/CueSheet.cc \
	-pthread `pkg-config --cflags --libs glibmm-2.4 giomm-2.4`

Scrobbling.o: Scrobbling/Scrobbling.cc Scrobbling/Scrobbling.h \
	Configuration/Configuration.h Base.h \
	Playback/Playback.h Metadata/Track.h \
	Scrobbling/FailedScrobblesDatabase.h
	g++ -g -Wall -pipe $(CFLAGS) -c Scrobbling/Scrobbling.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-lclastfm -pthread

FailedScrobblesDatabase.o: Scrobbling/FailedScrobblesDatabase.cc \
	Scrobbling/FailedScrobblesDatabase.h Parts.h \
	Base.h Metadata/Track.h
	g++ -g -Wall -pipe $(CFLAGS) -c -Wno-write-strings \
	Scrobbling/FailedScrobblesDatabase.cc -l sqlite3 \
	`pkg-config --cflags --libs gtkmm-3.0`

Tag.o: Metadata/Tag.cc Metadata/Tag.h
	g++ -g -Wall -pipe $(CFLAGS) -c Metadata/Tag.cc \
	-pthread `pkg-config --cflags --libs glibmm-2.4 giomm-2.4`

Track.o: Metadata/Track.cc Metadata/Track.h Metadata/Tag.h
	g++ -g -Wall -pipe $(CFLAGS) -c Metadata/Track.cc \
	-pthread `pkg-config --cflags --libs glibmm-2.4 giomm-2.4`

TimeConversion.o: TimeConversion.cc TimeConversion.h
	g++ -g -Wall -pipe -pthread $(CFLAGS) -c TimeConversion.cc

Configuration.o: Configuration/Configuration.cc Configuration/Configuration.h \
	Configuration/DefaultValue.h Configuration/DefaultValues.h
	g++ -g -Wall -pipe -pthread $(CFLAGS) -c -Wno-unused-variable \
	Configuration/Configuration.cc \
	`pkg-config --cflags --libs glibmm-2.4 giomm-2.4`	

DefaultValues.o: Configuration/DefaultValues.cc \
	Configuration/DefaultValues.h Configuration/DefaultValue.h
	g++ -g -Wall -pipe -pthread $(CFLAGS) -c \
	Configuration/DefaultValues.cc

DefaultValue.o: Configuration/DefaultValue.cc Configuration/DefaultValue.h
	g++ -g -Wall -pipe -pthread $(CFLAGS) -c \
	Configuration/DefaultValue.cc



clean:
	rm -rf *.o omp



install:
	install -Dm0755 omp $(DESTDIR)/usr/bin/omp
	install -Dm0644 Images/No_Cover.svg $(DESTDIR)/usr/share/pixmaps/No_Cover.png
	install -Dm0644 Images/OMP_Icon_16.png $(DESTDIR)/usr/share/pixmaps/OMP_Icon_16.png
	install -Dm0644 Images/OMP_Icon_32.png $(DESTDIR)/usr/share/pixmaps/OMP_Icon_32.png
	install -Dm0644 Images/OMP_Icon_48.png $(DESTDIR)/usr/share/pixmaps/OMP_Icon_48.png
	install -Dm0644 Images/OMP_Icon_64.png $(DESTDIR)/usr/share/pixmaps/OMP_Icon_64.png
	install -Dm0644 Images/OMP_Icon_128.png $(DESTDIR)/usr/share/pixmaps/OMP_Icon_128.png
	install -Dm0644 omp.desktop $(DESTDIR)/usr/share/applications/omp.desktop
