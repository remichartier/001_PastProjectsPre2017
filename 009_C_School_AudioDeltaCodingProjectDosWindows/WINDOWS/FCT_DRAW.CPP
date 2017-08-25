// FICHIER FCT_DRAW.CPP
// Ce fichier contient les fonctions qui permettent de dessiner


#include "classes.h"
#define TAILLEMAX 1000

/**************************************************************************
* La fonction PAINT permet de tracer les axes et les signaux sinusoidaux  *
*    des fichiers source et des fichires restitues                        *
**************************************************************************/
void TMyDrawWindow::Paint(HDC PaintDC, PAINTSTRUCT& )
{
  RECT myrect;
  POINT pt[TAILLEMAX];
  long taille;
  extern FILE *file_source, *file_dec, *file_cod;
  FILE *pfile;

// recuperation du contexte graphique
PaintDC = GetDC(HWindow);
// recuperation des dimensions de la fenetre
GetClientRect(HWindow, &myrect);

// on designe les bons pointeurs selon que l'on veut visuliser le fichier
//      source ou le fichier restitue
if (Source) pfile=file_source;
if (Dest) pfile=file_dec;

//Tracé des axes
MoveTo(PaintDC,20,myrect.top+10);
LineTo(PaintDC,20,myrect.bottom-10);
MoveTo(PaintDC,15,(myrect.bottom + myrect.top)/2);
LineTo(PaintDC,myrect.right -10,(myrect.bottom+ myrect.top)/2);
    // flèche axe des abscisses
MoveTo(PaintDC,myrect.right -15, ((myrect.bottom+ myrect.top)/2)-5);
LineTo(PaintDC,myrect.right -10,(myrect.bottom+ myrect.top)/2);
LineTo(PaintDC,myrect.right -15,((myrect.bottom+ myrect.top)/2)+5);
    // flèche axe des ordonnées
MoveTo(PaintDC,15,myrect.top+15);
LineTo(PaintDC,20,myrect.top+10);
LineTo(PaintDC,25,myrect.top+15);
    // graduations
MoveTo(PaintDC,15,myrect.top+20);
LineTo(PaintDC,25,myrect.top+20);
MoveTo(PaintDC,15,myrect.bottom-20);
LineTo(PaintDC,25,myrect.bottom-20);
Ellipse(PaintDC,12,((myrect.bottom+ myrect.top)/2)-5,17,((myrect.bottom+ myrect.top)/2)-11);
//Trace de la courbe
lit_sinus(pfile, pt, &taille, &myrect);
Polyline(PaintDC, pt, (int)taille);
// liberation du contexte graphique
ReleaseDC(HWindow, PaintDC);
};

/******************************************************************
*  fonction qui permet de quitter le fenetre                      *
*******************************************************************/

BOOL TMyDrawWindow::CanClose()
{
  if (MessageBox(HWindow,"","Quitter?",MB_YESNO)==IDYES) return TRUE;
  else return FALSE;
};

/******************************************************************
*  fonction qui permet de lire la longueur d'un bloc              *
*******************************************************************/
double TMyDrawWindow::lire_longueur_bloc(FILE *file)
{
	double position;
	double longueur=0;
	int octet[3];
	int i=0;
	position=ftell(file);
	/* positionnement 1er octet de longueur */
	fseek(file,1,1);
	while(i<3)
	{	octet[i]=getc(file);
		i++;
	}
	longueur=(double)octet[2]*65536;
	longueur+=(double)octet[0];
	longueur+=256*(double)octet[1];
	/* repositionnement debut bloc */
	fseek(file,position,0);
	return(longueur);
}

/*****************************************************************
* fonction qui permet de lire lee numero du bloc                 *
******************************************************************/
char TMyDrawWindow::lire_no_bloc(FILE *file)
{
	char numero;
	double position;
	position=ftell(file);
	numero=getc(file);
	fseek(file,position,0);	      /* se remet début bloc */
	return(numero);
}

/*****************************************************************
*   fonction qui permet de recuperer les points d'une sinusoide  *
*           dans un fichier .VOC                                 *
******************************************************************/ 

void TMyDrawWindow::lit_sinus(FILE *pfile, POINT *pt, long *taille, RECT *myrect)
{
int i, j, x0, y0;
short repet;
float pas, hauteur, coef_y;
char num;
unsigned char c;
double longueur;

x0=(*myrect).left+20;                      // (x0,y0) est l'origine des axes
y0=((*myrect).top+(*myrect).bottom)/2;
hauteur=(*myrect).bottom-(*myrect).top;
coef_y=(hauteur-40)/255;

fseek(pfile,26+4,0);                      // on passe l'entete et les 4 premiers bits du bloc 6
fread(&repet,sizeof(short),1,pfile);      // on lit le nomnbre de repet
longueur=lire_longueur_bloc(pfile)-2; //on ne compte pas SR ni compactage
(*taille)=(longueur * (repet+1));
pas=((*myrect).right-(*myrect).left-20)/(*taille);
fseek(pfile,6,1);
for (i=0;i<longueur;i++)
{
	if (i==0)
		 pt[i].x=x0;
	else
		 pt[i].x=(pt[i-1].x) + pas;
	fread(&c,sizeof(char),1,pfile);
	pt[i].y=(coef_y*(128-(int)c)+y0);
}

for (j=0;j<repet;j++)
{
	for (i=0;i<longueur;i++)
		{
		 pt[(longueur)*(j+1)+i].x=ceil(pt[(longueur)*(j+1)+i-1].x) + pas;
		 pt[(longueur)*(j+1)+i].y=pt[i].y;
		}
}

} 


