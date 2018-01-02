![Logo](/Images/Icon_Small.png?raw=true)
# OpenMusicPlayer
#### Version: 0.0.1

## COMPILING and Installing:

  To compile and install, run this command:   ./MAKEFILE_COMPILE


  That command just runs this script:

  time make -j10 -f Makefile
  sudo install -DCm0755 omp /usr/bin/omp"
  sudo install -Dm0644 Images/No_Cover.svg /usr/share/pixmaps/no_cover.png"
  sudo install -Dm0644 Images/Icon_Small.png "${pkgdir}/usr/share/pixmaps/omp.png"
  sudo install -Dm0644 omp.desktop "${pkgdir}/usr/share/applications/omp.desktop"



  You'll have to have everything used by the program before compiling. I've
  only tested this with the packages from the latest version of Arch Linux.
  You will also need to move No_Cover.svg to /home/YOUR_HOME_DIRECTORY/.omp/
  until I make an install script.

  ### Dependencies Include:

    - gtkmm3
    - GTK3+
    - GStreamer
         - gstream
         - gst-libav
         - gst-plugins-bad(optional)
         - gst-plugins-good
         - gst-plugins-base
         - gst-plugins-base-libs
         - gst-plugins-ugly(optional)
    - libconfig
    - taglib
    - libclastfm 


### KNOWN BUGS:

  - File browser has no OK or Cancel buttons.
  - Damaged files crash the program when metadata is read.
  - Left-clicking a row, holding it, holding shift, and then dragging breaks
    shit.
  - Double clicking while paused just stop playback.
  - Holding shifting while dragging breaks shit.
  - Dragging glitches sometimes on drug row.
  - Resizing artwork occassionally crashes program.
  - Scrobbling might not be disableable.
  
  
## Screenshots:
![Screenshot1](Images/Screenshots/NRnilGB.png?raw=true)
