char banner[]	= "TSC Menu v1.0          (C) INTECOM, Inc. 1993\n";


#include	<types.h>
#include	<wait.h>
#include	<stdio.h>
#include	<string.h>
#include	<stat.h>
#include	<dir.h>
#include	<a.out.h>
#include	<errno.h>
#include	"fe.h"

#define	MainScreen	0
#define	SysScreen	1
#define	FileScreen	2
#define	CopyScreen	3
#define	MoveScreen	4
#define	DelScreen	5
#define	FindScreen	6
#define	QuickDirScreen	7
#define	QuickFileScreen	8
#define	TarScreen	9
#define	SpoolScreen	10
#define	LogScreen	11
#define ListScreen	12
#define TaskScreen	13

extern int errno;

char	*msgErr[10] = {
		"Directory does not exist !",
		"Not a directory !"
};

FIELD	feOutMain[] = {
0,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
0,0,0,0, 3, 10, 57,0,0, '-', NULL,
0,0,0,0, 5, 10,  0,0,0,   0, "KEY   UTILITY     DESCRIPTION",
0,0,0,0, 6, 10, 57,0,0, '-', NULL,
0,0,0,0, 7, 10,  0,0,0,   0, "F1    EDIT        Edit utility (cr)",
0,0,0,0, 8, 10,  0,0,0,   0, "F2    DISPLAY     File display utility (cat)",
0,0,0,0, 9, 10,  0,0,0,   0, "F3    BACKUP      Backup utility (tar)",
0,0,0,0,10, 10,  0,0,0,   0, "F4    FILELIST    Display/Print a sorted file list (ls)",
0,0,0,0,11, 10,  0,0,0,   0, "F5    SEARCH      File search utility (find)",
0,0,0,0,12, 10,  0,0,0,   0, "F6    PATCH       Patch utility (patch)",
0,0,0,0,13, 10,  0,0,0,   0, "F7    SYSTEM      System Utilities sub-menu",
0,0,0,0,14, 10,  0,0,0,   0, "F8    FILES       Files Utilities sub-menu",
0,0,0,0,15, 10,  0,0,0,   0, "F9    CHECKSUM    Checksum calculation utility (ck)",
0,0,0,0,16, 10,  0,0,0,   0, "F10   QUICKDIR    Quick directory selection",
0,0,0,0,17, 10,  0,0,0,   0, "F11   QUICKFILE   Quick file selection",
0,0,0,0,18, 10,  0,0,0,   0, "F12   LOGOFF",
0,0,0,0,20, 10,  0,0,0,   0, "File : >>",
0,0,0,0,20, 68,  0,0,0,   0, "<<",
0,0,0,0,22, 10,  0,0,0,   0, "Default directory : ",
0,0,0,0,22, 30, 38,0,0, ' ', NULL
};

FIELD	feOutSys[] = {
1,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
1,0,0,0, 3, 10, 57,0,0, '-', NULL,
1,0,0,0, 5, 33,  0,0,0,   0, "SYSTEM UTILITIES",
1,0,0,0, 8, 10,  0,0,0,   0, "F1    DISKUSE     Disk usage utility (df)",
1,0,0,0, 9, 10,  0,0,0,   0, "F2    LOG REPORT  OS log report utility (logrpt) [future]",
1,0,0,0,10, 10,  0,0,0,   0, "F3    TASK LIST   Tasks Management utility (ps)",
1,0,0,0,11, 10,  0,0,0,   0, "F4    SPOOLER     List spooled files",
1,0,0,0,12, 10,  0,0,0,   0, "F12   MAIN MENU"
};

FIELD	feOutFile[] = {
2,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
2,0,0,0, 3, 10, 57,0,0, '-', NULL,
2,0,0,0, 5, 33,  0,0,0,   0, "FILE UTILITIES",
2,0,0,0, 7, 10,  0,0,0,   0, "F1    EDIT        Edit utility (cr)",
2,0,0,0, 8, 10,  0,0,0,   0, "F2    DISPLAY     File display utility (cat)",
2,0,0,0, 9, 10,  0,0,0,   0, "F3    DELETE      Delete a file (rm)",
2,0,0,0,10, 10,  0,0,0,   0, "F4    FILELIST    Display/Print a sorted file list (ls)",
2,0,0,0,11, 10,  0,0,0,   0, "F5    SEARCH      File search utility (find)",
2,0,0,0,12, 10,  0,0,0,   0, "F6    COPY        Copy file (cp)",
2,0,0,0,13, 10,  0,0,0,   0, "F7    MOVE        Move file (mv)",
2,0,0,0,14, 10,  0,0,0,   0, "F8    TRANSFER    File Transfer utility (ftp)",
2,0,0,0,15, 10,  0,0,0,   0, "F9    CHECKSUM    Checksum calculation utility (ck)",
2,0,0,0,16, 10,  0,0,0,   0, "F10   QUICKDIR    Quick directory selection",
2,0,0,0,17, 10,  0,0,0,   0, "F11   QUICKFILE   Quick file selection",
2,0,0,0,18, 10,  0,0,0,   0, "F12   MAIN MENU",
2,0,0,0,20, 10,  0,0,0,   0, "File : >>",
2,0,0,0,20, 68,  0,0,0,   0, "<<",
2,0,0,0,22, 10,  0,0,0,   0, "Default directory : ",
2,0,0,0,22, 30, 38,0,0, ' ', NULL
};

FIELD	feOutCopy[] = {
3,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
3,0,0,0, 3, 10, 57,0,0, '-', NULL,
3,0,0,0, 5, 33,  0,0,0,   0, "COPY UTILITY",
3,0,0,0, 7, 10,  0,0,0,   0, "F1    EXECUTE COPY",
3,0,0,0, 9, 10,  0,0,0,   0, "F10   QUICKDIR    Quick directory selection",
3,0,0,0,10, 10,  0,0,0,   0, "F11   QUICKFILE   Quick file selection",
3,0,0,0,11, 10,  0,0,0,   0, "F12   CANCEL",
3,0,0,0,16, 10,  0,0,0,   0, "from : >>",
3,0,0,0,16, 68,  0,0,0,   0, "<<",
3,0,0,0,18, 10,  0,0,0,   0, "to   : >>",
3,0,0,0,18, 68,  0,0,0,   0, "<<",
3,0,0,0,20, 19,  0,0,0,   0, "Make destination file contiguous Y/N ? >>",
3,0,0,0,20, 61,  0,0,0,   0, "<<",
3,0,0,0,14, 10,  0,0,0,   0, "Default directory : ",
3,0,0,0,14, 30, 38,0,0, ' ', NULL
};

FIELD	feOutMove[] = {
4,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
4,0,0,0, 3, 10, 57,0,0, '-', NULL,
4,0,0,0, 5, 33,  0,0,0,   0, "MOVE UTILITY",
4,0,0,0, 7, 10,  0,0,0,   0, "F1    EXECUTE MOVE",
4,0,0,0, 9, 10,  0,0,0,   0, "F10   QUICKDIR    Quick directory selection",
4,0,0,0,10, 10,  0,0,0,   0, "F11   QUICKFILE   Quick file selection",
4,0,0,0,11, 10,  0,0,0,   0, "F12   CANCEL",
4,0,0,0,16, 10,  0,0,0,   0, "from : >>",
4,0,0,0,16, 68,  0,0,0,   0, "<<",
4,0,0,0,18, 10,  0,0,0,   0, "to   : >>",
4,0,0,0,18, 68,  0,0,0,   0, "<<",
4,0,0,0,14, 10,  0,0,0,   0, "Default directory : ",
4,0,0,0,14, 30, 38,0,0, ' ', NULL
};

FIELD	feOutDel[] = {
5,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
5,0,0,0, 3, 10, 57,0,0, '-', NULL,
5,0,0,0, 5, 33,  0,0,0,   0, "DELETE FILE",
5,0,0,0, 8, 10,  0,0,0,   0, "F1    EXECUTE DELETE",
5,0,0,0,11, 10,  0,0,0,   0, "F12   CANCEL",
5,0,0,0,16, 10,  0,0,0,   0, "Do you want to delete this file ?",
5,0,0,0,18, 10,  0,0,0,   0, "file :",
5,0,0,0,18, 17, 38,0,0, ' ', NULL
};

FIELD	feOutFind[] = {
6,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
6,0,0,0, 3, 10, 57,0,0, '-', NULL,
6,0,0,0, 5, 33,  0,0,0,   0, "SEARCH UTILITY",
6,0,0,0, 7, 10,  0,0,0,   0, "F1    EXECUTE SEARCH",
6,0,0,0, 9, 10,  0,0,0,   0, "F10   QUICKDIR    Quick directory selection",
6,0,0,0,10, 10,  0,0,0,   0, "F11   QUICKFILE   Quick file selection",
6,0,0,0,11, 10,  0,0,0,   0, "F12   CANCEL",
6,0,0,0,14, 10,  0,0,0,   0, "file : >>",
6,0,0,0,14, 68,  0,0,0,   0, "<<",
6,0,0,0,16, 10,  0,0,0,   0, "in   : >>",
6,0,0,0,16, 68,  0,0,0,   0, "<<"
};

FIELD	feOutQdir[] = {
7,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
7,0,0,0, 3, 10, 57,0,0, '-', NULL,
7,0,0,0, 5, 28,  0,0,0,   0, "QUICK FINDER : DIRECTORY",
7,1,0,0, 9, 23,  0,0,0,   0, " 1. /",
7,2,0,0,11, 23,  0,0,0,   0, " 2. /bin",
7,3,0,0,12, 23,  0,0,0,   0, " 3. /dev",
7,4,0,0,13, 23,  0,0,0,   0, " 4. /etc",
7,5,0,0,14, 23,  0,0,0,   0, " 5. /net",
7,6,0,0,15, 23,  0,0,0,   0, " 6. /bin",
7,7,0,0, 9, 43,  0,0,0,   0, " 7. /ibx/cfg",
7,8,0,0,10, 43,  0,0,0,   0, " 8. /ibx/pfl",
7,9,0,0,11, 43,  0,0,0,   0, " 9. /ibx/tsk",
7,10,0,0,12, 43,  0,0,0,   0, "10. /ibx/iod",
7,11,0,0,14, 43,  0,0,0,   0, "11. /usr/cse",
7,12,0,0,15, 43,  0,0,0,   0, "12. /usr/tsc",
7,0,0,0,18, 10,  0,0,0,   0, "Enter your selection :    >>",
7,0,0,0,18, 40,  0,0,0,   0, "<<",
7,0,0,0,19, 10,  0,0,0,   0, "or",
7,0,0,0,20, 10,  0,0,0,   0, "Enter default directory : >>",
7,0,0,0,20, 69,  0,0,0,   0, "<<"
};

FIELD	feOutQfile[] = {
8,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
8,0,0,0, 3, 10, 57,0,0, '-', NULL,
8,0,0,0, 5, 29,  0,0,0,   0, "QUICK FINDER : FILE",
8,0,0,0, 9,  9,  0,0,0,   0, "System files :",
8,1,0,0,10, 12,  0,0,0,   0, " 1. Operating system",
8,2,0,0,11, 12,  0,0,0,   0, " 2. Hosts",
8,3,0,0,12, 12,  0,0,0,   0, " 3. Tape file list",
8,0,0,0,14,  9,  0,0,0,   0, "Configuration files :",
8,4,0,0,15, 12,  0,0,0,   0, " 4. tskcfg",
8,5,0,0,16, 12,  0,0,0,   0, " 5. pflcfg",
8,6,0,0,17, 12,  0,0,0,   0, " 6. iodcfg",
8,7,0,0,18, 12,  0,0,0,   0, " 7. siteid",
8,0,0,0, 9, 37,  0,0,0,   0, "Intecom E task files :",
8,8,0,0,10, 40,  0,0,0,   0, " 8. AB  Auto Backup task",
8,9,0,0,11, 40,  0,0,0,   0, " 9. AR  Account Recording task",
8,10,0,0,12, 40,  0,0,0,   0, "10. CM  Console Monitor task",
8,11,0,0,13, 40,  0,0,0,   0, "11. CP  Call Processing task",
8,12,0,0,14, 40,  0,0,0,   0, "12. FC  File Copy task",
8,13,0,0,15, 40,  0,0,0,   0, "13. MM  Man-Machine task",
8,14,0,0,16, 40,  0,0,0,   0, "14. OV  MM Overlays",
8,15,0,0,17, 40,  0,0,0,   0, "15. RT  Run Time library",
8,16,0,0,18, 40,  0,0,0,   0, "16. UT  Utilities task",
8,0,0,0,20, 25,  0,0,0,   0, "Enter your selection : >>",
8,0,0,0,20, 52,  0,0,0,   0, "<<"
};

FIELD	feOutTar[] = {
9,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
9,0,0,0, 3, 10, 57,0,0, '-', NULL,
9,0,0,0, 5, 30,  0,0,0,   0, "BACKUP/RESTORE UTILITY",
9,0,0,0, 7, 10,  0,0,0,   0, "F1    EDIT FILE LIST      = >>",
9,0,0,0, 7, 68,  0,0,0,   0, "<<",
9,0,0,0, 9, 10,  0,0,0,   0, "F2    BACKUP      to tape",
9,0,0,0,10, 10,  0,0,0,   0, "F3    RESTORE     from tape",
9,0,0,0,11, 10,  0,0,0,   0, "F4    LIST        tape",
9,0,0,0,13, 10,  0,0,0,   0, "F10   QUICKDIR    Quick directory selection",
9,0,0,0,14, 10,  0,0,0,   0, "F11   QUICKFILE   Quick file selection",
9,0,0,0,15, 10,  0,0,0,   0, "F12   CANCEL",
9,0,0,0,17, 15,  0,0,0,   0, "Object:",
9,0,0,0,17, 23,  0,0,0,   0, "PFLs..............P",
9,0,0,0,18, 23,  0,0,0,   0, "Single file.......S",
9,0,0,0,19, 23,  0,0,0,   0, "List of files.....L",
9,0,0,0,20, 23,  0,0,0,   0, "All files ........A",
9,0,0,0,20, 45,  0,0,0,   0, "= >>",
9,0,0,0,20, 50,  0,0,0,   0, "<<",
9,0,0,0,22, 10,  0,0,0,   0, "File : >>",
9,0,0,0,22, 68,  0,0,0,   0, "<<"
};

FIELD	feOutSpool[] = {
10,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
10,0,0,0, 3, 10, 57,0,0, '-', NULL,
10,0,0,0, 5, 30,  0,0,0,   0, "SPOOLER UTILITY",
10,0,0,0, 7, 10,  0,0,0,   0, "F1    PRINT       File print utility (lpr)",
10,0,0,0, 8, 10,  0,0,0,   0, "F2    DISPLAY     File display utility (cat)",
10,0,0,0, 9, 10,  0,0,0,   0, "F3    DELETE      Delete a file (rm)",
10,0,0,0,10, 10,  0,0,0,   0, "F12   CANCEL",
10,0,0,0,12, 25,  0,0,0,   0, "Enter your selection : >>",
10,0,0,0,12, 52,  0,0,0,   0, "<<",
10,0,0,0,14,  1,  0,0,0,   0, " 1.",
10,0,0,0,14,  5, 75,0,0, ' ', NULL,
10,0,0,0,15,  1,  0,0,0,   0, " 2.",
10,0,0,0,15,  5, 75,0,0, ' ', NULL,
10,0,0,0,16,  1,  0,0,0,   0, " 3.",
10,0,0,0,16,  5, 75,0,0, ' ', NULL,
10,0,0,0,17,  1,  0,0,0,   0, " 4.",
10,0,0,0,17,  5, 75,0,0, ' ', NULL,
10,0,0,0,18,  1,  0,0,0,   0, " 5.",
10,0,0,0,18,  5, 75,0,0, ' ', NULL,
10,0,0,0,19,  1,  0,0,0,   0, " 6.",
10,0,0,0,19,  5, 75,0,0, ' ', NULL,
10,0,0,0,20,  1,  0,0,0,   0, " 7.",
10,0,0,0,20,  5, 75,0,0, ' ', NULL,
10,0,0,0,21,  1,  0,0,0,   0, " 8.",
10,0,0,0,21,  5, 75,0,0, ' ', NULL,
10,0,0,0,22,  1,  0,0,0,   0, " 9.",
10,0,0,0,22,  5, 75,0,0, ' ', NULL,
10,0,0,0,23,  1,  0,0,0,   0, "10.",
10,0,0,0,23,  5, 75,0,0, ' ', NULL
};

FIELD	feOutLog[] = {
11,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
11,0,0,0, 3, 10, 57,0,0, '-', NULL,
11,0,0,0, 5, 30,  0,0,0,   0, "LOG REPORT UTILITY",
11,0,0,0, 7, 10,  0,0,0,   0, "F1    EDIT        Save and edit report (cr)",
11,0,0,0, 8, 10,  0,0,0,   0, "F2    DISPLAY     Display report",
11,0,0,0, 9, 10,  0,0,0,   0, "F3    SAVE        Save report to a file",
11,0,0,0,10, 10,  0,0,0,   0, "F12   CANCEL",
11,0,0,0,14, 10,  0,0,0,   0, "File : >>",
11,0,0,0,14, 68,  0,0,0,   0, "<<"
};

FIELD	feOutList[] = {
12,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
12,0,0,0, 3, 10, 57,0,0, '-', NULL,
12,0,0,0, 5, 23,  0,0,0,   0, "DISPLAY/PRINT A SORTED FILE LIST",
12,0,0,0, 8, 10,  0,0,0,   0, "F1    DISPLAY",
12,0,0,0,11, 10,  0,0,0,   0, "F2    PRINT",
12,0,0,0,14, 10,  0,0,0,   0, "F12   MAIN MENU",
};

FIELD	feOutTask[] = {
13,0,0,0, 2, 10,  0,0,0,   0, "Intecom E - (TSC) Technical Service Center - Utility Menu",
13,0,0,0, 3, 10, 57,0,0, '-', NULL,
13,0,0,0, 5, 30,  0,0,0,   0, "PS UTILITY",
13,0,0,0, 7, 10,  0,0,0,   0, NULL,
13,0,0,0, 8, 10,  0,0,0,   0, NULL,
13,0,0,0, 9, 10,  0,0,0,   0, "F1    KILL      kill a process (kill)",
13,0,0,0,10, 10,  0,0,0,   0, "F12   CANCEL",
13,0,0,0,12, 25,  0,0,0,   0, "Enter your selection : >>",
13,0,0,0,12, 52,  0,0,0,   0, "<<",
13,0,0,0,13,  5, 80,0,0,   0, "pid ppid pgrp pri text  stack data     time dev    user     S name",
13,0,0,0,14,  1,  0,0,0,   0, " 1.",
13,0,0,0,14,  5, 75,0,0, ' ', NULL,
13,0,0,0,15,  1,  0,0,0,   0, " 2.",
13,0,0,0,15,  5, 75,0,0, ' ', NULL,
13,0,0,0,16,  1,  0,0,0,   0, " 3.",
13,0,0,0,16,  5, 75,0,0, ' ', NULL,
13,0,0,0,17,  1,  0,0,0,   0, " 4.",
13,0,0,0,17,  5, 75,0,0, ' ', NULL,
13,0,0,0,18,  1,  0,0,0,   0, " 5.",
13,0,0,0,18,  5, 75,0,0, ' ', NULL,
13,0,0,0,19,  1,  0,0,0,   0, " 6.",
13,0,0,0,19,  5, 75,0,0, ' ', NULL,
13,0,0,0,20,  1,  0,0,0,   0, " 7.",
13,0,0,0,20,  5, 75,0,0, ' ', NULL,
13,0,0,0,21,  1,  0,0,0,   0, " 8.",
13,0,0,0,21,  5, 75,0,0, ' ', NULL,
13,0,0,0,22,  1,  0,0,0,   0, " 9.",
13,0,0,0,22,  5, 75,0,0, ' ', NULL,
13,0,0,0,23,  1,  0,0,0,   0, "10.",
13,0,0,0,23,  5, 75,0,0, ' ', NULL
};


FIELD	feInMain[] = {
0,0,5,0,20, 19, 49,0,0, ' ', NULL	/* Current file name */
};

FIELD	feInSys[] = {
1,0,5,0,20, 19, 49,0,0, ' ', NULL	/* dummy */
};

FIELD	feInFile[] = {
2,0,5,0,20, 19, 49,0,0, ' ', NULL	/* Current file name */
};

FIELD	feInCopy[] = {
3,0,5,0,16, 19, 49,0,0, ' ', NULL,	/* from file */
3,1,5,0,18, 19, 49,0,0, ' ', NULL,	/* to file */
3,2,7,0,20, 60,  1,0,0, ' ', NULL	/* contiguous ? */
};

FIELD	feInMove[] = {
4,0,5,0,16, 19, 49,0,0, ' ', NULL,	/* from file */
4,1,5,0,18, 19, 49,0,0, ' ', NULL	/* to file */
};

FIELD	feInDel[] = {
5,0,5,0,21, 19, 49,0,0, ' ', NULL	/* dummy */
};

FIELD	feInFind[] = {
6,0,5,0,14, 19, 49,0,0, ' ', NULL,	/* file */
6,1,5,0,16, 19, 49,0,0, ' ', NULL	/* in directory */
};

FIELD	feInQdir[] = {
7,0,7,0,18, 38,  2,0,0, '_', NULL,	/* directory selection */
7,1,5,0,20, 38, 30,0,0, ' ', NULL	/* default directory */
};

FIELD	feInQfile[] = {
8,0,7,0,20, 50,  2,0,0, '_', NULL	/* file selection */
};

FIELD	feInTar[] = {
9,1,7,0,20, 49,  1,0,0, '_', NULL,	/* PFL, Single, List or All */
9,2,5,0,22, 19, 36,0,0, ' ', NULL,	/* file */
9,2,5,0, 7, 40, 28,0,0, ' ', NULL	/* file list */
};

FIELD	feInSpool[] = {
10,0,7,0,12, 50,  2,0,0, '_', NULL	/* file selection */
};

FIELD	feInLog[] = {
11,0,5,0,14, 19, 49,0,0, ' ', NULL	/* File name */
};

FIELD	feInList[] = {
12,0,5,0,20, 19, 49,0,0, ' ', NULL	/* dummy */
};

FIELD	feInTask[] = {
13,0,7,0,12, 50, 2,0,0, '_', NULL	/* process selection */
};

#define numOutMain	(sizeof(feOutMain)/sizeof(FIELD))
#define numOutSys	(sizeof(feOutSys)/sizeof(FIELD))
#define numOutFile	(sizeof(feOutFile)/sizeof(FIELD))
#define numOutCopy	(sizeof(feOutCopy)/sizeof(FIELD))
#define numOutMove	(sizeof(feOutMove)/sizeof(FIELD))
#define numOutDel	(sizeof(feOutDel)/sizeof(FIELD))
#define numOutFind	(sizeof(feOutFind)/sizeof(FIELD))
#define numOutQdir	(sizeof(feOutQdir)/sizeof(FIELD))
#define numOutQfile	(sizeof(feOutQfile)/sizeof(FIELD))
#define numOutTar	(sizeof(feOutTar)/sizeof(FIELD))
#define numOutSpool	(sizeof(feOutSpool)/sizeof(FIELD))
#define numOutLog	(sizeof(feOutLog)/sizeof(FIELD))
#define numOutList	(sizeof(feOutList)/sizeof(FIELD))
#define numOutTask	(sizeof(feOutTask)/sizeof(FIELD))

#define numInMain	(sizeof(feInMain)/sizeof(FIELD))
#define numInSys	(sizeof(feInSys)/sizeof(FIELD))
#define numInFile	(sizeof(feInFile)/sizeof(FIELD))
#define numInCopy	(sizeof(feInCopy)/sizeof(FIELD))
#define numInMove	(sizeof(feInMove)/sizeof(FIELD))
#define numInDel	(sizeof(feInDel)/sizeof(FIELD))
#define numInFind	(sizeof(feInFind)/sizeof(FIELD))
#define numInQdir	(sizeof(feInQdir)/sizeof(FIELD))
#define numInQfile	(sizeof(feInQfile)/sizeof(FIELD))
#define numInTar	(sizeof(feInTar)/sizeof(FIELD))
#define numInSpool	(sizeof(feInSpool)/sizeof(FIELD))
#define numInLog	(sizeof(feInLog)/sizeof(FIELD))
#define numInList	(sizeof(feInList)/sizeof(FIELD))
#define numInTask	(sizeof(feInTask)/sizeof(FIELD))

struct	stat	fileStatus;
char	curdir[MAXPATHLEN];
char	fromfile[60] = {"\0"};
char	tofile[60] = {"\0"};
char	finddir[60] = {"\0"};
char tapefl[60] = {"/mylist"};

int	hasErr = 0;


/*
/***** converts all caps to small caps in a string
/***** the chars are supposed to be ASCII !!
/*
*/
convcaps (string)
char *string;
{
	char *caps	= "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int  i;
	
	for(i=0; i<strlen(string); i++) {
		if (strchr(caps, string[i])) string[i] += 0x20;
	}
}

/*
/*     Find a file name starting with a specific string in a config file.
/*     Returns the name of the file found or "".
/*
*/
char *findfile (taskString)
char *taskString;
{
	FILE	*fp;
	char	oneline[256];
	int	i;
	char	*filestr;

	fp = fopen("/ibx/cfg/tskcfg", "r");
	if (fp == NULL) {  /* could not open the config file */
		return("");
	}

	while (fgets(oneline, 256, fp) != NULL) { /* read one line */

	/* get rid of empty line or comment only lines */
		
		if (oneline[0] == '\n') continue; /* skip empty line */
		if (oneline[0] == '*') continue; /* skip comment line */
		i = strspn(oneline," \t");         /* skip spaces or TABS */
		if (oneline[i] == '*') continue; /* still a comment line */
	
	/* should be a file description line */

		convcaps(taskString); /* convert every ASCII character to small caps */
		convcaps(oneline);
		filestr = strstr(oneline, taskString); /* look for beginning of the file name */
		if (filestr == NULL) continue;  /* did not find "taskString" in this line */
		i = strcspn(filestr," ,\t\n*"); /* look for end of name */
		filestr[i] = '\0';
		return(filestr);  /* found the filename */
	}
	return(""); /* end of file hit */
} /* findfile */

/*
/*	checkFile : build a complete file name and check if the file exist
/*
/*	returns	0	file does not exist
/*		1	multiple files (file contains wildcards)
/*		2	multiple directories (dir contains wildcards)
/*		S_IFDIR	directory
/*		S_IFREG	regular ascii file
/*		OMAGIC	object file
/*		ZMAGIC	executable file
/*		else	special file.
*/
checkFile (file)
char	*file;
{
	int	filetype, fd, rc;
	struct	exec header;

	/* check if it is a single file or directory or if the name contains
			a wildcard char */
	if (file) {
		if (strcspn(file,"?*") != strlen(file)) return(1);
	}

	/* check if the file exist and the type of the file */

	if (stat(file, &fileStatus)) {
			/* file does not exist for all sorts of reasons */
		return(0);
	}
	else {
		filetype = fileStatus.st_mode & S_IFMT;
		if (filetype == S_IFREG) {
			/* this is a regular file, open it to analyse its type further */
			fd = open(file, 0);
			if (fd == -1) {
				return(0);
			}
	
			/* read file header information */
	
			rc = read(fd, &header, sizeof(struct exec));

			switch (header.a_magic) {
	
			case OMAGIC:
				filetype = OMAGIC;
				break;
			case ZMAGIC:
				filetype = ZMAGIC;
				break;
			default: break;
			}
			close(fd);
		}
	}
	return(filetype);
}

/*
/*	callSystem : goes back to COOKED mode and ask the OS to execute
/*					a utility command.
/*
*/
callSystem(command)
char	*command;
{
	int	status;

	fflush(stdout);
	feSuspend(); /* restore the COOKED mode for patch to run properly */
	
	status = system(command);

	feResume(); /* restore CBREAK mode to return to the menu */
	
	if (status == 0x7f) feError("System error - Press any key to continue");
	else feError("Press any key to continue");
	fePause();	/* wait for any key to be pressed and ignore the key */
	feError(NULL); /* erase the Error/Info line */
	return(status);
}

/*
/*	gocr : Executes "crisp" and wait
/*
*/
gocr (returnScreen, file)
int	returnScreen;
char	*file;
{	
	int	status;
	char	command[80];

	switch (checkFile(file)) {
		case 0:
			if (errno != ENOENT) {
				hasErr = feError(sys_errlist[errno]);
				sleep(1);
				feClearScreen();	
				feSwitchScreen(returnScreen);
				return(1); /* file cannot be built */
			}
			break;
		case 1:
		case 2:
			hasErr = feError("Cannot edit multiple files");
			sleep(1);
			feClearScreen();	
			feSwitchScreen(returnScreen);
			return(1);
		case OMAGIC:
		case ZMAGIC:
			hasErr = feError("Object file : cannot be edited with 'cr'");
			sleep(1);
			feClearScreen();	
			feSwitchScreen(returnScreen);
			return(1);
		case S_IFDIR:
			hasErr = feError("Directory : cannot be edited with 'cr'");
			sleep(1);
			feClearScreen();	
			feSwitchScreen(returnScreen);
			return(1);
		case S_IFREG:
			break;
		default:
			hasErr = feError("Special file : cannot be edited with 'cr'");
			sleep(1);
			feClearScreen();	
			feSwitchScreen(returnScreen);
			return(1);
		}


	strcpy (command, "/usr/local/bin/cr ");
	strcat (command, file);

	feClearScreen();
	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	gocat : Executes "cat" and wait
/*
*/
gocat (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	status;
	char	command[80] = {"/bin/cat "};

	switch (checkFile(file)) {
		case 0:
		hasErr = feError(sys_errlist[errno]);
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1); /* file does not exist */
		case OMAGIC:
		case ZMAGIC:
			hasErr = feError("Object file : cannot be displayed with 'cat'");
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1);
		case S_IFDIR:
			hasErr = feError("Directory : cannot be displayed with 'cat'");
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1);
		case 1:
			hasErr = feError("Cannot display multiple files");
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1);
		case S_IFREG:
			break;
		default:
			hasErr = feError("Special file : cannot be displayed with 'cat'");
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1);
		}

	feClearScreen();

	strcat(command, file);
	strcat(command, " | more");
	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	golpr : Executes "lpr" and wait
/*
*/
golpr (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	status;
	char	command[80] = {"/bin/lpr "};

	switch (checkFile(file)) {
		case 0:
			hasErr = feError(sys_errlist[errno]);
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1); /* file does not exist */
		case OMAGIC:
		case ZMAGIC:
			hasErr = feError("Object file : cannot be printed");
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1);
		case S_IFDIR:
			hasErr = feError("Directory : cannot be printed");
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1);
		case 1:
			hasErr = feError("Cannot print multiple files");
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1);
		case S_IFREG:
			break;
		default:
			hasErr = feError("Special file : cannot be printed");
			sleep(1);	
			feClearScreen();	
			feSwitchScreen(returnScreen);	
			return(1);
		}

	feClearScreen();

	strcat(command, file);
	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}




/*
/*	gotar : Executes "tar" and wait
/*
*/
gotar (direction, numoffiles, file)
int	direction, numoffiles;
char	*file;
{
	int	status;
	char	command[80];

	feClearScreen();

	strcpy(command, "/bin/tar ");

	switch (direction) {
	case FK2:
		strcat(command, "cvW");
		printf("Backing up ");
		break;
	case FK3:
		strcat(command, "xv");
		printf("Restoring ");
		break;
	default:
		strcat(command, "t");
		printf("Listing ");
		break;
	}

	switch (numoffiles) {
	case 'l':	/* the files listed in "file" */
		strcat(command, "T ");
		strcat(command, file);
		printf("files listed in %s.\n\n", file);
		break;
	case 'p':	/* the entire pfl directory */
		strcat(command, " /ibx/pfl");
		printf("PFL files\n\n");
		break;
	case 's':	/* the single file "file" */
		strcat(command, " ");
		strcat(command, file);
		printf("file %s.\n\n", file);
		break;
	}
	fflush(stdout);

	chdir("/"); /* set the current directory to the root for the duration of tar */
	callSystem(command);
	chdir(curdir);    /* restore the current directory */
	return(0);
}

/*
/*	goTarScreen : display the "Tar" screen
/*
*/
goTarScreen (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	i, code, ftype, status, numoffiles;
	FIELD	*fep;
	char	fieldstr[60], file1[65], filelist[65];

	numoffiles = 'p';

	feWrite(&feInTar[0], "P");
	feWrite(&feInTar[1], file);
	feWrite(&feInTar[2], tapefl);
	feSwitchScreen(TarScreen);
	fep = &feInTar[0];

	while (1) {
		code = feRead(fep, fieldstr, 0);
		
		if (hasErr) hasErr = feError(NULL);

		switch(code) {
		case FK10:
			goQdirScreen(TarScreen, fieldstr);
			break;
		case FK11:
			goQfileScreen(TarScreen, fieldstr);
			break;
		case FK12:
		case FK16:
			feSwitchScreen(returnScreen);
			return;
		}

		switch (fep->ID) {
		case 1:
			convcaps(fieldstr);
			switch (fieldstr[0]) {
				case 'a':
				case 'p':
				case 's':
				case 'l':
					numoffiles = fieldstr[0];
					break;
				default :
					hasErr = feError("Illegal response");
					continue;
			}
			break;
		case 2:
			strcpy(file, fieldstr);
			break;
		case 3:
			switch (checkFile(fieldstr)) {
			case 0:
			case S_IFREG:
				strcpy(tapefl, fieldstr);
				break;
			default:
				hasErr = feError("This file cannot be used to hold a file list");
				break;
			}
			break;
		}

		switch(code) {
		case TAB:
		case DOWN:
		case ENTER:
			fep = feSeek(2);
			break;
		case UP:
			fep = feSeek(3);
			break;
		case FK2:
		case FK3:
		case FK4:
			/* Execute the TAR command */

			switch (numoffiles) {
			case 'a':
			case 'p':
			case 's':
				status = gotar(code, numoffiles, file);
				break;
			case 'l':
				status = gotar(code, numoffiles, tapefl);
				break;
			}
			if (status != 0) { /* if tar failed, stay on the same screen */
				feRefresh();
				break;
			}
			feError(NULL);

			/* backup succeeded, or canceled : return to the previous screen */
			feSwitchScreen(returnScreen);
			return;
			break;
		case FK1:
			/* Edit the file list */
			gocr(TarScreen, tapefl);
		default:
			break;
		}
	}
}

/*
/*	gols : Executes "ls" and wait
/*
*/
gols (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	status;
	char	command[80] = {"/bin/ls -l "};

	feClearScreen();

	strcat(command, file);
	strcat(command, " | more");
	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*	
/*	goTaskScreen : list the processes and wait
/*
*/
goTaskScreen (returnScreen)
int	returnScreen;
{
	int	i;
	int 	n;			/* nb of processes displayed*/
	int 	nb;			/* nb of processes */
	int 	numfile;		/* nb processes fields yet displayed */
	int 	code;			/* key pushed by user		*/
	char	com[80] = {"/bin/ps -a| /bin/wc -l"};
	char	command[80] = {"/bin/ps -a"};
	char	fieldstr[6], linestr[200];
	char	fileName[10][20];	/* array to memorize process names */
	char	nbfiles[10];		/* nb of processes	*/	
	FILE	*fpIn,*fpother; 	/* pipe file pointer */
	FIELD	*fep;
	char	process[5];		/* process to kill	*/

	feClearScreen();
	feSwitchScreen(TaskScreen);

	while(1)
	{
		fep=feSeek(0);		/* initialize process fields	*/
		for(i=0;i<10;i++)
		{
			feWrite(&feOutTask[11+2*i], "");
		}

		numfile=0;
		n=0;
					/* reads nb of files in the directory */
		fpother = popen(com,"r");
		fgets(nbfiles,200,fpother);
		nb=atoi(nbfiles);
	
		pclose(fpother);	
	
		fpIn = popen(command, "r"); /* execute "ps-a" and open a pipe */
		fgets(linestr,200,fpIn);	/* discard first line of ps */

		if(fpIn == NULL)
		{
			perror("can't open a pipe\n");
			exit();
		}
		
		do			/* loop to see all processes with TAB */
		{
			feWrite(&feInTask[0], "");/* empty the response field */
			fep = feSeek(0);

			for(i=0;i<10;i++)	/* fill the screen */	
			{
				numfile++;
				if(numfile<nb-2)	
				{
					n++;
					fgets(linestr,200,fpIn);
					linestr[strlen(linestr)-1]='\0';
					strncpy(fileName[i],linestr,75);
					feWrite(&feOutTask[11+2*i], fileName[i]);
				}	
				else  
				{
					feWrite(&feOutTask[11+2*i], "");
					strcpy(fileName[i],"");
				} 
			}

			code = feRead(fep, fieldstr, 0);
		}
		while(  (code == TAB) && (numfile<nb-2) ) ;

		pclose(fpIn);
	
		if ((code == FK12) || (code == FK16)) 
		{
			feSwitchScreen(returnScreen);
			return(0);
		}

				/* make the user choose a selection nb	*/
		if( (atoi(fieldstr)>0) && 
		( (((n%10)!=0)&&(atoi(fieldstr)<=(n%10))) || 
		(((n%10)==0)&&( atoi(fieldstr)<=10)) ) )
{	
		if (hasErr) 
			hasErr = feError(NULL);

		strncpy(process,fileName[atoi(fieldstr)-1],3);
		process[4]='\0';
		switch (code) 
		{
			case FK1:
				feClearScreen();
				gokill(process);
				feClearScreen();
				feSwitchScreen(TaskScreen);		       	
				break;
		}
}
	}
}

/*	
/*	goSpoolScreen : list the files spooled and wait
/*
*/
goSpoolScreen (returnScreen)
int	returnScreen;
{
	int	i;
	int 	n;			/* nb of files yet displayed */
	int 	nb;			/* nb of files in the directory */
	int 	numfile;		/* nb file fields yet displayed	*/
	int 	code;			/* key pushed by user		*/
	/*char	com[80] = {"/bin/ls| /bin/wc -l"};
	*/char	com[80] = {"/bin/ls /ibx/ibxprt | /bin/wc -l"};
	char	command[80] = {"/bin/ls /ibx/ibxprt"};
	/*char	command[80] = {"/bin/ls"};
	*/char	fieldstr[6], linestr[200];
	char	fileName[10][20];	/* array to memorize file names */
	char	nbfiles[10];		/* nb of files in the directory	*/	
	char	directory[200];
	FILE	*fpIn,*fpother; 	/* pipe file pointer */
	FIELD	*fep;

	feClearScreen();
	feSwitchScreen(SpoolScreen);

	while(1)
	{
		n=0;
		numfile=0;
					/* reads nb of files in the directory */
		fpother = popen(com,"r");
		fgets(nbfiles,200,fpother);
		nb=atoi(nbfiles);
		pclose(fpother);	
	
		fpIn = popen(command, "r"); /* execute "ls" and open a pipe */
		
		do			/* loop to see all files with TAB */
		{
			feWrite(&feInSpool[0], "");/* empty the response field */
			fep = feSeek(0);

			for(i=0;i<10;i++)	/* fill the screen */	
			{
				numfile++;
				if(numfile<nb+1)	
				{
					n++;
					fgets(linestr,200,fpIn);
					linestr[strlen(linestr)-1]='\0';
					strcpy(fileName[i],linestr);
					feWrite(&feOutSpool[10+2*i], linestr);
				}	
				else  
				{
					feWrite(&feOutSpool[10+2*i], "");
					strcpy(fileName[i],"");
				} 
			}

			code = feRead(fep, fieldstr, 0);
		}
		while(  (code == TAB) && (numfile<nb+1) ) ;

		pclose(fpIn);
	
		if ((code == FK12) || (code == FK16)) 
		{
			feSwitchScreen(returnScreen);
			return(0);
		}

			/* make the user choose a select nb	*/
		if( (atoi(fieldstr)>0) && 
		( (((n%10)!=0)&&(atoi(fieldstr)<=(n%10))) || 
		(((n%10)==0)&&( atoi(fieldstr)<=10)) ) )
{	
		if (hasErr) 
			hasErr = feError(NULL);

		strcpy(directory,"/ibx/ibxprt/");
		/*strcpy(directory,"/usr/rchartie/menu/");
		*/strcat(directory,fileName[atoi(fieldstr)-1]);

		switch (code) 
		{
			case FK1:
				golpr(SpoolScreen,directory);
				break;
			case FK2:
				gocat(SpoolScreen,directory);
				break;
			case FK3:
				goDelScreen(SpoolScreen, fileName[atoi(fieldstr)-1]);
				break;
		}
}
	}
}

/*
/*	gopasswd : change password for the TSC account
/*
*/
gopasswd (returnScreen)
int returnScreen;
{
	int	status;
	char	command[80] = {"/bin/passwd"};

	feClearScreen();

	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	godf : Executes "df" and wait
/*
*/
godf (returnScreen)
int	returnScreen;
{
	int	status;
	char	command[80] = {"/bin/df"};

	feClearScreen();

	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	gologrpt : Executes "logrpt" and wait
/*
*/
gologrpt (fileName)
char	*fileName;
{
	int	status;
	char	command[80] = {"/bin/logrpt"};

	feGoto(22,1);
	if (fileName) {
		strcat(command, " > ");
		strcat(command, fileName);
		}
	else
	{
		feClearScreen();
		strcat(command, " |more");
	}
	callSystem(command);
	return(0);
}

/*
/*	Log report Screen handling
/*
*/
goLogScreen(returnScreen)
int	returnScreen;
{
	int	code, ftype;
	FIELD	*fep;
	char	fileName[65], fieldstr[60] = {"\0"};

	feWrite(&feInFile[0], "def.report");

	feSwitchScreen(LogScreen);
	fep = feSeek(0);

	while (1) {
		code = feRead(fep, fieldstr, 0);

		if ((code == FK12) || (code == FK16)) {
			feSwitchScreen(returnScreen);
			return(0);
		}

		/* build the complete fileName: unless explicitly complete or empty */

		if ((fieldstr[0]) && (fieldstr[0] != '/')) {
			strcpy(fileName, "/usr/tsc/");
			strcat(fileName, fieldstr);
		}
		else strcpy(fileName, fieldstr);

		feError(NULL);

		switch (code) {
		case FK1:
			if (fileName[0] != '\0') {
				gologrpt(fileName);
				gocr(LogScreen, fileName);
			}
			else feError("Please specify a file to save the report");
			break;
		case FK2:
			gologrpt(NULL);
			feSwitchScreen(LogScreen); /* the screen was erased */
			break;
		case FK3:
			if (fileName[0] != '\0') {
				gologrpt(fileName);
			}
			else feError("Please specify a file to save the report");
			break;
		}
	}
}

/*
/*	gops : Executes "ps" and wait
/*
*/
gops (returnScreen)
int	returnScreen;
{
	int	status;
	char	command[80] = {"/bin/ps -a | more"};

	feClearScreen();

	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	goftp : Executes "ftp" to other side and wait
/*
*/
goftp (returnScreen)
int	returnScreen;
{
	int	status, end;
	char	command[80] = {"/bin/ftp "};
	char	hostName[30];

	feClearScreen();

	gethostname(hostName, 29);
	if ((end = strlen(hostName)-1) > 0) {
		if (hostName[end] == 'a') hostName[end] = 'b';
		else {
			if (hostName[end] == 'b') hostName[end] = 'a';
			else printf("Invalid hostname >>%s<<\n", hostName);
		}

		strcat(command, hostName);
		printf("%s\n", command);
		callSystem(command);
	}
	else {
		printf("Error : cannot find my host name!");
		feError("Press any key to continue");
		fePause();	/* wait for any key to be pressed and ignore the key */
		feError(NULL); /* erase the Error/Info line */
	}
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	gopatch : Executes "patch" and wait
/*
*/
gopatch (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	status;
	char	command[80];

	switch (checkFile(file)) {
		case 0:
			hasErr = feError(sys_errlist[errno]);
			return(1); /* file does not exist */
		case 1:
			hasErr = feError("Cannot patch multiple files");
			return(1);
		case OMAGIC:
		case ZMAGIC:
			break;
		case S_IFDIR:
			hasErr = feError("Directory : cannot be patched with 'patch'");
			return(1);
		default:
			hasErr = feError("Not an object file : cannot be patched with 'patch'");
			return(1);
		}

	strcpy (command, "/bin/patch ");
	strcat (command, file);

	feClearScreen();
	printf("%s\n\n",command);

	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	gock : Executes "ck" and wait
/*
*/
gock (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	status;
	char	command[80];

	feClearScreen();

	strcpy(command, "/bin/ck ");
	strcat(command, file);
	if (checkFile(file) == S_IFDIR) strcat(command, "/*");
	printf("%s\n", command);
	strcat(command, " | more");
	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	gofind : Executes "find" and wait
/*
*/
gofind (returnScreen, dir, file)
int	returnScreen;
char	*dir, *file;
{
	int	status;
	char	command[80];

	if ((dir == NULL) || (file == NULL)) {
		feError("Invalid directory or file.");
		return(0);
		}
	if (file[0] == '\0') {
		feError("Please specify a file name.");
		return(0);
		}
	
	strcpy (command, "/bin/find ");
	strcat (command, dir);
	strcat (command, " -name ");
	strcat (command, file);

	feClearScreen();
	callSystem(command);
	feSwitchScreen(returnScreen);
	return(0);
}

/*
/*	goFindScreen : Executes "find" and wait
/*
*/
goFindScreen (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	i, code, ftype, status;
	FIELD	*fep;
	char	fieldstr[60];

	if (finddir[0] == '\0') strcpy(finddir, "/");

	feWrite(&feInFind[0], file);
	feWrite(&feInFind[1], finddir);
	feSwitchScreen(FindScreen);
	fep = feSeek(0);

	while (1) {
		code = feRead(fep, fieldstr, 0);

		if (code == FK10) goQdirScreen(FindScreen, fieldstr);

		if (code == FK11) goQfileScreen(FindScreen, fieldstr);

		if ((code == FK12) || (code == FK16)) {
			feSwitchScreen(returnScreen);
			return;
		}

		switch (fep->ID) {
		case 0:
			strcpy(file, fieldstr);
			feWrite(fep, file);
			break;
		case 1:
			strcpy(finddir, fieldstr);
			feWrite(fep, finddir);
			break;
		}

		switch(code) {
		case TAB:
		case DOWN:
		case ENTER:
			fep = feSeek(2);
			break;
		case UP:
			fep = feSeek(3);
			break;
		case FK1:
			/* Execute the find command */

			status = gofind(FindScreen, finddir, file);
			feError(NULL);
			break;
		default:
			break;
		}
	}
}

/*
/*	gocp : Executes "cp" or "cpcontig" and wait
/*
*/
gocp (contig, file1, file2)
char	contig, *file1, *file2;
{
	int	status;
	char	command[80];

	if (contig == 'y') strcpy (command, "/bin/cpconfig ");
	else strcpy (command, "/bin/cp ");
	strcat (command, file1);
	strcat (command, " ");
	strcat (command, file2);

	feGoto(22,1);
	return(callSystem(command));
}

/*
/*	goCopyScreen : Executes "cp" and wait
/*
*/
goCopyScreen (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	i, code, ftype, status;
	FIELD	*fep;
	char	fieldstr[60];

	if (fromfile[0] == '\0') strcpy(fromfile, file);
	if (tofile[0] == '\0') strcpy(tofile, file);

	feWrite(&feInCopy[0], fromfile);
	feWrite(&feInCopy[1], tofile);
	feWrite(&feInCopy[2], "N");
	feWrite(&feOutCopy[14], curdir);
	feSwitchScreen(CopyScreen);
	fep = feSeek(0);

	while (1) {
		code = feRead(fep, fieldstr, 0);

		if (code == FK10) goQdirScreen(CopyScreen, fieldstr);

		if (code == FK11) goQfileScreen(CopyScreen, fieldstr);

		if ((code == FK12) || (code == FK16)) {
			feSwitchScreen(returnScreen);
			return;
		}

		switch (fep->ID) {
		case 0:
			strcpy(fromfile, fieldstr);
			feWrite(fep, fromfile);
			break;
		case 1:
			strcpy(tofile, fieldstr);
			feWrite(fep, tofile);
			break;
		}

		switch(code) {
		case TAB:
		case DOWN:
		case ENTER:
			fep = feSeek(2);
			break;
		case UP:
			fep = feSeek(3);
			break;
		case FK1:
			/* Execute the copy command */

			convcaps(feInCopy[2].string);
			status = gocp(feInCopy[2].string[0], fromfile, tofile);
			if (status != 0) { /* if copy failed, stay on the same screen */
				feRefresh();
				break;
			}
			feError(NULL);

			/* copy succeeded, or canceled : return to the previous screen */
			feSwitchScreen(returnScreen);
			return;
			break;
		default:
			break;
		}
	}
}

/*
/*	gomv : Executes "mv" and wait
/*
*/
gomv (file1, file2)
char	*file1, *file2;
{
	int	status;
	char	command[80];

	strcpy (command, "/bin/mv ");
	strcat (command, file1);
	strcat (command, " ");
	strcat (command, file2);

	feGoto(22,1);
	return(callSystem(command));
}

/*
/*	goCopyScreen : Executes "mv" and wait
/*
*/
goMoveScreen (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	i, code, ftype, status;
	FIELD	*fep;
	char	fieldstr[60];

	if (fromfile[0] == '\0') strcpy(fromfile, file);
	if (tofile[0] == '\0') strcpy(tofile, file);

	feWrite(&feInMove[0], fromfile);
	feWrite(&feInMove[1], tofile);
	feWrite(&feOutMove[12], curdir);
	feSwitchScreen(MoveScreen);
	fep = feSeek(0);

	while (1) {
		code = feRead(fep, fieldstr, 0);

		if (code == FK10) goQdirScreen(CopyScreen, fieldstr);

		if (code == FK11) goQfileScreen(CopyScreen, fieldstr);

		if ((code == FK12) || (code == FK16)) {
			feSwitchScreen(returnScreen);
			return;
		}

		switch (fep->ID) {
		case 0:
			strcpy(fromfile, fieldstr);
			feWrite(fep, fromfile);
			break;
		case 1:
			strcpy(tofile, fieldstr);
			feWrite(fep, tofile);
			break;
		}

		switch(code) {
		case TAB:
		case DOWN:
		case ENTER:
			fep = feSeek(2);
			break;
		case UP:
			fep = feSeek(3);
			break;
		case FK1:
			/* Execute the move command */

			status = gomv(fromfile, tofile);
			if (status != 0) { /* if move failed, stay on the same screen */
				feRefresh();
				break;
			}
			feError(NULL);

			/* copy succeeded, or canceled : return to the previous screen */
			feSwitchScreen(returnScreen);
			return;
			break;
		default:
			break;
		}
	}
}

/*
/*	gorm : Executes "rm" and wait
/*
*/
gorm (file)
char	*file;
{
	int	status;
	char	command[80];

	strcpy (command, "/bin/rm ");
	strcat (command, file);

	feGoto(22,1);
	return(callSystem(command));
}

/*
/*	gokill : Executes "kill" and wait
/*
*/
gokill(process)
char	*process;
{
	int	status;
	char	command[80];

	strcpy (command, "/bin/kill -9 ");
	strcat (command, process);
	feGoto(22,1);
	return(callSystem(command));
}

/*
/*	goDelScreen : Manages Delete screen
/*
*/
goDelScreen (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	i, code, ftype, status;
	FIELD	*fep;
	char	fieldstr[60];

	/* build the comlete file name */

	if (file[0] == '/') strcpy(fieldstr, file);
	else {
		strcpy(fieldstr, curdir);
		strcat(fieldstr, "/");
		strcat(fieldstr, file);
	}
	feWrite(&feOutDel[7], fieldstr);
	feSwitchScreen(DelScreen);
	fep = feSeek(0);

	code = feGetCode();

	if ((code == FK1) || (code == 'y') || (code == 'Y')) gorm(file);

	feSwitchScreen(returnScreen);
	return;
}

/*
/*	goQdirScreen : Select a directory from a limited list or type it in.
/*
/*		if a buffer is provided by the caller, the new directory
/*		name will be returned in this buffer.
*/
goQdirScreen (returnScreen, dir)
int	returnScreen;
char	*dir;
{
	int	i, code, index;
	FIELD	*fep;
	char	fieldstr[60];

	feWrite(&feInQdir[0], ""); /* empty the response field */
	feWrite(&feInQdir[1], curdir);
	feSwitchScreen(QuickDirScreen);
	fep = feSeek(0);

	while (1) {
		if (hasErr == 0) hasErr = feInfo("F1 or ENTER : Select, F12 : Cancel");
		code = feRead(fep, fieldstr, 0);

		if (hasErr) hasErr = feError(NULL);

		switch(code) {
		case FK12:
		case FK16:
			feSwitchScreen(returnScreen);
			return;
			break;
		case TAB:
		case DOWN:
			fep = feSeek(2);
			break;
		case UP:
			fep = feSeek(3);
			break;
		case ENTER:
		case FK1:
			switch(fep->ID) {
			case 0:			/* get the directory selected in the quick list */
				index = atoi(fieldstr);
				for (i=0; i<numOutQdir && (feOutQdir[i].ID != index); i++);
				if ((index>0) && (i<numOutQdir )) strcpy (fieldstr, &(feOutQdir[i].string[4]));
				break;

			case 1:			/* new typed-in path name */
				break;
			}
			if (chdir(fieldstr)) { /* could not set the new default directory */
				hasErr = feError(sys_errlist[errno]);
				continue; /* loop back to get a better directory path */
			}

			strcpy(curdir, fieldstr);
			if (dir) strcpy(dir, fieldstr); /* copy in dir if a buffer was provided */
			feSwitchScreen(returnScreen);
			return;
			break;
		default:
			break;
		}
	}
}

/*
/*	goQfileScreen : Get a file from a limited list
/*
*/
goQfileScreen (returnScreen, file)
int	returnScreen;
char	*file;
{
	int	i, code, index;
	FIELD	*fep;
	char	fieldstr[6];

	feWrite(&feInQfile[0], ""); /* empty the response field */
	feSwitchScreen(QuickFileScreen);
	fep = feSeek(0);

	while (1) {
		if (hasErr == 0) hasErr = feInfo("F1 or ENTER : Select, F12 : Cancel");
		code = feRead(fep, fieldstr, 0);

		if (hasErr) hasErr = feError(NULL);

		switch(code) {
		case FK12:
		case FK16:
			feSwitchScreen(returnScreen);
			return;
			break;
		case ENTER:
		case FK1:
			/* get the file selected */
			switch  (atoi(fieldstr)) {
			case 1:
				i = readlink("/default.os", file, 60);
				file[i] = '\0';
				break;
			case 2:
				strcpy (file, "/etc/hosts");
				break;
			case 3:
				strcpy (file, tapefl);
				break;
			case 4:
				strcpy (file, "/ibx/cfg/tskcfg");
				break;
			case 5:
				strcpy (file, "/ibx/cfg/pflcfg");
				break;
			case 6:
				strcpy (file, "/ibx/cfg/iodcfg");
				break;
			case 7:
				strcpy (file, "/ibx/cfg/siteid");
				break;
			case 8:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("ab32"));
				break;
			case 9:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("ar32"));
				break;
			case 10:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("cm32"));
				break;
			case 11:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("cp32"));
				break;
			case 12:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("fc32"));
				break;
			case 13:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("mm32"));
				break;
			case 14:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("ov32"));
				break;
			case 15:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("rt32"));
				break;
			case 16:
				strcpy (file, "/ibx/tsk/");
				strcat (file, findfile("ut32"));
				break;
			}

			feSwitchScreen(returnScreen);
			return;
			break;
		default:
			break;
		}
	}
}

/*
/*	System Screen handling
/*
*/
goSysScreen(returnScreen)
int	returnScreen;
{
	int	code, ftype;
	FIELD	*fep;

	feSwitchScreen(SysScreen);

	while (1) 
	{	feGoto(22,1);
		code = feGetCode();

		if ((code == FK12) || (code == FK16))
		{ 
			feSwitchScreen(returnScreen);
			return(0);
		}

		if (hasErr) hasErr = feError(NULL);

		switch (code) 
		{
		case FK1:
			godf(SysScreen);
		break;
		case FK2:
			goLogScreen(SysScreen);
			break;
		case FK3:
			/*gops(SysScreen);*/
			goTaskScreen(SysScreen);
			break;
		case FK4:
			goSpoolScreen(SysScreen);
			break;
		}
	}
}

/*
/*	List Screen handling
/*
*/
goListScreen(returnScreen,fileName)
int	returnScreen;
char	*fileName;
{
	int	code;
	FIELD	*fep;
	char command[200] = {"/bin/ls | /bin/lpr "};
	feSwitchScreen(ListScreen);

	while (1) 
	{	feGoto(22,1);
		code = feGetCode();

		if ((code == FK12) || (code == FK16))
		{ 
			feSwitchScreen(returnScreen);
			return(0);
		}

		if (hasErr) hasErr = feError(NULL);

		switch (code) 
		{
		case FK1:
			gols(ListScreen,fileName);
		break;
		case FK2:
			strcat(command,fileName);	
			feClearScreen();
			callSystem(command);	
			feSwitchScreen(ListScreen);
		break;
		}
	}
}

/*
/*	File Screen handling
/*
*/
goFileScreen(returnScreen, fileName)
int	returnScreen;
char	*fileName;
{
	int	code, ftype;
	FIELD	*fep;
	char	fieldstr[60];

	feWrite(&feInFile[0], fileName);

	feSwitchScreen(FileScreen);
	fep = feSeek(0);

	while (1) {
		feWrite(&feOutFile[18], curdir);
		code = feRead(fep, fieldstr, 0);

		if (code == FK10) goQdirScreen(FileScreen, fieldstr);

		if (code == FK11) goQfileScreen(FileScreen, fieldstr);

		if ((code == FK12) || (code == FK16)) {
			feSwitchScreen(returnScreen);
			return(0);
		}

		if (hasErr) hasErr = feError(NULL);

		strcpy(fileName, fieldstr);
		feWrite(fep, fieldstr);  /* in case it changed!*/

		switch (code) {
		case FK1:
			gocr(FileScreen, fileName);
			break;
		case FK2:
			gocat(FileScreen, fileName);
			break;
		case FK3:
			goDelScreen(FileScreen, fileName);
			break;
		case FK4:
			goListScreen(FileScreen, fileName);
			break;
		case FK5:
			goFindScreen(FileScreen, fileName);
			break;
		case FK6:
			goCopyScreen(FileScreen, fileName);
			break;
		case FK7:
			goMoveScreen(FileScreen, fileName);
			break;
		case FK8:
			goftp(FileScreen);
			break;
		case FK9:
			gock(FileScreen, fileName);
			break;
			}
		}
	}


/*
/*	Main Screen handling
/*
*/
mainScreen()
{
	int	code, ftype;
	FIELD	*fep;
	char	fileName[60], fieldstr[60];

	feWrite(&feInMain[0], curdir);

	feSwitchScreen(MainScreen);
	fep = feSeek(0);

	while (1) {
		feWrite(&feOutMain[19], curdir);
		code = feRead(fep, fieldstr, 0);

		if (code == FK10) goQdirScreen(MainScreen, fieldstr);

		if (code == FK11) goQfileScreen(MainScreen, fieldstr);

		if ((code == FK12) || (code == FK16)) {
			return(0);
		 }

		if (hasErr) hasErr = feError(NULL);

		strcpy(fileName, fieldstr);
		feWrite(fep, fieldstr);  /* in case it changed!*/

		switch (code) {
		case FK1:
			gocr(MainScreen, fileName);
			break;
		case FK2:
			gocat(MainScreen, fileName);
			break;
		case FK3:
			goTarScreen(MainScreen, fileName);
			break;
		case FK4:
			goListScreen(MainScreen, fileName);
			break;
		case FK5:
			goFindScreen(MainScreen, fileName);
			break;
		case FK6:
			gopatch(MainScreen, fileName);
			break;
		case FK7:
			goSysScreen(MainScreen, fileName);
			break;
		case FK8:
			goFileScreen(MainScreen, fileName);
			strcpy(fieldstr, fileName); /* is case it changed ! */
			break;
		case FK9:
			gock(MainScreen, fileName);
			break;
			}
		}
	}


main(argc, argv)
int argc;
char **argv;
{
	int	i;

	getwd(curdir);

	feInit(1);
	printf("%s",banner);

	/* Prepare the Main Screen */

	for (i=0; i<numOutMain; i++) {
		feAdd (&feOutMain[i]);
		}
	feAdd (&feInMain[0]);

	/* Prepare the System Screen */

	for (i=0; i<numOutSys; i++) {
		feAdd (&feOutSys[i]);
		}

	/* Prepare the File Screen */

	for (i=0; i<numOutFile; i++) {
		feAdd (&feOutFile[i]);
		}
	feAdd (&feInFile[0]);

	/* Prepare the Copy Screen */

	for (i=0; i<numOutCopy; i++) {
		feAdd (&feOutCopy[i]);
		}
	for (i=0; i<numInCopy; i++) {
		feAdd (&feInCopy[i]);
		}

	/* Prepare the Move Screen */

	for (i=0; i<numOutMove; i++) {
		feAdd (&feOutMove[i]);
		}
	for (i=0; i<numInMove; i++) {
		feAdd (&feInMove[i]);
		}

	/* Prepare the Delete Screen */

	for (i=0; i<numOutDel; i++) {
		feAdd (&feOutDel[i]);
		}

	/* Prepare the Find Screen */

	for (i=0; i<numOutFind; i++) {
		feAdd (&feOutFind[i]);
		}
	for (i=0; i<numInFind; i++) {
		feAdd (&feInFind[i]);
		}

	/* Prepare the Quick Directory Screen */

	for (i=0; i<numOutQdir; i++) {
		feAdd (&feOutQdir[i]);
		}
	for (i=0; i<numInQdir; i++) {
		feAdd (&feInQdir[i]);
		}

	/* Prepare the Quick File Screen */

	for (i=0; i<numOutQfile; i++) {
		feAdd (&feOutQfile[i]);
		}
	feAdd (&feInQfile[0]);

	/* Prepare the Tar Screen */

	for (i=0; i<numOutTar; i++) {
		feAdd (&feOutTar[i]);
		}
	for (i=0; i<numInTar; i++) {
		feAdd (&feInTar[i]);
		}

	/* Prepare the Task Screen */

	for (i=0; i<numOutTask; i++)
	{	
		feAdd(&feOutTask[i]);
	}
	feAdd(&feInTask[0]);

	/* Prepare the Spool Screen */

	for (i=0; i<numOutSpool; i++)
	{	
		feAdd(&feOutSpool[i]);
	}
	feAdd(&feInSpool[0]);

	/* Prepare the Log report Screen */
	for (i=0; i<numOutLog; i++)
	{
		feAdd (&feOutLog[i]);
	}
	feAdd (&feInLog[0]);

	/* Prepare the List Screen */
	for (i=0; i<numOutList; i++)
	{
		feAdd (&feOutList[i]);
	}
	feAdd (&feInList[0]);
 	
	
	mainScreen();
	feExit();
	exit(0);
}

