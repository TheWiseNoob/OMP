OBJS = About.o Abouts.o Artwork.o ArtworkPanel.o Base.o ChildWindow.o \
	ChildWindows.o Configuration.o ConfigurationGUI.o ConfigurationGUIs.o \
	CueSheet.o DefaultValue.o DefaultValues.o GUI.o GUIPanel.o \
	Errors.o FailedScrobblesDatabase.o FileChoosers.o FileChooser.o \
	KeyboardShortcuts.o KeyboardShortcutsPanel.o Main.o MainMenu.o \
	MainMenus.o Metadata.o OutputPanel.o Panel.o ParserAndDecoder.o \
	Parts.o Playback.o PlaybackController.o PlaybackControllers.o \
	PlaybackPanel.o Playlist.o PlaylistChangesCancelDialog.o \
	PlaylistComboBox.o PlaylistComboBoxes.o PlaylistCreateDialog.o \
	PlaylistDeleteDialog.o PlaylistMenu.o PlaylistPanel.o \
	PlaylistRenameDialog.o PlaylistTreeStore.o Playlists.o \
	PlaylistsDatabase.o ReplayGainPanel.o Scrobbling.o ScrobblingPanel.o \
	Seekbar.o SpinButtonScale.o StatusBar.o Tag.o Tagview.o \
	TimeConversion.o Track.o TrackBin.o

CXXFLAGS += -g -std=c++17 -Wall -pipe -Wno-deprecated-declarations \
	-Wno-parentheses -fconcepts -Wno-catch-value -Wno-return-type

DESTDIR = ''

all: base 

base: $(OBJS) Makefile 
	$(CXX) $(CXXFLAGS) -o omp $(OBJS) \
	`pkg-config --cflags --libs gtkmm-3.0 gstreamer-1.0 taglib` \
	-lstdc++fs -lclastfm -lconfig++ -l sqlite3

About.o: GUI/Elements/Abouts/About.cc GUI/Elements/Abouts/About.h \
	GUI/GUIElement.h GUI/Elements/Abouts/Abouts.h \
	GUI/Elements/ChildWindows/ChildWindow.h GUI/GUI.h
	$(CXX) $(CXXFLAGS) -DAPP_DIR=$(APPDIR) -c GUI/Elements/Abouts/About.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \

Abouts.o: GUI/Elements/Abouts/Abouts.cc GUI/Elements/Abouts/Abouts.h \
	GUI/Elements/Abouts/About.h GUI/GUIElementList.h \
	GUI/Elements/ChildWindows/ChildWindow.h \
	GUI/Elements/ChildWindows/ChildWindows.h
	$(CXX) $(CXXFLAGS) -c GUI/Elements/Abouts/Abouts.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \

Artwork.o: GUI/Artwork.cc GUI/Artwork.h Base.h Configuration/Configuration.h
	$(CXX) $(CXXFLAGS) -c GUI/Artwork.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

ArtworkPanel.o: GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h \
	GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanelColumnRecord.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

Base.o: Base.cc Base.h Configuration/Configuration.h \
	GUI/Elements/ChildWindows/ChildWindow.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Errors/Errors.h KeyboardShortcuts/KeyboardShortcuts.h \
	GUI/Elements/FileChoosers/FileChoosers.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h \
	GUI/GUI.h Playback/Playback.h Metadata/Metadata.h \
	Scrobbling/Scrobbling.h Metadata/TimeConversion.h
	$(CXX) $(CXXFLAGS) -c Base.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4 giomm-2.4`

ChildWindow.o: GUI/Elements/ChildWindows/ChildWindow.cc \
	GUI/Elements/ChildWindows/ChildWindow.h \
	GUI/Elements/ChildWindows/ChildWindows.h \
	GUI/Elements/MainMenus/MainMenu.h \
	GUI/Elements/MainMenus/MainMenus.h \
	Parts.h Configuration/Configuration.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/GUI.h
	$(CXX) $(CXXFLAGS) -c GUI/Elements/ChildWindows/ChildWindow.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

ChildWindows.o: GUI/Elements/ChildWindows/ChildWindows.cc \
	GUI/Elements/ChildWindows/ChildWindows.h GUI/GUIElementList.h \
	GUI/Elements/ChildWindows/ChildWindow.h \
	Base.h Parts.h GUI/Elements/Playlists/Playlists.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ChildWindows/ChildWindows.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

Configuration.o: Configuration/Configuration.cc Configuration/Configuration.h \
	Configuration/DefaultValue.h Configuration/DefaultValues.h
	$(CXX) $(CXXFLAGS) -c -Wno-unused-variable \
	Configuration/Configuration.cc \
	`pkg-config --cflags --libs glibmm-2.4 giomm-2.4`	

ConfigurationGUI.o: GUI/Elements/ConfigurationGUIs/ConfigurationGUI.cc \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h\
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h GUI/GUIElement.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h Base.h \
	GUI/SpinButtonScale.h GUI/GUI.h \
	GUI/Elements/ConfigurationGUIs/PanelTreeViewColumnRecord.h \
	GUI/Elements/ConfigurationGUIs/Panels/**/*.h 
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

ConfigurationGUIs.o: GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.cc \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	GUI/GUIElementList.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	Base.h Configuration/Configuration.h GUI/GUI.h \
	GUI/Elements/ChildWindows/ChildWindow.h \
	GUI/Elements/ChildWindows/ChildWindows.h \
	GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanelColumnRecord.h \
	GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanelColumnRecord.h \
	GUI/Elements/MainMenus/MainMenus.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

CueSheet.o: Metadata/CueSheet.cc Metadata/CueSheet.h Metadata/Track.h \
	Metadata/TimeConversion.h
	$(CXX) $(CXXFLAGS) -c Metadata/CueSheet.cc \
	`pkg-config --cflags --libs glibmm-2.4 giomm-2.4`

DefaultValue.o: Configuration/DefaultValue.cc Configuration/DefaultValue.h
	$(CXX) -pthread $(CXXFLAGS) -c \
	Configuration/DefaultValue.cc

DefaultValues.o: Configuration/DefaultValues.cc \
	Configuration/DefaultValues.h Configuration/DefaultValue.h
	$(CXX) -pthread $(CXXFLAGS) -c \
	Configuration/DefaultValues.cc

Errors.o: Errors/Errors.cc Errors/Errors.h Parts.h \
	GUI/Elements/ChildWindows/ChildWindow.h \
	GUI/Elements/ChildWindows/ChildWindows.h
	$(CXX) $(CXXFLAGS) -c \
	Errors/Errors.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

FailedScrobblesDatabase.o: Scrobbling/FailedScrobblesDatabase.cc \
	Scrobbling/FailedScrobblesDatabase.h Parts.h \
	Base.h Metadata/Track.h
	$(CXX) $(CXXFLAGS) -c -Wno-write-strings \
	Scrobbling/FailedScrobblesDatabase.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-l sqlite3 

FileChooser.o: GUI/Elements/FileChoosers/FileChooser.cc \
	GUI/Elements/FileChoosers/FileChooser.h \
	Errors/Errors.h GUI/GUIElement.h GUI/Elements/FileChoosers/FileChoosers.h \
	Metadata/Metadata.h GUI/Elements/ChildWindows/ChildWindow.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistsDatabase.h Base.h \
	Configuration/Configuration.h Parts.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/FileChoosers/FileChooser.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

FileChoosers.o: GUI/Elements/FileChoosers/FileChoosers.cc \
	GUI/Elements/FileChoosers/FileChoosers.h GUI/GUI.h \
	GUI/GUIElementList.h GUI/Elements/FileChoosers/FileChooser.h \
	Base.h Parts.h GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/ChildWindows/ChildWindow.h \
	GUI/Elements/ChildWindows/ChildWindows.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h Metadata/Metadata.h \
	GUI/Elements/Playlists/Playlists.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/FileChoosers/FileChoosers.cc \
	`pkg-config --cflags --libs gtkmm-3.0`
	
GUI.o: Parts.h GUI/GUI.cc GUI/GUI.h Base.h Configuration/Configuration.h \
	Metadata/Metadata.h Playback/Playback.h Metadata/TimeConversion.h \
	Metadata/Track.h Scrobbling/Scrobbling.h GUI/Artwork.h \
	GUI/Elements/**/*.h GUI/StatusBar.h GUI/Tagview.h \
	GUI/SpinButtonScale.h GUI/Seekbar.h GUI/Tagview.h \
	KeyboardShortcuts/KeyboardShortcuts.h
	$(CXX) $(CXXFLAGS) -c GUI/GUI.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-Wno-reorder

GUIPanel.o: GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h GUI/GUI.h \
	GUI/Elements/MainMenus/MainMenu.h \
	GUI/Elements/MainMenus/MainMenus.h GUI/StatusBar.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

KeyboardShortcuts.o: Parts.h KeyboardShortcuts/KeyboardShortcuts.h \
	KeyboardShortcuts/KeyboardShortcuts.cc \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h GUI/GUI.h \
	GUI/Elements/MainMenus/MainMenus.h \
	GUI/Elements/Playlists/Playlists.h
	$(CXX) $(CXXFLAGS) -c \
	KeyboardShortcuts/KeyboardShortcuts.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

KeyboardShortcutsPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanel.h \
	GUI/GUI.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	GUI/Elements/FileChoosers/FileChoosers.h \
	Configuration/Configuration.h Playback/Playback.h \
	GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanelColumnRecord.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

MainMenu.o: GUI/Elements/MainMenus/MainMenu.cc \
	GUI/Elements/MainMenus/MainMenu.h GUI/GUIElement.h Parts.h GUI/GUI.h \
	Base.h Configuration/Configuration.h Playback/Playback.h \
	GUI/Elements/ChildWindows/ChildWindow.h GUI/Elements/Abouts/Abouts.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.h \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.h \
	GUI/Elements/FileChoosers/FileChoosers.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h \
	GUI/StatusBar.h
	$(CXX) $(CXXFLAGS) -c GUI/Elements/MainMenus/MainMenu.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-Wno-reorder

MainMenus.o: GUI/Elements/MainMenus/MainMenus.cc \
	GUI/Elements/MainMenus/MainMenus.h GUI/GUIElementList.h
	$(CXX) $(CXXFLAGS) -c GUI/Elements/MainMenus/MainMenus.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-Wno-reorder

Metadata.o: Metadata/Metadata.h Metadata/Metadata.cc Parts.h \
	Errors/Errors.h Metadata/TrackType.h Metadata/Track.h \
	Metadata/TimeConversion.h Metadata/CueSheet.h
	$(CXX) $(CXXFLAGS) -c \
	Metadata/Metadata.cc \
	`pkg-config --libs --cflags taglib  glibmm-2.4 giomm-2.4` \
	-lstdc++fs \

Main.o: Main.cc Base.h GUI/GUI.h GUI/Elements/ChildWindows/ChildWindow.h \
	GUI/Elements/ChildWindows/ChildWindows.h
	$(CXX) $(CXXFLAGS) -c Main.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4`

OutputPanel.o: GUI/Elements/ConfigurationGUIs/Panels/Output/OutputPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Output/OutputPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h GUI/SpinButtonScale.h \
	Playback/Playback.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Output/OutputPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

Panel.o: GUI/Elements/ConfigurationGUIs/Panel.cc \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	Base.h Configuration/Configuration.h \
	GUI/GUI.h GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h
	$(CXX) $(CXXFLAGS) -c GUI/Elements/ConfigurationGUIs/Panel.cc \
	`pkg-config --cflags --libs gtkmm-3.0`
	
ParserAndDecoder.o: Playback/ParserAndDecoder.h Playback/ParserAndDecoder.cc \
	Parts.h Metadata/Track.h
	$(CXX) -Wno-write-strings $(CXXFLAGS) -c Playback/ParserAndDecoder.cc \
	`pkg-config --cflags --libs gtkmm-3.0 gstreamer-1.0`

Parts.o: Parts.cc Parts.h Base.h Configuration/Configuration.h \
	Errors/Errors.h GUI/GUI.h GUI/StatusBar.h Playback/Playback.h \
	KeyboardShortcuts/KeyboardShortcuts.h \
	Metadata/Metadata.h GUI/Elements/**/*.h Scrobbling/Scrobbling.h \
	Metadata/TimeConversion.h
	$(CXX) $(CXXFLAGS) -c Parts.cc \
	`pkg-config --cflags --libs glibmm-2.4 gtkmm-3.0`

Playback.o: Parts.h Playback/Playback.cc Playback/Playback.h \
	Base.h Configuration/Configuration.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistsDatabase.h \
	GUI/Elements/Playlists/PlaylistColumnRecord.h GUI/GUI.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h GUI/GUI.h \
	GUI/Elements/MainMenus/MainMenu.h \
	GUI/Elements/MainMenus/MainMenus.h GUI/Seekbar.h GUI/StatusBar.h \
	Metadata/Metadata.h Scrobbling/Scrobbling.h Metadata/TimeConversion.h \
	Metadata/Track.h Playback/ParserAndDecoder.h Playback/TrackBin.h 
	$(CXX) -g -Wno-write-strings -pipe $(CXXFLAGS) -c Playback/Playback.cc \
	`pkg-config --cflags --libs gstreamer-1.0  glibmm-2.4 giomm-2.4 gtkmm-3.0`

PlaybackController.o: GUI/Elements/PlaybackControllers/PlaybackController.cc \
	GUI/Elements/PlaybackControllers/PlaybackController.h \
	GUI/Elements/PlaybackControllers/PlaybackControllers.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/GUIElement.h Playback/Playback.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/PlaybackControllers/PlaybackController.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4`

PlaybackControllers.o: \
	GUI/Elements/PlaybackControllers/PlaybackControllers.cc \
	GUI/Elements/PlaybackControllers/PlaybackControllers.h \
	GUI/GUIElementList.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/PlaybackControllers/PlaybackControllers.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4`

PlaybackPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h GUI/GUI.h \
	GUI/Elements/MainMenus/MainMenu.h GUI/Elements/MainMenus/MainMenus.h \
	Playback/Playback.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

Playlist.o: GUI/Elements/Playlists/Playlist.cc \
	GUI/Elements/Playlists/*.h \
	GUI/GUIElement.h Parts.h Base.h GUI/GUI.h \
	Configuration/Configuration.h \
	Metadata/Metadata.h Playback/Playback.h Playback/TrackBin.h \
	Metadata/Track.h GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h \
	Metadata/TimeConversion.h
	$(CXX) $(CXXFLAGS) -c GUI/Elements/Playlists/Playlist.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

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
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

PlaylistComboBoxes.o: GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.cc \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/Playlist.h \
	GUI/GUI.h GUI/GUIElement.h GUI/GUIElementList.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

PlaylistChangesCancelDialog.o: \
	GUI/Elements/Playlists/PlaylistChangesCancelDialog.cc \
	GUI/Elements/Playlists/PlaylistChangesCancelDialog.h \
	Configuration/Configuration.h Parts.h \
	GUI/Elements/Playlists/Playlists.h GUI/GUI.h Playback/Playback.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/Playlists/PlaylistChangesCancelDialog.cc \
	`pkg-config --cflags --libs gtkmm-3.0 gdk-3.0`

PlaylistCreateDialog.o: GUI/Elements/Playlists/PlaylistCreateDialog.cc \
	GUI/Elements/Playlists/PlaylistCreateDialog.h \
	Parts.h GUI/Elements/Playlists/Playlists.h GUI/GUI.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/Playlists/PlaylistCreateDialog.cc \
	`pkg-config --cflags --libs gtkmm-3.0 gdk-3.0`

PlaylistDeleteDialog.o: GUI/Elements/Playlists/PlaylistDeleteDialog.cc \
	GUI/Elements/Playlists/PlaylistDeleteDialog.h \
	Parts.h GUI/Elements/Playlists/Playlists.h GUI/GUI.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/Playlists/PlaylistDeleteDialog.cc \
	`pkg-config --cflags --libs gtkmm-3.0 gdk-3.0`

PlaylistMenu.o: GUI/Elements/Playlists/PlaylistMenu.cc \
	GUI/Elements/Playlists/PlaylistMenu.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h Parts.h \
	GUI/Elements/Playlists/Playlist.h GUI/Elements/Playlists/Playlists.h \
	Playback/Playback.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/Playlists/PlaylistMenu.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

PlaylistPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/Playlist/PlaylistPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Playlist/PlaylistPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Configuration/Configuration.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Playlist/PlaylistPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

PlaylistRenameDialog.o: GUI/Elements/Playlists/PlaylistRenameDialog.cc \
	GUI/Elements/Playlists/PlaylistRenameDialog.h \
	Parts.h GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h GUI/GUI.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/Playlists/PlaylistRenameDialog.cc \
	`pkg-config --cflags --libs gtkmm-3.0 gdk-3.0`

PlaylistTreeStore.o: GUI/Elements/Playlists/PlaylistTreeStore.cc \
	GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h Parts.h
	$(CXX) $(CXXFLAGS) \
	-c GUI/Elements/Playlists/PlaylistTreeStore.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

Playlists.o: GUI/Elements/Playlists/Playlists.cc \
	GUI/Elements/Playlists/Playlists.h GUI/GUIElementList.h \
	GUI/Elements/FileChoosers/FileChoosers.h \
	GUI/Elements/Playlists/Playlist.h Parts.h \
	GUI/Elements/Playlists/PlaylistColumn.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h \
	GUI/Elements/Playlists/PlaylistsDatabase.h \
	GUI/Elements/Playlists/PlaylistChangesCancelDialog.h \
	GUI/Elements/Playlists/PlaylistCreateDialog.h \
	GUI/Elements/Playlists/PlaylistDeleteDialog.h \
	GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/PlaylistMenu.h \
	GUI/Elements/Playlists/PlaylistsDatabase.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxColumnRecord.h \
	GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.h \
	GUI/GUI.h Metadata/Track.h Playback/Playback.h Playback/TrackBin.h \
	Scrobbling/Scrobbling.h Configuration/Configuration.h
	$(CXX) $(CXXFLAGS) -c GUI/Elements/Playlists/Playlists.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

PlaylistsDatabase.o: GUI/Elements/Playlists/PlaylistsDatabase.cc \
	GUI/Elements/Playlists/PlaylistsDatabase.h Parts.h \
	Base.h Metadata/Track.h GUI/Elements/Playlists/PlaylistColumnRecord.h \
	GUI/Elements/Playlists/Playlist.h \
	GUI/Elements/Playlists/Playlists.h \
	GUI/Elements/Playlists/PlaylistTreeStore.h
	$(CXX) $(CXXFLAGS) -c -Wno-write-strings \
	GUI/Elements/Playlists/PlaylistsDatabase.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-l sqlite3	

ReplayGainPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/ReplayGain/ReplayGainPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/ReplayGain/ReplayGainPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.h \
	Playback/Playback.h Configuration/Configuration.h
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/ReplayGain/ReplayGainPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0 gstreamer-1.0`

Scrobbling.o: Scrobbling/Scrobbling.cc Scrobbling/Scrobbling.h \
	Configuration/Configuration.h Base.h \
	Playback/Playback.h Metadata/Track.h \
	Scrobbling/FailedScrobblesDatabase.h
	$(CXX) $(CXXFLAGS) -c Scrobbling/Scrobbling.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-lclastfm

ScrobblingPanel.o: \
	GUI/Elements/ConfigurationGUIs/Panels/Scrobbling/ScrobblingPanel.cc \
	GUI/Elements/ConfigurationGUIs/Panels/Scrobbling/ScrobblingPanel.h \
	GUI/Elements/ConfigurationGUIs/Panel.h Base.h \
	GUI/Elements/ConfigurationGUIs/ConfigurationGUI.h \
	Scrobbling/Scrobbling.h Scrobbling/FailedScrobblesDatabase.h \
	Configuration/Configuration.h GUI/SpinButtonScale.h 
	$(CXX) $(CXXFLAGS) -c \
	GUI/Elements/ConfigurationGUIs/Panels/Scrobbling/ScrobblingPanel.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

Seekbar.o: GUI/Seekbar.cc GUI/Seekbar.h Playback/Playback.h \
	Metadata/TimeConversion.h Metadata/Track.h \
	Configuration/Configuration.h GUI/Elements/Playlists/Playlists.h
	$(CXX) $(CXXFLAGS) -c GUI/Seekbar.cc \
	`pkg-config --cflags --libs gtkmm-3.0 glibmm-2.4`

SpinButtonScale.o: GUI/SpinButtonScale.cc GUI/SpinButtonScale.h \
	GUI/GUI.h
	$(CXX) $(CXXFLAGS) -c GUI/SpinButtonScale.cc \
	`pkg-config --cflags --libs gtkmm-3.0`

StatusBar.o: Parts.h GUI/StatusBar.h GUI/StatusBar.cc GUI/GUI.cc GUI/GUI.h Base.h Configuration/Configuration.h \
	Metadata/Metadata.h Playback/Playback.h Metadata/TimeConversion.h \
	Metadata/Track.h Scrobbling/Scrobbling.h GUI/Artwork.h \
	GUI/Elements/**/*.h GUI/Tagview.h GUI/SpinButtonScale.h \
	GUI/Seekbar.h GUI/Tagview.h KeyboardShortcuts/KeyboardShortcuts.h
	$(CXX) $(CXXFLAGS) -c GUI/StatusBar.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-Wno-reorder

Tag.o: Metadata/Tag.cc Metadata/Tag.h
	$(CXX) $(CXXFLAGS) -c Metadata/Tag.cc \
	`pkg-config --cflags --libs glibmm-2.4 giomm-2.4`

Tagview.o: GUI/Tagview.cc GUI/Tagview.h Parts.h GUI/GUI.h Metadata/Track.h
	$(CXX) $(CXXFLAGS) -c GUI/Tagview.cc \
	`pkg-config --cflags --libs gtkmm-3.0` \
	-Wno-unused-but-set-variable

TimeConversion.o: Metadata/TimeConversion.cc Metadata/TimeConversion.h
	$(CXX) -pthread $(CXXFLAGS) -c Metadata/TimeConversion.cc

Track.o: Metadata/Track.cc Metadata/Track.h Metadata/Tag.h
	$(CXX) $(CXXFLAGS) -c Metadata/Track.cc \
	`pkg-config --cflags --libs glibmm-2.4 giomm-2.4`

TrackBin.o: Playback/TrackBin.h Playback/TrackBin.cc Playback/Playback.h \
	Metadata/Track.h Base.h Parts.h
	$(CXX) $(CXXFLAGS) -c Playback/TrackBin.cc \
	`pkg-config --cflags --libs gtkmm-3.0 gstreamer-1.0`



clean:
	rm -rf *.o omp



install:
	install -Dm0755 omp $(DESTDIR)/bin/omp
	install -Dm0755 Images/No_Cover.svg $(DESTDIR)/usr/share/OMP/No_Cover.svg
	install -Dm0755 Images/Icon.svg $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/com.openmusicplayer.OMP.svg
	install -Dm0755 Images/Icon_16.png $(DESTDIR)/usr/share/icons/hicolor/16x16/apps/com.openmusicplayer.OMP.png
	install -Dm0755 Images/Icon_32.png $(DESTDIR)/usr/share/icons/hicolor/32x32/apps/com.openmusicplayer.OMP.png
	install -Dm0755 Images/Icon_48.png $(DESTDIR)/usr/share/icons/hicolor/48x48/apps/com.openmusicplayer.OMP.png
	install -Dm0755 Images/Icon_64.png $(DESTDIR)/usr/share/icons/hicolor/64x64/apps/com.openmusicplayer.OMP.png
	install -Dm0755 Images/Icon_128.png $(DESTDIR)/usr/share/icons/hicolor/128x128/apps/com.openmusicplayer.OMP.png
	install -Dm0644 omp.desktop $(DESTDIR)/usr/share/applications/com.openmusicplayer.OMP.desktop
