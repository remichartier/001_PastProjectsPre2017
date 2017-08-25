#include <stdio.h>
#include <string.h>

#define MODE_ECRITURE 	"ab"				/* pas écrasement si existe déjà 	*/
#define MODE_LECTURE    "rb"

void decoder_octet_delta();
/***************************************************************
*	fonction ouvrir_fichier					*
*								*
*	rôle: 	ouvre un fichier  				*
*		test erreur d'ouverture	                        *
*		positionnement en début de fichier		*								*
*	données en entrées :	pointeur sur fichier            *
*				mode d'ouverture		*
*	données en sortie :	pointeur sur fichier ouvert	*
*	retour :		1 si tout est ok		*
*				0 si erreur ouverture		*
****************************************************************/

int ouvrir_fichier(file,mode)
FILE **file;
char mode[];

{
	char filename[15];
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
		rewind(*file);				/*place en début de fichier */
		return(1);
	}
}

main()
{
	FILE *file;
        int c;

	ouvrir_fichier(&file,MODE_LECTURE);
	rewind(file);
	while(getchar()!='q')
	{
		c=getc(file);
		printf("%x",c);
	}
	fclose(file);
}
