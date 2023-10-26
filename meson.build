project(
    'OMP',
    'cpp',
    default_options: [
        'cpp_std=c++17',
        'warning_level=2',
    ],
)


##################
# Module imports #
##################

gnome = import('gnome')
i18n = import('i18n')
pkgconfig = import('pkgconfig')

######################
# End module imports #
######################

################
# Dependencies #
################

#########
# Build #
#########

subdirs = [
  'src',
]

########################
# Conditional building #
########################

foreach dir : subdirs
  subdir(dir)
endforeach
