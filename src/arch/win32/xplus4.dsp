# Microsoft Developer Studio Project File - Name="xplus4" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=xplus4 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xplus4.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xplus4.mak" CFG="xplus4 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xplus4 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "xplus4 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "xplus4 - Win32 DX Release" (based on "Win32 (x86) Application")
!MESSAGE "xplus4 - Win32 DX Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xplus4 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\data"
# PROP Intermediate_Dir "libs\xplus4\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\msvc" /I ".\\" /I "..\..\platform" /I "..\..\\" /I "..\..\drive" /I "..\..\vdrive" /I "..\..\monitor" /I "..\..\lib\p64" /I "..\..\sid" /D "WIN32" /D "NODIRECTX" /D "IDE_COMPILE" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i ".\MSVC" /i ".\\" /i "..\..\\" /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib winmm.lib ole32.lib wsock32.lib version.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "xplus4 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\data"
# PROP Intermediate_Dir "libs\xplus4\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I ".\msvc" /I ".\\" /I "..\..\platform" /I "..\..\\" /I "..\..\drive" /I "..\..\vdrive" /I "..\..\monitor" /I "..\..\lib\p64" /I "..\..\sid" /D "WIN32" /D "NODIRECTX" /D "IDE_COMPILE" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i ".\MSVC" /i ".\\" /i "..\..\\" /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib winmm.lib ole32.lib wsock32.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "xplus4 - Win32 DX Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DXRelease"
# PROP BASE Intermediate_Dir "DXRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\data"
# PROP Intermediate_Dir "libs\xplus4\DXRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I ".\msvc" /I ".\\" /I "..\..\platform" /I "..\..\\" /I "..\..\drive" /I "..\..\vdrive" /I "..\..\monitor" /I "..\..\lib\p64" /I "..\..\sid" /D "WIN32" /D "IDE_COMPILE" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\msvc" /I ".\\" /I "..\..\platform" /I "..\..\\" /I "..\..\drive" /I "..\..\vdrive" /I "..\..\monitor" /I "..\..\lib\p64" /I "..\..\sid" /D "WIN32" /D "IDE_COMPILE" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /i ".\MSVC" /i ".\\" /i "..\..\\" /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE"
# ADD RSC /l 0x409 /i ".\MSVC" /i ".\\" /i "..\..\\" /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib winmm.lib dsound.lib ole32.lib dxguid.lib wsock32.lib version.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib winmm.lib dsound.lib ole32.lib dxguid.lib wsock32.lib version.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "xplus4 - Win32 DX Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DXDebug"
# PROP BASE Intermediate_Dir "DXDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\data"
# PROP Intermediate_Dir "libs\xplus4\DXDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I ".\msvc" /I ".\\" /I "..\..\platform" /I "..\..\\" /I "..\..\drive" /I "..\..\vdrive" /I "..\..\monitor" /I "..\..\lib\p64" /I "..\..\sid" /D "WIN32" /D "IDE_COMPILE" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I ".\msvc" /I ".\\" /I "..\..\platform" /I "..\..\\" /I "..\..\drive" /I "..\..\vdrive" /I "..\..\monitor" /I "..\..\lib\p64" /I "..\..\sid" /D "WIN32" /D "IDE_COMPILE" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /i ".\MSVC" /i ".\\" /i "..\..\\" /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE"
# ADD RSC /l 0x409 /i ".\MSVC" /i ".\\" /i "..\..\\" /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib winmm.lib dsound.lib ole32.lib wsock32.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib winmm.lib dsound.lib ole32.lib wsock32.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "xplus4 - Win32 Release"
# Name "xplus4 - Win32 Debug"
# Name "xplus4 - Win32 DX Release"
# Name "xplus4 - Win32 DX Debug"
# Begin Source File

SOURCE=..\..\main.c
# End Source File
# Begin Source File

SOURCE=.\resplus4.rc

!IF  "$(CFG)" == "xplus4 - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RESPL="..\..\debug.h"	"resplus4.rc"	"res.rc"	"rested.rc"	"resacia.rc"	"resdriveplus4.rc"	"resautostart.rc"	"resv364speech.rc"	"ressidcart.rc"	
# Begin Custom Build
InputPath=.\resplus4.rc

"resplus4cat.rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy /b resplus4.rc + res.rc + rested.rc + resacia.rc + resdriveplus4.rc + resautostart.rc + resv364speech.rc + ressidcart.rc resplus4cat.rc /b

# End Custom Build

!ELSEIF  "$(CFG)" == "xplus4 - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RESPL="..\..\debug.h"	"resplus4.rc"	"res.rc"	"rested.rc"	"resacia.rc"	"resdriveplus4.rc"	"resautostart.rc"	"resv364speech.rc"	"ressidcart.rc"	
# Begin Custom Build
InputPath=.\resplus4.rc

"resplus4cat.rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy /b resplus4.rc + res.rc + rested.rc + resacia.rc + resdriveplus4.rc + resautostart.rc + resv364speech.rc + ressidcart.rc resplus4cat.rc /b

# End Custom Build

!ELSEIF  "$(CFG)" == "xplus4 - Win32 DX Release"

# PROP Ignore_Default_Tool 1
USERDEP__RESPL="..\..\debug.h"	"resplus4.rc"	"res.rc"	"rested.rc"	"resacia.rc"	"resdriveplus4.rc"	"resautostart.rc"	"resv364speech.rc"	"ressidcart.rc"	
# Begin Custom Build
InputPath=.\resplus4.rc

"resplus4cat.rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy /b resplus4.rc + res.rc + rested.rc + resacia.rc + resdriveplus4.rc + resautostart.rc + resv364speech.rc + ressidcart.rc resplus4cat.rc /b

# End Custom Build

!ELSEIF  "$(CFG)" == "xplus4 - Win32 DX Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RESPL="..\..\debug.h"	"resplus4.rc"	"res.rc"	"rested.rc"	"resacia.rc"	"resdriveplus4.rc"	"resautostart.rc"	"resv364speech.rc"	"ressidcart.rc"	
# Begin Custom Build
InputPath=.\resplus4.rc

"resplus4cat.rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy /b resplus4.rc + res.rc + rested.rc + resacia.rc + resdriveplus4.rc + resautostart.rc + resv364speech.rc + ressidcart.rc resplus4cat.rc /b

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resplus4cat.rc
# End Source File
# Begin Source File

SOURCE=.\vice.manifest
# End Source File
# End Target
# End Project
