#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <nf_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include "player.h"
#include "map.h"

#define CENTER_X 128 // 256/2
#define CENTER_Y 96 // 192/2

void processInput(Player *player, Map* map) {
	scanKeys();
	int keys = keysHeld();
	bool moving = false;
	// Handle directional key presses
	if (keys & KEY_UP) {
		moving = true;
		map->setY(map->getY() - 2);
	}
	if (keys & KEY_DOWN) {
		moving = true;
		map->setY(map->getY() + 2);
	}
	if (keys & KEY_RIGHT) {
		moving = true;
		map->setX(map->getX() + 2);
		player->setFacingRight(true);
	}
	if (keys & KEY_LEFT) {
		moving = true;
		map->setX(map->getX() - 2);
		player->setFacingRight(false);
	}
	// Handle touch controls
	if (keys & KEY_TOUCH) {
		moving = true;

		touchPosition stylus;
		touchRead(&stylus);
		float xd = stylus.px - CENTER_X;
		float yd = stylus.py - CENTER_Y;
		float max = std::max(std::abs(xd), std::abs(yd));
		xd = xd/max * 2;
		yd = yd/max * 2;
		player->setFacingRight(xd >= 0);
		map->setX(map->getX() + xd);
		map->setY(map->getY() + yd);
	}
	player->nextFrame(moving);
	// KEY_A
	// KEY_START
}

int main(int argc, char** argv){

	powerOff(PM_BACKLIGHT_BOTTOM);

	u8 screenID = 0; // 0 is top, 1 is bottom

	// Turn on MODE 0 on the Top Screen
	NF_Set2D(screenID, 0);
	// Set the Root Folder (this is black magic)
	NF_SetRootFolder("NITROFS");

	// Initialize the Tiled Backgrounds System on the Top Screen
	NF_InitTiledBgBuffers();	
	NF_InitTiledBgSys(screenID);

	// Initialize Sprite Buffers
	NF_InitSpriteBuffers();		
	NF_InitSpriteSys(screenID);

	// Load the starting tiled background
	NF_LoadTiledBg("backgrounds/starting", "bg", 256, 256);
	NF_CreateTiledBg(screenID, 3, "bg");

	u8 spriteLoadID= 0;
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
		processInput(&player, &map);
		NF_SpriteFrame(screenID, spriteID, player.getAnimFrame());
		NF_HflipSprite(screenID, spriteID, !player.isFacingRight());

		NF_ScrollBg(screenID, 3, map.getX(), map.getY());
		
		NF_SpriteOamSet(screenID);		// Update NFLib's Sprite OAM System
		swiWaitForVBlank();		// Wait for the Vertical Blank
		oamUpdate(&oamMain);    // OAM update the top screen
	}

	return 0;
}
