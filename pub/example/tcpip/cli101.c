/*===========================================================================*
 *                                                                           *
 *  cli101.c                                                                 *
 *                                                                           *
 *  Written: 92/11/27  Pieter Hintjens                                       *
 *  Revised: 92/12/14  Test client - variation number 101                    *
 *                                                                           *
 *  This program is a tcp/ip client that works with srv101.c.  It:           *
 *                                                                           *
 *      - connects to the server on port 5111.                               *
 *      - enters a select() loop waiting for input or timeout.               *
 *      - if input message is "OKAY" it displays it                          *
 *      - if input message is "EXIT" is closes the socket and goes back      *
 *        to step A.                                                         *
 *                                                                           *
 *===========================================================================*/

#include "prelude.h"                    /*  Include standard definitions     */
#include "library.h"                    /*  Other library functions          */
#include "message.h"                    /*  Message display functions        */
#include "socketio.h"                   /*  Socket i/o functions             */
#include "trace.h"                      /*  Trace functions                  */

#include "cli101d.d"                    /*  Dialog definitions               */


/*  Identifiers for error messages                                           */

#define MSG_FATAL_ERROR         1000
#define MSG_BAD_PARENT_MSG      1001
#define MSG_BAD_CHILD_MSG       1002
#define MSG_BAD_PARENT_ACK      1003
#define MSG_BAD_CHILD_ACK       1004
#define MSG_WRONG_PASSWORD      1005
#define MSG_READ_TIMEOUT        1006
#define MSG_WRITE_ERROR         1007
#define MSG_READ_ERROR          1008
#define MSG_IMPOSSIBLE_MSG      1009
#define MSG_BAD_STATUS_CODE     1010
#define MSG_QUEUES_INIT_ERROR   1011
#define MSG_CLASS_FILE_NF       1012
#define MSG_CLASS_ERROR         1013
#define MSG_OUT_OF_MEMORY       1014
#define MSG_BAD_SELECT_CODE     1015
#define MSG_CHILD_NOT_READY     1016
#define MSG_CONNECTING          1017
#define MSG_DISCONNECTING       1018
#define MSG_INV_MSG_FOR_CHILD   1019
#define MSG_CHILD_FLOW          1020
#define MSG_PARENT_FLOW         1021
#define MSG_INTERRUPT           1022
#define MSG_NO_CONNECT_CHILD    1023
#define MSG_NO_CONNECT          1024
#define MSG_ERROR               1100


/*  Function prototypes                                                      */

local ShutDownProcess       (int Signal);


/*  Variables initialised from frt2l.ini                                     */

static HostDef
    Parent = { "", "", 5111, 10, 3, "", "" };

static int
    RetryDelay = 60;                    /*  Will retry every 60 seconds      */

static struct timeval
    LogonTimeout = { 5, 0 };


/*  Global variables used in this source only                                */

static int
    ParentSocket,                       /*  Socket to parent server          */
    SelectMask [MASK_SIZE];             /*  Select () bitmap for parent      */

static time_t
    ParentLogonTime;                    /*  Time to try and logon to parent  */

#define SOCKET_CLOSED   -1              /*  Impossible socket handle         */


int main ()
{
    int feedback = 0;                   /*  Return code from main()          */

    TraceState = TRUE;
    strcpy (TracePrefix, "CLI101>");

    #include "cli101.i"                 /*  Do dialog manager                */
}


/*************************   INITIALISE THE PROGRAM   ************************/

MODULE initialise_the_program (void)
{
    int Signal;

    ParentSocket   = SOCKET_CLOSED;     /*  Both sockets are closed          */

    /*  Install handlers for all interrupts                                  */
    for (Signal = 0; Signal < 32; Signal++)
        signal (Signal + 1, SIG_IGN);   /*  Ignore unwanted interrupts       */

    signal (SIGINT,  ShutDownProcess);  /*  Accept some other interrupts     */
    signal (SIGTERM, ShutDownProcess);
    signal (SIGABRT, ShutDownProcess);
    signal (SIGSEGV, ShutDownProcess);

    memset (SelectMask, 0, MASK_BYTES);
    OpenMessageFile ("frt2lmsg.txt");
    the_next_event = ok_event;
}


/************************   CONNECT TO PARENT HOST   *************************/

MODULE connect_to_parent_host (void)
{
    TraceLine ("Connect To Parent Host");

    ParentSocket = ConnectSocket (&Parent);
    if (ParentSocket == SOCKET_CLOSED)
        raise_exception (parent_error_event);
    else
        mask_set (SelectMask, ParentSocket);
}


/**********************   SET PARENT CONNECT TIMEOUT   ***********************/

MODULE set_parent_connect_timeout (void)
{
    ParentLogonTime = time (NULL) + RetryDelay;
}


/*********************   CLEAR PARENT CONNECT TIMEOUT   **********************/

MODULE clear_parent_connect_timeout (void)
{
    ParentLogonTime = 0;                /*  We're connected to parent        */
}


/*********************   CHECK IF TIME TO LOGON PARENT   *********************/

MODULE check_if_time_to_logon_parent (void)
{
    if (ParentLogonTime)
        if (time (NULL) >= ParentLogonTime)
            raise_exception (time_to_logon_event);
}


/************************   WAIT FOR PARENT MESSAGE   ************************/

local WaitForMessage (int *Mask, struct timeval *Timeout);

MODULE wait_for_parent_message (void)
{
    WaitForMessage (SelectMask, &LogonTimeout);
}


/*-----------------------------.
 |  WaitForMessage             |
 |-----------------------------`---------------------------------------------.
 |  local WaitForMessage (int *Mask, struct timeval *Timeout)                |
 |                                                                           |
 |  Description: calls SelectSockets () to check for input on a set of open  |
 |  sockets.  Sets the_next_event to from_parent, from_child, timeout, or    |
 |  raises a error exception if an error occurred.                           |
 `---------------------------------------------------------------------------*/

local WaitForMessage (int *Mask, struct timeval *Timeout)
{
    int result;                         /*  Return code from SelectSockets   */

    result = SelectSockets (Mask, Timeout);
    if (result == SELECT_TIMEOUT)
        the_next_event = timeout_event;
    else
    if (result == -1) {                 /*  Error, probably broken pipe      */
        PrintMessage (MSG_BAD_SELECT_CODE, result);
        PrintMessage (MSG_READ_ERROR, "2/High");
        raise_exception (parent_error_event);
    } else
        the_next_event = from_parent_event;
}


/**********************   DECODE MESSAGE FROM PARENT   ***********************/

MODULE decode_message_from_parent (void)
{
    char Msg [20];

    if (ParentSocket == SOCKET_CLOSED) {
        PrintMessage (MSG_NO_CONNECT, "2/high");
        raise_exception (error_event);
    } else
    if error (ReadSocket (ParentSocket, Msg, 20, MODE_VAR)) {
        PrintMessage (MSG_READ_ERROR, "2/high");
        raise_exception (parent_error_event);
    } else
    if (streq (Msg, "OKAY"))
        the_next_event = ok_event;
    else
    if (streq (Msg, "EXIT"))
        the_next_event = disconnect_event;
}


/****************************   DISPLAY MESSAGE   ****************************/

MODULE display_message (void)
{
    TraceLine ("OKAY");
}


/**********************   DISCONNECT FROM PARENT HOST   **********************/

MODULE disconnect_from_parent_host (void)
{
    TraceLine ("Disconnect From Parent Host");

    shutdown (ParentSocket, 2);
    close (ParentSocket);
    ParentSocket = SOCKET_CLOSED;
}


/***********************   SIGNAL FATAL ERROR ABORT   ************************/

MODULE signal_fatal_error_abort (void)
{
    PrintMessage (MSG_FATAL_ERROR);
}


/***************************   GET EXTERNAL EVENT   **************************/

MODULE get_external_event (void)
{
}


/*************************   TERMINATE THE PROGRAM   *************************/

MODULE terminate_the_program (void)
{
    the_next_event = terminate_event;
}


/***********************   GENERALLY USED FUNCTIONS   ************************/


/*-----------------------------.
 |  ShutDownProcess            |
 |-----------------------------`---------------------------------------------.
 |  local ShutDownProcess (Int Signal);                                      |
 |                                                                           |
 |  Description: this function is called when the process is terminated by a |
 |  kill or interrupt command.  It closes all files, sends sign-off messages |
 |  to all interlocutors, cuts their connections, and exits.                 |
 `---------------------------------------------------------------------------*/

local ShutDownProcess (int Signal)
{
    signal (SIGINT,  SIG_IGN);          /*  Ignore further interrupts until  */
    signal (SIGTERM, SIG_IGN);          /*     we're finished cleaning-up    */
    signal (SIGABRT, SIG_IGN);
    signal (SIGSEGV, SIG_IGN);

    PrintMessage (MSG_INTERRUPT, Signal);

    disconnect_from_parent_host ();
    terminate_the_program       ();
    exit (0);
}
