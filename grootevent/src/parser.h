#if !defined parser_h
#define parser_h

#include <glib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>

/* Modifier Mask */
#define ALT_MASK_STRING "ALT"
#define ALTGR_MASK_STRING "ALTGR"
#define SHIFT_MASK_STRING "SHIFT"
#define CONTROL_MASK_STRING "CONTROL"
#define SUPER_MASK_STRING "SUPER"

/* create some usefull aliases
 * I supposed some keycodes here. If you've defined some others
 * in your X configuration, please change them here or
 * use other modifiers.
 */
#define GDK_ALT_MASK GDK_MOD1_MASK
#define GDK_SUPER_MASK GDK_MOD4_MASK
#define GDK_ALTGR_MASK GDK_MOD5_MASK

/* internal command list */
#define IC_MOUSE_X "__mouse_x__"
#define IC_MOUSE_Y "__mouse_y__"
#define IC_WORKSPACE_UP "__workspace_up__"
#define IC_WORKSPACE_DOWN "__workspace_down__"

/* mouse wheel buttons */
#define SCROLL_UP_STRING "SCROLL_UP"
#define SCROLL_DOWN_STRING "SCROLL_DOWN"
#define SCROLL_LEFT_STRING "SCROLL_LEFT"
#define SCROLL_RIGHT_STRING "SCROLL_RIGHT"

void parseEventInfo (xmlDocPtr doc, xmlNodePtr cur);
GList *parseDoc (char *docname);
int key_char2int (char *key);
void setValidate (int validate);
int getValidate (void);
int button_char2int (char* button);

struct eventinfo
{
  GList *modifier;			 /* a modifier key */
  gint button;				 /* a mouse button or wheel */
  gint scroll;				 /* a mouse button or wheel */
  gchar *exec_app;			 /* a external app to exec or a internal command */
  gint exec_app_attribute;		 /* save attributes */
};

#endif
