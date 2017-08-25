/******************************************************************************
Source file for TAT sysol2 driver simple application note
This application will establish remote control on the target
then perform some simple driver calls
******************************************************************************/
#define AN_C
#include "an.h"
#undef AN_C

/* Main function */
int main(int argc, char **argv)
{
u32 vl_SLVersion;
u32 vl_SLRevision;
u32 vl_SLVersionForDV;
u32 vl_SLRevisionForDV;
u32 vl_DVVersion[2];
u32 vl_DVRevision;
u32 vl_Error;
bool vl_TATLimited;
u32 vl_Identity, vl_Mode, vl_EmbededVersion[2], vl_Key[4];
u8 vl_VersionType;
u16 vl_ADC1Value;

	/* printf usage */
	if ((argc<2)||(argc>=3))
	{
		printf("Usage : %s COMx [ENTER]\n",argv[0]);
		printf("For example : %s COM1 [ENTER]\n",argv[0]);
		printf("Press a key to quit.\n");
		getchar();
		exit(1);
	}

	/* Display Serial link version */
	vl_SLVersion = SL3_01VersionNumber();
	vl_SLRevision = SL3_00RevisionNumber();
	printf("SL Version, Revision : %08X, %08X\n", vl_SLVersion, vl_SLRevision);

	/* Display Driver version */
	dv0_10GetDriverVersion(vl_DVVersion, &vl_DVRevision);
	printf("Driver Version, Revision : %08X.%08X, %08X\n", vl_DVVersion[0],  vl_DVVersion[1], vl_DVRevision);
	dv0_29GetSLDriverVersion_to_use(&vl_SLVersionForDV, &vl_SLRevisionForDV);
	/* Check Drivers/SL compatibility */	
	if ((vl_SLVersionForDV != vl_SLVersion) || (vl_SLRevisionForDV != vl_SLRevision))
		printf("Warning : whith this driver you should use SL Version, Revision : %08X, %08X\n", vl_SLVersionForDV, vl_SLRevisionForDV);
	
	/* Allocate a thread local storage area */
	vl_Error = SL1_12TLSAllocate();
	if (vl_Error)
		exit(1);
		
	/* Launch SL */
	vl_Error = SL3_02InitSL(argv[1], SL_COM_RATE115, SL_IPEATIO2_NO_PARITY, FALSE, SL_INIT_BYTE49, SL_MODE_BYTE4F, 12, 0);
	if (vl_Error)
		exit(1);

	/* user message */
	printf("Connect your serial cable to PC and mobile.\n");
	printf("After you'll press ENTER, you'll have 10 seconds to start your mobile.\n");
	getchar();
	printf("Switch On your mobile........\n");
	
	/* Initialise some TAT variables */
	dv0_25InitTatVar();
	
	/* Wait for Mobile synchronisation */
	vl_Error = dv10_00BackgroundLoop(10000, &vl_TATLimited, &vl_Identity, &vl_Mode, vl_EmbededVersion,
										vl_Key, &vl_VersionType);	
	if (vl_Error)
		exit(1);
	
	/* Display Embeded software informations */
	printf("Embeded Version : %08X.%08X\n", vl_EmbededVersion[0], vl_EmbededVersion[1]);
	
	/*------------------------------*/
	/* DRIVERS : put your code here */
	/* An example : read ADC1 value */
	vl_Error = dv40_08GetADC1Value(2000, &vl_ADC1Value);
	if (vl_Error)
		exit(1);
	printf("ADC1 : %04X\n", vl_ADC1Value);
	getchar();
	/*------------------------------*/

	/* Quit SL */
	vl_Error = SL3_04CloseSL(atoi(argv[1]+strlen("COM")));
	if (vl_Error)
		exit(1);
		
	/* Free the thread local storage area */
	vl_Error = SL1_13TLSFree();	
	if (vl_Error)
		exit(1);
		
	exit(0);
}
