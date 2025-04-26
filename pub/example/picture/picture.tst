
    picture.tst  Input file for TESTPICT

    Written:  95/02/26   Pieter Hintjens
    Revised:  95/02/27   Pieter Hintjens

    Blank lines and lines starting with a space are ignored
    Lines starting with '!' are copied to the output
    Other lines are analysed as pictures and the results displayed

    These are valid pictures:

9
9(01)
9(02)
9(03)
9(04)
9(05)
9(06)
9(08)
9(09)
9(1)
9(10)
9(12)
9(13)
9(15)
9(16)
9(18)
9(2)
9(3)
9(3)V
V9(3)
9(3)V9(3)9(3)9(3)9(3)9(3)
9(4)
9(5)
9(6)
9(7)
9(8)
9(9)
9(9)+
+9(9)
+++++9(9)
9(9)V9(9)
99
99/99/99
9999
9999/99/99
999999
99BB
B99
B99B99B99B99
B99B99B99BB
S9(02)
S9(1)V9(17)
S9(13)
S9(15)
S9(3)
S9(3)V9(9)
S9(4)
S9(5)
S9(7)
S9(8)
S9(9)
S9(9)V9(2)
S9(9)V9(9)
SV9(18)
V9(18)
X
X(008)
X(0090)
X(01)
X(02)
X(03)
X(04)
X(05)
X(06)
X(07)
X(08)
X(080)
X(09)
X(1)
X(10)
X(1000)
X(104)
X(1043)
X(108)
X(11)
X(111)
X(112)
X(113)
X(12)
X(120)
X(1224)
X(1232)
X(126)
X(127)
X(13)
X(1362)
X(14)
X(1498)
X(15)
X(1544)
X(16)
X(17)
X(18)
X(180)
X(19)
X(192)
X(1921)
X(2)
X(20)
X(200)
X(21)
X(22)
X(220)
X(2221)
X(23)
X(2300)
X(24)
X(240)
X(25)
X(252)
X(26)
X(27)
X(28)
X(29)
X(3)
X(30)
X(31)
X(32)
X(33)
X(34)
X(35)
X(36)
X(37)
X(379)
X(38)
X(39)
X(4)
X(40)
X(41)
X(42)
X(43)
X(45)
X(46)
X(47)
X(48)
X(49)
X(5)
X(50)
X(51)
X(52)
X(53)
X(56)
X(57)
X(58)
X(6)
X(60)
X(61)
X(62)
X(63)
X(64)
X(65)
X(66)
X(67)
X(68)
X(69)
X(7)
X(70)
X(71)
X(72)
X(74)
X(75)
X(77)
X(78)
X(79)
X(792)
X(8)
X(80)
X(81)
X(82)
X(9)
X(95)
XX
Z(02)
Z(03)
Z(05)
Z(08)
Z(10)
Z(2),Z(3),Z(2)9.9(2)
Z(3),Z(2)9.9(2)
Z(3),Z(3),Z(2)9.9(2)
Z(3)9
Z(5)9.9(3)
Z(8)9
Z9
ZZ
ZZ9
ZZZ
ZZZ9
ZZZZ9
ZZZZZ9
-Z9
$Z9
$$$$9
-$$$$9
-$Z9
--
$$

    These are valid pictures, but not portable:

9X
9(2000)X
X9(2000)
X(64000)


    These are invalid pictures:

!   ')' missing after repeat specifier

9(01

!   Only one decimal point is allowed in picture

V9(3)V

!   Only one decimal point is allowed in picture

9(2).9(3).9

!   Decimals cannot be partly zero-suppressed

ZZZZZ.ZZ9

!   Sign cannot be specified twice in picture

+9(9)+
-999-

!   Floating sign can only come at start of picture

9(3)---
-999---

!   Sign must come at start or at end of picture

999-999
$$$$-9

!   Picture must contain at least one data position

-
$

!   '%c' not valid in picture at this point

9999Z999
XXXBXXXX
9999Z
XXXXB
XXXX+
S9(3)-

!   (...) must follow one of: B X Z 0 9 * / + - , $

9(01)(01)
(01)
9999,(2).(2)
