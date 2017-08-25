// remi chartier, le dimanche 4 septembre 1994

// programme de compression maximale

#include <stdio.h>
#include <time.h>
#include <alloc.h>	// pour farmalloc, farcoreleft, ...
#include <process.h>	// pour exit
#include <stdlib.h>	// pour exit
#include <conio.h>	// pour getch

#define MAXREAD		64512

// declaration des fonctions
void CompressionOctet (
			unsigned char 		Octet,
			unsigned char *		CompressedData
			) ;

void CompressionBit (
			unsigned char		Octet,
			unsigned char *		CompressedData,
			unsigned char		NumeroBit
			) ;

void FormatBinaire (
			unsigned char OctetAConvertir,
			char *		Octet
			) ;

unsigned char LectureEtCompression (
				unsigned long LongueurLecture,
				long PositionLecture,
				FILE * pFileSource,
				unsigned char far * pDonneesACompresser,
				unsigned char * DonneesCompressees );

unsigned char LectureDonnees (
		unsigned short int 	LongueurALire,
		unsigned char far *	PositionMemoire,
		FILE *			pFile
		) ;


int main ( int argc, char ** argv )
  {

  FILE * 		pFileSource ;
  FILE * 		pFileDestination ;
  long 			retour ;
  long			LongueurFichier = 0 ;
  unsigned char		OctetLu ;
  unsigned char		DonneesCompressees = 0 ;
  char 			OctetConverti[9] ;
  time_t		StartTop ;
  time_t		StopTop ;
  double		Temps ;
  long 			i ;
  unsigned char far *	pDonneesACompresser ;
  unsigned long		MemoireDisponible ;
  unsigned long		NombreLectures ;
  unsigned long		LecturesRestantes ;

  if ( argc != 3 )
    {
    perror ( " 1. Pas assez d'arguments " ) ;
    return 0 ;
    }

  // ouverture fichier à compresser
  pFileSource = fopen ( argv[1], "rb" ) ;
  if ( pFileSource == NULL )
    {
    perror ( " 2. Erreur ouverture fichier a compresser " ) ;
    return 0 ;
    }

  // positionnement en fin de fichier
  retour = fseek ( pFileSource, 0, 2 ) ;
  if ( retour != 0 )
    {
    perror ( " 3. Erreur dans positionnement " ) ;
    return 0 ;
    }

  // determination longueur de fichier
  LongueurFichier = ftell ( pFileSource ) ;
  printf ( "\nlongueur du fichier : %ld octets \n", LongueurFichier ) ;
  //getch () ;

  // Allocation memoire pour transfert donnees du disque dur vers le fichier

  MemoireDisponible = farcoreleft () ;
  printf ( "MemoireDisponible : %ld\n", MemoireDisponible ) ;
  //getch () ;

  MemoireDisponible = MemoireDisponible - ( MemoireDisponible % 1024 ) ; // prend une memoire en ko pour rapidite d'execution
  printf ( "MemoireDisponible : %ld\n", MemoireDisponible ) ;
  //getch () ;

  NombreLectures = LongueurFichier / MemoireDisponible ;
  printf ( "NombreLectures : %ld\n", NombreLectures ) ;
  //getch () ;

  LecturesRestantes = LongueurFichier % MemoireDisponible ;
  printf ( "LecturesRestantes : %ld\n", LecturesRestantes ) ;
  //getch () ;

  if ( NombreLectures != 0 )
    pDonneesACompresser = ( unsigned char far * ) farmalloc ( MemoireDisponible ) ;
  else
    pDonneesACompresser = ( unsigned char far * ) farmalloc ( LecturesRestantes ) ;
  if ( pDonneesACompresser == NULL )
    {
    perror ( " 3-2. Erreur dans allocation dynamique de memoire far " ) ;
    return 0 ;
    }



  // debut chronometrage
  StartTop = time ( NULL ) ;

  if ( LecturesRestantes != 0 )
    {
    retour = LectureEtCompression (	LecturesRestantes,
				( long ) ( NombreLectures * MemoireDisponible ),
				pFileSource,
				pDonneesACompresser,
				&DonneesCompressees
				) ;
    if ( retour == 0 )
      {
      perror( " 3-3. Erreur dans LectureEtCompression" ) ;
      return 0 ;
      }
    }

  while ( NombreLectures >= 1 )
    {
      retour = LectureEtCompression (
				MemoireDisponible,
				( long ) ( ( NombreLectures - 1 ) * MemoireDisponible ),
				pFileSource,
				pDonneesACompresser,
				&DonneesCompressees
				) ;
      if ( retour == 0 )
      {
	perror( " 3-4. Erreur dans LectureEtCompression " ) ;
	return 0 ;
      }

      NombreLectures -- ;
    } // fin du while ( NombreLectures >= 1 )



  // Arret du chronometrage
  StopTop = time ( NULL ) ;
  Temps = difftime ( StopTop, StartTop ) ;
  printf( "%ld octets compresses en %f secondes\n", LongueurFichier, Temps ) ;

  // Ecriture du fichier compresse
  fclose ( pFileSource ) ;
  pFileDestination = fopen ( argv[2], "wb" ) ;
  if ( pFileDestination == NULL )
    {
    perror ( " 5. Erreur ouverture fichier compresser " ) ;
    return 0 ;
    }

  // positionnement au debut du fichier
  rewind ( pFileDestination ) ;

  // ecriture du fichier compresse
  retour = fwrite ( &LongueurFichier, sizeof ( long ), 1, pFileDestination ) ;
  if ( retour != 1 )
    {
    perror ( " 6. Erreur en ecriture " ) ;
    return 0 ;
    }
  retour = fwrite ( &DonneesCompressees, 1, 1, pFileDestination ) ;
  if ( retour != 1 )
    {
    perror ( " 7. Erreur en ecriture " ) ;
    return 0 ;
    }

  // Fermeture des fichiers
  fclose ( pFileDestination ) ;
  farfree ( pDonneesACompresser ) ;

  return 1 ;
  } // fin du main ( )


void CompressionOctet (
			unsigned char 		Octet,
			unsigned char *		CompressedData
			)

  {

  unsigned char i ;
  char OctetConverti[9] ;

  //FormatBinaire ( *CompressedData, OctetConverti ) ;
  //printf ( "Entree fonction CompressionOctet\n" ) ;
  //getch () ;

  //for ( i = 0; i < 8; i ++ )
    //{
      //CompressionBit ( Octet, CompressedData, i ) ;
      //FormatBinaire ( *CompressedData, OctetConverti ) ;
      //printf ( "  %s\r ", OctetConverti ) ;
      //getch() ;
    //}

  //printf ( "Fin fonction Compression Octet\n" ) ;
  //getch () ;

  }  // fin CompressionOctet



void CompressionBit (
			unsigned char		Octet,
			unsigned char *		CompressedData,
			unsigned char		NumeroBit
			)
  {

  static unsigned char SommePlusBitOut = 0 ;
  static unsigned char PreviousBitOut = 0 ;
  unsigned char NewBit ;
  unsigned char BitOut ;

  //printf ( "Entree fonction CompressionBit\n" ) ;
  //getch () ;

  NewBit =  Octet & ( 1 << NumeroBit ) ;
  if ( NewBit != 0 )
    NewBit = 2 ;

  BitOut = *CompressedData & ( 1 << 7 ) ;
  if ( BitOut != 0 )
    BitOut = 1 ;

  SommePlusBitOut = ( SommePlusBitOut + PreviousBitOut + ( NewBit >> 1 ) ) % 2 ;

  *CompressedData =
	( ( *CompressedData << 1 ) & 0xFC ) |
	NewBit |
	SommePlusBitOut ;

  PreviousBitOut = BitOut ;

  //printf ( "Fin fonction CompressionBit\n" ) ;
  //getch () ;

  } // fin fonction CompressionBit()



void FormatBinaire (
			unsigned char OctetAConvertir,
			char * Octet
			)
  {

  // declaration de variables internes
  unsigned char i ;

  Octet[8] = '\0' ;

  for ( i=0; i<8; i++ )
    {
    if ( ( OctetAConvertir & ( 1<<i ) ) != 0 )
      Octet[7-i] = '1' ;
    else
      Octet[7-i] = '0' ;
    }
  } // fin fontion FormatBinaire



unsigned char LectureEtCompression (
				unsigned long LongueurLecture,
				long PositionLecture,
				FILE * pFileSource,
				unsigned char far * pDonneesACompresser,
				unsigned char * DonneesCompressees )
  {
  unsigned short int	Nombre63Ko ;
  unsigned short int	RestantDe63Ko ;
  unsigned long		retour ;
  unsigned long 	i ;

  //printf ( "Entree fonction LectureEtCompression\n" ) ;
  //getch () ;

  // positionnement dans fichier
  retour = fseek ( pFileSource, PositionLecture , 0 ) ;
  if ( retour != 0 )
    {
    perror ( " 3. Erreur dans positionnement " ) ;
    exit ( 0 ) ;
    }

  // Calcul du nombre de lecture a faire par buffer

  printf ( "LongueurLecture %lu\n",LongueurLecture ) ;
  //getch () ;

  Nombre63Ko = ( unsigned short int ) ( LongueurLecture / ( 64512L ) ) ;
  printf ( "Nombre63Ko : %u\n", Nombre63Ko ) ;
  //getch () ;

  RestantDe63Ko = ( unsigned short int ) ( LongueurLecture % ( 64512L ) ) ;
  printf ( "RestantDe63Ko : %u\n", RestantDe63Ko ) ;
  //getch () ;

  // lecture des donnees a compresser
  i = 0 ;
  while ( i < Nombre63Ko )
    {
    retour = LectureDonnees ( MAXREAD,
				( unsigned char far * )( pDonneesACompresser + i * 63 * 1024 ),
				pFileSource ) ;
    if ( retour == 0 )
      {
      perror ( "4. Erreur de lecture " ) ;
      exit ( 0 ) ;
      }
    i ++ ;
    } //fin du while ( i < Nombre63Ko )
  if ( RestantDe63Ko != 0 )
    {
    retour = LectureDonnees ( 	RestantDe63Ko,
			( unsigned char far * )( pDonneesACompresser + Nombre63Ko * 63 * 1024 ),
			pFileSource ) ;
    if ( retour == 0 )
      {
      perror ( "4. Erreur de lecture " ) ;
      return ( 0 ) ;
      }
    }

  // traitement de l'octet lu
  i = 0 ;
  while ( i<LongueurLecture )
    {
    //printf ( " %#04x Octet lu\n", OctetLu ) ;
    //FormatBinaire ( OctetLu, OctetConverti ) ;
    //printf ( "\n%s Octet lu\n", OctetConverti ) ;
    CompressionOctet ( *( pDonneesACompresser + i ), DonneesCompressees ) ;
    //getch () ;
    i++ ;
    }  // fin du while ( i<LongueurLecture )

    //printf ( "Fin fonction LectureEtCompression\n" ) ;
    //getch () ;

    return ( 1 ) ;
 }  // fin fonction LectureEtCompression





unsigned char LectureDonnees (
		unsigned short int 	LongueurALire,
		unsigned char far *	PositionMemoire,
		FILE *			pFile
		)
  {
  unsigned char retour ;

  //printf ("Entree fonction LectureDonnees\n" ) ;
  printf ( "LongueurALire : %u\n",LongueurALire ) ;
  //getch () ;

  retour = fread ( PositionMemoire, LongueurALire, 1, pFile ) ;
  if ( retour != 1 )
    {
    perror ( " 4-2. Erreur de lecture " ) ;
    return ( 0 ) ;
    }
  //printf ( "Fin fonction LectureDonnees\n" ) ;
  return ( 1 ) ;
  }


 // brouillon
 /*
 i = LongueurFichier ;
 // boucle de lecture des octets 1 par 1 et traitement
  while ( i >= 1 )
    {

    // positionnement dans fichier
    retour = fseek ( pFileSource, -nbOctetsLus-1 , 2 ) ;
    if ( retour != 0 )
      {
      perror ( " 3. Erreur dans positionnement " ) ;
      return 0 ;
      }

    // lecture de l'octet a compresser
    retour = fread ( &OctetLu, 1, 1, pFileSource ) ;
    if ( retour != 1 )
      {
      perror ( " 4. Erreur de lecture " ) ;
      return 0 ;
      }

    nbOctetsLus ++ ;

    // traitement de l'octet lu
//    printf ( " %#04x Octet lu\n", OctetLu ) ;
//    FormatBinaire ( OctetLu, OctetConverti ) ;
    //printf ( "\n%s Octet lu\n", OctetConverti ) ;
    CompressionOctet ( OctetLu, &DonneesCompressees ) ;
    //getch () ;


    // decrementation de la boucle
    i -- ;

    } // fin de la boucle while ( LongueurFichier >= 0 )

*/ // fin du brouillon