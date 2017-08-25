/*programme d'autocorr‚lation d'une sinuso‹de */
/****************************************************************************
*	FICHIERS INCLUDE
****************************************************************************/
#include <dos.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <graphics.h>

#define N 160
#define f 153
#define fe 8000
#define P 640
/****************************************************************************
*	DECLARATIONS DE VARIABLES
****************************************************************************/
int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
int    ErrorCode;		/* Reports any graphics errors		*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
struct palettetype palette;		/* Used to read palette info	*/
double AspectRatio;		/* Aspect ratio of a pixel on the screen*/


void initial();
void calcul_autocorrelation();
double maximum();
void tracex();
void tracecor();

/****************************************************************************
*	DECLARATIONS DE FONCTIONS
****************************************************************************/
void Initialize();

main()
{
	double x[P], autocor[P];

	FILE *pfile;
	int k;
	double maxy1;
	double maxy2;
	char c;


	initial( x, autocor );
	for ( k=0; k<4; k++)
	{
		calcul_autocorrelation( &x[160*k], &autocor[160*k] );
	}

	/*calcul_autocorrelation( x, autocor );*/

	/*printf("\nSauvegarde des r‚sultats");
	pfile = fopen( "results", "wb" );
	rewind ( pfile );

	for( k=0; k<N; k++ )
	{
		fprintf( pfile, "%lf\t%lf\n\r", x[k], autocor[k] );
	}

	fclose( pfile );
	*/
/****************************************************************************
*	PROGRAMME PRINCIPAL
****************************************************************************/
/*main()
{       */
	Initialize(); 		/* Set system into Graphics mode	*/
	cleardevice();			/* Clear graphics screen	*/
	setviewport( 0, 0, MaxX, MaxY, 1 );	/* Open port to full screen	*/
	/*outtext("bonjour");*/
	for(k=0;k<5;k++)
	line(k*160,0,k*160,MaxY);
	line(0,MaxY/4,MaxX,MaxY/4);
	line(0,3*MaxY/4,MaxX,3*MaxY/4);
	maxy1 = maximum( x );
	maxy2 = maximum( autocor );
	/*if (maxy2>maxy1)
		maxy1=maxy2;
	else
		maxy2=maxy1;*/
	tracex( x, maxy1, (double)MaxX, (double)MaxY );
	tracecor( autocor, maxy2, (double)MaxX, (double)MaxY );
	sleep(10);
	closegraph(); 		/* Return the system to text mode	*/
/*}*/
}

double maximum( vecteur )
double vecteur[P];
{
	int k;
	double max;
	max = 0;
	for ( k=0; k<P; k++ )
		if (fabs(vecteur[k]) > max )
			max = fabs(vecteur[k]);
	return( max );
}

void tracex( vecteur, maxiy, maxx, maxy )
double vecteur[P],maxiy,maxx,maxy;
{
	int k;
	moveto(0,maxy/4);
	for(k=0;k<P;k++)
	lineto(k,(int)ceil((-vecteur[k]*maxy/(6*maxiy))+(maxy/4)));
	/*putpixel(k,(int)ceil((-vecteur[k]*maxy/(6*maxiy))+(maxy/4)) ,1);
	putpixel((int)ceil(k*maxx/P),(int)ceil((vecteur[k]*maxy/maxiy)+(maxy/2)) ,1);*/
	/*putpixel((int)ceil(k*P/maxx),(int)ceil(vecteur[k]+(maxy/2)) ,1);*/
}
void tracecor( vecteur, maxiy, maxx, maxy )
double vecteur[P],maxiy,maxx,maxy;
{
	int k;
	moveto(0,3*maxy/4);
	for(k=0;k<P;k++)
	{
	lineto(k,(int)ceil((-vecteur[k]*maxy/(6*maxiy))+(3*maxy/4)));
	/*putpixel(k,(int)ceil((-vecteur[k]*maxy/(6*maxiy))+(3*maxy/4)) ,1);
	*/
	}
	/*putpixel((int)ceil(k*maxx/P),(int)ceil((vecteur[k]*maxy/maxiy)+(maxy/2)) ,1);*/
	/*putpixel((int)ceil(k*P/maxx),(int)ceil(vecteur[k]+(maxy/2)) ,1);*/
}
void initial( vecteur1, vecteur2 )
double vecteur1[P], vecteur2[P];
{
	int k;
	printf("\nInitialisation des vecteurs");
	for( k=0; k<P; k++ )
	{
		vecteur1[k] = sin ( 2*3.1415926536*f*k/fe );
		vecteur2[k] = 0;
	}
}

void calcul_autocorrelation( signal, autocorrelation )
double signal[N], autocorrelation[N];
{
	int k, n ;
	printf("\nCalcul de la fonction d'autocorr‚lation");
	for( k=0; k<N; k++ )
	{
		for( n=0; n<N-k; n++ )
			autocorrelation[k] = autocorrelation[k] + signal[n]*signal[n+k];
		/*autocorrelation[k] = autocorrelation[k]/(N-k);*/
	}
}






/****************************************************************************
*	INITIALISATION GRAPHIQUE :INITIALIZE()
****************************************************************************/
/*	INITIALIZE: Initializes the graphics system and reports 	*/
/*	any errors which occured.					*/

void Initialize(void)
{
  int xasp, yasp;			/* Used to read the aspect ratio*/

  GraphDriver = DETECT; 		/* Request auto-detection	*/
  initgraph( &GraphDriver, &GraphMode, "" );
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk ){		/* Error occured during init	*/
    printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
    exit( 1 );}
  getpalette( &palette );		/* Read the palette from board	*/
  MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/

  MaxX = getmaxx();
  MaxY = getmaxy();			/* Read size of screen		*/

  getaspectratio( &xasp, &yasp );	/* read the hardware aspect	*/
  AspectRatio = (double)xasp / (double)yasp; /* Get correction factor	*/

}


/****************************************************************************
*	REMARQUES
****************************************************************************/
/*
void closegraph(void) :referme le systŠme graphique.

getpalette donne des informations sur la palette courante.

getmaxcolor renvoie le plus grand num‚ro de couleur.

getmaxx,getmaxy.

getaspectratio retourne les coeff. d' aspect du mode graphique courant.
arc et les autres fonctions utilisent ce coeff d' aspect pour produire
des cercles ronds et non ovales.

yasp est toujours = 10000.
il faut prendre xasp=10000 lorsque les pixels sont carr‚s.
____________________<10000 _______________________ + hauts que large.

cleardevice efface l'‚cran graphique.

setviewport(left,top,right,bottom,clip) d‚finit la fenˆtre graphique
courante pour les sorties graphiques.

outtext: affiche du texte dans la fenˆtre graphique.

on ne peut pas imprimer un texte sans etre sorti du mode graphique.

L 'initialisation donne		graphdriver=7
				graphmode=0
				palette=16
				MaxColors=2
				MaxX=719
				MaxY=347
				xasp=7500
				yasp=10000

.POURQUOI?

*/