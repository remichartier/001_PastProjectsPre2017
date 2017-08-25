#include <userint.h>
#include "test_init.h"

#if !defined(TEST_C)
#define TEST_C
#endif

#include "test.h"


static int panelHandle;
void InitCom(char * ) ;
void Mobile_InitEnd (void);
void ProductionTests(void);
void ErrorReset(void);
void PrintStatus(char *pp_Message);
bool vg_ErrorNewReset=FALSE;
bool vg_StopTest=FALSE;


/****************************************************************************
**																		   **
**				 MAIN FUNCTION !!!										   **
**																		   **
**																		   **
*****************************************************************************/

int main (int argc, char *argv[])
{

/* Declarations */
  MC_SL_DECLARE_THREAD_VAR;
  int vl_OldCom=1;
  char s[10];


  if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
  if ((panelHandle = LoadPanel (0, "test_init.uir", PANEL)) < 0)
		return -1;
		
  /* Initialization Serial Link threads */
  SL1_12TLSAllocate();
  SL3_05SetActiveCom(vl_OldCom);
  SL3_10OneSpyWindowOnly(FALSE);
  MC_SL_EXTRACT_THREAD_VAR;
	
  sprintf(s,"COM%d",MC_SL_THREAD_COM);

  InitCom(s);
		   SetCtrlAttribute (panelHandle, PANEL_STARTTEST, ATTR_DIMMED, 1);
		   SetCtrlAttribute (panelHandle, PANEL_STOPTEST, ATTR_DIMMED, 1);
		
  DisplayPanel (panelHandle);
  RunUserInterface ();
  DiscardPanel (panelHandle);
	return 0;
}

/****************************************************************************
**																		   **
**				 QUIT CALLBACK  !!!										   **
**																		   **
**																		   **
*****************************************************************************/



int CVICALLBACK Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	u32 vl_Error=0;
   MC_SL_DECLARE_THREAD_VAR;
   MC_SL_EXTRACT_THREAD_VAR;                                                       
	
	switch (event)
		{
		case EVENT_COMMIT:
			
			/* Quit SL */
			vl_Error = SL3_04CloseSL(MC_SL_THREAD_COM);
			
			if (vl_Error)
				exit(1);
		
			/* Free the thread local storage area */
			vl_Error = SL1_13TLSFree();	
			if (vl_Error)
				exit(1);
		
			QuitUserInterface (0);

			break;
		}
	return 0;
}

/****************************************************************************
**																		   **
**				 INITCOM() FUNCTION !!! 								   **
**																		   **
**																		   **
*****************************************************************************/


void InitCom(char * vp_com) 
{


   int         vl_com=1;
   u32         vl_error = 0;

   MC_SL_DECLARE_THREAD_VAR;
   MC_SL_EXTRACT_THREAD_VAR;                                                       
   
   sscanf(vp_com,"COM%d", & vl_com);

   PrintStatus("PortCom initialization");
  p_sl_ptr=Mobile_InitEnd;


   SL3_02InitSL(vp_com, SL_COM_RATE115, SL_IPEATIO2_NO_PARITY, FALSE, (u8) SL_INIT_BYTE49, (u8) SL_MODE_BYTE4F,
               (u8) 12, (s32) 0);

	/* Initialise some TAT variables */
   dv0_25InitTatVar();

     
     
     PrintStatus("WAITING MOBILE INIT");
     
     /* Wait for the mobile to be switched on successfuly*/
   /* while ((v_sl_MobileOn == 0) && (v_ExitInitUART == FALSE))
      {
         SL2_01DoEvent();
      }
     
   */


}

/****************************************************************************
**																		   **
**				 MOBILE_INITEND() FUNCTION !!!							   **
**																		   **
**																		   **
*****************************************************************************/


void Mobile_InitEnd (void)
{
  u32         vl_error = 0,vl_SecurityKey[4],vl_SoftwareVersion[2],vl_Identity,vl_Mode;
  u8             vl_VersionType;
  char        pl_Versions[60];
  char        pl_PopupText[100];
  u8          vl_param=1;
bool vl_TATLimited;


MC_SL_DECLARE_THREAD_VAR;
MC_SL_EXTRACT_THREAD_VAR;
   
      MC_SL_V_BLOCKING    = 0;
      MC_SL_V_NO_BLOCKING = 0;

      if (MC_SL_V_MOBILEON==3)  
      {
         /* Wait for the RTK init message*/
         vl_error = dv10_00BackgroundLoop ((u32) 6*COMM_TIMEOUT, &vl_TATLimited,&vl_Identity,
                       &vl_Mode,vl_SoftwareVersion,vl_SecurityKey,&vl_VersionType);

         if ((vl_error & 0xFFFF ) != 0)
         {
         
            sprintf (pl_PopupText, "dv10_00BackgroundLoop function returned error %08X",vl_error);
			PrintStatus( pl_PopupText);
			
         }
         else
 		 {
   		   PrintStatus("MOBILE INITIALIZED in TAT");
           sprintf (pl_PopupText, "COM %d Serial Port Mobile Soft Version : %08X%08X",MC_SL_THREAD_INDEX+1,*vl_SoftwareVersion,*(vl_SoftwareVersion+1));
		   PrintStatus(pl_PopupText);
		   SetCtrlAttribute (panelHandle, PANEL_STARTTEST, ATTR_DIMMED, 0);
		   vg_StopTest=FALSE;
           MC_SL_V_MOBILEON=0;

		   FakeKeystroke (VAL_MENUKEY_MODIFIER|'A');
/*		   ProcessSystemEvents();*/
/*		   QueueUserEvent(EVENT_COMMIT,panelHandle, PANEL_STARTTEST); */
		   SetActivePanel ( panelHandle); /* to flush the new keystroke */

         }
 
      }

} /* Mobile_InitEnd */





/****************************************************************************
**																		   **
**				 ProductionTests FUNCTION !!!							   **
**																		   **
**																		   **
*****************************************************************************/

void ProductionTests(void)
{


  u16 vl_ADC1Value=0;
  u32 vl_Error=0;
  char        pl_PopupText[100];
  s16 vp_MinRx,vp_MinTx,vp_MinMon,vp_MaxRx,vp_MaxTx,vp_MaxMon,vp_AverageRx,vp_AverageTx,vp_AverageMon,vp_AveragePower;

	u8 i;
  
    p_sl_ptr=ErrorReset;   

/* orders for production tests sequences */
   for(i=0;i<100;i++)
/*  while((vg_ErrorNewReset!=TRUE)&&(vl_Error==0)&&(vg_StopTest!=TRUE))*/
  {
	/* An example : read ADC1 value */
	vl_Error = dv40_01GetRadioTemperature(2000, &vl_ADC1Value);
  	  if (!vl_Error)
		SetCtrlVal (panelHandle, PANEL_ADCNUMERICMETER, (unsigned int)vl_ADC1Value);
	  else
	  {
	        sprintf (pl_PopupText, "dv40_08GetADC1Value function returned error %08X",vl_Error);
			PrintStatus( pl_PopupText);

	  }
	  
	vl_Error=dv51_02MainPowerMeasure(2000, TAT_ABSOLUTE_POWER_MEASURE,
			    4,     0,
			    -100,0,
                            62,       &vp_MinRx,
                            &vp_MinTx,        &vp_MinMon,
                            &vp_MaxRx,        &vp_MaxTx,
                            &vp_MaxMon,       &vp_AverageRx,
                            &vp_AverageTx,    &vp_AverageMon,
                            &vp_AveragePower)  ; 

  	  if (!vl_Error)
		SetCtrlVal (panelHandle, PANEL_POWERMEASMAX, vp_MaxRx/8);
	  else
  	  
	  {
	        sprintf (pl_PopupText, "dv51_02MainPowerMeasure function returned error %08X",vl_Error);
			PrintStatus( pl_PopupText);

	  }
 
	i++;
	if 	   ((vg_ErrorNewReset==TRUE)||(vl_Error!=0)||(vg_StopTest==TRUE))
	i=110;
  }

    p_sl_ptr=Mobile_InitEnd;
	SetCtrlAttribute (panelHandle, PANEL_QUIT, ATTR_DIMMED, 0);
    
    if(vg_ErrorNewReset!=TRUE)
    { 
	  SetCtrlAttribute (panelHandle, PANEL_STOPTEST, ATTR_DIMMED, 1);
      SetCtrlAttribute (panelHandle, PANEL_STARTTEST, ATTR_DIMMED, 0);
	  PrintStatus("START TEST OR SWITCH ON NEW MOBILE");

	  vg_StopTest=FALSE;
    }
    else
    {
      vg_ErrorNewReset= FALSE;
	  PrintStatus("PLEASE SWITCH ON NEW MOBILE");
    }

}


/****************************************************************************
**																		   **
**				 ErrorReset FUNCTION !!!							   **
**																		   **
**																		   **
*****************************************************************************/

void ErrorReset(void)
{

  char        pl_PopupText[100];

MC_SL_DECLARE_THREAD_VAR;
MC_SL_EXTRACT_THREAD_VAR;

  
	vg_ErrorNewReset=TRUE;
    SetCtrlAttribute (panelHandle, PANEL_STOPTEST, ATTR_DIMMED, 1);
    SetCtrlAttribute (panelHandle, PANEL_STARTTEST, ATTR_DIMMED, 1);
	PrintStatus( "Mobile unexpected reset");
	PrintStatus( "--->PLEASE WAIT BEFORE SWITCH ON AGAIN");
	MC_SL_V_MOBILEON=0; /* sl.dll should detect again next initialisation bytes from the beginning,
							Would have different behaviour if keeps MC_SL_V_MOBILEON to 3 */
	
    
}


/****************************************************************************
**																		   **
**				 STOPTESTS FUNCTION !!!	         						   **
**																		   **
**																		   **
*****************************************************************************/

int CVICALLBACK StopTest (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			vg_StopTest=TRUE;
		    SetCtrlAttribute (panelHandle, PANEL_STOPTEST, ATTR_DIMMED, 0);
		    SetCtrlAttribute (panelHandle, PANEL_STARTTEST, ATTR_DIMMED, 1);
   			PrintStatus("Stop test Pushed");

			break;
		}
	return 0;
}


/****************************************************************************
**																		   **
**				 STARTTESTS FUNCTION !!!	         						   **
**																		   **
**																		   **
*****************************************************************************/


int CVICALLBACK StartTest (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			vg_StopTest=FALSE;
		    SetCtrlAttribute (panelHandle, PANEL_STOPTEST, ATTR_DIMMED, 0);
		    SetCtrlAttribute (panelHandle, PANEL_STARTTEST, ATTR_DIMMED, 1);
			SetCtrlAttribute (panelHandle, PANEL_QUIT, ATTR_DIMMED, 1);
		    
   			PrintStatus("Start test ---> Test in progress");

			ProductionTests();
			break;
		}
	return 0;
}



/****************************************************************************
**																		   **
**				 PRINT STATUS FUNCTION !!!	         						   **
**																		   **
**																		   **
*****************************************************************************/
void PrintStatus(char *pp_Message)
{
static u32 vg_nblines=0;

  InsertTextBoxLine (panelHandle, PANEL_TEXTBOX, vg_nblines++, pp_Message);
  SetCtrlAttribute (panelHandle, PANEL_TEXTBOX, ATTR_FIRST_VISIBLE_LINE,
					  vg_nblines);

}

#undef TEST_C


