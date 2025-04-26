/*===========================================================================*
 *                                                                           *
 *  socketio.c  socket read/write functions                                  *
 *                                                                           *
 *  Written:    92/10/22  Pieter Hintjens                                    *
 *  Revised:    92/11/02                                                     *
 *              92/11/04  PAH   Removed Tracing                              *
 *              92/11/11  PAH   Small changes                                *
 *              92/11/26  PAH   Corrected some error messages                *
 *              92/12/14  PAH   Increased size of select() mask              *
 *                                                                           *
 *===========================================================================*/

#include "prelude.h"
#include "socketio.h"


/*-----------------------------.
 |  ConnectSocket              |
 |-----------------------------`---------------------------------------------.
 |  int ConnectSocket (HostDef *host);                                       |
 |                                                                           |
 |  Description: establishes a socket connection to a server process on the  |
 |  host, using the Internet protocol layer.  Opens the socket, gets the     |
 |  name for the host, and tries to connect.  If the connect function fails, |
 |  ConnectSocket tries again after the delay specified in host-> delay.     |
 |  Host-> tries can be any integer 0 or greater (0 meaning the same as 1).  |
 |                                                                           |
 |  Returns: socket handle if okay, else -1 if an error occurred.  If an     |
 |  error occurred, displays an appropriate error message on stderr.         |
 |                                                                           |
 |  Comments: determines the host address from host-> addr and name; if both |
 |  are empty, assumes the current host, else converts from name to addr, or |
 |  vice-versa.  If both are supplied, addr takes precedence.                |
 `---------------------------------------------------------------------------*/

int ConnectSocket (HostDef *host)
{
    int    handle,                      /*  Handle for opened socket         */
           attempts;                    /*  Nbr of attempts to connect       */
    char   hostname [MAXHOSTNAMELEN+1]; /*  Name of this system              */
    struct hostent *hostdef;            /*  Host description of our system   */
    struct sockaddr_in service = {      /*  Destination socket:              */
            AF_INET,                    /*    protocol is internet           */
            0,                          /*    need to fill this in           */
            0,                          /*    need to fill this in           */
            ""                          /*    unused part of address         */
        };

    /*  Get host description, either from address, name, or default          */
    if (strused (host-> addr))
        hostdef = gethostbyaddr (host-> addr, strlen (host-> addr), AF_INET);
    else {
        if (strnull (host-> name))
            gethostname (hostname, sizeof (hostname));
        else
            strcpy (hostname, host-> name);

        hostdef = gethostbyname (hostname);
    }
    if (hostdef == NULL) {
        fprintf (stderr, "Problem with host %s...\n", hostname);
        perror ("Unable to get host information");
        return (-1);
    }

    /*  Set up socket parameters, and try to connect to host                 */
    memcpy (&service.sin_addr, hostdef-> h_addr, hostdef-> h_length);
    service.sin_port = host-> port;

    for (attempts = 0;;) {
        attempts++;
        printf ("Attempt %d to connect to port %d...",
            attempts, host-> port);

        handle = socket (AF_INET, SOCK_STREAM, 0);
        if (handle == -1) {
            perror ("Unable to open socket");
            return (-1);
        } else
        if (handle >= MASK_MAX) {       /*  Number of bits in select mask    */
            fprintf (stderr, "Too many open files or streams.\n");
            close (handle);
            return (-1);
        }
        if (connect (handle, &service, sizeof (service)) == 0)
            break;
        if (attempts >= host-> tries) {
            fprintf (stderr, "Cannot connect to port %d on host %s...\n",
                     host-> port, hostdef-> h_name);
            close (handle);
            return (-1);
        }
        close (handle);
        sleep (host-> delay);
    }
    return (handle);
}


/*-----------------------------.
 |  WriteSocket                |
 |-----------------------------`---------------------------------------------.
 |  int WriteSocket (int handle, char *buffer, size_t size, int mode);       |
 |                                                                           |
 |  Description: writes size bytes to the specified socket handle.  If mode  |
 |  is MODE_VAR, WriteSocket first sends a two-byte header indicating the    |
 |  size of the message.  If mode is MODE_FIXED, this header is not written. |
 |  The size can be 0 or more.  The MODE_VAR header is sent in network byte  |
 |  order, using htons().                                                    |
 |                                                                           |
 |  Returns: number of bytes written, 0..n, or -1 if an i/o error occurred.  |
 `---------------------------------------------------------------------------*/

int WriteSocket (int handle, char *buffer, size_t size, int mode)
{
    short header;                       /*  Size in network byte order       */

    if (mode == MODE_VAR) {             /*  Write size in network order      */
        header = htons (size);
        if (write (handle, &header, sizeof (short)) < sizeof (short)) {
            printf ("Error writing to socket %d", handle);
            return (-1);
        }
    }
    return (write (handle, buffer, size) == size? size: -1);
}


/*-----------------------------.
 |  ReadSocket                 |
 |---------------------------------------------------------------------------.
 |  int ReadSocket (int handle, char *buffer, size_t size, int mode);        |
 |                                                                           |
 |  Description: reads a message from the specified socket handle. If mode   |
 |  is MODE_VAR, ReadSocket first reads a two-byte header indicating the     |
 |  size of the message; if this is greater than size the message will be    |
 |  truncated.  If mode is MODE_FIXED, reads size bytes.  The buffer must be |
 |  at least size bytes long.                                                |
 |                                                                           |
 |  Returns: number of bytes read, 0..n, or -1 if an i/o error occurred.     |
 `---------------------------------------------------------------------------*/

int ReadSocket (int handle, char *buffer, size_t size, int mode)
{
    short readcount;                    /*  Nbr bytes to read from socket    */

    if (mode == MODE_VAR) {
        if (read (handle, &readcount, sizeof (short)) < sizeof (short)) {
            printf ("Error reading from socket %d", handle);
            return (-1);                /*  I/O error on read                */
        }
        if (ntohs (readcount) > size) {
            printf ("Message was too long for buffer; truncated");
            return (-1);
        }
        readcount = min (ntohs (readcount), size);
    } else
        readcount = size;

    return (read (handle, buffer, readcount) == readcount? readcount: -1);
}


/*-----------------------------.
 |  SelectSockets              |
 |-----------------------------`---------------------------------------------.
 |  int SelectSockets (int *mask, struct timeval *timeout)                   |
 |                                                                           |
 |  Description: calls select () to poll a set of streams or sockets for     |
 |  input data.  Mask is a bitmap of streams (ie. socket handles) to check.  |
 |  Timeout is the maximum poll delay, in seconds and microseconds.  Unix    |
 |  usually sets a lower limit of 1/1000 second.                             |
 |                                                                           |
 |  Returns: 0 to MASK_MAX - 1 indicates the socket which has input ready;   |
 |  SELECT_TIMEOUT indicates that no input arrived before the timeout; -1    |
 |  indicates a fatal error - in that case, SelectSockets displays a message |
 |  on stderr.                                                               |
 |                                                                           |
 |  Comments: SelectSockets does not poll for write or exception.            |
 `---------------------------------------------------------------------------*/

int SelectSockets (int *mask, struct timeval *timeout)
{
    int readmask [MASK_SIZE],           /*  Mask of streams for select ()    */
        result,                         /*  Return code from select ()       */
        stream;                         /*  Test each stream in turn         */

    memcpy (readmask, mask, MASK_BYTES);
    result = select (MASK_MAX, readmask, 0, 0, timeout);

    switch (result) {
        case -1:                        /*  Error on select ()               */
            perror ("Could not use select function");
            return (-1);

        case 0:                         /*  Timed-out without data ready     */
            return (SELECT_TIMEOUT);

        default:                        /*  Have input from someone          */
            for (stream = 0; stream < MASK_MAX; stream++)
                if ((mask_tst (mask, stream))
                AND (mask_tst (readmask, stream)))
                    return (stream);

            fprintf (stderr, "Bad return '%d' from select()\n", result);
            return (-1);
    }
}
