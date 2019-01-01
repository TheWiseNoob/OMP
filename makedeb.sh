#!/bin/bash

echo "Enter a version number"
read version

echo "Enter architecture"
read arch

echo "Enter your name"
read name

echo "Enter your E-Mail"
read email

mkdir -p omp-$version-$arch/
cd omp-$version-$arch/

# Create folders
mkdir -p DEBIAN

mkdir -p usr/bin

mkdir -p usr/share
mkdir -p usr/share/OMP

mkdir -p usr/share/icons/hicolor/scalable/apps
mkdir -p usr/share/icons/hicolor/16x16/apps
mkdir -p usr/share/icons/hicolor/32x32/apps
mkdir -p usr/share/icons/hicolor/48x48/apps
mkdir -p usr/share/icons/hicolor/64x64/apps
mkdir -p usr/share/icons/hicolor/128x128/apps

mkdir -p usr/share/applications/
mkdir -p usr/share/metadata

# Copy files to the folders
cp ../omp usr/bin/omp

cp ../Images/No_Cover.svg usr/share/icons/hicolor/scalable/apps/com.openmusicplayer.OMP.svg
cp ../Images/Icon.svg usr/share/icons/hicolor/scalable/apps/com.openmusicplayer.OMP.svg
cp ../Images/Icon_16.png usr/share/icons/hicolor/16x16/apps/com.openmusicplayer.OMP.png
cp ../Images/Icon_32.png usr/share/icons/hicolor/32x32/apps/com.openmusicplayer.OMP.png
cp ../Images/Icon_48.png usr/share/icons/hicolor/48x48/apps/com.openmusicplayer.OMP.png
cp ../Images/Icon_64.png usr/share/icons/hicolor/64x64/apps/com.openmusicplayer.OMP.png
cp ../Images/Icon_128.png usr/share/icons/hicolor/128x128/apps/com.openmusicplayer.OMP.png
cp ../omp.desktop usr/share/applications/com.openmusicplayer.OMP.desktop
cp ../com.openmusicplayer.OMP.appdata.xml usr/share/metadata/com.openmusicplayer.OMP.appdata.xml

# Create Debian control file
echo "Package: OMP
Version: $version
Architecture: $arch
Maintainer: $name <$email>
Description: Open Music Player
Homepage: https://www.openmusicplayer.com/
Priority: standard
Depends: sqlite (>=2.8.17-14)" > DEBIAN/control

cd ..
dpkg-deb -b omp-$version-$arch/
rm -rf omp-$version-$arch/
echo "Done"