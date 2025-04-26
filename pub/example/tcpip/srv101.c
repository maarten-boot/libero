/*===========================================================================*
 *                                                                           *
 *  srv002.c    test tcp/ip server variation 101                             *
 *                                                                           *
 *  Written:    92/11/27   Pieter Hintjens                                   *
 *  Revised:    92/12/02                                                     *
 *                                                                           *
 *  Standard:   ANSI C with Unix extensions.                                 *
 *              To compile on HP Unix use: cc $1.c -Aa -D_HPUX_SOURCE        *
 *                                                                           *
 *  Accepts connection on port 5111, then enters loop.  Sleeps for 5         *
 *  seconds, then sends OKAY message.  If interrupted, closes connection     *
 *  and continues.  After 3 kills, exits.                                    *
 *===========================================================================*/

#include "prelude.h"
#include "socketio.h"
#include "trace.h"
#include "msgt2.h"

#define PORT  5111

/*  Function prototypes                                                      */

local handle_connection (void);
local ShutDown1         (int Signal);
local ShutDown2         (int Signal);


/*  Global data used in this file only                                       */

static int
    Kills = 0,
    ConHandle,
    MsgHandle;                          /*  Handle of received socket        */


void main (void)
{
    struct sockaddr_in service = {      /*  Server socket:                   */
              AF_INET,                  /*    protocol is internet           */
              PORT,                     /*    port number for socket         */
              INADDR_ANY,               /*    accept any client systems      */
              ""                        /*    unused part of address         */
          },
          msgservice;                   /*  Socket address from client       */

    int socketlen,                      /*  Size of received socket address  */
        On = 1;

    TraceState = TRUE;
    strcpy (TracePrefix, "SRV101>");

    signal (SIGTERM, ShutDown1);
    signal (SIGINT,  ShutDown1);

    /*  Open the socket and bind to Internet port                            */
    ConHandle = socket (AF_INET, SOCK_STREAM, 0); /* Open socket for use     */
    if error (ConHandle) {
        TraceLine ("Unable to open socket");
        perror ("");
        exit (1);
    }
    setsockopt (ConHandle, SOL_SOCKET, SO_REUSEADDR, &On, sizeof (On));
    if error (bind (ConHandle, &service, sizeof (service))) {
        TraceLine ("Unable to bind to service");
        perror ("");
        close (ConHandle);
        exit (1);
    }

    /*  Set queue for a nominal number of connection requests                */
    if error (listen (ConHandle, 5)) {
        TraceLine ("Unable to set input queue for socket");
        perror ("");
        close (ConHandle);
        exit (1);
    }

    /*  Loop for ever, waiting for incoming client connection requests       */
    for ever {
        if error (MsgHandle = accept (ConHandle, &msgservice, &socketlen)) {
            TraceLine ("Unable to accept socket from client");
            perror ("");
            continue;
        }
        TraceLine ("Connection on socket %d", MsgHandle);
        switch (fork ()) {
            case FORK_ERROR:
                TraceLine ("Could not fork to handle connection");
                perror ("");
                exit (1);

            case FORK_CHILD:
                handle_connection ();
        }
        close (MsgHandle);
    }
}


local handle_connection (void)
{
    signal (SIGTERM, ShutDown2);
    signal (SIGINT,  ShutDown2);

    for ever {
        sleep (5);
        WriteSocket (MsgHandle, "OKAY", 5, MODE_VAR);
    }
}

local ShutDown1 (int Signal)
{
    TraceLine ("Parent interrupt %d [%d]...", Signal, Kills);
    if (Kills++ == 3) {
        shutdown (ConHandle, 2);
        close (ConHandle);
        exit (0);
    }
}

local ShutDown2 (int Signal)
{
    TraceLine ("Child interrupt %d...", Signal);
    WriteSocket (MsgHandle, "EXIT", 5, MODE_VAR);
    shutdown (MsgHandle, 2);
    close (MsgHandle);
    exit (0);
}
