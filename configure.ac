# libtool version: major:minor:patch
# Semantic versioning
# Given a version number MAJOR.MINOR.PATCH, increment the:
#
# MAJOR version when you make incompatible API changes,
# MINOR version when you add functionality in a backwards-compatible manner, and
# PATCH version when you make backwards-compatible bug fixes.
# Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.
m4_define([version_major], 1)
m4_define([version_minor], 1)
m4_define([version_patch], 0)

# Libtool versioning: see https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
m4_define([lt_current], [version_major])
m4_define([lt_revision], [version_minor])
m4_define([lt_age], 0)
m4_define([lt_version_info], [lt_current:lt_revision:lt_age])

AC_PREREQ([2.69])
AC_INIT([libcfg],
        [version_major.version_minor.version_patch],
        [gregory.david@p1sec.com],
        [],
        [https://github.com/p1-gdd/libcfg])
AC_LANG([C])
AC_CONFIG_SRCDIR([src/libcfg.c])
AC_CONFIG_HEADERS([src/autoconf.h])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_MACRO_DIRS([m4])
AM_SILENT_RULES([yes])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  tests/Makefile
  doc/Makefile
  doc/Doxyfile
  libcfg.pc
])
LIBCFG_SO_VERSION=lt_version_info
AC_SUBST(LIBCFG_SO_VERSION)

AC_DEFINE(LIBCFG_SO_VERSION, [{lt_current, lt_revision, lt_age}],
          [Libtool compatibility version])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_AR
LT_INIT

# Define configure options
AC_ARG_ENABLE(debug, [AS_HELP_STRING([--enable-debug], [build in DEBUG mode])])
AC_MSG_CHECKING([whether to build in DEBUG mode])
AS_IF([test "x${enable_debug}" = "xyes" ], AC_MSG_RESULT([yes]), AC_MSG_RESULT([no]))
AM_CONDITIONAL([DEBUG], [test x$enable_debug != xyes])
if test "x${enable_debug}" = "xyes"; then
   AM_SILENT_RULES([no])
fi

AC_ARG_ENABLE(coverage, [AS_HELP_STRING([--enable-coverage], [enable coverage test])])
AC_MSG_CHECKING([whether to enable coverage compilation])
AS_IF([test "x${enable_coverage}" = "xyes" ], AC_MSG_RESULT([yes]), AC_MSG_RESULT([no]))
AM_CONDITIONAL([ENABLE_COVERAGE],[test "x${enable_coverage}" = "xyes"])

AC_ARG_ENABLE(doc, [AS_HELP_STRING([--enable-doc], [enable documentation])])
AC_MSG_CHECKING([whether to enable documentation compilation])
AS_IF([test "x${enable_doc}" = "xyes" ], AC_MSG_RESULT([yes]), AC_MSG_RESULT([no]))
AM_CONDITIONAL([ENABLE_DOC],[test "x${enable_doc}" = "xyes"])

# gcovr
AC_CHECK_PROG([HAVE_GCOVR], [gcovr], [true], [false])
AM_CONDITIONAL([HAVE_GCOVR],[test "x${HAVE_GCOVR}" = "xtrue"])
if test "x$HAVE_GCOVR" = "xfalse"; then
   AC_MSG_ERROR([GCOVR unavailable])
fi

# Doxygen
AC_CHECK_PROG([HAVE_DOXYGEN], [doxygen], [true], [false])
if test "x$HAVE_DOXYGEN" = "xfalse"; then
   AC_MSG_ERROR([Doxygen unavailable])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Display some information about this build
echo
echo About this libcfg build:
echo
echo CC=\"$CC\"
echo CPPFLAGS=\"$CPPFLAGS\"
echo CFLAGS=\"$CFLAGS\"
echo LDFLAGS=\"$LDFLAGS\"
echo LIBS=\"$LIBS\"
echo
echo -n 'Documentation: '
test x$enable_doc = xyes && echo 'enabled' || echo 'disabled'
echo -n 'Coverage: '
test x$enable_coverage = xyes && echo 'enabled' || echo 'disabled'
echo -n 'Build mode: '
test x${enable_debug} = xyes && echo DEBUG || echo RELEASE
echo
echo We will compile the following things:
echo '  libcfg.so'
echo

AC_OUTPUT
