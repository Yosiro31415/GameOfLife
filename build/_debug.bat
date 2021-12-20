@echo off

call resource.bat
call shader.bat


setlocal enabledelayedexpansion

set kCL="C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.29.30133\bin\Hostx86\x86\cl.exe"
set kLIB="C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.29.30133\bin\Hostx86\x86\lib.exe"
set kOptInclude=^
/I "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.29.30133\include" ^
/I "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\VS\include" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\winrt" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\cppwinrt"
set kOptLibrary=/link ^
/SUBSYSTEM:CONSOLE ^
/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.29.30133\lib\x86" ^
/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\VS\lib\x86" ^
/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\ucrt\x86" ^
/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\um\x86" ^
/LIBPATH:"."

set kDelimiter= 
set pathFiles=
if exist "..\src\*.cpp" (
    for /f "tokens=* delims=" %%x in ('dir ..\src\*.cpp /S /B') do (
        set pathFiles=!pathFiles!%%x!kDelimiter!
    )
)

if "!pathFiles!"=="" (
    echo There is no file that should be compiled.
    exit /B
)

!kCL! /EHsc /Fe:main.exe !kOptInclude! !pathFiles! !kOptLibrary! ../res/resource.res
del *.obj