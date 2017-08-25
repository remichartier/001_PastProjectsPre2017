/*char banner[]	= "field editor v1.0          \n";


#include	<ioctl.h>
#include	<stdio.h>
#include	<string.h>
#include	<signal.h>
#include	<time.h>
#include	<wait.h>
#include	<errno.h>
#include	"fe.h"

extern int errno;

#define MAXSCREENS	16
#define MAXSEQUENCES	32
#define	MAXSEQLEN	20


int	inpos, colcur;
int	feInsMode = 1, feLine24 = 0;
char	errbuf[80];
char	*terminal, termcapbuf[2024], termbuf[256], *termptr;
char	*cm, *cd, *ho, *dc, *ktemp;
FIELD	*fecur;		/* field in which the cursor is currently */
FIELD	*screentable[MAXSCREENS];
int	screencur;	/* current screen displayed */
struct	sgttyb	my_sgtty;
struct	ltchars	my_tchars;
int	sav_flags;
char	sav_t_lnextc;

char	Nullstr[1] = {'\0'};
int	looktable[MAXSEQUENCES];
struct	seqs {
	int	code;
	int	len;
	char	*c;
	} seq[MAXSEQUENCES];

/*
/*	outc : print a single character
/*		This function is required to satisfy the tputs parameters
/*		Putchar could not be used directly because it is a macro.
*/
outc(cc)
char	cc;
{
	putchar(cc);
}

/*
/*	getCaract : get a single character or interpret a sequence for
/*			function keys and cursor movement.
*/
getCaract()
{
	int	rc, count, numberToLook, index, i;
	char	c[MAXSEQLEN];

	for (i = 0; i < MAXSEQUENCES; i++) looktable[i] = i;
	numberToLook = MAXSEQUENCES;

	for (index = 0; index < MAXSEQLEN; index++) {
		rc = read(0, &c[index], 1); /* read one char at a time */
		if (c[0] >= ' ') return (c[0]); /* it is a printable char */

		/* It is a sequence */
		count = 0;
		for (i = 0; i < numberToLook; i++) {
			if (c[index] == seq[looktable[i]].c[index]) {
				/* seq indentical so far */
				if (index == seq[looktable[i]].len-1) /* seq identical & complete */
					return (seq[looktable[i]].code);
				looktable[count] = looktable[i]; /* store this index for the next round */
				count++;
			}
		}
		if (count == 0) {
			if (index) return(BELL); /* no sequence remaining: illegal key combination */
			else return(c[0]); /* not a sequence start */
		}
		numberToLook = count;
	}
}

/*
/*	actualLength : calculates the actual length of a string in a field
/*
*/
actualLength(fep)
FIELD	*fep;
{
	int	i;
	
	for (i=0; i<fep->length; i++)
		if (fep->string[i] == fep->filler) break;
	return(i);
}


/*
/*	feInit : initialize the field editor
/*
*/
feInit (mode)
int	mode;
{
	int	i;
	char	keyname[3];

	feInsMode = mode;

	fecur = NULL;
	for (i=0; i<MAXSCREENS; screentable[i++] = NULL);
	screencur = -1; /* no screen displayed currently */
	
	/* create the termcap strings cm, cd, ho, etc... */
	
	terminal = (char *) getenv("TERM");
	if (terminal[0] == '/') terminal = "vt100";
	tgetent(termcapbuf, terminal);
	
	termptr = termbuf;
	cm = (char *) tgetstr("cm", &termptr); /* absolute cursor positionning */
	cd = (char *) tgetstr("cd", &termptr); /* clear screen from cursor */
	ho = (char *) tgetstr("ho", &termptr); /* home */

	for (i=0; i < MAXSEQUENCES; i++) {
		seq[i].code = BELL;
		seq[i].len  = 0;
		seq[i].c = Nullstr;
	}
	dc = (char *) tgetstr("dc", &termptr); /* delete char */
	seq[0].code = DEL;
	seq[0].len = strlen(dc);
	seq[0].c = dc;
	ktemp = (char *) tgetstr("kl", &termptr); /* left arrow key */
	seq[1].code = LEFT;
	seq[1].len = strlen(ktemp);
	seq[1].c = ktemp;
	ktemp = (char *) tgetstr("kr", &termptr); /* right arrow key */
	seq[2].code = RIGHT;
	seq[2].len = strlen(ktemp);
	seq[2].c = ktemp;
	ktemp = (char *) tgetstr("ku", &termptr); /* up arrow key */
	seq[3].code = UP;
	seq[3].len = strlen(ktemp);
	seq[3].c = ktemp;
	ktemp = (char *) tgetstr("kd", &termptr); /* down arrow key */
	seq[4].code = DOWN;
	seq[4].len = strlen(ktemp);
	seq[4].c = ktemp;
	keyname[0] = 'k';     		/* function keys */
	keyname[2] = '\0';
	for (i=0; i<16; i++) {
		if (i<9) keyname[1] = 0x31 + i;
		else {
			keyname[0] = 'l';
			keyname[1] = 0x27 + i;
		}
		ktemp = (char *) tgetstr(keyname, &termptr);
		seq[5+i].code = FK1+i;
		seq[5+i].len = strlen(ktemp);
		seq[5+i].c = ktemp;
	}

	/* initialize IOCTL in CBREAK mode */

	ioctl(0, TIOCGETP, &my_sgtty);
	sav_flags = my_sgtty.sg_flags;
	my_sgtty.sg_flags &= ALLDELAY; /* keep only delay flags */
	my_sgtty.sg_flags |= CBREAK | CRMOD;
	ioctl(0, TIOCSETP, &my_sgtty);

	ioctl(0, TIOCGLTC, &my_tchars);
	sav_t_lnextc = my_tchars.t_lnextc;
	my_tchars.t_lnextc = -1; /* disable special use of CTRL-V for TVI935 */
	ioctl(0, TIOCSLTC, &my_tchars);
	ioctl(0, TIOCFLUSH);

	/* clear screen */

	tputs(ho, 1, &outc);
	tputs(cd, 1, &outc);

	inpos = 0;
	errbuf[79] = '\0'; /* make sure errbuf will always be NULL terminated */
}

/*
/*	feSuspend : restore the CBREAK mode
/*
*/
feSuspend ()
{
	/* the structure "my_sgtty" was filled by feInit */
	/* restore terminal to original input mode */
	my_sgtty.sg_flags = sav_flags;
	ioctl(0, TIOCSETP, &my_sgtty);

	/* Note: the special use of CTRL-V is not restored so that the down arrow
			can be used on a tvi935. */
}

/*
/*	feResume : restore the CBREAK mode
/*
*/
feResume ()
{
	/* the structures "my_sgtty" and "my_chars" were filled by feInit */
	my_sgtty.sg_flags &= ALLDELAY; /* keep only delay flags */
	my_sgtty.sg_flags |= CBREAK | CRMOD;
	ioctl(0, TIOCSETP, &my_sgtty);

	my_tchars.t_lnextc = -1; /* disable special use of CTRL-V for TVI935 */
	ioctl(0, TIOCSLTC, &my_tchars);
	ioctl(0, TIOCFLUSH);
}

/*
/*	feExit : free all allocated fields
/*
*/
feExit ()
{
	int	i;

	screencur = -1;		/* make sure there is no current screen */
	for (i=0; i<MAXSCREENS; i++) feDeleteScreen(i);

	/* set the cursor to the bottom of the screen */
	/* could not go directly to line 24 because   */
	/* some terminals use it as a status line.    */

	tputs((char *) tgoto(cm, 0, 23), 1, &outc);
	outc('\n');

	/* the structures "my_sgtty" and "my_chars" were filled by feInit */
	/* restore terminal to original input mode */
	my_sgtty.sg_flags = sav_flags;
	my_tchars.t_lnextc = sav_t_lnextc;
	ioctl(0, TIOCSETP, &my_sgtty);
	ioctl(0, TIOCSLTC, &my_tchars);

}

/*
/*	feAdd : add a new field and place it in the list
/*
*/
feAdd (fep)
FIELD	*fep;
{
	int	i, start;
	char	*str;
	FIELD	*fei;

	/* check validity of position of the field, and screen */

	if ((fep->row > 24) || (fep->col > 80)) return (-1);
	if (fep->screen > MAXSCREENS) return(-1);

	fep->flags &= ~FE_MY_FIELD; /* I did not allocate the memory for this field */
	if (fep->length == 0) fep->length = strlen(fep->string);
	if (fep->flags & FE_0_BASED == 0) {
		fep->row = fep->row-1;	/* coordinates is 0 based */
		fep->col = fep->col-1;	/* ditto */
		fep->flags |= FE_0_BASED;
	}
	fep->rank = ((fep->row) * 80) + fep->col;

   	/* insert new field in the proper place in the field list */
	if (screentable[fep->screen]) {
		fei = screentable[fep->screen];
		while (fei->next) {
			if (fei->next->rank < fep->rank) fei = fei->next;
			else {
				if (fei->next->rank == fep->rank)
			      		{    /* cannot have 2 fields at the same location */
	     				feDelete(fei->next);
					}
				break;
				}
			}
	       	fep->next = fei->next;
		fei->next = fep;
	   	fep->prev = fei;
		if (fep->next)	fep->next->prev = fep;
	       	}
	else {		/* first and only field to exist for this screen */
		fep->next = NULL;
		fep->prev = NULL;
		screentable[fep->screen] = fep;
		}

	/* allocate a buffer for the actual string if required */

	if (fep->string) fep->flags &= ~FE_MY_STR;
	else {
		fep->string = (char *) malloc(fep->length+1);
		fep->flags |= FE_MY_STR;
		for (i=0; i<fep->length; i++) fep->string[i] = fep->filler;
		fep->string[fep->length] = '\0';	/* build a null terminated filler string */
	}

	feWrite(fep, NULL);

	return(0);
}

/*
/*	feCreate : create a new field and place it in the list
/*
*/
FIELD *feCreate (scr, id, row, col, len, fil, flags, str)
int	scr, id, row, col, len, flags;
char	fil, *str;
{
	int	i;
	FIELD	*fep, *fei;

	/* check validity of position of the field, and screen */

	if ((row > 24) || (col > 80)) return (NULL);
	if (scr > MAXSCREENS) return(NULL);

	/* allocate new structure of type FIELD */

	fep = (FIELD * ) malloc(sizeof(FIELD));
	fep->screen = scr;
	fep->ID = id;
	fep->flags = flags | FE_MY_FIELD; /* I allocated the memory for this field */
	fep->filler = fil;
	if (len) fep->length = len;	/* length = declared lenght... */
	else fep->length = strlen(str); /* ... or length of string.    */
	fep->row = row-1;	/* coordinates is 0 based */
	fep->col = col-1;	/* ditto */
	fep->flags |= FE_0_BASED;
	fep->rank = ((row-1) * 80) + col - 1;

   	/* insert new field in the proper place in the field list */
	if (screentable[scr]) {
		fei = screentable[scr];
		while (fei->next) {
			if (fei->next->rank < fep->rank) fei = fei->next;
			else {
				if (fei->next->rank == fep->rank)
			      		{    /* cannot have 2 fields at the same location */
	     				feDelete(fei->next);
					}
				break;
				}
			}
	       	fep->next = fei->next;
		fei->next = fep;
	   	fep->prev = fei;
		if (fep->next)	fep->next->prev = fep;
	       	}
	else {		/* first and only field to exist for this screen */
		fep->next = NULL;
		fep->prev = NULL;
		screentable[scr] = fep;
		}

	/* allocate a buffer for the actual string */

	fep->string = (char *) malloc(fep->length+1);
	fep->flags |= FE_MY_STR;
	for (i=0; i<fep->length; i++) fep->string[i] = fep->filler;
	fep->string[fep->length] = '\0';	/* build a null terminated filler string */
	if (str) {
		if (fep->length <= strlen(str))
			strncpy (fep->string, str, fep->length);
		else   	strncpy (fep->string, str, strlen(str));
	}

	feWrite(fep, NULL);

	return(fep);
}

/*
/*	feDelete : unchain and delete a field
/*
*/
feDelete (fep)
FIELD	*fep;
{
	int	i;

	/* make sure that global pointers do not point to a deleted structure */
	if (fep == screentable[fep->screen]) screentable[fep->screen] = NULL;
	if (fep == fecur) fecur = screentable[fep->screen];

	/* unchain the field */
	if (fep->prev) fep->prev->next = fep->next;
	if (fep->next) fep->next->prev = fep->prev;

	/* erase the field if it is displayed */
	if (fep->screen == screencur) {
		tputs((char *) tgoto(cm, fep->col, fep->row), 1, &outc);
		for (i=0; i < fep->length; i++) outc(' ');
		fflush(stdout);
	}

	if (fep->flags & FE_MY_STR) {
		free (fep->string);
		fep->string = NULL;
	}
	if (fep->flags & FE_MY_FIELD) free (fep);
	return(0);
}

/*
/*	feWrite : write a string in a field
/*
*/
feWrite (fep, str)
FIELD	*fep;
char	*str;
{
	int	i;

	if (str) {    /* replace with new string. Note : same size ! */
       		for (i = 0; i < fep->length; i++) fep->string[i] = fep->filler;
		if (fep->length <= strlen(str))
			strncpy (fep->string, str, fep->length);
		else	strncpy (fep->string, str, strlen(str));
	}

	if (fep->screen != screencur) return(-1); /* do not display */
	tputs((char *) tgoto(cm, fep->col, fep->row), 1, &outc);
	printf("%s", fep->string);
	fflush(stdout);
	return(0);
}

/*
/*	feGetCode : returns a editing code.
/*
*/
int	feGetCode ()
{
	int	i;
	int	key;	/* key pressed or sequence code */


	while (1) {
		key = getCaract(); /* get one char or movement code */

		switch (key) {
		case REFRESH:
			feRefresh();
			break;
		case BELL:
			outc(BELL);
			break;
		default:
			/* return the char or code */
			return(key);
			break;
		}
	fflush(stdout);
	}
}

/*
/*	feRead : editing a field. Returns the field's string in a buffer
/*		provided by the caller, and the "closing" character as
/*		return value.
/*
*/
int	feRead (fep, returnstr, where)
FIELD	*fep;
char	*returnstr;
int	where;
{
	int	i;
	char	inbuf[512];
	int	key;	/* key pressed or sequence code */

	if (fep == NULL) return (feGetCode());
	if ((fep->flags & FE_READ) == 0) return (-1);
	if (fep->screen != screencur) return(-1);

	fecur = fep; /* make this field current */

	strcpy(inbuf, fep->string);
	if (where == 0) where = fep->flags & FE_CURSPOS;
	if (fep->length == 1) where = FE_LEFT; /* for a single char field */

	switch(where) {
	case FE_RIGHT: 	/* position the cursor to the end of field */
		inpos = actualLength(fep);
		break;
	case FE_LEFT:	/* position cursor to beginning of field */
		inpos = 0;
		break;
	default:		/* position cursor on same column if possible */
		if ((colcur > fep->col) && (colcur <= fep->col+actualLength(fep)))
			inpos = colcur - fep->col;
		else {	/* position cursor at beginning of text if overwrite mode */
					/*	and end of text if insert mode. */
			if (feInsMode) inpos = actualLength(fep);
			else inpos = 0;
		}
		break;
	}

	tputs(tgoto(cm, fep->col+inpos, fep->row), 1, &outc);
	fflush(stdout);

	while (1) {
		key = getCaract(); /* get one char or movement code */

		switch (key) {
		case FK1:	case FK2:	case FK3:	case FK4:
		case FK5:	case FK6:	case FK7:	case FK8:
		case FK9:	case FK10:	case FK11:	case FK12:
		case FK13:	case FK14:	case FK15:	case FK16:
		case UP:
		case DOWN:
		case ENTER:
		case TAB:
			/* close the field and return the closing char */
			colcur = fep->col + inpos;
			strncpy(fep->string, inbuf, fep->length);
       			strcpy (returnstr, fep->string);
			for (i=0; (i < fep->length) && (returnstr[i] != fep->filler); i++);
			returnstr[i] = '\0'; /* eliminates the filler portion of the string */
			return(key);
			break;
		case BS:
			if (inpos) {
				inpos--;
		       		outc(BS);
				if (feInsMode) {
					for (i=inpos; i<fep->length; i++) {
						inbuf[i] = inbuf[i+1];
					}
					inbuf[fep->length-1] = fep->filler;
					printf("%s",&inbuf[inpos]);
					fflush(stdout);
			     		tputs((char *) tgoto(cm, fep->col+inpos, fep->row), 1, &outc);
				}
				else {	/* overwrite mode */
					inbuf[inpos] = fep->string[inpos];
					outc(inbuf[inpos]);
					outc(BS);
				}
			}
			else outc(BELL); /* beep */
			break;
		case DEL:		/* DEL */
			for (i=inpos; i<fep->length; i++) {
				inbuf[i] = inbuf[i+1];
			}
			inbuf[fep->length-1] = fep->filler;
			printf("%s",&inbuf[inpos]);
			fflush(stdout);
	     		tputs((char *) tgoto(cm, fep->col+inpos, fep->row), 1, &outc);
	     		break;
		case ESC:		/* ESC */
			/* shlould start a sequence */
			break;
		case REFRESH:
			feRefresh();
	     		tputs((char *) tgoto(cm, fep->col+inpos, fep->row), 1, &outc);
			break;
		case BELL:
			outc(BELL);
			break;
		case RIGHT:
			if ((fep->length > 1) && (inpos < fep->length) && (inbuf[inpos] != fep->filler)) {
				inpos++;
		     		tputs((char *) tgoto(cm, fep->col+inpos, fep->row), 1, &outc);
			}
			else outc(BELL);
			break;
		case LEFT:
			if (inpos) {
				inpos--;
		     		outc(BS);
			}
			else outc(BELL);
			break;
		default :
			if ((key < ' ') || (key > DEL)) break; /* process only printable keys here */
			if (key == fep->filler) {
				outc(BELL);
				break;
			}
			if (inpos < fep->length) {
				if (feInsMode)
			      		for (i=fep->length-1; i>inpos; i--) {
						inbuf[i] = inbuf[i-1];
				       		}
			    	inbuf[inpos] = key;
				printf("%s",&inbuf[inpos]);
				fflush(stdout);
				if (fep->length > 1) inpos++;
		     		tputs((char *) tgoto(cm, fep->col+inpos, fep->row), 1, &outc);
			}
			else outc(BELL); /* beep */
		}
	fflush(stdout);
	}
}

/*
/*	feSeek : go to an input field (next, previous or position)
/*
*/
FIELD	*feSeek (pos)
int	pos;
{
	FIELD	*fei;

	switch (pos) {
		case 0:			/* go to first readable field */
			fei = screentable[screencur];
       			while (fei) {
				if (fei->flags & FE_READ) break;
				fei = fei->next;
				}
			break;

		case 1:			/* go to next readable field */
			fei = fecur;
       			while (fei->next) {
				fei = fei->next;
				if (fei->flags & FE_READ) break;
		  	}
			if ((fei->flags & FE_READ) == 0) {
				fei = fecur; /* did not find an input field */
				outc(BELL);
				fflush(stdout);
			}
			break;

		case 2: 		/* go to next readable field and wrap */
			if (fecur->next) fei = fecur;
				/* if on the last field, wrap to the first field */
			else fei = screentable[screencur];
       			while (fei->next) {
				fei = fei->next;
				if (fei->flags & FE_READ) break;
			}
			if ((fei->flags & FE_READ) == 0) { /* restart from beginning of screen */
				fei = screentable[screencur];
	       			while (fei) {
					if (fei->flags & FE_READ) break;
					fei = fei->next;
       				}
			}
			break;

		case 3:		  	/* go to prev readable field */
			fei = fecur;
       			while (fei->prev) {
				fei = fei->prev;
				if (fei->flags & FE_READ) break;
		  	}
			if ((fei->flags & FE_READ) == 0) {
				fei = fecur; /* did not find an input field */
				outc(BELL);
				fflush(stdout);
			}
			break;

		}
	return(fei);
}

/*
/*	feDeleteScreen : delete a screen with all the related fields
/*
*/
feDeleteScreen(screen)
int	screen;
{
	FIELD	*fep, *fetemp;

	if (screen == screencur) {
		/* erase the screen */
		tputs(ho, 1, &outc);
		tputs(cd, 1, &outc);

		screencur = -1;
		}

	fep = screentable[screen];
	while (fep) {
		fetemp = fep;
		fep = fep->next;
		if (fetemp->flags & FE_MY_STR) {
			free (fetemp->string);
			fetemp->string = NULL;
		}   
		if (fetemp->flags & FE_MY_FIELD) free (fetemp);
	}
	screentable[screen] = NULL;
	return (0);
}

/*
/*	feSwitchScreen : switch to another screen
/*
*/
feSwitchScreen (screen)
int	screen;
{
	FIELD	*fep;

	screencur = screen;
	fecur = screentable[screen];
		
	/* erase the screen */

	tputs(ho, 1, &outc);
	tputs(cd, 1, &outc);

	/* display all the fields of that screen */

	fep = fecur;
	while (fep) {
		feWrite(fep, NULL);
		fep = fep->next;
	}
}

/*
/*	feAnalysis : Display the field list for a screen
/*		screen : screen number to analyse
/*		flags  : future use
/*
*/
feAnalysis (screen, flags)
int	screen, flags;
{
	FIELD	*fep;
	char	ccc;

	/* erase the screen */
	tputs(ho, 1, &outc);
	tputs(cd, 1, &outc);

	printf("type  address   next   string\n");
	fep = screentable[screen];
	while (fep) {
		printf("%3x %8x %8x %-50.50s\n", fep->flags, fep, fep->next, fep->string);
		fflush(stdout);
		fep = fep->next;
	}
	read(0, &ccc, 1);
	return (0);
}

/*
/*	feInsertMode : Set insert mode to insert (not 0) or overwrite (0)
/*
*/
feInsertMode (newMode)
int	newMode;
{
	feInsMode = newMode;
}

/*
/*	feRefresh : Redisplay the entire screen.
/*
*/
feRefresh()
{
	feSwitchScreen(screencur);
	if (feLine24) feInfo(errbuf); /* redisplay also the error line */
}

/*
/*	feClearScreen : Clear the screen.
/*
*/
feClearScreen ()
{
	tputs(ho, 1, &outc);
	tputs(cd, 1, &outc);
	fflush(stdout);
}

/*
/*	feError : print an error message on line 24. If message is
/*		NULL, erase line 24.
/*
*/
feError (message)
char	*message;
{
	int	i;

 	tputs((char *) tgoto(cm, 0, 23), 1, &outc);
	if (feLine24) {
		if (errbuf == message) {
			printf("%c%s", BELL, errbuf);
			feLine24 = 1;
		}
		else {
			tputs(cd, 1, &outc);
			feLine24 = 0;
		}
	}
	else
		if (message) {
			strncpy(errbuf, message, 79);
			printf("%c%s", BELL, errbuf);
			feLine24 = 1;
	}
	fflush(stdout);
	return(feLine24);
}

/*
/*	feInfo : print an information message on line 24. If message is
/*		NULL, erase line 24.
/*
*/
feInfo (message)
char	*message;
{
	int	i;

 	tputs((char *) tgoto(cm, 0, 23), 1, &outc);
	if (feLine24) {
		if (errbuf == message) {
			printf("%s", errbuf);
			feLine24 = 1;
		}
		else {
			tputs(cd, 1, &outc);
			feLine24 = 0;
		}
	}
	else
		if (message) {
			strncpy(errbuf, message, 79);
			printf("%s", errbuf);
			feLine24 = 1;
	}
	fflush(stdout);
	return(feLine24);
}

/*
/*	fePause : wait for the user type any key to continue. The key is discarded
/*
*/
fePause()
{
	fflush(stdout); /* just in case something was queud up */
	getCaract(); /* will absorb a key or sequence */
	return;
}

/*
/*	feGoto : set the cursor to an absolute screen position.
/*
*/
feGoto(row, col)
int	row, col;
{
 	tputs((char *) tgoto(cm, col, row), 1, &outc);
	fflush(stdout);
	return;
}


