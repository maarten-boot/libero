/*===========================================================================*
 *                                                                           *
 *  pctfile.c   Various general-purpose fileary functions.                   *
 *                                                                           *
 *  Written:    95/01/21  Pieter Hintjens                                    *
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

#include "pctdefn.h"                    /*  Standard header file             */
#include "pctlibr.h"                    /*  Library functions                */
#include "pctfile.h"                    /*  Prototypes for file functions    */

/*  Ensure our buffers will be big enough for dir + name + delimiters        */
#if ((LINE_MAX - FILE_NAME_MAX) < (FILE_DIR_MAX + 10))
    #error "Cannot compile; FILE_NAME_MAX is too large."
#endif

static char *build_next_path (char *dest, char *path, char *name);


/*  -------------------------------------------------------------------------
 *  File_where
 *
 *  Returns a fully-specified filename, by searching along a path and adding
 *  an extension as required.  Input parameters are:
 *
 *  mode:   one of 'r' = read, 'w' = write, 'a' = append, or 's' = static.
 *          This function tries to locate existing files somewhere on the
 *          path.  New files are always created in the current directory.
 *          'static' files are created in the first directory on the path.
 *
 *  path:   only used when mode is 'r', 'a', or 'a'.  If path is NULL or
 *          empty, it is ignored.  Otherwise, path is translated as an
 *          environment variable, and cut into a list of directory names.
 *          The path is cut up as follows:
 *
 *          MS-DOS: directory names separated by ';'.  ;; means current.
 *          Unix:   directory names separated by ':'.  :: means current.
 *          VAX:    directory names separated by ','.  " ", means current.
 *          Other:  single directory name.
 *
 *          When the mode is 'r' or 'a', searches the current directory
 *          before considering the path value.
 *
 *  name:   file name with or without extension.  Will be prefixed by the
 *          path if required, and optionally the extension.
 *
 *  ext:    default or mandatory extension.  If ext starts with a dot '.',
 *          it is always used.  Otherwise the extension is used only if the
 *          name does not already have an extension.  If ext is NULL or "",
 *          it is ignored.
 *
 *  The total length of a name including path, name, extension, and any
 *  delimiters is FILE_NAME_MAX.  Names are truncated if too long.
 *  Maximum size of one directory component is FILE_DIR_MAX chars.
 *
 *  All parameters are case-sensitive; the precise effect of this depends on
 *  the system.  On MS-DOS, filenames are always folded to uppercase, but the
 *  path must be supplied in uppercase correctly.  On Unix, all parameters are
 *  case sensitive.  On VAX/VMS, path and filenames are folded into uppercase.
 *
 *  Returns a pointer to a static character array containing the filename;
 *  if mode is 'r' and the file does not exist, returns NULL.  If the mode is
 *  'w', 'a', or 's', always returns a valid filename.
 */

char *
File_where (const char  mode, const char *path,
           const char *name, const char *ext)
{
    static char
        work_name [LINE_MAX],           /*  name plus ext                    */
        full_name [LINE_MAX];           /*  dir plus name plus ext           */
    char
        *pathptr;                       /*  End of directory in Path         */

    if (ext != NULL AND *ext)           /*  Append extension if not null     */
      {                                 /*    to get name + ext into         */
        if (ext [0] == '.')             /*    work_name.                     */
            Fixed_extension (work_name, name, ext);
        else
            Default_extension (work_name, name, ext);
      }
    else
        strcpy (work_name, name);

    if (NAMEFOLD)
        Str_upr (work_name);

    if (path != NULL AND *path)         /*  Get value of path, or NULL       */
      {
        pathptr = getenv (path);
        if (pathptr)
            if (PATHFOLD)               /*  Fold path value if required      */
                Str_upr (pathptr);
      }
    else
        pathptr = NULL;

    /*  Take care of 'w' and 's' options first                               */
    if (mode == 'w')                    /*  Create output file locally       */
        return (work_name);

    if (mode == 's')                    /*  Get specific directory name      */
      {
        build_next_path (full_name, pathptr, work_name);
        return (full_name);
      }

    if (File_exists (work_name))        /*  Find file in current directory?  */
        return (work_name);             /*  Then return name + ext           */

    /*  VMS lets you open a file like this 'path:name'                       */
#   ifdef VAX
    sprintf (full_name, "%s:%s", path, work_name);
    Str_upr (full_name);
    if ((mode == 'a')                   /*  Create file locally or find it   */
    OR  (File_exists (full_name)))      /*    using path symbol value        */
        return (full_name);
    else
        return (NULL);                  /*  Not found for reading            */
#   else

    if (!pathptr)                       /*  Now we need a path               */
        return (NULL);                  /*   - if none defined, give up      */

    for ever                            /*  Try each path component          */
      {
        pathptr = build_next_path (full_name, pathptr, work_name);
        if (File_exists (full_name))
            return (full_name);         /*  Until we find one,               */

        if (*pathptr == '\0')           /*    or we come to the end of       */
          {                             /*    the path                       */
            if (mode == 'r')
                return (NULL);          /*  Input file was not found...      */
            else
                return (full_name);
          }
      }
#   endif
}

static char *
build_next_path (char *dest, char *path, char *name)
{
    int
        length;                         /*  length of directory name         */

    length = strcspn (path, PATHSEP);
    strncpy (dest, path, length);
    path += length;                     /*  Bump past path delimiter         */
    if (*path)                          /*    unless we are at the end       */
        path++;                         /*    of the path                    */

    if ((length)
    AND (dest [length - 1] != PATHEND))
        dest [length++] = PATHEND;      /*  Add path-to-filename delimiter   */

    dest [length] = '\0';
    strcat (dest, name);
    return (path);
}


/*  -------------------------------------------------------------------------
 *  File_open
 *
 *  Opens the specified file and returns a file stream descriptor block
 *  for it.  Returns NULL if the file is not accessible or not found.
 *  'mode' can be one of: r=read, w=write, a=append, or u=update.
 *  files, or R, W, A, or U for binary files.  The flags can be any mix
 *  of the FILE_TYPE_... flags defined in pctfile.h. Allocates a default
 *  buffer for read/write whose size you can change using File_buffer().
 */

FSD *
File_open (const char mode, const char *name, const int flags)
{
    FSD
        fsd,                            /*  Holds file descriptor            */
        *newfsd = NULL;                 /*    until copied into here         */

#   ifdef CALL_OPEN
    int access;                         /*  File access mode                 */
#   endif

    fsd.buffer  = malloc (FILE_BUFF_DEFAULT);
    fsd.bufsize = FILE_BUFF_DEFAULT;    /*  Allocate new, empty buffer       */
    fsd.bufptr  = fsd.buffer;
    fsd.bufmax  = fsd.buffer;
    fsd.flags   = flags;

#   ifdef CALL_FOPEN
    fsd.stream = fopen (name, mode == 'a'? "a"   :
                              mode == 'r'? "r"   :
                              mode == 'w'? "w"   :
                              mode == 'm'? "r+w" :
                              /*  else  */ "r"   );
    if (fsd.stream != NULL)
      {
        newfsd = malloc (sizeof (FSD));
        if (newfsd)
            *newfsd = fsd;              /*  Copy fsd to allocated fsd        */
      }
#   else
    access = (mode == 'a'? O_APPEND :
              mode == 'r'? O_RDONLY :
              mode == 'w'? O_WRONLY :
              mode == 'm'? O_RDWR   :
              /* else */   O_RDONLY );

#   ifdef O_BINARY
    access |= O_BINARY;                 /*  Keep it in raw mode              */
#   endif
    fsd.handle = open (name, access);
    if (fsd.handle != -1)
      {
        newfsd = malloc (sizeof (FSD));
        if (newfsd)
            *newfsd = fsd;              /*  Copy fsd to allocated fsd        */
      }
#   endif
    return (newfsd);
}


/*  -------------------------------------------------------------------------
 *  File_read_line
 *
 *  Description: reads a line of text delimited by newline from the stream.
 *  The string must be at least LINE_MAX + 1 long.  All control characters
 *  at the end of the string (newline, carriage-returns, or end-of-files)
 *  are removed. If the FILE_TYPE_TAB flag was specified in the FSD, expands
 *  tabs to every 8th position.
 *
 *  Returns the number of characters in the line, or EOF at the end of file.
 */

int
File_read_line (FSD *fsd, char *line)
{
    byte
        *buffer = fsd-> buffer,         /*  Copy stuff from fsd block        */
        *bufptr = fsd-> bufptr,
        *bufmax = fsd-> bufmax,
        *bufeoln;                       /*  End of this line in buffer       */
    word
        ch_left,                        /*  How much of buffer is left       */
        ch_reqd,                        /*  How much we want to read         */
        ch_read,                        /*  How much did we read             */
        line_sz;                        /*  How big is this line             */

    /*  Look for next line in buffer                                         */
    ch_left = (size_t) (bufmax - bufptr);
    bufeoln = memchr (bufptr, '\n', ch_left);

    if (bufeoln == NULL)                /*  Not found; buffer needs filling  */
      {
        ch_reqd = fsd-> bufsize - ch_left;
        memmove (buffer, bufptr, ch_left);

        bufptr = buffer + ch_left;      /*  Point to start of fresh data     */
#       ifdef CALL_FOPEN                /*    and read next block of data    */
        ch_read = fread (bufptr, 1, ch_reqd, fsd-> stream);
#       else
        ch_read = read (fsd-> handle, bufptr, ch_reqd);
#       endif

        if ((ch_read == 0)              /*  No more data to read             */
        OR  (ch_read == -1U))           /*    or read error                  */
          {
            fsd-> bufptr = bufptr;      /*  Reset buffer to ensure that      */
            fsd-> bufmax = bufmax;      /*    any further calls to this      */
            return (EOF);               /*    function do not bomb           */
          }
        if (ch_read < ch_reqd)          /*  Ensure buffer ends in newline    */
            if (bufptr [ch_read - 1] != '\n')
                bufptr [ch_read++] = '\n';

        bufmax = bufptr + ch_read;      /*  Set pointer to end of data       */
        bufptr = buffer;                /*  and to start of data in buffer   */
        bufeoln = memchr (bufptr, '\n', ch_left + ch_read);
        ASSERT (bufeoln != NULL);
      }
    line_sz = (word) (bufeoln - bufptr);
    while (bufptr [line_sz - 1] < ' ')  /*  Drop any trailing control        */
        if (--line_sz == 0)             /*    characters, but stop if        */
            break;                      /*    line becomes empty             */

    memcpy (line, bufptr, line_sz);     /*  Copy line                        */
    line [line_sz] = 0;                 /*    and terminate properly         */

    fsd-> bufptr = bufeoln + 1;         /*  Update FSD pointers              */
    fsd-> bufmax = bufmax;

    if (fsd-> flags & FILE_TYPE_TABS)   /*  Expand tabs?                     */
        line_sz = strlen (Tab_expand (line));

    return (line_sz);
}


int
File_read_block (FSD *fsd, byte *block)
{
}


int
File_write_line (FSD *fsd, char *line)
{
}


int
File_write_block (FSD *fsd, byte *block)
{
}


void
File_close (FSD *fsd)
{
    ASSERT (fsd != NULL);

#   ifdef CALL_FOPEN
    if (fsd-> stream)
        fclose (fsd-> stream);
#   else
    if (fsd-> handle)
        close (fsd-> handle);
#   endif
    if (fsd-> buffer)
        free (fsd-> buffer);

    free (fsd);
}


void
File_buffer (FSD *fsd, size_t size)
{
    ASSERT (fsd != NULL);
    ASSERT (size > 0);

    if (fsd-> buffer)
        free (fsd-> buffer);

    fsd-> buffer  = malloc (size);
    fsd-> bufsize = size;
    fsd-> bufptr  = fsd-> buffer;
    fsd-> bufmax  = fsd-> buffer;
}


/*  -------------------------------------------------------------------------
 *  File_exists
 *
 *  Description: returns TRUE if the file exists, or FALSE if it does not.
 *  The file must both exist, and be available with read access.  If this
 *  function returns FALSE, you should use Io_error to print the reason for
 *  the problem.  The file may exist but be locked or be protected.
 */

Bool
File_exists (const char *filename)
{
    FILE *file;

    ASSERT (filename != NULL);
    file = fopen (filename, "rb");
    fclose (file);
    return (Bool) (file != NULL);
}


/*  -------------------------------------------------------------------------
 *  Default_extension
 *
 *  Description: copies src to dest and adds ext if necessary.
 *
 *  Returns: 0 if the name was okay, -1 if it was invalid.  Does not print
 *  any error messages for invalid names.
 *
 *  Comments: dest must be large enough for fully-formatted filename.  ext
 *  can start with or without a point.  If ext is NULL or empty, exits.
 */

int
Default_extension (char *dest, const char *src, const char *ext)
{
    int len, i;
    char *ptr;

    ASSERT (dest != NULL);
    ASSERT (src != NULL);
    ASSERT (ext != NULL);
    if (dest != src)                    /*  Copy src to dest if not same     */
        strcpy (dest, src);

    if (ext != NULL OR *ext != 0)
      {
        len = strlen (dest);
        for (i = len - 1, ptr = dest + i; i >= 0; i--, ptr--)
            if (*ptr == '.' OR *ptr == PATHEND)
                 break;

        if (i < 0 OR *ptr != '.')
          {
            if (*ext != '.')
              {
                dest [len++] = '.';
                dest [len] = '\0';
              }
            strcat (dest + len, ext);
          }
      }
    return (0);
}


/*  -------------------------------------------------------------------------
 *  Fixed_extension
 *
 *  Description: copies src to dest, removes any existing extension and adds
 *  ext to dest.  Does not change src.
 *
 *  Returns: 0 if the name was okay, -1 if it was invalid.  Does not print
 *  any error messages for invalid names.
 *
 *  Comments: dest must be large enough for fully-formatted filename.  ext
 *  can start with or without a point.
 */

int
Fixed_extension (char *dest, const char *src, const char *ext)
{
    ASSERT (dest != NULL);
    ASSERT (src != NULL);
    ASSERT (ext != NULL);
    if (dest != src)                    /*  Copy src to dest if not same     */
        strcpy (dest, src);

    Strip_extension (dest);
    return (Default_extension (dest, dest, ext));
}


/*  -------------------------------------------------------------------------
 *  Strip_extension
 *
 *  Description: removes dot and extension from name, if any was present.
 *  Returns: name.
 */

char
*Strip_extension (char *name)
{
    char *dot, *Path_end;

    ASSERT (name != NULL);
    dot = strrchr (name, '.');          /*  Find dot in name, if any         */
    Path_end = strrchr (name, PATHEND); /*  Find end of path, if any         */
    if (dot > Path_end)
        *dot = 0;                       /*  If we had a dot, truncate name   */

    return (name);
}


/*  -------------------------------------------------------------------------
 *  Strip_file_path
 *
 *  Description: removes leading file path from name, if any was present.
 *  Returns: name.
 */

char
*Strip_file_path (char *name)
{
    char *Path_end;

    ASSERT (name != NULL);
    Path_end = strrchr (name, PATHEND); /*  Find end of path, if any         */
    if (Path_end != NULL)
        memmove (Path_end + 1, name, strlen (Path_end));

    return (name);
}

#ifdef nosuchluck


/*  -------------------------------------------------------------------------
 *  Write_file
 *
 *  Description: writes string to the open stream.
 *  Returns: string, or NULL if no data could be written from the stream.
 */

char
*Write_file (FILE *stream, const char *string)
{
    fputs ((char *) string, stream);
    if (fputc ('\n', stream) == EOF)
        return (NULL);
    else
        return (string);
}
#endif
