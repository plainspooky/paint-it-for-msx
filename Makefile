DIST=Paint-it (2020) (Crunchworks)
AS=sdasz80
CC=sdcc
ECHO=echo
HEXBIN=./hexbin.py
RM=rm

INFILE=paint-it
OUTFILE=paint-it
LIBS=paint-it_libs

INCLUDE=-I./msx
PARAMS=-mz80 --no-std-crt0 --data-loc 0

NORMAL=--code-loc 0x107 ./msx/crt0msx_msxdos.rel
ADVANCED=--code-loc 0x178 ./msx/crt0msx_msxdos_advanced.rel

RELOCATE=./msx/putchar.rel ./msx/getchar.rel ./msx/dos.rel ./msx/conio.rel ${LIBS}.rel

.PHONY: normal advanced clean superclean libs dist

default:
	make normal

normal:
	make libs
	${CC} ${INCLUDE} ${PARAMS} ${NORMAL} ${RELOCATE} ${INFILE}.c
	${HEXBIN} ${OUTFILE}.ihx ${OUTFILE}.com

advanced:
	make libs
	${CC} ${INCLUDE} ${PARAMS} ${ADVANCED} ${RELOCATE} ${INFILE}.c
	${HEXBIN} ${OUTFILE}.ihx ${OUTFILE}.com

libs:
	${AS} -o ${LIBS}.s

dist:
	rm -f "${DIST}.zip"
	cp -f "distribution/${DIST}.dsk" "${DIST}.dsk"
	openmsx -machine msx2 -script install.tcl
	zip -m -9 "${DIST}.zip" "${DIST}.dsk"

clean:
	${RM} ${OUTFILE}.com ${OUTFILE}.ihx

superclean:
	${RM} -f ${OUTFILE}.com ${OUTFILE}.ihx ${OUTFILE}.asm\
		${OUTFILE}.lk ${OUTFILE}.lst ${OUTFILE}.map\
		${OUTFILE}.noi ${OUTFILE}.rel ${OUTFILE}.sym


