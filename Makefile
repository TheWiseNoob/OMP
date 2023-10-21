OBJS = Main.o

FLATPAK=0

CXXFLAGS += -g -DFLATPAK=$(FLATPAK) -std=c++17 -Wall -pipe -Wno-deprecated-declarations \
	-Wno-parentheses -fconcepts -Wno-catch-value -Wno-return-type

DESTDIR =



all: base 

base: $(OBJS) Makefile 
	$(CXX) $(CXXFLAGS) -o omp $(OBJS) \
	`pkg-config --cflags --libs gtkmm-4.0 gstreamer-1.0 taglib` \
	-lstdc++fs -lconfig++ -l sqlite3

Main.o: Main.cc Base.h GUI/GUI.h
	$(CXX) $(CXXFLAGS) -c Main.cc \
	`pkg-config --cflags --libs gtkmm-4.0 glibmm-2.68`


clean:
	rm -rf *.o omp



install:
ifeq ($(FLATPAK),1)
	install -Dm0755 omp $(DESTDIR)/bin/omp
	install -Dm0644 Images/No_Cover.svg $(DESTDIR)/share/OMP/No_Cover.svg
	install -Dm0644 Images/Icon.svg $(DESTDIR)/share/icons/hicolor/scalable/apps/com.openmusicplayer.OMP.svg
	install -Dm0644 Images/Icon_16.png $(DESTDIR)/share/icons/hicolor/16x16/apps/com.openmusicplayer.OMP.png
	install -Dm0644 Images/Icon_32.png $(DESTDIR)/share/icons/hicolor/32x32/apps/com.openmusicplayer.OMP.png
	install -Dm0644 Images/Icon_48.png $(DESTDIR)/share/icons/hicolor/48x48/apps/com.openmusicplayer.OMP.png
	install -Dm0644 Images/Icon_64.png $(DESTDIR)/share/icons/hicolor/64x64/apps/com.openmusicplayer.OMP.png
	install -Dm0644 Images/Icon_128.png $(DESTDIR)/share/icons/hicolor/128x128/apps/com.openmusicplayer.OMP.png
	install -Dm0644 com.openmusicplayer.OMP.desktop $(DESTDIR)/share/applications/com.openmusicplayer.OMP.desktop
	install -Dm0644 com.openmusicplayer.OMP.appdata.xml $(DESTDIR)/share/metainfo/com.openmusicplayer.OMP.appdata.xml
else
	install -Dm0755 omp $(DESTDIR)/usr/bin/omp
	install -Dm0644 Images/No_Cover.svg $(DESTDIR)/usr/share/OMP/No_Cover.svg
	install -Dm0644 Images/Icon.svg $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/com.openmusicplayer.OMP.svg
	install -Dm0644 Images/Icon_16.png $(DESTDIR)/usr/share/icons/hicolor/16x16/apps/com.openmusicplayer.OMP.png
	install -Dm0644 Images/Icon_32.png $(DESTDIR)/usr/share/icons/hicolor/32x32/apps/com.openmusicplayer.OMP.png
	install -Dm0644 Images/Icon_48.png $(DESTDIR)/usr/share/icons/hicolor/48x48/apps/com.openmusicplayer.OMP.png
	install -Dm0644 Images/Icon_64.png $(DESTDIR)/usr/share/icons/hicolor/64x64/apps/com.openmusicplayer.OMP.png
	install -Dm0644 Images/Icon_128.png $(DESTDIR)/usr/share/icons/hicolor/128x128/apps/com.openmusicplayer.OMP.png
	install -Dm0644 com.openmusicplayer.OMP.desktop $(DESTDIR)/usr/share/applications/com.openmusicplayer.OMP.desktop
	install -Dm0644 com.openmusicplayer.OMP.appdata.xml $(DESTDIR)/usr/share/metainfo/com.openmusicplayer.OMP.appdata.xml
endif
