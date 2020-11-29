#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <nf_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include "player.h"

bool processInput(Player* player) {
	scanKeys();
	int keys = keysHeld();
	bool moving = false;
	if(keys & KEY_UP) {
		moving = true;
		player->setY(player->getY() - 2);
	}
	if(keys & KEY_DOWN) {
		moving = true;
		player->setY(player->getY() + 2);
	}
	if(keys & KEY_RIGHT) {
		moving = true;
		player->setX(player->getX() + 2);
		player->setFacingRight(true);
	}
	if(keys & KEY_LEFT) {
		moving = true;
		player->setX(player->getX() - 2);
		player->setFacingRight(false);
	}
	if (keys & KEY_START) {
		return true;
	}
	if (keys & KEY_TOUCH) {
		touchPosition touch;
		touchRead(&touch);
		player->setX(touch.px);
		player->setY(touch.py);
	}
	player->nextFrame(moving);
	// KEY_A
	// KEY_START
	return false;
}

void vblank() {}

int main(int argc, char** argv){

	// powerOn(PM_BACKLIGHT_TOP);
	// powerOn(PM_BACKLIGHT_BOTTOM);

	// defaultExceptionHandler();
	// consoleDemoInit();

	u8 screenID = 0; // 0 is top, 1 is bottom

	NF_Set2D(screenID, 0);				// Turn on MODE 0 on the Top Screen
	NF_SetRootFolder("NITROFS");	// Set the Root Folder

	NF_InitTiledBgBuffers();	// Initialize the Tiled Backgrounds System on the Top Screen
	NF_InitTiledBgSys(screenID);

	// Load the starting tiled background
	NF_LoadTiledBg("backgrounds/starting", "bg", 256, 256);
	NF_CreateTiledBg(screenID, 3, "bg");

	NF_InitSpriteBuffers();		// Initialize Sprite Buffers
	NF_InitSpriteSys(screenID);		// Initialize Top Screen SpriteSystem

	u8 spriteLoadID= 0;
	u8 palleteLoadID = 0;
	NF_LoadSpriteGfx("sprites/walking_anim", spriteLoadID, 32, 32);	// Load our Sprite for the circle, cross and blank
	NF_LoadSpritePal("sprites/walking_anim", palleteLoadID);
	
	NF_VramSpriteGfx(screenID, 0, 0, false);	// Load the Gfx into VRAM - transfer all Sprites
	NF_VramSpritePal(screenID, 0, 0);		// Load the Palette into VRAM

	u8 spriteID = 0;
	NF_CreateSprite(screenID, spriteID, spriteLoadID, palleteLoadID, 50, 50);		// Create a Sprite in the designated spot

	Player player;
	
	while(1){
		if(processInput(&player)) break;
		NF_CreateSprite(screenID, spriteID, spriteLoadID, palleteLoadID, player.getX(), player.getY());
		NF_SpriteFrame(screenID, spriteID, player.getAnimFrame());
		NF_HflipSprite(screenID, spriteID, !player.isFacingRight());
		
		NF_SpriteOamSet(screenID);		// Update NFLib's Sprite OAM System
		swiWaitForVBlank();		// Wait for the Vertical Blank
		oamUpdate(&oamMain);    // OAM update the top screen
		
	}

	return 0;
}
