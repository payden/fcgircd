#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([fcgircd], [1.0], [payden@paydensutherland.com])
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([memcached],[memcached_create],[],[AC_MSG_ERROR([libmemcached is required])])
AC_CHECK_LIB([fcgi],[FCGI_Accept],[],[AC_MSG_ERROR([libfcgi is required])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
