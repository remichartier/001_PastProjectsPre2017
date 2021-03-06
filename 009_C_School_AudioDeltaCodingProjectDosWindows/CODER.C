
/************************************************************************************************
*												*
*				programme codage/decodage delta						*
*												*
*	R�mi CHARTIER	Isabelle MILLET		St�phane MATT					*
*									Juin 1993		*
*************************************************************************************************/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fonction.h"

#define MODE_ECRITURE 	"ab"				/* pas �crasement si existe d�j� 	*/
#define MODE_LECTURE    "rb"




void main()
{
	FILE *file_source,*file_dest;
	int test;					/* test retour de fonctions */
	char numero_bloc;
	double longueur;					/* longueur d'un bloc	*/
	int bloc_precedant=0;				/* numero bloc pr�c�dant */
	int first_data=1;			/* sert � tester si 1�re donn�e �chantillonn�e */
        unsigned char dernier_echantillon;			/* n�c�ssaire codage delta		*/
	float coef_approx=1.0;
	unsigned char somme_delta=0;
	unsigned delta = 7;						/* ouverture des fichiers */
	test=ouvrir_fichier(&file_source,MODE_LECTURE);
	if(test==0)
		exit(-1);
	test=ouvrir_fichier(&file_dest,MODE_ECRITURE);
	if(test==0)
		exit(-1);

	test=test_version_fichier(file_source);
	if(test!=0)
	{
        	printf("mauvaise version de fichier\n");
		exit(-1);
        }

	rewind(file_source);				/* recopier l'ent�te */
	recopier(file_source,file_dest,(double)26);

							/* plac� au d�but des donn�es */
        fseek(file_source,26,0);
	while((numero_bloc=lire_no_bloc(file_source))!=0)
	{
		switch(numero_bloc)
		{
			case(1):
				coder_bloc_delta(file_source,file_dest,1,bloc_precedant,&dernier_echantillon,&first_data,coef_approx,&somme_delta,delta);
				break;
			case(2):
				coder_bloc_delta(file_source,file_dest,2,bloc_precedant,&dernier_echantillon,&first_data,coef_approx,&somme_delta,delta);
				break;
			default:
                                printf("copie bloc sans son\n");
				longueur=lire_longueur_bloc(file_source)+4;
				recopier(file_source,file_dest,longueur);
				break;
		}					/* fin du switch bloc	*/
		bloc_precedant=numero_bloc;
	}						/* fin du while		*/
							/* recopie end_bloc */
	recopier(file_source,file_dest,(double)1);

							/* fermeture des fichiers */
	fclose(file_dest);
	fclose(file_source);
}							/* fin du main		*/



