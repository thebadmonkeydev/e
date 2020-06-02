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

clean:
	rm -f ./e ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f e ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/e
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	mkdir -p ${DESTDIR}${MANPREFIX}/man7
	sed "s/VERSION/${VERSION}/g" < man/e.1 > ${DESTDIR}${MANPREFIX}/man1/e.1
	sed "s/VERSION/${VERSION}/g" < man/e.7 > ${DESTDIR}${MANPREFIX}/man7/e.7
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/e.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man7/e.7

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/e\
		${DESTDIR}${MANPREFIX}/man1/e.1\
		${DESTDIR}${MANPREFIX}/man7/e.7

.PHONY: all options clean install uninstall
