#include <sys/types.h>
#include <sys/wait.h>

#include "parser.h"
#include "callbacks.h"
#include "grootevent.h"

/* the global event function */
void event_func (GdkEvent *event,
		 gpointer  data)
{
  GList *modifier;
  struct eventinfo *eventinfonode;

  
  /* detect wheelmouse events */
  if (event->type == GDK_SCROLL)
  {
    g_printerr ("scroll event\n");
    eventlist = g_list_first (eventlist);
    do
    {
       gboolean mod_bol = TRUE;
       eventlist = eventlist->next;
       eventinfonode = (struct eventinfo*) eventlist->data;     

       /* cycle through all modifiers */
      modifier = eventinfonode->modifier;
      modifier = g_list_first (modifier);
         
      /* if modifiers found */
      if (modifier != NULL)
      {
	do
	{
	  modifier = modifier->next;
		
	  if ((event->scroll.state & (int) modifier->data) != (int) modifier->data)
	  {
	    mod_bol = FALSE;	    
	  }

	  /* start app if all modifiers and wheel match */
	  if ((modifier->next == NULL) &&
	      (mod_bol == TRUE) &&
	      (eventinfonode->scroll == 1) &&
	      (event->scroll.direction == eventinfonode->button))
	  {
	    start_app (eventinfonode->exec_app,
		       event->button.x,
		       event->button.y);
	  
	  }
	} while (modifier->next != NULL);
      }
      else
      {
	g_printerr ("no modifier\n");
	/* exec app if wheel match and no modifiers are given */
	if ((event->scroll.direction == eventinfonode->button) &&
	    (eventinfonode->scroll == 1) &&
	    ((event->scroll.state & GDK_ALT_MASK) != GDK_ALT_MASK) &&
	    ((event->scroll.state & GDK_ALTGR_MASK) != GDK_ALTGR_MASK) &&
	    ((event->scroll.state & GDK_SUPER_MASK) != GDK_SUPER_MASK) &&
	    ((event->scroll.state & GDK_SHIFT_MASK) != GDK_SHIFT_MASK) &&
	    ((event->scroll.state & GDK_CONTROL_MASK) != GDK_CONTROL_MASK))
	{
	  g_printerr ("exec no modifier\n");
	  start_app (eventinfonode->exec_app,
		     event->button.x,
		     event->button.y);
	  
	}	
      }       
    } while (eventlist->next != NULL);            
  }


  /* detect button press events */
  if (event->type == GDK_BUTTON_PRESS)
  {
    g_printerr ("button press event\n");
    
    /* cycle through all events */
    eventlist = g_list_first (eventlist);
    do
    {
      g_printerr ("sd: %d\n", GDK_SCROLL_DOWN);
      gboolean mod_bol = TRUE;
      eventlist = eventlist->next;
      eventinfonode = (struct eventinfo*) eventlist->data;
      
      /* cycle through all modifiers */
      modifier = eventinfonode->modifier;
      modifier = g_list_first (modifier);
         
      /* if modifiers found */
      if (modifier != NULL)
      {	
	do
	{
	  modifier = modifier->next;	  
		
	  if ((event->button.state & (int) modifier->data) != (int) modifier->data)
	  {
	    mod_bol = FALSE;	    
	  }

	  /* start app if all modifiers and buttons match */
	  if ((modifier->next == NULL) &&
	      (mod_bol == TRUE) &&
	      (eventinfonode->scroll == 0) &&
	      (event->button.button == eventinfonode->button))
	  {
	    g_printerr ("mod\n");
	    start_app (eventinfonode->exec_app,
		       event->button.x,
		       event->button.y);
	  
	  }	
	} while (modifier->next != NULL);
      }
      else
      {	
	/* exec app if buttons match and no modifiers are given */
	if ((event->button.button == eventinfonode->button) &&
            (eventinfonode->scroll == 0) &&
	    ((event->button.state & GDK_ALT_MASK) != GDK_ALT_MASK) &&
	    ((event->button.state & GDK_ALTGR_MASK) != GDK_ALTGR_MASK) &&
	    ((event->button.state & GDK_SUPER_MASK) != GDK_SUPER_MASK) &&
	    ((event->button.state & GDK_SHIFT_MASK) != GDK_SHIFT_MASK) &&
	    ((event->button.state & GDK_CONTROL_MASK) != GDK_CONTROL_MASK))
	{
	  g_printerr ("no mod\n");
	  start_app (eventinfonode->exec_app,
		     event->button.x,
		     event->button.y);
	  
	}	
      }      
    } while (eventlist->next != NULL);
  }
  
  return;
}

/* start an application from the exec-string
 * int x - event x position
 * int y - event y position
 */
gint start_app (char *app, float x, float y)
{ 
  gint ret_val;
  gchar *argv[4];			 /* argument vector for execvp */
  gchar *const bin_sh = "/bin/sh";
  gchar *const parm_c = "-c";
  pid_t exec_pid;  
  size_t size_x, size_y;
  char *str_x;
  char *str_y; 
  int i;
  struct sigaction act;

  /*** Put this into main ? */
  /* Assign sig_chld as our SIGCHLD handler */
  act.sa_handler = sig_chld;

  /* We don't want to block any other signals in this example */
  sigemptyset(&act.sa_mask);

  /*
   * We're only interested in children that have terminated, not ones
   * which have been stopped (eg user pressing control-Z at terminal)
   */
  act.sa_flags = SA_NOCLDSTOP;

  /*
   * Make these values effective. If we were writing a real 
   * application, we would probably save the old value instead of 
   * passing NULL.
   */
  if (sigaction(SIGCHLD, &act, NULL) < 0) 
  {
    fprintf(stderr, "sigaction failed\n");
    return 1;
  }
  /*************/
      
  /* calculate space for x and y string coordinates */
  size_x = sizeof (char) * ((int) floor (log10 (x) + 1));
  if ((signed int) size_x < 1)
  {
      size_x = 1;
  }
  str_x = (char*) malloc (size_x);
  sprintf (str_x, "%.f", x);

  size_y = sizeof (char) * ((int) floor (log10 (y) + 1));
  if ((signed int) size_y < 1)
  {
      size_y = 1;
  }
  str_y = (char*) malloc (size_y);
  sprintf (str_y, "%.f", y);

  /* internal command list */
  replace (&app, IC_MOUSE_X, str_x);
  replace (&app, IC_MOUSE_Y, str_y);

  g_printerr ("app: %s\n", app);
  
  if (!(strcmp (app, IC_WORKSPACE_UP)))
  {
    switchWorkspace (UP, FALSE);    
    return 0;
  }

  if (!(strcmp (app, IC_WORKSPACE_DOWN)))
  {
    switchWorkspace (DOWN, FALSE);
    return 0;
  }
  
  argv[0] = bin_sh;
  argv[1] = parm_c;
  argv[2] = app;
  argv[3] = NULL;
  
  for (i=0;i<=3;i++)
  {
    g_printerr ("argv[%d]: %s\n", i, argv[i]);
  }

  exec_pid = fork ();
  if (exec_pid == 0)
  {
    /* execute the process with all args */
    ret_val = execvp (argv[0], argv);

    if (ret_val == -1)
    { 
      g_printerr ("couldn't start app\n");
      return 1;
    }    
  }
  else
  {
    //waitpid (exec_pid, NULL, WNOHANG);
    //wait (NULL);
    //printf ("wait\n");
  }
  
  return 0;
}

/*
 * The signal handler function -- only gets called when a SIGCHLD
 * is received, ie when a child terminates
 */
void sig_chld(int signo) 
{
    int status, child_val;

    /* Wait for any child without blocking */
    if (waitpid(-1, &status, WNOHANG) < 0) 
    {
        /*
         * calling standard I/O functions like fprintf() in a 
         * signal handler is not recommended, but probably OK 
         * in toy programs like this one.
         */
        fprintf(stderr, "waitpid failed\n");
        return;
    }

    /*
     * We now have the info in 'status' and can manipulate it using
     * the macros in wait.h.
     */
    if (WIFEXITED(status))                /* did child exit normally? */
    {
        child_val = WEXITSTATUS(status); /* get child's exit status */
        printf("child's exited normally with status %d\n", child_val);
    }
}

/* switches another workspace with help of libwnck */
void switchWorkspace (int direction, gboolean rotation)
{
#ifdef ENABLE_LIBWNCK
  
  WnckWorkspace *workspace;
  int workspace_number;
  int max_workspaces;
 
  workspace = wnck_screen_get_active_workspace (screen);
  workspace_number = wnck_workspace_get_number (workspace);
  max_workspaces = wnck_screen_get_workspace_count (screen);
    
  if (direction == UP)
  {
    /* switch to upper (left) workspace */
    if (workspace_number > 0)
      {
	workspace = wnck_screen_get_workspace (screen, workspace_number - 1);
	wnck_workspace_activate (workspace);      
      }
    else if (rotation == TRUE)		 /* move to last for cylce */
    {
      workspace = wnck_screen_get_workspace (screen, max_workspaces - 1);
      wnck_workspace_activate (workspace);
    }
  }
  else if (direction == DOWN)
  {
    /* switch to lower (right) workspace */
    if (workspace_number < max_workspaces - 1)
    {
      workspace = wnck_screen_get_workspace (screen, workspace_number + 1);
      wnck_workspace_activate (workspace);      
    }    
    else if (rotation == TRUE)		 /* move to first for cycle */
    {
      workspace = wnck_screen_get_workspace (screen, 0);
      wnck_workspace_activate (workspace);	
    }
  }
#endif
}
