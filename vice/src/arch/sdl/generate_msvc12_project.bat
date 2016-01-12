@echo off
if not exist ..\win32\vs_tmpl\mkmsvc.exe goto missingmkmsvc
echo generating MSVC12 project as win32-msvc12
if exist win32-msvc12 goto removedir
:makemsvcdir
mkdir win32-msvc12
cd ..\win32\vs_tmpl
mkmsvc.exe -sdl -12 vice
cd ..\..\sdl
copy msvc-files\make-bindist.bat.proto win32-msvc12\make-msvc-bindist.bat
copy msvc-files\debug.h win32-msvc12
copy msvc-files\dirent.h win32-msvc12
copy msvc-files\ide-config.h win32-msvc12
copy msvc-files\ide-siddefs.h win32-msvc12
copy msvc-files\ide-siddtvdefs.h win32-msvc12
copy msvc-files\inttypes.h win32-msvc12
copy msvc-files\stdint.h win32-msvc12
copy msvc-files\ffmpeg-config.h win32-msvc12
copy msvc-files\vice.manifest win32-msvc12
copy msvc-files\winid_arm.bat win32-msvc12
copy msvc-files\winid_x64.bat win32-msvc12
copy msvc-files\winid_x86.bat win32-msvc12
echo MSVC12 project files generated in win32-msvc12
goto end
:missingmkmsvc
echo ..\win32\vs_tmpl\mkmsvc.exe is missing
goto end
:removedir
echo y | del win32-msvc12\*.* /S
rd win32-msvc12 /s /q
if exist win32-msvc12 goto cannotremovedir
goto makemsvcdir
:cannotremovedir
echo can't delete the win32-msvc12 directory, please remove win32-msvc12 manually and run this batch file again.
:end
pause
