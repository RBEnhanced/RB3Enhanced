@echo off
setlocal enabledelayedexpansion

REM Set compilation variables.
set OUTNAME=RB3Enhanced
set BUILD_DIR=build_xbox
set OUT_DIR=out
set SRC_DIR=source
set INC_DIR=include
set LIBS=xapilib.lib xboxkrnl.lib xnet.lib xonline.lib
set DEFINES=-D _XBOX -D NDEBUG -D RB3E_XBOX -D RB3E -D RB3EDEBUG
REM Set the SDK paths to use.
set INCLUDE=%XEDK%\include\xbox
set LIB=%XEDK%\lib\xbox
set BINARYPATH=%XEDK%\bin\win32

REM Warning, since this build script is just to tide X360-only people over without the full toolchain.
echo It is highly recommended that you install the build tools as noted in BUILDING.md, and use "make xbox" to build an Xbox 360 version.
timeout /t 3

REM Create folders.
@mkdir %BUILD_DIR% >nul 2>&1
@mkdir %OUT_DIR% >nul 2>&1

REM Pre-build script
call scripts\version.bat > source\version.h

REM Compile all .c source files.
for %%a in (%SRC_DIR%/*.c) do "%BINARYPATH%\cl.exe" -c -Zi -nologo -W3 -WX- -Ox -Os %DEFINES% -GF -Gm- -MT -GS- -Gy -fp:fast -fp:except- -Zc:wchar_t -Zc:forScope -GR- -openmp- -FI"%XEDK%/include/xbox/xbox_intellisense_platform.h" -Fd"%BUILD_DIR%/" -I "%INC_DIR%" -Fo"%BUILD_DIR%/%%a.obj" -TC %SRC_DIR%/%%a

REM Fetch all created .obj files.
set OBJECTS=
for %%a in (%BUILD_DIR%/*.c.obj) do set OBJECTS=%BUILD_DIR%/%%a !OBJECTS!
echo Linking DLL...
"%BINARYPATH%\link.exe" -ERRORREPORT:PROMPT -INCREMENTAL:NO -NOLOGO %LIBS% -MANIFESTUAC:"level='asInvoker' uiAccess='false'" -DEBUG -STACK:"262144","262144" -OPT:REF -OPT:ICF -TLBID:1 -RELEASE -dll -entry:"_DllMainCRTStartup" -XEX:NO -OUT:"%BUILD_DIR%/%OUTNAME%.exe" -PDB:"%BUILD_DIR%/%OUTNAME%.pdb" -IMPLIB:"%BUILD_DIR%/%OUTNAME%" %OBJECTS%

REM Create a final XEXDLL.
echo Creating XEXDLL...
"%BINARYPATH%\imagexex.exe" -nologo -config:"xex.xml" -out:"%OUT_DIR%/%OUTNAME%.dll" "%BUILD_DIR%/%OUTNAME%.exe"

endlocal