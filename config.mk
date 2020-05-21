VERSION = 0.1

PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# Includes and libs
INCS =
LIBS =

# Flags
CFLAGS = -std=c99 -pedantic -Wall ${INCS}

# Compiler
CC = musl-gcc
