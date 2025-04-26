/*===========================================================================*
 *                                                                           *
 *  socketio.h  socket function definitions and prototypes                   *
 *                                                                           *
 *  Written:    92/10/22  Pieter Hintjens                                    *
 *  Revised:    92/11/02                                                     *
 *              92/11/04  PAH   Changed HOST_PARAMS to HostDef               *
 *              92/11/12  PAH   Cleaned-up comments                          *
 *              92/11/18  PAH   Changed definition of HostDef.Name           *
 *                        PAH   Removed 'protocol' definitions               *
 *              92/11/30  PAH   Added SOCKET_CLOSED equal to -1              *
 *              92/12/14  PAH   Increased MASK_MAX to 256                    *
 *===========================================================================*/

#ifndef _SOCKETIO_INCLUDED              /*  Allow multiple inclusions        */
#define _SOCKETIO_INCLUDED

/*-----------------------------.
 |  HostDef                    |
 |-----------------------------`---------------------------------------------.
 |  Structure used by ConnectSocket to define a host.  Could be set-up from  |
 |  a .ini file with a format like this:                                     |
 |                                                                           |
 |      [SERVER]                                                             |
 |          SystemId = <Internet-id>                                         |
 |          SystemName = <name>                                              |
 |          Port = <port-number>                                             |
 |          Connect = <attempts>[,<delay>]                                   |
 |          Password = <password1>[,<password2>]                             |
 |                                                                           |
 |  or as static definitions like this:                                      |
 |                                                                           |
 |      HostDef Server_host = { "", "", 5002, 10, 5, "acse ", "" };          |
 |                                                                           |
 `---------------------------------------------------------------------------*/

typedef struct {
    char  *addr,                        /*  IP address of host if known      */
          *name;                        /*  Text name of host if known       */
    word   port;                        /*  Port number to connect to        */
    int    tries,                       /*  Max number of connect attempts   */
           delay;                       /*  Seconds delay between attempts   */
    char  *password [2];                /*  First and second passwords       */
    } HostDef;


/*  Function prototypes                                                      */

int  ConnectSocket  (HostDef *host);
int  WriteSocket    (int handle, char *buffer, size_t size, int mode);
int  ReadSocket     (int handle, char *buffer, size_t size, int mode);
int  SelectSockets  (int *mask, struct timeval *timeout);


/*  Symbolic constants                                                       */

#define MASK_SIZE               8       /*  Nr ints in SelectSockets mask    */
#define MASK_BYTES              (sizeof(int) * MASK_SIZE)
#define MASK_MAX                (sizeof(int) * MASK_SIZE * 8)
#define SELECT_TIMEOUT          9999    /*  Return code from SelectSockets   */
#define MODE_VAR                0       /*  Modes for Read/WriteSocket       */
#define MODE_FIXED              1
#define SOCKET_CLOSED           -1      /*  Socket handle closed             */
#define INT_BITS                (sizeof (int) * 8)


/*  Macro pseudo-functions                                                   */

#define mask_msk(bit)           (1 << ((bit) % INT_BITS))
#define mask_set(mask,bit)      mask [(bit) / INT_BITS] |=  mask_msk (bit)
#define mask_clr(mask,bit)      mask [(bit) / INT_BITS] &= ^mask_msk (bit)
#define mask_tst(mask,bit)      mask [(bit) / INT_BITS] &   mask_msk (bit)


#endif
