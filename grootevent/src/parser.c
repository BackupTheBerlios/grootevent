#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gdk/gdk.h>

#include "parser.h"

GList *eventlist;			 /* a double linked list to hold all events */
static int validate = 1;

/* parser for a single EventInfo node */
void parseEventInfo (xmlDocPtr doc, xmlNodePtr cur)
{
  xmlChar *key;
  xmlChar *attribute;
  cur = cur->xmlChildrenNode;
  struct eventinfo *eventinfonode;
  int key_counter = 0;  
  
  eventinfonode = (struct eventinfo*) g_malloc (
    sizeof (struct eventinfo));
  
  eventlist = g_list_append (eventlist, (struct eventinfo*) eventinfonode);
  eventinfonode->modifier = NULL;
  
  while (cur != NULL)
  {
    /* found a <key>-value */
    if ((!xmlStrcmp (cur->name, (const xmlChar *) "key")))
    {
      /* create only a list if there are modifiers */      
      if (key_counter == 0)
      {	
	eventinfonode->modifier = g_list_alloc ();
      }
      
      key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
      eventinfonode->modifier = g_list_append (eventinfonode->modifier,
					       (int*) key_char2int ( (char*) key));
      xmlFree (key);
      key_counter++;
    }   
    
    /* found a <exec>-value */
    if ((!xmlStrcmp (cur->name, (const xmlChar *) "exec")))
    {            
      key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
      eventinfonode->exec_app = strdup ((char*) key);

      /* DIRTY HACK: use better data structure */
      /*attribute = xmlGetProp(cur, "scroll");
      
      if (!(strcmp (g_ascii_strup (key, -1), "TRUE")))
	eventinfonode->exec_app_attribute = 1;
      else
      eventinfonode->exec_app_attribute = 2;*/
      
      xmlFree (key);
    }

    /* found a <button>-value */
    if ((!xmlStrcmp (cur->name, (const xmlChar *) "button")))
    {
      int key_val;
      
      key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
      key_val = atoi (key);

      if ((key_val >= 1) && (key_val <= 3))
      {
	eventinfonode->button = key_val;
      }
      else
      {
	eventinfonode->button = button_char2int (key);
      }
      xmlFree (key);
    }
    
    cur = cur->next;
  }
  return;
}

/* parser for the complete xml file */
GList *parseDoc (char *docname)
{
  xmlDocPtr doc;
  xmlNodePtr cur;
    
  xmlParserCtxtPtr ctxt;
  
  eventlist = g_list_alloc ();		 /* allocates memory for new list */

  if (getValidate() == TRUE)
  {
    ctxt = xmlCreateFileParserCtxt(docname);

    if (ctxt == NULL)
    {    
      exit (1);
    }
 
    ctxt->validate = 1;			 /* check the XML's DTD */
    xmlParseDocument(ctxt);

    if (!ctxt->valid)
    {
      g_print ("Please correct this problem or grootevent isn't able to run.\n"
	       "Hint: You could also disable validating (--help for more infos)\n");
      exit (1);
    }
  } 

  doc = xmlParseFile (docname);
  
  if (doc == NULL)
  {
    fprintf (stderr, "Document not parsed successfully. \n");
    return NULL;
  }

  cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    fprintf (stderr, "empty document\n");
    xmlFreeDoc (doc);
    return NULL;
  }

  if (xmlStrcmp (cur->name, (const xmlChar *) "grootevent"))
  {
    fprintf (stderr, "document of the wrong type, root node != grootevent\n");
    xmlFreeDoc (doc);
    return NULL;
  }

  cur = cur->xmlChildrenNode;
  while (cur != NULL)
  {
    if ((!xmlStrcmp (cur->name, (const xmlChar *) "eventinfo")))
    {
      parseEventInfo (doc, cur);
    }

    cur = cur->next;
  }

  xmlFreeDoc (doc);
  return eventlist;
}

/* get parsed key chars and returns a GDK modifier mask */
int key_char2int (char *key)
{
  int mask;
  
  if (!(strcmp (g_ascii_strup (key, -1), ALT_MASK_STRING)))
  {
    mask = GDK_ALT_MASK;
    return mask;
  }
  
  if (!(strcmp (g_ascii_strup (key, -1), ALTGR_MASK_STRING)))
  {
    mask = GDK_ALTGR_MASK;
    return mask;
  }

  if (!(strcmp (g_ascii_strup (key, -1), SHIFT_MASK_STRING)))
  {
    mask = GDK_SHIFT_MASK;
    return mask;
  }

  if (!(strcmp (g_ascii_strup (key, -1), CONTROL_MASK_STRING)))
  {
    mask = GDK_CONTROL_MASK;
    return mask;
  }

  if (!(strcmp (g_ascii_strup (key, -1), SUPER_MASK_STRING)))
  {
    mask = GDK_SUPER_MASK;
    return mask;
  }
  
  return 0;
}

/* set if the XML file should be validated against a DTD */
void setValidate (gboolean val)
{
  validate = val;
}

/* get if the XML file should be validated against a DTD */
gboolean getValidate (void)
{
  return validate;
}

/* get parsed key chars and returns a GdkScrollDirection */
int button_char2int (char* key)
{
  if (!(strcmp (g_ascii_strup (key, -1), SCROLL_UP_STRING)))
  {    
    return GDK_SCROLL_UP;
  }

  if (!(strcmp (g_ascii_strup (key, -1), SCROLL_DOWN_STRING)))
  {    
    return GDK_SCROLL_DOWN;
  }

  if (!(strcmp (g_ascii_strup (key, -1), SCROLL_LEFT_STRING)))
  {    
    return GDK_SCROLL_LEFT;
  }

  if (!(strcmp (g_ascii_strup (key, -1), SCROLL_RIGHT_STRING)))
  {    
    return GDK_SCROLL_RIGHT;
  }

  return 0;
}
