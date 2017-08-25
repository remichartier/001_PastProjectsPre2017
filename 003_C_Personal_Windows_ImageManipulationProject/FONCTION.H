// fichier de declaration des fonctions de fonction.c pour image1.c
#include <windows.h>

typedef struct CouleurRgb
{
	BYTE cRed ;
	BYTE cGreen ;
	BYTE cBlue ;
} COULEURRGB ;
DWORD GetDibInfoHeaderSize (BYTE huge * lpDib) ;

BYTE huge * ReadDib (char * szFileName) ;

BYTE huge * GetDibBitsAddr (BYTE huge * lpDib) ;
     
WORD GetDibWidth (BYTE huge * lpDib) ;

WORD GetDibHeight (BYTE huge * lpDib) ;

void DrawBitmapBitBlt (	HDC hdc,
			HBITMAP hBitmap,
			short xStart,
			short yStart
		      ) ;

void DrawBitmapStretchBlt (	HDC hdc,
				HBITMAP hBitmap,
				short xStart,
				short yStart,
				POINT ptSizeWindow
			  ) ;

void Essai (	HDC hdc,
				HBITMAP hBitmap,
				short xStart,
				short yStart,
				POINT ptSizeWindow
			  ) ;


void ChangeColorRGBQUAD ( RGBQUAD  huge * rgbquad,
				 RGBQUAD  huge * CouleursInitiales,
				 short PourcentageRouge,
				 short PourcentageVert,
				 short PourcentageBleu,
				 short mode ) ;

void ChangeCouleurRGB ( COULEURRGB  * rgb,
				unsigned short PourcentageRouge,
				unsigned short PourcentageVert,
				unsigned short PourcentageBleu ) ;

short additif_couleur (unsigned short couleur,	short pourcentage) ;