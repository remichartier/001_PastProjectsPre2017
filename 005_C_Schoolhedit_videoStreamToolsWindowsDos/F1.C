#define F1_C
#include "function.h"
#undef F1_C

void f1NbToString(LPBYTE StreamStart,DWORD BitPosition,char StringAddress[],BYTE BitLength,BYTE * increment)
{
 char sremi;
				fseek(StreamStart,BitPosition/8,SEEK_SET);
				*increment=0;
				while(*increment<BitLength)
				{
					fread(&sremi,1,1,(FILE *)StreamStart);
					do
					{
					itoa((((unsigned char)sremi)>>(7-(BitPosition+ (*increment))%8))&0x01,
					&StringAddress[*increment],10);
					(*increment) ++;
					}
					while( ((BitPosition%8+*increment)%8!=0) && (*increment<BitLength));
				 }
				BitPosition+=BitLength;
				StringAddress[*increment]='\0';
} /* end f1NbToString*/

//LPBYTE StreamStart,DWORD BitPosition,char StringAddress[],BYTE BitLength,BYTE * increment)

void f1ReadNbFromStream(LPBYTE pStream,DWORD * pPos,BYTE BitLength,DWORD * pByte)
{
char * String;
HANDLE h;
BYTE i;
DWORD w;

LOCAL_ALLOC(BitLength+1,h,String);
f1NbToString(pStream,*pPos, String,BitLength,&i);
w=strtoul(String,NULL,2);
*pByte=w;
LOCAL_FREE(h,String);
*pPos+=BitLength;

} /* end f1ReadNbFromStream */