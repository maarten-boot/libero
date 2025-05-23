000100 IDENTIFICATION DIVISION. 					POS00059
000200 PROGRAM-ID.    PPICT.						96/01/14
000300									96/01/08
000400 AUTHOR.	      LEIF SVALGAARD.					96/01/08
000500 DATE-WRITTEN.  96/01/14						96/01/14
000600	   -REVISED:  96/04/03. 					96/04/03
000700									96/01/08
000800 ENVIRONMENT DIVISION.						96/01/08
000900									96/01/08
001000 CONFIGURATION SECTION.						96/01/08
001100 SOURCE-COMPUTER. PORTABLE.					96/01/08
001200 OBJECT-COMPUTER. PORTABLE.					96/01/08
001300									96/01/08
001400 DATA DIVISION.							96/01/08
001500									96/01/08
001600 WORKING-STORAGE SECTION. 					96/01/08
001700									96/01/08
001800 01  BEGIN-DYNAMIC-STORAGE       PIC X(4)   VALUE "<ETK".         96/01/08
001900									96/01/08
002000 01  SYMBOL-HANDLING.						96/01/14
002100	   02  THE-NUMBER	       PIC S9(9)  COMP. 		96/01/14
002200	   02  NUMERIC-EDITS	       PIC S9(3)  COMP. 		96/01/15
002300	   02  ALPHANUMERIC-EDITS      PIC S9(3)  COMP. 		96/01/14
002400	   02  CHAR-NBR 	       PIC S9(3)  COMP. 		96/01/14
002500	   02  EXPECT-CHAR	       PIC X.				96/01/15
002600	   02  REPEATED-CHAR	       PIC X.				96/01/16
002700	   02  THE-CHAR.						96/01/15
002800	       03  THE-DIGIT	       PIC 9.				96/01/14
002900	   02  THE-DECIMAL-POINT       PIC X.				96/01/16
003000	   02  THE-COMMA-SYMBOL        PIC X.				96/01/16
003100	   02  THE-CURRENCY-SYMBOL     PIC X.				96/01/16
003200									96/01/14
003300*DIALOG MANAGER.							96/01/08
003400 COPY PPICTD.							96/01/14
040300									96/01/08
040400******************    INITIALIZE THE PROGRAM    ******************96/01/08
040500									96/01/08
040600 INITIALIZE-THE-PROGRAM.						96/01/08
040700	   IF PPICT-CURRENCY-SYMBOL > SPACE				96/01/15
040800	       MOVE PPICT-CURRENCY-SYMBOL TO THE-CURRENCY-SYMBOL	96/01/15
040900	   ELSE 							96/01/15
041000         MOVE          "$"          TO THE-CURRENCY-SYMBOL        96/01/15
041100	   .								96/01/15
041200     IF PPICT-DECIMAL-POINT = ","                                 96/01/16
041300         MOVE "," TO THE-DECIMAL-POINT                            96/01/16
041400         MOVE "." TO THE-COMMA-SYMBOL                             96/01/16
041500	   ELSE 							96/01/16
041600         MOVE "." TO THE-DECIMAL-POINT                            96/01/16
041700         MOVE "," TO THE-COMMA-SYMBOL                             96/01/16
041800	   .								96/01/16
041900     MOVE "00" TO PPICT-ERROR-MESSAGE                             96/01/16
042000	   PERFORM SET-OK						96/01/08
042100	   .								96/01/08
042200									96/01/08
042300 SET-OK.								96/01/08
042400	   MOVE OK-EVENT TO THE-NEXT-EVENT				96/01/08
042500	   .								96/01/08
042600									96/01/08
042700*******************    TERMINATE THE PROGRAM    ******************96/01/08
042800									96/01/08
042900 TERMINATE-THE-PROGRAM.						96/01/08
043000	   MOVE TERMINATE-EVENT TO THE-NEXT-EVENT			96/01/08
043100	   .								96/01/08
043200									96/01/14
043300******************    EXECUTE THE OPERATION    *******************96/01/14
043400									96/01/14
043500 EXECUTE-THE-OPERATION.						96/01/14
043600     IF PPICT-OPERATION = "P"                                     96/04/03
043700	       MOVE  SPACE  TO PPICT-FEEDBACK				96/04/03
043800	       PERFORM SET-PICTURE					96/01/14
043900	   ELSE 							96/01/14
044000         MOVE "OPERR" TO PPICT-FEEDBACK                           96/04/03
044100	       PERFORM SET-ERROR					96/01/14
044200	   .								96/01/14
044300									96/01/14
044400 SET-PICTURE.							96/01/14
044500	   MOVE PICTURE-EVENT TO THE-NEXT-EVENT 			96/01/14
044600	   .								96/01/14
044700									96/01/14
044800 SET-ERROR.							96/01/14
044900	   MOVE ERROR-EVENT TO THE-NEXT-EVENT				96/01/14
045000	   .								96/01/14
045100									96/01/14
045200*******************    INIT PICTURE PARSING    *******************96/01/14
045300									96/01/14
045400 INIT-PICTURE-PARSING.						96/01/14
045500	   MOVE ZEROS TO CHAR-NBR					96/01/14
045600	   MOVE SPACE TO PICT-CHAR (40) 				96/01/14
045700									96/01/14
045800	   MOVE ZEROES TO PPICT-ITEM-SIZE				96/01/16
045900			  NUMERIC-EDITS 				96/01/14
046000			  ALPHANUMERIC-EDITS				96/01/14
046100									96/01/16
046200	   MOVE SPACES TO PPICT-ITEM-FLAGS				96/01/16
046300     MOVE  "?"   TO PPICT-ITEM-TYPE                               96/01/16
046400	   .								96/01/14
046500									96/01/14
046600*********************    GET NEXT SYMBOL	  **********************96/01/14
046700									96/01/14
046800 GET-NEXT-SYMBOL. 						96/01/14
046900	   PERFORM GET-NEXT-CHAR					96/01/14
047000     IF THE-CHAR = "X"                                            96/01/14
047100	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
047200	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
047300	       PERFORM SET-LETTER-X					96/01/14
047400	   ELSE 							96/01/14
047500     IF THE-CHAR = "9"                                            96/01/14
047600	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
047700	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
047800	       PERFORM SET-DIGIT-9					96/01/14
047900	       PERFORM COUNT-DIGITS					96/01/16
048000	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
048100	   ELSE 							96/01/14
048200     IF THE-CHAR = "("                                            96/01/14
048300	       PERFORM SET-LEFT-PAR					96/01/14
048400	   ELSE 							96/01/14
048500	   IF THE-CHAR = SPACE						96/01/14
048600	       PERFORM SET-AT-END					96/01/14
048700	   ELSE 							96/01/14
048800     IF THE-CHAR = "S"                                            96/01/14
048900	       MOVE SPACE TO REPEATED-CHAR				96/01/16
049000	       PERFORM SET-SIGNED-FLAG					96/01/16
049100	       PERFORM SET-LETTER-S					96/01/14
049200	   ELSE 							96/01/14
049300     IF THE-CHAR = "V"                                            96/01/14
049400	       MOVE SPACE  TO REPEATED-CHAR				96/01/16
049500	       MOVE ZEROES TO PPICT-DECIMALS				96/01/16
049600	       PERFORM SET-LETTER-V					96/01/14
049700	   ELSE 							96/01/14
049800     IF THE-CHAR = "B"                                            96/01/14
049900	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
050000	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
050100	       ADD 1 TO ALPHANUMERIC-EDITS				96/01/14
050200	       PERFORM SET-LETTER-B					96/01/14
050300	   ELSE 							96/01/14
050400     IF THE-CHAR = "A"                                            96/01/14
050500	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
050600	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
050700	       PERFORM SET-LETTER-A					96/01/14
050800	   ELSE 							96/01/14
050900     IF THE-CHAR = "P"                                            96/01/14
051000	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
051100	       PERFORM SET-LETTER-P					96/01/14
051200	       PERFORM COUNT-DIGITS					96/01/16
051300	   ELSE 							96/01/14
051400     IF THE-CHAR = "/" OR "0"                                     96/01/14
051500	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
051600	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
051700	       ADD 1 TO NUMERIC-EDITS					96/01/14
051800	       ADD 1 TO ALPHANUMERIC-EDITS				96/01/14
051900	       PERFORM SET-SLASH-OR-0					96/01/14
052000	   ELSE 							96/01/14
052100     IF THE-CHAR = "E"                                            96/01/14
052200	       MOVE SPACE TO REPEATED-CHAR				96/01/16
052300         MOVE "F" TO PPICT-ITEM-TYPE                              96/01/16
052400	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
052500	       PERFORM SET-LETTER-E					96/01/14
052600	   ELSE 							96/01/14
052700     IF THE-CHAR = "+" OR "-"                                     96/01/15
052800	       PERFORM SET-SIGN-OR-FLOATING-INSERT			96/01/16
052900	   ELSE 							96/01/14
053000	   IF THE-CHAR = THE-CURRENCY-SYMBOL				96/01/16
053100	       PERFORM SET-EDIT-OR-FLOATING-INSERT			96/01/16
053200	   ELSE 							96/01/14
053300     IF THE-CHAR = "Z" OR "*"                                     96/01/16
053400	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
053500	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
053600	       ADD 1 TO NUMERIC-EDITS					96/01/16
053700	       PERFORM SET-EDIT-SYMBOL					96/01/16
053800	       PERFORM COUNT-DIGITS					96/01/16
053900	   ELSE 							96/01/16
054000	   IF THE-CHAR = THE-COMMA-SYMBOL				96/01/16
054100	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
054200	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
054300	       ADD 1 TO NUMERIC-EDITS					96/01/16
054400	       PERFORM SET-EDIT-SYMBOL					96/01/16
054500	   ELSE 							96/01/16
054600	   IF THE-CHAR = THE-DECIMAL-POINT				96/01/16
054700	       MOVE SPACE  TO REPEATED-CHAR				96/01/16
054800	       MOVE ZEROES TO PPICT-DECIMALS				96/01/16
054900	       ADD 1 TO PPICT-ITEM-SIZE 				96/01/16
055000	       ADD 1 TO NUMERIC-EDITS					96/01/15
055100	       PERFORM SET-POINT					96/01/15
055200	   ELSE 							96/01/15
055300     IF THE-CHAR = "C"                                            96/01/14
055400         MOVE "R" TO EXPECT-CHAR                                  96/01/15
055500	       PERFORM CHECK-FOR-CR-DB-SYMBOL				96/01/15
055600	   ELSE 							96/01/14
055700     IF THE-CHAR = "D"                                            96/01/14
055800         MOVE "B" TO EXPECT-CHAR                                  96/01/15
055900	       PERFORM CHECK-FOR-CR-DB-SYMBOL				96/01/15
056000	   ELSE 							96/01/14
056100	       MOVE SPACE TO REPEATED-CHAR				96/01/16
056200	       PERFORM SET-OTHER					96/01/14
056300	   .								96/01/14
056400									96/01/14
056500 GET-NEXT-CHAR.							96/01/14
056600	   IF CHAR-NBR < 40						96/01/14
056700	       ADD 1 TO CHAR-NBR					96/01/14
056800	       MOVE PICT-CHAR (CHAR-NBR) TO THE-CHAR			96/01/14
056900	   ELSE 							96/01/14
057000	       MOVE SPACE TO THE-CHAR					96/01/14
057100	   .								96/01/14
057200									96/01/14
057300 SET-LETTER-X.							96/01/14
057400	   MOVE LETTER-X-EVENT TO THE-NEXT-EVENT			96/01/14
057500	   .								96/01/14
057600									96/01/14
057700 SET-DIGIT-9.							96/01/14
057800	   MOVE DIGIT-9-EVENT TO THE-NEXT-EVENT 			96/01/14
057900	   .								96/01/14
058000									96/01/14
058100 SET-LEFT-PAR.							96/01/14
058200	   MOVE LEFT-PAR-EVENT TO THE-NEXT-EVENT			96/01/14
058300	   .								96/01/14
058400									96/01/14
058500 SET-AT-END.							96/01/14
058600	   MOVE AT-END-EVENT TO THE-NEXT-EVENT				96/01/14
058700	   .								96/01/14
058800									96/01/14
058900 SET-LETTER-S.							96/01/14
059000	   MOVE LETTER-S-EVENT TO THE-NEXT-EVENT			96/01/14
059100	   .								96/01/14
059200									96/01/14
059300 SET-LETTER-V.							96/01/14
059400	   MOVE LETTER-V-EVENT TO THE-NEXT-EVENT			96/01/14
059500	   .								96/01/14
059600									96/01/14
059700 SET-LETTER-B.							96/01/14
059800	   MOVE LETTER-B-EVENT TO THE-NEXT-EVENT			96/01/14
059900	   .								96/01/14
060000									96/01/14
060100 SET-LETTER-A.							96/01/14
060200	   MOVE LETTER-A-EVENT TO THE-NEXT-EVENT			96/01/14
060300	   .								96/01/14
060400									96/01/14
060500 SET-LETTER-P.							96/01/14
060600	   MOVE LETTER-P-EVENT TO THE-NEXT-EVENT			96/01/14
060700	   .								96/01/14
060800									96/01/14
060900 SET-SLASH-OR-0.							96/01/14
061000	   MOVE SLASH-OR-0-EVENT TO THE-NEXT-EVENT			96/01/14
061100	   .								96/01/14
061200									96/01/14
061300 SET-LETTER-E.							96/01/14
061400	   MOVE LETTER-E-EVENT TO THE-NEXT-EVENT			96/01/14
061500	   .								96/01/14
061600									96/01/14
061700 SET-SIGN-OR-FLOATING-INSERT.					96/01/16
061800	   ADD 1 TO PPICT-ITEM-SIZE					96/01/16
061900	   IF THE-CHAR = REPEATED-CHAR					96/01/16
062000	       MOVE SPACE TO PPICT-SIGNED-FLAG				96/01/16
062100	       PERFORM SET-FLOATING-INSERT				96/01/16
062200	   ELSE 							96/01/16
062300	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
062400	       PERFORM SET-SIGNED-FLAG					96/01/16
062500	       PERFORM SET-SIGN 					96/01/16
062600	   .								96/01/16
062700									96/01/16
062800 SET-FLOATING-INSERT.						96/01/16
062900	   MOVE FLOATING-INSERT-EVENT TO THE-EXCEPTION-EVENT		96/01/16
063000     MOVE        "YES"          TO EXCEPTION-RAISED               96/01/16
063100	   .								96/01/16
063200									96/01/16
063300 SET-SIGN.							96/01/14
063400	   MOVE SIGN-EVENT TO THE-NEXT-EVENT				96/01/14
063500	   .								96/01/14
063600									96/01/14
063700 SET-EDIT-OR-FLOATING-INSERT.					96/01/16
063800	   ADD 1 TO PPICT-ITEM-SIZE					96/01/16
063900	   ADD 1 TO NUMERIC-EDITS					96/01/16
064000	   IF THE-CHAR = REPEATED-CHAR					96/01/16
064100	       PERFORM SET-FLOATING-INSERT				96/01/16
064200	   ELSE 							96/01/16
064300	       MOVE THE-CHAR TO REPEATED-CHAR				96/01/16
064400	       PERFORM SET-EDIT-SYMBOL					96/01/16
064500	   .								96/01/16
064600									96/01/16
064700 SET-EDIT-SYMBOL. 						96/01/14
064800	   MOVE EDIT-SYMBOL-EVENT TO THE-NEXT-EVENT			96/01/14
064900	   .								96/01/14
065000									96/01/14
065100 SET-POINT.							96/01/15
065200	   MOVE POINT-EVENT TO THE-NEXT-EVENT				96/01/15
065300	   .								96/01/15
065400									96/01/15
065500 CHECK-FOR-CR-DB-SYMBOL.						96/01/15
065600	   PERFORM SET-SIGNED-FLAG					96/01/16
065700	   MOVE SPACE TO REPEATED-CHAR					96/01/16
065800	   PERFORM GET-NEXT-CHAR					96/01/15
065900	   IF THE-CHAR = EXPECT-CHAR					96/01/15
066000	       ADD 2 TO PPICT-ITEM-SIZE 				96/01/16
066100	       ADD 1 TO NUMERIC-EDITS					96/01/15
066200	       PERFORM SET-EDIT-SYMBOL					96/01/15
066300	   ELSE 							96/01/15
066400	       PERFORM SET-OTHER					96/01/15
066500	   .								96/01/15
066600									96/01/15
066700 SET-OTHER.							96/01/14
066800	   MOVE OTHER-EVENT TO THE-NEXT-EVENT				96/01/14
066900	   .								96/01/14
067000									96/01/14
067100 COUNT-DIGITS.							96/01/16
067200	   IF PPICT-DIGIT-FLAG = SPACES 				96/01/16
067300	       MOVE ZEROES TO PPICT-DIGITS				96/01/16
067400	   .								96/01/16
067500	   IF PPICT-DIGITS < 98 					96/01/16
067600	       ADD 1 TO PPICT-DIGITS					96/01/16
067700	       IF PPICT-EXTERNAL-FLOAT					96/01/16
067800	       OR PPICT-DECIMAL-FLAG = SPACES				96/01/16
067900		   NEXT SENTENCE					96/01/16
068000	       ELSE							96/01/16
068100		   ADD 1 TO PPICT-DECIMALS				96/01/16
068200	   .								96/01/16
068300									96/01/16
068400 SET-SIGNED-FLAG. 						96/01/16
068500	   IF PPICT-SIGNED-FLAG = SPACE 				96/01/16
068600	       MOVE THE-CHAR TO PPICT-SIGNED-FLAG			96/01/16
068700	   .								96/01/16
068800									96/01/16
068900*********************    GET REPETITIONS	  **********************96/01/14
069000									96/01/14
069100 GET-REPETITIONS. 						96/01/14
069200	   MOVE ZERO TO THE-CHAR					96/01/14
069300	   MOVE ZERO TO THE-NUMBER					96/01/14
069400	   PERFORM GET-THE-NUMBER					96/01/14
069500	     UNTIL THE-CHAR NOT NUMERIC 				96/01/14
069600									96/01/14
069700     IF THE-CHAR NOT = ")"                                        96/01/14
069800	       PERFORM SIGNAL-RIGHT-PAR-MISSING 			96/01/14
069900	   ELSE 							96/01/16
070000	   IF REPEATED-CHAR = SPACE					96/01/16
070100	       PERFORM SIGNAL-INVALID-TO-REPEAT 			96/01/16
070200	   ELSE 							96/01/16
070300	       PERFORM CHECK-FOR-FLOATING-INSERT			96/01/16
070400	       SUBTRACT 1 FROM THE-NUMBER				96/01/16
070500	       COMPUTE PPICT-ITEM-SIZE = PPICT-ITEM-SIZE + THE-NUMBER	96/01/16
070600	       IF PPICT-DIGIT-FLAG NOT = SPACES 			96/01/16
070700		   PERFORM COUNT-DIGITS  THE-NUMBER TIMES		96/01/16
070800	   .								96/01/14
070900									96/01/14
071000 GET-THE-NUMBER.							96/01/14
071100	   COMPUTE THE-NUMBER = THE-NUMBER * 10 + THE-DIGIT		96/01/14
071200	   PERFORM GET-NEXT-CHAR					96/01/14
071300	   .								96/01/14
071400									96/01/14
071500 SIGNAL-RIGHT-PAR-MISSING.					96/01/14
071600	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
071700         MOVE "01Right Par. Missing" TO PPICT-ERROR-MESSAGE       96/01/16
071800	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
071900	   .								96/01/14
072000									96/01/14
072100 SIGNAL-INVALID-TO-REPEAT.					96/01/16
072200	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
072300         MOVE "01Invalid to Repeat" TO PPICT-ERROR-MESSAGE        96/01/16
072400	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
072500	   .								96/01/16
072600									96/01/16
072700 CHECK-FOR-FLOATING-INSERT.					96/01/16
072800     IF REPEATED-CHAR = "+" OR "-"                                96/01/16
072900	       MOVE SPACE TO PPICT-SIGNED-FLAG				96/01/16
073000	       PERFORM SET-FLOATING-INSERT				96/01/16
073100	   ELSE 							96/01/16
073200	   IF REPEATED-CHAR = THE-CURRENCY-SYMBOL			96/01/16
073300	       PERFORM SET-FLOATING-INSERT				96/01/16
073400	   .								96/01/16
073500									96/01/16
073600********************    SIGNAL ERROR FOUND    ********************96/01/14
073700									96/01/14
073800 SIGNAL-ERROR-FOUND.						96/01/14
073900	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
074000         MOVE "01Error in Picture" TO PPICT-ERROR-MESSAGE         96/01/16
074100	   .								96/01/14
074200									96/01/14
074300******************    SIGNAL FLOATING ERROR    *******************96/01/14
074400									96/01/14
074500 SIGNAL-FLOATING-ERROR.						96/01/14
074600	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
074700         MOVE "01Floating Format Error" TO PPICT-ERROR-MESSAGE    96/01/16
074800	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
074900	   .								96/01/14
075000									96/01/14
075100******************    SIGNAL INVALID SYMBOL    *******************96/01/14
075200									96/01/14
075300 SIGNAL-INVALID-SYMBOL.						96/01/14
075400	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
075500         MOVE "01Invalid Symbol in Picture" TO PPICT-ERROR-MESSAGE96/01/16
075600	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
075700	   .								96/01/14
075800									96/01/14
075900********************    SIGNAL NO PICTURE    *********************96/01/14
076000									96/01/14
076100 SIGNAL-NO-PICTURE.						96/01/14
076200	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
076300         MOVE "01No Picture Found" TO PPICT-ERROR-MESSAGE         96/01/16
076400	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
076500	   .								96/01/14
076600									96/01/14
076700******************    SIGNAL NOT ALPHABETIC    *******************96/01/14
076800									96/01/14
076900 SIGNAL-NOT-ALPHABETIC.						96/01/14
077000	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
077100         MOVE "01Not Alphabetic" TO PPICT-ERROR-MESSAGE           96/01/16
077200	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
077300	   .								96/01/14
077400									96/01/14
077500*****************    SIGNAL NOT ALPHANUMERIC    ******************96/01/14
077600									96/01/14
077700 SIGNAL-NOT-ALPHANUMERIC. 					96/01/14
077800	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
077900         MOVE "01Not Alphanumeric" TO PPICT-ERROR-MESSAGE         96/01/16
078000	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
078100	   .								96/01/14
078200									96/01/14
078300********************    SIGNAL NOT EDITED    *********************96/01/14
078400									96/01/14
078500 SIGNAL-NOT-EDITED.						96/01/14
078600	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
078700         MOVE "01Not Edited" TO PPICT-ERROR-MESSAGE               96/01/16
078800	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
078900	   .								96/01/14
079000									96/01/14
079100*******************    SIGNAL NOT FLOATING    ********************96/01/14
079200									96/01/14
079300 SIGNAL-NOT-FLOATING.						96/01/14
079400	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
079500         MOVE "01Not Floating" TO PPICT-ERROR-MESSAGE             96/01/16
079600	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
079700	   .								96/01/14
079800									96/01/14
079900********************    SIGNAL NOT NUMBER    *********************96/01/14
080000									96/01/14
080100 SIGNAL-NOT-NUMBER.						96/01/14
080200	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
080300         MOVE "01Not Number" TO PPICT-ERROR-MESSAGE               96/01/16
080400	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
080500	   .								96/01/14
080600									96/01/14
080700********************    SIGNAL NOT NUMERIC    ********************96/01/14
080800									96/01/14
080900 SIGNAL-NOT-NUMERIC.						96/01/14
081000	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
081100         MOVE "01Not Numeric" TO PPICT-ERROR-MESSAGE              96/01/16
081200	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
081300	   .								96/01/14
081400									96/01/14
081500*****************    SIGNAL NOTHING TO REPEAT    *****************96/01/14
081600									96/01/14
081700 SIGNAL-NOTHING-TO-REPEAT.					96/01/14
081800	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
081900         MOVE "01Nothing to Repeat" TO PPICT-ERROR-MESSAGE        96/01/16
082000	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
082100	   .								96/01/14
082200									96/01/14
082300******************    SIGNAL S MUST BE FIRST    ******************96/01/14
082400									96/01/14
082500 SIGNAL-S-MUST-BE-FIRST.						96/01/14
082600	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
082700         MOVE "01'S' Must be First" TO PPICT-ERROR-MESSAGE        96/01/16
082800	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
082900	   .								96/01/14
083000									96/01/14
083100*************************    STORE AE    *************************96/01/14
083200									96/01/14
083300 STORE-AE.							96/01/14
083400     MOVE "E" TO PPICT-ITEM-TYPE                                  96/01/14
083500	   PERFORM CHECK-SIZE						96/01/15
083600	   .								96/01/14
083700									96/01/14
083800**********************	STORE AE OR NE	  **********************96/01/14
083900									96/01/14
084000 STORE-AE-OR-NE.							96/01/14
084100	   IF NUMERIC-EDITS > ZERO					96/01/14
084200         MOVE "N" TO PPICT-ITEM-TYPE                              96/01/14
084300	   ELSE 							96/01/14
084400         MOVE "E" TO PPICT-ITEM-TYPE                              96/01/14
084500	   .								96/01/14
084600	   PERFORM CHECK-SIZE						96/01/15
084700	   .								96/01/15
084800									96/01/15
084900*************************    STORE AL    *************************96/01/14
085000									96/01/14
085100 STORE-AL.							96/01/14
085200     MOVE "A" TO PPICT-ITEM-TYPE                                  96/01/14
085300	   PERFORM CHECK-SIZE						96/01/15
085400	   .								96/01/14
085500									96/01/14
085600 CHECK-SIZE.							96/01/15
085700	   IF PPICT-ITEM-SIZE < 1					96/01/16
085800	       PERFORM SIGNAL-INVALID-SIZE				96/01/15
085900	       MOVE 1 TO PPICT-ITEM-SIZE				96/01/16
086000	   ELSE 							96/01/16
086100	   IF PPICT-DIGIT-FLAG > SPACES 				96/01/16
086200	   AND PPICT-DIGITS > 18					96/01/16
086300	       PERFORM SIGNAL-TOO-MANY-DIGITS				96/01/16
086400	       MOVE 18 TO PPICT-ITEM-SIZE				96/01/16
086500	   .								96/01/15
086600									96/01/15
086700 SIGNAL-INVALID-SIZE.						96/01/15
086800	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
086900         MOVE "01Invalid Size" TO PPICT-ERROR-MESSAGE             96/01/16
087000	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
087100	   .								96/01/15
087200									96/01/15
087300 SIGNAL-TOO-MANY-DIGITS.						96/01/16
087400	   IF PPICT-ERROR-CODE = ZEROES 				96/01/16
087500         MOVE "01Too Many Digits" TO PPICT-ERROR-MESSAGE          96/01/16
087600	       PERFORM SIGNAL-ERROR-FOUND				96/01/16
087700	   .								96/01/16
087800									96/01/16
087900**********************	STORE AL OR AE	  **********************96/01/14
088000									96/01/14
088100 STORE-AL-OR-AE.							96/01/14
088200	   IF NUMERIC-EDITS > ZERO					96/01/15
088300         MOVE "E" TO PPICT-ITEM-TYPE                              96/01/14
088400	   ELSE 							96/01/14
088500         MOVE "A" TO PPICT-ITEM-TYPE                              96/01/14
088600	   .								96/01/14
088700	   PERFORM CHECK-SIZE						96/01/15
088800	   .								96/01/15
088900									96/01/15
089000*******************    STORE AL OR AE OR NE    *******************96/01/14
089100									96/01/14
089200 STORE-AL-OR-AE-OR-NE.						96/01/14
089300	   IF NUMERIC-EDITS > ZERO					96/01/14
089400         MOVE "N" TO PPICT-ITEM-TYPE                              96/01/14
089500	   ELSE 							96/01/14
089600	   IF ALPHANUMERIC-EDITS > ZERO 				96/01/14
089700         MOVE "E" TO PPICT-ITEM-TYPE                              96/01/14
089800	   ELSE 							96/01/14
089900         MOVE "A" TO PPICT-ITEM-TYPE                              96/01/14
090000	   .								96/01/14
090100	   PERFORM CHECK-SIZE						96/01/15
090200	   .								96/01/15
090300									96/01/15
090400**********************	STORE AN OR AE	  **********************96/01/14
090500									96/01/14
090600 STORE-AN-OR-AE.							96/01/14
090700	   IF ALPHANUMERIC-EDITS > ZERO 				96/01/14
090800         MOVE "E" TO PPICT-ITEM-TYPE                              96/01/14
090900	   ELSE 							96/01/14
091000         MOVE "X" TO PPICT-ITEM-TYPE                              96/01/14
091100	   .								96/01/14
091200	   PERFORM CHECK-SIZE						96/01/15
091300	   .								96/01/15
091400									96/01/14
091500*************************    STORE FL    *************************96/01/14
091600									96/01/14
091700 STORE-FL.							96/01/14
091800     MOVE "F" TO PPICT-ITEM-TYPE                                  96/01/14
091900	   PERFORM CHECK-SIZE						96/01/15
092000	   .								96/01/14
092100									96/01/14
092200*************************    STORE NE    *************************96/01/14
092300									96/01/14
092400 STORE-NE.							96/01/14
092500     MOVE "N" TO PPICT-ITEM-TYPE                                  96/01/14
092600	   PERFORM CHECK-SIZE						96/01/15
092700	   .								96/01/14
092800									96/01/14
092900*************************    STORE NU    *************************96/01/14
093000									96/01/14
093100 STORE-NU.							96/01/14
093200     MOVE "9" TO PPICT-ITEM-TYPE                                  96/01/14
093300	   PERFORM CHECK-SIZE						96/01/15
093400	   .								96/01/15
093500									96/01/14
093600**********************	STORE NU OR NE	  **********************96/01/14
093700									96/01/14
093800 STORE-NU-OR-NE.							96/01/14
093900	   IF NUMERIC-EDITS > ZERO					96/01/14
094000         MOVE "N" TO PPICT-ITEM-TYPE                              96/01/14
094100	   ELSE 							96/01/14
094200         MOVE "9" TO PPICT-ITEM-TYPE                              96/01/14
094300	   .								96/01/14
094400	   PERFORM CHECK-SIZE						96/01/15
094500	   .								96/01/15
094600									96/04/03
094700*********************   GET EXTERNAL EVENT   *********************96/04/03
094800									96/04/03
094900 GET-EXTERNAL-EVENT.						96/04/03
095000	   EXIT 							96/04/03
095100	   .								96/04/03
095200									96/04/03
095300*******************   INITIALISE THE PROGRAM   *******************96/04/03
095400									96/04/03
095500 INITIALISE-THE-PROGRAM.						96/04/03
095600	   EXIT 							96/04/03
095700	   .								96/04/03