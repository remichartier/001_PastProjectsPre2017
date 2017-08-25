/******exemple de programme de fractales**********/
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <signal.h>
#include <conio.h>
#include <math.h>


#define COLONNE	0
#define LIGNE	1
#define MAX	129	/* doit etre du genre de 2exposant n +1	*/

struct POINT
	{
		int x;
		int y;
		struct POINT *suivant;
	};

typedef struct POINT point;
typedef struct POINT *pointeur_point;

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
/****************************************************************************
*	DECLARATIONS DE FONCTIONS
****************************************************************************/
void initialize();
void rempli_ecran_au_hasard_noir_et_blanc();
void rempli_ecran_par_ligne_noir_et_blanc();
void fractale_triangle();
void inserer_noeud_point();
void creer_noeud_point();
void initialiser_noeud_point();
void tracer_triangle();
void paysage_fractal();
int chercher_prochain_pt_sur_ligne();
int chercher_prochain_pt_sur_colonne();
void quadrilatere();



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
*	PROGRAMME PRINCIPAL
****************************************************************************/
main()
{

	Initialize(); 		/* Set system into Graphics mode	*/
	cleardevice();			/* Clear graphics screen	*/
	setviewport( 0, 0, MaxX, MaxY, 1 );	/* Open port to full screen	*/
	/*rempli_ecran_au_hasard_noir_et_blanc();*/
	/*rempli_ecran_par_ligne_noir_et_blanc(5);*/
	/*fractale_triangle();*/
	paysage_fractal();
	closegraph(); 		 /*Return the system to text mode	*/
}	/*fin du main*/

/****************************************************************************
*	FONCTION REMPLI_ECRAN_AU_HASARD_NOIR_ET_BLANC
****************************************************************************/

void rempli_ecran_au_hasard_noir_et_blanc()
{

        unsigned long i;
	int x,y;
	randomize();
	for(i=0;i<MaxX*MaxY-1;i++)
	{
		x=random(MaxX);
		y=random(MaxY);
		if(getpixel(x,y)==0)
			putpixel(x,y,1);
		else
			i--;
		if(kbhit()!=0)
		{
			i=getchar();
			i=MaxX*MaxY;
		}			/* fin du if			*/
	 }				/* fin du for			*/
}					/* fin fonction			*/

/****************************************************************************
*	FONCTION REMPLI_ECRAN_PAR_LIGNE_NOIR_ET_BLANC
****************************************************************************/

void rempli_ecran_par_ligne_noir_et_blanc(pas)
int pas;
{
	unsigned int i,j;
	for(i=0;i<MaxX;i=i+pas)
	{
		for(j=0;j<MaxY;j=j+pas)
		{
			putpixel(i,j,1);
			if(kbhit()!=0)
				{
				i=getchar();
				i=MaxX;
				j=MaxY;
				}
		}			/* fin du for			*/
	 }				/* fin du for			*/
}					/* fin fonction			*/

/****************************************************************************
*	FONCTION FRACTALE_TRIANGLE
****************************************************************************/
void fractale_triangle()
{
	pointeur_point tete,pointeur_depart,
		pointeur_arrivee,pointeur1,pointeur2,pointeur3;
	point p1,p2,p3;

	creer_noeud_point(&tete);
	initialiser_noeud_point(tete,0,0,NULL);
	creer_noeud_point(&tete->suivant);
	initialiser_noeud_point(tete->suivant,MaxX,MaxY,NULL);
	line((int)tete->x,(int)(tete->y)*AspectRatio,(int)tete->suivant->x,(int)(tete->suivant->y)*AspectRatio);
	while(getch()!='q')
	{
		pointeur_depart=tete;
		while(pointeur_depart->suivant!=NULL)
		{
                	pointeur_arrivee=pointeur_depart->suivant;
			p1.x=pointeur_arrivee->x/3+2*(pointeur_depart->x)/3;
			p1.y=(int)(pointeur_arrivee->y/3+2*(pointeur_depart->y)/3);
			p3.x=2*(pointeur_arrivee->x)/3+pointeur_depart->x/3;
			p3.y=2*(pointeur_arrivee->y)/3+pointeur_depart->y/3;
			p2.x=.5*(p3.x+p1.x)+.5*(sqrt(3))*(p3.y-p1.y);
			p2.y=-.5*(sqrt(3))*(p3.x-p1.x)+.5*(p1.y+p3.y);

			inserer_noeud_point(pointeur_depart,p1);
			pointeur1=pointeur_depart->suivant;
			inserer_noeud_point(pointeur1,p2);
			pointeur2=pointeur1->suivant;
			inserer_noeud_point(pointeur2,p3);
			pointeur3=pointeur2->suivant;

			tracer_triangle(pointeur1,pointeur2,pointeur3);

			pointeur_depart=pointeur_arrivee;
		}			/* fin du while			*/
	}                           	/* fin du while			*/
}





/****************************************************************************
*	FONCTION INSERER_NOEUD_POINT
****************************************************************************/
void inserer_noeud_point(pointeur,pt)
pointeur_point pointeur;
point pt;
{
	pointeur_point passage;
	passage=pointeur->suivant;
	creer_noeud_point(&pointeur->suivant);
	initialiser_noeud_point(pointeur->suivant,
					pt.x,
					pt.y,
					passage);
}

/****************************************************************************
*	FONCTION CREER_NOEUD_POINT
****************************************************************************/
void creer_noeud_point(p)
pointeur_point *p;
{
	*p=malloc(sizeof(point));
}
/****************************************************************************
*	FONCTION INITIALISER_NOEUD_POINT
****************************************************************************/
void initialiser_noeud_point(pointeur,x,y,suivant)
int x,y;
pointeur_point pointeur,suivant;
{
	pointeur->x=x;
	pointeur->y=y;
	pointeur->suivant=suivant;
}

/****************************************************************************
*	FONCTION TRACER_TRIANGLE
****************************************************************************/
void tracer_triangle(p1,p2,p3)
pointeur_point p1,p2,p3;
{
	setcolor(0);
	line((int)p1->x,(int)(p1->y)*AspectRatio,(int)p3->x,(int)(p3->y)*AspectRatio);
	setcolor(1);
	line((int)p1->x,(int)(p1->y)*AspectRatio,(int)p2->x,(int)(p2->y)*AspectRatio);
	line((int)p2->x,(int)(p2->y)*AspectRatio,(int)p3->x,(int)(p3->y)*AspectRatio);
}
/****************************************************************************
*	FONCTION PAYSAGE_FRACTAL
****************************************************************************/
void paysage_fractal()
{
	int altitude[MAX][MAX];
	int i;
	int j;
	int x1,x2,x,y;
	int pas;
	float delta_h,delta_v;
	scanf("\n%f",&delta_h);
	scanf("\n%f",&delta_v);
	cleardevice();			/* Clear graphics screen	*/
						/* initialisation*/
	for(i=0;i<MAX;i++)
	{for(j=0;j<MAX;j++)
		altitude[i][j]=-1;
	}
				/* altitude de quatre premiers points	*/
	randomize();
	altitude[0][MAX-1]=random(5);
	altitude[MAX-1][0]=random(3);
	altitude[0][0]=random(5);
	altitude[MAX-1][MAX-1]=random(3);
	quadrilatere(0,0,altitude[0][0],
		0,MAX-1,altitude[0][MAX -1],
		MAX-1,MAX-1,altitude[MAX-1][MAX-1],
		MAX-1,0,altitude[MAX-1][0]);
	while(getch()!='q')
	{
		cleardevice();
		i=0;
		j=-1;
		x1=chercher_prochain_pt_sur_ligne(i,j,altitude);
		x2=chercher_prochain_pt_sur_ligne(i,x1,altitude);
		pas=x2-x1;
		j=0;
		x=x1;
		y=0;
		while(i<((MAX-1)/pas))
		{
			while(j<((MAX-1)/pas))
			{
				if(altitude[x][y+.5*pas]==-1)
				altitude[x][y+.5*pas]=delta_v*pas*random(2)+.5*(altitude[x][y]+altitude[x][y+pas]);
				if(altitude[x+pas][y+.5*pas]==-1)
				altitude[x+pas][y+.5*pas]=delta_v*pas*random(2)+.5*(altitude[x+pas][y]+altitude[x+pas][y+pas]);
				if(altitude[x+pas/2][y]==1)
				altitude[x+pas/2][y]=delta_h*pas*random(2)+.5*(altitude[x][y]+altitude[x+pas][y]);
				if(altitude[x+pas/2][y+pas]==-1)
				altitude[x+pas/2][y+pas]=delta_h*pas*random(2)+.5*(altitude[x+pas][y+pas]+altitude[x][y+pas]);
				altitude[x+pas/2][y+pas/2]=.5*(delta_v+delta_h)*pas*random(2)+.25*(altitude[x+pas][y+pas/2]+altitude[x][y+pas/2]+
					altitude[x+pas/2][y]+altitude[x+pas/2][y+pas]);
				quadrilatere(x,y,altitude[x][y],
					x,y+pas/2,altitude[x][y+pas/2],
					x+pas/2,y+pas/2,altitude[x+pas/2][y+pas/2],
					x+pas/2,y,altitude[x+pas/2][y]);
				quadrilatere(x+pas/2,y,altitude[x+pas/2][y],
					x+pas,y,altitude[x+pas][y],
					x+pas,y+pas/2,altitude[x+pas][y+pas/2],
					x+pas/2,y+pas/2,altitude[x+pas/2][y+pas/2]);
				quadrilatere(x,y+pas/2,altitude[x][y+pas/2],
					x+pas/2,y+pas/2,altitude[x+pas/2][y+pas/2],
					x+pas/2,y+pas,altitude[x+pas/2][y+pas],
					x,y+pas,altitude[x][y+pas]);
				quadrilatere(x+pas/2,y+pas/2,altitude[x+pas/2][y+pas/2],
					x+pas,y+pas/2,altitude[x+pas][y+pas/2],
					x+pas,y+pas,altitude[x+pas][y+pas],
					x+pas/2,y+pas,altitude[x+pas/2][y+pas]);
				x=x+pas;
				j++;
			}		/* fin while			*/
			y=y+pas;
			x=0;
			j=0;
			i++;
		}			/* fin while			*/
	}				/* fin while			*/


}				/* fin paysage_fractal			*/




/****************************************************************************
*	FONCTION CHERCHER_PROCHAIN_PT_SUR_LIGNE
****************************************************************************/
int chercher_prochain_pt_sur_ligne(int ligne,int colonne, int tableau[][MAX])
{
	colonne++;
	while((tableau[ligne][colonne]==-1)&&(colonne<MAX))
		colonne++;
	if(colonne!=MAX)
		return(colonne);
	else
		return(-1);
}

/****************************************************************************
*	FONCTION CHERCHER_PROCHAIN_PT_SUR_COLONNE
****************************************************************************/
int chercher_prochain_pt_sur_colonne(int colonne,int ligne,int tableau[][MAX])
{
	ligne++;
	while((tableau[ligne][colonne]==-1)&&(ligne!=MAX))
		ligne++;
	if(ligne!=MAX)
		return(ligne);
	else
		return(-1);
}
/****************************************************************************
*	FONCTION LIGNE
****************************************************************************/
void quadrilatere(x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4)
int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
{
	int decalage_x=100;
	int decalage_y=175;
	float facteur=2.5;
	double cosi=.5*sqrt(2);
	double sinu=-.5*(sqrt(2));
	int polypoints[8];
	AspectRatio=0.62;
	setfillstyle(SOLID_FILL,0);

	polypoints[0]=decalage_x+facteur*(x1*cosi-y1*sinu);
	polypoints[2]=decalage_x+facteur*(x2*cosi-y2*sinu);
	polypoints[4]=decalage_x+facteur*(x3*cosi-y3*sinu);
	polypoints[6]=decalage_x+facteur*(x4*cosi-y4*sinu);
	polypoints[1]=decalage_y+facteur*(x1*sinu+y1*cosi-z1)*AspectRatio;
	polypoints[3]=decalage_y+facteur*(x2*sinu+y2*cosi-z2)*AspectRatio;
	polypoints[5]=decalage_y+facteur*(x3*sinu+y3*cosi-z3)*AspectRatio;
	polypoints[7]=decalage_y+facteur*(x4*sinu+y4*cosi-z4)*AspectRatio;
	fillpoly(4,polypoints);
	/*line(decalage_x+facteur*(x*cosi-y*sinu),
		decalage_y+facteur*(x*sinu+y*cosi-z)*AspectRatio,
		decalage_x+facteur*(x1*cosi-y1*sinu),
		decalage_y+facteur*AspectRatio*(x1*sinu+y1*cosi-z1));*/
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