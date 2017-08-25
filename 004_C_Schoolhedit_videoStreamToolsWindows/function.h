#if defined (FUNCTIONS_C)
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "sysmets.h"
#endif /*FUNCTIONS_C*/

#if defined (FUNCTIONS_C)
#define PACK_START_CODE "000001BA"
#define MARKER_BIT "1"
#define SYSTEM_HEADER_START_CODE "000001BB"
#define RESERVED_BYTE "11111111"
#define ISO_11172_END_CODE "000001B9"
#define PACKET_START_CODE_PREFIX "000001"
#define PRIVATE_STREAM_2 "BF"

BYTE Iso11172_stream(PHANDLE ph_Iso11172,LPHANDLE lp_hVideoBuffer );
BYTE Nextbits(LPBYTE StreamPtr, char s[], DWORD BitPos);
BYTE Pack(LPBYTE lpStream,DWORD * pBitPos,LPHANDLE lp_hVideoBuffer);
BYTE System_header(LPBYTE lpStream,DWORD * pBitPos);
BYTE Packet(LPBYTE lpStream,DWORD * pBitPos,LPHANDLE lp_hVideoBuffer );
void swap_bytes(char * s, BYTE bytelength);



#endif /* FUNCTIONS_C */