!============================================================================*
!                                                                            *
!   lrschast.c  LIBERO code generation schema for AST-driven programs        *
!               See lrschema.c for action codes syntax.                      *
!                                                                            *
!   Written:    93/05/20  Pieter Hintjens                                    *
!   Revised:    95/12/07                                                     *
!                                                                            *
!   Usage:      See description below and comments in code.  Creates         *
!               only one file, $dialog.d, containing all generated code.     *
!               Compile with /define=DEBUG to switch-on debugging code.      *
!                                                                            *
!   main ()                                                                  *
!       Data = (CONTEXT *) malloc (sizeof (CONTEXT));                        *
!       sys_dclast (ASTInitialise, Data);                                    *
!       sys_sleep ()                                                         *
!                                                                            *
!   initialise_the_thread (THREAD *Thread)                                   *
!       Thread-> the_next_event = ok_event;                                  *
!                                                                            *
!   terminate_the_thread (THREAD *Thread)                                    *
!       free (Thread-> Data);                                                *
!       Thread-> the_next_event = terminate_event;                           *
!       ASTContinue (Thread);                                                *
!                                                                            *
!   ModuleX (THREAD *Thread)                                                 *
!       Do whatever work...                                                  *
!       Access data in Thread-> Data->                                       *
!       Thread-> the_next_event = whatever_event;                            *
!       Either do asynch call:                                               *
!           sys_qio (......, ASTContinue, Thread,...);                       *
!       OR do (especially if qio failed):                                    *
!           ASTContinue (Thread);                                            *
!                                                                            *
!   Author:     Pieter A. Hintjens                                           *
!               Pijlstraat 9                                                 *
!               2060 Antwerpen, Belgium                                      *
!               ph@mymail.com                                                *
!               (+323) 231.5277                                              *
!                                                                            *
!   FSM Code Generator.  Copyright (c) 1991-95 Pieter A. Hintjens.           *
!                                                                            *
!   This program is free software; you can redistribute it and/or modify     *
!   it under the terms of the GNU General Public License as published by     *
!   the Free Software Foundation; either version 2 of the License, or        *
!   (at your option) any later version.                                      *
!                                                                            *
!   This program is distributed in the hope that it will be useful,          *
!   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
!   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
!   GNU General Public License for more details.                             *
!                                                                            *
!   You should have received a copy of the GNU General Public License        *
!   along with this program; if not, write to the Free Software              *
!   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
!============================================================================*
:if not exist $source.c
:echo "lr I: creating skeleton program $source.c..."
:output $source.c
:declare string me = "Your Name"
/*===========================================================================*
 *                                                                           *
 *  $source     description...                                               *
 *                                                                           *
 *  Written:    $date       $author                                          *
 *  Revised:    $date                                                        *
 *                                                                           *
 *  Skeleton generated by LIBERO $version on $fulldate, $time.               *
 *===========================================================================*/

#include "prelude.h"                    /*  Public definitions               */

/*- Structure definitions ---------------------------------------------------*/

typedef struct {                        /*  Thread's context block:          */
    int  rc;                            /*    Function return code           */
    long Channel;                       /*    Channel for i/o                */
    IOSB iosb;                          /*    i/o status block               */
} CONTEXT;

#include "$dialog.d"                    /*  Include dialog generated code    */

/*- Local function prototypes -----------------------------------------------*/


/*********************************   MAIN   **********************************/

int main (void)
{
    CONTEXT *Context;

    /*  Do process-wide initialisations here                                 */

    Context = (CONTEXT *) malloc (sizeof (CONTEXT));
    ASTInitialise (Context);
    sys_hiber ();
    return (RET_OKAY);
}


/*************************   INITIALISE THE THREAD   *************************/

MODULE $"initialise_the_thread" (THREAD *Thread)
{
/*  Do thread-wide initialisations here                                      */
    Thread-> Data-> Channel = 0;
    Thread-> $"the_next_event" = $"ok_event";
}


/*************************   TERMINATE THE THREAD   **************************/

MODULE $"terminate_the_thread" (THREAD *Thread)
{
    Thread-> $"the_next_event" = $"terminate_event";
    free (Thread-> Data);
    ASTContinue (Thread);
}

:close
:endif

:output $dialog.d
:echo "lr I: building $dialog.d..."
/*---------------------------------------------------------------------------*
 *  $dialog.d - LIBERO dialog data definitions for $source.                  *
 *  Generated by LIBERO $version on $fulldate, $time.                        *
 *  Schema file used: $schema                                                *
 *---------------------------------------------------------------------------*/

/*- Symbolic constants and event numbers ------------------------------------*/

:if check
#define _LR_STOP_ACTION                 0xFFFFL
#define _LR_NULL_EVENT                  -2
:endif
#define $"terminate_event"              -1
:do event
#define $name                           $number
:enddo
:do state
#define _LR_STATE_$name                 $number
:enddo

/*- Definition of context block for thread ----------------------------------*/

typedef struct {                        /*  Context for thread               */
    int
        _LR_event,                      /*  Event for state transition       */
        _LR_state,                      /*  Current dialog state             */
        _LR_savest,                     /*  Saved dialog state               */
        _LR_index,                      /*  Index into vector table          */
        $"the_next_event",              /*  Next event from module           */
        $"the_exception_event";         /*  Exception event from module      */
    word
        *_LR_vecptr;                    /*  Pointer into vector table        */
    Bool
        $"animation_enabled",           /*  If TRUE, animation switched on   */
        $"exception_raised";            /*  TRUE if exception raised         */
    CONTEXT
        *Data;                          /*  Pointer to user data area        */
} THREAD;

/*- Function prototypes -----------------------------------------------------*/

#ifndef MODULE
#define MODULE  static void             /*  Libero dialog modules            */
#endif

local  _LR_before_actions               (THREAD *Thread);
local  _LR_after_actions                (THREAD *Thread);
local  ASTInitialise                    (CONTEXT *Data);
local  ASTContinue                      (THREAD *Thread);
local  $"raise_exception"               (THREAD *Thread, int event);
MODULE $"initialise_the_thread"         (THREAD *Thread);
:do module
MODULE $name                            (THREAD *Thread);
:enddo

/*- Static areas shared by all threads --------------------------------------*/

static word _LR_nextst [][$events] =
{
:do nextst
:  if $overflow = 0
    { $row }$comma
:  else
    { $row
:    do overflow
     $row
:    enddo
    }$comma
:  endif
:enddo
};

static word _LR_action [][$events] =
{
:set array_base=1
:do action
:  if $overflow = 0
    { $row }$comma
:  else
    { $row
:    do overflow
     $row
:    enddo
    }$comma
:  endif
:enddo
:set array_base=0
};

static word _LR_vector [][$maxaction+1] = {
    {0},
:   do vector
    {$row,0xFFFFL}$comma
:   enddo
};

typedef void (*ASTfunction) (THREAD *);
static ASTfunction _LR_module [] = {
:   do module
    $name$comma
:   enddo
};

:if animate
#ifdef DEBUG
static char *_LR_mname [$modules] = {
:    do module
     "$name"$comma
:    enddo
};

static char *_LR_sname [$states] = {
:    do state
     "$name"$comma
:    enddo
};

static char *_LR_ename [$events+2] = {
    "(NULL)",
    "Terminate",
:    do event
     "$name"$comma
:    enddo
};
#endif
:endif

!   _LR_before_actions handles the event in the state.  If the event is not
!   valid, aborts with an error message.  The terminate_event signals the end
!   of the thread; the dialog releases its context and does a normal return
!   without specifying a new AST routine.
!
local _LR_before_actions (THREAD *Thread)
{
:   if animate
#   ifdef DEBUG
    if (Thread-> $"animation_enabled")
        fprintf (stderr, "_LR_before_actions state=%s event=%s\n",
                 _LR_sname [Thread-> _LR_state],
                 _LR_ename [Thread-> $"the_next_event" + 2]);
#   endif
:   endif
    if (Thread-> $"the_next_event" == $"terminate_event") {
        free (Thread);                  /*  Release thread context           */
        return;                         /*    and give-up AST control        */
    }
:   if check
    if ((Thread-> $"the_next_event" >= $events)
    OR  (Thread-> $"the_next_event" < 0)) {
        fprintf (stderr, "Invalid event %d in state %d\n",
                 Thread-> $"the_next_event", Thread-> _LR_state);
        sys_exit (0);                   /*  Fatal error in dialog            */
    }
:   endif
    Thread-> _LR_event  = Thread-> $"the_next_event";
    Thread-> _LR_index  = _LR_action [Thread-> _LR_state]
                                     [Thread-> _LR_event];
    Thread-> _LR_savest = Thread-> _LR_state;
:   if defaults
    /*  If no action for this event, try the defaults state                  */
    if (Thread-> _LR_index == 0) {
        Thread-> _LR_state = $defaults;
        Thread-> _LR_index = _LR_action [Thread-> _LR_state]
                                        [Thread-> _LR_event];
    }
:   endif
:   if check
    if (Thread-> _LR_index == 0) {
        fprintf (stderr, "Bad event %d in state %d\n",
                 Thread-> _LR_event, Thread-> _LR_savest);
        sys_exit (0);                   /*  Fatal error in dialog            */
    }
    Thread-> $"the_next_event"      = _LR_NULL_EVENT;
:   endif
    Thread-> $"the_exception_event" = _LR_NULL_EVENT;
    Thread-> $"exception_raised"    = FALSE;
    Thread-> _LR_vecptr = _LR_vector [Thread-> _LR_index];
    ASTContinue (Thread);
}

!   Handles exceptions and events after the action list finishes.  If an
!   exception occurred, sets the appropriate new state.  Chains back to
!   _LR_before_actions to continue in the new state.
!
local _LR_after_actions (THREAD *Thread)
{
:   if animate
#   ifdef DEBUG
    if (Thread-> $"animation_enabled")
        fprintf (stderr, "_LR_after_actions state=%s event=%s exception=%d\n",
                 _LR_sname [Thread-> _LR_state],
                 _LR_ename [Thread-> $"the_next_event" + 2],
                 Thread-> $"exception_raised");
#   endif
:   endif
    if (Thread-> $"exception_raised") {
        if (Thread-> $"the_exception_event" != _LR_NULL_EVENT)
            Thread-> _LR_event = Thread-> $"the_exception_event";
        Thread-> $"the_next_event" = Thread-> _LR_event;
    } else
        Thread-> _LR_state = _LR_nextst [Thread-> _LR_state]
                                        [Thread-> _LR_event];
:   if check
    if (Thread-> $"the_next_event" == _LR_NULL_EVENT) {
        fprintf (stderr, "No next event set after event %d in state %d\n",
                 Thread-> _LR_event, Thread-> _LR_savest);
        sys_exit (0);                   /*  Fatal error in dialog            */
    }
:   endif
:   if defaults
    if (Thread-> _LR_state == $defaults)
        Thread-> _LR_state = Thread-> _LR_savest;
:   endif
    _LR_before_actions (Thread);        /*  Do main state-machine loop       */
}

/*- Standard dialog routines ------------------------------------------------*/

!   ASTInitialise: create new thread and start up dialog state machine.
!   Call this from the mainline code or from another AST module when you
!   want to spawn a thread.  Note that initialise_the_thread is run in AST
!   mode so should not do any synchronous i/o or heavy processing.
!   You must typedef a struct CONTEXT, and allocate a block in your main
!   program.  Pass this block to ASTInitialise, who sticks into the Thread.
!
local ASTInitialise (CONTEXT *Data)
{
    THREAD *Thread;

    Thread = (THREAD *) malloc (sizeof (THREAD));
    if (Thread == NULL) {
        fprintf (stderr, "LIBERO: Out of memory error.\n");
        return;                         /*  Abort if no memory available     */
    }
    Thread-> $"animation_enabled" = FALSE;
:   if animate
#   ifdef DEBUG
    if (Thread-> $"animation_enabled")
        fprintf (stderr, "ASTInitialise\n");
#   endif
:   endif
    Thread-> Data = Data;               /*  Attach user context to Thread    */
    Thread-> _LR_state = 0;             /*  First state is always 0          */
    $"initialise_the_thread" (Thread);  /*  Call user initialisations        */
    _LR_before_actions (Thread);        /*    and start with state machine   */
}

!   Executes the next action for the state-event transition.  If the action
!   list is finished, or the previous action raised an exception, halts and
!   steps to _LR_after_actions.  Note that the action lists always end in a
!   _LR_STOP_ACTION value (0xFFFFL).  Empty action lists are valid here but
!   will trigger an error in _LR_after_actions (no event set).  Every dialog
!   module (except initialise_the_thread) must either nominate ASTContinue
!   as the successor for an asynch i/o, or call it directly.
!
local ASTContinue (THREAD *Thread)
{
    if ((*Thread-> _LR_vecptr == _LR_STOP_ACTION)
    OR   (Thread-> $"exception_raised"))
        _LR_after_actions (Thread);
    else {
:       if animate
#       ifdef DEBUG
        if (Thread-> $"animation_enabled") {
            fprintf (stderr, "ASTContinue state=%s module=%s\n",
                     _LR_sname [Thread-> _LR_state],
                     _LR_mname [*Thread-> _LR_vecptr]);
        }
#       endif
:       endif
        (*_LR_module [*Thread-> _LR_vecptr++]) (Thread);
    }
}

local $"raise_exception" (THREAD *Thread, int event)
{
    Thread-> $"exception_raised" = TRUE;
    if (event >= 0)
        Thread-> $"the_exception_event" = event;
}
:close

!   Generate stubs for all modules not yet defined in source file            *

:set module_line="MODULE %s (THREAD *Thread)"
:internal initialise_the_thread
:do stubs

MODULE $name (THREAD *Thread)
{
    ASTContinue (Thread);
}

:enddo
