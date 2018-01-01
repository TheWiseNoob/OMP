![Logo](/Images/Icon_Small.png?raw=true)
# OpenMusicPlayer
#### Version: 0.0.1

## COMPILING:

  Run this from the main folder, which has Makefile: time make -j6 -f Makefile

  **NOTE:** Change -j6 to -j3 if you have a dual-core processor. Change it to 
    -j9 for a 6-core processor and keep it at -j6 for a quad-core. Following a
    pattern of an additonal 3 for every two cores(1.5 for every 1 core) is 
    an acceptable practice.

  ./MAKEFILE_COMPILE is a script of the command above. You may change it
  at your conveinence.

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
  - Scrobbling might not be disablable.
  
  
## Screenshots:
![Screenshot1](Images/Screenshots/NRnilGB.png?raw=true)
