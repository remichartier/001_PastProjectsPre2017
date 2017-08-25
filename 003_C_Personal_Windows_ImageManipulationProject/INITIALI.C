#include <stdio.h>
#include <graphics.h>

int GraphDriver;            /*the graphics device driver           */
int GraphMode;		    /*the graphics mode values             */
int ErrorCode;		    /*reports any graphics errors	   */
int MaxColors;              /*the max of colors available          */
int MaxX,MaxY;		    /*the maximum resolution of the screen */
struct palettetype palette; /*used to read palette info            */
double AspectRatio;	    /*aspect ratio of a pixel on the screen*/


void Initialize()                       /*set system into graphics mode */
{
  int xasp, yasp;			/* Used to read the aspect ratio*/

  GraphDriver = DETECT; 		/* Request auto-detection	*/
  initgraph( &GraphDriver, &GraphMode, "" );
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk ){		/* Error occured during init	*/
    printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
    exit( 1 );
}
  getpalette( &palette );		/* Read the palette from board	*/
  MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/

  MaxX = getmaxx();
  MaxY = getmaxy();			/* Read size of screen		*/

  getaspectratio( &xasp, &yasp );	/* read the hardware aspect	*/
  AspectRatio = (double)xasp / (double)yasp; /* Get correction factor	*/
}

