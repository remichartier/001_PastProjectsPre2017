/************************************************************************************************
*												*
*				programme codage/decodage delta						*
*												*
*	Rémi CHARTIER	Isabelle MILLET		Stéphane MATT					*
*									Juin 1993		*
*************************************************************************************************/



#include <stdio.h>
#include <string.h>
#include "fonction.h"
#include <stdlib.h>

#define MODE_ECRITURE 	"ab"				/* pas écrasement si existe déjà 	*/
#define MODE_LECTURE    "rb"








/************************************************************************************************
*												*
*				programme codage/decodage delta						*
*												*
*	Rémi CHARTIER	Isabelle MILLET		Stéphane MATT					*
*									Juin 1993		*
*************************************************************************************************/





void main()
{
	FILE *file_source,*file_dest,*file_rest;
	int test,position;
						/* test retour de fonctions */
	unsigned char numero_bloc;
	double longueur;					/* longueur d'un bloc	*/
	int bloc_precedant=0;				/* numero bloc précédant */
	int first_data=1;			/* sert à tester si 1ère donnée échantillonnée */
	unsigned char dernier_echantillon=0;			/* nécéssaire codage delta		*/
	unsigned char *delta;
	unsigned char delta_min=10;
	unsigned char delta_max=10;
	float alpha_plus=1.5;
	float alpha_moins=1/alpha_plus;
	int *dernier_d;
	int *premier_octet;
	float coef_approx=1.0;
	unsigned char *x,*x_,*y,*y_;

	*dernier_d=1;
				/* ouverture des fichiers */
	test=ouvrir_fichier(&file_source,MODE_LECTURE);
	if(test==0)
		exit(-1);
	test=ouvrir_fichier(&file_dest,MODE_ECRITURE);
	if(test==0)
		exit(-1);
	test=ouvrir_fichier(&file_rest,MODE_ECRITURE);
	if(test==0)
		exit(-1);
	test=test_version_fichier(file_source);
	if(test!=0)
	{
		printf("mauvaise version de fichier\n");
		exit(-1);
	}

	rewind(file_source);
	rewind(file_dest);
        rewind(file_rest);
							/* recopie entˆte		*/
	recopier(file_source,file_dest,(double)26);
	rewind(file_source);
        recopier(file_source,file_rest,(double)26);
							/* placé au début des données */
	fseek(file_source,26,0);
	while((numero_bloc=lire_no_bloc(file_source))!=0)
	{
		switch(numero_bloc)
		{
			case(1):
				decoder_bloc_delta_diff(file_source,file_dest,file_rest,1,bloc_precedant,&dernier_echantillon,&first_data,
					delta,
					delta_min,
					delta_max,
					alpha_plus,
					alpha_moins,
					dernier_d,
					premier_octet,
					coef_approx,
					x,x_,y,y_);
				break;
			case(2):
				decoder_bloc_delta_diff(file_source,file_dest,file_rest,2,bloc_precedant,&dernier_echantillon,&first_data,
					delta,
					delta_min,
					delta_max,
					alpha_plus,
					alpha_moins,
					dernier_d,
					premier_octet,
					coef_approx,
					x,x_,y,y_);
				break;
			default:
				printf("copie bloc sans son\n");
                                position=ftell(file_source);
				longueur=lire_longueur_bloc(file_source)+4;
				recopier(file_source,file_dest,longueur);
				fseek(file_source,position,0);
                                recopier(file_source,file_rest,longueur);
				break;
		}					/* fin du switch bloc	*/
		bloc_precedant=numero_bloc;
	}						/* fin du while		*/
							/* recopie end_bloc */
        position=ftell(file_source);
	recopier(file_source,file_dest,(double)1);
	fseek(file_source,position,0);
        recopier(file_source,file_rest,(double)1);

							/* fermeture des fichiers */
	fclose(file_source);
	fclose(file_dest);
        fclose(file_rest);

}							/* fin du main		*/






