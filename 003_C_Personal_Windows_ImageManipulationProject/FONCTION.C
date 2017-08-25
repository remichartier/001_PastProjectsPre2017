// fonctions du programme image1.c
#include "fonction.h"
#include "image1.h"
#include <windows.h>
#include <windowsx.h>	// pour les fonctions GlobalAllocPtr et GlobalFreePtr

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
DWORD GetDibInfoHeaderSize (BYTE huge * lpDib)
     {
     return ((BITMAPINFOHEADER huge *) lpDib)->biSize ;
     }
BYTE huge * ReadDib (char * szFileName)
     {
     BITMAPFILEHEADER bmfh ;
     BYTE huge *      lpDib ;
     DWORD            dwDibSize, dwOffset, dwHeaderSize ;
     int              hFile ;
     WORD             wDibRead ;

     if (-1 == (hFile = _lopen (szFileName, OF_READ | OF_SHARE_DENY_WRITE)))
          return NULL ;


     if (_lread (hFile, (LPSTR) &bmfh, sizeof (BITMAPFILEHEADER)) !=
                                       sizeof (BITMAPFILEHEADER))
          {
          _lclose (hFile) ;
          return NULL ;
          }

     if (bmfh.bfType != * (WORD *) "BM")
          {
          _lclose (hFile) ;
          return NULL ;
          }

     dwDibSize = bmfh.bfSize - sizeof (BITMAPFILEHEADER) ;

     lpDib = (BYTE huge * ) GlobalAllocPtr (GMEM_MOVEABLE, dwDibSize) ;

     if (lpDib == NULL)
          {
          _lclose (hFile) ;
          return NULL ;
          }

     dwOffset = 0 ;

     while (dwDibSize > 0)
          {
          wDibRead = (WORD) MIN (32768ul, dwDibSize) ;

          if (wDibRead != _lread (hFile, (LPSTR) (lpDib + dwOffset), wDibRead))
               {
               _lclose (hFile) ;
               GlobalFreePtr (lpDib) ;
               return NULL ;
               }

          dwDibSize -= wDibRead ;
          dwOffset  += wDibRead ;
          }

     _lclose (hFile) ;

     dwHeaderSize = GetDibInfoHeaderSize (lpDib) ;

     if (dwHeaderSize < 12 || (dwHeaderSize > 12 && dwHeaderSize < 16))
          {
          GlobalFreePtr (lpDib) ;
          return NULL ;
          }

     return lpDib ;
     }




BYTE huge * GetDibBitsAddr (BYTE huge * lpDib)
     {
     DWORD dwNumColors, dwColorTableSize ;
     WORD  wBitCount ;

     if (GetDibInfoHeaderSize (lpDib) == sizeof (BITMAPCOREHEADER))
          {
          wBitCount = ((BITMAPCOREHEADER huge *) lpDib)->bcBitCount ;

          if (wBitCount != 24)
               dwNumColors = 1L << wBitCount ;
          else
               dwNumColors = 0 ;

          dwColorTableSize = dwNumColors * sizeof (RGBTRIPLE) ;
          }
     else
          {
          wBitCount = ((BITMAPINFOHEADER huge *) lpDib)->biBitCount ;

          if (GetDibInfoHeaderSize (lpDib) >= 36)
               dwNumColors = ((BITMAPINFOHEADER huge *) lpDib)->biClrUsed ;
          else
               dwNumColors = 0 ;

          if (dwNumColors == 0)
               {
               if (wBitCount != 24)
                    dwNumColors = 1L << wBitCount ;
               else
                    dwNumColors = 0 ;
               }

          dwColorTableSize = dwNumColors * sizeof (RGBQUAD) ;
          }

     return lpDib + GetDibInfoHeaderSize (lpDib) + dwColorTableSize ;
     }

WORD GetDibWidth (BYTE huge * lpDib)
     {
     if (GetDibInfoHeaderSize (lpDib) == sizeof (BITMAPCOREHEADER))
          return (WORD) (((BITMAPCOREHEADER huge *) lpDib)->bcWidth) ;
     else
          return (WORD) (((BITMAPINFOHEADER huge *) lpDib)->biWidth) ;
     }

WORD GetDibHeight (BYTE huge * lpDib)
     {
     if (GetDibInfoHeaderSize (lpDib) == sizeof (BITMAPCOREHEADER))
          return (WORD) (((BITMAPCOREHEADER huge *) lpDib)->bcHeight) ;
     else
          return (WORD) (((BITMAPINFOHEADER huge *) lpDib)->biHeight) ;
     }


		

void DrawBitmapBitBlt (	HDC hdc,
			HBITMAP hBitmap,
			short xStart,
			short yStart
		      )
	{
	BITMAP bm ;
	HDC hdcMem ;
	DWORD dwSize ;
	POINT ptSize, ptOrg ;

	hdcMem = CreateCompatibleDC ( hdc ) ;
	SelectObject ( hdcMem, hBitmap ) ;
	
	SetMapMode ( hdcMem, GetMapMode ( hdc ) ) ;
	GetObject ( hBitmap, sizeof ( BITMAP ), ( LPSTR ) &bm ) ;
	ptSize.x = bm.bmWidth ;
	ptSize.y = bm.bmHeight ;
	
	DPtoLP ( hdc, &ptSize, 1 ) ;
	


	

	ptOrg.x = 0 ;
	ptOrg.y = 0 ;
	DPtoLP ( hdcMem, &ptOrg, 1 ) ;

	BitBlt (
			hdc,
			xStart,
			yStart,
			ptSize.x,
			ptSize.y,
			hdcMem,
			ptOrg.x,
			ptOrg.y,
			SRCCOPY ) ;

			 
	DeleteDC ( hdcMem ) ;
	}



void DrawBitmapStretchBlt (	HDC hdc,
				HBITMAP hBitmap,
				short xStart,
				short yStart,
				POINT ptSizeWindow )
	{
	BITMAP bm ;
	HDC hdcMem ;
	DWORD dwSize ;
	POINT ptSize, ptOrg ;

	hdcMem = CreateCompatibleDC ( hdc ) ;
	SelectObject ( hdcMem, hBitmap ) ;
	
	SetMapMode ( hdcMem, GetMapMode ( hdc ) ) ;
	GetObject ( hBitmap, sizeof ( BITMAP ), ( LPSTR ) &bm ) ;
	ptSize.x = bm.bmWidth ;
	ptSize.y = bm.bmHeight ;
	
	DPtoLP ( hdc, &ptSize, 1 ) ;
	


	

	ptOrg.x = 0 ;
	ptOrg.y = 0 ;
	DPtoLP ( hdcMem, &ptOrg, 1 ) ;

        DPtoLP ( hdc, &ptSizeWindow, 1 ) ;
	StretchBlt ( 	hdc,
			xStart,
			yStart,
			ptSizeWindow.x,
                        ptSizeWindow.y,
			hdcMem,
			ptOrg.x,
			ptOrg.y,
			ptSize.x,
			ptSize.y,
			SRCCOPY ) ;

			 
	DeleteDC ( hdcMem ) ;
	}


void Essai (	HDC hdc,
				HBITMAP hBitmap,
				short xStart,
				short yStart,
				POINT ptSizeWindow )
	{
	BITMAP bm ;
	HDC hdcMem ;
	DWORD dwSize ;
	POINT ptSize, ptOrg ;

	hdcMem = CreateCompatibleDC ( hdc ) ;
	SelectObject ( hdcMem, hBitmap ) ;
	
	SetMapMode ( hdcMem, GetMapMode ( hdc ) ) ;
	GetObject ( hBitmap, sizeof ( BITMAP ), ( LPSTR ) &bm ) ;
	ptSize.x = bm.bmWidth ;
	ptSize.y = bm.bmHeight ;
	
	DPtoLP ( hdc, &ptSize, 1 ) ;
	


	

	ptOrg.x = 0 ;
	ptOrg.y = 0 ;
	DPtoLP ( hdcMem, &ptOrg, 1 ) ;

        DPtoLP ( hdc, &ptSizeWindow, 1 ) ;
	StretchBlt ( 	hdc,
			xStart,
			yStart,
			ptSizeWindow.x,
                        ptSizeWindow.y,
			hdcMem,
			ptOrg.x,
			ptOrg.y,
			ptSize.x,
			ptSize.y,
			SRCCOPY ) ;

	
	DeleteDC ( hdcMem ) ;
	}


BYTE calcul ( BYTE couleur, short pourcentage )
	{
	short reste ;

	if ( pourcentage < 0 )
		reste = couleur + 1 ;
	else
		reste = 255 - couleur ;

	return ( couleur + ( reste * pourcentage ) / 100 ) ;
	}
  
void ChangeColorRGBQUAD ( RGBQUAD huge * rgbquad,
				RGBQUAD huge * CouleursInitiales,
				short PourcentageRouge,
				short PourcentageVert,
				short PourcentageBleu,
				short mode )
	{
	//rgbquad->rgbRed = rgbquad->rgbRed + additif_couleur ( rgbquad->rgbRed, PourcentageRouge ) ;//( rgbquad->rgbRed * PourcentageRouge ) / 100 ;
//	rgbquad->rgbGreen = rgbquad->rgbGreen + additif_couleur ( rgbquad->rgbGreen, PourcentageVert ) ; //( rgbquad->rgbGreen * PourcentageVert ) / 100 ;
//	rgbquad->rgbBlue = rgbquad->rgbBlue + additif_couleur ( rgbquad->rgbBlue, PourcentageBleu ) ; //( rgbquad->rgbBlue * PourcentageBleu ) / 100 ;
	switch ( mode ) {
	  case IDD_SATURATION :
	    rgbquad->rgbRed = MIN ((int)CouleursInitiales->rgbRed + (int)PourcentageRouge, 255 ) ;//( rgbquad->rgbRed * PourcentageRouge ) / 100 ;
	    rgbquad->rgbRed = MAX ((int)rgbquad->rgbRed + (int)PourcentageRouge, 0 ) ;//( rgbquad->rgbRed * PourcentageRouge ) / 100 ;
	    rgbquad->rgbGreen =MIN ((int)CouleursInitiales->rgbGreen +(int) PourcentageVert, 255 ) ;//( rgbquad->rgbRed * PourcentageRouge ) / 100 ;
	    rgbquad->rgbGreen = MAX ((int)rgbquad->rgbGreen +(int) PourcentageVert, 0 ) ;//( rgbquad->rgbRed * PourcentageRouge ) / 100 ;
	    rgbquad->rgbBlue = MIN ((int)CouleursInitiales->rgbBlue +(int) PourcentageBleu, 255 ) ;//( rgbquad->rgbRed * PourcentageRouge ) / 100 ;
            rgbquad->rgbBlue = MAX ((int)rgbquad->rgbBlue +(int) PourcentageBleu, 0 ) ;//( rgbquad->rgbRed * PourcentageRouge ) / 100 ;
	    break ;
	  case IDD_POURCENTAGE :
	    rgbquad->rgbBlue = calcul ( CouleursInitiales->rgbBlue, PourcentageBleu ) ;
	    rgbquad->rgbGreen = calcul ( CouleursInitiales->rgbGreen, PourcentageVert ) ;
	    rgbquad->rgbRed = calcul ( CouleursInitiales->rgbRed, PourcentageRouge ) ;
	    break ;
          }
        }

void ChangeCouleurRGB ( COULEURRGB  * rgb,
				unsigned short PourcentageRouge,
				unsigned short PourcentageVert,
				unsigned short PourcentageBleu )
	{
	rgb->cRed = ( ( unsigned long )rgb->cRed * ( unsigned long )PourcentageRouge ) / 100 ;
	rgb->cGreen = ( ( unsigned long )rgb->cGreen * ( unsigned long )PourcentageVert ) / 100 ;
	rgb->cBlue = ( ( unsigned long )rgb->cBlue * ( unsigned long )PourcentageBleu ) / 100 ;
	}

short additif_couleur ( unsigned short couleur, short pourcentage )
	{
	//#define additif_couleur ( couleur, pourcentage ) (  (pourcentage) <= 0 ? ( ( ( (couleur) + 1 ) * (pourcentage) ) / 100 ) : ( ( ( 255 - (couleur) ) * (pourcentage) ) / 100 ) )
	if ( pourcentage <= 0 )
		return ( ( ( couleur+1 ) * pourcentage ) / 100 ) ;
	else
		return ( ( ( 255 - couleur ) * pourcentage ) / 100 ) ;
	}