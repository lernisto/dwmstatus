
include config.mk

SRC = main.c 
OBJ = ${SRC:.c=.o}

all: options dwmstatus

options:
	@echo dwmstatus build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	@echo creating $@ from config.def.h
	@cp config.def.h $@

dwmstatus: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -vf dwmstatus ${OBJ} 

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -pv ${DESTDIR}${PREFIX}/bin
	@cp -fv dwmstatus ${DESTDIR}${PREFIX}/bin
	@chmod -v 755 ${DESTDIR}${PREFIX}/bin/dwmstatus

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -vf ${DESTDIR}${PREFIX}/bin/dwmstatus

.PHONY: all options clean install uninstall
