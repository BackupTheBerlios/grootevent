dnl Process this file with autoconf to produce a configure script.

AC_INIT(configure.ac)
AM_INIT_AUTOMAKE(grootevent, 0.2)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

pkg_modules="gtk+-2.0 >= 1.3.13"

AC_ARG_ENABLE(nls,
  [  --disable-libwnck           Disable libwnck])

if test "$enable_libwnck" != "no"; then
	AC_DEFINE(ENABLE_LIBWNCK, , [Define to enable libwnck support])
	pkg_modules="$pkg_modules libwnck-1.0 >= 2.2.0"
	#features="${features} libwnck"
 	#TARGETS=$TARGETS' libwnck'
fi

PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

AC_PATH_PROG(XML2_CONFIG, xml2-config)

LIBXML2_CFLAGS=`xml2-config --cflags`
CC="$CC $LIBXML2_CFLAGS"

LIBXML2_LIBS=`xml2-config --libs`
LIBS="$LIBS $LIBXML2_LIBS"

dnl Use -Wall if we have gcc.
changequote(,)dnl
if test "x$GCC" = "xyes"; then
  case " $CFLAGS " in
  *[\ \	]-Wall[\ \	]*) ;;
  *) CFLAGS="$CFLAGS -Wall" ;;
  esac
fi
changequote([,])dnl

AC_OUTPUT([
Makefile
src/Makefile
dtd/Makefile
])

