
/****************************************************************************\
|   Log Driver Definitions                                                   |
\****************************************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__
#include "/usr/include/ibxe/sllist.h"
#include "/usr/include/ibxe/bitmap.h"

#define DUMMYDATA       2
#define BLOCKSIZE       512
#define LOGSIZE         (BLOCKSIZE * 1024)

#define LOGDEVICE       "/dev/logd"
#define LOGFILE         "/usr/rchartie/logfile"	/* it was /logfile before */

#define MSGBLKSIZE      (BLOCKSIZE - sizeof(long) - sizeof(char))
#define LOGMSGHDRSIZE   (sizeof(LOGMSG) - DUMMYDATA)
#define VLOGBUFSIZE     128

/* Possible message types. If adding a new type increase LOG_MAX. */

#define LOG_CLEAR       0   /* No data in LOGMSG */
#define LOG_TEXT        1   /* Alphanumeric text in LOGMSG */

typedef struct {
    long    time;               /* Time message was put in log */
    char    type;               /* Message type per "LOG" defines above */
    char    count;              /* Count in bytes of following message */
    uchar   data[DUMMYDATA];    /* Start of logged data */
} LOGMSG;

typedef struct {
    long            chain;      /* Chain word for queueing */
    long            time;       /* Timestamp when last msg put in blk */
    unsigned char   msgcount;   /* Count of msgs in block */
} LOGBLOCK;

typedef struct {
    char    buf[VLOGBUFSIZE];   /* Buffer to build user msg in */
    int     count;              /* Buffer length */
} VLOGBUF;

typedef struct {
    char    *fmt;
    int     args;
} FMTANDARGS;

#ifdef __STDC__
extern int logd( int, char *, unsigned int );
#else
extern int logd();
#endif

#endif

