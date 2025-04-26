/*===========================================================================*
 *									     *
 *  prelude.h	Standard include file for C functions			     *
 *									     *
 *  Written:	93/03/29  Pieter Hintjens				     *
 *  Revised:	95/12/07						     *
 *									     *
 *  Author:	Pieter A. Hintjens					     *
 *		Pijlstraat 9						     *
 *		2060 Antwerpen, Belgium 				     *
 *		ph@mymail.com   					     *
 *		(+323) 231.5277 					     *
 *									     *
 *  FSM Code Generator.  Copyright (c) 1991-95 Pieter A. Hintjens.	     *
 *									     *
 *  This program is free software; you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation; either version 2 of the License, or	     *
 *  (at your option) any later version. 				     *
 *									     *
 *  This program is distributed in the hope that it will be useful,	     *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of	     *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	     *
 *  GNU General Public License for more details.			     *
 *									     *
 *  You should have received a copy of the GNU General Public License	     *
 *  along with this program; if not, write to the Free Software 	     *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.		     *
 *===========================================================================*/

#ifndef _PRELUDE_INCLUDED		/*  Allow multiple inclusions	     */
#define _PRELUDE_INCLUDED


/*- Generally-useful include files ------------------------------------------*/

#if (defined (WIN32) || defined (WINDOWS))
#   include <windows.h>
#   include <dos.h>
#   include <io.h>
#   include <fcntl.h>
#   include <malloc.h>
#   if defined (WIN32)
#       define sleep  Sleep             /*  'sleep' is more portable         */
#   endif
#endif
#if (defined (__MSDOS__))
#   include <dos.h>
#   include <io.h>
#   include <fcntl.h>
#   include <alloc.h>
#endif

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
typedef long int       lint;
typedef int	       Bool;
typedef void (*function) (void);	/*  Address of simple function	     */
#define local static void		/*  Shorthand for local functions    */


/*- Check compiler data type sizes ------------------------------------------*/

#if (UCHAR_MAX != 0xFF)
#   error "Cannot compile: must change definition of 'byte'."
#endif
#if (USHRT_MAX != 0xFFFFU)
#   error "Cannot compile: must change definition of 'word'."
#endif
#if (ULONG_MAX != 0xFFFFFFFFUL)
#   error "Cannot compile: must change definition of 'long'."
#endif


/*- Pseudo-functions --------------------------------------------------------*/

#define ever		(;;)		    /*	for ever { ... }	     */
#define until(expr)	while (!(expr))     /*	do { ... } until (expr)      */
#define error(expr)	((expr) < 0)	    /*	if error (expr) { ...	     */
#define loop(var,n)	for (var = 0; var < n; var++)

#define strlast(str)	(str [strlen (str) - 1])
#define strterm(str)	(str [strlen (str))
#define streq(s1, s2)	(!strcmp ((s1), (s2)))
#define strneq(s1, s2)	(strcmp ((s1), (s2)))
#define strused(s)	(*(s) != 0)
#define strnull(s)	(*(s) == 0)
#define strclr(s)	(*(s) = 0)

#define bit_msk(bit)	(1 << (bit))
#define bit_set(x, bit) ((x) |=  bit_msk (bit))
#define bit_clr(x, bit) ((x) &= ~bit_msk (bit))
#define bit_tst(x, bit) ((x) &	 bit_msk (bit))

#define tblsize(x)	(sizeof (x) / sizeof ((x) [0]))
#define tbllast(x)	(x [tblsize (x) - 1]);

#ifdef random
#   undef random
#   undef randomize
#endif
#ifdef min
#   undef min
#   undef max
#endif

#define random(num)	(int) ((long) rand () % (num))
#define randomize()	srand ((unsigned) time (NULL))
#define min(a,b)	(((a) < (b))? (a): (b))
#define max(a,b)	(((a) > (b))? (a): (b))


/*- ASSERT and debugging ----------------------------------------------------*/

#if (defined (DEBUG))
    /*	Define _Assert function here locally				     */
    local _Assert (char *File, unsigned Line)
    {
	fflush	(stdout);
        fprintf (stderr, "\nAssertion failed: %s, line %u\n", File, Line);
	fflush	(stderr);
	abort	();
    }
#   define ASSERT(f)		\
	if (f)			\
	    ;			\
	else			\
	    _Assert (__FILE__, __LINE__)
#else
#   define ASSERT(f)
#endif


/*- Boolean operators and constants -----------------------------------------*/

#ifdef TRUE
#   undef TRUE
#   undef FALSE
#endif
#define TRUE	       (1==1)
#define FALSE	       (1==0)
#define AND	       &&
#define OR	       ||


/*- Symbolic constants ------------------------------------------------------*/

#ifndef LINE_MAX
#define LINE_MAX	255
#endif
#define TAB '\t'


/*- System-specific definitions ---------------------------------------------*/

#if (defined (VMS) || defined (__VMS))
#   define RET_OKAY		1	/*  Return codes for main()	     */
#   define RET_ERROR		0
#   define FOPEN_READ_TEXT      "r"     /*  Dec C does not like the "t"      */
#   define FOPEN_WRITE_TEXT     "w"
#   define FOPEN_APPEND_TEXT    "a"
#else
#   define RET_OKAY		0
#   define RET_ERROR		1
#   define FOPEN_READ_TEXT      "rt"    /*  Unix and DOS are happy with      */
#   define FOPEN_WRITE_TEXT     "wt"    /*  "t" in fopen modes               */
#   define FOPEN_APPEND_TEXT    "at"
#endif


#endif					/*  Include PRELUDE.H		     */
