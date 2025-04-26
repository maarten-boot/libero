-------------------------------------------------------------------------------
Package:    COBOL Picture Parser
-------------------------------------------------------------------------------

Written:    22 Oct 1995
Revised:    3 Apr 1996
Version:    1.2

Author:     Pieter A. Hintjens
            Pijlstraat 9
            2060 Antwerpen, Belgium
            ph@mymail.com
            (+323) 231.5277

Notes:      This function is part of an unfinished free COBOL compiler,
            PCT.  The picture parsing is interesting because it is very hard
            (impossible?) to do in Lex or Flex.  The file i/o functions, even
            though some are empty, are pretty fast.

            This program shows how you can parse complex text that was not
            defined with today's tools (i.e. lex) in mind.

Contents:   File:           Purpose:
            README   TXT    This file
            PCTPICT  C      Source code for picture parsing function
            PCTPICTD L      Dialog for picture parsing function
            PCTPICT  H      Prototypes for picture parsing function
            PCTFILE  C      File i/o functions
            PCTFILE  H      Prototypes
            PCTLIBR  C      Library functions
            PCTLIBR  H      Prototypes
            PRELUDE  H      General 'C' include file
            PCTDEFN  H      General definitions for PCT project
            TESTPICT C      Test program for picture parsing function
            PICTURE  TST    Test data for picture parsing function
            PPICT    COB    COBOL version of picture parser
            PPICTD   L      Dialog for this program
            PPICTR   COB    Copy book containing parameters for CALL

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
