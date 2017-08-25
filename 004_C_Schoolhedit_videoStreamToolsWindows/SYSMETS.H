/*--------------------------------------------------------------------------
SYSMETS.H -- Structure pour afficher la taille par defaut des objets Windows
----------------------------------------------------------------------------*/


#if defined (SYSMETS_C) || defined (SYSMETS_RC)
#include <windows.h>
#endif  /* (SYSMETS_C) || defined (SYSMETS_RC) */

#if defined (SYSMETS_C)
#include <stdlib.h>   // pour les constantes _MAX_PATH, _MAX_FNAME, _MAX_EXT
#include <commdlg.h>
#include <windowsx.h>	// pour les fonctions GlobalAllocPtr et GlobalFreePtr
#include <stdio.h>
#include <string.h>


#define NUMLINES ((int) (sizeof sysmetrics / sizeof sysmetrics [0]))
#endif /* SYSMETS_C */



#if defined (SYSMETS_C) || defined (SYSMETS_RC)
#define IDM_OPEN					 10
#define IDM_QUITTER				 20
#define IDM_DECODAGE			 30
#endif /* (SYSMETS_C) || defined (SYSMETS_RC) */


#if defined (SYSMETS_C)
struct
{
int  nIndex ;
char *szLabel ;
char *szDesc ;
}
sysmetrics [] =
{
SM_CXSCREEN,          "SM_CXSCREEN",          "Largeur ecran en pixels",
SM_CYSCREEN,          "SM_CYSCREEN",          "Hauteur ecran en pixels",
SM_CXVSCROLL,         "SM_CXVSCROLL",         "Largeur fleche defilement V.",
SM_CYHSCROLL,         "SM_CYHSCROLL",         "Hauteur fleche defilement H.",
SM_CYCAPTION,         "SM_CYCAPTION",         "Hauteur barre de titre",
SM_CXBORDER,          "SM_CXBORDER",          "Largeur bordure fenetre",
SM_CYBORDER,          "SM_CYBORDER",          "Hauteur bordure fenetre",
SM_CXDLGFRAME,        "SM_CXDLGFRAME",        "Largeur cadre fenetre dialogue",
SM_CYDLGFRAME,        "SM_CYDLGFRAME",        "Hauteur cadre fenetre dialogue",
SM_CYVTHUMB,          "SM_CYVTHUMB",          "Hauteur curseur defilement V.",
SM_CXHTHUMB,          "SM_CXHTHUMB",          "Largeur curseur defilement H.",
SM_CXICON,            "SM_CXICON",            "Largeur icone",
SM_CYICON,            "SM_CYICON",            "Hauteur icone",
SM_CXCURSOR,          "SM_CXCURSOR",          "Largeur curseur",
SM_CYCURSOR,          "SM_CYCURSOR",          "Hauteur curseur",
SM_CYMENU,            "SM_CYMENU",            "Hauteur barre de menu",
SM_CXFULLSCREEN,      "SM_CXFULLSCREEN",      "Largeur zone client plein ecran",
SM_CYFULLSCREEN,      "SM_CYFULLSCREEN",      "Hauteur zone client plein ecran",
SM_CYKANJIWINDOW,     "SM_CYKANJIWINDOW",     "Hauteur fenetre Kanji",
SM_MOUSEPRESENT,      "SM_MOUSEPRESENT",      "Indicateur de presence souris",
SM_CYVSCROLL,         "SM_CYVSCROLL",         "Hauteur fleche defilement V.",
SM_CXHSCROLL,         "SM_CXHSCROLL",         "Largeur fleche defilement H.",
SM_DEBUG,             "SM_DEBUG",             "Indicateur version de debogage",
SM_SWAPBUTTON,        "SM_SWAPBUTTON",        "Indicateur d'inversion boutons",
SM_RESERVED1,         "SM_RESERVED1",         "Reserve",
SM_RESERVED2,         "SM_RESERVED2",         "Reserve",
SM_RESERVED3,         "SM_RESERVED3",         "Reserve",
SM_RESERVED4,         "SM_RESERVED4",         "Reserve",
SM_CXMIN,             "SM_CXMIN",             "Largeur minimale fenetre",
SM_CYMIN,             "SM_CYMIN",             "Hauteur minimale fenetre",
SM_CXSIZE,            "SM_CXSIZE",            "Largeur icone Reduire/Agrandir",
SM_CYSIZE,            "SM_CYSIZE",            "Hauteur icone Reduire/Agrandir",
SM_CXFRAME,           "SM_CXFRAME",           "Largeur cadre fenetre",
SM_CYFRAME,           "SM_CYFRAME",           "Hauteur cadre fenetre",
SM_CXMINTRACK,        "SM_CXMINTRACK",        "Deplacement min. fenetre en x",
SM_CYMINTRACK,        "SM_CYMINTRACK",        "Deplacement min. fenetre en y",
SM_CXDOUBLECLK,       "SM_CXDOUBLECLK",       "Tolerance double click en x",
SM_CYDOUBLECLK,       "SM_CYDOUBLECLK",       "Tolerance double click en y",
SM_CXICONSPACING,     "SM_CXICONSPACING",     "Espace horizontal entre icones",
SM_CYICONSPACING,     "SM_CYICONSPACING",     "Espace vertical entre icones",
SM_MENUDROPALIGNMENT, "SM_MENUDROPALIGNMENT", "Deroulement menu bouton G ou D",
SM_PENWINDOWS,        "SM_PENWINDOWS",        "Extensions stylo installees"
} ;

#endif /* SYSMETS_C */


#if defined (SYSMETS_C)
	HWND        hwnd ;
#else
	extern HWND hwnd;
#endif /* SYSMETS_c */