OBJS = main.o ../common/squares.o ../common/options.o ../common/text.o ../common/texture.o ../common/score.o ../common/hud.o ../common/theme.o ../common/game.o ../common/rand.o ../common/level.o vmuify.o icon.o lcd.o snake.o romdisk.o

all:squares.bin

include $(KOS_BASE)/Makefile.rules

KOS_CFLAGS += -I../include -g  -Iinclude -DDREAMCAST -DOPENGL

squares.bin: ${OBJS}
	kos-c++ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -g -o squares.elf \
		$(OBJS) -L. -L$(KOS_BASE)/lib -lc99 \
		-ldcplib -ltsunami -lparallax -lkosutils -limageload -lkmg \
		-lpng -ljpeg -lgl -ltremor -lbz2 -lz -lm $(LIBS)
	sh-elf-strip squares.elf
	sh-elf-objcopy -R .stack -O binary squares.elf squares.bin

clean:
	rm -rf ${OBJS} squares.elf squares.bin

romdisk.img:
	$(KOS_GENROMFS) -f romdisk.img -d romdisk -v

icon.c: icon.hex
	$(KOS_BASE)/utils/bin2c/bin2c icon.hex icon.c icon

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

