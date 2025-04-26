/*=========================================================================*
 *                                                                         *
 *  stripper.c  C/C++ comment stripper                                     *
 *                                                                         *
 *  Written:    95/03/30  Pieter Hintjens <ph@imatix.com>                  *
 *  Revised:    96/05/15  Frans Janssens <frans@sebb.bel.alcatel.be>       *
 *                                                                         *
 *=========================================================================*/

#include "prelude.h"                    /*  Standard include file          */
#include "stripper.d"                   /*  Include dialog data            */


/*- Global variables used in this source file only ------------------------*/

static int
    thisch,                             /*  Current character              */
    comment_level,                      /*  Comment nesting level          */
    feedback;                           /*  Feedback for calling program   */

int main (void)
{
    feedback = 0;                       /*  No errors so far               */
#   include "stripper.i"                /*  Include dialog interpreter     */
}

/************************   INITIALISE THE PROGRAM   ***********************/

MODULE initialise_the_program (void)
{
    the_next_event = ok_event;
}

/*************************   GET CHAR FROM INPUT   *************************/

MODULE get_char_from_input (void)
{
    thisch = getchar ();
    switch (thisch)
      {
        case '"':   the_next_event = quote_event;         break;
        case '/':   the_next_event = slash_event;         break;
        case '*':   the_next_event = star_event;          break;
        case '\n':  the_next_event = newline_event;       break;
        case EOF:   the_next_event = end_of_file_event;   break;
        default:    the_next_event = other_event;
      }
}

/**************************   PUT CHAR TO OUTPUT   *************************/

MODULE put_char_to_output (void)
{
    putchar (thisch);
}

/*************************   PUT SLASH TO OUTPUT   *************************/

MODULE put_slash_to_output (void)
{
    putchar ('/');
}

/***************************   START NEW COMMENT   ***************************/

MODULE start_new_comment (void)
{
    comment_level++;                    /*  We have a new comment          */
}

/************************   OTHER IF NESTED COMMENT   ************************/

MODULE other_if_nested_comment (void)
{
    comment_level--;                    /*  We've finished a comment       */
    if (comment_level > 0)              /*    but still be in one...       */
        raise_exception (other_event);
}

/*********************   SIGNAL UNTERMINATED COMMENT   *********************/

MODULE signal_unterminated_comment (void)
{
    fprintf (stderr, "stripper: unterminated comment\n");
    feedback = 1;                       /*  Return code for main program   */
}

/***************************   GET EXTERNAL EVENT   ************************/

MODULE get_external_event (void)
{
}

/************************   TERMINATE THE PROGRAM   ************************/

MODULE terminate_the_program (void)
{
    the_next_event = terminate_event;
}
