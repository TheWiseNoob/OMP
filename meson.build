project(
    'OMP',
    'cpp',
    default_options: [
        'cpp_std=c++17',
        'warning_level=2',
    ],
)

add_project_arguments('-Wno-deprecated-declarations', language: 'cpp')
add_project_arguments('-Wno-parentheses', language: 'cpp')
add_project_arguments('-fconcepts', language: 'cpp')
add_project_arguments('-Wno-catch-value', language: 'cpp')
add_project_arguments('-Wno-return-type', language: 'cpp')
add_project_arguments('-Wno-unused-variable', language: 'cpp')
add_project_arguments('-Wno-write-strings', language: 'cpp')
add_project_arguments('-Wno-reorder', language: 'cpp')

gtkmm_dep = dependency('gtkmm-3.0')
thread_dep = dependency('threads')
gst_dep = dependency('gstreamer-1.0')
tag_dep = dependency('taglib')
glibmm_dep = dependency('glibmm-2.4')
giomm_dep = dependency('giomm-2.4')
gdk_dep = dependency('gdk-3.0')
lastfm_dep = dependency('libclastfm')
sqlite_dep = dependency('sqlite3')
config_dep = dependency('libconfig++')

executable(
    'omp', [
        'Base.cc',
        'Main.cc',
        'Parts.cc',

        'Configuration/Configuration.cc',
        'Configuration/DefaultValue.cc',
        'Configuration/DefaultValues.cc',

        'Errors/Errors.cc',

        'KeyboardShortcuts/KeyboardShortcuts.cc',

        'Metadata/TimeConversion.cc',
        'Metadata/CueSheet.cc',
        'Metadata/Tag.cc',
        'Metadata/Metadata.cc',
        'Metadata/Track.cc',

        'Playback/TrackBin.cc',
        'Playback/ParserAndDecoder.cc',
        'Playback/Playback.cc',

        'Scrobbling/Scrobbling.cc',
        'Scrobbling/FailedScrobblesDatabase.cc',

        'GUI/Artwork.cc',
        'GUI/GUI.cc',
        'GUI/Seekbar.cc',
        'GUI/SpinButtonScale.cc',
        'GUI/StatusBar.cc',
        'GUI/Tagview.cc',

        'GUI/Elements/Abouts/About.cc',
        'GUI/Elements/Abouts/Abouts.cc',
        'GUI/Elements/ChildWindows/ChildWindow.cc',
        'GUI/Elements/ChildWindows/ChildWindows.cc',
        'GUI/Elements/ConfigurationGUIs/ConfigurationGUI.cc',
        'GUI/Elements/ConfigurationGUIs/ConfigurationGUIs.cc',
        'GUI/Elements/ConfigurationGUIs/Panel.cc',
        'GUI/Elements/ConfigurationGUIs/Panels/Artwork/ArtworkPanel.cc',
        'GUI/Elements/ConfigurationGUIs/Panels/GUI/GUIPanel.cc',
        'GUI/Elements/ConfigurationGUIs/Panels/KeyboardShortcuts/KeyboardShortcutsPanel.cc',
        'GUI/Elements/ConfigurationGUIs/Panels/Output/OutputPanel.cc',
        'GUI/Elements/ConfigurationGUIs/Panels/Playback/PlaybackPanel.cc',
        'GUI/Elements/ConfigurationGUIs/Panels/Playlist/PlaylistPanel.cc',
        'GUI/Elements/ConfigurationGUIs/Panels/ReplayGain/ReplayGainPanel.cc',
        'GUI/Elements/ConfigurationGUIs/Panels/Scrobbling/ScrobblingPanel.cc',
        'GUI/Elements/FileChoosers/FileChooser.cc',
        'GUI/Elements/FileChoosers/FileChoosers.cc',
        'GUI/Elements/MainMenus/MainMenu.cc',
        'GUI/Elements/MainMenus/MainMenus.cc',
        'GUI/Elements/PlaybackControllers/PlaybackController.cc',
        'GUI/Elements/PlaybackControllers/PlaybackControllers.cc',
        'GUI/Elements/PlaylistComboBoxes/PlaylistComboBox.cc',
        'GUI/Elements/PlaylistComboBoxes/PlaylistComboBoxes.cc',
        'GUI/Elements/Playlists/Playlist.cc',
        'GUI/Elements/Playlists/PlaylistChangesCancelDialog.cc',
        'GUI/Elements/Playlists/PlaylistCreateDialog.cc',
        'GUI/Elements/Playlists/PlaylistDeleteDialog.cc',
        'GUI/Elements/Playlists/PlaylistMenu.cc',
        'GUI/Elements/Playlists/Playlists.cc',
        'GUI/Elements/Playlists/PlaylistsDatabase.cc',
        'GUI/Elements/Playlists/PlaylistTreeStore.cc',
    ],
    dependencies: [
        gtkmm_dep,
        thread_dep,
        gst_dep,
        tag_dep,
        glibmm_dep,
        giomm_dep,
        gdk_dep,
        lastfm_dep,
        sqlite_dep,
        config_dep,
    ],
    install: true,
)

icons_files = files(
    'Images/No_Cover.svg',
    'Images/OMP_Icon_16.png',
    'Images/OMP_Icon_32.png',
    'Images/OMP_Icon_48.png',
    'Images/OMP_Icon_64.png',
    'Images/OMP_Icon_128.png',
)

install_data(
    icons_files,
    install_dir: '/usr/share/omp',
    install_mode: 'rw-r--r--',
)

install_data(
    'omp.desktop',
    install_dir: '/usr/share/applications'
)
