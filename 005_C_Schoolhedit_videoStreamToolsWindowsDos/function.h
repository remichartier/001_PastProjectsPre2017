#if defined (FUNCTIONS_C) || defined(F1_C)

#if !defined(__DOS__)
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "sysmets.h"
#else /*__DOS__*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sysmets.h"
#endif /* __DOS__ */

#endif /*FUNCTIONS_C*/


/******EXTERNAL INCLUDES******/


/******INTERNAL INCLUDES******/
#if defined (FUNCTIONS_C) || defined (F1_C)
#include "functions.hic"
#include "functions.hip"
#endif /* FUNCTIONS_C */