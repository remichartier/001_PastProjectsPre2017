
// RECOIT4.C


// R‚mi CHARTIER MAI 1994


#include <stdio.h>
#include <stdlib.h>	// pour ldiv
#include <time.h>
#include <conio.h>      // pour clrscr
#include <alloc.h>	// pour farmalloc
#include <dos.h>
#include "recoit4.h"

void transfert_memoire_buffer_16bits( short far *, short far *, unsigned int );
void transfert_port_buffer_16bits( short far *, unsigned int ) ;


int main()
{


// declarations initialisations

unsigned long		taille_buffer_o ;
unsigned long		taille_fichier_o ;
unsigned int		nbelements ;
unsigned int		taille_buffer_ko ;
unsigned short int	nbblocs ;
unsigned short int	maxwr = 65500u ;
unsigned int		retour ;
short far		*buffer1 ;
short far		*buffer2 ;
int			i = 0 ;
unsigned char		mode ;
unsigned char		taille_port ;
double			temps;
ldiv_t			nb_write ;
time_t			time1, time2 ;
FILE			*pfile, *presults ;


// entr‚e des paramŠtres taille fichier et buffer

printf( "transfert provenant : \n1 : d'un port ?\n2 : de la m‚moire ? : " ) ;
mode = getchar() ;
fflush( stdin ) ;
printf( "\ntransmission de donn‚es se fera par :\n1 : mots de 16 bits ?\n2 : doubles mots de 32 bits ? : " ) ;
scanf("%c",&taille_port) ;
fflush( stdin ) ;
printf( "\ntaille buffer n * ko : n= " ) ;
scanf( "%u", &taille_buffer_ko ) ;
printf( "\nnombre de blocs n : n= " ) ;
scanf( "%u", &nbblocs ) ;

taille_buffer_o = taille_buffer_ko * 1024L ;
taille_fichier_o = taille_buffer_o * nbblocs ;


// creation buffer d'acquisition

if ( ( buffer1 = ( short far * ) farmalloc( taille_buffer_o ) ) == NULL )
{
	perror( "erreur1.1 allocation dynamique du buffer d'acquisition\n" ) ;
	return( 1 ) ;
}


// creation buffer d'acquisition

if ( mode != '1' )
if ( ( buffer2 = ( short far * ) farmalloc( taille_buffer_o ) ) == NULL )
{
	perror( "erreur1.2 allocation dynamique du buffer d'acquisition\n" ) ;
	return( 1 ) ;
}


// ouverture du fichier sur disque

if( ( pfile = fopen( "filedisk", "wb" ) ) == 0 )
{
	perror( "erreur2 ouverture fichier FILEDISK\n" ) ;
	return( 1 ) ;
}


// positionnement au d‚but de fichier

rewind( pfile ) ;


// transfert sur le disque

nb_write =	ldiv( taille_buffer_o, maxwr ) ;
nbelements =	( unsigned int ) ( taille_buffer_o / 2 ) ;
if ( taille_port == '2' )
	nbelements = (unsigned int ) ( nbelements / 2 ) ;


// mise en route timer

time1 = time( NULL ) ;


// boucle lecture m‚moire ‚criture fichier / disque dur

for( i=0; i < nbblocs; i++ )			// traite autant de buffer de taille maximale possible
{					// donc pour chaque buffer


	// acquisition buffer complet

	if ( mode == '1' )
		if (taille_port == '1' )
			//transfert_port_buffer_16bits( buffer1, nbelements );
			rec16(buffer1,(unsigned long)nbelements);
		else
			//transfert_port_buffer_32bits(
			  //	( unsigned long far * ) buffer1,
			    //	nbelements ) ;
			rec32(buffer1,(unsigned long)nbelements);
	else
		if (taille_port == '1' )
			transfert_memoire_buffer_16bits( buffer1,
						buffer2,
						nbelements ) ;
		else
			transfert_memoire_buffer_32bits( ( unsigned long far * ) buffer1,
						( unsigned long far * ) buffer2,
						nbelements ) ;


	// ‚criture sur disque

	if ( nb_write.quot != 0 )	//si le buffer est plus grand que 65535
	{				//ecriture du maximum de fois de 65535 octets en une seule fois
		;/*retour = fwrite( buffer1, maxwr,
				 ( unsigned int ) nb_write.quot, pfile ) ;
		if ( retour != nb_write.quot )
		{
			perror( "erreur3 en ‚criture du buffer(n' a pas tout ‚crit)" ) ;
			printf(" %ld\n ", nb_write.quot ) ;
			return( 1 ) ;
		}*/
	}

	if ( nb_write.rem != 0 )	// si en d‚coupant en blocs de 65535 octets, il en reste encore dans le buffer, on ‚crit le reste
	{				// ‚criture de la fin du buffer sur le disque

		;/*retour = fwrite (
			buffer1 + maxwr * (unsigned int ) nb_write.quot,
			( unsigned int ) nb_write.rem, 1, pfile ) ;
		if( retour != 1 )
		{
			perror( "erreur4 en ‚criture du buffer(n' a pas tout ‚crit)" ) ;
			return( 1 ) ;
		}*/
	 }

}


// arrˆt timer

time2 = time( NULL ) ;


// fermeture du fichier

fclose( pfile );


// calcul dur‚e

temps = difftime( time2, time1 ) ;


// ‚criture des temps dans un fichier

if( ( presults = fopen( "wrdsk.res", "a+" ) ) == 0 )
{
	perror( "erreur5 ouverture fichier RESULTS\n" ) ;
	return( 1 ) ;
}

if( temps != 0.0 )
{
	fprintf( pfile, "%10lu%15lu%19.2lf%21.3f\n",
	 taille_fichier_o ,
	 taille_buffer_o ,
	 temps ,
	 taille_fichier_o * 8 / ( 1000 * temps ) ) ;
	 printf( "%10lu%15lu%19.2lf%21.3f\n",
	 taille_fichier_o ,
	 taille_buffer_o ,
	 temps ,
	 taille_fichier_o * 8 / ( 1000 * temps ) ) ;
}
else
	perror( "erreur6 calcul de temps" ) ;

fclose( presults ) ;
farfree( buffer1 ) ;
farfree( buffer2 ) ;


// valeur de retour de la fonction main

getch( ) ;
return( 0 ) ;
}



void transfert_memoire_buffer_16bits(
		short far *buffer1 ,
		short far *buffer2 ,
		unsigned int nbelements )
{
	asm push es
	asm push ds
	asm les di,buffer1
	asm lds si,buffer2
	asm mov cx,nbelements	// voir sur des longs nbelements si ca marche
	asm rep movsw


	asm pop ds
	asm pop es
}



void transfert_port_buffer_16bits(
		short far *buffer1 ,
		unsigned int nbelements )
{
	asm les di, buffer1
	asm mov dx, 0x3fc
	asm mov cx,nbelements
	asm rep insw
}
