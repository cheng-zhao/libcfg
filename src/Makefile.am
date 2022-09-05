SUBDIRS = .

SOURCE_FILES = libcfg.c

AM_CPPFLAGS = -I$(top_srcdir) -Wall -Werror -Wshadow -Wduplicated-cond -Wunused-parameter -fstack-protector

if ENABLE_COVERAGE
AM_CPPFLAGS += --coverage
AM_LDFLAGS = --coverage
endif

lib_LTLIBRARIES = libcfg.la

libcfg_la_SOURCES = $(SOURCE_FILES)
libcfg_la_CFLAGS = $(AM_CPPFLAGS)
libcfg_la_LDFLAGS = $(AM_LDFLAGS) -export-dynamic -version-info @LIBCFG_SO_VERSION@

libcfgincludedir = $(includedir)
libcfginclude_HEADERS = libcfg.h

clean-local: clean-gcno clean-gcda
	-rm -f *.orig

clean-gcno:
	-rm -f *.gcno
	-rm -f .libs/*.gcno

clean-gcda:
	-rm -f *.gcda
	-rm -f .libs/*.gcda

check-local: clean-gcda
