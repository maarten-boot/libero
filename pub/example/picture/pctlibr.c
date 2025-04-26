/*===========================================================================*
 *                                                                           *
 *  pctlibr.c   Various general-purpose library functions.                   *
 *                                                                           *
 *  Written:    92/10/28  Pieter Hintjens                                    *
 *  Revised:    95/02/28                                                     *
 *                                                                           *
 *  String functions:                                                        *
 *              Str_dup             - Make duplicate of string               *
 *              Str_skp             - Skip leading spaces in string          *
 *              Str_set             - Set string to single character         *
 *              Str_pad             - Pad string with character to length    *
 *              Str_lwr             - Convert string to lowercase            *
 *              Str_upr             - Convert string to uppercase            *
 *              Str_crop            - Remove trailing spaces from string     *
 *              Str_open            - Insert blank into string               *
 *              Str_close           - Remove one character from string       *
 *              Str_center          - Center string within desired width     *
 *              X_Str_Cat           - Concatenate multiple strings           *
 *              Lex_cmp             - Compare two strings, ignoring case     *
 *              Tab_expand          - Expand tabs in string                  *
 *                                                                           *
 *  Message functions:                                                       *
 *              Io_error            - Print message with io reason           *
 *                                                                           *
 *  Mathematic functions:                                                    *
 *              Calc_hash_value     - Calculate hash for block of memory     *
 *                                                                           *
 *  Conversion functions:                                                    *
 *              Hex_word            - Convert hex string to word             *
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

#include "pctdefn.h"                    /*  Standard include file            */
#include "pctlibr.h"                    /*  Library functions & definitions  */


/*  -------------------------------------------------------------------------
 *  Str_dup
 *
 *  Makes a duplicate of string, obtaining space with a call to malloc().
 *  The allocated space is strlen (string) + 1 bytes long.  The caller is
 *  responsible for freeing the space allocated by Str_dup when it is no
 *  longer needed.  Returns a pointer to the allocated string, which holds
 *  a copy of the parameter string.  Returns NULL if there was insufficient
 *  heap storage available to allocate the string, or if input string was
 *  null, or was empty (containing only a null byte).
 */

char
*Str_dup (const char *string)
{
    char *copy;

    if (string != NULL AND *string)
      {
        copy = malloc (strlen (string) + 1);
        if (copy)
            strcpy (copy, string);
      }
    else
        copy = NULL;

    return (copy);
}


/*  -------------------------------------------------------------------------
 *  Str_skp
 *
 *  Description: skips leading spaces in string, and returns first non-blank
 *  character.  When this is a null, the end of the string was reached.
 */

char
*Str_skp (char *string)
{
    ASSERT (string != NULL);
    while (*string == ' ')
        string++;
    return (string);
}


/*  -------------------------------------------------------------------------
 *  Str_set
 *
 *  Description: sets all characters in string up to but not including the
 *  final null character to ch.  Returns string.
 */

char
*Str_set (char *string, char ch)
{
    char *scan;

    ASSERT (string != NULL);
    scan = string;
    while (*scan)
        *scan++ = ch;
    return (string);
}


/*  -------------------------------------------------------------------------
 *  Str_pad
 *
 *  Description: returns string of length characters, padding with ch or
 *  truncating if necessary.  String must be at least length + 1 long.
 */

char
*Str_pad (char *string, char ch, int length)
{
    int cursize;

    ASSERT (string != NULL);
    cursize = strlen (string);          /*  Get current length of string     */
    while (cursize < length)            /*  Pad until at desired length      */
        string [cursize++] = ch;

    string [cursize++] = '\0';          /*  Add terminating null             */
    return (string);                    /*    and return to caller           */
}


/*  -------------------------------------------------------------------------
 *  Str_lwr
 *
 *  Description: converts all alphabetic characters in string to lowercase,
 *  stopping at the final null character.  Returns string.
 */

char
*Str_lwr (char *string)
{
    char *scan;

    ASSERT (string != NULL);
    scan = string;
    while (*scan)
      {
        *scan = (char) tolower (*scan);
        scan++;
      }
    return (string);
}


/*  -------------------------------------------------------------------------
 *  Str_upr
 *
 *  Description: converts all alphabetic characters in string to uppercase,
 *  stopping at the final null character.  Returns string.
 */

char
*Str_upr (char *string)
{
    char *scan;

    ASSERT (string != NULL);
    scan = string;
    while (*scan)
      {
        *scan = (char) toupper (*scan);
        scan++;
      }
    return (string);
}


/*  -------------------------------------------------------------------------
 *  Str_crop
 *
 *  Description: drops trailing whitespace from string by truncating string
 *  to the last non-whitespace character, or specified minimum size.
 *  Returns string.
 */

char
*Str_crop (char *string, int minimum)
{
    char
        *last,                          /*  Current end of string            */
        *min;                           /*  Stop position                    */

    ASSERT (string != NULL);
    last = string + strlen (string);
    min  = string + minimum + 1;
    while (last > string AND last > min)
      {
        if (!isspace (*(last - 1)))
            break;
        last--;
      }
    *last = 0;
    return (string);
}


/*  -------------------------------------------------------------------------
 *  Str_open
 *
 *  Description: inserts a character at string, and places a blank in the
 *  gap.  If align is TRUE, makes room by reducing the size of the next gap
 *  of 2 or more spaces.  If align is FALSE, extends the size of the string.
 *  Returns string.
 */

char
*Str_open (char *string, Bool align)
{
    char *gap;
    int  length;

    ASSERT (string != NULL);
    length = strlen (string) + 1;       /*  By default, move string + NULL   */
    if (align)                          /*  If align is TRUE, find gap       */
      {
        gap = strstr (string, "  ");
        if (gap)
            length = (int) (gap - string);
      }
    memmove (string + 1, string, length);
    string [0] = ' ';                   /*  Stick a space into string        */
    return (string);
}


/*  -------------------------------------------------------------------------
 *  Str_close
 *
 *  Description: removes the character at string, and shifts the remainder
 *  down by one.  If align is TRUE, only shifts up to the next gap of 2 or
 *  more spaces.  Returns string.
 */

char
*Str_close (char *string, Bool align)
{
    char *gap;
    int  length;

    ASSERT (string != NULL);
    length = strlen (string);           /*  By default, move string + NULL   */
    if (align)                          /*  If align is TRUE, find gap       */
      {
        gap = strstr (string, "  ");
        if (gap AND gap != string)
            length = (int) (gap - string);
      }
    memmove (string, string + 1, length);
    return (string);
}


/*  -------------------------------------------------------------------------
 *  Str_center
 *
 *  Description: adds leading spaces so that the string is centered within
 *  the desired width.  The dest string must be large enough to accomodate
 *  the formatted result.  Does not add spaces at right.  If width is less
 *  than the size of src, does nothing.  Skips leading spaces in src.
 *  Returns dest.
 */

char
*Str_center (char *dest, char *src, int width)
{
    int indent;                         /*  Amount of space to add at left   */

    ASSERT (dest != NULL);
    ASSERT (src != NULL);

    src = Str_skp (src);                /*  Skip leading spaces in src       */
    indent = (width - strlen (src) + 1) / 2;

    if (indent > 0)
      {
        memset (dest, ' ', indent);     /*  Start with left indent           */
        strcpy (dest + indent, src);    /*    and append source string       */
      }
    return (dest);
}


/*  -------------------------------------------------------------------------
 *  X_Str_Cat
 *
 *  Description: concatenates multiple strings into a single result.  Eg.
 *  X_Str_Cat (buffer, "A", "B", NULL) stores "AB" in buffer.  Returns dest.
 *
 *  [DDJ Nov 1992, p. 155]
 */

char
*X_Str_Cat (char *dest, char *src, ...)
{
    char   *feedback = dest;
    va_list va;

    ASSERT (dest != NULL);
    ASSERT (src != NULL);
    va_start (va, src);
    while (src != NULL)
      {
        while (*src)
            *dest++ = *src++;
        src = va_arg (va, char *);
      }
    *dest = 0;
    va_end (va);
    return (feedback);
}


/*  -------------------------------------------------------------------------
 *  Lex_cmp
 *
 *  Description: performs unsigned comparison of two strings without regard
 *  to the case of any letters in the strings.  Returns a value that is
 *
 *   < 0  if string1 is less than string2
 *   == 0 if string1 is equal to string2
 *   > 0  if string1 is greater than string2
 */

int
Lex_cmp (char *string1, char *string2)
{
    int cmp;

    ASSERT (string1 != NULL);
    ASSERT (string2 != NULL);
    do
      {
        cmp = (byte) tolower (*string1) - (byte) tolower(*string2);
      }
    while (*string1++ AND *string2++ AND cmp == 0);
    return (cmp);
}


/*  -------------------------------------------------------------------------
 *  Tab_expand
 *
 *  Description: expands TAB characters in string to every 8 positions.
 *  String must be large enough for expanded value.  Returns string.
 */

char *
Tab_expand (char *string)
{
    char
        *next_tab,                      /*  Tab found in current line        */
        *tab_stop;                      /*  Where text is tabbed-to          */

    next_tab = strchr (string, TAB);
    while (next_tab != NULL)
      {
        tab_stop = ((word) (next_tab - string) / 8 + 1) * 8 + string;
        memmove (tab_stop, next_tab + 1, strlen (next_tab) + 1);
        memset (next_tab, ' ', (word) (tab_stop - next_tab));
        next_tab = strchr (tab_stop, TAB);
      }
    return (string);
}


/*  -------------------------------------------------------------------------
 *  Io_error
 *
 *  Prints specified message on stdout, then prints system error message.
 */

void
Io_error (const char *Format, ...)
{
    char *Message;
    va_list argptr;                     /*  Argument list pointer            */

    va_start (argptr, Format);          /*  Start variable arguments list    */
    vfprintf (stderr, Format, argptr);
    va_end   (argptr);                  /*  End variable arguments list      */

    Message = strerror (errno);         /*  Get system error message and     */
    if (strlast (Message) == '\n')      /*    remove final line if any       */
        strlast (Message) = '\0';
    fprintf (stderr, ": %s.\n", Message);
}


/*  -------------------------------------------------------------------------
 *  Calc_hash_value
 *
 *  Calculates the hash value for a block of memory using the 'hashpjw'
 *  function by P. J. Weinberger; "Compilers - Principles, Techniques, and
 *  Tools", by Aho, Sethi, and Ullman, Addison-Wesley, 1988. Pp. 435-436.
 *
 *  For each byte in block, shift hash up by 4 bits and add-in the byte.
 *  If any of four high bits of hash are 1, shift high bits right by 24
 *  stops and XOR them into hash, then clear the high-bits.
 */

long
Calc_hash_value (const void *Block, int Size, long Prime)
{
    dword
        Hash = 0,
        High_bits;
    byte
        *Next_byte = (byte *) Block;

    ASSERT (Size > 0);
    ASSERT (Block != NULL);
    ASSERT (Prime > 0);

    while (Size-- > 0)
      {
        Hash = (Hash << 4) + *(Next_byte++);
        High_bits = Hash & 0xF0000000L;
        if (High_bits)
          {
            Hash = Hash ^ (High_bits >> 24);
            Hash = Hash ^ High_bits;
          }
      }
    return (Hash % Prime);
}


/*  -------------------------------------------------------------------------
 *  Hex_word
 *
 *  Converts a hex string into an unsigned integer.  Convert the number of
 *  bytes specified, without check for overflow.
 */

word
Hex_word (const char *Hexchars, int Size)
{
    word Value = 0;
    int  Nextch;

    while (Size--)
      {
        Nextch = *Hexchars;
        if (Nextch > 9)
            Nextch = Nextch - 'A' + 10;

        Value = Value * 16 + Nextch;
      }
    return (Value);
}
