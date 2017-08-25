#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class Flacon
{
private:
  string nom;
  double volume;
  double pH;

public:
  /*****************************************************
    Compléter le code à partir d'ici
  *******************************************************/
	
	/*
   Flacon flacon1("Eau", 600.0, 7.0);
  Flacon flacon2("Acide chlorhydrique", 500.0, 2.0);
  Flacon flacon3("Acide perchlorique",  800.0, 1.5);
  */
	Flacon(string n, double v, double p):nom(n),volume(v),pH(p){}

	ostream& etiquette(ostream& sortie) const{
	
			sortie << nom << " : " << volume << " ml, ph " << pH ;
			
			return sortie;
	}
	
	string getNom(){
		
		return nom;
	}
	
	double getVolume(){
		return volume;
	}
	
	double getPH(){
		return pH;
	}
	

	
	/*  Il devra enfin être également possible de mélanger deux « flacons » au moyen de l'opérateur + :
	soient deux Flacons de nom, volume et pH respectifs : nom1, volume1, ph1 et nom2, volume2, ph2 ; le mélange de ces deux flacons au moyen de l'opérateur + donne un nouveau « flacon » dont :

    le nom est « nom1 + nom2 », par exemple : « Eau + Acide chlorhydrique » ;
    le volume est la somme de volume1 et volume2;
    et le pH est (que les chimistes nous pardonnent cette grossière approximation !) :

    pH=−log10(volume1×10−ph1+volume2×10−ph2volume1+volume2)

    La fonction log10 s'écrit log10 en C++ et pour faire le calcul 10-x utiliser l'expression pow(10.0, -x).
	*/
	

	
};

	/* une surcharge de l'opérateur d'affichage « pour cette classe ; cette surcharge devra utiliser la méthode etiquette précédente.*/

	/*ostream& operator<<(string a, Flacon const& f){
		ostream& s 
		return (a << f.etiquette(s));

	}*/
	
	ostream& operator<<(ostream& sortie, Flacon const& f) {
		return f.etiquette(sortie);
	}

	Flacon operator+(Flacon f1, Flacon f2) {
		string nom (f1.getNom() + " + " + f2.getNom());
		double volume = f1.getVolume() + f2.getVolume();		
		double pH = -log10( ( (f1.getVolume()*pow(10.0, -f1.getPH()) ) + (f2.getVolume()*pow(10.0, -f2.getPH()) ) ) / (f1.getVolume() + f2.getVolume())) ;
		Flacon f3(nom,volume,pH);

		return f3;
	}
/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

void afficher_melange(Flacon const& f1, Flacon const& f2)
{
  cout << "Si je mélange " << endl;
  cout << "\t\"" << f1 << "\"" << endl;
  cout << "avec" << endl;
  cout << "\t\"" << f2 << "\"" << endl;
  cout << "j'obtiens :" << endl;
  cout << "\t\"" << (f1 + f2) << "\"" << endl;
}

int main()
{
  Flacon flacon1("Eau", 600.0, 7.0);
  Flacon flacon2("Acide chlorhydrique", 500.0, 2.0);
  Flacon flacon3("Acide perchlorique",  800.0, 1.5);

  afficher_melange(flacon1, flacon2);
  afficher_melange(flacon2, flacon3);

  return 0;

}
