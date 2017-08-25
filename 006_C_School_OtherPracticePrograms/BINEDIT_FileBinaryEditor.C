/*______________________________________________________________________
l	R‚mi CHARTIER					20 octobre 1992	l
l                                                                       l
l			BINEDIT.C				        l
l                                                                       l
l	Programme d'‚dition de fichier en binaire		        l
l_______________________________________________________________________l*/






#include<stdio.h>

#define  MODE1 "rb"

/*******************************************************************
*                                        			   *
*		D‚claration des fonctions                          *
*                                                                  *
********************************************************************/

void editoctet();


/*******************************************************************
*                                        			   *
*		D‚claration des variables globales	           *
*                                                                  *
********************************************************************/

FILE *pointeurfichier;
unsigned char A=128;


/*******************************************************************
*                                        			   *
*		Fonction main()				           *
*                                                                  *
********************************************************************/


main()
{
	unsigned char octet;	/*octet … traduire en binaire*/
	char nomfichier[9];	/*nom du fichier qu'on traduira en binaire*/
	long longueur_fichier;	/*longueur du fichier … lire*/
	int i;
	long essai;

	printf("\n\tEntrez le nom du fichier … lire : ");
	scanf("\n%s",nomfichier);

	/*ouverture du fichier … traduire*/
	pointeurfichier=fopen(nomfichier,MODE1);

	if(pointeurfichier==NULL)

		/*cas ouverture fichier impossible*/
		printf("\nimpossible d'ouvrir ce fichier.\n");

	else
	{	/*cas fichier ouvert*/

		/*positionnement d‚but fichier*/
		rewind(pointeurfichier);

		/*recherche longueur du fichier*/
		fseek(pointeurfichier,0,2);	/*positionnement fin de fichier*/
		longueur_fichier=ftell(pointeurfichier);
		rewind(pointeurfichier);
		printf("\nlong:%d",longueur_fichier);

		/*lecture longueur fichier compress‚*/
		fread(&essai,sizeof(essai),1,pointeurfichier);
		printf("\nlongueur fichier compress‚:%d",essai);
		rewind(pointeurfichier);

		printf("\n");
		i=1;
		do
		{
			fread(&octet,sizeof(octet),1,pointeurfichier);
			editoctet(octet);
			fseek(pointeurfichier,i,0);
			i++;
		}
		while(ftell(pointeurfichier)<longueur_fichier);
	}
	fclose(pointeurfichier);
}


void editoctet(caractereunsigned)
unsigned char caractereunsigned;
{
	char ecriturebinaire[9];
	int i=0;

	ecriturebinaire[8]='\0';
	while(i<8)
	{
/*		printf("%d %d;",caractereunsigned&(A>>i),A>>i);*/
		if((caractereunsigned&(A>>i))==(A>>i))
		ecriturebinaire[i]='1';
		else
		ecriturebinaire[i]='0';
		i++;
	}
	printf("\t%s",ecriturebinaire);
}



