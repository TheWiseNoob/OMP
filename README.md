![Logo](/Images/OMP_Icon_128.png?raw=true)
# OpenMusicPlayer
#### Version: 0.0.3





## Compiling and Installing

### Dependencies

  - gtkmm3
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



### Compile & Install Instructions

#### Method 1 : Makefile and Manual Dependency Retrieval

  You will need all of the dependencies manually installed before being able to
  use the compile and install instructions below.


  To compile and then install, run the following commands:<br />


  make<br />
  sudo make install<br />


  Once compiled and installed, you can run omp as a command to open it.<br />



#### Method 2: Arch User Repository


  You can also install OMP via the AUR with the official omp-git package.


  Once compiled and installed, you can run omp as a command to open it.





## Known Bugs

  - File browser has no OK button or Cancel button if it's windowed.
  - Damaged files crash the program when metadata is read.
  - Left-clicking a row, holding it, holding shift, and then dragging breaks
    dragging.
  - Holding shift while dragging breaks dragging.
  - Dragging glitches when drag ends on non-valid drop.
  - Scrobbling might not be disableable.
  - Dragging row to top of queue causes invalid path.
  - Playing queue after closing might cause crash.





## Screenshot
![Screenshot1](Images/Screenshots/NRnilGB.png?raw=true)
