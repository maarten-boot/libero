#include "prelude.h"                    /*  All standard includes            */
#include "pctfile.h"
#include "pctpict.h"

static void Test_picture (char *string);

void main
(int argc, char **argv)
{
    FSD  *fsd;
    char Line [LINE_MAX + 1];
    int
	Chars_read;

    if (argc > 1)
      {
	Test_picture (argv [1]);
	exit (0);
      }

    fsd  = File_open ('r', "picture.tst", 0);
    if (fsd == NULL)
      {
        puts ("picture.tst not found");
	exit (1);
      }
    for ever
      {
	Chars_read = File_read_line (fsd, Line);
	if (Chars_read == EOF)
	    break;

        if (Line [0] == '!')
	    puts (Line);
	else
        if (Line [0] != ' ' AND Line [0] != 0)
	    Test_picture (Line);
      }
}


static void
Test_picture (char *Line)
{
    static PICTURE_CONTROL
	Picture;
    int Feedback;

    Picture.Picture	= Line;
    Picture.Comma_char  = ',';
    Picture.Point_char  = '.';
    Picture.Dollar_char = '$';

    Feedback = Pic_analyse (&Picture);
    printf ("%-28s: ", Line);
    printf ("%-20s ", Picture.Cleaned);

    printf ("size=%-4d ", Picture.Size);
    if (Picture.Type == PIC_TYPE_UNDEFINED)
        printf ("undefined");
    else
    if (Picture.Type == PIC_TYPE_CHARACTER)
        printf ("character");
    else
    if (Picture.Type == PIC_TYPE_NUMERIC)
        printf ("numeric  ");
    else
    if (Picture.Type == PIC_TYPE_ALPHANUM)
        printf ("alphanum ");
    else
    if (Picture.Type == PIC_TYPE_EDITED)
        printf ("edited   ");
    else
        printf ("!error   ");

    if (Picture.Decimals)
        printf ("%d decs ", Picture.Decimals);

    if (Picture.Flags & PIC_FLAG_NEG)
        printf ("- ");
    else
    if (Picture.Flags & PIC_FLAG_POS)
        printf ("+ ");
    else
    if (Picture.Flags & PIC_FLAG_SIGNED)
        printf ("signed ");

    if (Picture.Flags & PIC_FLAG_FLOAT)
        printf (">>");

    printf ("\n");
    if (Feedback == PIC_ERROR)
      {
	int Spaces;
	for (Spaces = 1; Spaces < Picture.Position; Spaces++)
            printf (" ");
        printf ("^\n%s\n", Picture.Message);
      }
}
