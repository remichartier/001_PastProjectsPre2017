
import java.util.ArrayList;


public class SommeProduit {
	
	// écrire une fonction qui retourne la somme des chiffres d'un nombre passé en paramètre
	// Ecrivez une fonction appelée somme qui, en appliquant la méthode décrite ci-dessus, 
	// retourne la somme des chiffres du nombre entier qu'elle reçoit en paramètre. La valeur de retour est donc également un nombre entier.
	
	static boolean printDebug = false;

	// ExtractChiffres
	static int extractChiffresAndOperation(int number, char op){
		//		ArrayList<Integer> chiffres = new ArrayList<Integer>();
		int ret=0;
		int num= number;
		if(op=='+'){ ret=0;}
		else if(op=='*') {ret=1;}
			
		while(num !=0){
			//			chiffres.add(n%10);
			if(op=='+'){ ret=ret +(num%10);}
			else if(op=='*') {ret=ret *(num%10);}
			if (printDebug) { System.out.println("ret=" + ret + " "  );}
			num /= 10;	
		}
		
		return ret;
	}

	static int somme(int n){
		return extractChiffresAndOperation(n, '+');
	}
	
	// Ecrivez ensuite une fonction appelée produit qui retourne le produit des chiffres 
	// d'un nombre passé en paramètre. 

	static int produit(int n){
		return extractChiffresAndOperation(n, '*');
	}	

	// Utilisez ensuite vos deux fonctions pour écrire une fonction sommeProduitEgaux 
	// qui teste si la somme des chiffres d'un nombre entier passé en paramètre 
	// est égale au produit de ces mêmes chiffres.
	// Note : on peut écrire cette fonction en une seule ligne.
	
	
	static boolean sommeProduitEgaux(int n){
		if (printDebug) { System.out.println("n=" + n + " Somme " + somme(n) + " Produit " + produit(n) );}
		return (somme(n) == produit(n));
	}
	
	
	public static void main(String[] args) {
			
		// trouver des nombres dont la somme des chiffres est égale à leur produit. 
		// Par exemple 123 est un tel nombre : 1 + 2 + 3 = 1×2×3.
		
		// Utilisez enfin cette fonction sommeProduitEgaux pour afficher les 20 premiers nombres 
		// entiers plus grands que 10 dont la somme des chiffres est égale au produit de ces mêmes chiffres. 
		// Pour vous aider à vérifier votre programme, voici les 5 premiers nombres que vous devriez 
		// trouver : 22, 123, 132, 213 et 231.
		
		// respecter strictement le format d'affichage suivant : liste de nombres en une seule ligne, séparés par un espace. Il n'y aura qu'un seul retour à la ligne, à la fin. Pour faciliter l'écriture de votre code le dernier nombre peut également être suivi d'une espace. Avec 5 nombres, cela donnerait :
		// 22 123 132 213 231
		
		// ArrayList<Integer> chiff = new ArrayList<Integer>();
		
		//		ExtractChiffres(123, chiff);
		//		System.out.println(chiff);
		
		// chiff.clear();
		
		// Utilisez enfin cette fonction sommeProduitEgaux pour afficher les 20 premiers 
		// nombres entiers plus grands que 10 dont la somme des chiffres est égale 
		// au produit de ces mêmes chiffres. Pour vous aider à vérifier votre programme, 
		// voici les 5 premiers nombres que vous devriez trouver : 22, 123, 132, 213 et 231.\
			int n=11;
			int iter=0;
			while(iter<20){
				if (sommeProduitEgaux(n)) {
					System.out.print(n + " ");
					iter ++;
				}
				n++;
			}
			System.out.println();
		}
	
		
	}
	

