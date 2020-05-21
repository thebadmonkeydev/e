# E

include config.mk

SRC = e.c
OBJ = ${SRC:.c=.o}

all: options e

options:
	@echo e build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

clean:
	rm -f ./e ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f e ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/e

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/e

.PHONY: all options clean install
