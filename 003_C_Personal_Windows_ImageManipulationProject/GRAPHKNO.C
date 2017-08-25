resume de mon savoir graphisme



bar(left,top,right,bottom): /*les parametressont tous entiers*/
	-left et top definissent le pixel de depart;
	-rigft et bottom definiront les dimensions de la barre
	 a partir du point de pixels;

bar3d(left,top,right,bottom,depth,topflag)
	-depth profondeur
	-topflag: ????

circle(x,y,radius)
	-x et y coordonn‚es du centre;
	-radius valeur du rayon;

cleardevice() :
	-efface l'ecran graphique;

closegraph():
	-referme le systeme graphique;

clearviewport():
	-efface fenetre graphique suivante;

drawpoly(int numpoints,int polypoints[]):
	-trace une ligne polygonale
	-l' argument polypoints contient numpoints*2 valeurs;
	-chaque couple donne les coordonn‚es x et y d' un sommet;

fillpoly(int numpoints,int polypoints[]):
	-trace et remplit le polygone;


ellipse(x,y,stangle,endangle,xradius,yradius) :
	-trace arc d' ellipse de centre (x,y);
	-stangle,endangle mesures en degr‚s d' origines et de fins;
	-xradius axe horizontal;
	-yradius axe vertical;

fillellipse(x,y,xradius,yradius) :
	-trace l' ellipse de centre(x,y);
	-l' arc est rempli avec le motif de remplissage;

