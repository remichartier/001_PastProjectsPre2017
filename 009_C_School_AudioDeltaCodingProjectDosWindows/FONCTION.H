#include <stdio.h>
#include <string.h>


int ouvrir_fichier(FILE **,char *);
void recopier(FILE *,FILE *,double);
char lire_no_bloc(FILE *);
double lire_longueur_bloc(FILE *);
void coder_octet_delta(FILE *,
			FILE *,
			unsigned char *,
			unsigned char *,
			char *,
			float,
			unsigned char *,
			unsigned char);
void coder_octet_delta_diff(FILE *,
			FILE *,
			unsigned char *,
			unsigned char *,
			char *,
			float,
			unsigned char *,
			unsigned char *,
			unsigned char,
			unsigned char,
			float,
			float,
			int *,
			int *);
void coder_longueur(double,char *);
void coder_bloc_delta(FILE *,FILE *,
			int,
			int,
			unsigned char *,
			int *,
			float,
			unsigned char *,
			unsigned char);
void affichebit(unsigned char);
int test_version_fichier(FILE *);
void decoder_bloc_delta(FILE *,
			FILE *,
			int,
			int,
			unsigned char *,
			int *,
			unsigned char);
void decoder_bloc_delta_diff(FILE *,
			FILE *,
			FILE *,
			int,
			int,
			unsigned char *,
			int *,
			unsigned char *,
			unsigned char,
			unsigned char,
			float,
			float,
			int *,
			int *,
			float,
			unsigned char *,
			unsigned char *,
			unsigned char *,
			unsigned char * );
void decoder_octet_delta(FILE *,
			unsigned char *,
			unsigned char,
			int,
			unsigned char );
void decoder_octet_delta_diff(FILE *,
			FILE *,
			unsigned char *,
			unsigned char,
			int,
			unsigned char *,
			unsigned char ,
			unsigned char ,
			float ,
			float,
			int *,
			int *,
			float,
                        unsigned char *,
			unsigned char *,
			unsigned char *,
			unsigned char * );
unsigned char echantillon_sinus(double,double);
unsigned char fonction_filtre(unsigned char,unsigned char,unsigned char);

void coder_bloc_delta_diff(FILE *,
			FILE *,
			int,
			int,
			unsigned char *,
			int *,
			float ,
			unsigned char*,
			unsigned char *,
			unsigned char ,
			unsigned char ,
			float ,
			float ,
			int *,
			int *);