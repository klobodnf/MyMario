PREFIX  = /home/opt
DATADIR = $(PREFIX)/share/$(TARGET)
CFLAGS  = -g -Wall -O2
CXXFLAGS= $(CFLAGS)
LDFLAGS = -lSDL -lSDL_mixer -lSDL_ttf -lSDL_image -lGL
DEFINES = -DDATADIR=\"$(DATADIR)/\"
OBJS    = src/bonus.o src/gamepad.o src/killerblume.o src/player.o \
  src/bowser.o      src/global.o    src/levels.o      src/spiny.o  \
  src/enemy.o       src/goomba.o    src/main.o        src/turtle.o \
  src/firecircle.o  src/goombabig.o src/menu.o        src/SDL_gfxPrimitives.o \
  src/flybrett.o    src/opengl.o    src/iniparser.o   src/unixutils.o \
  src/functions.o   src/keyinput.o  src/particle.o
TARGET  = megamario

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) -o $@ -c $<

install: $(TARGET)
	rm -fr $(DATADIR)
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/share
	install -p -m 755 $(TARGET) $(PREFIX)/bin
	cp -a data $(DATADIR)
	# fix broken permission <sigh>
	chmod 755 $(DATADIR)/gfx/tiles/pipes/left
	for i in `find $(DATADIR) -name "*.PNG"`; do \
		mv $$i `echo $$i|sed s/PNG/png/`; \
	done
	for i in `find $(DATADIR) -name "*.JPG"`; do \
		mv $$i `echo $$i|sed s/JPG/jpg/`; \
	done
	
clean:
	rm -f $(OBJS) $(TARGET) *~ src/*~
