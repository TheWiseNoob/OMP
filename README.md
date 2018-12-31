![Logo](/Images/Icon.png?raw=true)
# OMP : Open Music Player
#### Version: 0.1 : December 30th, 2018





## Screenshot
![Screenshot1](Images/Screenshots/Main.png?raw=true)





## About OMP

OMP is an open-source music player being developed for Linux. It is
programmed in C++ and some C using gtkmm3, GStreamer, TagLib, libconfig, 
libclastfm, and standard C and C++ libraries. It can play mp3, FLAC, Ogg,
Ogg FLAC, ALAC, APE, and AAC(m4a container).





## Next Main OMP Release

#### Version 0.1.1 : January 20th, 2018





## Compiling and Installing

### Dependencies

  - GStreamer
     - gstreamer
     - gst-libav
     - gst-plugins-bad
     - gst-plugins-base
     - gst-plugins-base-libs
     - gst-plugins-good
     - gst-plugins-ugly
  - gtkmm3
  - libclastfm 
  - libconfig
  - sqlite
  - taglib





### Compile & Install Instructions

#### Method 1: Flatpak

  OMP now has Flatpak support! That means any distribution that supports
  Flatpak is now supported by OMP! An official Flathub package will be 
  provided soon. It is still being worked to meet Flathub's approval standards.
  More news about it is coming in the near future. Check for mentions about it
  in the Weekly News Updates on OMP's website at OpenMusicPlayer.com.

  Until the the official Flathub release, read [the guide for installing OMP's Flatpak](https://www.openmusicplayer.com/flatpak/ "OMP Flatpak")
  at OMP's official website.
  


#### Method 2: Arch User Repository

  You can install OMP via AUR with the official [omp AUR package](https://aur.archlinux.org/packages/omp/)
  package for the stable build or the [omp-git AUR package](https://aur.archlinux.org/packages/omp-git/)
  for the latest git build.

  Once compiled and installed, you can run omp as a command to open it.



#### Method 3 : Manual Dependency Retrieval and Makefile

  You will need to manually install all of the dependencies before being able
  to use the compile and install instructions for this method. OMP is currently
  only tested as working with Arch Linux.

  To compile and then install, run the following commands in a new folder
  containing the source.

  
  make<br />
  sudo make install

  Once compiled and installed, you can run omp as a command to open it.<br />



