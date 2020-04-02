AS=sdasz80
CAT=cat
CC=sdcc
ECHO=echo
HEXBIN=./hexbin.py
RM=rm

INFILE=paint-it.c
OUTFILE=paint-it

LIBS=set_colors.rel set_video_mode.rel set_tiles.rel vdp.rel

INCLUDE=-I./msx
PARAMS=-mz80 --no-std-crt0 --data-loc 0

NORMAL=--code-loc 0x107 ./msx/crt0msx_msxdos.rel
ADVANCED=--code-loc 0x178 ./msx/crt0msx_msxdos_advanced.rel

RELOCATE=./msx/putchar.rel ./msx/getchar.rel ./msx/dos.rel ./msx/conio.rel ${LIBS}

.PHONY: normal advanced clean superclean libs

default:
	make normal

normal:
	make libs
	${CC} ${INCLUDE} ${PARAMS} ${NORMAL} ${RELOCATE} ${INFILE}
	${HEXBIN} ${OUTFILE}.ihx ${OUTFILE}.com

advanced:
	make libs
	${CC} ${INCLUDE} ${PARAMS} ${ADVANCED} ${RELOCATE} ${INFILE}
	${HEXBIN} ${OUTFILE}.ihx ${OUTFILE}.com

clean:
	${RM} ${OUTFILE}.com ${OUTFILE}.ihx

superclean:
	${RM} -f ${OUTFILE}.com ${OUTFILE}.ihx ${OUTFILE}.asm\
		${OUTFILE}.lk ${OUTFILE}.lst ${OUTFILE}.map\
		${OUTFILE}.noi ${OUTFILE}.rel ${OUTFILE}.sym

libs:
	${AS} -o set_colors.s
	${AS} -o set_video_mode.s
	${AS} -o set_tiles.s
	${AS} -o vdp.s
