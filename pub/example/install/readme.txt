-------------------------------------------------------------------------------
Package:    Libero 'install' Script For UNIX
-------------------------------------------------------------------------------

Written:    22 Oct 1995
Revised:    12 Dec 1995
Version:    1.2

Author:     Pieter A. Hintjens
            Pijlstraat 9
            2060 Antwerpen, Belgium
            ph@mymail.com
            (+323) 231.5277

Notes:      This script can be reused for other applications; it is driven by
            a file that lists all the files in the application and what to do
            with each one.

            The approach I take here is quite different from the standard one
            used by a 'make' file.  Firstly, I want the install script to be
            capable of being quite smart about its environment.  This means,
            for instance, that it tries to detect the UNIX system type and
            use the right compiler switches automatically.  Secondly, Libero
            is straight-forward enough (on purpose) to not need a complex
            make file.

Contents:   File:           Purpose:
            README   TXT    This file
            INSTALL  L      Dialog file for install script
            INSTALL  MOD    Source file (modules) for install script
            INSTALL         Generated install script (Korn shell)
            INSTALL  CAT    List of files in application

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