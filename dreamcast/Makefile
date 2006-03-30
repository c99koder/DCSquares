TIKI_BASE = /Users/sam/tiki
OBJS = ../src/menus/DCSMenu.o ../src/menus/TitleScreen.o ../src/menus/GamePlay.o ../src/menus/LevelStats.o ../src/menus/HighScores.o ../src/menus/NameEntry.o ../src/menus/MultiPlaySetup.o ../src/menus/HowToPlay.o
OBJS += ../src/drawables/menuList.o ../src/drawables/playField.o ../src/drawables/squaresBg.o ../src/drawables/squaresHUD.o ../src/drawables/box.o
OBJS += ../src/main.o ../src/TikiSquares.o ../src/squares.o ../src/text.o ../src/texture.o ../src/score.o ../src/hud.o ../src/theme.o ../src/game.o ../src/rand.o ../src/level.o #vmuify.o icon.o lcd.o snake.o romdisk.o
OBJS += lcd.o romdisk.o

all:squares.bin

include $(KOS_BASE)/Makefile.rules

KOS_CFLAGS += -I. -I../include -I../include/menus -g  -I$(TIKI_BASE)/include -I$(TIKI_BASE)/dc/include -DDREAMCAST -DOPENGL -DGOAT -DTIKI
LDFLAGS += -L$(TIKI_BASE)/dc

squares.bin: ${OBJS}
	kos-c++ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -g -o squares.elf \
		$(OBJS) -L. -L$(KOS_BASE)/lib -lmenu \
		-ltiki -lkosutils -limageload -lkmg \
		-lpng -ljpeg -lgl -ltremor -lbz2 -lz -lm $(LIBS)
#	sh-elf-strip squares.elf
	sh-elf-objcopy -R .stack -O binary squares.elf squares.bin

clean:
	rm -rf ${OBJS} squares.elf squares.bin

romdisk.img:
	$(KOS_GENROMFS) -f romdisk.img -d ..//goat -v -x .svn

icon.c: icon.hex
	$(KOS_BASE)/utils/bin2c/bin2c icon.hex icon.c icon

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

