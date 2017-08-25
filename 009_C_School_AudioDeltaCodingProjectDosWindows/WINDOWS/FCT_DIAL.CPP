// FICHIER FCT_DIAL.CPP
// Ce fichier contient les fonctions concernant les boites de dialogue


#include "classes.h"


float a=1,alpha_plus,alpha_moins;
unsigned char delta='5',delta_max,delta_min;

/*-----------------------------------------------------------------------*\
 *   fonction qui recupere les parametres                                *
 *       de la modulation delta adaptative                               *           I
\*-----------------------------------------------------------------------*/

   void TParamDialogAmod::recup_param_Amod()
   {
     GetDlgItemText(HWindow,ID_ALPHA_PLUS,AlphaPlus,5);
     GetDlgItemText(HWindow,ID_ALPHA_MOINS,AlphaMoins,5);
     GetDlgItemText(HWindow,ID_DELTA_MAX,DeltaMax,5);
     GetDlgItemText(HWindow,ID_DELTA_MIN,DeltaMin,5);

   }
/*-------------------------------------------------------------------*\
 *  fonction qui recupere les parametres de la                       *
 *          Modulation Delta Simple                                  *
\*-------------------------------------------------------------------*/

   void TParamDialogSmod::recup_param_Smod()
   {
      GetDlgItemText(HWindow,ID_A,A,5);
      GetDlgItemText(HWindow,ID_DELTA,Delta,5);
   }
/*-------------------------------------------------------------------*\
 *   fonction qui réagit au VALIDER de la commande Parametres        *
 *           (dans Modulation Delta Adaptative)                      *
\*-------------------------------------------------------------------*/
   void TParamDialogAmod::Valider_Amod(RTMessage)
   {
   
   PTParamDialogAmod MyParamDialogAmod;
   
   alpha_plus = (float)atof(MyParamDialogAmod->AlphaPlus);
   if ( alpha_plus <0)
   {
       MessageBox(HWindow," alpha_plus ne peut pas etre negatif ",
       	"ERREUR",MB_OK);
   }
   alpha_moins = (float)atof(MyParamDialogAmod->AlphaMoins);
   if ( alpha_moins < 0 )
   {
       MessageBox(HWindow," alpha_plus ne peut pas etre negatif ",
       	"ERREUR",MB_OK);
   }
   delta_min=(MyParamDialogAmod->DeltaMin)[0];
   delta_max=(MyParamDialogAmod->DeltaMax)[0];
   CloseWindow();
   }

 /*-------------------------------------------------------------------*\
  *   fonction qui réagit au VALIDER de la commande Parametres dans   *
  *         la commande Modulation Delta Simple                       *
  *  Cette fonction enregistre les parametres                         *
 \*-------------------------------------------------------------------*/
   void TParamDialogSmod::Valider_Smod(RTMessage)
   {
   PTParamDialogSmod MyParamDialogSmod;
   a=atof(MyParamDialogSmod->A);
   delta=(MyParamDialogSmod->Delta)[0];
   if (a<0)
	  {
	    MessageBox(HWindow," a ne peut pas etre negatif ","ERREUR",MB_OK); 
	  
	  }
   CloseWindow();
   }

  /*-----------------------------------------------------------------*\
   *  fonction qui réagit au ANNULER de la commande Parametres       *
   *    dans la commande MODULATION Delta Adaptative                 *
  \*-----------------------------------------------------------------*/
    void TParamDialogAmod::Cancel(RTMessage)
   {
    CloseWindow();
   }
     


/*------------------------------------------------------------------*\
 *    fonction qui réagit au ANNULER de la commande Parametre       *
 *         pour la modulation delta simple                          *
\*------------------------------------------------------------------*/

     void TParamDialogSmod::Cancel(RTMessage)
   {
       CloseWindow();
   }


