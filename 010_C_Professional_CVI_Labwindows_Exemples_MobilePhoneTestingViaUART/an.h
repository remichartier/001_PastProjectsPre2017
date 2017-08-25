/******************************************************************************
File header for TAT sysol2 driver simple application note
******************************************************************************/

#ifndef AN_H 
#define AN_H  

/* list of all external includes useful for the module */
#if defined (AN_C)

/* Defines acces rights for includes */
#define ACCESS_RIGHTS_FA
#define ACCESS_RIGHTS_MTX

/* Compilation switches file */
#include "pc_hard.opt"
//#define MAX_SIZE_RW 200

/* OS includes */
#include <windows.h>

/* Compiler includes */
#include <utility.h>
#include <ansi_c.h>

/* Embeded includes */
#include "master.h"
#include "pccdef.h"
#include "bai.h"
#include "dsp.h"
#include "hsmc.h"
#include "hee.h"
#include "hfd.h"
#include "tattop.h"
#include "tatmain.h"
#include "tatadc.h"
#include "tataudio.h"
#include "tateepro.h"
#include "tatflash.h"
#include "tatrf.h"
#include "tatsc.h"
#include "tatsim.h"
#include "tatlcd.h"
#include "matman.h"
#include "mateng.h"
#include "dua.h"
#include "datamng.h"

/* Serial link include */
#include "slbud.h" /* SL*_* prototypes */

/* Driver includes */
#include "dvtop.h" /* dv0_* prototypes */ 
#include "dvmain.h" /* dv10_* prototypes */
#include "dvadc.h" /* dv40_* prototypes */
#include "dv1int.h"
#include "dv17pmu.h"
#include "dv18int.h"
#include "dv8int.h"
#include "dv9int.h"
#include "dvaudio.h"
#include "dveepro.h"
#include "dvflash.h"
#include "dvhfda.h"
#include "dvrf.h"
#include "dvsim.h"
#include "dvsc.h"
#include "dvlcd.h"
#include "dvmateng.h"

#endif /* AN_C */

/* list of includes provided to other modules */

/* NONE */

/* list of all internal includes useful for the module */

/* NONE */

#undef ACCESS_RIGHTS_FA
#undef ACCESS_RIGHTS_MTX

#endif /* AN_H */
