#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*******************************************
 * Completez le programme a partir d'ici.
 *******************************************/

 
class Auteur{
	
	private:
		string nom;
		bool prime;
		
	public:
	
		/*Auteur(string n, bool p):prime(false){
			
		} // end constructeur Auteur()
		*/
		
		Auteur(string n, bool p=false):nom(n),prime(p){
		} // end constructeur Auteur()
		
	Auteur(Auteur const&) = delete;
		
		string getNom(){
			return nom;
		} // end getNom()
		
		bool getPrix(){
			return prime;
		} // end getPrix()
			
}; // end class Auteur


 
class Oeuvre{
	
	private:
		string titre;
		string langue;
		Auteur& auteur;
	
	public:
	
		Oeuvre(string t, Auteur& a, string l):titre(t),langue(l),auteur(a){
		} // end contructeur Oeuvre()
		
		Oeuvre(Auteur& a):auteur(a){
			
		} // end contructeur Oeuvre()
		
		
		Oeuvre(Oeuvre const&) = delete;
		
		
		string getTitre(){
			return titre;
		} // end getTitre()
	
		// une méthode getAuteur retournant une référence constante sur l'auteur (veillez à bien respecter ce type) ;
//		const Auteur& getAuteur(){
		Auteur& getAuteur(){
			return auteur; 
		} // end getAuteur;
		
		string getLangue(){
			return langue;
		} // end getLangue()
		
		
		void affiche(){
			
			cout << titre << ", " << auteur.getNom() << ", en " << langue ;
		} // end affiche
		
		~Oeuvre(){
			cout << "L'oeuvre \"" << titre << ", " << auteur.getNom() << ", en " << langue << "\" n'est plus disponible." << endl;
		} // end destructeur ~Oeuvre()
}; // end Oeuvre

class Exemplaire{
	
	private:
		Oeuvre& oeuvre;
		
	public:
	
		Exemplaire(Oeuvre& p_o):oeuvre(p_o){
			
			cout << "Nouvel exemplaire de : " ;
			oeuvre.affiche();
			cout << endl;
		} // end contructeur Exemplaire()
		
		Exemplaire(const Exemplaire& exemp):oeuvre(exemp.oeuvre){
			cout << "Copie d'un exemplaire de : " ;
//			cout << "Nouvel exemplaire de : " ;
			oeuvre.affiche();
			cout << endl;
		} // end constructeur copie() Exemplaire()
		
		~Exemplaire(){
				cout << "Un exemplaire de \"" ; 
				oeuvre.affiche();
				cout << "\" a été jeté !" << endl ;
		} // end destructeur ~Examplaire()
		
		// une méthode getOeuvre retournant une référence constante à l'œuvre ;
		Oeuvre& getOeuvre(){
			return oeuvre;
		} // end getOeuvre()

		void affiche(){
			cout << "Exemplaire de : " ;
			oeuvre.affiche();
		} // end affice()
			
}; // end class Exemplaire



class Bibliotheque{
	
	private:
		string nom;
		vector<Exemplaire *> ensemble;
		
	public:
		Bibliotheque(string n):nom(n){
				cout << "La bibliothèque " << nom << " est ouverte !" << endl;	
		} // end constructeur Bibliotheque()
		
		string getNom(){
			return nom;
		} // end getNom()

			
		// permettant d'ajouter un ou plusieurs exemplaires d'une œuvre dans la bibliothèque ; elle doit être conforme au main fourni, avec l'ordre suivant des paramètres :
		void stocker(Oeuvre& o, int n=1){
			// a remplir
			//cette méthode va ajouter à l'ensemble d'exemplaires de la bibliothèque n exemplaires de l'œuvre fournie, dynamiquement alloués ; les nouveaux exemplaires devront impérativement être ajoutés à la fin du tableau dynamique ;
			for(int i=0;i<n;i++){
							Exemplaire * e= new Exemplaire(o);		
							ensemble.push_back(e);
			}
		} // end stocker() 

		~Bibliotheque(){
			cout << "La bibliothèque " << nom << " ferme ses portes," << endl << "et détruit ses exemplaires :" << endl;
			
			//puis libère les zones mémoires liées à ses exemplaires.
			for(auto ex:ensemble){
			delete ex;
			}
			
			ensemble.clear();
			
		} // end destructeur Bibliotheque()
		
		void lister_exemplaires(string lg=""){
			// affichant tous les exemplaires d'une œuvre écrite dans une langue donnée ; si aucune langue n'est donnée (chaîne vide), tous les exemplaires de la bibliothèque seront affichés ; 
			// les exemplaires devront être affichés au moyen de la méthode d'affichage qui leur est spécifique et il y aura un saut de ligne à la fin de l'affichage de chaque exemplaire (voir l'exemple de déroulement fourni plus bas) ;
			if(lg.empty()){
				for(auto ex:ensemble){
					ex->affiche();
					cout << endl;
				}
			}
			else{
				for(auto ex:ensemble){
					Oeuvre& o=ex->getOeuvre();
					string l=o.getLangue();
					if(l == lg){
						ex->affiche();
						cout<<endl;
					}
				}
			}	
			
		} // end lister_exemplaires(void)
		
		void afficher_auteurs(bool affichePrix=false){
			//indiquant si l'on veut afficher uniquement les auteurs à prix ; 
			// cette méthode affichera les noms des auteurs dont un exemplaire est stocké dans la bibliothèque ; si le booléen vaut true, seuls s'afficheront les noms des auteurs avec un prix ; un saut de ligne sera fait après l'affichage de chaque nom ;
			for(auto ex:ensemble){
				bool prix=ex->getOeuvre().getAuteur().getPrix();
				if(affichePrix==false){
					cout << ex->getOeuvre().getAuteur().getNom() << endl;
				}
				else {
					if (prix){
					cout << ex->getOeuvre().getAuteur().getNom() << endl;						
					}
				}
			}
		} // end afficher_auteurs()
	
		
		
		
 		int compter_exemplaires(Oeuvre& o){
			// retournant le nombre d'exemplaires d'une œuvre donnée passée en paramètre ;
			int i(0);
			
			/*string titre;
			string langue;
			Auteur& auteur; --> nom*/

			for(auto ex:ensemble){
				string titre;
				string langue;
				string nom;
				
				titre = ex->getOeuvre().getTitre();
				langue = ex->getOeuvre().getLangue();
				nom = ex->getOeuvre().getAuteur().getNom();
				
				if ( (titre == o.getTitre() ) && (langue == o.getLangue() ) && ( nom == o.getAuteur().getNom() ) ){
					i++;
				}
			
			}
		return i;
			
		} // end compter_exemplaire()
		
}; // end class Bibliotheque()

/*******************************************
 * Ne rien modifier apres cette ligne.
 *******************************************/

int main()
{
  Auteur a1("Victor Hugo"),
         a2("Alexandre Dumas"),
         a3("Raymond Queneau", true);

  Oeuvre o1("Les Misérables"           , a1, "français" ),
         o2("L'Homme qui rit"          , a1, "français" ),
         o3("Le Comte de Monte-Cristo" , a2, "français" ),
         o4("Zazie dans le métro"      , a3, "français" ),
         o5("The Count of Monte-Cristo", a2, "anglais" );

  Bibliotheque biblio("municipale");
  biblio.stocker(o1, 2);
  biblio.stocker(o2);
  biblio.stocker(o3, 3);
  biblio.stocker(o4);
  biblio.stocker(o5);

  cout << "La bibliothèque " << biblio.getNom()
       << " offre les exemplaires suivants :" << endl;
  biblio.lister_exemplaires();

  const string langue("anglais");
  cout << " Les exemplaires en "<< langue << " sont :" << endl;
  biblio.lister_exemplaires(langue);

  cout << " Les auteurs à succès sont :" << endl;
  biblio.afficher_auteurs(true);

  cout << " Il y a " << biblio.compter_exemplaires(o3) << " exemplaires de "
       << o3.getTitre() << endl;

  return 0;
}
