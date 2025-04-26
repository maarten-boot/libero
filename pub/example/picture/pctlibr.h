/*===========================================================================*
 *                                                                           *
 *  pctlibr.h   header file for general-purpose library functions            *
 *                                                                           *
 *  Written:    92/10/25  Pieter Hintjens                                    *
 *  Revised:    95/02/28                                                     *
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

#ifndef _PCTLIBF_INCLUDED               /*  Allow multiple inclusions        */
#define _PCTLIBF_INCLUDED

/*  Function prototypes                                                      */

char   *Str_dup            (const char *string);
char   *Str_skp            (char *string);
char   *Str_set            (char *string, char ch);
char   *Str_pad            (char *string, char ch, int length);
char   *Str_lwr            (char *string);
char   *Str_upr            (char *string);
char   *Str_crop           (char *string, int minumum);
char   *Str_open           (char *string, Bool align);
char   *Str_close          (char *string, Bool align);
char   *Str_center         (char *dest, char *src, int width);
char   *X_Str_Cat          (char *dest, char *src, ...);
char   *Tab_expand         (char *string);
int     Lex_cmp            (char *string1, char *string2);
void    Io_error           (const char *format,...);
long    Calc_hash_value    (const void *block, int size, long prime);
word    Hex_word           (const char *Hexchars, const int Size);

#endif
