OBJS = main.o ../common/squares.o ../common/text.o ../common/texture.o ../common/options.o ../common/score.o ../common/hud.o sys_sdl.o input_sdl.o ../common/theme.o ../common/game.o ../common/rand.o ../common/level.o

all:squares

CPPFLAGS += -I../include -Iinclude -I/usr/include/plib -DLINUX -DSDL -DUNIX `sdl-config --cflags`

squares: ${OBJS}
	g++ $(CPPFLAGS) $(LDFLAGS) -o squares $(OBJS) -L/usr/X11R6/lib -lSDL -lSDL_mixer -lSDL_image /usr/lib/libplibfnt.a /usr/lib/libplibul.a -logg -lvorbis -lvorbisfile -lGL -lGLU -lm -lpthread
	cp squares loki/bin/Linux/x86/glibc-2.1/dcsquares
clean:
	rm -rf ${OBJS} squares
package:
	~/loki_setup/makeself/makeself.sh --follow loki dcsquares-2.0-linux.run dcsquares ./setup.sh
