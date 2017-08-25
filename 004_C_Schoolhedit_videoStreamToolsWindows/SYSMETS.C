/*------------------------------------------------------------------
   SYSMETS.C -- Affichage de la taille par defaut des objets Windows
		Version definitive
		(c) Charles Petzold, 1992
  ------------------------------------------------------------------*/

#define SYSMETS_C
#include "sysmets.h"
#undef SYSMETS_C

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

long FAR PASCAL _export WndProc (HWND, UINT, UINT, LONG) ;

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
		    LPSTR lpszCmdLine, int nCmdShow)
     {
     static char szAppName[] = "hedit" ;
/*     HWND        hwnd ;  */
     MSG         msg ;
     WNDCLASS    wndclass ;

     if (!hPrevInstance) 
	  {
	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	  wndclass.lpfnWndProc   = WndProc ;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = hInstance ;
	  wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
		wndclass.lpszMenuName  = szAppName ;
		wndclass.lpszClassName = szAppName ;

	  RegisterClass (&wndclass) ;
	  }

     hwnd = CreateWindow (szAppName, "Taille des objets",
			  WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, nCmdShow) ;
     UpdateWindow (hwnd) ;

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
	static long  			cxChar, cxCaps, cyChar, cxClient, cyClient, nMaxWidth,
		   								nVscrollPos, nVscrollMax, nHscrollPos, nHscrollMax ;
  char          			szBuffer[10] ;
  HDC           			hdc ;
	long	         			x, y, nPaintBeg, nPaintEnd, nVscrollInc, nHscrollInc;
	long								i,j;
  PAINTSTRUCT   			ps ;
	TEXTMETRIC    			tm ;
	static char   			szFileName  [_MAX_PATH],
											szTitleName [_MAX_FNAME + _MAX_EXT] ;
	static char * 			szFilter[] = { 	"Fichiers Mpeg (*.mpg)", "*.mpg",
																"Tous fichiers (*.*)"  , "*.*",
															"Fichiers Hexa (*.hex)"  ,"*.hex","" } ;
  static OPENFILENAME	ofn ;
	HMENU								hMenu;
	HFILE 							hFile;
	static HGLOBAL 			hGlobalMemory=NULL, hVideoBuffer=NULL;
	LPBYTE        			lpbnzGlobalMemory;
	static LONG					lFileLength,lNbLinesToDisplay;
	BYTE								sDisplayLine[49],sHexaNumber[3];
  BYTE								ret;


	switch (message)
	{
		case WM_CREATE:
	  	hdc = GetDC (hwnd) ;
	    GetTextMetrics (hdc, &tm) ;
	    cxChar = tm.tmAveCharWidth ;
	    cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
	    cyChar = tm.tmHeight + tm.tmExternalLeading ;

	    ReleaseDC (hwnd, hdc) ;

			nMaxWidth = 40 * cxChar + 22 * cxCaps ;

			ofn.lStructSize       = sizeof (OPENFILENAME) ;
      ofn.hwndOwner         = hwnd ;
      ofn.lpstrFilter       = szFilter [0] ;
      ofn.lpstrFile         = szFileName ;
      ofn.nMaxFile          = _MAX_PATH ;
      ofn.lpstrFileTitle    = szTitleName ;
      ofn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT ;
	    ofn.lpstrDefExt       = "hex" ;
			ofn.lpstrInitialDir	  = "c:\\remi\\c\\hedit" ;
	    return 0 ;

		case WM_COMMAND :
	  	hMenu = GetMenu ( hwnd ) ;
			switch ( wParam )
			{
				case IDM_OPEN :
					if(GetOpenFileName (&ofn))
					{
						/* traitement du fichier */
						/* Open the input file (read only). */

	 					if (-1 == (hFile = _lopen (szFileName, OF_READ | OF_SHARE_DENY_WRITE)))
							return (NULL) ;

						lFileLength=_llseek(hFile,0,2);

						if(lFileLength == HFILE_ERROR)
						{
					 		MessageBox(hwnd,"IDM_OPEN : seek end file","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);

							_lclose(hFile);
							return (NULL);
						}

						lNbLinesToDisplay = lFileLength /16;

						if (hGlobalMemory!= NULL)
							GlobalFree(hGlobalMemory);

						hGlobalMemory = GlobalAlloc(GMEM_MOVEABLE,lFileLength);

						if (hGlobalMemory == NULL)
						{
							MessageBox(hwnd,"IDM_OPEN : GlobalAlloc","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
							_lclose(hFile);
							return NULL;
						}

						lpbnzGlobalMemory=GlobalLock(hGlobalMemory);

						if (lpbnzGlobalMemory == NULL)
						{
							MessageBox(hwnd,"IDM_OPEN :GlobalLock","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
							if(hGlobalMemory!= NULL)
								GlobalFree(hGlobalMemory);
							_lclose(hFile);
							return NULL;
						}

						if (HFILE_ERROR==_llseek(hFile,0,0))
						{
							MessageBox(hwnd,"IDM_OPEN : seek position 0","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
							if(hGlobalMemory!= NULL)
								GlobalFree(hGlobalMemory);
							_lclose(hFile);
							return NULL;
						}

						if((-1L) ==_hread(hFile,lpbnzGlobalMemory,lFileLength ))
						{
							MessageBox(hwnd,"IDM_OPEN : error read","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
							if(hGlobalMemory!= NULL)
								GlobalFree(hGlobalMemory);
							_lclose(hFile);
							return NULL;
						}			
				
						_lclose(hFile);

						GlobalUnlock(hGlobalMemory);

					} /* end if getopenfilename() */
				return 0 ;
			case IDM_QUITTER :
		  	SendMessage ( hwnd, WM_CLOSE, 0, 0L ) ;
				return ( 0 ) ;
			case IDM_DECODAGE :
				if(hGlobalMemory!= NULL)
				{
//					MessageBox(hwnd,"IDM_DECODAGE: hGlobalMemory!=NULL","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);

					if (hVideoBuffer!= NULL)
					{
						MessageBox(hwnd,"IDM_DECODAGE: hVideoBuffer!=NULL","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
						GlobalFree(hVideoBuffer);
          }
					hGlobalMemory = GlobalAlloc(GMEM_MOVEABLE,lFileLength);			

					ret=Iso11172_stream((PHANDLE) &hGlobalMemory,(LPHANDLE)  &hVideoBuffer);
        }
				return (0) ;

			} /* switch wParam */
    	break; /* case WM_COMMAND */

	  case WM_SIZE:
	  	cxClient = LOWORD (lParam) ;
	    cyClient = HIWORD (lParam) ;

			nVscrollMax = max (0, lNbLinesToDisplay + 2 - cyClient / cyChar) ;
	    nVscrollPos = min (nVscrollPos, nVscrollMax) ;

	    SetScrollRange (hwnd, SB_VERT, 0, nVscrollMax, FALSE) ;
	    SetScrollPos   (hwnd, SB_VERT, nVscrollPos, TRUE) ;

	    nHscrollMax = max (0, 2 + (nMaxWidth - cxClient) / cxChar) ;
	    nHscrollPos = min (nHscrollPos, nHscrollMax) ;

	    SetScrollRange (hwnd, SB_HORZ, 0, nHscrollMax, FALSE) ;
	    SetScrollPos   (hwnd, SB_HORZ, nHscrollPos, TRUE) ;
	    return 0 ;

	  case WM_VSCROLL:
	  	switch (wParam)
		  {
		    case SB_TOP:
			 		nVscrollInc = -nVscrollPos ;
			 		break ;

		    case SB_BOTTOM:
			 		nVscrollInc = nVscrollMax - nVscrollPos ;
			 		break ;

		    case SB_LINEUP:
			 		nVscrollInc = -1 ;
			 		break ;

		    case SB_LINEDOWN:
			 		nVscrollInc = 1 ;
			 		break ;

		    case SB_PAGEUP:
			 		nVscrollInc = min (-1, -cyClient / cyChar) ;
			 		break ;

		    case SB_PAGEDOWN:
			 		nVscrollInc = max (1, cyClient / cyChar) ;
			 		break ;

		    case SB_THUMBTRACK:
			 		nVscrollInc = LOWORD (lParam) - nVscrollPos ;
			 		break ;

		    default:
			 		nVscrollInc = 0 ;
		  }

	    nVscrollInc = max (-nVscrollPos,
			     					min (nVscrollInc, nVscrollMax - nVscrollPos)) ;

	    if (nVscrollInc != 0)
		  {
		    nVscrollPos += nVscrollInc ;
		    ScrollWindow (hwnd, 0, -cyChar * nVscrollInc, NULL, NULL) ;
		    SetScrollPos (hwnd, SB_VERT, nVscrollPos, TRUE) ;
		    UpdateWindow (hwnd) ;
		  }
	    return 0 ;

	  case WM_HSCROLL:
	  	switch (wParam)
		  {
		    case SB_LINEUP:
			 		nHscrollInc = -1 ;
					break ;

		    case SB_LINEDOWN:
			 		nHscrollInc = 1 ;
			 		break ;

		    case SB_PAGEUP:
			 		nHscrollInc = -8 ;
			 		break ;

		    case SB_PAGEDOWN:
			 		nHscrollInc = 8 ;
			 		break ;

		    case SB_THUMBPOSITION:
			 		nHscrollInc = LOWORD (lParam) - nHscrollPos ;
			 		break ;

		    default:
			 		nHscrollInc = 0 ;
		  }
	    nHscrollInc = max (-nHscrollPos,
			     min (nHscrollInc, nHscrollMax - nHscrollPos)) ;

	    if (nHscrollInc != 0)
		  {
		    nHscrollPos += nHscrollInc ;
		    ScrollWindow (hwnd, -cxChar * nHscrollInc, 0, NULL, NULL) ;
		    SetScrollPos (hwnd, SB_HORZ, nHscrollPos, TRUE) ;
		  }
	    return 0 ;

	  case WM_KEYDOWN:
	  	switch (wParam)
		  {
		    case VK_HOME:
			 		SendMessage (hwnd, WM_VSCROLL, SB_TOP, 0L) ;
			 		break ;

		    case VK_END:
			 		SendMessage (hwnd, WM_VSCROLL, SB_BOTTOM, 0L) ;
			 		break ;

		    case VK_PRIOR:
			 		SendMessage (hwnd, WM_VSCROLL, SB_PAGEUP, 0L) ;
			 		break ;

		    case VK_NEXT:
			 		SendMessage (hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L) ;
			 		break ;

		    case VK_UP:
			 		SendMessage (hwnd, WM_VSCROLL, SB_LINEUP, 0L) ;
			 		break ;

		    case VK_DOWN:
			 		SendMessage (hwnd, WM_VSCROLL, SB_LINEDOWN, 0L) ;
			 		break ;

		    case VK_LEFT:
			 		SendMessage (hwnd, WM_HSCROLL, SB_PAGEUP, 0L) ;
			 		break ;

		    case VK_RIGHT:
			 		SendMessage (hwnd, WM_HSCROLL, SB_PAGEDOWN, 0L) ;
			 		break ;
		  }
	    return 0 ;

	  case WM_PAINT:
	  	hdc = BeginPaint (hwnd, &ps) ;

	    nPaintBeg = max (0, nVscrollPos + ps.rcPaint.top / cyChar - 1) ;
			nPaintEnd = min (lNbLinesToDisplay,
			nVscrollPos + ps.rcPaint.bottom / cyChar) ;
				if (hGlobalMemory!= NULL)
				{
					lpbnzGlobalMemory=GlobalLock(hGlobalMemory);
					if (lpbnzGlobalMemory == NULL)
					{
								 MessageBox(hwnd,"globallock","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);

						if(hGlobalMemory!= NULL)
							GlobalFree(hGlobalMemory);
						_lclose(hFile);
						return NULL;
					}
					for (i = nPaintBeg ; i < nPaintEnd ; i++)
		  		{
		    		x = cxChar * (1 - nHscrollPos) ;
		    		y = cyChar * (1 - nVscrollPos + i) ;

				
						sDisplayLine[0] = '\0';
						for(j=0;j<16;j++)
						{
							sprintf(sHexaNumber,"%02hX ",
										(BYTE)*(char far *)(lpbnzGlobalMemory +i*16 +j));

							strcat(sDisplayLine,sHexaNumber);

						} /* end for */

						TextOut (hdc, x, y, sDisplayLine,
										 lstrlen (sDisplayLine));


				/*
		    TextOut (hdc, x, y,
			     sysmetrics[i].szLabel,
					 lstrlen (sysmetrics[i].szLabel)) ;

		    TextOut (hdc, x + 22 * cxCaps, y,
			     sysmetrics[i].szDesc,
			     lstrlen (sysmetrics[i].szDesc)) ;

		    SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;

		    TextOut (hdc, x + 22 * cxCaps + 40 * cxChar, y,
			     szBuffer,
			     wsprintf (szBuffer, "%5d",
				  GetSystemMetrics (sysmetrics[i].nIndex))) ;

				SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
				*/

				}/* end for (i = nPaintBeg ; i < nPaintEnd ; i++) */

				GlobalUnlock(hGlobalMemory);
			} /* end if (hGlobalMemory!= NULL) */


	    EndPaint (hwnd, &ps) ;
	    return 0 ;

		case WM_CLOSE :
 //						MessageBox(hwnd,"CLOSE","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
				DestroyWindow ( hwnd ) ;
			return 0 ;

		case WM_DESTROY:
				PostQuitMessage (0) ;
			if(hGlobalMemory!= NULL)
				GlobalFree(hGlobalMemory);
			return 0 ;
	}

  return DefWindowProc (hwnd, message, wParam, lParam) ;
}

