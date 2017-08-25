#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MODE1	"a+"
#define MODE2	"w"
#define MAXLET	26

		/***************************************/
		/*	DEFINITION DES STRUCTURES      */
		/***************************************/

struct NOEUD
{
	char mot[MAXLET];
	struct NOEUD *traduction;
	struct NOEUD *suivant;
};

typedef struct NOEUD noeud;
typedef noeud *pnoeud;

struct NOEUD1
{
	pnoeud adresse;
	struct NOEUD1 *suivant;
};

typedef struct NOEUD1 noeud1;
typedef noeud1 *pnoeud1;



		/***************************************/
		/*	DECLARATION DES FONCTIONS      */
		/***************************************/

void chargedico();
int dicovide();
int menu0();
void ajoutermot();
void traduiremot();
pnoeud1 synmot();
int afficherliste();
void modifierdico();
void savedico();
void chargedico();
void ordonne();
pnoeud cherchetrad();
int menu1();
void printtab();
void effacer();
void questionnaire();
void questionnaire_alphabetique();
void destroydoublons();





		/***********************************************/
		/*	DECLARATION DE VARIABLES GLOBALES      */
		/***********************************************/

FILE *pdico;
char foreignlang[MAXLET];
pnoeud francais,etranger,queuef,queuee;
int longueur=0;


		/*****************************/
		/*	FONCTION MAIN()      */
		/*****************************/

main()
{
	/*d‚claration variables*/
	int choix;
	char langueetrangere[MAXLET];
	char car;
	char c;
	pnoeud pointeur;
	pnoeud1 synonyme;
	char s[MAXLET],s0[MAXLET];

	/*initialisations*/
	francais=NULL;
	etranger=NULL;
	queuef=NULL;
	queuee=NULL;

	printf("\nQuelle langue ‚trangŠre voulez vous consulter?: \n");
	gets(foreignlang);
	strncpy(langueetrangere,foreignlang,8);
	strcat(langueetrangere,".dic");

	/*ouverture du fichier dictionnaire*/
	pdico=fopen(langueetrangere,MODE1);
	if (pdico==NULL)
	{
	   printf("\nImpossible d'ouvrir ce fichier.\nVeuiller v‚rifier	le chemin de ce fichier");
	}
	else
	{
	   rewind(pdico);
	   chargedico();

	   /*test sur le contenu du dico*/
	   /*printtab();*/
	   do
	   {
	      choix=1;
	      if(dicovide())
		printf("\nLe dictionnaire est vide.\nLa seule possibilit‚ est de le remplir\n");
	      choix=menu0();
	      if(dicovide())
		ajoutermot();
	      else
	      {
		 switch(choix)
		 {
		    case(1):
			traduiremot();
			break;
		    case(2):
			do
			{
			ajoutermot();
                        printf("\nEntrer 2 pour sortir ou une autre touche pour continuer ");
			car=getchar();
			}while(car!='2');
			break;
		    case(6):
			questionnaire();
			break;
		    case(4):
                        printf("\nEntrer le mot: ");
			getchar();
			gets(s);
			printf("\nEst-ce fran‡ais ou %s ?: ",foreignlang);
			scanf("\n%s",s0);
			if((strcmp(s0,"fran‡ais")!=0)&&(strcmp(s0,foreignlang)!=0))
			printf("\nerreur,la langue indiqu‚e n'est ni fran‡ais ni %s\n",foreignlang);
			else
				synonyme=synmot(s,s0);
			if(synonyme==NULL)
			printf("\nCe mot n'a aucun synonyme pr‚sent dans le dictionnaire");
			else
			{
			printf("\nles synonymes de %s sont: ",s);
			while(synonyme!=NULL)
			{
			printf("\n%s\t%s",synonyme->adresse->mot,synonyme->adresse->traduction->mot);
			synonyme=synonyme->suivant;
			}
			}
			break;
		    case(5):
			afficherliste();
			break;
		    case(3):
			modifierdico();
			break;
		    case(10):
			printf("\nEtes-vous s–r? (Y/N): ");

			break;
		    case(7):
			questionnaire_alphabetique();
			break;
		    default:
			printf("\nNe r‚pondez pas … cot‚ !");
			break;
		 }
	      }
	      if(choix==10)
	      {
	      scanf("\n%c",&choix);
	      if (choix=='y'||choix=='Y')
	      {
	      choix=11;
	      }
	      }
	   }
	   while(choix!=11);
	   savedico(langueetrangere);
	   fclose(pdico);
	}
}

		/************************/
		/*	FONCTIONS	*/
		/************************/

/***********************FONCTION MENU0()************************************/
int menu0()
{
	int a=0;
	printf("\n");
	printf("\n\t\t1  -TRADUIRE UN MOT");
	printf("\n\t\t2  -AJOUTER UN MOT");
	printf("\n\t\t3  -MODIFIER LE DICTIONNAIRE");
	printf("\n\t\t4  -CHERCHER SYNONYMES D'UN MOT");
	printf("\n\t\t5  -AFFICHER LA LISTE");
	printf("\n\t\t6  -SUBIR TEST DE VOCABULAIRE");
	printf("\n\t\t7  -SUBIR TEST DE VOCABULAIRE alphab‚tique");
	printf("\n\t\t10 -QUITTER");
	printf("\n\n\tQuel est votre choix? ");
	scanf("\n%d",&a);
	return(a);
}

/***********************FONCTION TRADUIREMOT()******************************/
void traduiremot()
{
	char s[MAXLET],s0[MAXLET];
	pnoeud t;
	printf("\nTraduction d'un mot");
	getchar();
	printf("\nEntrer le mot … traduire\n");
	gets(s);
	printf("\nEst-ce fran‡ais ou %s ?: ",foreignlang);
	scanf("\n%s",s0);
	if((strcmp(s0,"fran‡ais")!=0)&&(strcmp(s0,foreignlang)!=0))
		printf("\nerreur,la langue indiqu‚e n'est ni fran‡ais ni %s\n",foreignlang);
	else
	{
		if(strcmp(s0,"fran‡ais")==0)
			t=francais;
		else
			t=etranger;
		ordonne(&t);
		if((t=cherchetrad(s,t))!=NULL)
			printf("\ntraduction: %s\n",t->traduction->mot);
		else
			printf("\nce mot n'est pas pr‚sent dans le dico\n");
	}
}



/***********************FONCTION AFFICHELISTE()*****************************/
int afficherliste()
{
	int choix,n=0;
	char c;
	pnoeud t;
	ordonne(&francais);
	ordonne(&etranger);
	destroydoublons(francais);
	destroydoublons(etranger);
	choix=menu1();
	printf("\n\nEntrez 7 pour arreter d‚finitivement le parcours\n");
	switch(choix)
	{       case(1):
			t=francais;
			while(t!=NULL)
			{
				printf("\n%s",t->mot);
				n++;
				if(n==10)
				{
				n=0;
				c=getchar();
				if((c=getchar())==7)
				return(0);
				}
				t=t->suivant;

			}
			break;
		case(2):
			t=etranger;
			while(t!=NULL)
			{
				printf("\n%s",t->mot);
				n++;
				if(n==10)
				{
				n=0;
				c=getchar();
				if((c=getchar())==7)
				return(0);
				}
				t=t->suivant;
			}
			break;
		case(3):
			t=francais;
			while(t!=NULL)
			{
				printf("\n%s",t->mot);
				printf("\t%s",t->traduction->mot);
				n++;
				if(n==10)
				{
				n=0;
				c=getchar();
				if((c=getchar())==7)
				return(0);
				}
				t=t->suivant;
			}
			break;
		case(4):
			t=etranger;
			while(t!=NULL)
			{
				printf("\n%s",t->mot);
				printf("\t%s",t->traduction->mot);
				n++;
				if(n==10)
				{
				n=0;
				c=getchar();
				if((c=getchar())==7)
				return(0);
				}
				t=t->suivant;
			}
			break;
		default:
			printf("\nVous avez tap‚ un choix non indiqu‚");
	}
}


/***********************FONCTION MODIFIERDICO()*****************************/
void modifierdico()
{
	pnoeud t;
	char c;
	char s1[MAXLET];
	char s2[MAXLET];
	printf("\n Entrer le mot … modifier en fran‡ais: ");
	getchar();
	gets(s1);
	printf("\nEntrer le mot … modifier en anglais: ");
	getchar();
	gets(s2);
	t=francais;
	while((strcmp(s1,t->mot)!=0)&&(t!=NULL))
		t=t->suivant;
	if(t!=NULL)
	{
		printf("\nModification du mot fran‡ais: ");
		gets(s1);
		printf("\nModification du mot %s: ",foreignlang);
		gets(s2);
		if(strlen(s1)!=0)
			strcpy(t->mot,s1);
		else
			printf("\nPas de modification du mot fran‡ais.");
		if(strlen(s2)!=0)
			strcpy(t->traduction->mot,s2);
		else
			printf("\n\nPas de modification du mot anglais.");
		if((strlen(s1)==0)&&(strlen(s2)==0))
		{
			printf("\nvous voulez peut-ˆtre gommer cet enregistrement? (Y/N): ");
			scanf("\n%c",&c);
			if((c=='y')||(c=='Y'))
			{
				printf("\nEtes-vous s–r? (Y/N): ");
				scanf("\n%c",&c);
				if (c=='y'||c=='Y')
				{
					effacer(t);
				}
			}
		}
	}
	else
		printf("\Cet enregistrement n' est pas dans le dictionnaire");
}

/***********************FONCTION SAVEDICO()*********************************/
void savedico(a)
char a[];
{
	pnoeud t;
	ordonne(&francais);
	/*printtab();*/
	fclose(pdico);
	pdico=fopen(a,MODE2);
	rewind(pdico);
	t=francais;
	while(t!=NULL)
	{
		fputs(t->mot,pdico);
		putc('\n',pdico);
		fputs(t->traduction->mot,pdico);
		putc('\n',pdico) ;
		t=t->suivant;
	}
	fclose(pdico);
}

/***********************FONCTION DICOVIDE()*********************************/
int dicovide()
{
	if(francais==NULL)
	{
		printf("\nLe dictionnaire est vide");
		return(1);
	}
	else
	return(0);
}

/***********************FONCTION CHARGEDICO()*******************************/
void chargedico()
{
	char c;
	int n=0;
	long depl;
	pnoeud pose,posf;
	printf("\n\nchargement du dictionnaire\n");
	francais=(pnoeud)malloc(sizeof(noeud));
	etranger=(pnoeud)malloc(sizeof(noeud));
	queuee=NULL;
	queuef=NULL;
	posf=francais;
	pose=etranger;
	rewind(pdico);
	c=getc(pdico);
	rewind(pdico);
	while((fgets(posf->mot,36,pdico))!=NULL)
	{
		n++;
		posf->mot[strlen(posf->mot)-1]='\0';
		fgets(pose->mot,36,pdico);
		pose->mot[strlen(pose->mot)-1]='\0';
		posf->traduction=pose;
		pose->traduction=posf;
		queuee=pose;
		queuef=posf;
		pose->suivant=(pnoeud)malloc(sizeof(noeud));
		posf->suivant=(pnoeud)malloc(sizeof(noeud));
		pose=pose->suivant;
		posf=posf->suivant;
	}
	if(posf==francais)
	{
		francais=NULL;
		etranger=NULL;
	}
	else
	{
		queuee->suivant=NULL;
		queuef->suivant=NULL;
	}
	printf("\nLe dictionnaire contient %d enregistrements",n);
	longueur=n;
}




/***********************FONCTION CHERCHETRAD()******************************/
pnoeud cherchetrad(s,t)
char s[];
pnoeud t;
{
	do
	{
		if(strcmp(s,t->mot)==0)
			return(t);
		else
			t=t->suivant;

	}while(strcmp(s,t->mot)>=0);
	return(NULL);
}

/***********************FONCTION MENU1()************************************/
int menu1()
{
	int a=0;
	printf("\n");
	printf("\nAFFICHER LA LISTE :");
	printf("\n\t\t1-EN FRANCAIS");
	printf("\n\t\t2-EN %s",foreignlang);
	printf("\n\t\t3-EN FRANCAIS-%s",foreignlang);
	printf("\n\t\t4-EN %s-FRANCAIS\n",foreignlang);
	printf("\nQuel est votre choix? ");
	scanf("\n%d",&a);
	return(a);
}

/***********************FONCTION PRINTTAB()**********************************/
void printtab()
{
	pnoeud t;
	t=francais;
	printf("\n\n\naffichage contenu du dico");
	printf("\n%d",francais);
	while(t!=NULL)
	{
		printf("\n%d %s\t%s %d",t,t->mot,t->traduction->mot,t->suivant);
		t=t->suivant;
	}
	printf("\n\n");
}


/***********************FONCTION AJOUTERMOT()*******************************/
void ajoutermot() 	/*fonctionne*/
{       pnoeud pose,posf;
	char c;
	printf("\nAjout de mots dans le dico\n");
	if(francais==NULL)
	{
		francais=(pnoeud)malloc(sizeof(noeud));
		etranger=(pnoeud)malloc(sizeof(noeud));
		posf=francais;
		pose=etranger;
	}
	else
	{
	queuef->suivant=(pnoeud)malloc(sizeof(noeud));
	queuee->suivant=(pnoeud)malloc(sizeof(noeud));
	pose=queuee->suivant;
	posf=queuef->suivant;
	}
	do
	{
		c=getchar();
		printf("\nEntrer le mot francais: ");
		gets(posf->mot);
		printf("\nEntrer le mot %s:  ",foreignlang);
		gets(pose->mot);
	}
	while((strlen(pose->mot)==0)||(strlen(posf->mot)==0));
	pose->suivant=NULL;
	posf->suivant=NULL;
	pose->traduction=posf;
	posf->traduction=pose;
	queuee=pose;
	queuef=posf;
	longueur++;


}

/***********************FONCTION ORDONNE()**********************************/
void ordonne(t)
pnoeud *t;
{
	pnoeud pointeur,courant,precourant,pos,prepos;
	char mot[MAXLET];
	courant=*t;
	precourant=NULL;
	prepos=precourant;
	pos=courant;
	pointeur=*t;
	strcpy(mot,pos->mot);
	while(courant!=NULL)
	{
		/*minliste(courant,pos,prepos);*/
		while(pointeur->suivant!=NULL)
		{
		if(strcmp(pointeur->suivant->mot,mot)<=0)
		{
			prepos=pointeur;
			pos=pointeur->suivant;
			strcpy(mot,pos->mot);
		}
		pointeur=pointeur->suivant;
		}
	/*permute(pos,prepos,courant,precourant);*/
	if (pos!=courant)
	{
		if(precourant!=NULL)
		{
			precourant->suivant=pos;
			prepos->suivant=pos->suivant;
			pos->suivant=courant;
		}
		else
		{
			*t=pos;
			prepos->suivant=pos->suivant;
			pos->suivant=courant;
		}
		courant=pos;
	}
	precourant=courant;
	courant=courant->suivant;
	pos=courant;
	prepos=precourant;
	strcpy(mot,pos->mot);
	pointeur=courant;
	}
	if(t==&francais)
		queuef=precourant;
	else
		queuee=precourant;
}

/***********************FONCTION SYNMOT()***********************************/
pnoeud1 synmot(s,s0)
char s[MAXLET];
char s0[MAXLET];
{
	pnoeud1 tete=NULL,courant=NULL;
	pnoeud t;
	if(strcmp(s0,"fran‡ais")==0)
		t=francais;
	else
		t=etranger;
	while(t!=NULL)
	{
		if (strcmp(t->mot,s)==0)
		{
		if(tete==NULL)
		{
			tete=(pnoeud1)malloc(sizeof(noeud1));
			courant=tete;
		}
		else
		{
			courant->suivant=(pnoeud1)malloc(sizeof(noeud1));
			courant=courant->suivant;
		}
		courant->adresse=t;
		courant->suivant=NULL;

		}
		t=t->suivant;
	}
	return(tete);
}

/***********************FONCTION EFFACER()***********************************/
void effacer(french)
pnoeud french;
{
	pnoeud pre;
	pnoeud pointeur;
	pre=NULL;


	/*recherche predecesseur noeud fran‡ais*/
	pointeur=francais;
	while(pointeur!=french)
	{
		pre=pointeur;
		pointeur=pointeur->suivant;
	}
	if(pre==NULL)
		francais=french->suivant;
	if(french==queuef)
	{
		pre->suivant=NULL;
		queuef=pre;
	}
	if((french!=queuef)&&(pre!=NULL))
		pre->suivant=french->suivant;
	french=french->traduction;
	pre=NULL;
	pointeur=etranger;
        while(pointeur!=french)
	{
		pre=pointeur;
		pointeur=pointeur->suivant;
	}
	if(pre==NULL)
		etranger=french->suivant;
	if(french==queuee)
	{
		pre->suivant=NULL;
		queuee=pre;
	}
	if((french!=queuee)&&(pre!=NULL))
		pre->suivant=french->suivant;

}
/***********************FONCTION QUESTIONNAIRE()****************************/
void questionnaire()
{
	int nbquestions=0,nb=0;
	float reponsesbonnes=0,n=0;
	int a,b;
	pnoeud1 apprendre=NULL;
	pnoeud1 learn=NULL;
	pnoeud1 synonyme;
	char c;
	pnoeud t;
	pnoeud1 pos;
	int compteur=0;
	char reponse[MAXLET];
	char s0[MAXLET];
	randomize();
	c=getchar();
	printf("\nSi vous voulez cesser les questions, entrez 6\n");
	while(strcmp(reponse,"6")!=0)
	{
		compteur++;
		if(compteur==10)
		{
			compteur=0;
			printf("\nSi vous voulez cesser les questions, entrez 6\n");
		}
		nb=0;
		a=random(longueur);
		b=random(100);
		if(b-2*((int)(b/2))!=0)
		{
			t=etranger;
			strcpy(s0,foreignlang);
		}
		else
		{
			t=francais;
			strcpy(s0,"fran‡ais");
		}
		while(a!=0)
		{
			t=t->suivant;
			a=a-1;
		}
		printf("\n%s: ",t->mot);
		gets(reponse);
		if(strcmp(reponse,"6")!=0)
		{
			if(strcmp(reponse,t->traduction->mot)!=0)
			{
			synonyme=synmot(t->mot,s0);
			/*printf("\n********************");
			printf("%d",synonyme);
			pos=synonyme;
			while(pos!=NULL)
			{
				printf("\nsyn %s",pos->adresse->traduction->mot);
				pos=pos->suivant;
			}
			printf("\n"); */
			while(synonyme!=NULL)
			{
			if(strcmp(synonyme->adresse->traduction->mot,reponse)==0)
			{
				nb=1;
				synonyme=NULL;
			}
			else
				synonyme=synonyme->suivant;
			}
			if(nb==0)
				printf("\n%s %s",t->mot,t->traduction->mot);
			else
				reponsesbonnes++;
			if(apprendre==NULL)
			{
				apprendre=(pnoeud1)malloc(sizeof(noeud1));
				learn=apprendre;
			}
			else
			{
				learn->suivant=(pnoeud1)malloc(sizeof(noeud1));
				learn=learn->suivant;
			}
			learn->adresse=t;
			learn->suivant=NULL;
			}
			else
				reponsesbonnes++;
			nbquestions++;
		}
	}
	printf("\nVous n'avez r‚pondu correctement qu'… %.2f%c des cas sur %d questions",(reponsesbonnes/nbquestions)*100,'%',nbquestions);
	if(apprendre!=NULL)
	{
	printf("\n\nVoici la liste des mots que vous devriez apprendre");
	learn=apprendre;
	while(learn!=NULL)
	{
		printf("\n%s",learn->adresse->mot);
		printf("\t%s",learn->adresse->traduction->mot);
		n++;
		if(n==10)
		{
			n=0;
			c=getchar();
		}
		learn=learn->suivant;
	}
	}
}
/***********************FONCTION QUESTIONNAIRE_ALPHABETIQUE()****************************/
void questionnaire_alphabetique()
{
	int nbquestions=0,nb=0;
	float reponsesbonnes=0,n=0;
	int a,b;
	pnoeud1 apprendre=NULL;
	pnoeud1 learn=NULL;
	pnoeud1 synonyme;
	char c;
	pnoeud t,q;
	pnoeud1 pos;
	int compteur=0;
	char reponse[MAXLET];
	char s0[MAXLET];
	char langue[MAXLET];
	c=getchar();
	printf("\nEn quelle langue voulez vous ˆtre int‚rog‚ : %s ou fran‡ais? ",foreignlang);
	scanf("\n%s",langue);
	if ((strcmp(langue,"francais")==0)||(strcmp(langue,"fran‡ais")==0))
	{
		t=francais;
		q=queuef;
	}
	else
	{
		t=etranger;
		q=queuee;
	}
	printf("\nSi vous voulez cesser les questions, entrez 6\n");
	while((strcmp(reponse,"6")!=0)&&(t!=q))
	{
		compteur++;
		if(compteur==10)
		{
			compteur=0;
			printf("\nSi vous voulez cesser les questions, entrez 6\n");
		}
		nb=0;
		printf("\n%s: ",t->mot);
		gets(reponse);
		if(strcmp(reponse,"6")!=0)
		{
			if(strcmp(reponse,t->traduction->mot)!=0)
			{
			synonyme=synmot(t->mot,s0);
			/*printf("\n********************");
			printf("%d",synonyme);
			pos=synonyme;
			while(pos!=NULL)
			{
				printf("\nsyn %s",pos->adresse->traduction->mot);
				pos=pos->suivant;
			}
			printf("\n"); */
			while(synonyme!=NULL)
			{
			if(strcmp(synonyme->adresse->traduction->mot,reponse)==0)
			{
				nb=1;
				synonyme=NULL;
			}
			else
				synonyme=synonyme->suivant;
			}
			if(nb==0)
				printf("\n%s %s",t->mot,t->traduction->mot);
			else
				reponsesbonnes++;
			if(apprendre==NULL)
			{
				apprendre=(pnoeud1)malloc(sizeof(noeud1));
				learn=apprendre;
			}
			else
			{
				learn->suivant=(pnoeud1)malloc(sizeof(noeud1));
				learn=learn->suivant;
			}
			learn->adresse=t;
			learn->suivant=NULL;
			}
			else
				reponsesbonnes++;
			nbquestions++;
		}
	t=t->suivant;
	}
	printf("\nVous n'avez r‚pondu correctement qu'… %.2f%c des cas sur %d questions",(reponsesbonnes/nbquestions)*100,'%',nbquestions);
	if(apprendre!=NULL)
	{
	printf("\n\nVoici la liste des mots que vous devriez apprendre");
	learn=apprendre;
	while(learn!=NULL)
	{
		printf("\n%s",learn->adresse->mot);
		printf("\t%s",learn->adresse->traduction->mot);
		n++;
		if(n==10)
		{
			n=0;
			c=getchar();
		}
		learn=learn->suivant;
	}
	}
}


/***********************FONCTION DESTROYDOUBLONS()****************************/
void destroydoublons(tete_de_liste)
pnoeud tete_de_liste;
{
	while(tete_de_liste!=NULL)
	{
		if((strcmp(tete_de_liste->mot,tete_de_liste->suivant->mot)==0)&&(strcmp(tete_de_liste->traduction->mot,tete_de_liste->suivant->traduction->mot)==0))
		{
			tete_de_liste->suivant=tete_de_liste->suivant->suivant;
			tete_de_liste->traduction->suivant=tete_de_liste->traduction->suivant->suivant;
		}
		tete_de_liste=tete_de_liste->suivant;
	}
}






