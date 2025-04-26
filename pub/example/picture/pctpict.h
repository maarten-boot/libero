/*===========================================================================*
 *                                                                           *
 *  pctpict.h   Header file for picture-parsing function                     *
 *                                                                           *
 *  Written:    95/02/27  Pieter Hintjens                                    *
 *  Revised:    95/03/01                                                     *
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

#ifndef _PCTPICT_INCLUDED               /*  Allow multiple inclusions        */
#define _PCTPICT_INCLUDED

typedef struct {                        /*  Pic_analyse parameter block      */
                                        /*  Provided by caller:              */
    char *Picture;                      /*    Picture to analyse             */
    char  Comma_char;                   /*    Comma character  (eg. ',')     */
    char  Point_char;                   /*    Decimal point    (eg. '.')     */
    char  Dollar_char;                  /*    Currency sign    (eg. '$')     */
                                        /*  Returned by Pic_analyse:         */
    long  Size;                         /*    Size of data in item           */
    int   Type;                         /*    Type of elementary item        */
    int   Flags;                        /*    Various bit flags              */
    int   Decimals;                     /*    Number of decimals             */
    char *Cleaned;                      /*    Cleaned-up picture             */
    char *Message;                      /*    If error: message text         */
    int   Position;                     /*    If error: position of error    */
} PICTURE_CONTROL;
                                        /*  Values for type field:           */
#define PIC_TYPE_UNDEFINED  0           /*    Undefined picture type         */
#define PIC_TYPE_CHARACTER  1           /*    Character X                    */
#define PIC_TYPE_NUMERIC    2           /*    Numeric S 9 V                  */
#define PIC_TYPE_ALPHANUM   3           /*    Alphanumeric X 9               */
#define PIC_TYPE_EDITED     4           /*    Edited 9 Z * $ , . B /         */

                                        /*  Values for flags field:          */
#define PIC_FLAG_NEG        1           /*    Field has '-' sign             */
#define PIC_FLAG_POS        2           /*    Field has '+' sign             */
#define PIC_FLAG_FLOAT      4           /*    Field has floating edit        */
#define PIC_FLAG_SIGNED     8           /*    Field has virtual 'S' sign     */

                                        /*  Values for return feedback:      */
#define PIC_OKAY            0           /*    No errors                      */
#define PIC_ERROR           1           /*    Syntax error                   */


/*  Function prototypes                                                      */

int Pic_analyse (PICTURE_CONTROL *Control);


#endif                                  /*  Include PCTPICT.H                */
