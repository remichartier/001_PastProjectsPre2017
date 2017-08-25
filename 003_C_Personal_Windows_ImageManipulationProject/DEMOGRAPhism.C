#include <stdio.h>
#include <graphics.h>

#define MAXPOLY		50	    /*nbre de points max du polygone	   */

int GraphDriver;       		  	   /*the graphics device driver            */
int GraphMode;			  	   /*the graphics mode values              */
int ErrorCode;			  	   /*reports any graphics errors	   */
int MaxColors;      	          	   /*the max of colors available           */
int MaxX,MaxY;			    	   /*the maximum resolution of the screen  */
struct palettetype palette;		   /*used to read palette info             */
double AspectRatio;		    	   /*aspect ratio of a pixel on the screen */
int x,y,radius;             	   	   /*cercle				   */
int l,t,r,b;            	   	   /*barre 				   */
int le,to,ri,bo,de,tf;  	    	   /*barre3d				   */
int numpoints,polypoints[2*MAXPOLY];       /*polygone				   */
int x1,y1,stangle,endangle,xradius,yradius;/*ellipse                               */
int x2,y2,border;			   /*floodfil				   */
int x3,y3,x4,y4;			   /*ligne				   */
int dx,dy;				   /*lignerel				   */
char far *textstring;                      /*outstring				   */
char a[MAXPOLY];                           /*outstring				   */
int x5,y5,stangle1,endang1,radius1;	   /*sectangle				   */
int left1,top1,right1,bottom1,clip1;	   /*fenetre				   */


void Initialize();
void barre();
void barre3D();
void cercle();
void polygone();
void elliptique();
void floodfil();
void ligne();
void lignerel();
void sectangle();
void fenetre();

main()
{
	/*cercle();
	barre();
	barre3D();
	polygone();
	elliptique();
	floodfil();
	ligne();
	lignerel();
	sectangle();*/
	fenetre();
	Initialize();
	/*circle(x,y,radius);
	bar(l,t,r,b);
	bar3d(le,to,ri,bo,de,tf);
	drawpoly(numpoints,polypoints);
	fillpoly(numpoints,polypoints);
	ellipse(x1,y1,stangle,endangle,xradius,yradius);
	fillellipse(x1,y1,xradius,yradius);
	floodfill(x2,y2,border);
	line(x3,y3,x4,y4);
	linerel(dx,dy);*/
	strcpy(a,"remi");
	textstring = a;
	setviewport(left1,top1,right1,bottom1,clip1);
        outtext(textstring);
	/*cleardevice();
	closegraph();*/
}
/**************************************************************************/
/*remarque:pour toutes les fonctions graphiques,...appeler les fonctions
 puis ensuite les executer dans le programme principal comme indiqu‚*/
/**************************************************************************/
void cercle()
{

	printf("\nEntrer x ");
	scanf("%d",&x);
	printf("\nEntrer y ");
	scanf("%d",&y);
	printf("Entrer radius ");
	scanf("%d",&radius);

}      /*inserer l'instruction: circle(x,y,radius); dans le prog suorce*/

/**************************************************************************/
void barre()
{

	printf("Entrer left :");
	scanf("%d",&l);
	printf("Entrer top :");
	scanf("%d",&t);
	printf("Entrer right :");
	scanf("%d",&r);
	printf("Entrer bottom :");
	scanf("%d",&b);


}     /* inserer l'instruction: bar(l,t,r,b);ds le prog principal */
/**************************************************************************/
void barre3D()
{

	printf("Entrer left :");
	scanf("%d",&le);
	printf("Entrer top :");
	scanf("%d",&to);
	printf("Entrer right :");
	scanf("%d",&ri);
	printf("Entrer bottom :");
	scanf("%d",&bo);
	printf("Entrer depth :");
	scanf("%d",&de);
	printf("Entrer topflag :");
	scanf("%d",&tf);


}   /*inserer l'instruction:  bar3d(le,to,ri,bo,de,tf); ds le prog principal*/
/**************************************************************************/
void polygone()
{
	int i;
	float j;

	printf("Entrer le nombre de points: ");
	scanf("%d",&numpoints);
	for(i=0;i<2*numpoints;i=i+2)
	{
		j = i/2+1;
		printf("point nø %e\n  ",j);
		printf("x = ");
		scanf("%d",&polypoints[i]);
		printf("\n");
		printf("y = ");
		scanf("%d",&polypoints[i+1]);
	}
}        /*inserer dans le programme principal :drawpoly(numpoints,polypoints);*/
/**************************************************************************/
void elliptique()
{
	printf("Entrer x :");
	scanf("%d",&x1);
	printf("Entrer y :");
	scanf("%d",&y1);
	printf("Entrer stangle :");
	scanf("%d",&stangle);
	printf("Entrer endangle :");
	scanf("%d",&endangle);
	printf("Entrer xradius :");
	scanf("%d",&xradius);
	printf("Entrer yradius :");
	scanf("%d",&yradius);
} /*inserer l'instruction : ellipse(x,y,stangle,endangle,xradius,yradius)*/
/*prendre par exemple (200,150,0,360,50,50)*/
/**************************************************************************/
void floodfil()
{
	printf("Entrer x :");
	scanf("%d",&x2);
	printf("Entrer y :");
	scanf("%d",&y2);
	printf("Entrer border :");
	scanf("%d",&border);
}
/*inserer l'instruction : floodfill(x,y,border)*/
/**************************************************************************/
void ligne()
{
	printf("Entrer x 1er point:");
	scanf("%d",&x3);
	printf("Entrer y 1er point :");
	scanf("%d",&y3);
	printf("Entrer x 2nd point :");
	scanf("%d",&x4);
	printf("Entrer y 2nd point :");
	scanf("%d",&y4);
}
/*inserer l' instruction : line(x3,y3,x4,y4)*/
/**************************************************************************/
void lignerel()
{
	printf("Entrer dx :");
	scanf("%d",&dx);
	printf("Entrer dy :");
	scanf("%d",&dy);
}/*inserer linerel(dx,dy)*/
/**************************************************************************/
void sectangle()
{
	printf("Entrer x :");
	scanf("%d",&x5);
	printf("Entrer y :");
	scanf("%d",&y5);
	printf("Entrer stangle :");
	scanf("%d",&stangle1);
	printf("Entrer endangle :");
	scanf("%d",&endang1);
	printf("Entrer radius :");
	scanf("%d",&radius1);
}/*inserer : pieslice(x5,y5,stangle1,endang1,radius1); */

/**************************************************************************/
void fenetre()
{
	printf("Entrer left :");
	scanf("%d",&left1);
	printf("Entrer top :");
	scanf("%d",&top1);
	printf("Entrer right :");
	scanf("%d",&right1);
	printf("Entrer bottom :");
	scanf("%d",&bottom1);
	printf("Entrer clip :");
	scanf("%d",&clip1);
}/*inserer setviewport(left1,top1,right1,bottom1,clip1); */
/**************************************************************************/
void Initialize()
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
/*************************************************************************/

