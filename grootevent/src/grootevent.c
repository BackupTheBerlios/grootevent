#include "grootevent.h"
#include "callbacks.h"
#include "parser.h"

int main (int argc, char **argv)
{
    GdkWindow *rootwindow;		 /* RootWindow of X11*/
    GMainLoop *loop;                       /* the event loop */
    char *pw_dir;
    char *xml_file;
    uid_t uid;
    struct passwd *pass;

    /* inits some gdk stuff */
    gdk_init(&argc, &argv);

#ifdef ENABLE_LIBWNCK
    screen = wnck_screen_get_default ();
    wnck_screen_force_update (screen);  
#endif
  
    parse_options(&argc, &argv);
  
    uid = getuid();
    pass = getpwuid (uid);
    pw_dir = pass->pw_dir;
  
    xml_file = (char*) malloc (sizeof (char) * (strlen (XML_FILE) + strlen (pw_dir) + 1));
    sprintf (xml_file, "%s%s", pw_dir, XML_FILE);
    
    eventlist = parseDoc (xml_file);
    if (eventlist == NULL)
    {
	g_print ("xml error or no elements in list\n");
	exit (1);
    }
    free (xml_file);

    rootwindow = gdk_window_foreign_new (gdk_x11_get_default_root_xwindow());
    /*rootwindow = gdk_window_foreign_new (0x1200002); */

    if (rootwindow == NULL)
    {
	g_print ("rootwindow == NULL\n");
	exit (1);
    }
  
    /* check if GDK_BUTTON_PRESS_MASK is available */
    gdk_error_trap_push ();

    /* ... Call the X function which may cause an error here ... */
    gdk_window_set_events (rootwindow, GDK_BUTTON_PRESS_MASK);
    gdk_window_set_events (rootwindow, GDK_SCROLL_MASK);
  
    /* Flush the X queue to catch errors now. */
    gdk_flush ();

    if (gdk_error_trap_pop ())
    {
	char error[] = "grootevent: an error occured while querying the X server.\n"
	    "A window manager or an application is already grapping\n"
	    "the buttons from the root window.\n\n"
	    "Please read the documentation to get more help about this.\n";
	g_print (error);
	exit (1);
    }
    
    gdk_event_handler_set (event_func, NULL, NULL);

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
   
    return 0;
}

void parse_options (int *argc_ptr, char ***argv_ptr)
{
    int argc = *argc_ptr;
    char **argv = *argv_ptr;
    int i = 0;
   
    if (argc > 1)
    {
	for (i = 0; i < argc; i++)
	{
	    if (!(strcmp (argv[i], "--help")))
	    {	
		help ();
	    }
	    else if (!(strcmp (argv[i], "-?")))
	    {
		help ();
	    }
	    else if (!(strcmp (argv[i], "--no-validate")))
	    {
		setValidate (FALSE);	
	    }
	}
    }
}

void help (void)
{
    printf ("Usage: grootevent [OPTION...]\n\n"
	    "Help options\n"
	    "-?, --help\t\t\t Show this help message\n\n"
	    //"--usage\t\t\t Display brief usage message\n\n"
	    "Application options\n"
	    "--no-validate\t\t\t Disable validating of XML file\n");

    exit (0);
}
