/*
 * EXEMPLE 15-1
 *
 * Montagnes fractales
 *
 * Calcul‚ pour une carte VGA
 */

 #include"graphics.h"
 #include"conio.h"
 #include"stdlib.h"
 #include"stdio.h"
 #include"math.h"
 #include"time.h"
 #include"dos.h"

 # define COTE 128 /* quantit‚ multiple de deux */

 int frac[COTE+1][COTE+1]={0};
 int face[5][2];
 float rugosite;

 /* prototype */
 void setfrac(int,int,int,int);

 void main(void)
 {
 int  gestionnaire = VGA, mode = VGAHI, erreur_graphique;
 int  i, j, n, nbr, ind, h, X1, X2, Y1, Y2;
 char c;

 initgraph(&gestionnaire,&mode,"");
 if ((erreur_graphique=graphresult())!=grOk)
 { printf("Erreur graphique :%s\n",grapherrormsg(erreur_graphique));
   getch();
   exit(1); }

 /* initialisation */

 do
 { randomize();
   rugosite        = 100.0 + random(200);
   frac[0][COTE]   = random(100) + 100;
   frac[0][0]      = -random(100);
   frac[COTE][COTE]= random(100) + 100;
   frac[COTE][0]   = -random(100);
   nbr = 0;
   do nbr++;
   while (!((COTE>>nbr)&1));

  /* g‚n‚ration */
  for (n=1;n<=nbr;n++)
  { ind=COTE>>(n-1);
    for (i=0;i<COTE;i+=ind)
     for (j=0;j<COTE;j+=ind)
      setfrac(i,i+ind,j,j+ind); }

  /* "lacs" (s'il en y a) */
  for (i=0;i<=COTE;i++)
   for (j=0;j<=COTE;j++)
   if (frac[i][j]<0) frac[i][j]=0;
   cleardevice();

  /* tra‡age */

  for(j=COTE-1;j>=0;j--)
  {
   for (i=0;i<COTE;i++)
   {
     X1=(int) (480.0/COTE*i);
     Y1=(int) ((300.0/COTE*j));
     X2=(int) (480.0/COTE*(i+1));
     Y2=(int) ((300.0/COTE*(j+1)));
     h=(frac[i][j]+frac[i+1][j]+frac[i+1][j+1]+frac[i][j+1])/4;
     if (h<=0)
     {
      setfillstyle(SOLID_FILL,EGA_LIGHTBLUE);
      setcolor(EGA_BLUE);
     }
     else if (h<=10)
     {
      setfillstyle(SOLID_FILL,EGA_LIGHTGREEN);
      setcolor(EGA_LIGHTGRAY);
     }
     else if (h<30)
     {
      setfillstyle(SOLID_FILL,EGA_GREEN);
      setcolor(EGA_LIGHTGRAY);
     }
     else if (h<=90)
     {
      setfillstyle(SOLID_FILL,EGA_LIGHTGRAY);
      setcolor(EGA_DARKGRAY);
     }
     else
     {
      setfillstyle(SOLID_FILL,EGA_WHITE);
      setcolor(EGA_LIGHTGRAY);
     }

     face[0][0]=X1+Y1/2;
     face[0][1]=479-(frac[i][j]+Y1/2);
     face[1][0]=X2+Y1/2;
     face[1][1]=479-(frac[i+1][j]+Y1/2);
     face[2][0]=X2+Y2/2;
     face[2][1]=479-(frac[i+1][j+1]+Y2/2);
     face[3][0]=X1+Y2/2;
     face[3][1]=479-(frac[i][j+1]+Y2/2);
     face[4][0]=X1+Y1/2;
     face[4][1]=479-(frac[i][j]+Y1/2);

     fillpoly(5,(int*)face);}}

     c=0;
     if(kbhit())
     c=getch();
     if (c==32)
     getch();      }while(c!=27);

     closegraph();
     }

     void setfrac(int id,int ifi,int jd,int jf)
     {
      int dim, z1, z2, z3, z4, im,jm;

      /* le point est choisi en fonction de l'‚cart des indices */

      dim=(int)(rugosite/COTE*(abs(ifi-id)));

      z1=frac[id][jd];
      z2=frac[ifi][jd];
      z3=frac[id][jf];
      z4=frac[ifi][jf];

      im=(id+ifi)/2;
      jm=(jd+jf)/2;

      frac[im][jd]  = (z1+z2)/2+random(dim)-dim/2;
      frac[id][jm]  = (z1+z3)/2+random(dim)-dim/2;
      frac[ifi][jm] = (z2+z4)/2+random(dim)-dim/2;
      frac[im][jf]  = (z3+z4)/2+random(dim)-dim/2;
      frac[im][jm]  = (z1+z2+z3+z4)/4+2*random(dim)-dim;
      /* if((im==COTE/2)&&(jm==COTE/2))frac[im][jm]=200 */ }









