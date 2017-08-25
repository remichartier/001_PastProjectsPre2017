#define FUNCTIONS_C
#include "functions.h"
#undef FUNCTIONS_C 

/* function Iso11172_stream

Input parameters : handle on bitstream iso11172
									global handle video buffer 

Return value : 0 if no error
							1 if global alloc error
              2 if ISO_11172_END_CODE not found at the end
              
*/

BYTE Iso11172_stream(PHANDLE ph_Iso11172,LPHANDLE lp_hVideoBuffer )
{
LPBYTE lptr_Iso11172;
BYTE   ret=0;
DWORD   BitPos = 0;

MessageBox(hwnd,"Iso11172_stream: Enter function","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
lptr_Iso11172=GlobalLock(*ph_Iso11172);
if( lptr_Iso11172 == NULL)
{
	ret = 1;
	MessageBox(hwnd,"Iso11172_stream: lptr_Iso11172=NULL","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
}
if (!ret)
{
	while(Nextbits((LPBYTE)lptr_Iso11172,(char *)PACK_START_CODE,(DWORD) BitPos))
		Pack((LPBYTE)lptr_Iso11172,(DWORD *) &BitPos,(LPHANDLE) lp_hVideoBuffer);
	if (!Nextbits((LPBYTE)lptr_Iso11172,(char *)ISO_11172_END_CODE,(DWORD) BitPos))
	{
		ret = 2;
		MessageBox(hwnd,"Iso11172_stream: pas de iso_111172_end_code","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
  }
} /* end if (!ret) */
GlobalUnlock(*ph_Iso11172);
GlobalFree(*ph_Iso11172);
return ((BYTE)ret) ;
} /* end function Iso11172_stream */

BYTE Nextbits(LPBYTE StreamPtr, char s[], DWORD BitPos)
{
/*

Input parameters :

s[] : char stream terminated by '\0' representing stream to search
			in the stream pointed  by ptr_stream

StreamPtr : Pointer on stream to compare

BitPos : Bit number where the comparison should start from the stream pointed
					by StreamPtr

returns :
			1    : fits
			0    : does not fit, without error



Algorithm :

calculate length of char stream
if length != 0
	create char stream with length for comparison (memory allocation) (string)
	fill this char stream with bits to compare, 1 by 1
	for i=0;i<length,i++
		string[i]=(converted to char) ( ( (StreamPtr+ NbBytesOffset) >>
															(inversedBitPos in byte)
														 ) & 0x01 )
		BitPos ++
  string[i] = '\0' (end of char stream)  														
	compare string and s
	free alloc (string)
else
	return error;
END 

*/

/* Declarations */

BYTE i,length;  								/* -> max length = 256 */
char * string;
HANDLE h;

length=strlen(s); /* strlen does not count '\0' */
h=LocalAlloc(LMEM_FIXED,length+1);
string = LocalLock(h);

for(i=0;i<length;i++)
{

	itoa( (((unsigned char)*(StreamPtr+BitPos/8))>>(7-BitPos%8))&0x01, /*value*/
				&string[i],/*char *string*/
				10);/* int radix*/

	BitPos ++;
}
string[i] = '\0';
if (!strcmp(s,string)) /* means fits */
	i = 1;
else
	i = 0;
LocalUnlock(h);
LocalFree(h);
return((BYTE)i);

} /* Nextbits */


/* function Pack
Input : ptr on iso11172 bit stream
				bit position in the stream
        global handle on video buffer
return:
				0 no problem
				1 "0010" not found
				2 marker bit not found
Algorithm :
	pack_start_code already checked in precedent function -> bypass
		by incrementing Bitpos
	check if nextbit = "0010"
	for (i=0;i<3;i++)
		bypass system_Clock_reference -> increment Bitpos
		check if nextbit = marker_bit
	check if nextbit = marker_bit
	bypass muxrate -> increment muxrate
	check if nextbit = marker_bit
	if(nextbit = system_header_start_code)
		system_header;
	while nextbit = packet_stqrt_code_prefix
		packet
end
*/

BYTE Pack(LPBYTE lpStream,DWORD * pBitPos,LPHANDLE lp_hVideoBuffer)
{
BYTE ret=0,i;

MessageBox(hwnd,"Pack : enter function pack","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
*pBitPos += strlen(PACK_START_CODE);
if(!Nextbits((LPBYTE) lpStream,"0010",*pBitPos))
{
	ret = 1;
	MessageBox(hwnd,"Pack : 0010 not found","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
}
else
	*pBitPos+=4; /* length "0010" */

if (!ret) /* first 3 bit system_clock_reference */
{
		*pBitPos+=3; /* length 3 bit system_clock_reference */
		if (!Nextbits((LPBYTE) lpStream,MARKER_BIT,*pBitPos))
		{
			ret = 2;
			MessageBox(hwnd,"Pack : MARKER_BIT1 not found","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		}
		else
			*pBitPos+=1; /* bypass marker bit */
} /* end if !ret */


if (!ret)  /* following 15 bits system_clock_reference */
{
		*pBitPos+=15; /* length 3 bit system_clock_reference */
		if (!Nextbits((LPBYTE) lpStream,MARKER_BIT,*pBitPos))
    {
			ret = 2;
			MessageBox(hwnd,"Pack : MARKER_BIT2 not found","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		}
		else
			*pBitPos+=1; /* bypass marker bit */
} /* end if !ret */


if (!ret)  /* last 15 bits system_clock_reference */
{
		*pBitPos+=15; /* length 3 bit system_clock_reference */
		if (!Nextbits((LPBYTE) lpStream,MARKER_BIT,*pBitPos))
		{
			ret = 2;
			MessageBox(hwnd,"Pack : MARKER_BIT3 not found","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
    }
		else
			*pBitPos+=1; /* bypass marker bit */
} /* end if !ret */

if (!Nextbits((LPBYTE) lpStream,MARKER_BIT,*pBitPos))
{
	ret = 2;
	MessageBox(hwnd,"Pack : MARKER_BIT4 not found","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
}
else
	*pBitPos+=1; /* bypass marker bit */

if (!ret) /* mux_rate */
{
	*pBitPos+=22; /* bypass mux_rate (22 bits) */
	if (!Nextbits((LPBYTE) lpStream,MARKER_BIT,*pBitPos))
	{
		ret = 2;
		MessageBox(hwnd,"Pack : MARKER_BIT5 not found","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
	}
	else
		*pBitPos+=1; /* bypass marker bit */
} /* end if !ret */

if( !ret && Nextbits((LPBYTE) lpStream,SYSTEM_HEADER_START_CODE,*pBitPos))
//	ret=System_header((LPBYTE) lpStream,(DWORD *)pBitPos);
		MessageBox(hwnd,"Pack : System_header_start_code found","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);

//while(!ret && Nextbits((LPBYTE) lpStream,PACKET_START_CODE_PREFIX,*pBitPos))
//	ret=Packet((LPBYTE)lpStream,(DWORD *)pBitPos,(LPHANDLE) lp_hVideoBuffer );

MessageBox(hwnd,"Pack : end of function","hedit", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);

return(ret);
} /* end pack */


/* function System_header
Input : ptr on iso11172 bit stream
				bit position in the stream
return:
				0 no problem
				1 "11" not found
				2 marker bit not found
        3 RESERVED_BYTE not FOUND
Algorithm :
end
*/

BYTE System_header(LPBYTE lpStream,DWORD * pBitPos)
{

BYTE ret=0;

*pBitPos+=32; /* bypass system_header_start_code */
*pBitPos+=16; /* bypass header_length */

if (!Nextbits((LPBYTE) lpStream,MARKER_BIT,*pBitPos))
	ret = 2;
else
	*pBitPos+=1; /* bypass marker bit */

if (!ret) /* bypass rate_bound */
	*pBitPos+=22; /* bypass */

if (!ret && (!Nextbits((LPBYTE) lpStream,MARKER_BIT,*pBitPos)) )
	ret = 2;
else
	*pBitPos+=1; /* bypass marker bit */

if (!ret) /* bypass audio_bound */
	*pBitPos+=6; /* bypass */

if (!ret) /* bypass fixed flag */
	*pBitPos+=1; /* bypass */


if (!ret) /* csps flag */
	*pBitPos+=1; /* bypass */

if (!ret) /* system_audio_lock_ flag */
	*pBitPos+=1; /* bypass */

if (!ret) /* system_video_lock_ flag */
	*pBitPos+=1; /* bypass */

if (!ret && (!Nextbits((LPBYTE) lpStream,MARKER_BIT,*pBitPos)))
	ret = 2;
else
	*pBitPos+=1; /* bypass marker bit */

if (!ret) /* bypass video_bound */
	*pBitPos+=5; /* bypass */

/* reserved byte */
if (!ret && (!Nextbits((LPBYTE) lpStream,RESERVED_BYTE,*pBitPos)))
	ret = 3;
else
	*pBitPos+=strlen(RESERVED_BYTE); /* bypass reserved byte */

while ( !ret && (!Nextbits((LPBYTE) lpStream,"1",*pBitPos)) )
{
	*pBitPos+=8; /* bypass streamid */
	if(!Nextbits((LPBYTE) lpStream,"11",*pBitPos))
		ret = 1;
	else
  {
		*pBitPos+=2; /* bypass 11 */
		*pBitPos+=1; /* bypass STD_buffer_bound_scale */
		*pBitPos+=13; /* bypass STD_buffer_size_bound */
	} /* end else */
} /*end while nextbit == 1 */

return((BYTE)ret);

} /* End System_header */




/* function Packet

Input : ptr on iso11172 bit stream
				bit position in the stream
				global handle on video buffer
        
return:
				0 no problem
				2 marker bit not found
Algorithm :
simple, only for 1 video stream, for simple application
end
*/

BYTE Packet(LPBYTE lpStream,DWORD * pBitPos,LPHANDLE lp_hVideoBuffer )
{

BYTE ret=0;
BYTE stream_id=0;
BYTE i;
WORD packet_length=0;
char s[9]; /* for conversion streamid to string */

*pBitPos+=24; /* bypass packet_start_code_prefix */

/* store streamid */
for(i=0;i<8;i++)
{
	stream_id |= (((BYTE)*(lpStream+ (*pBitPos/8))>>(7-(*pBitPos)%8))) && 0x01;
	*pBitPos ++;
} /* end for i=0 to 8*/

/* store packet length */
for(i=0;i<16;i++)
{
	packet_length |= ((((BYTE)*(lpStream+(*pBitPos/8)))>>(7-(*pBitPos)%8)) && 0x0001)<<(15-i);
	*pBitPos ++;
} /* end for i=0 to 16*/
swap_bytes((char *)&packet_length , (BYTE)sizeof(WORD));

/* if streamid != private_stream_2 */
itoa((WORD)stream_id, s, 2);
if (!strcmp(PRIVATE_STREAM_2,s))
{
	while(Nextbits(lpStream,"11111111",*pBitPos))
		*pBitPos +=8; /* stuffing byte */
	if(Nextbits(lpStream,"01",*pBitPos))
	{
		*pBitPos +=2; /* bypass 01 */
		*pBitPos +=1; /* bypass STD_buffer_scale */
		*pBitPos +=13; /* bypass STD_buffer_size */
	} /*end if nextbit=01 */

	if(Nextbits(lpStream,"0010",*pBitPos))
	{
		*pBitPos +=4; /* bypass 0010 */
		*pBitPos +=3; /* bypass presentation_time_stamp */
		if(!Nextbits(lpStream,MARKER_BIT,*pBitPos))
			ret=2;
		else
			*pBitPos +=1; /* bypass Marker bit */
    if(!ret)
			*pBitPos +=15; /* bypass presentation_time_stamp */
		if( !ret && !Nextbits(lpStream,MARKER_BIT,*pBitPos))
			ret=2;
		else
			*pBitPos +=1; /* bypass Marker bit */
					*pBitPos +=1; /* bypass Marker bit */
		if(!ret)
			*pBitPos +=15; /* bypass presentation_time_stamp */
		if( !ret && !Nextbits(lpStream,MARKER_BIT,*pBitPos))
			ret=2;
		else
			*pBitPos +=1; /* bypass Marker bit */
	} /*end if nextbit=0010 */
	else
	{
		if(Nextbits(lpStream,"0011",*pBitPos))
		{
			*pBitPos +=4; /* bypass 0011 */
			if(!Nextbits(lpStream,MARKER_BIT,*pBitPos))
				ret=2;
			if(!ret)
      {
				*pBitPos +=1; /* bypass marker bit */
				*pBitPos +=3; /* bypass presentation_time_stamp */
			}
			if(!Nextbits(lpStream,MARKER_BIT,*pBitPos))
				ret=2;
			if(!ret)
      {
				*pBitPos +=1; /* bypass marker bit */
				*pBitPos +=15; /* bypass presentation_time_stamp */
			}
			if(!Nextbits(lpStream,MARKER_BIT,*pBitPos))
				ret=2;
			if(!ret)
      {
				*pBitPos +=1; /* bypass marker bit */
				*pBitPos +=15; /* bypass presentation_time_stamp */
			}
 			if(!Nextbits(lpStream,MARKER_BIT,*pBitPos))
				ret=2;
			if(!ret)
				*pBitPos +=1; /* bypass marker bit */
			if(!Nextbits(lpStream,"0001",*pBitPos))
				ret=2;
			if(!ret)
				*pBitPos +=4; /* bypass 0001 */
			if(!ret)
			{

				*pBitPos +=1; /* bypass marker bit */
				*pBitPos +=3; /* bypass presentation_time_stamp */
			
				if(!Nextbits(lpStream,MARKER_BIT,*pBitPos))
					ret=2;
				if(!ret)
      	{
					*pBitPos +=1; /* bypass marker bit */
					*pBitPos +=15; /* bypass presentation_time_stamp */
				}
				if(!Nextbits(lpStream,MARKER_BIT,*pBitPos))
					ret=2;
				if(!ret)
      	{
					*pBitPos +=1; /* bypass marker bit */
					*pBitPos +=15; /* bypass presentation_time_stamp */
				}
				if(!Nextbits(lpStream,MARKER_BIT,*pBitPos))
					ret=2;
				if(!ret)
					*pBitPos +=1; /* bypass marker bit */

      } /* if !ret */
			else
			{
				if(!Nextbits(lpStream,"00001111",*pBitPos))
        	ret=2;
      } /* end else */     	
		} /* 0011 */
  } /* end else 0010


	/* si video; create buffer etc :::: */


} /* end if strcmp */

return((BYTE)ret);

} /* End Packet */





/*
function swap_bytes :

*/

void swap_bytes(char * s, BYTE bytelength)
{
char * s1;
HANDLE h;
BYTE i;

h = LocalAlloc(LMEM_FIXED,bytelength);
s1=LocalLock(h);
for(i=0;i<bytelength;i++)
	s1[i]=s[bytelength-1-i];
for(i=0;i<bytelength;i++)
	s[i]=s1[i];
LocalUnlock(h);
LocalFree(h);
}/* end swap_bytes */


