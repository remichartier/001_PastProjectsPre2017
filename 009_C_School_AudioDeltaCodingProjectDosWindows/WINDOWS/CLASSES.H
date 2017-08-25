#include <string.h>
#include <owl.h>
#include <filedial.h>
#include <dialog.h>
#include <math.h>
#include <mdi.h>
#include <stdio.h>
#include <owldefs.h>
// FICHIER CLASSES.H

#include <stdlib.h>
#include <dir.h>
#include <array.h>
#include <abstarry.h>

/*----------------------------------------------------------*\
I               DEFINITION DES IDENTIFICATEURS              I
\*----------------------------------------------------------*/
#define ID_PAROLE               101
#define ID_SINUSOIDE            102
#define ID_MUSIQUE              103
#define ID_FERMETURE            104
#define ID_SCHEMA_SMOD         2011
#define ID_PARAM_SMOD          2012
#define ID_SCHEMA_AMOD         2021
#define ID_PARAM_AMOD          2022
#define ID_PARFAIT              301
#define ID_BRUITE               302
#define ID_SCHEMA_SDEMOD       4011
#define ID_ACTION_SDEMOD       4012
#define ID_SCHEMA_ADEMOD       4021
#define ID_ACTION_ADEMOD       4022
#define ID_VISU_SIG_SOURCE     6011
#define ID_VISU_SIG_RESTI      6012
#define ID_VISU_SPECTRE         602 
#define ID_ECOUTE               701

#define ID_A                    107
#define ID_DELTA		108
#define ID_ALPHA_PLUS           110
#define ID_ALPHA_MOINS          111
#define ID_DELTA_MAX            112
#define ID_DELTA_MIN            113
#define ID_VALIDER_SMOD         106
#define ID_VALIDER_AMOD         108


/*----------------------------------------------------------*\
I              DEFINITION DES CLASSES                       I
\*----------------------------------------------------------*/ 

// Classe Application

class TMyApp : public TApplication
{
public:
  TMyApp(LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
    : TApplication(AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow) {};
  virtual void InitMainWindow();
};

// Classe TWindow

   // Declaration d'une classe de fenêtres MDI

_CLASSDEF(TMyWindow)
class TMyWindow : public TMDIFrame
{
public:
  TMyWindow(LPSTR ATitle, LPSTR MenuName):TMDIFrame(ATitle,MenuName){};

  virtual BOOL CanClose();
  virtual void Init_file();
  virtual void Close_file();
  virtual void Ouverture();

  // déclaration des fonctions liées à la commande Génération
  virtual void CMParole(RTMessage Msg)
    = [CM_FIRST + ID_PAROLE];
  virtual void CMSinusoide(RTMessage Msg)
    = [CM_FIRST + ID_SINUSOIDE];
  virtual void CMMusique(RTMessage Msg)
    = [CM_FIRST + ID_MUSIQUE];
  virtual void CMFermeture(RTMessage Msg)
    = [CM_FIRST + ID_FERMETURE];

  // déclaration des fonctions liées à la commande Modulation
  virtual void CMSchema_Smod(RTMessage Msg)
    = [CM_FIRST + ID_SCHEMA_SMOD];
  virtual void CMParam_Smod(RTMessage Msg)
    = [CM_FIRST + ID_PARAM_SMOD];
  virtual void CMSchema_Amod(RTMessage Msg)
    = [CM_FIRST + ID_SCHEMA_AMOD];
  virtual void CMParam_Amod(RTMessage Msg)
    = [CM_FIRST + ID_PARAM_AMOD];

   // déclaration des fonctions liées à la commande Canal
  virtual void CMParfait(RTMessage Msg)
    = [CM_FIRST + ID_PARFAIT];
  virtual void CMBruite(RTMessage Msg)
    = [CM_FIRST + ID_BRUITE];

   // déclaration des fonctions liées à la commande Démodualtion
  virtual void CMSchema_Sdemod(RTMessage Msg)
    = [CM_FIRST + ID_SCHEMA_SDEMOD];
  virtual void CMAction_Sdemod(RTMessage Msg)
    = [CM_FIRST + ID_ACTION_SDEMOD];
  virtual void CMSchema_Ademod(RTMessage Msg)
    = [CM_FIRST + ID_SCHEMA_ADEMOD];
  virtual void CMAction_Ademod(RTMessage Msg)
    = [CM_FIRST + ID_ACTION_ADEMOD];

  // déclaration des fonctions liées à la commande Visualisation
  virtual void CMVisu_sig_source(RTMessage Msg)
    = [CM_FIRST + ID_VISU_SIG_SOURCE];
  virtual void CMVisu_sig_resti(RTMessage Msg)
    = [CM_FIRST + ID_VISU_SIG_RESTI];
  virtual void CMSpectre(RTMessage Msg)
    = [CM_FIRST + ID_VISU_SPECTRE];

  // déclaration de la fonction liée à la commande Ecoute
  virtual void CMEcoute(RTMessage Msg)
    = [CM_FIRST + ID_ECOUTE];

  // declaration des fonctions traitant directement la modulation
  //   	et la demodulation
  virtual void recopier(FILE *,FILE *,double);
  virtual char lire_no_bloc(FILE *);
  virtual double lire_longueur_bloc(FILE *);
  virtual void coder_s(FILE *,FILE *,unsigned char,float);
  virtual void decoder(FILE *,FILE *,unsigned char,unsigned char,float,float);
  virtual void coder_octet_delta(FILE *,
			FILE *,
			unsigned char *,
			unsigned char *,
			char *,
			float,
			unsigned char*,
			unsigned char);
  virtual void coder_octet_delta_diff(FILE *,
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
  virtual void coder_longueur(double,char *);
  virtual void coder_bloc_delta(FILE *,FILE *,
			int,
			int,
			unsigned char *,
			int *,
			float,
			unsigned char *,
			unsigned char);
  virtual void decoder_bloc_delta(FILE *,
			FILE *,
			int,
			int,
			unsigned char *,
			int *,
			unsigned char);
  virtual void decoder_bloc_delta_diff(FILE *,FILE *,int,int,unsigned char *,int *,
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
  virtual void decoder_octet_delta(FILE *,
			unsigned char *,
			unsigned char,
			int,unsigned char);
  virtual void decoder_octet_delta_diff(FILE *,unsigned char *,unsigned char,int,
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
  virtual unsigned char fonction_filtre(unsigned char,unsigned char,unsigned char);

  virtual void coder_bloc_delta_diff(FILE *,
			FILE *,
			int,
			int,
			unsigned char *,
			int *,
			float ,
			unsigned char *,
			unsigned char *,
			unsigned char ,
			unsigned char ,
			float ,
			float ,
			int *,
			int *);
 };




// Déclaration d'une classe de fenêtres pour dessiner

_CLASSDEF(TMyDrawWindow)
class _CLASSTYPE TMyDrawWindow : public TWindow
{
public:

  BOOL  Source, Dest;
  TMyDrawWindow(PTWindowsObject AParent, LPSTR ATitle)
    : TWindow(AParent, ATitle){  Source = FALSE; Dest = FALSE;};

  virtual void lit_sinus(FILE *, POINT *, long *, RECT *);
  virtual double lire_longueur_bloc(FILE *);
  virtual char lire_no_bloc(FILE *);
  virtual void Paint(HDC , PAINTSTRUCT& );
  virtual BOOL CanClose();
};


// Déclaration de deux classes de fenêtres pour dialoguer

_CLASSDEF(TParamDialogSmod)
class TParamDialogSmod : public TDialog
{
public:
    char A[6]; // parametre a
    char Delta[6]; // parametre delta
    TParamDialogSmod(PTWindowsObject AParent, LPSTR AName): TDialog(AParent, AName){};
    //virtual BOOL CanClose();
    virtual void Valider_Smod(RTMessage Msg)
      = [ID_FIRST + ID_VALIDER_SMOD];
    virtual void Cancel(RTMessage Msg)
      = [ID_FIRST + IDCANCEL];
    virtual void recup_param_Smod();
};


_CLASSDEF (TParamDialogAmod)
class TParamDialogAmod : public TDialog
{
public:
    char AlphaPlus[6];  // parametre alpha+
    char AlphaMoins[6]; // parametre alpha-
    char DeltaMax[6];   // paramètre delta_max
    char DeltaMin[6];   // paramètre delta_min

    TParamDialogAmod(PTWindowsObject AParent, LPSTR AName): TDialog(AParent, AName){};
    
    virtual void Valider_Amod(RTMessage Msg)
      = [ID_FIRST + ID_VALIDER_AMOD];
    virtual void Cancel(RTMessage Msg)
      = [ID_FIRST + IDCANCEL]; 
    virtual void recup_param_Amod();
};
