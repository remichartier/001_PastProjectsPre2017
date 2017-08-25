#if defined (FUNCTIONS_HIP)
#else
#define FUNCTIONS_HIP


BYTE Iso11172_stream(PHANDLE ph_Iso11172,LPHANDLE lp_hVideoBuffer );
BYTE Nextbits(LPBYTE StreamPtr, char s[], DWORD BitPos);
BYTE Pack(LPBYTE lpStream,DWORD * pBitPos,LPHANDLE lp_hVideoBuffer);
BYTE System_header(LPBYTE lpStream,DWORD * pBitPos);
BYTE Packet(LPBYTE lpStream,DWORD * pBitPos,LPHANDLE lp_hVideoBuffer );
void swap_bytes(char * s, BYTE bytelength);
void f1NbToString(LPBYTE StreamStart,DWORD BitPosition,char StringAddress[],BYTE BitLength,BYTE * increment);
void f1ReadNbFromStream(LPBYTE pStream,DWORD * pPos,BYTE BitLength,DWORD * pByte);



#endif /* FUNCTIONS_HIP */