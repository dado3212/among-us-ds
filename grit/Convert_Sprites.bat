@echo off
cls

del /Q ..\resources\sprites\*

grit.exe ..\raw\sprites\*.bmp -g -gTFF00FF -p -pS -ftb -O%sprite_pallete%

for /f "delims==" %%F in ('dir /b *.bin') do ren "%%~nxF" "%%~nF"
move *.img ..\resources\sprites
move *.pal ..\resources\sprites
del /Q *.map
del /Q *.h

exit