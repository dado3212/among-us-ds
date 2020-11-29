@echo off
cls

del /Q ..\resources\backgrounds\*

grit.exe ..\raw\backgrounds\*.bmp -g -gTFF00FF -m -mRtf -mLs -p -ftb

for /f "delims==" %%F in ('dir /b *.bin') do ren "%%~nxF" "%%~nF"
move *.img ..\resources\backgrounds
move *.map ..\resources\backgrounds
move *.pal ..\resources\backgrounds
del /Q *.h

exit