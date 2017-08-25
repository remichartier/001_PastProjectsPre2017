#if defined (TEST_C)

#define ACCESS_RIGHTS_FA
#define ACCESS_RIGHTS_MTX

/* OS includes */
#include <windows.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "test_init.h"
#include "pc_hard.opt"

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
#include "dvadc.h"
#include "dv1int.h"
#include "dv17pmu.h"
#include "dv18int.h"
#include "dv8int.h"
#include "dv9int.h"
#include "dvrf.h"


#undef ACCESS_RIGHTS_FA
#undef ACCESS_RIGHTS_MTX

#endif /* TEST_C */
