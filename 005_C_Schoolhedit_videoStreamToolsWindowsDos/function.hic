#if defined (FUNCTIONS_HIC)
#else
#define FUNCTIONS_HIC



#define PACK_START_CODE "00000000000000000000000110111010"//"000001BA"
#define MARKER_BIT "1"
#define SYSTEM_HEADER_START_CODE "00000000000000000000000110111011"//"000001BB"
#define RESERVED_BYTE "11111111"
#define ISO_11172_END_CODE "00000000000000000000000111011001"//"000001B9"
#define PACKET_START_CODE_PREFIX "000000000000000000000001"//"000001"
#define PRIVATE_STREAM_2 "10111111"//"BF"


#endif /*FUNCTIONS_HIC*/
