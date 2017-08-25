/****************************************************************************\
|   Console Log Report Printer                                               |
\****************************************************************************/

#include <fcntl.h>
#include <types.h>
#include <file.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "/usr/rchartie/logger.h"

#define LINESIZE 80

struct MY_DATE 
{
	char year[5];
	char month[3];
	char day[3];
	char hour[3];
	char min[3];
	char sec[3];
};
typedef struct MY_DATE my_date;
typedef my_date *pmy_date;


static unsigned int     logsize=LOGSIZE;
static unsigned long    logseq=0;

/****************************************************************************\
|   adjust_number(): if string == "xx" it's ok but if == "x", changes in "0x"|
|	where x represents a digits. 					     |
\****************************************************************************/

adjust_number( char *number )
{
	if( strlen(number) == 1 )
	{
		number[2] = '\0';
		number[1] = number[0];
		number[0] = '0';
	}
}

/****************************************************************************\
|   make_format_date() makes a string of date with a special format	     |
|   		format used : "YYYYMMDDhhmmss"				     |
\****************************************************************************/

make_format_date( char date[15], pmy_date pmy_date )
{
	adjust_number( pmy_date->month );
	adjust_number( pmy_date->day );
	adjust_number( pmy_date->hour );
	adjust_number( pmy_date->min );
	adjust_number( pmy_date->sec );

	strcpy( date, pmy_date->year );
	strcat( date, pmy_date->month );
	strcat( date, pmy_date->day );
	strcat( date, pmy_date->hour );
	strcat( date, pmy_date->min );
	strcat( date, pmy_date->sec );
}

/****************************************************************************\
|   make_format_dateofthisclock() : make string of date with a special format|
|   		format used : "YYYYMMDDhhmmss"				     |
\****************************************************************************/

make_format_dateofthisclock( char date[15], long *clock )
{
	struct tm *tm;
	my_date   mydate;

	tm = localtime( clock );
	
	sprintf( mydate.year,  "%d", tm->tm_year + 1900 );
	sprintf( mydate.month, "%d", tm->tm_mon + 1 );
	sprintf( mydate.day,   "%d", tm->tm_mday );
	sprintf( mydate.hour,  "%d", tm->tm_hour );
	sprintf( mydate.min,   "%d", tm->tm_min );
	sprintf( mydate.sec,   "%d", tm->tm_sec );

	make_format_date( date, &mydate );
}

/****************************************************************************\
|   check_number_string() : checks if string contains only digits or spaces  |
\****************************************************************************/

int check_number_string( char *string )
{
	int i;
	for ( i=0; i < strlen(string); i++ )
	{
		if( ( isdigit(string[i]) == 0 ) && 
		    ( isspace(string[i])==0 ) )
			return(0);
	}
	return(1);
}

/****************************************************************************\
|   check_date() : checks if date at the good format		             |
|   		format used : "YYYYMMDDhhmmss"				     |
\****************************************************************************/

int check_date( char date[15] )
{
	char exam[5];
 	char *p;
	
	strncpy( exam, date, 4 );
	exam[4] = '0';
	if( check_number_string(date) == 0 )
	{
		perror("\nYou must write the date with digits\n");
		return(0);
	}
	if( atoi(exam) < 1970 )
	{
		perror("\nError : the year must be over 1970\n");
    		return(0);
	}
	
	p = &date[4] ;
	strncpy( exam, p, 2 );
	exam[2] = '\0' ;
	if( ( atoi(exam) < 1 ) || ( atoi(exam) > 12 ) )
	{
		perror("\nError : the month must be between 1 and 12 \n");
		return(0);
	}

	p = &date[6] ;
	strncpy( exam, p, 2 );
	exam[2] = '\0' ;
	if( ( atoi(exam) < 1 ) || ( atoi(exam) > 31 ) )
	{
		perror("\nError : the day must be between 1 and 31 \n");
		return(0);
	}

	p = &date[8] ;
	strncpy( exam, p, 2);
	exam[2] = '\0' ;
	if( ( atoi(exam) < 0 ) || ( atoi(exam) > 23 ) )
	{
		perror("\nError : the hour must be between 0 and 23 \n");
		return(0);
	}

	p = &date[10] ;
	strncpy( exam, p, 2 );
	exam[2] = '\0' ;
	if( ( atoi(exam) < 0 ) || ( atoi(exam) > 59 ) )
	{
		perror("\nError : the minutes must be between 0 and 59 \n");
		return(0);
	}

	p = &date[12] ;
	strncpy( exam, p, 2 );
	exam[2] = '\0' ;
	if( ( atoi(exam) < 0 ) || ( atoi(exam) > 59 ) )
	{
		perror("\nError : the seconds must be between 0 and 59 \n");
		return(0);
	}
	return(1);
}

/****************************************************************************\
|   itoa() Convert integer to display string of given radix                  |
\****************************************************************************/

char *itoa(num, str, radix)

    unsigned int            num;
    char                    *str;
    int                     radix;

{
    register unsigned int   num2;
    register int            digits;

    num2 = num;
    digits = 0;

    if ((radix < 37) && (radix > 1)) {
        while (num2/=radix)
            digits++;
        str[digits+1] = '\0';
        for (; digits > -1; digits--) {
            num2 = num % radix;
            if (num2 < 10)
                str[digits] = (char) '0'+num2;
            else
                str[digits] = (char) 'A'+(num2-10);
            num /= radix;
        }
    }

    return str;
}

/****************************************************************************\
|   DumpHex                                                                  |
\****************************************************************************/

int DumpHex(dumpsfd, title, dump, base, length, modulo)

    FILE            *dumpsfd;   /* Stream File descriptor to dump to */
                                /* Defaults to standard out */
    char            *title;     /* Title of thing being dumped */
    unsigned char   *dump;      /* Pointer to block to be dumped */
    unsigned int    base;       /* Base address used in display */
    unsigned int    length;     /* Length of data at 'dump' to be formatted */
    int             modulo;     /* Modulus of number of chars to format */
                                /* before inserting a space character */
{
    unsigned char   pline1[LINESIZE];
    unsigned char   pline2[LINESIZE];
    unsigned char   pline3[10];
    unsigned long   offset;
    int             inserted;
    int             group;
    int             i,j,k,l;

    offset =
    inserted = 0;

    if (length <= 0)
        return 0;

    if (dumpsfd == 0)
        dumpsfd = stdout;

    if (modulo == 0)
        modulo = 8;
    else
        modulo *= 2;

    fprintf(dumpsfd, "\n%s\nAddress   Offset\n", title);

    do {  
        bzero(&pline1[0],LINESIZE);
        bzero(&pline2[0],LINESIZE);
        
        if (length-offset > 16)
            group = 16;
        else
            group = length-offset;  
       
        for (i=offset, j=offset+group, inserted=0, k=0, l=0; i<j; i++, k++) {
            itoa(dump[i], pline3, 16);
            if(strlen(pline3) != 2) {
                pline1[l++] = '0';
                pline1[l++] = pline3[0];
            }
            else {
                pline1[l++] = pline3[0];
                pline1[l++] = pline3[1];
            }
            if ((modulo) && (((l-inserted) % modulo) == 0)) {
                pline1[l++] = ' ';
                inserted++;
            }
            isascii(dump[i]) ? ((isprint(dump[i])) ?
                   (pline2[k] = dump[i]) : (pline2[k] = '.')):
                    (pline2[k] = '.');
        }
        fprintf(dumpsfd, "%07lX: %07lX: %s * %s *\n",
                base+offset, offset, pline1, pline2);
        offset+=group;
    } while (offset < length);

    return(0);
}

/****************************************************************************\
|   FindOldestBlock()                                                        |
|                                                                            |
|   Locate the oldest block in the log file by scanning thru and looking at  |
|   the timestamps on all the blocks.                                        |
\****************************************************************************/

long FindOldestBlock(FILE *logfd)

{
    unsigned int    status;
    unsigned int    i;
    unsigned int    oi;
    unsigned int    buff[BLOCKSIZE/sizeof(int)];
    LOGBLOCK        *LogBuf;

    i =
    oi = 0;
    LogBuf = (LOGBLOCK *)&buff[0];

    /* Position to start of file */

    if (fseek(logfd, 0, 0))
        return -1;

    /* Look for oldest block */

    for (i=0; i<logsize; i+=BLOCKSIZE) {
        if ((status=fread(buff, BLOCKSIZE, 1, logfd)) != 1) {
            if (status) {
                perror("LOGRPT:  Error reading log file ");
                exit(1);
            }
            logsize = i;
            break;
        }

        if (logseq <= LogBuf->time) {
            oi = i;
            logseq = LogBuf->time;
        }
    }

    oi += BLOCKSIZE;
    if (oi >= logsize)
        oi = 0;

    return (long)oi;
}

/****************************************************************************\
|   ProcessLOGBLOCK							     |
\****************************************************************************/

int ProcessLOGBLOCK( char *buff, char start[15], char stop[15],
			char log_diff[5] )
{
    LOGBLOCK    *blk;
    LOGMSG      *msg;
    char        savec;
    static long	lasttime=0;
    char	date[15]; 

    blk = (LOGBLOCK *)buff;
    msg = (LOGMSG *)(buff + sizeof(LOGBLOCK));
    
    while ( blk->msgcount )
    {
        make_format_dateofthisclock( date, (time_t *)&msg->time );

        if( atof(stop) < atof(date) )
            return(0);

        if( atof(start) > atof(date) )
        {
            lasttime = msg->time;
            blk->msgcount--;
            msg = (LOGMSG *)(((char *) msg) + sizeof(LOGMSG) -
                                               DUMMYDATA + msg->count);
         }
    
         if( ( atof(start) <= atof(date) ) && ( atof(date) <= atof(stop) ) )
         {
             if ( msg->time > lasttime + atoi(log_diff)*60 ) 
             {
                printf("Log timestamp : %s  ", ctime((time_t *)&msg->time) );
	        lasttime = msg->time;
             }
             else
	         printf("  ");

       	     switch( msg->type )
	     {

		case LOG_CLEAR:
            	     break;

        	case LOG_TEXT:
         	     	savec = msg->data[msg->count];
            		msg->data[msg->count] = 0;
            		printf("%s", msg->data);
            		if (msg->data[msg->count-1] != '\n')
               			printf("\n");
            		msg->data[msg->count] = savec;
            	     break;

        	     default:
            		printf("Log message type (%x) not supported in report program\n", msg->type);
            		if (msg->count)
               		DumpHex(0, "Unknown message type",msg->data, 0, msg->count, 0);
		     break;
                  }
              
              blk->msgcount--;
              msg = (LOGMSG *)(((char *) msg) + sizeof(LOGMSG) -
                                                   DUMMYDATA + msg->count);
                	
        }
    }    
return 0;
}

/****************************************************************************\
|   search_alarm(): search an alarm in a block				     |
\****************************************************************************/

search_alarm( char *buff, char alarm[20] )
{
    LOGBLOCK    *blk;
    LOGMSG      *msg;
    char        savec;
    char	*string; 
    
    blk = (LOGBLOCK *)buff;
    msg = (LOGMSG *)(buff + sizeof(LOGBLOCK));
 
    while( blk->msgcount ) 
    {
        switch (msg->type)
	{

            case LOG_CLEAR:
            break;

            case LOG_TEXT:
	        savec=msg->data[msg->count];
	        msg->data[msg->count]='\0';
                if( (strstr( msg->data, " MIN " ) != NULL ) ||
            	    (strstr( msg->data, " MAJ " ) != NULL ) ||
            	    (strstr( msg->data, " INF " ) != NULL ) )
	        {
		    string = strstr( msg->data, "-" ) + 1 ;
		    string[4] = '\0' ;
		    if ( strcmp(string,alarm) == 0 )
     	            {	
	                msg->data[msg->count]=savec;
		        return(msg->time);
                    }
	        }
	        msg->data[msg->count]=savec;
            break;

            default:
                printf("Log message type (%x) not supported in report program\n", msg->type);
                if (msg->count)
                    DumpHex(0, "Unknown message type", msg->data, 0, 
				msg->count, 0);
            break;
        }

        blk->msgcount--;
        msg = (LOGMSG *)(((char *) msg) + sizeof(LOGMSG) - DUMMYDATA + msg->count);
    }
    return(0);
}

/****************************************************************************\
|   read_first_time : returns first time of a block                          |
\****************************************************************************/

time_t read_first_time(char *buff )
{
    LOGBLOCK    *blk;
    LOGMSG      *msg;

    blk = (LOGBLOCK *)buff;
    msg = (LOGMSG *)(buff + sizeof(LOGBLOCK));
    return( (time_t )msg->time );
}

/****************************************************************************\
|   DumpLog                                                                  |
\****************************************************************************/

int DumpLog( FILE *logfd, char start[15] , char stop[15], char mode,
	     char time_before_alarm[5], char alarm[20],char log_diff[5] )
{
    int         i,j;
    int         rnd,rnd2;
    int         status;
    time_t      TheTime;
    time_t	time1,time2,time3;
    char        logbuff[BLOCKSIZE+4];
    char	date1[15];
    char	date2[15];
    int 	position;
    rnd = FindOldestBlock(logfd);

    TheTime = time(0);
    printf("\t\t\tOS Log Report - Printed %s\n\n", ctime(&TheTime));
    fflush(stdout);

    if( mode == '2')
    {
        i=0;
        if ((status = fseek(logfd, rnd, 0)) == -1) 
        {
            perror("Could not position into log file ");
            exit(1);
        }
        position = ftell( logfd );
        status = fread( logbuff, BLOCKSIZE, 1, logfd );
        if ( status != 1 ) 
        {
            perror("Failure 0 reading log file ");
            return(1);
        }
        time1 = read_first_time(logbuff);
        fseek(logfd,position,0);
    } 

    for ( j=0; j < (LOGSIZE/BLOCKSIZE); j++ )
    {
        if ( ( status = fseek( logfd, rnd, 0 ) ) == -1 )
        {
            perror("Could not position into log file ");
            exit(1);
        }
        status = fread( logbuff, BLOCKSIZE, 1, logfd );
        if ( status != 1 ) 
	{
            perror("Failure 1 reading log file ");
            return(1);
        }
        
        if( mode == '1' )
            ProcessLOGBLOCK( logbuff, start, stop, log_diff );

	if( mode == '2' )
        {
	    time2 = search_alarm( logbuff, alarm ) ;
 	    if( time2 != 0 )
            {
        	status = fread( logbuff, BLOCKSIZE, 1, logfd );
        	if (status != 1) 
		{
            	    perror("Failure 1 reading log file ");
                    return(1);
                }
                time3 = read_first_time( logbuff );

                if( time2 - time1 >= 60*atoi( time_before_alarm ) )
                       time1 = time2 - 60*atoi( time_before_alarm );

                make_format_dateofthisclock( date1, &time1 );
                make_format_dateofthisclock( date2, &time3 );

                rnd2 = position ; 
                if ( ( atof(date2) > atof(start) ) &&
			 ( atof(date1) < atof(stop) ) ) 
		{
		    printf("\n***********ALARM DETECTED*********** at log timestamp %s",ctime(&time2));
		    if ( atof(date1) < atof(start) )
		        strcpy(date1,start);
		    if ( atof(date2) > atof(stop) )
		        strcpy(date2,stop);

		    while(i<=j)
	            {
                        if ((status = fseek(logfd, rnd2, 0)) == -1) 
			{
                             perror("Could not position into log file ");
                             exit(1);
                        }
                        status = fread(logbuff, BLOCKSIZE, 1, logfd);
                        if (status != 1) 
			{
                             perror("Failure 2 reading log file ");
                             return(1);
                        }
	                ProcessLOGBLOCK( logbuff, date1, date2, log_diff );
		        i++;                    
                        if ((rnd2 += BLOCKSIZE) >= LOGSIZE)
                            rnd2 = 0;
		    } 
                    position=ftell( logfd );
                }
            }
        }
        if ((rnd += BLOCKSIZE) >= LOGSIZE)
        rnd = 0;
    }
    return (0);
}

/****************************************************************************\
|   help(): help message if error in using the logrpt command                |
\****************************************************************************/

help()
{
	struct timeval time;
	struct timezone zone;
	char date[15];
	long clock;
	
	gettimeofday(&time,&zone);
	clock=(long)time.tv_sec;
	make_format_dateofthisclock(date,&clock);
	printf("You need some help.\n\n");
	printf("Optinnal arguments : \n\n");
	printf("-d start stop -a alarm -l min1 -L min2\n\n");
	printf("for the dates : \n");
	printf("if you want to write : %sjust put %s\n",ctime(&clock),date); 
	printf("The default values are :\n");
	printf("start : 19700000000000 stop : date and time of right now : %s\n\n",date);
	printf("alarm : to specify messages containing the alarm you want to display\n\n");
	printf("min1: displays messages of the 5 (or min1 if precised) last minutes before every wanted alarm\n\n");
	printf("min2: displays log timestamp at every change (or at every min2 minutes change if precised)\n\n");
	printf("Here is a good use:\nlogrpt -d 19930706102056 19930706112245 -a MAJ -l 2 -L 5\n");
	exit(0);
}	

/****************************************************************************\
|   M A I N                                                                  |
\****************************************************************************/

int main(argc,argv)
int argc;
char **argv;
{
    char    *logfile;
    FILE    *logfd;
    char    start[15];
    char    stop[15];
    char    date[15]; 
    struct  timeval    time;
    struct  timezone   zone;
    long    clock;
    char    alarm[5];
    char    time_before_alarm[5];
    char    mode;
    int     i,j;
    char    log_diff[5];
 
    logfile = LOGFILE ;
    mode= '1' ;
    strcpy( start, "19700101000000" );
    gettimeofday( &time, &zone );
    clock=(long)time.tv_sec;	
    make_format_dateofthisclock( stop, &clock ); 
    strcpy( time_before_alarm, "5" );
    strcpy( alarm, "4444" );
    strcpy( log_diff, "5" );
 
    for( i=1; i<argc; i++ )
    {
        if( argv[i][0] == '-' )
        {
	    switch( argv[i][1] )
            {

		case 'd' :
		    if( mode != 2 )
			mode = '1';
		    if( argc <= i )
			printf("\nstop will be by default %s", ctime(&clock) );
		    else
		    {
		        if( check_date(argv[i+1]) == 0 )
			    help();
			else
			{
			    strcpy( start, argv[i+1] );
			    i++;
			    if( argc <= i )
				printf("\ndefault stop : today\n");
			    else
			    {
				if( check_date(argv[i+1]) == 0 )
				    help();
				else
				{
				    strcpy( stop, argv[i+1] );
				    i++;    
				}	
			    }		
			} 
		    } 
		    break;

		case 'l' :
		    if( (argc <= i) || ( check_number_string(argv[i+1]) == 0 ) )
			help();
		    else
		    {
			i++;
			strcpy( time_before_alarm, argv[i] );
		    }
		    break;

		case 'a':
		    mode = '2';
		    if( ( argc <= i ) || 
			( check_number_string(argv[i+1]) == 0 ) || 
			( strlen(argv[i+1]) > 4 ) )
	                help();
		    else
		    {
		    	i++;
			if( strlen(argv[i]) < 4 )
			{
			    strcpy( alarm, " " );
			    for( j=0; j < strlen(argv[i]) -3 ; j++ )
			    {
				strcat( alarm, " " );
			    }
			    strcat( alarm, argv[i] );
			}
			else
			    strcpy( alarm, argv[i] );
		    } 
		    break;

		case 'L':
		    if( ( argc <= i ) || 
			( check_number_string(argv[i+1]) == 0 ) )
			help();
		    else
		    {
			i++;
			strcpy( log_diff, argv[i] );
		    }
		    break;

		default :
		    help();
		    break;
	    }
        }
        else
	    help();
    }   


   if ( atof(start) > atof(stop) )
   {
	strcpy( date, start );
	strcpy( start, stop );
	strcpy( stop, date );
   }

   logfd = fopen( logfile, "r" );
   if ( logfd == NULL )
   {
       fprintf(stderr, "LOGRPT:  Cannot open log - %s ", logfile);
       perror("");
       exit(1);
   }
  
   system("clear");   
   printf("The process can be quite long so don't worry, it's gonna come one day\n\n\n"); 
   printf("Thanks again for your patience ......\n"); 
   fflush(stdout); 
   return ( DumpLog( logfd, start, stop, mode, time_before_alarm,
		     alarm, log_diff ) );
}

