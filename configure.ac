AC_INIT([libsecret-cli], [0.0], [thatslyude@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall])

# Check for programs
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O

AC_HEADER_STDBOOL

PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([LIBSECRET], [libsecret-1])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
