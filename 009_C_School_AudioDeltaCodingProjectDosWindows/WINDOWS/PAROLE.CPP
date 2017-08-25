/*----------------------------------------------------------*\
I       Programme réalisant une interface pour traiter      I
I             la compression de parole                      I
I  nom parole.cpp                                           I
I  Concepteurs : CHARTIER Rémi                              I
I		 MILLET Isabelle                            I
I		 MATT Stéphane                              I
\*----------------------------------------------------------*/

#include "classes.h"

// variables globales

char FileName[255], chaine_cod[255],chaine_dec[255];
char type_signal[20];
extern unsigned char delta;
extern float a;
FILE *file_source, *file_cod, *file_dec;

/*-------------------------------------------------------------*\
 I      Fonctions liées à la commande Génération               I
\*------------------------------------------------------------ */

//fonction qui permet de selectionner un fichier .voc contenant de la parole
void TMyWindow::CMParole(RTMessage)
{
if (file_source!=NULL)
	MessageBox(HWindow, "Fermez-le pour pouvoir en ouvrir un autre!",
	  "Un fichier est deja ouvert",MB_OK);
else
  {
  if ( GetModule()->ExecDialog(new TFileDialog(this, SD_FILEOPEN,
    _fstrcpy(FileName, "*.VOC"))) == IDOK )
     // Si l'utilisateur a choisi l'ouverture du fichier
       {
         if  (MessageBox(HWindow, FileName, "Ouverture du fichier:", MB_OKCANCEL) == IDOK)
	  // La variable type_signal prend la valeur "parole"
		strcpy(type_signal,"parole");
	  // On ouvre les fichiers source.voc, source.cod et Rsource.voc
		Ouverture();
       }
   }	
}


//fonction qui permet de selectionner un fichier .voc contenant une sinusoide
void TMyWindow::CMSinusoide(RTMessage)
{
 if (file_source!=NULL)
	MessageBox(HWindow, "Fermez-le pour pouvoir en ouvrir un autre!",
	  "Un fichier est deja ouvert",MB_OK);
 else
   {
   if ( GetModule()->ExecDialog(new TFileDialog(this, SD_FILEOPEN,
    _fstrcpy(FileName, "*.VOC"))) == IDOK )
       // Si l'utilisateur a choisi l'ouverture du fichier
       {
        if (MessageBox(HWindow, FileName, "Ouverture du fichier:", MB_OKCANCEL) == IDOK)
	 // La variable type_signal prend la valeur "sinusoide"
	 strcpy(type_signal,"sinusoide");
        // On ouvre les fichiers source.voc, source.cod et Rsource.voc
	  Ouverture();
       }	  
    }	
}

//fonction qui permet de selectionner un fichier .voc contenant de la musique
void TMyWindow::CMMusique(RTMessage)
{
 if (file_source!=NULL)
	MessageBox(HWindow, "Fermez-le pour pouvoir en ouvrir un autre!",
	  "Un fichier est deja ouvert",MB_OK);
 else
   {
   if ( GetApplication()->ExecDialog(new TFileDialog(this, SD_FILEOPEN,
    _fstrcpy(FileName, "*.VOC"))) == IDOK )
      // Si l'utilisateur a choisi l'ouverture du fichier
       {
	if (MessageBox(HWindow, FileName, "Ouverture du fichier:", MB_OKCANCEL) == IDOK)
	 // La variable type_signal prend la valeur "musique"
	 strcpy(type_signal,"musique");
         // On ouvre les fichiers source.voc, source.cod source.dec et Rsource.voc
	   Ouverture();
       }
    }
}

// fonction qui ouvre les fichiers source.voc, source.cod source.dec et Rsource.voc
void TMyWindow::Ouverture()
{
char c; // chaine[255];
int i=0;

do
 {
   c=FileName[i];
   chaine_cod[i]=c;
   chaine_dec[i]=c;
   i++;
 }
 while (c!='.');
 chaine_cod[i]='\0';
 chaine_dec[i]='\0';
 strcat(chaine_cod,"COD");
 strcat(chaine_dec,"ess");
 /* strcpy(chaine_dec,"f:\\user\\chartier\\projet\\");
 do
 {
  i=24;
  c=FileName[i];
  chaine[i-24]=c;
  i++;
 }
 while (c!='\0');
 strcat(chaine_dec,"R");
 strcat(chaine_dec,chaine);   */ 
 file_source=fopen(FileName,"rb");
 file_cod=fopen(chaine_cod,"wb");
 file_dec=fopen(chaine_dec,"wb");
}

// Fonction qui ferme les fichiers source.voc, source.cod source.dec et Rsource.voc
// En plus, elle réinitialise les pointeurs des fichiers à NULL
void TMyWindow::CMFermeture(RTMessage)
{
    if (file_source==NULL)
	 MessageBox(HWindow, " Fichier deja ferme!",
	  "Fermeture des fichiers:",MB_OK);
    else
      {
	if ( MessageBox(HWindow, FileName, "Fermeture des fichiers:",
    		MB_OKCANCEL) == IDOK)
        {
	 Close_file(); 
	 Init_file();
	}
     }
}

void TMyWindow::Init_file()
{       // initialisation des pointeurs de fichiers a NULL
	file_source=NULL;
	file_cod=NULL;
	file_dec=NULL;
}

void TMyWindow::Close_file()
{       // on ferme les fichiers source.voc, source.cod et Rsource.voc
	fclose(file_source);
	fclose(file_cod);
        fclose(file_dec);
}

/*--------------------------------------------------------------*\
 *      Fonctions concernant la visualisation                   *
\*------------------------------------------------------------- */
// fonction qui permet de visualiser le signal sinusoidal source
void TMyWindow::CMVisu_sig_source(RTMessage)
{
 PTMyDrawWindow MyWindow;
	if (file_source==NULL)
	    MessageBox(HWindow,"ATTENTION! Ouvrez un fichier!",
	     "ERREUR",NULL);
	else
	{
	 if (strcmp(type_signal,"sinusoide")==0)
         {
          MyWindow = new TMyDrawWindow(this,"Visualisation");
          GetApplication()->MakeWindow(MyWindow);
	  MyWindow->Source = TRUE;
	  InvalidateRect(MyWindow->HWindow, NULL, TRUE);
	 }
	 else
	    MessageBox(HWindow,"Cette fonctionnalite n'a pas encore ete developpee",
		"Visualisation d'un signal non sinusoidal",NULL);
        }
}

// fonction qui permet de visualiser le signal sinusoidal restitué
void TMyWindow::CMVisu_sig_resti(RTMessage)
{
 PTMyDrawWindow MyWindow;
	if (file_source==NULL)
	   MessageBox(HWindow,"ATTENTION! Ouvrez un fichier!",
	     "ERREUR",NULL);
	else
	{
	 if (strcmp(type_signal,"sinusoide")==0)
	 {
	  fclose(file_dec);
	  file_dec=fopen(chaine_dec,"rb");
          MyWindow = new TMyDrawWindow(this,"Visualisation");
          GetApplication()->MakeWindow(MyWindow);
	  MyWindow->Dest = TRUE;
	  InvalidateRect(MyWindow->HWindow, NULL, TRUE);
	 }
	 else
	    MessageBox(HWindow,"Cette fonctionnalite n'a pas encore ete developpee",
		"Visualisation d'un signal non sinusoidal",NULL);
	}
}

// fonction qui permettrait de visuliser le spectre d'un signal
void TMyWindow::CMSpectre(RTMessage)
{
   MessageBox(HWindow,"Cette fonctionnalité n'a pas encore été développée","Spectre",MB_OKCANCEL);

}


/*-------------------------------------------------------------------*\
 *  Fonctions concernant les schémas de réalisation                  *
\*-------------------------------------------------------------------*/

// schéma de réalisation de la modulation Delta
void TMyWindow::CMSchema_Smod(RTMessage)
{

  PTWindowsObject toto=new TDialog(this,"DIALOG_1");
  if(toto==NULL)
    {
    MessageBox(NULL,"Error","ERR",MB_OK);
    return;
    }
  GetApplication()->ExecDialog(toto);
}

// schéma de réalisation de la démodulation Delta simple
void TMyWindow::CMSchema_Sdemod(RTMessage)
{
 PTWindowsObject toto1=new TDialog(this,"DIALOG_2");
if(toto1==NULL)
{
    MessageBox(NULL,"Error","ERREUR",MB_OK);
    return;
}
GetApplication()->ExecDialog(toto1);
}

// schéma de réalisation de la démodulation delta adaptatif
void TMyWindow::CMSchema_Ademod(RTMessage)
  {
  PTWindowsObject toto2=new TDialog(this,"DIALOG_5");
  if(toto2==NULL)
    {
    MessageBox(NULL,"Error","ERREUR",MB_OK);
    return;
    }
  GetApplication()->ExecDialog(toto2);
  }

// schéma de réalisation de la modulation delta adaptatif
void TMyWindow::CMSchema_Amod(RTMessage)
  {
  PTWindowsObject toto3=new TDialog(this,"DIALOG_6");
  if(toto3==NULL)
    {
    MessageBox(NULL,"Error","ERREUR",MB_OK);
    return;
    }
  GetApplication()->ExecDialog(toto3);
  }

 /*-------------------------------------------------------------------*\
  *  fonction qui lance le traitement de la modulation delta simple   *
 \*-------------------------------------------------------------------*/
  void TMyWindow::CMAction_Sdemod(RTMessage)
   {
       // traitement spécifique
       coder_s(file_source,file_cod,delta,a);
       fclose(file_cod);
       file_cod=fopen(chaine_cod,"rb");
       decoder(file_cod,file_dec,delta,delta,a,a);
       MessageBox(HWindow,"Traitement termine","ACTION",NULL);
    }

/*--------------------------------------------------------------------*\
 * fonction qui lance le traitement de la modulation delta adaptative *
 *  FONCTION NON ACTIVE                                               *
\*--------------------------------------------------------------------*/
    void TMyWindow::CMAction_Ademod(RTMessage)
   {
     {
       // 
	MessageBox(HWindow,"Cette fonctionnalité n'a pas encore été developpée",
	"ACTION",NULL);
     }
    }


/*-------------------------------------------------------------------*\
I   Fonction concernant les choix de paramètres                       I
\*--------------------------------------------------------------------*/
// choix des paramètres a et delta pour la modulation simple
void TMyWindow::CMParam_Smod(RTMessage)
{
     // lancement d'une boîte de dialogue
     GetModule()->ExecDialog(new TParamDialogSmod(this, "DIALOG_3"));
}


// choix des paramètres alpha+, alpha-, delta max, delta min
void TMyWindow::CMParam_Amod(RTMessage)
{
     // lancement d'une boîte de dialogue
     GetModule()->ExecDialog(new TParamDialogAmod(this, "DIALOG_4"));
}


/*-------------------------------------------------------------------*\
 *              Fonctions concernant le canal                        *
\*-------------------------------------------------------------------*/

// pour le canal parfait
void TMyWindow::CMParfait(RTMessage)
{
   MessageBox(HWindow,"Choix d'un canal parfait","Canal Parfait",MB_OKCANCEL);
}

// pour un canal bruite FONCTION NON ACTIVE
void TMyWindow::CMBruite(RTMessage)
{
   MessageBox(HWindow," Cette fonctionnalité n'a pas été développée.",
	"Canal Bruité",MB_OKCANCEL);
}

/*-------------------------------------------------------------------*\
 *   Fonction concernant la commande Ecoute                          *
\*-------------------------------------------------------------------*/


     void TMyWindow::CMEcoute(RTMessage)
{
    // char tab[255];
    // strcpy(tab,"wav2voc ");
    // strcat(tab,FileName);
    // WinExec(tab,SW_SHOW);
    // lancement de l'executable qui transforme un fichier *.VOC en *.WAV
    WinExec("voc2wav f:\user\chartier\projet\sin4.voc",SW_SHOW);
    // lancement d'un executable qui permet l'ecoute
    WinExec("soundply",SW_SHOW);
}


/*-------------------------------------------------------------------*\
I   Fonction pour quitter la session                                 I
\*--------------------------------------------------------------------*/

BOOL TMyWindow::CanClose()
{
  if (file_source!=NULL)
  	{
	 MessageBox(HWindow,"ATTENTION! Fermez les fichiers!",
	     "ERREUR",NULL);
	 return FALSE;
	}
  else
    {
     return MessageBox(HWindow, "Voulez-vous vraiment quitter SOUND ?",
     "SORTIE", MB_YESNO | MB_ICONQUESTION) == IDYES;
    }
}


void TMyApp::InitMainWindow()
{
  MainWindow = new TMyWindow("SOUND", "MENU_1");
  ((PTMyWindow)MainWindow)->ChildMenuPos=6;
}


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdShow)
{
  TMyApp MyApp("Modulation de la Parole", hInstance, hPrevInstance,
               lpCmdLine, nCmdShow);
  MyApp.Run();
  return MyApp.Status;
}
