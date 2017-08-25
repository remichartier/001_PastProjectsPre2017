#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>   // pour les constantes _MAX_PATH, _MAX_FNAME, _MAX_EXT
#include <windowsx.h>	// pour les fonctions GlobalAllocPtr et GlobalFreePtr
#include <math.h>	// pour powl
#include "image1.h"
#include "fonction.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))



// definition variables globales
unsigned int retour;
HANDLE hInst ;

long FAR PASCAL  WndProc (	HWND ,
				WORD ,
				WORD ,
				LONG );



int PASCAL WinMain(	//point d'entrée du programme windows contenant le code qui sera exécuté à son lancement.le point d'entrée de tout programme windows s'appelle WinMain
			HANDLE hInstance,
			HANDLE hPrevInstance,
			LPSTR lpszCmdLine,
			int nCmdShow )
{
  static char szAppName [] = "image1";
  MSG msg;
  HWND hwnd;
  WNDCLASS windclass; //contains class attributes that are registered by the RegisterClass fuction
  

  //register the window class
  if (!hPrevInstance)
  {
    windclass.style = CS_HREDRAW |CS_VREDRAW |CS_BYTEALIGNCLIENT;	//pour ouvrir un fenêtre, un prog informe le système sur le type de celle ci.				//UINT
    windclass.lpfnWndProc =(WNDPROC)WndProc;		//WNDPROC : points to the window procedure. see the WindowProc callback function
    windclass.cbClsExtra = 0;			//int : specifies the number of bytes to allocate following the window-class structures
    windclass.cbWndExtra = 0;			//int : specifies the number of bytes to allocate following the window instance.
    windclass.hInstance = hInstance;			//HINSTANCE : identifies the class module. must be an instance handle
    windclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);	//HICON : identifies the class icon. must be a handle to an icon resource
    windclass.hCursor = LoadCursor (NULL, IDC_ARROW);   //HCURSOR : identifies the class cursor. must be a handle to a cursor resource
    windclass.hbrBackground = GetStockObject( WHITE_BRUSH );//HBRUSH : identifies the class background brush. can be a handle to the physical brush or a color value
    windclass.lpszMenuName = szAppName ;                 //LPCSTR : points to a string that specifies the resource name of the class menu
    windclass.lpszClassName = szAppName;           //LPCSTR : points to a string that specifies the name of the window class
 		
    RegisterClass(&windclass);	//la fonction RegisterClass enregistre la classe de la fenêtre. Dès lors, toutes les fenêtres créées par cette application seront de la même classe, c'est à dire qu'elles présenteront des caractéristiques identiques.
  }


  // à ce point du programme, aucune fenêtre n'apparait encore à l'écran. Pour l'afficher réellement, le programme doit encore faire appel à 3 fonctions : CreateWindow, ShowWindow et UpdateWindow, qui créent, affichent et mettent à jour le contenu de la fenêtre

  //create the window

  hwnd =		       		//type HWND
	CreateWindow(
	szAppName,
	szAppName,			//type LPCSTR : address of registerd class name
				//type LPCSTR : address of window text
	WS_OVERLAPPEDWINDOW,		//DWORD : window style
	CW_USEDEFAULT,			//int : horizontal position of window
	CW_USEDEFAULT,				//int : vertical position of window
	CW_USEDEFAULT,			//int : window width
	CW_USEDEFAULT,				//int : window height
	NULL,				//HWND : handle of parent window
	NULL,				//HMENU : handle of menu or child-window identifier
	hInstance,			//HINSTANCE : handle of application instance
	NULL);				//void FAR * : address of window-creation data



  ShowWindow(          			//renvoie BOOL. sets the given window's visibility state
	hwnd,				//HWND : handle of window . Identifies the window
	nCmdShow);			//int : window visibility flag . Specifies how the window is to be shown. This parameter can be one of the following values :  SW_HIDE, SW_MINIMIZE, SW_RESTORE, SW_SHOW, SW_SHOWMAXIMIZED, SW_SHOWMINIMIZED, SW_SHOWMINNOACTIVE, SW_SHOWNOACTIVATE, SW_SHOWNA, SW_SHOWNORMAL
 
  UpdateWindow (hwnd); 			//updates the client area of the given window by sending a WM_PAINT message to the window if the update region for the window is not empty. The function sends a WM_PAINT message directly to the window procedure of the given window, bypassing the application queue. If the update region is empty, no message is sent.

  

  				



  //la boucle suivante représente le coeur de l'application windows. Elle récupère les messages depuis la file d'attente des messages et les transmet à la fonction WndProc,appelée automatiquement par le système après DispatchMessage. Chaque action de l'utilisateur (clic de souris, sélection menu, etc.) se traduit par un message, analysé puis traité dans cette fonction.
  while(GetMessage			// returns BOOL .
	(&msg,				// MSG FAR* : address of structure with message
	NULL,				// HWND : handle of the window
	0,				// UINT : first message
	0))				// UINT : last message
  {
    TranslateMessage (&msg);
    DispatchMessage(&msg);
  }


  return msg.wParam;
   
} // fin du WinMain


long FAR PASCAL WndProc (
	HWND hwnd,
	WORD message,
	WORD wParam,
	LONG lParam
	)
{

// variable pour afficher un texte dans le rectangle
HDC 			hdc ;
PAINTSTRUCT 		ps ;

// variables pour transfert fichier DIB compresse en memoire
static BYTE huge *	lpDibEnMemoire ;
BYTE huge *		lpDibBitsEnMemoire ;
BYTE huge *		lpDibColorTable ;
static short		cxClient, cyClient ;
static char         	szFileName  [_MAX_PATH],
                    	szTitleName [_MAX_FNAME + _MAX_EXT] ;
static char *       	szFilter[] = { "Fichiers Bitmap (*.BMP)",  "*.bmp",
                                   "Fichiers DIB (*.DIB)",     "*.dib",
					"" } ;
static OPENFILENAME	ofn ;
static WORD 		grandeur = IDM_BITBLT_GRANDEUR_REELLE ;
HMENU			hMenu ;
short			DibWidth, DibHeight ;
HBITMAP			hBitmap ;
POINT			ptPoint ;
RECT 			rect ;
float			ratio ;

// manipulation des couleurs
unsigned short		PourcentageRouge = 99 ;
unsigned short		PourcentageVert = 99 ;
unsigned short		PourcentageBleu = 99 ;
unsigned long		NbCouleursBitmap ;
unsigned long		i ;

static FARPROC		lpfnColorsDlgProc ;


  switch (message)
  {
    case WM_CREATE :
	ofn.lStructSize       = sizeof (OPENFILENAME) ;
               ofn.hwndOwner         = hwnd ;
               ofn.lpstrFilter       = szFilter [0] ;
               ofn.lpstrFile         = szFileName ;
               ofn.nMaxFile          = _MAX_PATH ;
               ofn.lpstrFileTitle    = szTitleName ;
               ofn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT ;
	       ofn.lpstrDefExt       = "bmp" ;
	       ofn.lpstrInitialDir	= "c:\\remi\\images" ;

	return 0 ;
    case WM_COMMAND :
	hMenu = GetMenu ( hwnd ) ;
	switch ( wParam )
		{
		case IDM_OPEN :
			if(GetOpenFileName (&ofn)){
			     	if (lpDibEnMemoire != NULL)
                	                {
                        	         GlobalFreePtr (lpDibEnMemoire) ;
                                	 lpDibEnMemoire = NULL ;
				   	}
				lpDibEnMemoire = ReadDib ( szFileName ) ;
				EnableMenuItem ( hMenu, IDM_REGLAGE_COULEURS, MF_ENABLED ) ;
				InvalidateRect (hwnd, NULL, TRUE) ;
				}
			return 0 ;
		case IDM_QUITTER :
		  	SendMessage ( hwnd, WM_CLOSE, 0, 0L ) ;
		    	return ( 0 ) ;
		case IDM_STRETCHDIBITS :
		case IDM_SETDIBITSTODEVICE :
		case IDM_BITBLT_GRANDEUR_REELLE :
		case IDM_STRETCHBLT_GRANDEUR_FENETRE :
		case IDM_STRETCHBLT_GRANDEUR_FENETRE_PROPORTIONNEL :
			CheckMenuItem ( hMenu, grandeur, MF_UNCHECKED ) ;
			grandeur = wParam ;
			CheckMenuItem ( hMenu, grandeur, MF_CHECKED ) ;

			InvalidateRect ( hwnd, NULL, TRUE ) ;
			return 0 ;
		case IDM_CHANGEMENT_COULEURS :
                        // CALCUL NB DE STRUCTURES RGBSQUAD

			if ( (	( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biClrUsed == 0 ) &&
			    ( (  ( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biBitCount == 1 ) ||
				(( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biBitCount == 4 ) ||
				(( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biBitCount == 8 ) ||
				(( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biBitCount == 24 ) ) )
				{
				if ( ( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biBitCount != 24 )
					NbCouleursBitmap = powl ( 2 , ( ( BITMAPINFOHEADER huge * ) lpDibEnMemoire )->biBitCount ) ;
				else
					NbCouleursBitmap = ( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biWidth *
						       (( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biHeight ;
				}
			else
				{
				if ( ( ( BITMAPINFOHEADER huge * ) lpDibEnMemoire )->biClrUsed != 0 )
					{
                                        NbCouleursBitmap = ( ( BITMAPINFOHEADER huge * ) lpDibEnMemoire ) -> biClrUsed ;
                                        }
				else
					{
					MessageBox ( hwnd, "Erreur dans table couleur et bitmapinfoheader", "Message d'erreur", MB_OK ) ;
					exit ( 0 ) ;
					}
                                }


			// CHANGEMENT DES COULEURS RVB

			if ( ( ( BITMAPINFOHEADER huge * ) lpDibEnMemoire )->biBitCount != 24 )
				{
				lpDibColorTable = ( BYTE huge * )lpDibEnMemoire + ( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biSize ;
				for ( i = 0 ; i < NbCouleursBitmap ; i ++ )
					{
					ChangeCouleurRGBQUAD ( (RGBQUAD huge * ) (lpDibColorTable + i * sizeof ( RGBQUAD ) ),
								PourcentageRouge,
								PourcentageVert,
								PourcentageBleu ) ;
                                	} // fin du for
				}	// fin du if
			else
				{
				for ( i = 0 ; i < NbCouleursBitmap ; i ++ )
					{
					ChangeCouleurRGB ( (COULEURRGB * )(lpDibBitsEnMemoire + i * sizeof ( COULEURRGB )),
								PourcentageRouge,
								PourcentageVert,
								PourcentageBleu ) ;
                                	} // fin du for
				} // fin du if
				InvalidateRect ( hwnd, NULL, TRUE ) ;
			return 0 ;
		}
		
	break ;
			
    case WM_SIZE :
	cxClient = LOWORD ( lParam ) ;
	cyClient = HIWORD ( lParam ) ;
	return 0 ;
    case WM_PAINT :
	    hdc = BeginPaint (hwnd, &ps) ;
	    if ( lpDibEnMemoire != NULL )
		{
		DibWidth = GetDibWidth ( lpDibEnMemoire ) ;
		DibHeight = GetDibHeight ( lpDibEnMemoire ) ;
		lpDibBitsEnMemoire = GetDibBitsAddr ( lpDibEnMemoire ) ;
			
		SetStretchBltMode ( hdc, COLORONCOLOR ) ;
		switch ( grandeur )
			{
			case IDM_STRETCHDIBITS :
				StretchDIBits( 	hdc,
						0,
						0,
						cxClient,
						cyClient,
						0,
						0,
						DibWidth ,
						DibHeight ,
						( LPSTR ) lpDibBitsEnMemoire ,
						( LPBITMAPINFO ) lpDibEnMemoire,
						DIB_RGB_COLORS, SRCCOPY );
				break ;
			case IDM_SETDIBITSTODEVICE :
				
				SetDIBitsToDevice (
							hdc,
							0,
							0,
							DibWidth ,
							DibHeight ,
							0,
							0,
							0,
							DibHeight ,
							( LPSTR ) lpDibBitsEnMemoire ,
							( LPBITMAPINFO ) lpDibEnMemoire,
							DIB_RGB_COLORS ) ;
				break ;
			case IDM_BITBLT_GRANDEUR_REELLE :
				SetMapMode ( hdc, MM_ISOTROPIC ) ;
				hBitmap = CreateDIBitmap (
								hdc,
								( LPBITMAPINFOHEADER ) lpDibEnMemoire,
								CBM_INIT,
								( LPSTR ) lpDibBitsEnMemoire,
								( LPBITMAPINFO ) lpDibEnMemoire,
								DIB_RGB_COLORS ) ;
				DrawBitmapBitBlt ( hdc, hBitmap, 0 , 0 ) ;
                                DeleteObject ( hBitmap ) ;
				break ;
			case IDM_STRETCHBLT_GRANDEUR_FENETRE :
				hBitmap = CreateDIBitmap (
								hdc,
								( LPBITMAPINFOHEADER ) lpDibEnMemoire,
								CBM_INIT,
								( LPSTR ) lpDibBitsEnMemoire,
								( LPBITMAPINFO ) lpDibEnMemoire,
								DIB_RGB_COLORS ) ;

				GetClientRect ( hwnd, &rect ) ;
				ptPoint.x = rect.right ;
                                ptPoint.y = rect.bottom ;
				DrawBitmapStretchBlt ( hdc, hBitmap, 0 , 0, ptPoint ) ;
				DeleteObject ( hBitmap ) ;
				break ;
			case IDM_STRETCHBLT_GRANDEUR_FENETRE_PROPORTIONNEL :
				 hBitmap = CreateDIBitmap (
								hdc,
								( LPBITMAPINFOHEADER ) lpDibEnMemoire,
								CBM_INIT,
								( LPSTR ) lpDibBitsEnMemoire,
								( LPBITMAPINFO ) lpDibEnMemoire,
								DIB_RGB_COLORS ) ;

	
				GetClientRect ( hwnd, &rect ) ;
			
				if ( (float)rect.right/(float)DibWidth >= (float)rect.bottom / (float)DibHeight )
					ratio = (float)rect.bottom / (float)DibHeight ;
				else
                                	ratio = (float)rect.right / (float)DibWidth ;
				ptPoint.x = (int) (ratio * (float)DibWidth) ;
				ptPoint.y = (int) ( ratio * (float)DibHeight) ;
				DPtoLP ( hdc, &ptPoint, 1 ) ;
				DrawBitmapStretchBlt ( hdc, hBitmap, 0 , 0, ptPoint ) ;
				DeleteObject ( hBitmap ) ;
				break ;
				 
			} // fin du switch
		} 	// fin du if ( lpDibMemoire != NULL )
	    EndPaint (hwnd, &ps) ;  //termine le dessin de la fenêtre
	    break ;

    case WM_CLOSE :
	DestroyWindow ( hwnd ) ;
	return 0 ;
    case WM_DESTROY:
      if ( lpDibEnMemoire != NULL )
		GlobalFreePtr ( lpDibEnMemoire ) ;
      PostQuitMessage (0);  		// insère un message quitter dans la file d'attente ou posts a message to Windows indicating that an application is requesting to terminate execution (quit).
      return(0);
    default : return (DefWindowProc(	// LRESULT : calls the default window procedure which provides default processing for any window messages that an application does not process, and so ensures that every message is processed
			hwnd,		// HWND : handle of window
			message,	// UINT : type of message
			wParam,		// WPARAM : first message parameter
			lParam)); 	// LPARAM : second message parameter
  }

  return 0;
} // fin de WndProc


