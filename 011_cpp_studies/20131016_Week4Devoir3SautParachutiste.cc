#include <iostream>
#include <cmath>    // pour exp()
using namespace std;

int main()
{
  double masse(80.0);
  do {
    cout << "masse du parachutiste (>= 40) ? ";
    cin >> masse;
  } while (masse < 40.0);

  double h0(39000.0);
  do {
    cout << "hauteur de depart du parachutiste (>= 250) ? ";
    cin >> h0;
  } while (h0 < 250.0);

  /*******************************************
   * Completez le programme a partir d'ici.
   *******************************************/
        // 1. commencez par déclarer dans la méthode main une constante g de valeur 9.81, 
        	// et deux variables de type double (qui seront modifiées lors de l'ouverture du parachute) : 
        	// v0, initialisée à 0 et t0, initialisée à 0. 
        const double G = 9.81;
        double v0 = 0.0, t0=0.0;
        
        // 2. Définissez ensuite les variables nécessaires à la description 
        	// du parachutiste telle que donnée ci-dessus, autre que sa masse que nous avons 
        	// déjà définie (voir le début de la méthode main dans le fichier téléchargé): 
        	// vitesse pour sa vitesse, hauteur pour son altitude, 
        	// accel pour son accélération et t pour le temps. 
        	//  On initialisera la surface du parachutiste à 2.0 m2, 
        	// son altitude avec la valeur de h0, 
        	// sa vitesse avec celle de v0 et son accélération avec celle de g.
        	// Définissez enfin une variable t initialisée à la valeur de t0. 
        double vitesse=v0, hauteur=h0, accel=G, t=t0, surface=2.0;	
        bool parachuteOn=false;
        
        
        // 3. Affichez les valeurs initiales telles que définies ci-dessus 
			// Utilisez cette ligne pour l'affichage
        // cout << t << ", " << hauteur << ", " << vitesse << ", " << accel << endl
        
        //  Chute libre
        	//	Pour calculer l'évolution du sportif en chute libre nous aurons 
        		// besoin des deux expressions suivantes : s qui est la surface 
        		//	du sportif divisée par sa masse ;
        double s = surface / masse ;
        		// un « terme » noté q et valant q=exp(−s×(t−t0)), 
        		// où t représente le temps courant et t0 le temps initial
        		// de la chute, initialisé à 0 dans la question précédente.
        		// Note : la fonction exp s'écrit simplement exp en Java, par exemple : Math.exp(x).
        
        // calculer l'évolution de la chute du sportif tel qu'initialisé 
        	// dans la question précédente : faites le calcul, de seconde 
        	// en seconde (c'est-à-dire ajouter à chaque fois 1 au temps t), 
        	// tant que le sportif n'atteint pas le sol, c'est-à-dire tant
        	// que son altitude h est positive. 
        	// Affichez les caractéristiques du sportif à chaque seconde 
        	//en respectant le format de la question précédente. 
        bool vitesseSon = false, vitesseMax=false;
        while (hauteur > 0) 
        {
        	
        	if ((accel <0.5) && (vitesseMax == false))
        	{
        		cout << "## Felix a atteint sa vitesse maximale" << endl;
        		vitesseMax = true;
        	}
        	
        	// dès que l'altitude du sportif est plus petite que 2500 m, 
        	// le programme change la valeur de la surface du sportif de 2.0 m2 
        	// (avant l'ouverture du parachute) à 25.0 m2 (après l'ouverture du parachute). 

        	// q=exp(−s×(t−t0))
        	// v(t) 	=g/s×(1−q)+v0×q
        	double q = exp(-s*(t-t0));
        	vitesse = ((G/s)*(1-q)) +v0*q;
        	// h(t) 	=h0−gs×(t−t0)−v0−g/ss×(1−q)
        	hauteur = h0 - ((G/s)*(t-t0)) - ((1-q)*(v0-(G/s))/s);
        	//a(t) 	=g−s×v(t)
        	accel = G -s*vitesse;
        	// On vous demande maintenant d'étendre votre programme précédent de sorte que :
        	//   dès que la vitesse du sportif dépasse la vitesse du son (343 m/s), 
        	//	le programme affiche (en plus, mais qu'une seule fois) le message suivant :
        	//   ## Felix depasse la vitesse du son
        	//   Ce message doit s'afficher AVANT les informations de temps, altitude, vitesse et accélération : 
        	if ((vitesse >= 343)&&(vitesseSon == false)){
        		cout << "## Felix depasse la vitesse du son" << endl;
        		vitesseSon = true;
        	}
        	// dès que son accélération est inférieure à 0.5 m/s2, le programme affiche 
        	//	(en plus, mais qu'une seule fois) le message suivant :
        	//	## Felix a atteint sa vitesse maximale
        	if ( (hauteur != 250) && (hauteur < 2500) && parachuteOn==false) 
        	{
        		surface = 25.0;
        		s = surface / masse;
        		// Il faut aussi changer les « conditions initiales » t0, v0 et h0 
            	// avec les valeurs actuelles du sportif 
            	// (de sorte que les équations d'évolution soient correctes pour la suite de la chute).
            	t0=t; v0=vitesse; h0=hauteur;
            	// De plus, le programme doit afficher le message suivant :
            	// ## Felix ouvre son parachute
            	// Ce message doit s'afficher AVANT les informations de temps, altitude, vitesse et accélération : 
            	cout << "## Felix ouvre son parachute" << endl;
            	parachuteOn = true;
        	}
        
        	        	
        	if ((accel <0.5) && (vitesseMax == false))
        	{
        		cout << "## Felix a atteint sa vitesse maximale" << endl;
        		vitesseMax = true;
        	}
        	
        	if (hauteur >0) {
        		cout << t << ", " << hauteur << ", " << vitesse << ", " << accel << endl ;
        	}
        	if (hauteur == 250) {cout << "## Felix ouvre son parachute" << endl;
        	parachuteOn = true;}
        	t=t+1;
        } //while (hauteur > 0)
     


  /*******************************************
   * Ne rien modifier apres cette ligne.
   *******************************************/

  return 0;
}
