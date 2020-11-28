#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <nf_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include "player.h"

void renderPlayer(Player* player) {
	glBoxFilled(player->getX(), player->getY() - 15, player->getX() + 15, player->getY(), RGB15(0, 255, 0));
}

void renderBackground() {
	glBoxFilled(0, 0, 256, 192, RGB15(50, 0, 0));
}

bool processInput(Player* player) {
	scanKeys();
	int keys = keysHeld();
	if(keys & KEY_UP) {
		player->setY(player->getY() - 2);
	}
	if(keys & KEY_DOWN) {
		player->setY(player->getY() + 2);
	}
	if(keys & KEY_RIGHT) {
		player->setX(player->getX() + 2);
	}
	if(keys & KEY_LEFT) {
		player->setX(player->getX() - 2);
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
	// KEY_A
	// KEY_START
	return false;
}

void vblank() {}

int main(int argc, char** argv){
	NF_Set2D(0, 0);				// Turn on MODE 0 on the Top Screen
	NF_SetRootFolder("NITROFS");	// Set the Root Folder

	NF_InitTiledBgBuffers();	// Initialize the Tiled Backgrounds System on the Top Screen
	NF_InitTiledBgSys(0);

	// Load the starting tiled background
	NF_LoadTiledBg("backgrounds/starting", "bg", 256, 256);
	NF_CreateTiledBg(0, 3, "bg");

	NF_InitSpriteBuffers();		// Initialize Sprite Buffers
	NF_InitSpriteSys(0);		// Initialize Top Screen SpriteSystem

	NF_LoadSpriteGfx("sprites/Sprite_TicTacToe", 0, 32, 32);	// Load our Sprite for the circle, cross and blank
	NF_LoadSpritePal("sprites/Sprite_TicTacToe", 0);
	
	NF_VramSpriteGfx(0, 0, 0, false);	// Load the Gfx into VRAM - transfer all Sprites
	NF_VramSpritePal(0, 0, 0);		// Load the Palette into VRAM

	u8 spriteID = 0;
	NF_CreateSprite(0, spriteID, 0, 0, 50, 50);		// Create a Sprite in the designated spot
	NF_SpriteFrame(0, 3, 0); // Set its Frame to a blank one
	
	while(1){
		NF_SpriteFrame(0, spriteID, 1);
		NF_SpriteOamSet(0);		// Update NFLib's Sprite OAM System
		swiWaitForVBlank();		// Wait for the Vertical Blank
		oamUpdate(&oamMain);
		
	}
	// // powerOn(PM_BACKLIGHT_TOP);
	// // powerOn(PM_BACKLIGHT_BOTTOM);

	// defaultExceptionHandler();
	// consoleDemoInit();
	// videoSetMode(MODE_5_3D);
	// // 	videoSetModeSub(MODE_5_2D);

	// irqSet(IRQ_VBLANK, vblank);
	// srand(time(NULL));
	// glScreen2D();

	// Player player;

	// while(1) {
	// 	if(processInput(&player)) break;
	// 	glBegin2D();

	// 	renderBackground();

	// 	renderPlayer(&player);
	// 	glEnd2D();
	// 	glFlush(0);
	// 	swiWaitForVBlank();
	// }

	return 0;
}
