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

/*
  Das Verfahren ist Unix-Standard: erzeuge ein Kindprozeß mit fork() und
  starte das andere Programm (gnuplot, xmgr), erzeuge 1 oder 2 Pipes mit
  pipe(), weise stdout und stdin den pipes zu mit dup(), und Dein
  Prog kann dann über die Pipe(s) an das andere Prog schreiben und auch
  die Ausgabe lesen. Eine Routine dazu sieht etwa so aus:
*/

#ifdef NO
int start_child(char *cmd, FILE **readpipe, FILE **writepipe) {
  int childpid, pipe1[2], pipe2[2];

  if ((pipe(pipe1) < 0) || (pipe(pipe2) < 0)) {
    perror("pipe"); exit(-1);
  }

  if ((childpid = fork()) < 0) {
    perror("fork"); exit (-1);
  }
  else
    if (childpid > 0) {  /* parent */
      close(pipe1[0]); close(pipe2[1]);
      /* write to child is pipe1[1], read from child is pipe2[0] */
      *readpipe  = fdopen(pipe2[0], "r");
      *writepipe = fdopen(pipe1[1], "w");
      return childpid;
    }
    else {  /* child */
      close(pipe1[1]); close(pipe2[0]);
      /* read from parent is pipe1[0], write to parent is pipe2[1] */
      dup2(pipe1[0], 0);
      dup2(pipe2[1], 1);
      close(pipe1[0]); close(pipe2[1]);

      if (execlp(cmd,cmd,NULL) < 0) perror("execlp");

      /* never returns */
    }
}
#endif

/*
  Zu Beginn Deines Progs startes Du z.B. gnuplot einmal! mit:
  
  FILE *readfrom, *writeto;
  start_child("gnuplot", &readfrom, &writeto);
  
  Später, da wo bei Dir der system()-Aufruf erfolgt, schickst Du Deine
  Befehle direkt an gnuplot mit fprintf(writeto, ...) + fflush(writeto),
  z.B.
  
  fprintf(writeto, "set logscale x\n"); fflush(writeto);

  Die Ausgabemeldungen von gnuplot kannst Du ebenso einfach z.B. per
  fgets(..., readfrom) einlesen. Wenn das nicht gewünscht ist, kann
  alles mit readfrom und pipe2 im obigen Kode weggelassen werden.
*/


