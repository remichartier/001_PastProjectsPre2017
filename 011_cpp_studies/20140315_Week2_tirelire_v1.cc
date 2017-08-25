#include <iostream>
using namespace std;

/*******************************************
 * Complétez le programme à partir d'ici.
 *******************************************/
 class Tirelire {
		public:
		
			double getMontant(){
				return total;
			} // end GetMontant()
			
			void afficher(){
				if(total==0.0)
					cout << "Vous êtes sans le sous.\n";
				else
					cout << "Vous avez : " << total << " euros dans votre tirelire.\n";
				
			} // end afficher()
			
			
			void secouer(){
				if(total>0.0)
					cout << "Bing bing\n";
			} // end secouer()
			
			void remplir(double m){
				if(m>0.0)
					total += m;
			} // end remplir()
			
			void vider(){
				total=0.0;
			} // end vider()
			
			void puiser(double m){
				if(m>=0.0){
					if(m>total)
						vider();
					else
						total -= m;
				}	
			} // end puiser()
			
			
			bool montant_suffisant(double budget, double & solde){
				if(budget <= 0.0){
					solde = total;
					return true;
				}
				
				if (budget<=total){
						solde = total - budget;
						return true;
				}
				else{
						solde = budget - total ;
						return false;
				}
			} // end montant_suffisant()
			
			
			
		private:
		
			double total;
	 
 };  // end class Tirelire


/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  Tirelire piggy;

  piggy.vider();
  piggy.secouer();
  piggy.afficher();

  piggy.puiser(20.0);
  piggy.secouer();
  piggy.afficher();

  piggy.remplir(550.0);
  piggy.secouer();
  piggy.afficher();

  piggy.puiser(10.0);
  piggy.puiser(5.0);
  piggy.afficher();

  cout << endl;

  // le budget de vos vacances de rève.
  double budget;

  cout << "Donnez le budget de vos vacances : ";
  cin >> budget;

  // ce qui resterait dans la tirelire après les
  // vacances
  double solde(0.0);

  if (piggy.montant_suffisant(budget, solde)) {
    cout << "Vous êtes assez riche pour partir en vacances !"
         << endl
         << "Il vous restera " << solde << " euros"
         << " à la rentrée." << endl << endl;
    piggy.puiser(budget);
  } else {
    cout << "Il vous manque " << solde << " euros"
         << " pour partir en vacances !" << endl << endl;
  }
  return 0;
}
