
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 21
#define MODE1 "r"      /**mode lecture uniquement**/
#define MODE2 "w+"     /**mode lect + ecrit + ecras. +cr‚at. **/
#define MODE3 "w"      /**mode ecriture uniquement**/

int lirechaine();      /**lit prochain mot**/
int cherchemot();      /**cherche mot identique,incremente puis efface**/
void effacer();        /**eface un mot dans un texte**/
void fremplissage();   /**remplit le fichier resultat nom + occurence**/
void copie();          /**copie fic.original ds autre fichier en minuscules**/
void minuscules();     /**convertie un mot en minuscules si besoin est**/

char mot[MAX];

main()
{
	char texte[MAX];
	FILE *ptexte,*ptexte1,*pprocess1;
	int occurence,i;
	long cour;                                      /**memoire pointeur courant ds fich. texte**/

	printf("\nNom du fichier texte :  ");
	scanf("%s",texte);
	ptexte=fopen(texte,MODE1);
	ptexte1= fopen("t1",MODE2);
	printf("%d",ptexte1);
	copie(ptexte,ptexte1);   			/**ptexte1 contient tous mots de ptexte en minuscules**/
	fclose(ptexte);
	pprocess1 = fopen("process1",MODE3);
	i = lirechaine(mot,ptexte1);      		/**lit 1er mot ds ptexte1 et -->mot[]**/

	while(i!=1)
	{
		cour = ftell(ptexte1);
		occurence = 1 + cherchemot(ptexte1);    /**on a maintenant l' occurence du mot et il est efface dans**/
		fremplissage(pprocess1,occurence);	/**ptexte1-->impossible de le rencontrer ds prochain parcours**/
		fseek(ptexte1,cour,0);
		i = lirechaine(mot,ptexte1);
	}
	fclose(ptexte1);
	fclose(pprocess1);
}


void effacer(tampon,pointeur)
FILE *pointeur;
long tampon;
{
	long tamponfin;

	tamponfin = ftell(pointeur);
	fseek(pointeur,tampon,0);
	while(ftell(pointeur) != tamponfin)
		putc(' ',pointeur);
	fseek(pointeur,tamponfin,0);
}


int cherchemot(pointeur)
FILE *pointeur;
{
	int n,i;
	char s[MAX];
	long tampon;        				/**position courante pointeur ds texte**/
	n = 0;						/**occurence ds fin texte**/
	do
	{
		tampon = ftell(pointeur);
		i = lirechaine(s,pointeur);
		if (strcmp(s,mot) == 0)
		{
			effacer(tampon,pointeur);
			n++;
		}
	}
	while(i != 1);
	return(n);
}


int lirechaine(s,pointeur)
FILE *pointeur;
char s[];
{
	int i =0;

	s[i] = getc(pointeur);
	while((isalpha(s[i])==0)&&(s[i]!=EOF))		/**recherche premier caractere**/
	{
		s[i] = getc(pointeur);
	}
	if (s[i] == EOF)				/**cas fin fichier**/
		return(1);
	i++;
	s[i] = getc(pointeur);
	while((isalpha(s[i]) !=0)&& (i<MAX-1))		/**stockage du mot ds s[]**/
		{
			i++;
			s[i] = getc(pointeur);
		}
	if(i==MAX-1)    				/**si + de 20 caracteres,recherche fin mot**/
	{       do
		s[i] = getc(pointeur);
		while(isalpha(s[i]) !=0);
	}
	s[i] = '\0';

	return(0);
}


void fremplissage(pointeur,n)
FILE *pointeur;
int n;
{
	fputs(mot,pointeur);
	putc(' ',pointeur);
	itoa(n,mot,10);
	fputs(mot,pointeur);
	putc('\n',pointeur);
}


void copie(pointeur1,pointeur2)
FILE *pointeur1,*pointeur2;
{
	char s[MAX];
	int i;
	i = lirechaine(s,pointeur1);
	while(i != 1)
	{
		minuscules(s);
		fputs(s,pointeur2);
		putc(' ',pointeur2);
		i = lirechaine(s,pointeur1);
	}
	rewind(pointeur2);
}


void minuscules(s)
char s[];
{
	int i;
        i =0;
	while(s[i]!='\0')
	{
		if ((isupper(s[i]))!=0)
		{
			s[i] =tolower(s[i]);
		}

		i++;
	}
}