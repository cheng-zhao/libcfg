SUBDIRS = src tests
if ENABLE_DOC
SUBDIRS += doc
endif

EXTRA_DIST = libtool ltmain.sh
ACLOCAL_AMFLAGS = -Im4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libcfg.pc

if ENABLE_COVERAGE
if HAVE_GCOVR
COVERAGE_NAME = coverage
GCOVR_BIN = gcovr
GCOVR_OPTS = --object-directory src

clean-local:
	-rm -f $(COVERAGE_NAME).*

check-local:
	@echo You can run \'make $(COVERAGE_NAME)\' now to produce the coverage reports

$(COVERAGE_NAME): $(COVERAGE_NAME).html $(COVERAGE_NAME).json $(COVERAGE_NAME).xml

$(COVERAGE_NAME).html: check
	$(GCOVR_BIN) $(GCOVR_OPTS) --sort-percentage --html-self-contained --html-details -o $@
	@echo You can visit the file $@ with your favorite browser to see the report. Try \'firefox $@\' or visit file://${PWD}/$@

$(COVERAGE_NAME).json: check
	$(GCOVR_BIN) $(GCOVR_OPTS) --json-pretty -o $@
	@echo You can visit the file $@ to see the report in JSON format.

$(COVERAGE_NAME).xml: check
	$(GCOVR_BIN) $(GCOVR_OPTS) --exclude-unreachable-branches --print-summary --xml-pretty -o $@
	@echo You can visit the file $@ to see the report in XML format.

endif
endif
