add_project_arguments('-Wno-deprecated-declarations', language: 'cpp')
add_project_arguments('-Wno-parentheses', language: 'cpp')
add_project_arguments('-fconcepts', language: 'cpp')
add_project_arguments('-Wno-catch-value', language: 'cpp')
add_project_arguments('-Wno-return-type', language: 'cpp')
add_project_arguments('-Wno-unused-variable', language: 'cpp')
add_project_arguments('-Wno-write-strings', language: 'cpp')
add_project_arguments('-Wno-reorder', language: 'cpp')
add_project_arguments('-Wlogical-op', language: 'cpp')

gtk = dependency('gtk4', version: '>= 4.12.0')
libadwaita = dependency('libadwaita-1', version: '>= 1.4.alpha')
taglib = dependency('taglib')

program_name = 'omp'

blueprints = custom_target('blueprints',
  input: files(
      'resources/blueprints/sidebar.blp',
      'resources/blueprints/window.blp'
  ),
  output: '.',
  command: [find_program('blueprint-compiler'), 'batch-compile', '@OUTPUT@', '@CURRENT_SOURCE_DIR@', '@INPUT@'],
)

omp_resources = gnome.compile_resources(
  'omp_resources',
  join_paths(
    'resources', 'omp.gresource.xml'
  ),
  dependencies: blueprints,
  source_dir: 'resources'
)

executable(
    program_name, [
        'app.cc',
        'sidebar.cc',
        'sidebar.h',
        'appwin.cc',
        'main.cc',
        omp_resources,
    ],
    dependencies: [
        gtk,
        libadwaita,
        taglib,
    ],
    install: true,
)

icons_files = files(
    'images/No_Cover.svg',
    'images/Icon_16.png',
    'images/Icon_32.png',
    'images/Icon_48.png',
    'images/Icon_64.png',
    'images/Icon_128.png',
)

install_data(
    icons_files,
    install_dir: get_option('install_directory'),
    install_mode: 'rw-r--r--',
)

desktop_file_directory = get_option('desktop_file_directory')

install_data(
    'omp.desktop',
    install_dir: desktop_file_directory
)
