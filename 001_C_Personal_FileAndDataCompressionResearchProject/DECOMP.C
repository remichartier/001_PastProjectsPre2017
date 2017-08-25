/*projet telecom			juillet 1992*/
/*sujet: decompression de texte*/

#include <stdio.h>
#include <stdlib.h>	/*pour malloc() et calloc()*/
#include<ctype.h>

#define MODE1 "r+b"	/*mode permettant uniquement lecture de fichier*/
#define MODE2 "a+b"	/*ecriture,creation si non existant,ecrase pas si existe,positionnement fin fichier*/
#define NBLET 20


	/******************************************/
	/*					  */
	/*DECLARATIONS STRUCTURES et VARIABLES	  */
	/*					  */
	/******************************************/

struct ARBRE{
	unsigned char caractere;
	struct ARBRE *succd;
	struct ARBRE *succg;
	struct ARBRE *suivant;
	};

typedef struct ARBRE arbre;

typedef arbre *parbre;

parbre racine,tete;

	/******************************************/
	/*					  */
	/*	DECLARATIONS DE FONCTIONS	  */
	/*					  */
	/******************************************/

void reconstitution_arbre();
int litbit();
void litcharoctet();
parbre creenoeudarbre();
void affichebit();
void decodage();
void chargeoctet();

	/******************************************/
	/*					  */
	/*		FONCTION MAIN()		  */
	/*					  */
	/******************************************/
FILE *pcmp,*pdcmp;
char nom[NBLET];
unsigned int dernierbit;
long longueur;
unsigned char caractere,octet,B,A=128;
int bitcourant;
parbre position;
int nboctet;


main()
{

	/*ouverture fichier compress‚*/
	printf("\n\n*******ENTRER NOM FICHIER COMPRESSE*******\n\n");
	scanf("\n%s",nom);
	pcmp=fopen(nom,MODE1);
	if(pcmp==NULL)
		printf("\nERREUR:fichier non trouv‚");
	else
	{
		/*magouille*/
		B=(A>>3)|(A>>4)|(A>>6);
		rewind(pcmp);	/*positionnement au d‚but*/

		racine=NULL;

		/*lecture de la longueur fichier compress‚*/

		fread(&longueur,sizeof(longueur),1,pcmp);
		printf("\nlongueur fichier compress‚: %d",longueur);

		nboctet=sizeof(longueur);

		if(longueur!=0)
		{
		/*lecture 1er octet de codage*/
		chargeoctet();

		/*lecture dernierbit*/
		bitcourant=2;
		dernierbit=octet>>(7-bitcourant);
		printf("\ndernier bit significatif: %d",dernierbit);

		/*reconstitution de l'arbre par lecture debut fichier compress‚*/
		bitcourant=3;
		reconstitution_arbre(NULL);

		/*affichage de l'arbre*/
		/*position=tete;
		printf("\n\nadresse racine: %d\n\n",racine);
		while(position!=NULL)
		{
			printf("\n%d %c %d %d ",position,position->caractere,
			position->succg,position->succd);
			position=position->suivant;
		}*/

		/*ouverture fichier decompress‚*/
		printf("\n\n*******ENTRER NOM FICHIER DECOMPRESSION*******\n\n");
		scanf("\n%s",nom);
		pdcmp=fopen(nom,MODE2);
		if(pdcmp==NULL)
			printf("\nERREUR:un fichier du mˆme nom existe d‚j…");
		else
		{
			rewind(pdcmp);	/*positionnement au d‚but*/
			decodage();
			fclose(pdcmp);
		}
		}
		fclose(pcmp);
	}
}

		/****************************************/
		/**************FONCTIONS*****************/
		/****************************************/

/**********************FONCTION RECONSTITUTION_ARBRE*********************/

void reconstitution_arbre(p)
parbre p;
{
	int i;
	/*printf("\reconstitutionarbre");*/
	if(p==NULL)
	{
		p=(parbre)malloc(sizeof(arbre));
		tete=p;
		tete->suivant=NULL;
	}
	i=litbit();
	if(i==0)
	{
                litcharoctet();
		p->caractere=caractere;
		/*printf(" %c",caractere);*/
		p->succg=NULL;
		p->succd=NULL;
	}
	else
	{
		p->succg=creenoeudarbre();
		reconstitution_arbre(p->succg);
		p->succd=creenoeudarbre();
		reconstitution_arbre(p->succd);
	}
	racine=p;
}

/********************FONCTION LITBIT()****************************/

int litbit()
{
	unsigned char car;
	int tampon;
	char b;
	/*printf("\nlitbit");*/
	car=octet;
	/*printf("\n bitcourant %d",bitcourant);
	b=getchar();*/
	tampon=bitcourant;
	if(bitcourant!=7)
		bitcourant++;
	else
	{
		bitcourant=0;
		chargeoctet();
	}
	/*printf("\ntest %d %d",A>>tampon,(A>>tampon)&car);*/
	if(((A>>tampon)&car)==(A>>tampon))
	{
		/*printf(" litbit retourne 1");*/
		return(1);
	}
	else
	{
		/*printf(" litbit retourne 0");*/
		return(0);
	}
}

/*******************FONCTION LITCHAROCTET()******************************/

void litcharoctet()
{
	/*printf("\litcharoctet");*/
	if(bitcourant==0)
	{
		caractere=octet;
		chargeoctet();
	}
	else
	{
		caractere=octet<<bitcourant;
		chargeoctet();
		caractere=caractere^(octet>>(8-bitcourant));
	}
	/*affichebit(caractere);*/
}


/**************************FONCTION CREENOEUDARBRE()************************/
parbre creenoeudarbre()
{
	parbre adresse;
	/*printf("\ncree noeud arbre");*/
	adresse=(parbre)malloc(sizeof(arbre));
	adresse->suivant=tete;
	tete=adresse;
	return(adresse);
}
/*************************FONCTION AFFICHENVOI()****************************/

void affichebit(b)
unsigned char b;
{
	int i=0;
	char a[9];
	a[8]='\0';
	while(i!=8)
	{
		if(((b)&(A>>i))==(A>>i))
		a[i]='1';
		else
		a[i]='0';
		i++;
	}
	printf(" %d",a);

}
/*****************FONCTION DECODAGE()*************************************/
void decodage()
{
	int i;
	parbre p;
	char b;
	p=racine;
	while(((nboctet==longueur)&&(bitcourant==dernierbit))==0)
	{
		i=litbit();
		/*b=getchar();
		printf(" i=%d; ",i);*/
		if(i==0)
			p=p->succd;
		else
			p=p->succg;
		if((p->succg==NULL)&&(p->succd==NULL))
		{
			fprintf(pdcmp,"%c",p->caractere);
			/*printf("\n%c",p->caractere);*/
			p=racine;
		}
	}
}
/***********************FONCTION CHARGEOCTET()*****************************/
void chargeoctet()
{
	int a,b;
	char z;
	a=ftell(pcmp);
	/*printf("\npositionnement: %d ",a);*/
	b=fread(&octet,sizeof(octet),1,pcmp);
	/*printf(" retour: %d ",b);*/
	/*if(b==0)
		octet=B;
	rewind(pcmp);
	fseek(pcmp,a+1,0);*/
	nboctet++;
	/*printf(" charge octet. ");*/
	/*affichebit(octet);
	z=getchar();*/
}