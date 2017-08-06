@echo off
set EXE=12bitBin2TextUtil\Release\12bitBin2TextUtil.exe 
REM set EXE=12bitBin2TextUtil\Debug\12bitBin2TextUtil.exe 

set FCMP=fc /L
del ..\dataset\test*.txt
call %EXE%

call %EXE% ..\dataset\test1.bin ..\dataset\test1.txt
call %FCMP% ..\dataset\test1.out ..\dataset\test1.txt

call %EXE% ..\dataset\test2.bin ..\dataset\test2.txt
call %FCMP% ..\dataset\test2.out ..\dataset\test2.txt

call %EXE% ..\dataset\test3.bin ..\dataset\test3.txt
call %FCMP% ..\dataset\test3.out ..\dataset\test3.txt

call %EXE% ..\dataset\test4.bin ..\dataset\test4.txt
call %FCMP% ..\dataset\test4.out ..\dataset\test4.txt

call %EXE% ..\dataset\test5.bin ..\dataset\test5.txt
call %FCMP% ..\dataset\test5.out ..\dataset\test5.txt

pause
