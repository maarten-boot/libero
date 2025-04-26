/* -------------------------------------------------------------------------
 *  gate    - serialise access to a command
 *
 *  Written:  95/05/10  Pieter Hintjens
 *  Revised:  95/10/22  Pieter Hintjens
 *
 *  This command restricts access to a command to one user at a time,
 *  system wide.  Use this to serialise access to files, programs, or
 *  do other tasks that cannot be run by several users at the same time.
 *
 *  Gate uses the semaphore functions to create and manage semaphores at
 *  the system level.  Each semaphore is named after a file, which much
 *  exist when the semaphore is created or used.
 *
 *  Semaphores are created and initialised automatically the first time
 *  they are used.  Gate also provides 4 functions to manage semaphores:
 *  create, delete, reset, and view.
 *
 *  For help, run gate without arguments.
 * ------------------------------------------------------------------------*/

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

/*  Symbolic constant definitions                                          */

#define  NBR_SEMAPHORES        1    /*  How many semaphores do we need     */
#define  APPLICATION_KEY       1    /*  Application key (arbitrary value)  */

#define  SEMAPHORE_INITIAL     1    /*  Initial semaphore value            */
#define  SEMAPHORE_REQUEST    -1    /*  Operation code to request access   */
#define  SEMAPHORE_RELEASE     1    /*  Operation code to release access   */

/*  Local function prototypes                                              */

static int  SemLookup    (char *filename);
static int  SemCreate    (char *filename);
static void SemReport    (int Handle);
static void SemReset     (int Handle);
static void SemDelete    (int Handle);
static void SemWait      (int Handle);
static void SemSignal    (int Handle);
static void FatalError   (char *format, ...);
static void IgnoreAbort  (int Signal);


int main (int argn, char *argv[])
{
    int Semaphore;

    if (argn != 3)
      {
        puts ("");
        puts (" gate - serialise access to a command");
        puts ("");
        puts (" Author:  Pieter Hintjens");
        puts ("");
        puts (" Use gate to control access to a file so that only one");
        puts (" process can ever access it at once.  The file must exist,");
        puts (" and be accessible to the process.");
        puts ("");
        puts (" gate filename \"command\"");
        puts ("");
        puts ("     filename  - full name of file to lock");
        puts ("     command   - command to execute:");
        puts ("                 -create = create the semaphore");
        puts ("                 -delete = delete the semaphore");
        puts ("                 -reset  = reset semaphore value");
        puts ("                 -show   = show semaphore value");
        puts ("                 other   = Unix command to run");
        puts ("");
        puts (" You can use a lock file to serialise an arbitrary command.");
        puts (" Each access creates a semaphore at system level, so it is");
        puts (" not a good idea to use gate on many files; instead use it");
        puts (" on one or a few central files.");
        puts ("");
        exit (0);
      }

    Semaphore = SemLookup (argv [1]);

    /*  If -show specified, show semaphore value and exit                  */
    if (strcmp (argv [2], "-show") == 0)
      {
        if (Semaphore == -1)
            fprintf (stderr, "gate: semaphore for %s not found\n", argv [1]);
        else
            SemReport (Semaphore);

        exit (0);
      }
    if (strcmp (argv [2], "-create") == 0)
      {
        if (Semaphore == -1)
          {
            Semaphore = SemCreate (argv [1]);
            SemReset (Semaphore);
          }
        else
            fprintf (stderr, "gate: semaphore for %s exists\n", argv [1]);

        exit (0);
      }

    Semaphore = SemCreate (argv [1]);
    if (semctl (Semaphore, 0, GETVAL, 0) > SEMAPHORE_INITIAL)
        SemReset (Semaphore);       /*  If value is invalid, reset it      */

    /*  If -reset specified, reset semaphore and exit                      */
    if (strcmp (argv [2], "-reset") == 0)
      {
        SemReset (Semaphore);
        printf ("gate: semaphore for %s value reset to %d\n",
                 argv [1], SEMAPHORE_INITIAL);
        exit (0);
      }
    /*  If -delete specified, delete semaphore and exit                    */
    if (strcmp (argv [2], "-delete") == 0)
      {
        SemDelete (Semaphore);
        printf ("gate: semaphore for %s deleted\n", argv [1]);
        exit (0);
      }

    SemWait (Semaphore);            /*  Request semaphore & wait           */
    system (argv [2]);              /*  Execute command                    */
    SemSignal (Semaphore);          /*  Release semaphore                  */
}

static int SemLookup (char *filename)
{
    key_t FileKey;
    int   Handle;

    /*  Get unique key for file                                            */
    FileKey = ftok (filename, APPLICATION_KEY);
    if (FileKey == -1)
        FatalError ("gate: file %s not found, or protected", filename);

    /*  Look for handle for existing semaphore                             */
    Handle = semget (FileKey, NBR_SEMAPHORES, 0);
    return (Handle);
}

static int SemCreate (char *filename)
{
    key_t FileKey;
    int   Handle;

    /*  Get unique key for file                                            */
    FileKey = ftok (filename, APPLICATION_KEY);
    if (FileKey == -1)
        FatalError ("gate: file %s not found, or protected", filename);

    /*  Look for handle for existing semaphore                             */
    Handle = semget (FileKey, NBR_SEMAPHORES, 0);

    /*  If not found, create semaphore and initialise value                */
    if (Handle == -1)
      {
        Handle = semget (FileKey, 1, IPC_CREAT + S_IRUSR + S_IWUSR
                           + S_IRGRP + S_IWGRP + S_IROTH + S_IWOTH);
        if (Handle == -1)
            FatalError ("gate: cannot create semaphore for %s", filename);

        SemReset (Handle);          /*  Initialise value after creating    */
      }
    return (Handle);
}

static void SemReport (int Handle)
{
    int rc;                         /*  General-purpose return code        */

    rc = semctl (Handle, 0, GETVAL, 0);
    if (rc == -1)
        FatalError ("gate: semaphore not found");

    printf ("gate: semaphore value is %d\n", rc);
}

static void SemReset (int Handle)
{
    int rc;                         /*  General-purpose return code        */

    rc = semctl (Handle, 0, SETVAL, SEMAPHORE_INITIAL);
    if (rc == -1)
        FatalError ("gate: cannot reset semaphore value");
}

static void SemDelete (int Handle)
{
    int rc;                         /*  General-purpose return code        */

    rc = semctl (Handle, 0, IPC_RMID, 0);
    if (rc == -1)
        FatalError ("gate: cannot delete semaphore");
}

static void SemWait (int Handle)
{
    int rc;                         /*  General-purpose return code        */
    struct sembuf RequestOp = { 0, SEMAPHORE_REQUEST, 0 };

    signal (SIGABRT, IgnoreAbort);
    signal (SIGINT,  IgnoreAbort);
    signal (SIGTERM, IgnoreAbort);
    signal (SIGSEGV, IgnoreAbort);

    rc = semop (Handle, &RequestOp, 1);
    if (rc == -1)
        FatalError ("gate: cannot request semaphore");
}

static void SemSignal (int Handle)
{
    int rc;                         /*  General-purpose return code        */
    struct sembuf ReleaseOp = { 0, SEMAPHORE_RELEASE, 0 };

    rc = semop (Handle, &ReleaseOp, 1);
    if (rc == -1)
        FatalError ("gate: cannot release semaphore");
}

/*  Signal fatal error and halt with exit code 1                           */

static void FatalError (char *format,...)
{
    va_list argptr;

    va_start (argptr, format);
    vfprintf (stderr, format, argptr);
    fprintf  (stderr, ": %s\n", strerror (errno));
    va_end   (argptr);
    exit (1);
}

static void IgnoreAbort (int Signal)
{
    fprintf (stderr, "gate: interrupted");
}
