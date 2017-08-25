#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Pour simplifier
typedef string Forme   ;
typedef string Couleur ;

class Brique
{
private:
  Forme   forme   ;
  Couleur couleur ;

public:
  /*****************************************************
    Compléter le code à partir d'ici
  *******************************************************/
	/*un constructeur prenant deux paramètres, respectivement de type Forme et Couleur (dans cet ordre) ; il n'y aura pas de constructeur par défaut pour cette classe ;*/

	Brique(string f, string c):forme(f),couleur(c){
	}
	
	/*
	 *  une méthode publique de prototype
		ostream& afficher(ostream& sortie) const ;

		lui permettant d'afficher son contenu au format :

    si la couleur n'est pas la chaîne vide :
    (forme, couleur)

    comme par exemple (d'autres exemples sont fournis plus bas) :
    (obliqueG, rouge)
    si la couleur est la chaîne vide, afficher simplement la forme (sans autre signe) ;

	 * 
	 * 
	 * */
	ostream& afficher(ostream& sortie) const{
			if (!couleur.empty() ){
			sortie << "(" << forme << ", " << couleur << ")";	
			}
			else{
				sortie << forme ;
			}
			return sortie;	
	}

};

ostream& operator<<(ostream& sortie, Brique const& b) {
		return b.afficher(sortie);
}

class Construction
{
	friend class Grader;
  
	private:
		vector<vector<vector<Brique>>> contenu;
		//vector<Brique> contenu;
	public:
	
		/*un constructeur prenant une Brique comme paramètre et construisant le contenu comme un tableau 1x1x1 contenant cette seule brique ;*/
		Construction(Brique b){
			
			contenu.push_back(vector<vector<Brique>>(1, vector<Brique>(1, Brique(b))));		
		};
		
		
		/* une méthode publique de prototype
			ostream& afficher(ostream& sortie) const ;
			affichant son contenu couche par couche comme illustré plus bas dans l'exemple de déroulement (un message « Couche numéro : » est intercalé entre l'affichage de chaque couche de la construction) et n'affichant rien si le contenu de la construction est vide.
		*/
		
		ostream& afficher(ostream& sortie) const {
			
			if ( contenu.empty()  ){
				
				sortie << "";
			}
			else {
				
				
			}
			
			return sortie;
			
		}
		
		/* Operateurs de la Classe */
		
		
		/* 'opérateur interne operator^= et l'opérateur externe operator^ qui ajoutent une Construction au dessus :

			a ^= b; ajoute la Construction b au dessus de la Construction a ;
			a ^ b; crée une nouvelle Construction qui est le résultat de la Construction b mise au dessus de la Construction a, comme illustré ici : 
		*/
		
		void operator^=(Construction const& c ){
			
			
		}
		
		
		

};

/* Operateur Classe Construction */

/* l'opérateur (externe) d'affichage « utilisant la méthode afficher précédente ; */
ostream& operator<<(ostream& sortie, Construction const& c) {
	
		return c.afficher(sortie);
}


/*
const Construction operator*(unsigned int n, Construction const& a)
{
}

const Construction operator/(unsigned int n, Construction const& a)
{
}

const Construction operator%(unsigned int n, Construction const& a)
{
}

*/

/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  // Modèles de briques
  Brique toitD("obliqueD", "rouge");
  Brique toitG("obliqueG", "rouge");
  Brique toitM(" pleine ", "rouge");
  Brique mur  (" pleine ", "blanc");
  Brique vide ("                 ", "");

 /*  unsigned int largeur(4);
  unsigned int profondeur(3);
  unsigned int hauteur(3); // sans le toit


  // on construit les murs
  Construction maison( hauteur / ( profondeur % (largeur * mur) ) );

  // on construit le toit
  Construction toit(profondeur % ( toitG + 2*toitM + toitD ));
  toit ^= profondeur % (vide + toitG + toitD);

  // on pose le toit sur les murs
  maison ^= toit;

  // on admire notre construction
  cout << maison << endl;

*/

  return 0;
}
