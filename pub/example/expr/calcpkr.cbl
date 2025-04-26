000100 01  CALCPK-CONTROL.                                              POS00001
000200*CONTENTS                                                         95/04/26
000300     02  CALCPK-OPERATION        PIC X.                           95/04/26
000400     02  CALCPK-FEEDBACK         PIC X.                           95/04/26
000500     02  CALCPK-POINT-CHAR       PIC X.                           95/04/26
000600     02  CALCPK-ERROR-RETURN.                                     95/04/26
000700         03  CALCPK-ERROR-CODE   PIC X(2).                        95/04/26
000800         03  CALCPK-ERROR-MESG   PIC X(30).                       95/04/26
000900     02  CALCPK-ERROR-POSN       PIC 9(2).                        95/04/26
001000     02  CALCPK-EXPRESSION.                                       95/04/26
001100         03  EXPR-CHAR           PIC X          OCCURS 80 TIMES.  95/04/26
001200     02  CALCPK-RESULT           PIC S9(9)V9(9).                  95/04/26
001300     02  CALCPK-VARIABLE         PIC S9(9)V9(9) OCCURS 5 TIMES.   95/04/26