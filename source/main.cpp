#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <nf_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <time.h>

#include "player.h"
#include "map.h"

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

int initializeGame(int screenID) {
	if (screenID == 0) {
		powerOn(PM_BACKLIGHT_TOP);
		powerOff(PM_BACKLIGHT_BOTTOM);
	} else {
		powerOff(PM_BACKLIGHT_TOP);
		powerOn(PM_BACKLIGHT_BOTTOM);
	}

	// Turn on MODE 0 on the Top Screen
	NF_Set2D(screenID, 0);

	// Initialize the Tiled Backgrounds System on the Top Screen
	NF_InitTiledBgBuffers();	
	NF_InitTiledBgSys(screenID);

	// Initialize Sprite Buffers
	NF_InitSpriteBuffers();		
	NF_InitSpriteSys(screenID);

	// Load the starting tiled background
	NF_LoadTiledBg("backgrounds/starting", "bg", 256, 256);
	NF_CreateTiledBg(screenID, 3, "bg");

	u8 spriteLoadID = 0;
	u8 palleteLoadID = 0;
	NF_LoadSpriteGfx("sprites/walking_anim", spriteLoadID, 32, 32);	// Load our Sprite for the circle, cross and blank
	NF_LoadSpritePal("sprites/walking_anim", palleteLoadID);
	
	NF_VramSpriteGfx(screenID, 0, 0, false);	// Load the Gfx into VRAM - transfer all Sprites
	NF_VramSpritePal(screenID, 0, 0);		// Load the Palette into VRAM

	u8 spriteID = 0;
	// Spawn the character in the middle of the screen
	NF_CreateSprite(screenID, spriteID, spriteLoadID, palleteLoadID, CENTER_X - 32 / 2, CENTER_Y - 32 / 2);

	Player player;
	Map map;
	
	while(1) {
		if (processInput(&player, &map)) {
			break;
		}
		NF_SpriteFrame(screenID, spriteID, player.getAnimFrame());
		NF_HflipSprite(screenID, spriteID, !player.isFacingRight());

		NF_ScrollBg(screenID, 3, map.getX(), map.getY());
		
		NF_SpriteOamSet(screenID);		// Update NFLib's Sprite OAM System
		swiWaitForVBlank();		// Wait for the Vertical Blank
		if (screenID == 0) {
			oamUpdate(&oamMain);    // OAM update the top screen
		} else {
			oamUpdate(&oamSub);    // OAM update the bottom screen
		}
	}

	return initializeGame(screenID == 0 ? 1 : 0);
}

int main(int argc, char** argv) {
	// Set the Root Folder (this is black magic)
	NF_SetRootFolder("NITROFS");

	// Initialize sound
	soundEnable();
	NF_InitRawSoundBuffers();
	u8 backgroundMusic = 0;
	u8 footsteps = 1;
	NF_LoadRawSound("sounds/footsteps", footsteps, 11025, 0);
	NF_LoadRawSound("sounds/background", backgroundMusic, 22050, 0);
	NF_PlayRawSound(
		backgroundMusic,
		127, // volume (0-127)
		64, // pan (0-64-127)
		true, // loop?
		0 // loop start point
	);

	// This is a hack
	// Play the footstep song on a loop, just change if it's audible
	NF_PlayRawSound(
		footsteps,
		127, // volume (0-127)
		64, // pan (0-64-127)
		true, // loop?
		0 // loop start point
	);
	soundPause(footsteps);

	// 0 for top screen, 1 for bottom screen
	return initializeGame(0);
}
