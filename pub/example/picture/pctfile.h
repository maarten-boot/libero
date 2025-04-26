/*===========================================================================*
 *                                                                           *
 *  pctfile.h   header file for library file i/o functions                   *
 *                                                                           *
 *  Written:    95/01/21  Pieter Hintjens                                    *
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

#ifndef _PCTFILE_INCLUDED               /*  Allow multiple inclusions        */
#define _PCTFILE_INCLUDED

/*  -------------------------------------------------------------------------
 *  Files are accessed using the most efficient technique available.  When
 *  possible, this is the Unix open/read/write/close model.  Otherwise this
 *  is the ANSI fopen/fread/fwrite/fclose model.
 *
 *  An FSD (file stream descriptor) encapsulates this difference.
 */

#ifdef O_APPEND
#define CALL_OPEN   1
typedef struct {
    int   handle;                       /*  File handle is integer           */
    word  bufsize;                      /*  Size of buffer in bytes          */
    byte *buffer;                       /*  Buffer for i/o                   */
    byte *bufmax;                       /*  Pointer to last char in buffer   */
    byte *bufptr;                       /*  Pointer to next char in buffer   */
    byte  flags;                        /*  Read/write processing flags      */
} FSD;
#else
#define CALL_FOPEN  1
typedef struct {
    FILE *stream;                       /*  File stream is FILE pointer      */
    word  bufsize;                      /*  Size of buffer in bytes          */
    byte *buffer;                       /*  Buffer for i/o                   */
    byte *bufmax;                       /*  Pointer to last char in buffer   */
    byte *bufptr;                       /*  Pointer to next char in buffer   */
    byte  flags;                        /*  Read/write processing flags      */
} FSD;
#endif

#define FILE_TYPE_TABS          1       /*  Expand/compress tabs in text     */
#define FILE_NAME_MAX           128     /*  Maximum size of filename         */
#define FILE_DIR_MAX            64      /*  Max size of directory name       */
#define FILE_BUFF_DEFAULT       8192    /*  Default buffer size              */

#if defined (__MSDOS__)
#   define PATHSEP     ";"              /*  Separates path components        */
#   define PATHEND     '\\'             /*  Delimits directory and filename  */
#   define NAMEFOLD    TRUE             /*  Convert filenames to uppercase?  */
#   define PATHFOLD    TRUE             /*  Convert pathvalue to uppercase?  */
#elif defined (VAX)
#   define PATHSEP     ","
#   define PATHEND     ':'
#   define NAMEFOLD    TRUE
#   define PATHFOLD    TRUE             /*  Convert path to uppercase?       */
#else
#   define PATHSEP     ":"
#   define PATHEND     '/'
#   define NAMEFOLD    FALSE
#   define PATHFOLD    FALSE            /*  Convert path to uppercase?       */
#endif


/*  Function prototypes                                                      */

char   *File_where        (const char mode, const char *path,
                           const char *name, const char *ext);
FSD    *File_open         (const char mode, const char *name, const int flags);
int     File_read_line    (FSD *fsd, char *line);
int     File_read_block   (FSD *fsd, byte *block);
int     File_write_line   (FSD *fsd, char *line);
int     File_write_block  (FSD *fsd, byte *block);
void    File_close        (FSD *fsd);
void    File_buffer       (FSD *fsd, size_t size);

Bool    File_exists       (const char *filename);
int     Default_extension (char *dest, const char *src, const char *ext);
int     Fixed_extension   (char *dest, const char *src, const char *ext);
char   *Strip_extension   (char *name);
char   *Strip_file_path   (char *name);

#endif
