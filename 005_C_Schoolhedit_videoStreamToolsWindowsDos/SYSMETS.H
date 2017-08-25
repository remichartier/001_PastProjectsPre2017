#if defined(SYSMETS_C)
#include <stdio.h>
#include "function.h"
#endif /*SYSMETS_C*/

/******EXTERNAL INCLUDES******/
#if defined(SYSMETS_C) || defined (FUNCTIONS_C) || defined(F1_C)
#include "sysmets.het"
#include "sysmets.hem"
#endif /*SYSMETS_C || FUNCTIONS_C*/


/******EXTERNAL INCLUDES******/
#if defined(SYSMETS_C)
#endif /*SYSMETS_C*/



/*--------------------------------------------------------------------------
SYSMETS.H -- Structure pour afficher la taille par defaut des objets Windows
----------------------------------------------------------------------------*/


#if !defined(__DOS__)

#if defined (SYSMETS_C) || defined (SYSMETS_RC)
#include <windows.h>
#endif  /* (SYSMETS_C) || defined (SYSMETS_RC) */

#if defined (SYSMETS_C)
#include <stdlib.h>   // pour les constantes _MAX_PATH, _MAX_FNAME, _MAX_EXT
#include <commdlg.h>
#include <windowsx.h>	// pour les fonctions GlobalAllocPtr et GlobalFreePtr
#include <stdio.h>
#include <string.h>
#endif /* SYSMETS_C */



#if defined (SYSMETS_C) || defined (SYSMETS_RC)
#define IDM_OPEN					 10
#define IDM_QUITTER				 20
#define IDM_DECODAGE			 30
#endif /* (SYSMETS_C) || defined (SYSMETS_RC) */





#if defined (SYSMETS_C)
	HWND        hwnd ;
#else
	extern HWND hwnd;
#endif /* SYSMETS_c */

#endif /* __DOS__ */
