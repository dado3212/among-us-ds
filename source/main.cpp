#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include "player.h"
#include "skeld.h"

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
	defaultExceptionHandler();
	consoleDemoInit();
	videoSetMode(MODE_5_3D);
	irqSet(IRQ_VBLANK, vblank);
	srand(time(NULL));
	glScreen2D();

	Player player;

	while(1) {
		if(processInput(&player)) break;
		glBegin2D();

		renderBackground();

		renderPlayer(&player);
		// checkHitbox(&player, &pipes);
		// renderPipes(&pipes);
		glEnd2D();
		glFlush(0);
		swiWaitForVBlank();
	}

	// touchPosition touch;

	// //Set Upper Screen
	// videoSetMode(MODE_5_2D);
    // vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	// // videoSetMode(MODE_0_2D);
	// // vramSetBankA(VRAM_A_MAIN_SPRITE);
	// oamInit(&oamMain, SpriteMapping_1D_128, false);

	// //Set Bottom Screen
	// videoSetModeSub(MODE_0_2D);
	// vramSetBankD(VRAM_D_SUB_SPRITE);

	// int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	// dmaCopy(skeldBitmap, bgGetGfxPtr(bg3), 256*256);
	// dmaCopy(skeldPal, BG_PALETTE, 256*2);
	
	// oamInit(&oamSub, SpriteMapping_1D_128, false);

	// spriteKeyDown.gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_Bmp);
	// spriteTouchDown.gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_Bmp);

	// while(1){

	// 	scanKeys();
	// 	int key = keysHeld();

	// 	if(key & KEY_UP)
	// 		spriteKeyDown.y -= speed;
	// 	if(key & KEY_DOWN)
	// 		spriteKeyDown.y += speed;
	// 	if(key & KEY_RIGHT)
	// 		spriteKeyDown.x += speed;
	// 	if(key & KEY_LEFT)
	// 			spriteKeyDown.x -= speed;
	// 	if(key & KEY_TOUCH) {
	// 		touchRead(&touch);
	// 			spriteTouchDown.x = touch.px;
	// 			spriteTouchDown.y = touch.py;
	// 	}

	// 	createSquare(spriteKeyDown, &oamMain, 1);
	// 	createSquare(spriteTouchDown, &oamSub, 2);

	// 	swiWaitForVBlank(); // clean the screen
	// 	oamUpdate(&oamMain); // (sub) updates the oam before so VBlank can update the graphics on screen
	// 	oamUpdate(&oamSub); // (sub) updates the oam before so VBlank can update the graphics on screen
	// }

	return 0;
}
