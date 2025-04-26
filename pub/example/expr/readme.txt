-------------------------------------------------------------------------------
Package:    Expression Parser
-------------------------------------------------------------------------------

Written:    22 Oct 1995
Revised:    28 Dec 1996
Version:    1.2

Author:     Pieter A. Hintjens <ph@imatix.com>

Notes:      This package has expression calculation functions in various
            languages: assembler, COBOL, C, Visual Basic, Rexx.

Contents:   File:           Purpose:
            README   TXT    This file
            CALCUL   C      C expression function: calculate()
            CALCUL   H      Prototypes for calcul.c
            CALCULD  L      Dialog file
            PRELUDE  H      Include file for C programs
            CALCPK   COB    COBOL expression package: "CALCPK"
            CALCPKR  CBL    Copy book for calling "CALCPK"
            CALCPKD  L      Dialog file
            EXPRCOM  ASM    Assembler expression evaluator: .COM file
            EXPRCOM  L      Dialog file
            EXPREXE  ASM    Assembler expression evaluator: .EXE file
            EXPREXE  L      Dialog file
            EXPRSUB  ASM    Assembler expression evaluator: C subroutine
            EXPRSUB  L      Dialog file
            EXPRREX         Rexx expression calculator
            EXPRREX  L      Dialog file
            EXPRREX  MOD    Source modules for Rexx version
            VBCALC   BAS    Visual Basic expression function: vbcalc()
            VBCALC   L      Dialog file
            CPP      ZIP    C++ expression calculator
            JAVA     ZIP    JAVA expression calculator

Copyright:  (c) 1991-97 iMatix.

            This program is free software; you can redistribute it and/or
            modify it under the terms of the GNU General Public License as
            published by the Free Software Foundation; either version 2 of
            the License, or (at your option) any later version.

            This program is distributed in the hope that it will be useful,
            but WITHOUT ANY WARRANTY; without even the implied warranty of
            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
            GNU General Public License for more details.

            You should have received a copy of the GNU General Public License
            along with this program; if not, write to the Free Software
            Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


Documentation:

-------------------------------------------------------------------------------
                            C Expression Evaluator
-------------------------------------------------------------------------------

Syntax: int calculate (calc_control *control)

Description:  Calculates  the  double value of an expression.  The
expression can include the operators +, -, /, *, **, (, ), strings
in  quotes,  numbers, and user-defined functions.   Before calling
calculate, you must define a calc_control variable.

The following program demonstrates how to call calculate ():

------------cut-here-8<--------------------------------------------

#include "prelude.h"
#include "calcul.h"

static double function_i (int i);
static double function_l (long l);
static double function_d (double d);
static double function_s (char *s);

void main ()
{
    char *calc_error [] = {
        "No errors.",
        "Invalid or unrecognised token.",
        "Unexpected end of expression.",
        "Left parenthesis is missing.",
        "Right parenthesis is missing.",
        "Quotes missing after string.",
        "Too many operands in expression.",
        "Too many levels of parentheses in expression.",
        "Number is required here.",
        "String between quotes is required here.",
        "Unknown function name."
    };

    static char input [80];

    static calc_fct fctlist [] = {
        { NULL, "FINT",    'i' },
        { NULL, "FLONG",   'l' },
        { NULL, "FDOUBLE", 'd' },
        { NULL, "FSTRING", 's' },
        { NULL, "",       NULL }
    };

    static calc_control control = {
        input,                          /*  Expression to evaluate          */
        0,                              /*  Result of evaluation            */
        0,                              /*  Error position if any           */
        fctlist                         /*  Pointer to list of functions    */
    };
    int feedback;

    fctlist [0].fct.i = function_i;     /*  Set-up addresses of functions   */
    fctlist [1].fct.l = function_l;
    fctlist [2].fct.d = function_d;
    fctlist [3].fct.s = function_s;

    puts ("Functions: FINT, FLONG, FDOUBLE, FSTRING");

    for ever {
        printf ("==> ");
        gets (input);
        if (strlen (input) == 0)
            break;

        feedback = calculate (&control);

        if (feedback) {
            printf ("    %*c\n", control.error_posn + 1, '^');
            puts (calc_error [feedback]);
        } else
            printf ("Result is: %g\n", control.result);
    }
}

static double function_i (int i)
{
    return (i - 1);
}

static double function_l (long l)
{
    return (l - 2);
}

static double function_d (double d)
{
    return (d - 3);
}

static double function_s (char *s)
{
    return (strlen (s));
}

-------------------------------------------------------------------------------
                         Assembler Expression Evaluator
-------------------------------------------------------------------------------

    EXPRCOM - simple command-line expression calculator

    expr [/?] expression

        /?          - show help
        expression  - mathematical expression consisting of numbers,
                      operators (+, -, /, *) and parentheses ( and ).

    EXPRCOM calculates the expression, and displays the result. The
    result and all numbers must fit into a 16-bit word.  Does not
    print negative results correctly.

    The result is also truncated to 8 bits and returned in the MS-DOS
    errorlevel variable.
