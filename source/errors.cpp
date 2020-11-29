#include <nds.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "errors.h"

[[ noreturn ]] void throwError(const char* text) {
    // Prepare bottom screen for throwing up text
	consoleDemoInit();
	consoleClear();
	setBrightness(3, 0);

	iprintf("Error: %s\n", text);

	while (1) {
		swiWaitForVBlank();
	}
}