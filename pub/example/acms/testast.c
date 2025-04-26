/*===========================================================================*
 *                                                                           *
 *  testast     Test of Libero/AST                                           *
 *                                                                           *
 *  Written:    93/04/21    Pieter Hintjens                                  *
 *  Revised:    95/12/07                                                     *
 *                                                                           *
 *  Usage:      Ansi C with VAX library calls                                *
 *                                                                           *
 *===========================================================================*/

#include "prelude.h"                    /*  Public definitions               */

/*- Structure definitions ---------------------------------------------------*/

typedef struct {                        /*  Thread's context block:          */
    long Channel;                       /*    Channel for i/o                */
    int  InputSize;                     /*    Number of chars read           */
    char Input [LINE_MAX + 1];          /*    Input from terminal            */
    IOSB iosb;                          /*    IO status from QIO ()          */
} CONTEXT;

#include "testast.d"                    /*  Include dialog generated code    */

/*- Local function prototypes -----------------------------------------------*/

local  WriteChannel      (THREAD *Thread, char *Text);
local  ReadChannel       (THREAD *Thread);
local  PrintErrorMessage (char *Msg, long rc);


/*********************************   MAIN   **********************************/

int main (void)
{
    CONTEXT *Context;

    Context = (CONTEXT *) malloc (sizeof (CONTEXT));
    ASTInitialise (Context);
    sys_hiber ();
    return (RET_OKAY);
}


/*************************   INITIALISE THE THREAD   *************************/

MODULE InitialiseTheThread (THREAD *Thread)
{
    Thread-> Data-> Channel = 0;
    Thread-> TheNextEvent = OkEvent;
}


/*************************   OPEN TERMINAL CHANNEL   *************************/

MODULE OpenTerminalChannel (THREAD *Thread)
{
    static desc
        String (TTDevice, "TT");
    long rc;                            /*  Return code from VAX functions   */

    rc = sys_assign (&TTDevice, &Thread-> Data-> Channel, 0, 0, 0);
    if ((rc & 1) == 0) {
        PrintErrorMessage ("Cannot assign TT channel", rc);
        RaiseException    (Thread, IoErrorEvent);
    }
    ASTContinue (Thread);
}


/***********************   WRITE MESSAGE TO CHANNEL   ************************/

MODULE WriteMessageToChannel (THREAD *Thread)
{
    WriteChannel (Thread, "Choice (A=Again X=Exit):\n\r");
}


/***********************   READ MESSAGE FROM CHANNEL   ***********************/

MODULE ReadMessageFromChannel (THREAD *Thread)
{
    ReadChannel (Thread);
}


/*************************   CHECK IO STATUS BLOCK   *************************/

MODULE CheckIoStatusBlock (THREAD *Thread)
{
    if ((Thread-> Data-> iosb.Status & 1) == 0) {
        PrintErrorMessage ("I/O failed", Thread-> Data-> iosb.Status);
        RaiseException    (Thread, IoErrorEvent);
    }
    ASTContinue (Thread);
}


/************************   CHECK MESSAGE CONTENTS   *************************/

MODULE CheckMessageContents (THREAD *Thread)
{
    switch (toupper (Thread-> Data-> Input [0])) {
        case 'A':
            Thread-> TheNextEvent = AgainEvent;
            break;
        default:
            Thread-> TheNextEvent = ExitEvent;
    }
    ASTContinue (Thread);
}


/************************   CLOSE TERMINAL CHANNEL   *************************/

MODULE CloseTerminalChannel (THREAD *Thread)
{
    long rc;                            /*  Return code from VAX functions   */

    if (Thread-> Data-> Channel) {
        rc = sys_dassgn (Thread-> Data-> Channel);
        Thread-> Data-> Channel = 0;
        if ((rc & 1) == 0) {
            PrintErrorMessage ("Cannot de-assign LAT channel", rc);
            RaiseException    (Thread, IoErrorEvent);
        }
    }
    ASTContinue (Thread);
}


/*************************   TERMINATE THE THREAD   **************************/

MODULE TerminateTheThread (THREAD *Thread)
{
    Thread-> TheNextEvent = TerminateEvent;
    free (Thread-> Data);
    ASTContinue (Thread);
}


/***********************   GENERAL PURPOSE FUNCTIONS   ***********************/

/*-----------------------------.
 |  WriteChannel / CHAIN       |
 |-----------------------------`---------------------------------------------.
 |  local WriteChannel (THREAD *Thread, char *Text);                         |
 |                                                                           |
 |  Writes the text to the current channel and chains to ASTContinue.  If    |
 |  the QIO failed, prints an error message and raises the io_error event.   |
 `---------------------------------------------------------------------------*/

local WriteChannel (THREAD *Thread, char *Text)
{
    long rc;                            /*  Return code from VAX functions   */

    /*  Write text to port                                                   */
    rc = sys_qio (0,                    /*  Event flag                       */
            Thread-> Data-> Channel,    /*  Channel                          */
            IO_WRITEVBLK,               /*  Function code                    */
            &Thread-> Data-> iosb,      /*  IO status block                  */
            ASTContinue, Thread,        /*  AST address and parameter        */
            Text,                       /*  Block to write                   */
            strlen (Text),              /*  Size of block                    */
            0, 0, 0, 0);                /*  Parameters 4 to 6 unused         */

    /*  If QIO failed, print message and raise exception                     */
    if ((rc & 1) == 0) {
        PrintErrorMessage ("Error writing to channel", rc);
        RaiseException    (Thread, IoErrorEvent);
        ASTContinue       (Thread);
    }
}

/*-----------------------------.
 |  ReadChannel / CHAIN        |
 |-----------------------------`---------------------------------------------.
 |  local ReadChannel (THREAD *Thread, char *Text, int Length);              |
 |                                                                           |
 |  Reads a field from the channel into Thread-> Data-> Input, and its size  |
 |  into Thread-> Data-> InputSize.  If the QIO failed, prints an error      |
 |  error message and raises the io_error event.                             |
 `---------------------------------------------------------------------------*/

local ReadChannel (THREAD *Thread)
{
    long rc;                            /*  Return code from VAX functions   */

    /*  Read text from port                                                  */
    rc = sys_qio (0,                    /*  Event flag                       */
            Thread-> Data-> Channel,    /*  Channel                          */
            IO_READVBLK,                /*  Function code                    */
            &Thread-> Data-> iosb,      /*  IO status block                  */
            ASTContinue, Thread,        /*  AST address and parameter        */
            Thread-> Data-> Input,      /*  Input buffer                     */
            LINE_MAX,                   /*  Size of block                    */
            0, 0, 0, 0);                /*  Parameters 4 to 6 unused         */

    /*  If QIO failed, print message and raise exception                     */
    if ((rc & 1) == 0) {
        PrintErrorMessage ("Error reading from channel", rc);
        RaiseException    (Thread, IoErrorEvent);
        ASTContinue       (Thread);
    }
}

/*-----------------------------.
 |  PrintErrorMessage          |
 |-----------------------------`---------------------------------------------.
 |  void PrintErrorMessage (char *Msg, long rc);                             |
 |                                                                           |
 |  Writes error message to stderr and displays the system-specific message  |
 |  text if possible.  Prints the message with this format:                  |
 |                                                                           |
 |      Text of message: (return code) system message.                       |
 `---------------------------------------------------------------------------*/

static void PrintErrorMessage (char *Message, long rc)
{
    static char
        ErrorText [256];                /*  Error message text               */
    static dsc_descriptor_s             /*  Descriptor for name              */
        ErrorDesc = { 256, DSC_K_DTYPE_T, DSC_K_CLASS_S, ErrorText };
    word Length;

    sys_getmsg (rc, &Length, &ErrorDesc, 0, 0);
    ErrorText [Length] = 0;
    fprintf (stderr, "%s: (%ld) %s\n", Message, rc, ErrorText);
}
