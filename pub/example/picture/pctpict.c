/*===========================================================================*
 *                                                                           *
 *  pctpict     Parse COBOL picture string                                   *
 *                                                                           *
 *  Written:    95/02/26    Pieter Hintjens                                  *
 *  Revised:    95/12/07                                                     *
 *                                                                           *
 *  Version:    PCT V0.2    Alpha version 0.2                                *
 *                                                                           *
 *  Author:     Pieter A. Hintjens                                           *
 *              Pijlstraat 9                                                 *
 *              2060 Antwerpen, Belgium                                      *
 *              ph@mymail.com                                                *
 *              (+323) 231.5277                                              *
 *                                                                           *
 *  Portable COBOL Translator.  Copyright (c) 1993-95 Pieter A. Hintjens.    *
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

#include "pctdefn.h"                    /*  Standard include file            */
#include "pctlibr.h"                    /*  Library functions & definitions  */

#include "pctpict.h"                    /*  Own header file                  */
#include "pctpictd.d"                   /*  Include dialog data              */


/*- Function prototypes -----------------------------------------------------*/

static long Get_repeat_count (void);


/*- Global variables used in this source file only --------------------------*/


static int
    Feedback,                           /*  Feedback for calling program     */
    Char_nbr,                           /*  Current offset into picture      */
    Point_pos,                          /*  Position of point, if any        */
    Cleaned_size;                       /*  Size of cleaned-up picture       */

static long
    Cursize;                            /*  Size of current picture piece    */

static char
    Thisch,                             /*  Current character in picture     */
    *Picture,                           /*  Points to picture string         */
    Cleaned [LINE_MAX + 1],             /*  Cleaned-up picture               */
    Message [LINE_MAX + 1];             /*  Error message                    */

static Bool
    Point_found,                        /*  TRUE if we found a point         */
    Sign_found;                         /*  TRUE if we found a sign          */

static PICTURE_CONTROL
    *Control;

/*  -------------------------------------------------------------------------
 *  Pic_analyse
 *
 *  Accepts a control block with input and output parameters as follows:
 *
 *    char *Picture         Null-terminated picture string to parse
 *    char  Comma_char      Comma character to accept
 *    char  Point_char      Decimal point character to accept
 *    char  Dollar_char     Currency sign to accept
 *
 *    long  Size            Size of data part of picture
 *    int   Type            Type of picture:
 *                          PIC_TYPE_UNDEFINED - Undefined picture type
 *                          PIC_TYPE_CHARACTER - Character X
 *                          PIC_TYPE_NUMERIC   - Numeric S 9 V
 *                          PIC_TYPE_ALPHANUM  - Alphanumeric X 9
 *                          PIC_TYPE_EDITED    - Edited 9 Z * $ , . B /
 *    int   Flags           Various bit flags:
 *                          PIC_FLAG_NEG       - Field has '-' sign
 *                          PIC_FLAG_POS       - Field has '+' sign
 *                          PIC_FLAG_FLOAT     - Field has floating edit
 *                          PIC_FLAG_SIGNED    - Field has virtual 'S' sign
 *    int   Decimals        Number of decimal places after point, if any
 *    char *Cleaned         Cleaned-up picture, if edited.  Character and
 *                          numeric pictures can be ignored once the type,
 *                          sign, decimals, and size is known.  For edited
 *                          pictures, the cleaned-up picture simplifies the
 *                          business of insertion.  Note that strings longer
 *                          than 30 characters are compressed as repeats.
 *
 *    char *Message         Error message, if feedback is PIC_ERROR
 *    int   Position        Position in picture of error, starting at zero
 *
 *  The cleaned-up picture and error message point to static areas.  If you
 *  need to store these, you must allocate memory yourself.  When not used,
 *  they contain an empty string (consisting of only a null character).
 *
 *  No check is made for the maximum size of the field; this is left as an
 *  exercise for the calling program.  (Such checks depend on external needs.)
 */

int Pic_analyse (PICTURE_CONTROL *Control_block)
{
    Feedback = 0;                       /*  No errors so far                 */

    Control = Control_block;            /*  Address control block            */
    Picture = Control-> Picture;        /*    and picture to analyse         */

#   include "pctpictd.i"                /*  Include dialog interpreter       */
}


/*************************   INITIALISE THE PROGRAM   ************************/

MODULE Initialise_the_program (void)
{
    Control-> Type     = PIC_TYPE_UNDEFINED;
    Control-> Flags    = 0;             /*  Clear all bit flags              */
    Control-> Size     = 0;
    Control-> Decimals = 0;
    Control-> Cleaned  = Cleaned;       /*  Point to cleaned-up picture      */
    Control-> Message  = Message;
    Control-> Position = 0;

    Point_found  = FALSE;               /*  No decimal point found so far    */
    Sign_found   = FALSE;               /*  No sign found so far             */
    Cleaned_size = 0;                   /*  Cleaned picture is empty         */
    Char_nbr     = 0;                   /*  Point to start of picture        */

    Str_crop (Picture, 0);              /*  Truncate any trailing spaces     */
    strclr   (Message);                 /*  No error message so far          */

    The_next_event = Ok_event;
}


/*************************   GET NEXT PICTURE CHAR   *************************/

MODULE Get_next_picture_char (void)
{
    Thisch = Picture [Char_nbr++];
    switch (Thisch)
      {
        case 'X':
            The_next_event = X_character_event;
            break;

        case '9':
            The_next_event = Nine_numeric_event;
            break;

        case 'S':
            The_next_event = S_virtual_sign_event;
            break;

        case 'V':
            The_next_event = V_virtual_point_event;
            break;

        case 'Z':
            The_next_event = Z_zero_suppress_event;
            break;

        case '*':
            The_next_event = Star_zero_suppress_event;
            break;

        case 'B':
        case '/':
        case '0':
            The_next_event = Edit_character_event;
            break;

        case '+':
        case '-':
            The_next_event = Sign_character_event;
            break;

        case '(':
            The_next_event = Open_parenthesis_event;
            break;

        case 0:
            The_next_event = Finished_event;
            break;

        default:
            /*  Handle variable picture characters                           */
            if (Thisch == Control-> Point_char)
                The_next_event = Point_character_event;
            else
            if (Thisch == Control-> Comma_char)
                The_next_event = Comma_character_event;
            else
            if (Thisch == Control-> Dollar_char)
                The_next_event = Dollar_character_event;
            else
                The_next_event = Error_event;
      }
}


/***********************   COLLECT REPETITION OR RUN   ***********************/

MODULE Collect_repetition_or_run (void)
{
    if (Picture [Char_nbr] == '(')      /*  Collect count '(nnn)'            */
        Cursize = Get_repeat_count ();
    else
        Cursize = 1;                    /*  Count 1 for Thisch               */

    for ever                            /*  Repeat for repeated characters   */
      {
        if (Picture [Char_nbr] == Thisch)
          {
            Char_nbr++;
            if (Picture [Char_nbr] == '(')
                Cursize += Get_repeat_count ();
            else
                Cursize++;              /*  Count 1                          */
          }
        else
            break;
      }
}

/*  -------------------------------------------------------------------------
 *  Gets count within parenthesis
 *
 *  Char_nbr should point to '('.  Halts with Char_nbr pointing to character
 *  after ')', if possible, otherwise first non-digit.  Does not check for
 *  overflow in picture counts.
 */

static long
Get_repeat_count (void)
{
    long
        Count = 0;

    ASSERT (Picture [Char_nbr] == '(');
    Char_nbr++;                         /*  Skip first '('                   */
    while (isdigit (Picture [Char_nbr]))
        Count = Count * 10 + Picture [Char_nbr++] - '0';

    if (Picture [Char_nbr] == ')')
        Char_nbr++;                     /*  Skip following ')'               */
    else
      {
        sprintf (Message, "')' missing after repeat specifier.\n");
        Raise_exception (Error_event);
      }
    return (Count);
}


/*************************   CHECK IF ONE OR MANY   **************************/

MODULE Check_if_one_or_many (void)
{
    if (Cursize == 0)
        Raise_exception (Error_event);
    else
    if (Cursize == 1)
        The_next_event = One_event;
    else
        The_next_event = Many_event;
}


/***********************   COUNT PICTURE COMPONENT   *************************/

MODULE Count_picture_component (void)
{
    Control-> Size += Cursize;          /*  Add to total data item size      */

    if (Cursize == 1)                   /*  Add to cleaned-picture           */
        Cleaned [Cleaned_size++] = Thisch;
    else
    if (Cursize < 30 AND Thisch != 'X')
      {
        int Repeat_count;
        for (Repeat_count = 0; Repeat_count < Cursize; Repeat_count++)
            Cleaned [Cleaned_size++] = Thisch;
      }
    else
      {
        sprintf (Cleaned + Cleaned_size, "%c(%05u)", Thisch, Cursize);
        Cleaned_size += 8;
      }
}


/**************************   HAVE SIGN CHARACTER   **************************/

MODULE Have_sign_character (void)
{
    if (Thisch == '-')
        Control-> Flags |= PIC_FLAG_NEG;
    else
        Control-> Flags |= PIC_FLAG_POS;

    Sign_found = TRUE;
}


/**************************   HAVE DECIMAL POINT   ***************************/

MODULE Have_decimal_point (void)
{
    if (Point_found)
      {
        sprintf (Message, "only one decimal point is allowed in picture\n");
        Raise_exception (Error_event);
      }
    Point_pos = (int) Control-> Size;   /*  Position of point, 0..n          */
    Point_found = TRUE;
}


/***************************   HAVE VIRTUAL SIGN   ***************************/

MODULE Have_virtual_sign (void)
{
    Control-> Flags |= PIC_FLAG_SIGNED;
}


/*************************   HAVE FLOATING INSERT   **************************/

MODULE Have_floating_insert (void)
{
    Control-> Flags |= PIC_FLAG_FLOAT;
}


/*********************   CHECK SUPPRESS ENDS AT POINT   **********************/

MODULE Check_suppress_ends_at_point (void)
{
    if ((Point_found)
    AND (Picture [Char_nbr - 1] != Control-> Point_char))
      {
        sprintf (Message, "decimals cannot be partly zero-suppressed\n");
        Raise_exception (Error_event);
      }
}


/**********************   ALLOW ONE SIGN ONLY AT END   ***********************/

MODULE Allow_one_sign_only_at_end (void)
{
    if (Sign_found)
      {
        sprintf (Message, "sign cannot be specified twice in picture\n");
        Raise_exception (Error_event);
      }
    else
    if (Cursize > 1)
      {
        sprintf (Message, "floating sign can only come at start of picture\n");
        Raise_exception (Error_event);
      }
    else
    if (Picture [Char_nbr])
      {
        sprintf (Message, "sign must come at start or at end of picture\n");
        Raise_exception (Error_event);
      }
}


/*************************   SET CHARACTER PICTURE   *************************/

MODULE Set_character_picture (void)
{
    Control-> Type = PIC_TYPE_CHARACTER;
    Cleaned_size = 0;                   /*  Return empty cleaned picture     */
}


/**************************   SET NUMERIC PICTURE   **************************/

MODULE Set_numeric_picture (void)
{
    Control-> Type = PIC_TYPE_NUMERIC;
    Cleaned_size = 0;                   /*  Return empty cleaned picture     */
}


/***********************   SET ALPHANUMERIC PICTURE   ************************/

MODULE Set_alphanumeric_picture (void)
{
    Control-> Type = PIC_TYPE_ALPHANUM;
}


/**************************   SET EDITED PICTURE   ***************************/

MODULE Set_edited_picture (void)
{
    Control-> Type = PIC_TYPE_EDITED;
}


/*************************   SIGNAL EMPTY PICTURE   **************************/

MODULE Signal_empty_picture (void)
{
    sprintf (Message, "picture must contain at least one data position\n");
    Raise_exception (Error_event);
}


/**********************   SIGNAL INVALID PICTURE CHAR   **********************/

MODULE Signal_invalid_picture_char (void)
{
    ASSERT (isprint (Thisch));
    sprintf (Message, "'%c' not valid in picture at this point\n", Thisch);
    Raise_exception (Error_event);
}


/***********************   SIGNAL INVALID REPETITION   ***********************/

MODULE Signal_invalid_repetition (void)
{
    sprintf (Message, "(...) must follow one of: B X Z 0 9 * / + - %c %c\n",
                       Control-> Comma_char, Control-> Dollar_char);
    Raise_exception (Error_event);
}


/*************************   RETURN ERROR FEEDBACK   *************************/

MODULE Return_error_feedback (void)
{
    Control-> Position = Char_nbr;
    Feedback = 1;
}


/***************************   GET EXTERNAL EVENT   **************************/

MODULE Get_external_event (void)
{
}


/*************************   TERMINATE THE PROGRAM    ************************/

MODULE Terminate_the_program (void)
{
    Cleaned [Cleaned_size] = 0;
    if (Point_found)
        Control-> Decimals = (int) Control-> Size - Point_pos;

    The_next_event = Terminate_event;
}
