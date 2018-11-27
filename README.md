![Logo](/Images/OMP_Icon_128.png?raw=true)
# OMP : Open Music Player
#### Version: 0.0.20 : November 25th, 2018





## Screenshot
![Screenshot1](Images/Screenshots/Main.png?raw=true)





## About OMP

OMP is an open-source music player being developed for Linux. It is
programmed in modern C++ and some C using GTK, GStreamer, TagLib, libconfig, 
libclastfm, and standard C and C++ libraries. It can play most of the 
prevalent music audio file types gaplessly and stores tracks in a database.





## Next OMP Release

#### Version 0.0.20 : December 9th, 2018





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

#### Method 1 : Manual Dependency Retrieval and Makefile

  You will need to manually install all of the dependencies before being able
  to use the compile and install instructions for this method. OMP is currently
  only tested as working with Arch Linux, but it will work with most 
  distributions. Packages for distributions other than Arch Linux will be 
  supported by the release of version 0.2 of OMP.


  To compile and then install, run the following commands:

  
  make<br />
  sudo make install

  Once compiled and installed, you can run omp as a command to open it.<br />



#### Method 2: Arch User Repository

  You can also install OMP via the AUR with the official omp-git package.


  Once compiled and installed, you can run omp as a command to open it.
