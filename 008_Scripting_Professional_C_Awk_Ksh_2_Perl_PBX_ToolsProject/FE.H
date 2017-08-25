

#define	LEFT		0x80
#define	RIGHT		0x81
#define	UP		0x82
#define	DOWN		0x83
#define	FK1		0xa0
#define	FK2		0xa1
#define	FK3		0xa2
#define	FK4		0xa3
#define	FK5		0xa4
#define	FK6		0xa5
#define	FK7		0xa6
#define	FK8		0xa7
#define	FK9		0xa8
#define	FK10		0xa9
#define	FK11		0xaa
#define	FK12		0xab
#define	FK13		0xac
#define	FK14		0xad
#define	FK15		0xae
#define	FK16		0xaf
#define	ESC		0x1b
#define DEL		0x7f
#define	TAB		'\t'
#define	ENTER		'\n'
#define BS		'\b'
#define BELL		0x07
#define REFRESH		0x05

/* flags item bit combination */

#define FE_READ		0x00000001	/* field is Read (bit 0 = 1) or Write only (bit 0 = 0) */
#define FE_MY_FIELD	0x00000010	/* field structure allocated by fe */
#define FE_MY_STR	0x00000020	/* string buffer allocated by fe */
#define FE_0_BASED	0x00000100	/* col and row information is 0 based */
#define FE_CURSPOS	0x00000006	/* mask to get cursor position information */
#define FE_RIGHT	0x00000004	/* by default position cursor to the end of text in the field */
#define FE_LEFT	0x00000006	/* by default position cursor to the beginning of text in the field */

typedef struct	fe {
	int		screen;
	int		ID;
	int		flags;
	int		rank;	/* position of the first char. */
	int		row;
	int		col;
	int		length;
	struct fe	*next;
	struct fe	*prev;
	char		filler;
	char  		*string;
	} FIELD;

/* prototype of functions returning something else than "int" */

extern FIELD	*feCreate ();
extern FIELD	*feSeek ();

