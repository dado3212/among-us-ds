@echo off
cls

del /Q ..\resources\backgrounds\*

REM -g = include gfx data
REM -gTFF00FF = set FF00FF to the transparent color
REM -m = include map data
REM -mRtf = reduce tiles flipped (reg 8bpp)
REM -mLs = Map layout sbb
REM -p = include palette data
REM -ftb = file type bin
grit.exe ^
..\raw\backgrounds\*.bmp ^
-g ^
-gTFF00FF ^
-m ^
-mRtf ^
-mLs ^
-p ^
-ftb

for /f "delims==" %%F in ('dir /b *.bin') do ren "%%~nxF" "%%~nF"
move *.img ..\resources\backgrounds
move *.map ..\resources\backgrounds
move *.pal ..\resources\backgrounds
del /Q *.h

exit