![Logo](/Images/OMP_Icon_128.png?raw=true)
# OpenMusicPlayer
#### Version: 0.0.2

## COMPILING and Installing:

  To compile and then install, run the following commands:

  make

  make install



  You can also install OMP via the PKGBUILD provided or install it via omp-git
  that is on AUR.


  You'll have to have everything used by the program before compiling if you're
  not using the PKGBUILD. Only the packages from the latest version of 
  Arch Linux have been tested with OMP.



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
    - Boost


  Once compiled and installed, you can run omp as a command to open it.



### KNOWN BUGS:

  - File browser has no OK or Cancel buttons.
  - Damaged files crash the program when metadata is read.
  - Left-clicking a row, holding it, holding shift, and then dragging breaks
    dragging.
  - Holding shift while dragging breaks dragging.
  - Dragging glitches sometimes on drug row.
  - Resizing artwork occassionally crashes program.
  - Scrobbling might not be disableable.



## Screenshot:
![Screenshot1](Images/Screenshots/NRnilGB.png?raw=true)
