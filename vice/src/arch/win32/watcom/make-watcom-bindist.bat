@echo off

rem make-watcom-bindist.bat - make binary distribution for the Windows port
rem
rem Written by
rem  Marco van den Heuvel <blackystardust68@yahoo.com>
rem
rem This file is part of VICE, the Versatile Commodore Emulator.
rem See README for copyright notice.
rem
rem  This program is free software; you can redistribute it and/or modify
rem  it under the terms of the GNU General Public License as published by
rem  the Free Software Foundation; either version 2 of the License, or
rem  (at your option) any later version.
rem
rem  This program is distributed in the hope that it will be useful,
rem  but WITHOUT ANY WARRANTY; without even the implied warranty of
rem  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem  GNU General Public License for more details.
rem
rem  You should have received a copy of the GNU General Public License
rem  along with this program; if not, write to the Free Software
rem  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
rem  02111-1307  USA.

call ..\vice-version.bat
set VICEDIR=WinVICE-%VICEVERSION%-x86
if not exist x64.exe goto missingwatcom
if not exist x64dtv.exe goto missingwatcom
if not exist x64sc.exe goto missingwatcom
if not exist xscpu64.exe goto missingwatcom
if not exist x128.exe goto missingwatcom
if not exist xcbm2.exe goto missingwatcom
if not exist xcbm5x0.exe goto missingwatcom
if not exist xpet.exe goto missingwatcom
if not exist xplus4.exe goto missingwatcom
if not exist xvic.exe goto missingwatcom
if not exist vsid.exe goto missingwatcom
if not exist c1541.exe goto missingwatcom
if not exist cartconv.exe goto missingwatcom
if not exist petcat.exe goto missingwatcom
echo generating OPENWATCOM WIN32 binary port distribution
if exist %VICEDIR% goto removedir
:makevicedir
mkdir %VICEDIR%
copy x64.exe %VICEDIR%
copy x64dtv.exe %VICEDIR%
copy x64sc.exe %VICEDIR%
copy xscpu64.exe %VICEDIR%
copy x128.exe %VICEDIR%
copy xvic.exe %VICEDIR%
copy xpet.exe %VICEDIR%
copy xplus4.exe %VICEDIR%
copy xcbm2.exe %VICEDIR%
copy xcbm5x0.exe %VICEDIR%
copy vsid.exe %VICEDIR%
copy c1541.exe %VICEDIR%
copy petcat.exe %VICEDIR%
copy cartconv.exe %VICEDIR%
mkdir %VICEDIR%\C128
copy ..\..\..\..\data\C128\basic* %VICEDIR%\C128
copy ..\..\..\..\data\C128\*.vpl %VICEDIR%\C128
copy ..\..\..\..\data\C128\charg* %VICEDIR%\C128
copy ..\..\..\..\data\C128\*.vrs %VICEDIR%\C128
copy ..\..\..\..\data\C128\kernal* %VICEDIR%\C128
copy ..\..\..\..\data\C128\win_*.v* %VICEDIR%\C128
mkdir %VICEDIR%\C64
copy ..\..\..\..\data\C64\basic %VICEDIR%\C64
copy ..\..\..\..\data\C64\*.vpl %VICEDIR%\C64
copy ..\..\..\..\data\C64\c64mem.sym %VICEDIR%\C64
copy ..\..\..\..\data\C64\chargen %VICEDIR%\C64
copy ..\..\..\..\data\C64\*.vrs %VICEDIR%\C64
copy ..\..\..\..\data\C64\edkernal %VICEDIR%\C64
copy ..\..\..\..\data\C64\gskernal %VICEDIR%\C64
copy ..\..\..\..\data\C64\jp* %VICEDIR%\C64
copy ..\..\..\..\data\C64\kernal %VICEDIR%\C64
copy ..\..\..\..\data\C64\sxkernal %VICEDIR%\C64
copy ..\..\..\..\data\C64\win_*.v* %VICEDIR%\C64
mkdir %VICEDIR%\SCPU64
copy ..\..\..\..\data\SCPU64\*.vpl %VICEDIR%\SCPU64
copy ..\..\..\..\data\SCPU64\chargen %VICEDIR%\SCPU64
copy ..\..\..\..\data\SCPU64\*.vrs %VICEDIR%\SCPU64
copy ..\..\..\..\data\SCPU64\scpu64 %VICEDIR%\SCPU64
copy ..\..\..\..\data\SCPU64\jp* %VICEDIR%\SCPU64
copy ..\..\..\..\data\SCPU64\scpu64mem.sym %VICEDIR%\SCPU64
copy ..\..\..\..\data\SCPU64\win_*.v* %VICEDIR%\SCPU64
mkdir %VICEDIR%\C64DTV
copy ..\..\..\..\data\C64DTV\basic %VICEDIR%\C64DTV
copy ..\..\..\..\data\C64DTV\*.vpl %VICEDIR%\C64DTV
copy ..\..\..\..\data\C64DTV\c64mem.sym %VICEDIR%\C64DTV
copy ..\..\..\..\data\C64DTV\chargen %VICEDIR%\C64DTV
copy ..\..\..\..\data\C64DTV\*.vrs %VICEDIR%\C64DTV
copy ..\..\..\..\data\C64DTV\kernal %VICEDIR%\C64DTV
copy ..\..\..\..\data\C64DTV\win_*.v* %VICEDIR%\C64DTV
copy ..\..\..\..\data\C64DTV\dtvrom.bin %VICEDIR%\C64DTV
mkdir %VICEDIR%\CBM-II
copy ..\..\..\..\data\CBM-II\*.vpl %VICEDIR%\CBM-II
copy ..\..\..\..\data\CBM-II\basic.* %VICEDIR%\CBM-II
copy ..\..\..\..\data\CBM-II\chargen.* %VICEDIR%\CBM-II
copy ..\..\..\..\data\CBM-II\kernal* %VICEDIR%\CBM-II
copy ..\..\..\..\data\CBM-II\*.vrs %VICEDIR%\CBM-II
copy ..\..\..\..\data\CBM-II\win_*.v* %VICEDIR%\CBM-II
mkdir %VICEDIR%\DRIVES
copy ..\..\..\..\data\DRIVES\d* %VICEDIR%\DRIVES
mkdir %VICEDIR%\PET
copy ..\..\..\..\data\PET\*.vpl %VICEDIR%\PET
copy ..\..\..\..\data\PET\basic* %VICEDIR%\PET
copy ..\..\..\..\data\PET\chargen* %VICEDIR%\PET
copy ..\..\..\..\data\PET\edit* %VICEDIR%\PET
copy ..\..\..\..\data\PET\kernal* %VICEDIR%\PET
copy ..\..\..\..\data\PET\*.vrs %VICEDIR%\PET
copy ..\..\..\..\data\PET\win_*.v* %VICEDIR%\PET
copy ..\..\..\..\data\PET\*.bin %VICEDIR%\PET
mkdir %VICEDIR%\PLUS4
copy ..\..\..\..\data\PLUS4\3plus1* %VICEDIR%\PLUS4
copy ..\..\..\..\data\PLUS4\basic %VICEDIR%\PLUS4
copy ..\..\..\..\data\PLUS4\c2lo.364 %VICEDIR%\PLUS4
copy ..\..\..\..\data\PLUS4\*.vpl %VICEDIR%\PLUS4
copy ..\..\..\..\data\PLUS4\*.vrs %VICEDIR%\PLUS4
copy ..\..\..\..\data\PLUS4\kernal %VICEDIR%\PLUS4
copy ..\..\..\..\data\PLUS4\kernal.* %VICEDIR%\PLUS4
copy ..\..\..\..\data\PLUS4\win_*.v* %VICEDIR%\PLUS4
mkdir %VICEDIR%\PRINTER
copy ..\..\..\..\data\PRINTER\cbm1526 %VICEDIR%\PRINTER
copy ..\..\..\..\data\PRINTER\mps* %VICEDIR%\PRINTER
copy ..\..\..\..\data\PRINTER\*.vpl %VICEDIR%\PRINTER
copy ..\..\..\..\data\PRINTER\nl10-cbm %VICEDIR%\PRINTER
mkdir %VICEDIR%\VIC20
copy ..\..\..\..\data\VIC20\basic %VICEDIR%\VIC20
copy ..\..\..\..\data\VIC20\chargen %VICEDIR%\VIC20
copy ..\..\..\..\data\VIC20\*.vpl %VICEDIR%\VIC20
copy ..\..\..\..\data\VIC20\*.vrs %VICEDIR%\VIC20
copy ..\..\..\..\data\VIC20\kernal %VICEDIR%\VIC20
copy ..\..\..\..\data\VIC20\win_*.v* %VICEDIR%\VIC20
mkdir %VICEDIR%\fonts
copy ..\..\..\..\data\fonts\*.fon %VICEDIR%\fonts
mkdir %VICEDIR%\html
copy ..\..\..\..\doc\html\*.html %VICEDIR%\html
mkdir %VICEDIR%\html\images
copy ..\..\..\..\doc\html\images\* %VICEDIR%\html\images
copy ..\..\..\..\FEEDBACK %VICEDIR%
copy ..\..\..\..\README %VICEDIR%
copy ..\..\..\..\COPYING %VICEDIR%
copy ..\..\..\..\NEWS %VICEDIR%
mkdir %VICEDIR%\doc
copy ..\..\..\..\doc\vice.chm %VICEDIR%\doc
copy ..\..\..\..\doc\vice.hlp %VICEDIR%\doc
copy ..\..\..\..\doc\vice.pdf %VICEDIR%\doc
echo WIN32 port binary distribution directory generated as %VICEDIR%
goto end
:missingwatcom
echo executables are missing, please build the project first.
goto end
:removedir
echo y | del %VICEDIR%\*.* /S
rd %VICEDIR% /s /q
if exist %VICEDIR% goto cannotremovedir
goto makevicedir
:cannotremovedir
echo can't delete the %VICEDIR% directory, please remove %VICEDIR% manually and run this batch file again.
:end
pause
