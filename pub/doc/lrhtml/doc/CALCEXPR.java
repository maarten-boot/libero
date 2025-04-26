/*  ----------------------------------------------------------------<Prolog>-
    Name:       calcexpr.java
    Title:      Expression parser applet

    Written:    96/06/26  Pascal Antonnaux <pascal@imatix.com>
    Revised:    96/07/14

    Copyright:  Copyright (c) 1991-1996 iMatix
    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2 of the License, or (at your option)
    any later version. This program is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.  You should have received a copy of the
    GNU General Public License along with this program; if not, write to the
    Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 ------------------------------------------------------------------</Prolog>-*/

import java.awt.*;
import java.applet.*;

public class calcexpr extends Applet
{
    //  Applet members
    private calcul
        calc = new calcul ();
    Label
        label1,
        label2,
        label3,
        Result;
    TextField
        expression;

    // Applet methods
    public void init ()
    {
        super.init();

        setLayout (null);
        resize (341, 130);

        label1 = new Label ("Libero JAVA example - Expression Evaluation");
        label1.setFont (new Font ("Dialog", Font.BOLD, 10));
        add (label1);
        label1.reshape (0, 0, 280, 15);

        label2 = new Label ("Enter a expression (i.e. : (2 + 3) * (4 ** 2) )");
        label2.setFont (new Font ("Dialog", Font.PLAIN, 10));
        add (label2);
        label2.reshape (0, 23, 266, 15);

        label3 = new Label ("and press the ENTER key to evaluate it:");
        label3.setFont (new Font ("Dialog", Font.PLAIN, 10));
        add (label3);
        label3.reshape (0, 45, 266, 15);

        expression = new TextField (32);
        expression.setFont (new Font ("Dialog", Font.PLAIN, 10));
        add ("West", expression);
        expression.reshape (9, 69, 266, 23);

        Result = new Label ("");
        Result.setFont (new Font ("Dialog", Font.BOLD, 10));
        add (Result);
        Result.reshape (0, 99, 329, 15);

        expression.requestFocus ();
    }


    public boolean handleEvent (Event event)
    {
        if (event.id == Event.ACTION_EVENT)
          {
            evaluate ();
            return (true);
          }
        return super.handleEvent (event);
    }


    private void evaluate ()
    {
        String
            exp;
        int
            feedback;

        exp = expression.getText ();
        feedback = calc.parse (exp);

        if (feedback == 0)
            Result.setText ("Result : " + Double.toString(calc.result));
        else
          {
            Result.setText (calc.err_str);
            expression.select (calc.err_pos, calc.err_pos + 1);
          }
    }
}
