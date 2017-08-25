#include<stdio.h>
#include <stdlib.h>
#define N1 9
#define N2 4
#define N3 10
#define NULLE -1
int pions[N1]={1,2,3,4,5,6,7,8,9};
int comb[N2]={1,2,3,4};



void saisie(c)
int c[];
{
	int compteur=0;
	while(compteur<N2)
	{
		printf("\n\n    Entrez le pion nø %d\n",compteur+1);
		scanf("%d",&c[compteur]);
		compteur++;
	}
}



int test_validite(b)
int b[N2];
{
	int i,j;
	for(i=0;i<N2;i++)
	{
		j=0;
		while(j<N1)
		{
			if(b[i]==pions[j])
			j=N1+1;
			else
			j++;
		}
		if(j==N1)
		return(0);
	}
	return(1);
}



void cpy(a,b)
int a[],b[];
{
       int i;
	for(i=0;i<N2;i++)
	a[i]=b[i];


}









void stockage(nb,b,c,d)
int nb,b[][N2],c[],d[][N2+2];
{
	cpy(b,comb);
	cpy(&d[nb][0],c);
	cpy(b[1],c);
}


void comparaison(a)
int a[][N2];
{
	int ligne,colonne1,colonne2;
	ligne=2;
	for(colonne1=0;colonne1<N2;colonne1++)
	a[ligne][colonne1]=NULLE;
	colonne1=0;
	ligne=1;
	while(colonne1<N2)
	{
		if(a[ligne][colonne1]==a[ligne-1][colonne1])
		{
			a[ligne+1][colonne1]=1;
			a[ligne][colonne1]=NULLE;
			a[ligne-1][colonne1]=NULLE;
		}
		colonne1++;
	}
	colonne2=0;
	while(colonne2<N2)
	{
		if(a[ligne][colonne2]!=NULLE)
		{
			colonne1=0;
			while(colonne1<N2)
			{
			if(a[ligne][colonne2]==a[ligne-1][colonne1])
			{
				a[ligne+1][colonne2]=0;
				a[ligne][colonne2]=NULLE;
				a[ligne-1][colonne1]=NULLE;
			}
				colonne1++;
			}
		}
		colonne2++;
	}
}



int interpretation(nb,a,b)
int nb,a[][N2],b[][N2+2];
{
	int colonne, bienplace,malplace;
	bienplace=0;
	malplace=0;
	for(colonne=0;colonne<N2;colonne++)
	{
		if(a[2][colonne]==1)
		bienplace++;
		if(a[2][colonne]==0)
		malplace++;
	}
	printf("malplace %d",malplace);
	b[nb][N2]=bienplace;
	b[nb][N2+1]=malplace;
	if(bienplace==N2)
	return(1);
	else
	return(0);
}




void impression(nb,a)
int nb,a[][N2+2];
{
	int ligne,colonne;
	printf("\n\n  COMBINAISON             BIENPLACE\tMALPLACE\n\n");
	for(ligne=1;ligne<=nb;ligne++)
	{
		for(colonne=0;colonne<N2;colonne++)
			printf("%2d",a[ligne][colonne]);
		printf("\t");
		for(colonne=N2;colonne<N2+2;colonne++)
		printf("\t\t%d",a[ligne][colonne]);

		printf("\n");
	}
}



void combin()  /**creation d' une combinaison aleatoire (bonne combinaison)***/
{
	int i;
	for(i=0;i<N2;i++)
	comb[i] = (random(N1+1));
}



main()
{
	int i=0,comb_proposee[N2],nbessai;
	int compare[N3][N2],tabresult[N3+1][N2+2];
	nbessai=0;
	combin();
	do
	{
		saisie(comb_proposee);
		nbessai++;
		stockage(nbessai,compare,comb_proposee,tabresult);
		comparaison(compare);
		for(i=0;i<N2;i++)
		printf("%d ",compare[2][i]);
		i=interpretation(nbessai,compare,tabresult);
		impression(nbessai,tabresult);
	}
	while((i==0)&&(nbessai<N3));
	if(i==1)
	printf("\ngagn‚\n");
	else
	printf("perdu");
}
