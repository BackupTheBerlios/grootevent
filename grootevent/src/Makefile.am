## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = grootevent

grootevent_SOURCES = \
	grootevent.c grootevent.h \
	parser.c parser.h \
	shellcmd.c shellcmd.h \
	callbacks.c callbacks.h

grootevent_LDADD = @PACKAGE_LIBS@

