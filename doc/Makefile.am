SUBDIRS = .

## Process this file with automake to produce Makefile.in
MAINTAINERCLEANFILES = Makefile.in

CLEANFILES = doxygen-build.stamp

DOX = Doxyfile

EXTRA_DIST =

INSTIMAGES = html/doxygen.png

DOC_STAMPS = html-build.stamp

DOC_DIR = $(HTML_DIR)
INCDIR = ../src
LIBCFG_HEADER_SRC = \
	$(INCDIR)/libcfg.h

.PHONY: all-local doxygen

all-local: doxygen

doxygen: doxygen-build.stamp
	@echo Visit HTML documentation file://${PWD}/html/index.html

doxygen-build.stamp: $(DOX) $(OSCLOOPER_HEADER_SRC)
	@echo '*** Running doxygen ***'
	doxygen $(DOX)
	touch doxygen-build.stamp

clean-local:
	-rm -f *~ *.bak $(DOC_STAMPS)
	-rm -fr html
	-rm -fr latex
	-rm -fr man

distclean-local: clean
	-rm -f *.stamp
	-rm -rf html
