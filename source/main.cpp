#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <time.h>

#include "lib.h"
#include "player.h"
#include "map.h"
#include "sound.h"
#include "errors.h"

#define CENTER_X 128 // 256/2
#define CENTER_Y 96 // 192/2
#define SPEED 2.5 // player speed

bool processInput(Player *player, Map* map) {
	scanKeys();

	// OTHER
	if (keysUp() & KEY_START) {
		// Swap screens
		return true;
	}
	// KEY_A

	// MOVEMENT
	int keys = keysHeld();
	float dx = 0;
	float dy = 0;
	// Handle directional key presses
	if (keys & KEY_UP) {
		dy = -1;
	}
	if (keys & KEY_DOWN) {
		dy = 1;
	}
	if (keys & KEY_RIGHT) {
		dx = 1;
	}
	if (keys & KEY_LEFT) {
		dx = -1;
	}
	// Handle touch controls
	if (keys & KEY_TOUCH) {
		touchPosition stylus;
		touchRead(&stylus);
		dx = stylus.px - CENTER_X;
		dy = stylus.py - CENTER_Y;
		float max = std::max(std::abs(dx), std::abs(dy));
		dx = dx/max;
		dy = dy/max;
	}
	bool moving = (dx != 0 || dy != 0);
	player->nextFrame(moving);
	if (moving) {
		 soundResume(1);
		if (dx != 0) {
			player->setFacingRight(dx > 0);
		}

		// Scale according to angle
		if (dx == 0) {
			map->setY(map->getY() + dy * SPEED);
		} else if (dy == 0) {
			map->setX(map->getX() + dx * SPEED);
		} else {
			float new_y = sqrt(1 / (1 + (pow(dx, 2.0) / pow(dy, 2.0))));
			if (dy < 0) {
				new_y = -1 * new_y;
			}
			float new_x = new_y * dx / dy;
			map->setY(map->getY() + new_y * SPEED);
			map->setX(map->getX() + new_x * SPEED);
		}
	} else {
		soundPause(1);
	}
	return false;
}

int main(int argc, char** argv) {
	// Set the Root Folder (this is black magic)
	SetRootFolder("NITROFS");

	// if (nitroFSInit(NULL)) {
	// 	// NitroFS ok
	// 	// Si es correcto, cambia al ROOT del NitroFS
	// 	chdir("nitro:/");
	// } else {
	// 	throwError("Failed to load NitroFS directory. Your ROM flashcard is probably set up differently.");
	// }

	// Initialize sound
	soundEnable();
	soundInit();
	u8 backgroundMusic = 0;
	u8 footsteps = 1;
	loadSound("sounds/footsteps", footsteps, 11025, SoundFormat_8Bit);
	loadSound("sounds/background", backgroundMusic, 22050, SoundFormat_8Bit);
	playSound(
		backgroundMusic,
		0, // volume (0-127) // MUTED, REMOVE THIS
		64, // pan (0-64-127)
		true, // loop?
		0 // loop start point
	);

	// This is a hack
	// Play the footstep song on a loop, just change if it's audible
	playSound(
		footsteps,
		127, // volume (0-127)
		64, // pan (0-64-127)
		true, // loop?
		0 // loop start point
	);
	soundPause(footsteps);

	// 0 for top screen, 1 for bottom screen
	// if (screenID == 0) {
	// 	powerOn(PM_BACKLIGHT_TOP);
	// 	// powerOff(PM_BACKLIGHT_BOTTOM);
	// } else {
	// 	// powerOff(PM_BACKLIGHT_TOP);
	// 	powerOn(PM_BACKLIGHT_BOTTOM);
	// }
	int screenID = 0;

	// Turn on MODE 0 on the Top Screen
	Set2D(screenID, 0);

	// Initialize the Tiled Backgrounds System on the Top Screen
	InitTiledBgBuffers();	
	InitTiledBgSys(screenID);

	// Initialize Sprite Buffers
	InitSpriteBuffers();		
	InitSpriteSys(screenID);

	swiWaitForVBlank();

	// Load the starting tiled background
	LoadTiledBg("backgrounds/skeld_13", "skeld_13", 256, 256);
	CreateTiledBg(screenID, 3, "skeld_13");

	LoadTiledBg("backgrounds/skeld_14", "skeld_14", 256, 256);
	// CreateTiledBg(1, 3, "skeld_14");

	u8 spriteLoadID = 0;
	u8 palleteLoadID = 0;
	LoadSpriteGfx("sprites/walking_anim", spriteLoadID, 32, 32);	// Load our Sprite for the circle, cross and blank
	LoadSpritePal("sprites/walking_anim", palleteLoadID);
	
	VramSpriteGfx(screenID, 0, 0, false);	// Load the Gfx into VRAM - transfer all Sprites
	VramSpritePal(screenID, 0, 0);		// Load the Palette into VRAM

	u8 spriteID = 0;
	// Spawn the character in the middle of the screen
	CreateSprite(screenID, spriteID, spriteLoadID, palleteLoadID, CENTER_X - 32 / 2, CENTER_Y - 32 / 2);

	Player player;
	Map map;

	consoleDemoInit();
	std::vector<std::string> backgrounds = map.getBackgrounds();
	iprintf("%s, %s, %s, %s\n", backgrounds[0].c_str(), backgrounds[1].c_str(), backgrounds[2].c_str(), backgrounds[3].c_str());
	
	while(1) {
		if (processInput(&player, &map)) {
			break;
		}
		SpriteFrame(screenID, spriteID, player.getAnimFrame());
		HflipSprite(screenID, spriteID, !player.isFacingRight());

		ScrollBg(screenID, 3, map.getX(), map.getY());
		
		SpriteOamSet(screenID);		// Update NFLib's Sprite OAM System
		swiWaitForVBlank();		// Wait for the Vertical Blank
		if (screenID == 0) {
			oamUpdate(&oamMain);    // OAM update the top screen
		} else {
			oamUpdate(&oamSub);    // OAM update the bottom screen
		}
	}

	return 0;
}
