static char * sccs_id="@(#)gfi.c	1.4 05/07/93";
/*
* gfi.c - get field info
*
* This program, passed a string, searches the database include files 
* the information assocatied with that field. Upon finding the entry,
* it extracts the important information into a structure.
*/

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <regex.h>

#define LINELEN 256

char *buf;
char *p;
regex_t r1; 
regex_t r2; 
regmatch_t pmatch1 [3];

void init (filename)
   char *filename;
{
   struct stat sbuf;
   int f;

   regcomp (&r1, 
       "^#.?define (.{6}_[A-Z0-9]+)[^ ]+ +([^ ]+)", REG_EXTENDED);
   regcomp (&r2, "^(.{6}_[A-Z0-9]+)", REG_EXTENDED);

   if ((f = open (filename, O_RDONLY)) == -1) {
       fprintf (stderr, "Unable to open file %s for reading.\n", filename);
       exit (0);
   }
       
   if (!fstat (f, &sbuf)) {
       buf = (char *) malloc (sbuf.st_size+1);
       if (read (f, buf, sbuf.st_size) > 0) {
           buf [sbuf.st_size] = '\0';
           p = buf;
           close (f); 
           return;
       }
   }

   fprintf (stderr, "File could not be read in.\n");
   close (f); 
   exit (0);
}


/*
* Read in a line of input from a file
*/
int readstr (str)
   char * str;
{
   while (*p != '\n') {
       if (*p == '\0')
           goto error;
       *(str++) = *(p++); 
   }

   *str = '\0';
   p++;
   return 0;
   
   error:
       exit (0);
}

/*
* Return true if the current line defines the field being
* searched for
*/
int is_match (fieldname, line)
   char *fieldname;
   char *line;
{
   char token [LINELEN];
   char *p;

   token [0] = '\0';
   if (!regexec (&r1, line, r1.re_nsub+1, pmatch1, 0)) {
       for (p = token; pmatch1 [1].rm_so < pmatch1 [1].rm_eo;
           p++, pmatch1 [1].rm_so ++)
           *p = line [pmatch1 [1].rm_so];
       *p = '\0';
   }

   return !strcmp (token, fieldname);
}

/*
* Return true if the line is a redeffed line. Redeffed lines
* will contain no commas.
*/
int is_redeffed (line)
   char *line;
{
   char *subexp2;

   subexp2 = &(line [pmatch1 [2].rm_so]);

   if (strchr (subexp2, ',') == (char *) NULL) 
       return 1;
   else
       return 0;
}

void get_redef_token (line, token)
   char *line;
   char *token;
{
   regmatch_t pmatch2 [6];
   char *p;
   char *subexp2;

   subexp2 = &(line [pmatch1 [2].rm_so]);
   if (!regexec (&r2, subexp2, r2.re_nsub+1, pmatch2, 0)) {
       for (p = token; pmatch2 [1].rm_so < pmatch2 [1].rm_eo;
           p++, pmatch2 [1].rm_so ++)
           *p = subexp2 [pmatch2 [1].rm_so];
       *p = '\0';
   }
   else {
       fprintf (stderr, "Regular expression failed.\n");
       exit (0);
   }
}

/*
* Given a field name, this function searches the input file
* for the record containing the field name. It assigns the
* field's arguments to entry.
*/
void filefind (fieldname)
   char * fieldname;
{
   char token [LINELEN];
   char line [LINELEN];

   while (readstr (line) != -1) {
       if (is_match (fieldname, line))
           goto match;
   }
   return;

   match:

       if (is_redeffed (line)) {
           get_redef_token (line, token);
           p = buf;
           filefind (token);
       }

       fprintf (stderr, "%s\n", line);
}


main (argc, argv)
   int argc;
   char **argv;
{
   int return_val;
   char *filename;
   char *fieldname;
  
   if (argc != 3)
       return 1;

   fieldname = argv [1];
   filename = argv [2];

   init (filename);
   filefind (fieldname); 
}
