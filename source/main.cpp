#include <nds.h>
#include <nds/arm9/input.h>
#include <gl2d.h>
#include <nf_lib.h>
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
	// powerOn(PM_BACKLIGHT_TOP);
	// powerOn(PM_BACKLIGHT_BOTTOM);

	defaultExceptionHandler();
	consoleDemoInit();
	videoSetMode(MODE_5_3D);
	// 	videoSetModeSub(MODE_5_2D);

	irqSet(IRQ_VBLANK, vblank);
	srand(time(NULL));
	glScreen2D();

	Player player;

	while(1) {
		if(processInput(&player)) break;
		glBegin2D();

		renderBackground();

		renderPlayer(&player);
		glEnd2D();
		glFlush(0);
		swiWaitForVBlank();
	}

	return 0;
}
