# Among Us DS 🎮

This example consist of:

- Touching the bottom screen and moving one sprite. The upper one will mirror your touch position
- Using the Arrow Keys to move the second bottom sprite

 
# Build Instructions
Follow the instructions at https://devkitpro.org/wiki/Getting_Started to install devkitPro. Install it with libnds.

Git clone https://github.com/DeathCamel57/libgl2d_ds and follow the installation instructions.

Using the nflib directory from https://github.com/PolyMarsDev/Snake-DS, which should be a compiled version of Nightfox's Lib.

cd to this directory and run `make` and hope like heck that the build environment works and that it doesn't depend on some obscure Windows 10 thing that I had preinstalled.

# Images

For images, they need to be in Indexed mode (Image > Mode > Indexed Color), and then exported as .bmp (forced into 8bit). Then use the GRIT tool in the
NFLib checkout with Compile_Backgrounds.bat.

## Thanks To

These Github repositories were a huge help at varying stages of the development cycle.
https://github.com/Mika-Westphal/FlappyNDS
https://sourceforge.net/projects/nflib/
https://github.com/PolyMarsDev/Snake-DS
https://gbatemp.net/threads/ds-programming-for-newbies.322106/page-8#post-4445495

## Sprites Thanks To
https://www.reddit.com/r/AmongUs/comments/ir6nl0/main_player_sprite_sheet_for_those_who_wanted_it/
https://among-us.fandom.com/wiki/The_Skeld