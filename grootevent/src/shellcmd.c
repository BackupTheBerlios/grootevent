#include <shellcmd.h>

/* allocates memory for *str and hooks 'count' chars
 * from string 'append'
 */
void strnadd (char **str, char *append, unsigned int count)
{
  if (*str == NULL)			 /* if pointer of char == NULL -> alloc new memory */
  { 
    *str = (char *) malloc (sizeof (char)* (count + 1));
    **str = 0;
  }
  else					 /* realloc memory of *str */
  {
    *str = (char *) realloc (*str, sizeof (char)* (count + strlen (*str) + 1));
  }
  
  strncat (*str, append, count);	 /* cat 'count' chars from 'append' to *str */
}


/* replace needle in *str with replace (all occourences)
 */
void replace (char **str, char *needle, char *replace)  
{
  char *pstr = *str, *p2str=*str;
  char *newstr = NULL;
  unsigned int needlesize = strlen (needle), replacesize = strlen (replace);

  /* search for needle at the current possition in the string */
  while (1) {

    /* no more needles found */
    if ((p2str = strstr (pstr, needle)) == NULL) 
    { 
      if (*pstr)
      {
	strnadd (&newstr, pstr, strlen(pstr)); /* write rest of string in a new one */
      }
      
      break; /* end loop */
    }
    
    /* needle found */
    if (p2str > pstr)
    {
      strnadd (&newstr, pstr, p2str - pstr); /* write region between last and current
						string into the new string */
    }
    strnadd (&newstr, replace, replacesize); /* write 'replace' into the new string*/
    pstr = p2str += needlesize;		 /* move size of 'needle' in the string */
  }

  /* change 'str' to new string */
  *str = newstr;
}

int substitute (char **exec_string)
{
  char *exec_str_ptr = *exec_string;
  char *backquote_pos = exec_str_ptr;
  int i = 0;
  
  while (1)
  {
    backquote_pos = strchr (backquote_pos, BACKQUOTE);

    /* exit loop if no backquote found */
    if (backquote_pos == NULL)      
      break;
        
    printf ("b:%d\n", strlen (backquote_pos));

    /* exit loop at last position of string */
    if (strlen (backquote_pos) == 1)
      break;
    else
      backquote_pos += 1;
        
    i++;
  }

  /* return with error code */
  if (i == 0)
    return SUBSTITUTE_ERROR_NOT_FOUND; 
  else if (!(i % 2) == 0)
    return SUBSTITUTE_ERROR_UNCLOSED_BACKQUOTE;
    
  return SUBSTITUTE_OK;
}



