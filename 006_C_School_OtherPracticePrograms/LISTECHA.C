/******************************************************************/
/*         tp11 ann‚e 1  janvier 1992                             */
/******************************************************************/




/*janvier 1992*/
/*fonctionne,mais peut etre perfectionn‚*/




#include <stdio.h>
#include <string.h>

#define MAX 20

void init_liste(),imprime(),insertion(),ajout_elt_tete(),ajout_elt_apres();
void elt_suivant(),premier_elt();
int fin_liste();

typedef struct
{	int annee;
	int mois;
	int jour;
}DATE;


typedef struct
{	char nom[MAX];
	char prenom[MAX];
	DATE birthdate;
}INDIVIDU;


 struct individuchaine
{	INDIVIDU individu;
	struct individuchaine *suivant;
};
typedef  struct  individuchaine INDIVIDUCHAINE;

INDIVIDU lire_elt_courant();

typedef INDIVIDUCHAINE *PINDIVIDUCHAINE;

PINDIVIDUCHAINE tete,courant;

/*************************************************************************/
/*		programme source					 */
/*************************************************************************/
main()
{
	int e=0;
	INDIVIDU elt;

	init_liste();
	while(e!=4)
	{
	printf("\n\n********************************************\n\n");
	printf("\n               ***CHOIX***\n");
	printf("\n\t1-->initialiser la liste.");
	printf("\n\t2-->remplir la liste.");
	printf("\n\t3-->imprimer la liste.\n");
	printf("\n\t4-->sortir.\n");
	printf("\n\n            VOTRE CHOIX ? :\n\n");
	scanf("\n%d",&e);
	if(e==1)
		init_liste();
	if (e==2)
	{
		printf("\nentrer le nom: ");
		scanf("%s",elt.nom);
		printf("\n______ le prenom: ");
		scanf("%s",elt.prenom);
		printf("\n______ __ jour de naissance: ");
		scanf("%d",&elt.birthdate.jour);
		printf("\n______ __ mois: ");
		scanf("%d",&elt.birthdate.mois);
		printf("\n______ l' ann‚e: ");
		scanf("%d",&elt.birthdate.annee);

		if (tete==NULL)
			ajout_elt_tete(elt);
		else
			insertion(elt);
		printf("%s",elt.nom);
	}
	if (e==3)
		imprime();
	}
}







/*************************************************************************/
/*		fonctions        					 */
/*************************************************************************/

void init_liste()
{
	tete=NULL;
	courant=NULL;
	printf("\nok init_liste()\n");
}

INDIVIDU lire_elt_courant()
{
	INDIVIDU a;
	a=(*courant).individu;
	return(a);
}

void elt_suivant()
{
	courant=(*courant).suivant;
}

void premier_elt()
{
	courant=tete;
}

void ajout_elt_apres(elt)
INDIVIDU elt;
{
	PINDIVIDUCHAINE p;
	p=(PINDIVIDUCHAINE)malloc(sizeof(INDIVIDUCHAINE));
	(*p).suivant=(*courant).suivant;
	(*courant).suivant=p;
	(*p).individu=elt;
}

void ajout_elt_tete(elmt)
INDIVIDU elmt;
{
	PINDIVIDUCHAINE p;
	printf("%s",elmt.nom);
	p=(PINDIVIDUCHAINE)malloc(sizeof(INDIVIDUCHAINE));
	if (tete==NULL)
	{
		tete=p;
		(*tete).suivant=NULL;
	}
	else
	{
		(*p).suivant=tete;
		tete=p;
	}
	(*tete).individu=elmt;
	courant=tete;
}

int fin_liste()
{
	if((*courant).suivant==NULL)
		return(1);
	else
		return(0);
}

void insertion(elt)
INDIVIDU elt;
{
	PINDIVIDUCHAINE place;
	premier_elt();
	while ((courant!=NULL)&&(strcmp((*courant).individu.nom,elt.nom)<=0))
	{
		place=courant;
		elt_suivant();
	}
	if(courant==tete)
	{
		ajout_elt_tete(elt);
		printf("ok");
	}
	else
	{
		courant=place;
		ajout_elt_apres(elt);
	}
}

void imprime()
{       INDIVIDU a;
	premier_elt();
	printf("\n\nNOM          PRENOM      DATE DE NAISSANCE\n\n\n");
	while(courant!=NULL)
	{
		a=lire_elt_courant();
		printf("%s     %s     %d %d %d\n",a.nom,a.prenom,a.birthdate.jour,a.birthdate.mois,a.birthdate.annee);
		elt_suivant();
	}
}



