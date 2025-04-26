-------------------------------------------------------------------------------
Package:    Multithreaded Agent For VAX/ACMS
-------------------------------------------------------------------------------

Written:    22 Oct 1995
Revised:    07 Dec 1995
Version:    1.1

Author:     Pieter A. Hintjens
            Pijlstraat 9
            2060 Antwerpen, Belgium
            ph@mymail.com
            (+323) 231.5277

Notes:      This package is not for the weak of heart!  You will only find
            something here if you are doing ACMS programming, or multithreaded
            work on other systems.

Contents:   File:           Purpose:
            README   TXT    This file
            LRSCHAST C      Schema for AST multithreaded programming
            TESTAST  C      Example program using LRSCHAST.C schema
            TESTAST  L      Dialog for example program
            PRELUDE  H      Include file for C programs
            AETKVLIB H      Include file for VAX/VMS prototypes and constants

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

-------------------------------------------------------------------------------
                         Why Write Multithreaded Code?
-------------------------------------------------------------------------------

  Digital VAX/ACMS  is a  client-server layer  that runs  on top of the Digital
VAX/VMS operating system.  It is a transaction processor somewhat akin to IBM's
CICS, or Tuxedo for Unix

  At one  end, a  group of  heavy programs  wait for  incoming  messages;  they
process these  then send  back the  answer.   These are  the 'servers'.  At the
other end  there are  a bunch  of users  tapping furiously  at their terminals,
asking to  see what  hotels still  have space  for a  party of  fifty-four non-
smoking pax  going to  San Turino  for a  week in  August, or  confirming a car
reservation  in  Madrid,  or  checking  the  availability  on  flights  between
Amsterdam Schiphol  and Lisbon  from 15  to 21  September,...   these  are  the
clients.

  Often, the  server programs  run on  a big central VAX, while the clients are
connected to a few smaller VAXen in regional offices around the country.

  In ACMS,  each client  terminal requires a program that controls the terminal
keyboard and  screen, and  sends back  messages to  the servers  when the  user
presses 'Execute'  or some  such key.   This  program is  an 'agent'.  It works
something like  this: the  agent shows a form; the user fills-in the blanks and
presses 'Execute'.   The  agent sends  the form back to the appropriate server,
which processes it, and sends back a reply.

  In our project we were unable to use any of the standard Digital agents (e.g.
for DecForms),  since we  wanted to  do our  own screen  handling.    The  only
recourse was  to write  our own  agent.  This is easy enough when each user can
run in  their own  interactive session.   The  agent program  is then  a normal
program which can handle a terminal and talk to ACMS.

  The fun  starts when you need to connect several hundred users at once.  VMS,
like any  operating system,  cannot handle  so many  interactive sessions.  The
only valid  approach is  to handle all the terminals in a single process.  This
means writing a multithreaded program where each thread manages one terminal.

  This sounds  a little  complex, but  is really quite simple.  When someone is
just looking  at the  screen, or  taking a  sip of  coffee, or scratching their
nose, they  don't need  any attention  from the  agent.  When they press a key,
they get  a tiny  slice of attention, just enough to react to the key.  If each
user press  a key  at the  same time,  some of  them will have to wait a little
while until  their terminal  reacts.   Normally, things  are  very  smooth  and
efficient, which is what the whole business is about.

  The catch  is that  it can  be very  difficult to  write multithreaded  code.
VAX/VMS provides  an interrupt  mechanism called  the AST  (asynchronous system
trap), that  lets you build a cooperatively multithreaded program.  The program
must do  all its  work in small steps.  Any input/output happens asynchronously
(i.e.   the code  does not  wait for an i/o to complete).  For example, to read
something from  a terminal  keyboard, the program calls the sys$qio() function.
VMS suspends  the program,  does the  i/o (which may take ages), then kicks the
program back into life when the results come in.  This 'kick' is the AST.  When
you call  sys$qio(), you  have to  give VMS  a 'completion address', i.e.  what
part of your code should get the AST.
  The program  becomes multithreaded  when you build-in the notion that several
i/o's can  be pending at the same time.  Each returns to a different completion
address.   VMS will  queue these i/o's nicely.  However, you have to figure-out
what to do when the code at the completion address gets control again.

-------------------------------------------------------------------------------
                          A Walk-through of the Agent
-------------------------------------------------------------------------------

  The agent  accepts connections  using the LAT protocol.  This is just one way
of doing  it.  When the agent starts up it creates a LAT service, which it maps
to a dedicated LAT port.  A dedicated port provides a 'service' to which a user
can connect using standard commands like 'connect'; this is a convenient way to
connect a  user to a process (our agent) without going through the standard VMS
log-in procedure.   The  agent then  initialises various  tables, and goes into
hibernation.   The LAT service is shown along with the standard services when a
user connects  to the  system.   Rather than connecting to the usual VAX system
name, they now enter:

  local> connect xxx+

  At this  point the agent receives an AST which triggers the code that handles
a new user.

  This code  allocates a  context block  for the  user, signs-in  to  ACMS  and
decides on  the ACMS  application and  task to  execute.  It then tells ACMS to
start the  application.   From now  on, the  user exists  as a  'thread' in the
awareness of  the agent.   The  thread is defined simply as a memory pointer to
the context  block.   Every AST  routine receives  this  memory  pointer  as  a
parameter, so  knows the  state of that specific thread.  The number of threads
is limited  only by  the available  memory; since  one thread allocates several
Kbytes of memory, a thousand threads is quite feasible.

  So, the  ACMS task  has started  up.  Our 'thread' is now in the hands of the
ACMS execution  controller, which  executes a  task (a  type  of  ACMS  control
program) running  on the  back-end.   The task  initialises, then  invokes  the
application root  program.   It in  turn initialises,  and decides to display a
screen - for example an application sign-on screen.

  The agent  is totally  event-driven.   Thus, it expects, and handles, various
incoming events.   One  of these  is a  request from  ACMS (from  the task)  to
display a  screen.   When this  event comes in, the agent (ie.  the AST handler
for the  event) decodes  the incoming  message, then  decides on  the action to
take.   In our  example, this  would be  to format  and display the application
sign-on screen.

  Control now  passes to  the AST  code that interacts with the user, accepting
control keys  and data  input.  When the screen has been filled (i.e.  the user
presses a  function key like [Enter]), the agent prepares a message holding the
changed screen  data.   It sends  this back to ACMS and the server program, and
the cycle continues.

  Eventually,  ACMS  tells  the  agent  that  the  user  has  stopped  working.
Typically, the  application root program told the task to terminate.  Sometimes
this happens  because of  problems at the back-end or front- end.  Whatever the
cause, the agent now runs-down the thread, then releases the context-block back
to heap memory.

  In practice,  the agent  is handling  many, perhaps hundreds of such threads,
simultaneously.  Each interaction with ACMS or the VMS system requires at least
one, and  often two  separate AST  steps.    You  can  see  that  the  possible
complexity of such a program is monumental.

  When our  client asked an independent consulting firm to estimate the cost of
the agent  we planned  to develop,  the figure,  after much  explanation on our
part, was several man years' work.

  Luckily they  were wrong.   In  fact, it took about 40 days to write and test
the multithreaded agent.

  I was  able to  work this quickly for a number of reasons.  Firstly, I worked
in C,  a good  language for  the job,  and a  language  I  already  knew  well.
Secondly, I  was able  to choose my work environment - working largely at home,
on a  fast PC, with uploading and testing on the VAX as necessary.  Thirdly, my
design turned  out to  be the  right one.   With the right tools I could pretty
much hide  the multithreaded  AST aspects of the problem, leaving only the real
work.

-------------------------------------------------------------------------------
                          Writing Multithreaded Code
-------------------------------------------------------------------------------

  There are two basic ways to write multithreaded code under VMS:

   + Re-entrant code: when an AST arrives, the current code is suspended and
     the AST  handler is started up.  Such a program is usually written as a
     large mainline procedure with some AST interrupt- handlers.

   + Chained code: ASTs for a process are queued as they arrive, and each is
     handled by  the appropriate  piece of code.  Each AST handler specifies
     its successor routine, building a chain of ASTs.  Such programs consist
     of a small mainline procedure, with the bulk of code as AST handlers.

  Re-entrant code  is good for large programs that may perform heavy processing
- even  a long  processing step  will not lock-out ASTs.  Its disadvantages are
the overhead  of context  saved at  each AST,  and the  fact that  code can  be
interrupted at  any time.  This last fact means, for instance, that global data
is not secure without some system of locking.

  Chained AST code is more complex to write, and only works if each AST handler
is quick,  but has  advantages over  re-entrant code:  there is  no  danger  of
interruption, so  global data  can be  accessed safely;  there is  no  context-
switching, so  the code runs faster.  We decided to follow Digital's advice and
use a chained-AST model.

  In the chained-AST model, the code does a little work, then another i/o, with
another completion address.  The result looks like this:

       ...
       step 15: do something, then go to step 16
       step 16: go to step 21 if there was an error
       step 17: do something, then go to step 23
       step 18: do something, if okay goto step 15, else 16
       ...

  It works,  just, for a toy program.  It is horrendous for a realistic program
where there  may be  several hundred  'steps'.  You start to see why people say
'uh-oh' when  they talk  about  writing  multithreaded  code.    This  type  of
programming gives  new significance to the word 'bug', since a simple error can
easily produce  sporadic and  untraceable problems.   The existence of a thread
rests entirely  in the  fact that  each AST  handler passes  control, like  the
Olympic torch,  to the  one correct successor in the chain.  It is very easy to
pass the thread to the wrong handler, to two handlers, or to no handler at all.

  Needless to  say, such  code is difficult to maintain, since there is no easy
way to understand its structure.

  Fundamentally, multithreaded  AST code is rather like a Fortran program which
I once  heard of.   The  author worked on punched cards, and was paranoid about
the key-punchers  dropping and  mixing the  cards.  His elegant solution was to
end each  line (i.e.   each  card) in  a GOTO to the next card.  So long as the
first card  (which he  made on  blue carton)  could be  put at  the start,  his
program was sure to run correctly.

-------------------------------------------------------------------------------
                           The Abstraction Mechanism
-------------------------------------------------------------------------------

  Like many  apparently simple  solutions, the  use  of  a  state  machine  for
multithreaded programming was not obvious at first.

  I saw  quite early  on that  the difficulty of AST programming lay largely in
the chaining  mechanism I describe above.  Digital had suggested that we try to
use an  alternative multi-threading  mechanism,  DecThreads,  but  this  proved
useless for  serious work,  since it  happily does synchronous i/o calls, which
really wait, and thus block the entire process.  All i/o (and other slow system
calls) must be done using asynchronous AST-type calls.

  The paradigm shift is this:

   + The agent has a number of states, like any program.

   + Each AST  handler can  be considered  as a step in the dialog, which we
     call an action.

   + The events  consist of  function return  codes,  keyboard  input,  ACMS
     messages, and so on.

   + Rather than  chaining to the actual successor routine, each AST routine
     chains to  the dialog  code, which  can  itself  decide  the  successor
     routine, depending  on the  current state, event, etc.  The dialog code
     interprets a Libero dialog, with all the standard functionality such as
     defaults state, exceptions, etc.  The agent program consists of a large
     number of  action modules,  all following  a standard style: they must,
     for instance, always return control to the dialog code.

  The LRSCHAST.C  schema generates  the necessary code.  Look at this schema to
see how it works.

  A program written using this schema looks in part like this:

/*- Structure definitions ---------------------------------------------------*/

typedef struct {                        /*  Thread's context block:          */
    IOSB    iosb;                       /*    IO status from QIO ()          */
    ACSB    acsb;                       /*    IO status from ACMS calls      */
    ACMSID  SubmitterId,                /*    Various identifiers            */
            ExchangeId,                 /*      returned from ACMS calls     */
            ProcedureId,                /*      like get_procedure_info.     */
            ConnectId,                  /*                                   */
            CallerId,                   /*                                   */
            StreamId;                   /*                                   */
    ...                                 /*    ... etc.
} CONTEXT;


/*- Local function prototypes -----------------------------------------------*/

static Boolean CheckStatus       (THREAD *Thread, long rc, char *Reason);


/*- Global data used in this source only ------------------------------------*/

static long
    rc;                                 /*  Return code from VAX functions   */

static CONTEXT
    *Data;                              /*  Points to user data area         */

#include "myprogd.d"                    /*  Include dialog generated code    */


/*********************************   MAIN   **********************************/

int main (void)
{
    Data = (CONTEXT *) calloc (sizeof (CONTEXT), sizeof (char));
    if (Data)                           /*  Spawn a new thread               */
        sys_dclast (ASTInitialise, Data, 0);
    else
        fprintf (stderr, "Heap memory exhausted\n");

    acms_wait (&MainAcsb);              /*  Suspend process mainline code    */
    return (RET_OKAY);
}

local InitialiseTheThread (THREAD *Thread)
{
    Data = Thread-> Data;               /*  Address user context block       */

    Data-> SubmitterId  =               /*  No connection to ACMS            */
    Data-> ExchangeId   =               /*  No exchange i/o handle           */
    Data-> ProcedureId  =               /*  No procedure queried             */
    Data-> ConnectId    =               /*  No connection to ACMS            */
    Data-> CallerId     = BlankId;      /*  No ACMS CALL started             */
    Data-> acsb.Status  = 1;            /*  No ACMS errors                   */
}


/*****************************   ACMS SIGN IN   ******************************/

MODULE AcmsSignIn (THREAD *Thread)
{
    static dsc_descriptor_s
        UserName = {
            "AETKSERV", DSC_K_DTYPE_T, DSC_K_CLASS_S, 8
        };

    Data = Thread-> Data;               /*  Address user context block       */

    rc = acms_sign_in_a (
        &Data-> SubmitterId,            /*  Submitter ID                     */
        &UserName,                      /*  User name                        */
        0,                              /*  Device name                      */
        0, 0,                           /*  Cancel routine and parameter     */
        &Data-> acsb, &ACMS_EFN,        /*  Status block and event flag      */
        ASTContinue, Thread             /*  AST address and parameter        */
    );
    CheckStatus (Thread, rc, "ACMS$SIGN_IN");
}


/*************************   ACMS INIT EXCHANGE IO   *************************/

MODULE AcmsInitExchangeIo (THREAD *Thread)
{
    static long
        IoFlags = ACMS_M_IO_DISABLE_TDMS + ACMS_M_IO_DISABLE_DECFORMS;

    Data = Thread-> Data;               /*  Address user context block       */

    rc = acms_init_exchange_io_a (
        &Data-> SubmitterId,            /*  Submitter ID                     */
        &Data-> ExchangeId,             /*  Exchange I/O ID                  */
        &IoFlags,                       /*  I/O enable flags                 */
        &Data-> IoList,                 /*  Item list                        */
        0,                              /*  I/O capabilities                 */
        &Data-> acsb, &ACMS_EFN,        /*  Status block and event flag      */
        ASTContinue, Thread             /*  AST address and parameter        */
    );
    CheckStatus (Thread, rc, "ACMS$INIT_EXCHANGE_IO");
}


/************************   ACMS GET PROCEDURE INFO   ************************/

MODULE AcmsGetProcedureInfo (THREAD *Thread)
{
    static dsc_descriptor_s
        TaskName = {
            "AETKTASK", DSC_K_DTYPE_T, DSC_K_CLASS_S, 8
        },
        ApplName = {
            "AETKAPPL", DSC_K_DTYPE_T, DSC_K_CLASS_S, 8
        };

    Data = Thread-> Data;               /*  Address user context block       */

    rc = acms_get_procedure_info_a (
        &Data-> SubmitterId,            /*  Submitter ID                     */
        &TaskName, &ApplName,           /*  Task name and application name   */
        &Data-> ProcList,               /*  Item list                        */
        &Data-> acsb, &ACMS_EFN,        /*  Status block and event flag      */
        ASTContinue, Thread             /*  AST address and parameter        */
    );
    CheckStatus (Thread, rc, "ACMS$GET_PROCEDURE_INFO");
}


/****************************   ACMS START CALL   ****************************/

MODULE AcmsStartCall (THREAD *Thread)
{
    Data = Thread-> Data;               /*  Address user context block       */

    rc = acms_start_call_a (
        &Data-> SubmitterId,            /*  Submitter ID                     */
        &Data-> ProcedureId,            /*  Procedure ID                     */
        &Data-> CallerId,               /*  Call ID                          */
        &Data-> CallList,               /*  Call arguments                   */
        0,                              /*  Transaction id                   */
        &Data-> acsb, &ACMS_EFN,        /*  Status block and event flag      */
        ASTContinue, Thread             /*  AST address and parameter        */
    );
    CheckStatus (Thread, rc, "ACMS$START_CALL");
}


/*-----------------------------.
 |  CheckStatus                |
 |-----------------------------'---------------------------------------------.
 |  Boolean CheckStatus (THREAD *Thread, long rc, char *Reason);             |
 |                                                                           |
 |  If the return-code in rc is even, prints the Reason text and raises an   |
 |  io_error event, then passes control to ASTContinue.  Returns FALSE if    |
 |  if found an error: in this case, do NOT call ASTContinue again or you    |
 |  will clone the thread.  Stores Reason text in Data-> Reason for later    |
 |  use if necessary.                                                        |
 '---------------------------------------------------------------------------*/

static Boolean CheckStatus (THREAD *Thread, long rc, char *Reason)
{
    strncpy (Data-> Reason, Reason, sizeof (Data-> Reason) - 1);
    if (rc & 1)                         /*  If no errors, return with        */
        return (TRUE);                  /*    okay indicator                 */

    /*  Else print error message and abort the thread                        */
    printf ("%s failed: %s\n", Reason, SysMsg (rc));
    RaiseException (Thread, io_error_event);
    ASTContinue (Thread);
    return (FALSE);
}

  Note that  each module  branches to ASTContinue either indirectly via an AST,
or explicitly  via a  call.   If the  asynchronous function  call fails for any
reason, rc is set, and CheckStatus traps this.  It then raises an exception and
passes control  back to  the dialog  engine.   The dialog's  exception handling
mechanism then takes over.

  The overhead  compared to  a conventional  AST program  is slight:  one  more
function call  and some stepping through tables in memory.  The benefits of the
abstraction are  major: one  can write a dialog that reuses the same AST module
in several places, without changing the module's code.
