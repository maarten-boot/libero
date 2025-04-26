/*===========================================================================*
 *                                                                           *
 *  pctdefn.h   Standard header file for private PCT use                     *
 *              Defines all global variables, plus miscellaneous function    *
 *              prototypes.                                                  *
 *                                                                           *
 *  Written:    92/10/25  Pieter Hintjens                                    *
 *  Revised:    95/03/10                                                     *
 *                                                                           *
 *  Version:    PCT V0.2    Alpha version 0.2                                *
 *                                                                           *
 *  Author:     Pieter A. Hintjens                                           *
 *              Pijlstraat 9                                                 *
 *              2060 Antwerpen, Belgium                                      *
 *              pahint@belgium.eu.net                                        *
 *              (+323) 231.5277                                              *
 *                                                                           *
 *  Part of the PCT package.  Copyright (c) 1993-1994 Pieter A. Hintjens.    *
 *                                                                           *
 *  This program is free software; you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation; either version 2 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program; if not, write to the Free Software              *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *===========================================================================*/

#ifndef _PCTDEFN_INCLUDED               /*  Allow multiple inclusions        */
#define _PCTDEFN_INCLUDED


/*- Version information -----------------------------------------------------*/

#define PCT_VERSION     "0.2"

#ifdef PCT_DEBUG
    #define PCTREL      "Debugging Release"
#else
    #define PCTREL      "Normal Release"
#endif

#define ABOUT_TEXT1  \
    "'pct v" PCT_VERSION "' Portable COBOL Translator - " PCTREL
#define ABOUT_TEXT2  \
    "Copyright (c) 1993 - 1995 Pieter A. Hintjens"
#define ABOUT_TEXT3  \
    "May be freely used and distributed according to the GPL."
#define ABOUT_TEXT      ABOUT_TEXT1 "\n" ABOUT_TEXT2 "\n" ABOUT_TEXT3 "\n"


/*- Generally-useful include files ------------------------------------------*/

#if defined (__MSDOS__)
#   include <dos.h>                     /*  Need _stklen from this file      */
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
typedef char           Bool;
typedef void (*function) (void);        /*  Address of simple function       */
#define local static void               /*  Shorthand for local functions    */


/*- Pseudo-functions --------------------------------------------------------*/

#define ever            (;;)                /*  for ever { ... }             */
#define until(expr)     while (!(expr))     /*  do { ... } until (expr)      */
#define error(expr)     ((expr) < 0)        /*  if error (expr) { ...        */
#define loop(var,n)     for (var = 0; var < n; var++)

#define strlast(str)    (str [strlen (str) - 1])
#define strterm(str)    (str [strlen (str)])
#define streq(s1, s2)   (!strcmp ((s1), (s2)))
#define strneq(s1, s2)  (strcmp ((s1), (s2)))
#define strused(s)      (*(s) != '\0')
#define strnull(s)      (*(s) == '\0')
#define strclr(s)       *(s) = '\0'

#define tblsize(t)      (sizeof (t) / (sizeof ((t) [0])))

#define bit_msk(bit)    (1 << (bit))
#define bit_set(x, bit) ((x) |=  bit_msk (bit))
#define bit_clr(x, bit) ((x) &= ~bit_msk (bit))
#define bit_tst(x, bit) ((x) &   bit_msk (bit))

#undef random
#undef randomize
#undef max
#undef min

#define random(num)     (int) ((long) rand () % (num))
#define randomize()     srand ((unsigned) time (NULL))
#define max(a,b)        (((a) > (b))? (a): (b))
#define min(a,b)        (((a) < (b))? (a): (b))


/*- ASSERT and debugging ----------------------------------------------------*/

#ifdef DEBUG
    /*  Define _Assert function here locally                                 */
    local _Assert (char *File, unsigned Line)
    {
        fflush  (stdout);
        fprintf (stderr, "\nAssertion failed: %s, line %u\n", File, Line);
        fflush  (stderr);
        abort   ();
    }
#   define ASSERT(f)            \
        if (f)                  \
            ;                   \
        else                    \
            _Assert (__FILE__, __LINE__)
#else
#   define ASSERT(f)
#endif


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


/*- Boolean operators and constants -----------------------------------------*/

#ifndef TRUE
#define TRUE           (1==1)
#define FALSE          (1==0)
#endif
#define AND            &&
#define OR             ||


/*- Symbolic constants ------------------------------------------------------*/

#define FORK_ERROR      -1
#define FORK_CHILD      0
#ifndef LINE_MAX
#define LINE_MAX        255
#endif

#define TAB             '\t'
#define QUOTES          '"'
#define APOST           '\''

/*- System-specific definitions ---------------------------------------------*/

#if defined (__MSDOS__)
#   define RET_OKAY     0               /*  Return codes to system           */
#   define RET_ERROR    1
#elif defined (VAX)
#   define RET_OKAY     1               /*  Return codes to system           */
#   define RET_ERROR    0
#else
#   define RET_OKAY     0               /*  Return codes to system           */
#   define RET_ERROR    1
#endif



/*- Limits and Constants ----------------------------------------------------*/

#define MARGIN_A        7               /*  Margin A starts in column 8      */
#define MARGIN_B        11              /*  Margin B starts in column 12     */

#define PCT_PID_MAX     30              /*  Max length of PROGRAM ID         */
#define PCT_PIC_MAX     30              /*  Max length of picture            */
#define PCT_ERROR_MAX   250             /*  Max errors before abort          */
#define PCT_NAME_MAX    30              /*  Max length of COBOL name         */
#define PCT_INDENT_MAX  18              /*  Max IF nesting level (warning)   */
#define PCT_LEVEL_MAX   49              /*  Max data definition level        */
#define PCT_ERRBUF_MAX  1024            /*  Size of error buffer (chars)     */
#define PCT_ERRIND_MAX  40              /*    and max. lines in buffer       */
#define PCT_FNAME_MAX   8               /*  Max size of simple filename      */
#define PCT_NUMBER_MAX  30              /*  Real max size of number          */

                                        /*  Thunk indicators for symbols:    */
#define PCT_THUNK_NUM   1               /*    Number symbol type             */
#define PCT_THUNK_STR   2               /*    String symbol type             */
#define PCT_THUNK_HEX   3               /*    Hex string symbol type         */
#define PCT_THUNK_PIC   4               /*    Picture symbol type            */

#define yyerror         Error           /*  Implemented as a macro           */


/*- Various functions -------------------------------------------------------*/

int  yyparse            (void);         /*  In PCTL.C                        */
void yy_reset           (void);         /*  In PCTL.C                        */
int  input              (void);         /*  In PCTL.C                        */
int  unput              (int);          /*  In PCTL.C                        */


/*- Global variables --------------------------------------------------------*/

extern FILE
    *PCT_output,                        /*  Output file for compiled code    */
    *PCT_list;                          /*  Output file for listing          */

enum Division
{                                       /*  Indicates current division       */
    Identification_division,            /*  Used for semantic checks         */
    Configuration_section,              /*    which depend on division       */
    Working_storage_section,
    Linkage_section,
    Procedure_division
};
extern enum Division
    Cur_division;                       /*  Current division                 */


/*- Command-line Parameter Flags and Values ---------------------------------*/

extern Bool                             /*  See PCT.C for details            */
    Arg_source,
    Arg_verbose,
    Arg_xref,
    Arg_warn,
    Arg_portable,
    Arg_dump;

extern char
   *Arg_copy_ext,
   *Arg_data_path,
   *Arg_list_file,
   *Arg_output_file;

extern int
    Arg_max_errors;

#endif                                  /*  Include PCTDEFN.H                */
