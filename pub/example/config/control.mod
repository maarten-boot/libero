#==============================================================================
#-
#-  monitor.mod - module file for monitor.l
<help>
#
#   control     Example of configuration management script
#
#   Written:    95/10/22  Pieter Hintjens
#   Revised:    95/10/22
#
#   Syntax:     control program operation
#
#               operation   One of checkin / checkout / delete / integ /
#                           browse / undelete / ...
#
#               Returns 0 if the operation is allowed, 1 if not.
<end>
#-  Skeleton generated by LIBERO 2.10 on 22 Oct, 1995, 12:00.
#==============================================================================

#   Put definitions here that will go at the start of the script
<data>
DATABASE=$CONTROL\CONTROL.dat
<end>

<code>

set_event ()
{
    if   test "$1" = "browse"; then
        return $browse_event
    elif test "$1" = "checkin"; then
        return $checkin_event
    elif test "$1" = "checkout"; then
        return $checkout_event
    elif test "$1" = "compile"; then
        return $compile_event
    elif test "$1" = "delete"; then
        return $delete_event
    elif test "$1" = "edit"; then
        return $edit_event
    elif test "$1" = "integrate"; then
        return $integrate_event
    elif test "$1" = "request"; then
        return $request_event
    elif test "$1" = "undelete"; then
        return $undelete_event
    else
        echo control: no operation $1 defined
        return $error_event
    fi
}


##########################   INITIALISE THE PROGRAM   #########################

function initialise_the_program
{
    #   Read state of program from CONTROL.dat; if not found, we set Ok;
    #   if there is an error we set error and display a message; if found
    #   we set LR_state to the appropriate state number, and call the
    #   set_event function to set an appropriate next event for the
    #   operation specified as argument.

    #   $1 is program name; $2 is operation name

    PROG=`echo $1 | tr "[a-z]" "[A-Z]"`
    OPERATION=`echo $2 | tr "[A-Z]" "[a-z]"`

    #  CONTROL.dat is in the $CONTROL directory.  Each line contains the
    #  program name, then state name, and possible other info afterwards.

    STATE=`egrep "^$PROG" $DATABASE | cut -f1 -d" "`

    if test "$STATE" = ""; then
        the_next_event=$ok_event
    else
        LR_state=`eval echo '$LR_STATE_'$STATE`
        START_STATE=$LR_state           #   Original state for program
        set_event $OPERATION
        the_next_event=$?
    fi
}


############################   DEFINE NEW PROGRAM   ###########################

function define_new_program
{
    #   Define new program in database
    return
}


#############################   LOCK THE PROGRAM   ############################

function lock_the_program
{
    #   Copy program to developer's directory, lock in central directory
    return
}


############################   UNLOCK THE PROGRAM   ###########################

function unlock_the_program
{
    #   Return program to central directory, if changed, and ask developer
    #   to give reason for changes.
    return
}


############################   BROWSE THE PROGRAM   ###########################

function browse_the_program
{
    #   Call editor in read-only mode
    return
}


###########################   COMPILE THE PROGRAM   ###########################

function compile_the_program
{
    #   Compile & link program
    return
}


#############################   EDIT THE PROGRAM   ############################

function edit_the_program
{
    #   Call editor in read-write mode
    return
}


############################   DELETE THE PROGRAM   ###########################

function delete_the_program
{
    #   Move program to central 'deleted' directory
    return
}


###########################   UNDELETE THE PROGRAM   ##########################

function undelete_the_program
{
    #   Move program to central source directory
    return
}


##########################   TRIGGER THE SAME EVENT   #########################

function trigger_the_same_event
{
    #   Re-set the-next-event by calling SET_EVENT

    set_event $OPERATION
    the_next_event=$?
}


#############################   REJECT OPERATION   ############################

function reject_operation
{
    echo 'control: $OPERATION is not allowed on $PROG'
    echo 'control: check administrator for details'
    feedback=1                          #   Return code to caller; reject
}


############################   GET EXTERNAL EVENT   ###########################

function get_external_event
{
    #   This module is called before we get to the next state, except in
    #   the cases where we do a 'trigger-the-same-event'.
    #   We update the program's state in CONTROL.dat.

    if [ $START_STATE -ne $LR_state ] ; then
        STATE=`eval echo '$LR_STATE_NAME_'$LR_state`
        gate $DATABASE \
        "egrep -v ^$PROG $DATABASE>tmp;cat $PROG $STATE>>tmp;mv tmp $DATABASE"
    fi

    #   Set the next event to terminate_event - this ends the dialog.

    the_next_event=$terminate_event
}


#########################   SIGNAL INVALID OPERATION   ########################

function signal_invalid_operation
{
    echo "control: invalid operation code " $OPERATION " - rejected"
}


##########################   TERMINATE THE PROGRAM    #########################

function terminate_the_program
{
    the_next_event=$terminate_event
}
