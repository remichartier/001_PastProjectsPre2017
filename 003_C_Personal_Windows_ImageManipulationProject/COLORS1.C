
#include <commdlg.h>
#include <stdlib.h>   // pour les constantes _MAX_PATH, _MAX_FNAME, _MAX_EXT
#include <windowsx.h>	// pour les fonctions GlobalAllocPtr et GlobalFreePtr
#include <math.h>	// pour powl
#include "image1.h"
#include "fonction.h"
#include "colors1.h"
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))



// definition variables globales
unsigned int retour;
HANDLE hInst ;
short		PourcentageComposanteCouleur[3] = {0,0,0};

long FAR PASCAL _export WndProc (HWND, UINT, UINT, LONG) ;
BOOL FAR PASCAL _export ColorsDlgProc (HWND, UINT, UINT, LONG) ;

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
		    LPSTR lpszCmdLine, int nCmdShow)
     {
     static char szAppName [] = "Image1" ;
     MSG         msg;
     HWND        hwnd ;
     WNDCLASS    wndclass ;

     if (!hPrevInstance) 
	  {
	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	  wndclass.lpfnWndProc   = WndProc ;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = hInstance ;
	  wndclass.hIcon         = LoadIcon (hInstance, szAppName) ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = szAppName ;
	  wndclass.lpszClassName = szAppName ;

	  RegisterClass (&wndclass) ;
	  }

     hwnd = CreateWindow (szAppName, "Demonstration A propos de ...",
			  WS_OVERLAPPEDWINDOW,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, nCmdShow) ;
     UpdateWindow (hwnd); 

     while (GetMessage (&msg, NULL, 0, 0))
	  {
	  TranslateMessage (&msg) ;
	  DispatchMessage (&msg) ;
	  }
     return msg.wParam ;
     }

long FAR PASCAL _export WndProc (HWND hwnd, UINT message, UINT wParam,
							  LONG lParam)
     {
     static FARPROC lpfnColorsDlgProc ;
     static HANDLE  hInstance ;
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
static WORD 		grandeur = IDM_STRETCHBLT_GRANDEUR_FENETRE_PROPORTIONNEL ;
HMENU			hMenu ;
short			DibWidth, DibHeight ;
HBITMAP			hBitmap ;
POINT			ptPoint ;
RECT 			rect ;
float			ratio ;

// manipulation des couleurs
static BYTE huge *		lpCouleursInitiales ;
static unsigned long		NbCouleursBitmap ;
unsigned long		i ;


     switch (message)
	  {
	  case WM_CREATE:
	       ofn.lStructSize       = sizeof (OPENFILENAME) ;
               ofn.hwndOwner         = hwnd ;
               ofn.lpstrFilter       = szFilter [0] ;
               ofn.lpstrFile         = szFileName ;
               ofn.nMaxFile          = _MAX_PATH ;
               ofn.lpstrFileTitle    = szTitleName ;
               ofn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT ;
	       ofn.lpstrDefExt       = "bmp" ;
	       ofn.lpstrInitialDir	= "c:\\remi\\images" ;

	       hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;

	       lpfnColorsDlgProc = MakeProcInstance ((FARPROC) ColorsDlgProc,
						    hInstance) ;
	       return 0 ;

	  case WM_COMMAND:
	       hMenu = GetMenu ( hwnd ) ;
	       switch (wParam)
		    {
		    case IDM_OPEN :
			if(GetOpenFileName (&ofn)){
			     	if (lpDibEnMemoire != NULL)
                	                {
                        	         GlobalFreePtr (lpDibEnMemoire) ;
                                	 lpDibEnMemoire = NULL ;
				   	}
				lpDibEnMemoire = ReadDib ( szFileName ) ;
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

				// memorisation des couleurs initiales
				lpCouleursInitiales = ( BYTE huge * ) GlobalAllocPtr ( GMEM_MOVEABLE,NbCouleursBitmap * 4 ) ;

				if ( ( ( BITMAPINFOHEADER huge * ) lpDibEnMemoire )->biBitCount != 24 )
					{
					lpDibColorTable = ( BYTE huge * )lpDibEnMemoire + ( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biSize ;
					for ( i = 0 ; i < NbCouleursBitmap ; i ++ )
						{

						( ( RGBQUAD huge * ) ( lpCouleursInitiales + i * sizeof ( RGBQUAD ) ) ) ->rgbBlue = ( (RGBQUAD huge * ) (lpDibColorTable + i * sizeof ( RGBQUAD ) ) )-> rgbBlue ;
						( ( RGBQUAD huge * ) ( lpCouleursInitiales + i * sizeof ( RGBQUAD ) ) ) ->rgbRed = ( (RGBQUAD huge * ) (lpDibColorTable + i * sizeof ( RGBQUAD ) ) )-> rgbRed ;
						( ( RGBQUAD huge * ) ( lpCouleursInitiales + i * sizeof ( RGBQUAD ) ) ) ->rgbGreen = ( (RGBQUAD huge * ) (lpDibColorTable + i * sizeof ( RGBQUAD ) ) )-> rgbGreen ;

						//ChangeColorRGBQUAD ( (RGBQUAD huge * ) (lpDibColorTable + i * sizeof ( RGBQUAD ) ),
						//		PourcentageComposanteCouleur[ROUGE],
						//		PourcentageComposanteCouleur[VERT],
						//		PourcentageComposanteCouleur[BLEU] ) ;
                                		} // fin du for
					}	// fin du if


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
		    case IDM_REGLAGE_COULEURS:
			 DialogBox (hInstance, "ColorsBox", hwnd,
					lpfnColorsDlgProc) ;
			 return 0 ;
		    case IDM_CHANGEMENT_COULEURS :


			// CHANGEMENT DES COULEURS RVB

			if ( ( ( BITMAPINFOHEADER huge * ) lpDibEnMemoire )->biBitCount != 24 )
				{
				lpDibColorTable = ( BYTE huge * )lpDibEnMemoire + ( ( BITMAPINFOHEADER huge * )lpDibEnMemoire)->biSize ;
				for ( i = 0 ; i < NbCouleursBitmap ; i ++ )
					{
					ChangeColorRGBQUAD ( (RGBQUAD huge * ) (lpDibColorTable + i * sizeof ( RGBQUAD ) ),
								( RGBQUAD huge * ) (lpCouleursInitiales + i * sizeof ( RGBQUAD ) ) ,
								PourcentageComposanteCouleur[ROUGE],
								PourcentageComposanteCouleur[VERT],
								PourcentageComposanteCouleur[BLEU],
								LOWORD (lParam) ) ;
                                	} // fin du for
				}	// fin du if
			else
				{
				for ( i = 0 ; i < NbCouleursBitmap ; i ++ )
					{
					ChangeCouleurRGB ( (COULEURRGB * )(lpDibBitsEnMemoire + i * sizeof ( COULEURRGB )),
							    PourcentageComposanteCouleur[ROUGE],
								PourcentageComposanteCouleur[VERT],
								PourcentageComposanteCouleur[BLEU] ) ;
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
	  
	  case WM_DESTROY :
	       if ( lpDibEnMemoire != NULL )
		GlobalFreePtr ( lpDibEnMemoire ) ;
	       PostQuitMessage (0) ;
	       return 0 ;
	  }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
     }

BOOL FAR PASCAL _export ColorsDlgProc (HWND hDlg, UINT message, UINT wParam,
							       LONG lParam)
     {
     HWND         hwndParent, hCtrl ;
     short        nCtrlID, nIndex ;
     static short		bouton = IDD_POURCENTAGE;
     static short 			minScroll = -100 ;
     static short			maxScroll = 100 ;

     switch (message)
	  {
	  case WM_INITDIALOG:
	       for (nCtrlID = 10 ; nCtrlID < 13 ; nCtrlID++)
                    {
                    hCtrl = GetDlgItem (hDlg, nCtrlID) ;
		    SetScrollRange (hCtrl, SB_CTL, minScroll, maxScroll, FALSE) ;
		    SetScrollPos   (hCtrl, SB_CTL, 0, TRUE) ;
		    CheckRadioButton ( hDlg, IDD_POURCENTAGE,IDD_SATURATION,bouton ) ;
		    SetDlgItemInt (hDlg,  nCtrlID + 3, 0, TRUE) ;
		    }
	       return TRUE ;

	  case WM_VSCROLL :
               hCtrl   = HIWORD (lParam) ;
               nCtrlID = GetWindowWord (hCtrl, GWW_ID) ;
               nIndex  = nCtrlID - 10 ;
               hwndParent = GetParent (hDlg) ;
		
               switch (wParam)
                    {
                    case SB_PAGEDOWN :
			 PourcentageComposanteCouleur [nIndex] -= 10 ;        // enchainer
                    case SB_LINEDOWN :
			 PourcentageComposanteCouleur [nIndex] = max (minScroll, PourcentageComposanteCouleur [nIndex] - 1) ;
                         break ;
		    case SB_PAGEUP :
			 PourcentageComposanteCouleur [nIndex] += 10 ;	// enchainer
                    case SB_LINEUP :
			 PourcentageComposanteCouleur [nIndex] = min (maxScroll, PourcentageComposanteCouleur [nIndex] + 1) ;
                         break ;
                    case SB_TOP:
			 PourcentageComposanteCouleur [nIndex] = maxScroll ;
                         break ;
                    case SB_BOTTOM :
			 PourcentageComposanteCouleur [nIndex] = minScroll ;
                         break ;
                    case SB_THUMBPOSITION :
                    case SB_THUMBTRACK :
			 PourcentageComposanteCouleur [nIndex] = -LOWORD (lParam) ;
			 break ;
                    default :
                         return FALSE ;
		    }
	       //PourcentageComposanteCouleur[nIndex] = min ( PourcentageComposanteCouleur[nIndex], 255 ) ;
	       //PourcentageComposanteCouleur[nIndex] = max ( PourcentageComposanteCouleur[nIndex], -255 ) ;
	       SetScrollPos  (hCtrl, SB_CTL,      -PourcentageComposanteCouleur [nIndex], TRUE) ;
	       SetDlgItemInt (hDlg,  nCtrlID + 3, PourcentageComposanteCouleur [nIndex], TRUE) ;

               //DeleteObject (
		 //   SetClassWord (hwndParent, GCW_HBRBACKGROUND,
                   //      CreateSolidBrush (
                     //         RGB (color[0], color[1], color[2])))) ;

               //InvalidateRect (hwndParent, NULL, TRUE) ;
	       return TRUE ;
	  case WM_COMMAND:
		hwndParent = GetParent (hDlg) ;
	       switch (wParam)
		    {
		    case IDOK:
		    //case IDCANCEL:
                      //ne marche pas	InvalidateRect ( hwndParent, NULL, TRUE ) ;
		    	 EndDialog (hDlg, 0) ;
			 return TRUE ;
		    case IDD_REDESSINER :
			SendMessage ( hwndParent, WM_COMMAND, IDM_CHANGEMENT_COULEURS, bouton ) ;
			// ne marche pas InvalidateRect ( hwndParent, NULL, TRUE ) ;
			return ( TRUE ) ;
		    case IDD_POURCENTAGE :
		    case IDD_SATURATION :
			CheckRadioButton ( hDlg, IDD_POURCENTAGE, IDD_SATURATION,
						wParam);
			bouton = wParam ;
			if ( bouton == IDD_POURCENTAGE )
			  {
			  minScroll = - 100 ;
			  maxScroll = 100 ;
			  }
			else
			  {
			  minScroll = -255 ;
			  maxScroll = 255 ;
			  }
			//SetScrollRange (hCtrl, SB_CTL, minScroll, maxScroll, FALSE) ;
			//SetScrollPos   (hCtrl, SB_CTL, 0, TRUE) ;
                        SendMessage (hDlg, WM_INITDIALOG,0L,0L ) ;
			return TRUE ;

		    }
	       break ;

	  }
     return FALSE ;
     }
