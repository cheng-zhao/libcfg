SUBDIRS = .

AM_CPPFLAGS = -std=c99 -I$(top_srcdir) -I$(top_srcdir)/src -Wall -Werror -Wextra -Wshadow -Wduplicated-cond -Wunused-parameter

EXTRA_DIST = input.conf

TESTS = \
	example

check_PROGRAMS = $(TESTS)

example_SOURCES = example.h example.c
example_CPPFLAGS = $(AM_CPPFLAGS) -DDEFAULT_CONF_FILE="\"$(srcdir)/input.conf\""
example_LDADD = ../src/libcfg.la
