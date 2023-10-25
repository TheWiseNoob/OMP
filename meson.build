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

gtk_dep = dependency('gtk4')
libadwaita = dependency('libadwaita-1', version: '>= 1.4.alpha')
#thread_dep = dependency('threads')
#gst_dep = dependency('gstreamer-1.0')
#tag_dep = dependency('taglib')
#glibmm_dep = dependency('glibmm-2.68')
#giomm_dep = dependency('giomm-2.68')
#lastfm_dep = dependency('libclastfm')
#sqlite_dep = dependency('sqlite3')
#config_dep = dependency('libconfig++')

program_name = 'omp'

executable(
    program_name, [
        'Main.cc',
    ],
    dependencies: [
        gtk_dep,
        libadwaita,
#        thread_dep,
#        gst_dep,
#        tag_dep,
#        glibmm_dep,
#        giomm_dep,
#        lastfm_dep,
#        sqlite_dep,
#        config_dep,
    ],
    install: true,
)

icons_files = files(
    'Images/No_Cover.svg',
    'Images/Icon_16.png',
    'Images/Icon_32.png',
    'Images/Icon_48.png',
    'Images/Icon_64.png',
    'Images/Icon_128.png',
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
