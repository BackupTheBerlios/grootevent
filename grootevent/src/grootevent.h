#include "config.h"
#ifdef ENABLE_LIBWNCK
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#endif
#include <stdio.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>

#define XML_FILE "/.grootevent.xml"

void parse_options (int *argc, char ***argv);
void help (void);

#ifdef ENABLE_LIBWNCK
WnckScreen *screen;
#endif

