/*  ----------------------------------------------------------------<Prolog>-
    Name:       prelude.h
    Title:      Universal Header File for C programming
    Package:    Standard Function Library (SFL)

    Written:    93/03/29    Pieter Hintjens <ph@imatix.com>
    Revised:    97/02/24    Pieter Hintjens <ph@imatix.com>

    Version:    1.44
    <TABLE>
    1.4_PH      Released with SFL 1.4
    1.41_PH     Define EXIT_SUCCESS, EXIT_FAILURE if not defined.
    1.42_PH     Define extern char **environ under SunOS.
    1.43_PH     Define TRUE/FALSE with typecase (Bool)
    1.44_PH     That last change was NFG... caused cpp errors
    1.45_PH     Changes for SCO UnixWare
    </TABLE>

    Synopsis:   This header file encapsulates many generally-useful include
                files and defines lots of good stuff.  The intention of this
                header file is to hide the messy #ifdef's that you typically
                need to make real programs compile & run.  To use, specify
                as the first include file in your program.

                The main contributors to this file were:
                <Table>
                PH   Pieter Hintjens    <ph@imatix.com>
                EDM  Ewen McNeill       <ewen@naos.co.nz>
                PA   Pascal Antonnaux   <pascal@imatix.com>
                PS   Peter Seebach      <seebs@solon.com>
                </Table>

    Copyright:  Copyright (c) 1991-1996 iMatix
    License:    This is free software; you can redistribute it and/or modify
                it under the terms of the SFL License Agreement as provided
                in the file LICENSE.TXT.  This software is distributed in
                the hope that it will be useful, but without any warranty.
 ------------------------------------------------------------------</Prolog>-*/

#ifndef _PRELUDE_INCLUDED                                  /*  Allow multiple inclusions        */
#define _PRELUDE_INCLUDED

/*- Establish the compiler and computer system ------------------------------*/
/*
 *  Defines zero or more of these symbols, for use in any non-portable code:
 *
 *  __UNIX__            System is UNIX
 *
 *  __IS_32BIT__        OS/compiler is 32 bits
 *  __IS_64BIT__        OS/compiler is 64 bits
 *
 *  When __UNIX__ is defined, we also define exactly one of these:
 *  __UTYPE_LINUX       Linux
 *
 */

#if (defined (__64BIT__))                                  /*  EDM 96/05/30                     */
#define __IS_64BIT__                                       /*  May have 64-bit OS/compiler      */
#else
#define __IS_32BIT__                                       /*  Else assume 32-bit OS/compiler   */
#endif

/*  Try to define a __UTYPE_xxx symbol...                                    */
/*  unix                SunOS at least                                       */
/*  __unix__            gcc                                                  */
/*  _POSIX_SOURCE is various UNIX systems, maybe also VAX/VMS                */
#if (defined (unix) || defined (__unix__) || defined (_POSIX_SOURCE))
#if (!defined (__VMS__))
#undef __UNIX__
#define __UNIX__
#undef  __IS_32BIT__
#define __IS_64BIT__
#endif
#endif

#if (defined (linux))
#define __UTYPE_LINUX
#define __UNIX__
#elif (defined __UNIX__)
#define __UTYPE_GENERIC
#endif

/*- Standard ANSI include files ---------------------------------------------*/

#ifdef __cplusplus                                         /*  PA 96/05/29                      */
#include <iostream.h>                                      /*  A bit of support for C++         */
#endif

#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <signal.h>
#include <setjmp.h>

/*- System-specific include files -------------------------------------------*/

#if (defined (__UNIX__))
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

/*- Data types --------------------------------------------------------------*/

typedef int Bool;                                          /*  Boolean TRUE/FALSE value         */
typedef unsigned char byte;                                /*  Single unsigned byte = 8 bits    */
typedef unsigned short dbyte;                              /*  Double byte = 16 bits            */
typedef unsigned short word;                               /*  Alternative for double-byte      */
typedef unsigned long dword;                               /*  Double word >= 32 bits           */

#if (defined (__IS_32BIT__))
typedef unsigned long qbyte;                               /*  Quad byte = 32 bits              */
#else
typedef unsigned int qbyte;                                /*  Quad byte = 32 bits              */
#endif

typedef void (
    *function
) (
    void
);                                                         /*  Address of simple function       */

#define local static void                                  /*  Shorthand for local functions    */

typedef struct {                                           /*  Memory descriptor                */
    size_t size;                                           /*    Size of data part              */
    byte *data;                                            /*    Data part follows here         */
} DESCR;

/*- Check compiler data type sizes ------------------------------------------*/

#if (UCHAR_MAX != 0xFF)
#error "Cannot compile: must change definition of 'byte'."
#endif

#if (USHRT_MAX != 0xFFFFU)
#error "Cannot compile: must change definition of 'dbyte'."
#endif

#if (defined (__IS_32BIT__))
#if (ULONG_MAX != 0xFFFFFFFFUL)
#error "Cannot compile: must change definition of 'qbyte'."
#endif
#else
#if (UINT_MAX != 0xFFFFFFFFU)
#error "Cannot compile: must change definition of 'qbyte'."
#endif
#endif

/*- Pseudo-functions --------------------------------------------------------*/

#define FOREVER         for (;;)                           /*  FOREVER { ... }              */
#define until(expr)     while (!(expr))                    /*  do { ... } until (expr)      */

#define streq(s1,s2)    (!strcmp ((s1), (s2)))
#define strneq(s1,s2)   (strcmp ((s1), (s2)))
#define strused(s)      (*(s) != 0)
#define strnull(s)      (*(s) == 0)
#define strclr(s)       (*(s) = 0)
#define strlast(s)      (s [strlen (s) - 1])
#define strterm(s)      (s [strlen (s)])

#define bit_msk(bit)    (1 << (bit))
#define bit_set(x,bit)  ((x) |=  bit_msk (bit))
#define bit_clr(x,bit)  ((x) &= ~bit_msk (bit))
#define bit_tst(x,bit)  ((x) &   bit_msk (bit))

#define tblsize(x)      (sizeof (x) / sizeof ((x) [0]))
#define tbllast(x)      (x [tblsize (x) - 1])

#if (defined (random))
#undef random
#undef randomize
#endif

#if (defined (min))
#undef min
#undef max
#endif

#if (defined (__IS_32BIT__))
#define random(num)     (int) ((long) rand () % (num))
#else
#define random(num)     (int) ((int)  rand () % (num))
#endif

#define randomize()     srand ((unsigned) time (NULL))
#define min(a,b)        (((a) < (b))? (a): (b))
#define max(a,b)        (((a) > (b))? (a): (b))

/*- ASSERT ------------------------------------------------------------------*/

#if (defined (DEBUG))
    /*  Define _Assert function here locally                                 */
local _Assert(
    char *File,
    unsigned Line
)
{
    fflush(stdout);
    fprintf(stderr, "\nAssertion failed: %s, line %u\n", File, Line);
    fflush(stderr);
    abort();
}

#define ASSERT(f) if (f) { ; } else { _Assert (__FILE__, __LINE__) }
#else
#define ASSERT(f)
#endif

/*- Boolean operators and constants -----------------------------------------*/

#if (!defined (TRUE))
#define TRUE        1                                      /*  ANSI standard                    */
#define FALSE       0
#endif

/*- Symbolic constants ------------------------------------------------------*/

#define FORK_ERROR      -1                                 /*  Return codes from fork()         */
#define FORK_CHILD      0

#if (!defined (LINE_MAX))                                  /*  Length of line from text file    */
#define LINE_MAX     255                                   /*    if not previously #define'd    */
#endif

#if (!defined (PATH_MAX))                                  /*  Length of path variable          */
#define PATH_MAX     2048                                  /*    if not previously #define'd    */
#endif                                                     /*  EDM 96/05/28                     */

#if (!defined (EXIT_SUCCESS))                              /*  ANSI, and should be in stdlib.h  */
#define EXIT_SUCCESS 0                                     /*    but not defined on SunOs with  */
#define EXIT_FAILURE 1                                     /*    GCC, sometimes.                */
#endif

/*- System-specific definitions ---------------------------------------------*/

/*  We define the STARTUP macro: this performs any initialisation for the
 *  main function that is system-dependent.  In theory a portable main
 *  program should do this...                                                */

#if (defined (__MAC__))
#define STARTUP         argc = ccommand (&argv);
#else
#define STARTUP
#endif

/*  UNIX defines sleep() in terms of second; Win32 defines Sleep() in
 *  terms of milliseconds.  We want to be able to use sleep() anywhere.      */

#if (defined (__WINDOWS__))
#if (defined (WIN32))
#define sleep(a) Sleep(a*1000)                             /*  UNIX sleep() is seconds          */
#else
#define sleep(a)                                           /*  Do nothing?                      */
#endif
#endif

/*  We define constants for the way the current system formats filenames;
 *  we assume that the system has some type of path concept.                 */

#if (defined (__UNIX__))                                   /*  All UNIXes                       */
#define PATHSEP      ":"
#define PATHEND      '/'
#define PATHFOLD     FALSE
#define NAMEFOLD     FALSE

#else
#error "No definitions for PATH constants"
#endif

/*- Capability definitions --------------------------------------------------*/
/*
 *  Defines zero or more of these symbols, for use in any non-portable
 *  code:
 *
 *  DOES_SOCKETS        We can use (at least some) BSD socket functions
 *  DOES_UID            We can use (at least some) uid access functions
 *  DOES_TIMERS         We can use the BSD setitimer functions
 */

#if (defined (AF_INET))
#define DOES_SOCKETS                                       /*  System supports BSD sockets      */
#else
#undef  DOES_SOCKETS
#define ntohs(x)     (x)                                   /*  Needs to be correct for the      */
#define ntohl(x)     (x)                                   /*    platform as far as possible    */
#define htons(x)     (x)
#define htonl(x)     (x)
#endif

#if (defined (__UNIX__) || defined (__VMS__) || defined (__OS2__))
#define DOES_UID                                           /*  System supports uid functions    */
#else
#undef  DOES_UID
typedef int gid_t;                                         /*  Group id type                    */
typedef int uid_t;                                         /*  User id type                     */
#endif

#if (defined (ITIMER_REAL))
#define DOES_TIMERS                                        /*  Supports BSD setitimer calls     */
#else
#undef  DOES_TIMERS
#if (!defined (SIGALRM))                                   /*  We can use SIGALRM in code       */
#define SIGALRM  1                                         /*    though it may never happen     */
#endif
#endif

#endif                                                     /*  Include PRELUDE.H                */
