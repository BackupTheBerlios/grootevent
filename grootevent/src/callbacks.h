#include "config.h"
#include <gdk/gdk.h>
#include <math.h>
#include <unistd.h>
#include <shellcmd.h>
#ifdef ENABLE_LIBWNCK
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#endif

/* direction of workspace switch */
#define UP 0
#define DOWN 1

void event_func (GdkEvent *event, gpointer  data);
gint start_app (char *app, float x, float y);
void sig_chld(int);     /* prototype for our SIGCHLD handler */
void switchWorkspace (int direction, gboolean rotation);

extern GList *eventlist;

