/************************************************************************************************
*												*
*				programme codage/decodage delta						*
*												*
*	R�mi CHARTIER	Isabelle MILLET		St�phane MATT					*
*									Juin 1993		*
*************************************************************************************************/



#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fonction.h"

#define MODE_ECRITURE 	"ab"				/* pas �crasement si existe d�j� 	*/
#define MODE_LECTURE    "rb"



void decoder(FILE *file_source,
	FILE *file_dest,
	FILE *file_rest,
	unsigned char delta_min,
	unsigned char delta_max,
	float alpha_plus,
	float alpha_moins)
{
	
	int test,position;
	unsigned char *delta;					/* test retour de fonctions */
	unsigned char numero_bloc;
	double longueur;					/* longueur d'un bloc	*/
	int bloc_precedant=0;				/* numero bloc pr�c�dant */
	int first_data=1;			/* sert � tester si 1�re donn�e �chantillonn�e */
	unsigned char dernier_echantillon=0;			/* n�c�ssaire codage delta		*/
	
	int *dernier_d=1;
	int *premier_octet;
	float coef_approx=1.0;
	unsigned char *x,*x_,*y,*y_;
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
							/* recopie ent�te		*/
	recopier(file_source,file_dest,(double)26);
	rewind(file_source);
        recopier(file_source,file_rest,(double)26);
							/* plac� au d�but des donn�es */
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

}

void coder_s(FILE *file_source,
	     FILE *file_dest,
	     unsigned char delta)
{
	
	int test;					/* test retour de fonctions */
	char numero_bloc;
	double longueur;					/* longueur d'un bloc	*/
	int bloc_precedant=0;				/* numero bloc pr�c�dant */
	int first_data=1;			/* sert � tester si 1�re donn�e �chantillonn�e */
        unsigned char dernier_echantillon;			/* n�c�ssaire codage delta		*/
	float coef_approx=1.0;
	unsigned char somme_delta=0;
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



/***************************************************************
*	fonction ouvrir_fichier					*
*								*
*	r�le: 	ouvre un fichier  				*
*		test erreur d'ouverture	                        *
*		positionnement en d�but de fichier		*								*
*	donn�es en entr�es :	pointeur sur fichier            *
*				mode d'ouverture		*
*	donn�es en sortie :	pointeur sur fichier ouvert	*
*	retour :		1 si tout est ok		*
*				0 si erreur ouverture		*
****************************************************************/

int ouvrir_fichier(FILE **file,char mode[])
{
	char filename[50];
	if(strcmp(mode,MODE_ECRITURE)==0)
	{
		printf("nom du fichier destination\n");
	}
	else
	{
		if(strcmp(mode,MODE_LECTURE)==0)
		{
			printf("nom du fichier source\n");
		}
		else
			return(0);                      /* erreur dans mode			*/
	}
	scanf("\n%s",filename);
	*file=fopen(filename,mode);
	if(*file==NULL)					/* cas erreur ouverture */
		return(0);
	else
	{
		rewind(*file);				/*place en d�but de fichier */
		return(1);
	}
}



/***************************************************************
*	fonction recopier					*
*								*
*	r�le: 	recopie des octets d'un fichier a un autre	*
*	donn�es en entr�es :	pointeur sur les 2 fichiers     *
*				longueur en octets � recopier	*
*	donn�es en sortie :	pointeur sur les 2 fichiers	*
*	retour :		rien				*
*	remarque : 		en appelant la fonction, les 	*
*				positions dans les fichiers	*
*				doivent d�j� �tre aux endroits	*
*				ad�quates			*
****************************************************************/
void recopier(FILE *file_source,FILE *file_dest,double longueur)
{
	char buffer[256];
	fread(buffer,sizeof(char),longueur,file_source);
	fwrite(buffer,sizeof(char),longueur,file_dest);
}


/***************************************************************
*	fonction lire_no_bloc					*
*								*
*	r�le: 	lire le numero d'un bloc			*
*		on doit �tre positionn� au 1er octet du bloc    *
*			en lan�ant cette fonction		*
*		repositionnement en d�but de bloc		*								*
*	donn�es en entr�es :	pointeur sur fichier            *
*	retour :		numero de bloc lu		*
****************************************************************/
char lire_no_bloc(FILE *file)
{
	char numero;
	double position;
	position=ftell(file);
	numero=getc(file);
	fseek(file,position,0);				/* se remet d�but bloc */
	return(numero);
}

/***************************************************************
*	fonction lire_longueur_bloc				*
*								*
*	r�le: 	lit longueur d'un bloc 				*
*		repositionnement en d�but de bloc		*								*
*	donn�es en entr�es :	pointeur sur fichier            *
*	retour :		longueur du bloc lue		*
****************************************************************/
double lire_longueur_bloc(FILE *file)
{
	double position;
	double longueur=0;
	int octet[3];
	int i=0;
	position=ftell(file);
							/* positionnement 1er octet de longueur */
	fseek(file,1,1);
	while(i<3)
	{	octet[i]=getc(file);
		i++;
	}
	longueur=(double)octet[2]*65536;
	longueur+=(double)octet[0];
	longueur+=256*(double)octet[1];
							/* repositionnement debut bloc */
	fseek(file,position,0);
	return(longueur);
}


/***************************************************************
*	fonction coder_octet_delta				*
*								*
*	r�le: 	code 1 octet �chantillon� du bloc 		*
*		   en 1 bit par modulation delta                *
*	donn�es en entr�es :	pointeurs sur fichier source et *
*					destination             *
*				dernier �chantillon pris 	*
*				octet servant � coder		*
*				dernier bit significatif	*
*	retour :		rien				*
****************************************************************/
void coder_octet_delta(FILE *file_source,
			FILE *file_dest,
			unsigned char *dernier_echantillon,
			unsigned char *octet,
			char *bit,
			float coef_approx,
			unsigned char *somme_delta,
			unsigned char delta)
{
	char octet_lu;
	unsigned char c=0x80; 					/* cod� comme 1000 0000		*/
	fread(&octet_lu,sizeof(octet_lu),1,file_source);
	if/*((*/(octet_lu<=*somme_delta/*+delta*/)/*&&(*somme_delta-delta<*somme_delta))
          ||((octet_lu>*somme_delta + delta)&&(*somme_delta+delta<*somme_delta)))*/
	{                                               /* cod� en -delta		*/
		(*bit)=(*bit)+1;					/* revient � mettre 0		*/
                (*somme_delta)=(*somme_delta)- delta;
	}
	else
	{						/* cod� en +delta		*/
		(*octet)=(*octet)|(c>>(*bit));		/* ajoute un 1 � la bonne place	 */
		(*bit)=(*bit)+1;
		(*somme_delta)=(*somme_delta) + delta;
	}
	/**dernier_echantillon=(char)(coef_approx*(float)octet_lu);*/
	if(*bit==8)
	{
		fwrite(octet,sizeof(*octet),1,file_dest);
		*octet=0;
		*bit=0;
	}
	/*printf("%d, %d\n",octet_lu,*somme_delta);
	getchar(); */
}
/***************************************************************
*	fonction coder_octet_delta_diff				*
*								*
*	r�le: 	code 1 octet �chantillon� du bloc 		*
*		   en 1 bit par modulation delta                *
*	donn�es en entr�es :	pointeurs sur fichier source et *
*					destination             *
*				dernier �chantillon pris 	*
*				octet servant � coder		*
*				dernier bit significatif	*
*	retour :		rien				*
****************************************************************/
void coder_octet_delta_diff(FILE *file_source,
		FILE *file_dest,
		unsigned char *dernier_echantillon,
		unsigned char *octet,
		char *bit,
		float coef_approx,
		unsigned char *somme_delta,
		unsigned char *delta,
		unsigned char delta_min,
		unsigned char delta_max,
		float alpha_plus,
		float alpha_moins,
		int *dernier_d,
		int *premier_octet)
{
	int d;
	unsigned char octet_lu;
	unsigned char c=0x80; 					/* cod� comme 1000 0000		*/
	fread(&octet_lu,sizeof(octet_lu),1,file_source);
	if(*premier_octet==1)
	{
		*delta=delta_min;
		*premier_octet=0;
	}
	if/*((*/(octet_lu<=*somme_delta/*+*delta*/)/*&&(*somme_delta-*delta>*somme_delta))
	  ||((octet_lu>*somme_delta+*delta)&&(*somme_delta+*delta<*somme_delta)))*/
	{
		d=0;                                          /* cod� en -delta		*/
		(*bit)=(*bit)+1;					/* revient � mettre 0		*/
                (*somme_delta)=(*somme_delta)- (*delta);
	}
	else
	{
		d=1;					/* cod� en +delta		*/
		(*octet)=(*octet)|(c>>(*bit));		/* ajoute un 1 � la bonne place	 */
		(*bit)=(*bit)+1;
                (*somme_delta)=(*somme_delta) + (*delta);
	}
	/**dernier_echantillon=(char)(coef_approx*(float)octet_lu);*/
	if(*bit==8)
	{
		fwrite(octet,sizeof(*octet),1,file_dest);
		*octet=0;
		*bit=0;
	}
	/* choix du pas*/
	if(*dernier_d==d)
		*delta=(unsigned char)(alpha_plus*(float)(*delta));
	else
		*delta=(unsigned char)(alpha_moins*(float)(*delta));
				/* choix des deltas	*/
	if(*delta<delta_min)
		*delta=delta_min;
	if(*delta>delta_max)
		*delta=delta_max;
	*dernier_d=d;
	/*printf("%d, %d\n",octet_lu,*somme_delta);
	getchar(); */
}
/***************************************************************
*	fonction coder_longueur					*
*								*
*	r�le: 	code longueur en trois octets	 		*
*	donn�es en entr�es :	longueur � coder		*
*	donn�es en sortie :	tab contenant longueur cod�e    *
*	retour :		rien				*
****************************************************************/
void coder_longueur(double longueur,char tab[])
{
	int var;
	var=(int)(longueur/65536);
	tab[2]=(char)var;
	var=(int)((longueur-var*65536)/256);
	tab[1]=(char)var;
	tab[0]=longueur-tab[1]*256-tab[2]*65536;
}


/***************************************************************
*	fonction coder_bloc_delta				*
*								*
*	r�le: 	code donn�es �chantillon�es du bloc 		*
*			par modulation delta                    *
*	donn�es en entr�es :	pointeurs sur fichier source et *
*					destination             *
*				type de bloc � coder (1 ou 2)	*
*				type bloc pr�c�dant		*
*				dernier �chantillon pris 	*
*				first data:si 1er � coder ou pas*
*	retour :		rien				*
****************************************************************/
void coder_bloc_delta(FILE *file_source,
	FILE *file_dest,
	int type_bloc,
	int type_bloc_precedant,
	unsigned char *dernier_echantillon,
	int *first_data,
	float coef_approx,
	unsigned char *somme_delta,
	unsigned char delta)
{
	double longueur,new_longueur;			/* longueur initiale et finale du bloc	*/
	double nbbits;
	unsigned char bit;
	int i;
	unsigned char octet_longueur[3];				/* sert � codage nouvelle longueur bloc	*/
	unsigned unsigned char octet; 				/* octet contenant codage delta		*/
	longueur=lire_longueur_bloc(file_source);
        						/* recopier 1er octet			*/
	recopier(file_source,file_dest,(double)1);
	i=ftell(file_source);						/* calcul nouvelle longueur		*/
	if((type_bloc_precedant==0)||			/* si faut pas changer 1er octet	*/
		(type_bloc_precedant==6)||
		(type_bloc_precedant==3)||
                (*first_data==1))
		nbbits=(longueur-1)+8;			/* 1 octet->1bit sauf 1er octet		*/
	else
		nbbits=longueur;
	if(type_bloc==1)
		nbbits=nbbits-2+16;			/* pas oublier SR et compactage		*/
	if((((int)(nbbits/8))*8)==nbbits) 		/* si nbbbits=0[8]			*/
		i=1; 					/* rajoute dernier octet bit significatif*/
	else
		i=2; 					/* rajoute en + octet fin codage	 */
	new_longueur=(int)(nbbits/8)+i;
        coder_longueur(new_longueur,octet_longueur);	/* code et �crit longueur su 3 octets	*/
	 i=fwrite(octet_longueur,sizeof(octet_longueur[0]),3,file_dest);                             /* r��crit nouvelle longueur		*/
	fseek(file_source,3,1);

	i=ftell(file_dest);
	if(type_bloc==1)
	{
		recopier(file_source,file_dest,(double)2);
		longueur=longueur-2;
	}
	if((type_bloc_precedant==0)||			/* si faut pas changer 1er octet	*/
		(type_bloc_precedant==6)||
		(type_bloc_precedant==3)||
                (*first_data==1))
	{
		fread(dernier_echantillon,sizeof(*dernier_echantillon),1,file_source);
		fwrite(dernier_echantillon,sizeof(*dernier_echantillon),1,file_dest);
		longueur=longueur-1;

		if(*first_data==1)
		{
			*first_data=0;			/* 1er octet pass�			*/
			*somme_delta=*dernier_echantillon;/* fin du if				*/
                }
        }

        bit=0;					/* initialisation dernier bit utilis�	*/
	octet=0;					/* initialisation octet codage=0000 0000*/

	while(longueur!=0)
	{
		coder_octet_delta(file_source,file_dest,dernier_echantillon,
		&octet,&bit,coef_approx,somme_delta,delta);
		longueur--;
		i=ftell(file_source);
	}
	i=ftell(file_source);
	if(bit!=0)
		fwrite(&octet,sizeof(octet),1,file_dest);/* �criture fi )r     bloc		*/
	fwrite(&bit,sizeof(bit),1,file_dest);		 /* �criture dernier bit significatif	*/
	i=ftell(file_source);
}							/* fin fonction coder_bloc_delta	*/
/***************************************************************
*	fonction coder_bloc_delta_diff				*
*								*
*	r�le: 	code donn�es �chantillon�es du bloc 		*
*			par modulation delta                    *
*	donn�es en entr�es :	pointeurs sur fichier source et *
*					destination             *
*				type de bloc � coder (1 ou 2)	*
*				type bloc pr�c�dant		*
*				dernier �chantillon pris 	*
*				first data:si 1er � coder ou pas*
*	retour :		rien				*
****************************************************************/

void coder_bloc_delta_diff(FILE *file_source,
	FILE *file_dest,
	int type_bloc,
	int type_bloc_precedant,
	unsigned char *dernier_echantillon,
	int *first_data,
	float coef_approx,
	unsigned char *somme_delta,
	unsigned char *delta,
	unsigned char delta_min,
	unsigned char delta_max,
	float alpha_plus,
	float alpha_moins,
	int *dernier_d,
	int *premier_octet)
{
	double longueur,new_longueur;			/* longueur initiale et finale du bloc	*/
	double nbbits;
	unsigned char bit;
	int i;
	unsigned char octet_longueur[3];				/* sert � codage nouvelle longueur bloc	*/
	unsigned char octet; 				/* octet contenant codage delta		*/
	longueur=lire_longueur_bloc(file_source);
        						/* recopier 1er octet			*/
	recopier(file_source,file_dest,(double)1);
	i=ftell(file_source);						/* calcul nouvelle longueur		*/
	if((type_bloc_precedant==0)||			/* si faut pas changer 1er octet	*/
		(type_bloc_precedant==6)||
		(type_bloc_precedant==3)||
                (*first_data==1))
		nbbits=(longueur-1)+8;			/* 1 octet->1bit sauf 1er octet		*/
	else
		nbbits=longueur;
	if(type_bloc==1)
		nbbits=nbbits-2+16;			/* pas oublier SR et compactage		*/
	if((((int)(nbbits/8))*8)==nbbits) 		/* si nbbbits=0[8]			*/
		i=1; 					/* rajoute dernier octet bit significatif*/
	else
		i=2; 					/* rajoute en + octet fin codage	 */
	new_longueur=(int)(nbbits/8)+i;
        coder_longueur(new_longueur,octet_longueur);	/* code et �crit longueur su 3 octets	*/
	 i=fwrite(octet_longueur,sizeof(octet_longueur[0]),3,file_dest);                             /* r��crit nouvelle longueur		*/
	fseek(file_source,3,1);

	i=ftell(file_dest);
	if(type_bloc==1)
	{
		recopier(file_source,file_dest,(double)2);
		longueur=longueur-2;
	}
	if((type_bloc_precedant==0)||			/* si faut pas changer 1er octet	*/
		(type_bloc_precedant==6)||
		(type_bloc_precedant==3)||
                (*first_data==1))
	{
		fread(dernier_echantillon,sizeof(*dernier_echantillon),1,file_source);
		fwrite(dernier_echantillon,sizeof(*dernier_echantillon),1,file_dest);
		longueur=longueur-1;

		if(*first_data==1)
		{
			*first_data=0;			/* 1er octet pass�			*/
			*somme_delta=*dernier_echantillon;/* fin du if				*/
                        *premier_octet=1;
                }
        }

        bit=0;					/* initialisation dernier bit utilis�	*/
	octet=0;					/* initialisation octet codage=0000 0000*/

	while(longueur!=0)
	{
		coder_octet_delta_diff(file_source,
					file_dest,
					dernier_echantillon,
					&octet,
					&bit,
					coef_approx,
					somme_delta,
					delta,
					delta_min,
					delta_max,
					alpha_plus,
					alpha_moins,
					dernier_d,
					premier_octet);
		longueur--;
		i=ftell(file_source);
	}
	i=ftell(file_source);
	if(bit!=0)
		fwrite(&octet,sizeof(octet),1,file_dest);
	fwrite(&bit,sizeof(bit),1,file_dest);
	i=ftell(file_source);
}


void affichebit(unsigned unsigned char a)
{
	unsigned char A=128;
	unsigned char s[9];
	int n=7;
	s[8]='\0';
	while(n!=-1)
	{
		if((a&(A>>(7-n)))==(A>>(7-n)))
			s[7-n]='1';
		else
			s[7-n]='0';
		n=n-1;
	}
	printf( "%s",s);
}

int test_version_fichier(FILE *file)
{
	int test=0,i;
	char texte[20];
	unsigned char a;
	rewind(file);
	fread(texte,sizeof(char),19,file);
	texte[19]='\0';
	if(strcmp(texte,"Creative Voice File")!=0)
		printf("1er test n�gatif\n"),test++;
	fread(&a,sizeof(a),1,file);
	if(a!=0x1A)
		printf("2nd test n�gatif\n"),test++;
        fread(&a,sizeof(a),1,file);
	if(a!=0x1a)
		printf("3�me test n�gatif\n"),test++;
	fread(&a,sizeof(a),1,file);
	if(a!=0x00)
		printf("4�me test n�gatif\n"),test++;
	return(test);
}

/****************************************************************
*	fonction  decoder_bloc_delta				*
*								*
*	role: decode les donnees codees des blocs 1 et 2        *
*								*
*	donnees en entree :   pointeurs sur les fichiers source *
*					et destination		*
*			      type du bloc a decoder (1 ou 2)   *
*			      type du bloc precedant		*
*			      dernier echantillon pris		*
*			      first data: donnee initiale	*
*								*
*	retour : 	      rien				*
****************************************************************/
void decoder_bloc_delta(FILE *file_source,FILE *file_dest,int type_bloc,int type_bloc_precedant,
			unsigned char *dernier_echantillon,int *first_data,unsigned char delta)
{
double longueur,new_longueur,compteur,cour;
int  pos_dernier_bit;
unsigned char octet;
char tab[3];

longueur=lire_longueur_bloc(file_source);               /*   file_dest                   */
recopier(file_source,file_dest,(double)1);
compteur=longueur;                                      /* compteur represente le nombre */
							/* d'octets de donnees qui restent*/
                                                        /* a parcourir dans file_source  */

cour=ftell(file_source);				/* bloc courant de file_source   */
fseek(file_source,longueur+3-1,1);   			/* positionnement sur dernier	 */
							/* octet du champs de donnees	 */
pos_dernier_bit=getc(file_source);
fseek(file_source,cour,0);    				/* retour a l'octet 5 du bloc	 */
if(type_bloc==1)
	longueur=longueur-2;
if(pos_dernier_bit!=0)
	new_longueur=(longueur-2)*8+pos_dernier_bit;
else
	new_longueur=(longueur-1);
if ((type_bloc_precedant==0)||    			/* si &er octet du bloc cod� */
	(type_bloc_precedant==3)||
	(type_bloc_precedant==6)||
	((*first_data)==1))
	new_longueur=new_longueur -8 +1;
if(type_bloc==1)
{
	new_longueur=new_longueur+2;
	longueur=longueur+2;
}

coder_longueur(new_longueur,tab);			/* codage sur 3 octets		 */
fwrite(&tab[0],sizeof(char),1,file_dest);		/* ecriture de la nouvelle 	 */
fwrite(&tab[1],sizeof(char),1,file_dest); 		/* longueur sur les 3 octets     */
fwrite(&tab[2],sizeof(char),1,file_dest);		/* n�2,3 et 4 prevus.            */

fseek(file_source,3,1);
if (type_bloc==1)					/* si on decode un bloc1         */
{
	recopier(file_source,file_dest,(double)2);		/* recopie des octets SR et de   */
							/* compactage dans file_dest     */
	compteur=compteur-2;
}

if ((type_bloc_precedant==0)||
	(type_bloc_precedant==3)||
	(type_bloc_precedant==6)||
	((*first_data)==1))
{
	fread(dernier_echantillon,sizeof(*dernier_echantillon),1,file_source);
							/* recopie du 1er octet de donnees*/
	fwrite(dernier_echantillon,sizeof(*dernier_echantillon),1,file_dest);
	compteur--;
	if ((*first_data)==1) (*first_data)=0;          /* on reinitialise first_data     */
}

while(compteur>1)
{
	fread(&octet,sizeof(octet),1,file_source);	/* lecture d'un octet de donnees  */
        compteur--;
	if ((compteur==1)&&(pos_dernier_bit!=0))
		decoder_octet_delta(file_dest,dernier_echantillon,octet,pos_dernier_bit,delta);
							/* on decode le dernier octet     */
	else
		decoder_octet_delta(file_dest,dernier_echantillon,octet,(int)8,delta);       /* on decode l'octet lu           */

}

fseek(file_source,1,1);					/* positionnement en fin de bloc  */
							/* dans le fichier source         */

}




/****************************************************************
*	fonction decoder_octet_delta				*
*                                                               *
*	role :	decode n bits d'un octet de donnees d'un bloc 1 *
*		ou 2 du fichier source; ecrit le resultat du    *
*		decodage dans le champs donnees du bloc de      *
*		file_destination				*
*								*
*	donnees en entree : pointeur sur le fichier destination *
*			    le dernier echantillon		*
*			    l'octet considere                   *
*			    nombre de bits de l'octet a decoder *
*								*
*	retour : 	    rien				*
*								*
****************************************************************/
void decoder_octet_delta(FILE *file_dest,unsigned char *dernier_echantillon,unsigned char octet,int n,unsigned char delta)
{
int j,d;
float coef_approx=1;
for (j=0;j<n;j++)
{

	d=(octet>>(7-j))&1;                            /* d est le bit n�(8-j) de l'octet */
	if(d==0)
	{
		if(coef_approx*((double)(*dernier_echantillon))-(delta)>(double)(0))                   /* on doit retrancher delta a l'echantillon precedent */
			(*dernier_echantillon)=(unsigned char)(coef_approx*(float)(*dernier_echantillon))-(delta);
		else
			(*dernier_echantillon)=(unsigned char)(0);
	}
	else
	{
		if (coef_approx*((double)(*dernier_echantillon))+(delta)<(double)(255))      					     /* on ajoute delta a l'echantillon precedent */
			(*dernier_echantillon)=(unsigned char)(coef_approx*(float)(*dernier_echantillon))+(delta);
		else
			(*dernier_echantillon)=(unsigned char)255;
	}
	/*printf("%d\n",*dernier_echantillon);
	getchar();*/
	fwrite(dernier_echantillon,sizeof(*dernier_echantillon),1,file_dest);
} 				         /* ecriture du nouvel echantillon dans file_dest */
}

/****************************************************************
*	fonction  decoder_bloc_delta_diff				*
*								*
*	role: decode les donnees codees des blocs 1 et 2        *
*								*
*	donnees en entree :   pointeurs sur les fichiers source *
*					et destination		*
*			      type du bloc a decoder (1 ou 2)   *
*			      type du bloc precedant		*
*			      dernier echantillon pris		*
*			      first data: donnee initiale	*
*								*
*	retour : 	      rien				*
****************************************************************/
void decoder_bloc_delta_diff(FILE *file_source,FILE *file_dest,FILE *file_rest,int type_bloc,int type_bloc_precedant,
			unsigned char *dernier_echantillon,int *first_data,
			unsigned char *delta,
			unsigned char delta_min,
			unsigned char delta_max,
			float alpha_plus,
			float alpha_moins,
			int *dernier_d,
			int *premier_octet,
			float coef_approx,
			unsigned char *x,
			unsigned char *x_,
			unsigned char *y,
			unsigned char *y_)
{
double longueur,new_longueur,compteur,cour;
int  pos_dernier_bit;
unsigned char octet;
char tab[3];
int position;

position=ftell(file_source);
longueur=lire_longueur_bloc(file_source);               /*   file_dest                   */
recopier(file_source,file_dest,(double)1);
fseek(file_source,position,0);
recopier(file_source,file_rest,(double)1);

compteur=longueur;                                      /* compteur represente le nombre */
							/* d'octets de donnees qui restent*/
                                                        /* a parcourir dans file_source  */

cour=ftell(file_source);				/* bloc courant de file_source   */
fseek(file_source,longueur+3-1,1);   			/* positionnement sur dernier	 */
							/* octet du champs de donnees	 */
pos_dernier_bit=getc(file_source);
fseek(file_source,cour,0);    				/* retour a l'octet 5 du bloc	 */
if(type_bloc==1)
	longueur=longueur-2;
if(pos_dernier_bit!=0)
	new_longueur=(longueur-2)*8+pos_dernier_bit;
else
	new_longueur=(longueur-1);
if ((type_bloc_precedant==0)||    			/* si &er octet du bloc cod� */
	(type_bloc_precedant==3)||
	(type_bloc_precedant==6)||
	((*first_data)==1))
	new_longueur=new_longueur -8 +1;
if(type_bloc==1)
{
	new_longueur=new_longueur+2;
	longueur=longueur+2;
}

coder_longueur(new_longueur,tab);			/* codage sur 3 octets		 */
fwrite(&tab[0],sizeof(char),1,file_dest);		/* ecriture de la nouvelle 	 */
fwrite(&tab[1],sizeof(char),1,file_dest); 		/* longueur sur les 3 octets     */
fwrite(&tab[2],sizeof(char),1,file_dest);		/* n�2,3 et 4 prevus.            */
fwrite(tab,sizeof(char),3,file_rest);
fseek(file_source,3,1);
if (type_bloc==1)					/* si on decode un bloc1         */
{
	position=ftell(file_source);
	recopier(file_source,file_dest,(double)2);		/* recopie des octets SR et de   */
	fseek(file_source,position,0);
        recopier(file_source,file_rest,(double)2);
							/* compactage dans file_dest     */
	compteur=compteur-2;
}

if ((type_bloc_precedant==0)||
	(type_bloc_precedant==3)||
	(type_bloc_precedant==6)||
	((*first_data)==1))
{
	fread(dernier_echantillon,sizeof(*dernier_echantillon),1,file_source);
							/* recopie du 1er octet de donnees*/
	fwrite(dernier_echantillon,sizeof(*dernier_echantillon),1,file_dest);
	compteur--;
	*x=*dernier_echantillon;
	*x_=0;
	*y_=0;
	*y=fonction_filtre(*x,*x_,*y_);
        fwrite(y,sizeof(*y),1,file_rest);
	if ((*first_data)==1)
	{
		 (*first_data)=0;          /* on reinitialise first_data     */
		 (*premier_octet)=1;
        }
}

while(compteur>1)
{
	fread(&octet,sizeof(octet),1,file_source);	/* lecture d'un octet de donnees  */
        compteur--;
	if ((compteur==1)&&(pos_dernier_bit!=0))
		decoder_octet_delta_diff(file_dest,file_rest,dernier_echantillon,octet,pos_dernier_bit,
			delta,
			delta_min,
			delta_max,
			alpha_plus,
			alpha_moins,
			dernier_d,
			premier_octet,
			coef_approx,x,x_,y,y_);
							/* on decode le dernier octet     */
	else
		decoder_octet_delta_diff(file_dest,file_rest,dernier_echantillon,octet,(int)8,
			delta,
			delta_min,
			delta_max,
			alpha_plus,
			alpha_moins,
			dernier_d,
			premier_octet,
			coef_approx,
			x,x_,y,y_);       /* on decode l'octet lu           */

}

fseek(file_source,1,1);					/* positionnement en fin de bloc  */
							/* dans le fichier source         */

}




/****************************************************************
*	fonction decoder_octet_delta_diff			*
*                                                               *
*	role :	decode n bits d'un octet de donnees d'un bloc 1 *
*		ou 2 du fichier source; ecrit le resultat du    *
*		decodage dans le champs donnees du bloc de      *
*		file_destination				*
*								*
*	donnees en entree : pointeur sur le fichier destination *
*			    le dernier echantillon		*
*			    l'octet considere                   *
*			    nombre de bits de l'octet a decoder *
*								*
*	retour : 	    rien				*
*								*
****************************************************************/
void decoder_octet_delta_diff(FILE *file_dest,
			FILE *file_rest,
			unsigned char *dernier_echantillon,
			unsigned char octet,
			int n,
			unsigned char *delta,
			unsigned char delta_min,
			unsigned char delta_max,
			float alpha_plus,
			float alpha_moins,
			int *dernier_d,
			int *premier_octet,
			float coef_approx,
			unsigned char *x,unsigned char *x_,
			unsigned char *y,unsigned char *y_)
{
int j,d;
for (j=0;j<n;j++)
{
	d=(octet>>(7-j))&1;                            /* d est le bit n�(8-j) de l'octet */
					/* 1er d�codage : comme delta � pas fixe */
	if(*premier_octet==1)
	{
		*delta=delta_min;
		*premier_octet=0;
	}
	else
	{               		/* choix des alpha */
		if(*dernier_d==d)
			*delta=(unsigned char)(alpha_plus*(float)(*delta));
		else
			*delta=(unsigned char)(alpha_moins*(float)(*delta));
					/* choix des deltas	*/
		if(*delta<delta_min)
			*delta=delta_min;
		if(*delta>delta_max)
			*delta=delta_max;
	}			/* fin du else	*/
	if(d==0)
	{
		if((double)((coef_approx*(float)(*dernier_echantillon))-(*delta))>0)
			(*dernier_echantillon)=(unsigned char)(coef_approx*(float)(*dernier_echantillon))-(*delta);
		else
			(*dernier_echantillon)=(unsigned char)(0);
	}
	else
	{
		if((double)((coef_approx*(float)(*dernier_echantillon))+(*delta))<255)
			(*dernier_echantillon)=(unsigned char)(coef_approx*(float)(*dernier_echantillon))+(*delta);
		else
			(*dernier_echantillon)=(unsigned char)255;
	}
	*dernier_d=d;
	/*printf("%d\n",*dernier_echantillon);
	getchar();*/


	*y_=*y;
		*x_=*x;
		*x=(*dernier_echantillon);
		*y=fonction_filtre(*x,*x_,*y_);
	fwrite(dernier_echantillon,sizeof(*dernier_echantillon),1,file_dest);
        fwrite(y,sizeof(*y),1,file_rest);
} 				         /* ecriture du nouvel echantillon dans file_dest */
}

unsigned char echantillon_sinus(double t,double f)
{
	double PI=3.141592654;
	double result;
	unsigned char octet;
	result=127+cos(2*PI*f*t)*127;
	octet=(unsigned char)(int)result;
	printf("%d\n",octet);
	getchar();
	return(octet);
}

unsigned char fonction_filtre(unsigned char x,unsigned char x_,unsigned char y_)
{
	float K=.9226264;
	float a0=1/K;
	float a1=.84525/K;
	float b1=1.0;
	unsigned char result;

	result=(unsigned char)
		(a0*(float)x +a1*(float)x_ -b1*(float)y_);
	return(result);
}





