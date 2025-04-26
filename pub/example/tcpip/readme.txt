-------------------------------------------------------------------------------
Package:    TCP/IP Server
-------------------------------------------------------------------------------

Written:    22 Oct 1995
Revised:    07 Dec 1995
Version:    1.1

Author:     Pieter A. Hintjens
            Pijlstraat 9
            2060 Antwerpen, Belgium
            ph@mymail.com
            (+323) 231.5277

Notes:      This package is deceptive, since the example programs do not
            include a TCP/IP server written using Libero!  The programs I
            wrote for this project are (a) not mine to share, and (b) too
            specific for general-purpose examples.

            I plan to write a HTTP server at some time in the future.  In
            the meantime, this package is waiting for someone to provide a
            nice TCP/IP server written with Libero...

Contents:   File:           Purpose:
            README   TXT    This file
            SOCKETIO C      TCP/IP socket i/o functions
            SOCKETIO H      prototypes
            PRELUDE  H      General 'C' include file
            CLI101   C      TCP/IP client, uses a dialog
            CLI101D  L      Dialog file
            SRV101   C      TCP/IP server, does not use a dialog

Copyright:  (c) 1991-96 Pieter A. Hintjens.

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

   -----------------------------------------------------------------
                           Internet Sockets
   -----------------------------------------------------------------

     Internet  sockets   provide  a   simple   Open/Read/Write/Close
   interface to  a sophisticated  and  robust  communications  layer
   (TCP/IP).   However, to  use sockets  well,  a  program  must  be
   written the right way.

     -  There must be a server and a client.  This distinction is
        both structural  and practical.   Structurally,  a server
        can handle  many clients  for the  same  service,  but  a
        client can  only connect  to one  server for a particular
        service.   Practically, the  server is  the one who has a
        `public' address  and  who  accepts  incoming  connection
        requests.  The client can choose when and where to try to
        connect.     Once  a   connection  is   established,  the
        distinctions are  irrelevant  -  either  party  can  send
        messages or cut the connection.

     -  The server  must `sit'  on a  well-known port  on a well-
        known system.   This  port is used for new connections to
        the server.  User-defined ports are usually numbered from
        5000 upwards.

     -  The server must be able to handle one, and often, several
        clients at  the same time.  Both client and server should
        be prepared to accept messages at any time, and be robust
        against missing  messages or  a broken  connection.  -The
        client and  server must  agree on  a  common  format  for
        messages  going   through  the  socket.    For  instance,
        messages can  be fixed at a certain size, or start with a
        header that  indicates the size of the message following.
        This lets  the recipient  of a  message  check  that  the
        expected number of characters were received - if not, the
        connection  was  probably  broken  in  the  middle  of  a
        message.

     There are  a number  of function calls that a C program uses to
   make a  socket connection.   This  diagram shows  the  connection
   between a typical client and server:

     -Server-------------------    Time   -Client-----------------
   1  handle = socket ()           : |
   2  bind (handle, )              : |
   3  listen (handle, )            : |
   4  newhdl = accept (handle, )   `-|-.   handle = socket ()   4
                                   .-|-'   connect (handle, )   5
   6  select ()                    `-|-.   write (handle, )     7
   8  read (newhandle, )         <---|-'

     Sockets (like  files) are  referenced through a `handle'.  This
   is an  integer which the program uses whenever it does i/o on the
   socket.   In the  above diagram  the two  partners go  through  a
   number of steps:

     -  The server opens a socket for connections.

     -  The  server  uses  bind()  to  attach  the  socket  to  a
        particular port.

     -  The server  uses listen()  to define a queue for incoming
        connection requests.

     -  The client also opens a socket, and then tries connect(),
        specifying the server's port and system name.

     -  The server  uses accept() to wait for incoming connection
        requests.   When a  request comes in, Unix provides a new
        socket number  which the  server uses for all i/o through
        that socket.

     -  The client  and server  can now  read and  write  to  the
        socket.

     -  Finally, one partner closes the socket.

     A critical  function is select().  This lets a program check or
   wait for data coming in on a socket.  If a server does a select()
   on its  primary socket,  it can  check  for  incoming  connection
   requests -  thus avoiding  a wait in an accept() call.  If either
   partner uses  select() on  the connected  socket, it  can avoid a
   wait in the read() call.

     An Internet socket address `struct in_addr' looks like this:

     short          sin_family;   - must be AF_INET
     u_short        sin_port;     - port number (<1024 = reserved)
     struct in_addr sin_addr;     - 32-bit Internet address struct
     char           sin_zero [8]; - unused.

     To connect  a socket,  the client  needs to  know the  server's
   system id and port number.  The system id can be derived from the
   system name  if that  is known.  The port number must be known in
   advance by the client.

     Many connections  can exist  on the  same port, since each gets
   its own  socket handle.   At least two techniques are possible to
   handle multiple clients:

     -  The server  can use  fork() to  create a  sub-process for
        each client.

     -  The server  can store a table of open socket handles, and
        manage messages on all these sockets simultaneously.

   -----------------------------------------------------------------
                              SOCKETIO.C
   -----------------------------------------------------------------

     This module provides general-purpose socket i/o functions which
   cover client  connection, reading,  and writing.   The prototypes
   for this module are defined in SOCKETIO.H.

     SOCKETIO.C is  portable to  any Unix environment which supports
   ANSI C and Internet sockets.

     SOCKETIO.C  works   with  Internet  sockets  and  provides  all
   functions needed  by a  normal client  program.   Server programs
   will need  to provided  their own  functions  to  open  and  bind
   sockets for  incoming connections.   A client program should call
   ConnectSocket() to  establish connection  to a host server.  When
   this succeeds  (possibly after  a series of retries), the program
   can use  ReadSocket  and  WriteSocket  to  communicate  with  the
   server. It  is a  good idea to use SelectSockets() before doing a
   read -  otherwise the  process  blocks  until data arrives.  In a
   typical server program, the program uses  SelectSockets() to wait
   for data from any of a number of sources, then calls ReadSocket()
   or does some action depending on which socket showed input.

   -----------------------------------------------------------------
                             ConnectSocket
   -----------------------------------------------------------------

     int ConnectSocket (HostDef *host);

     Establishes a  socket connection  to a  server process  on  the
   host, using  the Internet protocol layer.  Opens the socket, gets
   the name  for the  host, and  tries to  connect.   If the connect
   function  fails,   ConnectSocket  tries  again  after  the  delay
   specified in  host-> delay.  Host-> tries can be any integer 0 or
   greater (0 meaning the same as 1).

     Returns the  socket  handle  if  okay,  else  -1  if  an  error
   occurred.   If an  error occurred,  displays an appropriate error
   message on  stderr. Determines  the host address from host-> addr
   and name;  if both  are empty,  assumes the  current  host,  else
   converts from name to addr, or vice-versa.

     If both are supplied, addr takes precedence.

     The hostdef structure is defined in SOCKETIO.H as follows:

     typedef struct {
         char  *addr,         /*  IP address of host if known   */
               *name;         /*  Text name of host if known    */
         word   port;         /*  Port number to connect to     */
         int    tries,        /*  Max number of connect attempts */
                delay;        /*  Seconds delay between attempts */
         char  *password [2]; /*  First and second passwords     */
         } HostDef;

   -----------------------------------------------------------------
                              WriteSocket
   -----------------------------------------------------------------

     int WriteSocket (int handle, char *buffer, size_t size, int
     mode);

     Writes size  bytes to  the specified socket handle.  If mode is
   MODE_VAR, WriteSocket  first sends  a two-byte  header indicating
   the size  of the  message.  If mode is MODE_FIXED, this header is
   not written.   The size can be 0 or more.  The MODE_VAR header is
   sent in network byte order, using htons().  Returns the number of
   bytes written, 0..n, or -1 if an i/o error occurred.

   -----------------------------------------------------------------
                              ReadSocket
   -----------------------------------------------------------------

     int ReadSocket (int handle, char *buffer, size_t size, int
     mode);

     Reads a  message from  the specified socket handle.  If mode is
   MODE_VAR, ReadSocket first reads a two-byte header indicating the
   size of  the message;  if this  is greater  than size the message
   will be truncated.  If mode is MODE_FIXED, reads size bytes.  The
   buffer must  be at  least size  bytes long. Returns the number of
   bytes read, 0..n, or -1 if an i/o error occurred.

   -----------------------------------------------------------------
                             SelectSockets
   -----------------------------------------------------------------

     int SelectSockets (int mask, struct timeval *timeout)

     Calls the select() function to poll a set of streams or sockets
   for input data.  Mask is a bitmap of streams (ie. socket handles)
   to check.   Timeout  is the  maximum poll  delay, in  seconds and
   microseconds.   Unix usually sets a lower limit of 1/1000 second.
   Returns 0 to MASK_MAX indicating the first socket which has input
   ready, or  SELECT_TIMEOUT indicating that no input arrived before
   the timeout;  returns -1 to indicate a fatal error. SelectSockets
   does not poll the socket for write or exception.
