/*===========================================================================*
 *                                                                           *
 *  prelude.h   Standard include file for ETK C routines                     *
 *                                                                           *
 *  Written:    93/03/29  Pieter Hintjens                                    *
 *  Revised:    94/03/09                                                     *
 *                                                                           *
 *  Standard:   ANSI C with VAX C extensions                                 *
 *              Permits C test compilation under MS-DOS.                     *
 *                                                                           *
 *================================= Copyright (c) 1993 Sema Group Belgium. ==*/

#ifndef _PRELUDE_INCLUDED               /*  Allow multiple inclusions        */
#define _PRELUDE_INCLUDED


/*- Generally-useful include files ------------------------------------------*/

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>


/*- Data types --------------------------------------------------------------*/

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;
typedef char           Bool;
typedef void (*function) (void);        /*  Address of simple function       */
#define local static void               /*  Shorthand for local functions    */


/*- Check compiler data type sizes ------------------------------------------*/

#if (UCHAR_MAX != 0xFF)
    #error "Cannot compile: must change definition of 'byte'."
#endif
#if (USHRT_MAX != 0xFFFFU)
    #error "Cannot compile: must change definition of 'word'."
#endif
#if (ULONG_MAX != 0xFFFFFFFFUL)
    #error "Cannot compile: must change definition of 'long'."
#endif


/*- Pseudo-functions --------------------------------------------------------*/

#define ever            (;;)                /*  for ever { ... }             */
#define until(expr)     while (!(expr))     /*  do { ... } until (expr)      */
#define error(expr)     ((expr) < 0)        /*  if error (expr) { ...        */
#define loop(var,n)     for (var = 0; var < n; var++)

#define strlast(str)    (str [strlen (str) - 1])
#define strterm(str)    (str [strlen (str)])
#define streq(s1, s2)   (!strcmp ((s1), (s2)))
#define strneq(s1, s2)  (strcmp ((s1), (s2)))
#define strused(s)      (*(s) != NULL)
#define strnull(s)      (*(s) == NULL)
#define strclr(s)       (*(s) = NULL)

#define bit_msk(bit)    (1 << (bit))
#define bit_set(x, bit) ((x) |=  bit_msk (bit))
#define bit_clr(x, bit) ((x) &= ~bit_msk (bit))
#define bit_tst(x, bit) ((x) &   bit_msk (bit))

#define tblsize(x)      (sizeof (x) / sizeof ((x) [0]))
#define tbllast(x)      (x [tblsize (x) - 1]);

#ifdef random
#    undef random
#    undef randomize
#endif
#ifdef min
#    undef min
#    undef max
#endif

#define random(num)     (int) ((long) rand () % (num))
#define randomize()     srand ((unsigned) time (NULL))
#define min(a,b)        (((a) < (b))? (a): (b))
#define max(a,b)        (((a) > (b))? (a): (b))


/*- Boolean operators and constants -----------------------------------------*/

#define TRUE           (1==1)
#define FALSE          (1==0)
#define AND            &&
#define OR             ||


/*- Symbolic constants ------------------------------------------------------*/

#define FORK_ERROR      -1
#define FORK_CHILD      0
#ifndef LINE_MAX
#define LINE_MAX        255
#endif


/*- System-specific definitions ---------------------------------------------*/

#ifdef __MSDOS__
#   define RET_OKAY     0               /*  Return codes for subroutines     */
#   define RET_ERROR    1
#   include "aetkvlib.h"                /*  Include VAX library headers      */
#endif


#ifdef VAX
#   define RET_OKAY     1
#   define RET_ERROR    0
#   include <descrip.h>
#   include <devdef.h>
#   include <dvidef.h>
#   include <iodef.h>
#   include <jpidef.h>
#   include <latdef.h>
#   include <lib$routines.h>            /*  These include files define the   */
#   include <libdef.h>                  /*    routines and condition codes   */
#   include <libvmdef.h>                /*    used to call the library and   */
#   include <lnmdef.h>
#   include <opcdef.h>
#   include <ppl$def.h>                 /*    functions.                     */
#   include <ppl$routines.h>            /*    and parallel-processing        */
#   include <ppldef.h>
#   include <ssdef.h>
#   include <starlet.h>
#   include <syidef.h>
#   include <trmdef.h>
#   include <tt2def.h>
#   include <ttdef.h>
#   include "aetkvlib.h"                /*  Include VAX library names        */
#endif


#endif                                  /*  Include PRELUDE.H                */
