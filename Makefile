# Makefile for building luaproc

# lua version
LUA_VERSION=5.3
# path to lua header files (lua-devel)
LUA_INCDIR=/usr/include/lua${LUA_VERSION}
# path to lua library (lua-devel)
LUA_LIBDIR=/usr/lib64/
# path to install library
LUA_CPATH=/usr/lib64/lua/${LUA_VERSION}

# standard makefile variables
CC=gcc
SRCDIR=src
BINDIR=bin
CFLAGS=-c -O2 -Wall -fPIC -I${LUA_INCDIR}
# MacOS X users should replace LIBFLAG with the following definition
# LIBFLAG=-bundle -undefined dynamic_lookup
LIBFLAG=-shared
#
LDFLAGS=${LIBFLAG} -L${LUA_LIBDIR} 
SOURCES=${SRCDIR}/basic.c
OBJECTS=${SOURCES:.c=.o}

# basic specific variables
LIBNAME=basic
LIB=${LIBNAME}.so

# build targets
all: ${BINDIR}/${LIB}
	@lua test/test.lua

${BINDIR}/${LIB}: ${OBJECTS}
	${CC} $^ -o $@ ${LDFLAGS} 

basic.o: basic.c
	${CC} ${CFLAGS} $^

install: 
	cp -v ${BINDIR}/${LIB} ${LUA_CPATH}

clean:
	rm -f ${OBJECTS} ${BINDIR}/${LIB}

# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: clean install

# (end of Makefile)

