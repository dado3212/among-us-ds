# Among Us DS 🎮

This current demo consist of:

- Using the arrow keys or touch screen to move the character around.
- Pressing start will swap between the top and bottom screens.
 
# Build Instructions
I ran this on Windows 10, so assume a mostly typical Windows 10 environment. If you're doing this on Unix, hopefully it works without any major changes. The only thing is that `make images` probably won't work. If you want to tweak the images, you'll need to extract the `grit` commands from the batch files.

Follow the instructions at https://devkitpro.org/wiki/Getting_Started to install devkitPro. Install it with libnds.

Git clone https://github.com/DeathCamel57/libgl2d_ds and follow the installation instructions.

Using the nflib directory from https://github.com/PolyMarsDev/Snake-DS, which should be a compiled version of Nightfox's Lib.

cd to this directory and run `make` and hope like heck that the build environment works and that it doesn't depend on some obscure Windows 10 thing that I had preinstalled.

# Images

For images, they need to be in Indexed mode (Image > Mode > Indexed Color), and then exported as .bmp (forced into 8bit). Then use the GRIT tool in the
NFLib checkout with Compile_Backgrounds.bat.

This has been simplified to just running `make images`.

# Sounds
To convert sound files to RAW format I used [Switch](http://www.nch.com.au/switch/plus.html) as recommended by NightFox. I exported as Mono, 8 bis signed at 11025hz for sound effects, and 22050hz for background music.

Skeld background music - https://www.youtube.com/watch?v=oRuoaG6ugW0
Footsteps sound - https://www.myinstants.com/instant/among-us-walking-35307/

## Thanks To

These Github repositories were a huge help at varying stages of the development cycle.
https://github.com/Mika-Westphal/FlappyNDS
https://sourceforge.net/projects/nflib/
https://github.com/PolyMarsDev/Snake-DS
https://gbatemp.net/threads/ds-programming-for-newbies.322106/page-8#post-4445495

## Sprites Thanks To
https://www.reddit.com/r/AmongUs/comments/ir6nl0/main_player_sprite_sheet_for_those_who_wanted_it/
https://among-us.fandom.com/wiki/The_Skeld
https://www.spriters-resource.com/pc_computer/amongus/sheet/139386/