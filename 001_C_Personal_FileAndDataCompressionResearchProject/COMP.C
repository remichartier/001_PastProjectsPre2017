// remi chartier, le dimanche 10 septembre 1994
// voulait faire avec structure speciale pour coder arbre mais mise en place des
//operations elementaires trop long
// programme de compression maximale

#include <stdio.h>
#include <time.h>
#include <alloc.h>	// pour farmalloc, farcoreleft, free ...
#include <process.h>	// pour exit
#include <stdlib.h>	// pour exit
#include <conio.h>	// pour getch

#define MAXREAD		64512

// definition de structures

typedef struct POLYNOME
  {
  unsigned long		Coef ;
  struct POLYNOME *	Next ;
  } Polynome ;			// premier coefficient de la chaine : degre du polynome

typedef struct POLYNOME *	PPolynome ;

typedef struct RETENUE1
  {
  PPolynome		Position ;
  struct RETENUE1 *	Next ;
  } Retenue1 ;

typedef struct RETENUE1 *	pRetenue1 ;


// declaration des fonctions
unsigned char CompressionOctet (
			unsigned char 		Octet,
			PPolynome		pCompressedData
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
				PPolynome pDonneesCompressees
				);

unsigned char LectureDonnees (
		unsigned short int 	LongueurALire,
		unsigned char far *	PositionMemoire,
		FILE *			pFile
		) ;


unsigned char MultiplicationPolynomeX256 (
						PPolynome pCompressedData
						) ;






int main ( int argc, char ** argv )
  {

  FILE * 		pFileSource ;
  FILE * 		pFileDestination ;
  long 			retour ;
  long			LongueurFichier = 0 ;
  unsigned char		OctetLu ;
  PPolynome		pDonneesCompressees ;
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

  // ouverture fichier a compresser
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

  // Allocation memoire pDonneesCompressees
  pDonneesCompressees = ( PPolynome ) malloc ( sizeof ( Polynome ) ) ;

  if ( pDonneesCompressees == NULL )
    {
    perror ( "4. Erreur allocation dynamique de pDonneesCompressees" ) ;
    return 0 ;
    }
  pDonneesCompressees->Coef = 0 ;
  pDonneesCompressees->Next = ( PPolynome ) malloc ( sizeof ( Polynome ) ) ;
  if ( pDonneesCompressees->Next == NULL )
    {
    perror ( "5. Erreur allocation dynamique de pDonneesCompressees" ) ;
    return 0 ;
    }



  // Allocation memoire pour transfert donnees du disque dur vers le fichier

  MemoireDisponible = farcoreleft () ;
  printf ( "MemoireDisponible : %ld\n", MemoireDisponible ) ;
  //getch () ;

  //MemoireDisponible = MemoireDisponible - ( MemoireDisponible % 1024 ) ; // prend une memoire en ko pour rapidite d'execution
  MemoireDisponible = 65536L ;
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
    perror ( " 6. Erreur dans allocation dynamique de memoire far " ) ;
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
				pDonneesCompressees
				) ;
    if ( retour == 0 )
      {
      perror( " 7. Erreur dans LectureEtCompression" ) ;
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
				pDonneesCompressees
				) ;
      if ( retour == 0 )
      {
	perror( " 8. Erreur dans LectureEtCompression " ) ;
	return 0 ;
      }

      NombreLectures -- ;
    } // fin du while ( NombreLectures >= 1 )



  // Arret du chronometrage
  StopTop = time ( NULL ) ;
  Temps = difftime ( StopTop, StartTop ) ;
  printf( "%ld octets compresses en %f secondes\n", LongueurFichier, Temps ) ;

  // Ecriture du fichier compresse
  fclose ( pFileSource ) ; // fermeture ici car sinon provoque une erreur vu qu'il y a beaucoup de memoire allouee
  pFileDestination = fopen ( argv[2], "wb" ) ;
  if ( pFileDestination == NULL )
    {
    perror ( " 9. Erreur ouverture fichier compresser " ) ;
    return 0 ;
    }

  // positionnement au debut du fichier
  rewind ( pFileDestination ) ;

  // ecriture du fichier compresse
  retour = fwrite ( &LongueurFichier, sizeof ( long ), 1, pFileDestination ) ;
  if ( retour != 1 )
    {
    perror ( " 10. Erreur en ecriture " ) ;
    return 0 ;
    }
  retour = fwrite ( pDonneesCompressees,
		 ( pDonneesCompressees->Coef+2 ) * sizeof ( Polynome ),
		 1,
		 pFileDestination
		 ) ;
  if ( retour != 1 )
    {
    perror ( " 11. Erreur en ecriture " ) ;
    return 0 ;
    }

  // Fermeture des fichiers
  fclose ( pFileDestination ) ;

  // Liberation des memoires allouees dynamiquement
  farfree ( pDonneesACompresser ) ;
  free ( pDonneesCompressees ) ;

  return 1 ;
  } // fin du main ( )









unsigned char LectureEtCompression (
				unsigned long LongueurLecture,
				long PositionLecture,
				FILE * pFileSource,
				unsigned char far * pDonneesACompresser,
				PPolynome pDonneesCompressees )
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
    perror ( " 12. Erreur dans positionnement " ) ;
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
      perror ( "13. Erreur de lecture " ) ;
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
      perror ( "14. Erreur de lecture " ) ;
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
    retour = CompressionOctet ( *( pDonneesACompresser + i ), pDonneesCompressees ) ;
    if ( retour == 0 )
      {
      perror ( " 15. erreur dans l'appel de la fonction Compression Octet" ) ;
      return 0 ;
      }

    //getch () ;
    i++ ;
    }  // fin du while ( i<LongueurLecture )

    //printf ( "Fin fonction LectureEtCompression\n" ) ;
    //getch () ;

    return ( 1 ) ;
 }  // fin fonction LectureEtCompression







unsigned char CompressionOctet (
			unsigned char 		Octet,
			PPolynome 		pCompressedData
			)

  {

  unsigned char i ;
  char OctetConverti[9] ;
  unsigned char retour ;
  //FormatBinaire ( *CompressedData, OctetConverti ) ;
  //printf ( "Entree fonction CompressionOctet\n" ) ;
  //getch () ;

  // Calcul du fils
  retour = MultiplicationPolynomeX256 ( pCompressedData ) ;
  if ( retour == 0 )
    {
    perror ( "16. Erreur dans fonction MultiplicationPolynomeX256" ) ;
    return 0 ;
    }

  //for ( i = 0; i < 8; i ++ )
    //{
      //CompressionBit ( Octet, CompressedData, i ) ;
      //FormatBinaire ( *CompressedData, OctetConverti ) ;
      //printf ( "  %s\r ", OctetConverti ) ;
      //getch() ;
    //}



  //printf ( "Fin fonction Compression Octet\n" ) ;
  //getch () ;
  return 1 ;

  }  // fin CompressionOctet






unsigned char MultiplicationPolynomeX256 (
						PPolynome pCompressedData
						)
  {
  PPolynome Pposition ;
  unsigned long i = 0 ;
  unsigned char retour ;
  pRetenue1 retenue1 = NULL ;
  pRetenue1 PositionRetenue1 = NULL ;
  pRetenue1 r ;


  Pposition = pCompressedData->Next ;

  while ( i < pCompressedData->Coef + 1 )
    {

    if ( Pposition->Coef <= 16777215L )
      Pposition->Coef = Pposition->Coef * 256L ;
    else
      {


      if ( Pposition->Next == NULL )
	{
	Pposition->Next = ( PPolynome ) malloc ( sizeof ( Polynome ) ) ;
	if ( Pposition->Next == NULL )
	  {
	  perror ( "17. Erreur dans allocation dynamique de memoire pour PPolynome " ) ;
	  return 0 ;
	  }
	Pposition->Next->Coef = 0 ;
	Pposition->Next->Next = NULL ;
	pCompressedData->Coef +=1 ;
	}


      if ( PositionRetenue1 != NULL )
	{
	PositionRetenue1->Next = ( pRetenue1 ) malloc ( sizeof ( Retenue1 ) ) ;
	if ( PositionRetenue1->Next == NULL )
	  {
	  perror ( "18. Erreur allocation dynamique de memoire pour retenue1 " ) ;
	  return 0 ;
	  }
	PositionRetenue1 = PositionRetenue1->Next ;
	}
      else
	{
	PositionRetenue1 = ( pRetenue1 ) malloc ( sizeof ( Retenue1 ) ) ;
	//printf ( "m‚moire disponible %d\n", coreleft () ) ;
	if ( PositionRetenue1 == NULL )
	  {
	  perror ( "19. Erreur allocation dynamique de memoire pour retenue1 " ) ;
	  return 0 ;
	  }
	retenue1 = PositionRetenue1 ;
	r = retenue1 ;
	}



      PositionRetenue1->Position = Pposition->Next ;
      PositionRetenue1->Next = NULL ;
      Pposition->Coef = ( Pposition->Coef - 16777215L ) * 256L ;
      Pposition = Pposition->Next ;
      } // fin du else
      i++ ;
    }// fin du while

    //Traitement des retenues
    while ( retenue1 != NULL )
      {
      if ( retenue1->Position->Coef <= 4294967284L )
	{
	retenue1->Position->Coef += 1L ;
	retenue1 = retenue1->Next ;
	}
      else
	{
	retenue1->Position->Coef = 0L ;
	if ( retenue1->Position->Next == NULL )
	  {
	  ( retenue1->Position )->Next = ( PPolynome ) malloc ( sizeof ( Polynome ) ) ;
	  if ( retenue1->Position->Next == NULL )
	    {
	    perror ( "20. Erreur allocation dynamique de Polynome" ) ;
	    return 0 ;
	    }
	  pCompressedData->Coef +=1 ;
	  retenue1->Position->Next->Next = NULL ;
	  retenue1->Position->Next->Coef = 1 ;
	  retenue1 = retenue1->Next ;
	  }
	else
	  retenue1->Position = retenue1->Position->Next ;
	}
      }  // fin du while ( retenue ...

      free ( r ) ;
     return 1 ;
  } //  fin fonction unsigned char MultiplicationPolynomeX256 (










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
    perror ( " 21. Erreur de lecture " ) ;
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




unsigned char MultiplicationPolynomeX256 (
						PPolynome pCompressedData
						)
  {
  PPolynome Pposition ;
  unsigned long i = 0 ;
  unsigned long Retenue = 0 ;
  unsigned char retour ;

  DegrePolynome = pCompressedData->Coef ;
  Pposition = pCompressedData->Next ;

  while ( i < pCompressionData->Coef + 1 )
  {

//    if ( Retenue == 1 )
//      {
      //retour = AdditionPolynome ( 1, Pposition, pCompressedData ) ;
      //if ( retour == 0 )
	//{
	//perror ( "Erreur dans appel fonction AdditionPolynome" ) ;
//	//return ( 0 ) ;
	//}
      //} // fin if ( Retenue == 1 )


    if ( Pposition->Coef <= 16777215L )
      Pposition->Coef = Pposition->Coef * 256L ;
    else
      {
//      Retenue = 1 ;
      Pposition->Coef = ( Pposition->Coef - 16777215L ) * 256L ;
      // traitement de la retenue
//      if ( Pposition->Next != NULL )
//	{
//	retour = AdditionPolynome ( 1, Pposition->Next, pCompressedData ) ;
//	if ( retour == 0 )
//	  {
//	  perror ( "Erreur dans appel fonction AdditionPolynome" ) ;
//	  return ( 0 ) ;
//	  }
//	} // fin  if ( Pposition->suivant != NULL )
//      else
//	{
//	// allocation dynamique de memoire du prochain
//	Pposition->Next = ( PPolynome ) malloc ( Polynome )
//	if ( Pposition->Next == NULL )
//	  {
//	  perror ( "Erreur allocation dynamique de memoire" ) ;
//	  return 0 ;
//	  }
//	( Pposition->Next )->Coef = 1L ;
//	( Pposition->Next )->Next = NULL ;
//	pCompressedData->Coef = pCompressedData->Coef + 1 ;
//	}
      } // fin else
    Pposition = Pposition->Next ;
    }// fin du while
  } //  fin fonction unsigned char MultiplicationPolynomeX256 (


*/ // fin du brouillon
