
/************************************************************************************************
*												*
*				programme codage/decodage delta						*
*												*
*	Rémi CHARTIER	Isabelle MILLET		Stéphane MATT					*
*									Juin 1993		*
*************************************************************************************************/
#include <stdio.h>
#include "fonction.h"
#include <math.h>
#include <stdlib.h>

#define MODE_LECTURE "rb"
#define MODE_ECRITURE "ab"


void main()
{
	FILE *file_source,*file_dest,*file;
	int test;
	unsigned char c;					/* test retour de fonctions */
	double longueur;					/* longueur d'un bloc	*/
	double f,fe;				/* fr‚quence signal et d'‚chantillonage	*/
	char tab[3];				/* sert … coder longueur	*/
	int compteur;				/* pour les r‚p‚titions		*/
	double duree;				/* dur‚e qu'on veut avoir	*/
	char SR;                                /* champ codant la fr‚quence d'‚chantillonage*/
	double t;
	unsigned char octet;

							/* ouverture des fichiers */
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
	file=fopen("c:\sinusess.doc","ab");
	rewind(file);
	rewind(file_source);
	rewind(file_dest);
							/* recopie entˆte		*/
	longueur=26;
	recopier(file_source,file_dest,longueur);


	f=1000;						/* fr‚quence du signal sinuso‹dal	*/
	fe=20000;					/* fr‚quence d'‚chantillonage		*/



	/*bloc6*/
	/*écriture no bloc*/
	c=6;
	fwrite(&c,sizeof(c),1,file_dest);
        /*écriture longueur bloc6*/
	coder_longueur((double)2,tab);
	fwrite(tab,sizeof(char),3,file_dest);
	/*calcul nb répétitions*/
	duree=2;
	compteur=2/*duree*f*/;
	fwrite(&compteur,sizeof(compteur),1,file_dest);

	/*bloc1*/
	c=1;
	fwrite(&c,sizeof(c),1,file_dest);
	longueur=fe/f+2;
	coder_longueur(longueur,tab);
	fwrite(tab,sizeof(tab[0]),3,file_dest);
	SR=256-(char)(100000/fe);
	fwrite(&SR,sizeof(SR),1,file_dest);
	/* ‚criture compactage*/
	c=0;
	fwrite(&c,sizeof(c),1,file_dest);
	/*‚criture des donn‚es ‚chantillonn‚es*/
	t=0;
	while(t<1/f)
	{
		octet=echantillon_sinus(t,f);
		t=t+1/fe;
		fwrite(&octet,sizeof(octet),1,file_dest);
                fprintf(file,"%d  ",octet);
	}

	/*bloc7*/
	c=7;
	fwrite(&c,sizeof(c),1,file_dest);
	coder_longueur((double)0,tab);
	fwrite(tab,sizeof(tab[0]),3,file_dest);

	/* bloc0 */
	c=0;
	fwrite(&c,sizeof(c),1,file_dest);
	fclose(file_dest);
	fclose(file_source);
        fclose(file);
}							/* fin du main		*/



