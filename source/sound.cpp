#include <nds.h>
#include <filesystem.h>
#include <fat.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "errors.h"
#include "sound.h"

char* BUFFER_RAWSOUND[SLOTS_RAWSOUND];
RAWSOUND_INFO RAWSOUND[SLOTS_RAWSOUND];

void soundInit(void) {
	for (u8 n = 0; n < SLOTS_RAWSOUND; n++) {
		BUFFER_RAWSOUND[n] = NULL;
        // Defaults
		RAWSOUND[n].available = true;
		RAWSOUND[n].size = 0;
		RAWSOUND[n].freq = 0;
		RAWSOUND[n].format = SoundFormat_8Bit;
	}
}

void soundReset(void) {
	// Free buffers
	for (u8 n = 0; n < SLOTS_RAWSOUND; n++) {
		free(BUFFER_RAWSOUND[n]);
	}
	// Reinitialize to defaults
	soundInit();
}

void loadSound(const char* file, u16 id, u16 freq, SoundFormat format) {
	if ((id < 0) || (id >= SLOTS_RAWSOUND)) {
        throwError("Sound index out of range");
	}

	if (!RAWSOUND[id].available) {
        throwError("Don't load sound index into filled slot.");
	}

	free(BUFFER_RAWSOUND[id]);
	BUFFER_RAWSOUND[id] = NULL;

	FILE* file_id;
	char filename[256];

    // Load the filename
	sprintf(filename, "/%s.raw", file);
	file_id = fopen(filename, "rb");
    // If the file doesn't exist, error
	if (!file_id) {
        throwError("Filename doesn't exist.");
    }
    // Get the file size, and then start at the beginning
    fseek(file_id, 0, SEEK_END);
    u32 fileSize = ftell(file_id);
    rewind(file_id);
    // If it's too big
    if (fileSize > (1 << 18)) {
        throwError("File is too large to load, must be under 256kb.");
    }
    RAWSOUND[id].size = fileSize;
    // Alloc RAM space
    BUFFER_RAWSOUND[id] = (char*) calloc(fileSize, sizeof(char));
    // Not enough free ram
    if (BUFFER_RAWSOUND[id] == NULL) {		// Si no hay suficiente RAM libre
        throwError("Out of memory, can't be allocated.");
    }
    // Read it into ram.
    fread(BUFFER_RAWSOUND[id], 1, fileSize, file_id);
    // Close the file
	fclose(file_id);

	// Save sound effect metadata
	RAWSOUND[id].freq = freq;
	RAWSOUND[id].format = format;
	RAWSOUND[id].available = false;
}

void unloadSound(u8 id) {
	if ((id < 0) || (id >= SLOTS_RAWSOUND)) {
        throwError("Sound index out of range");
    }

	if (RAWSOUND[id].available) {
        throwError("Sound index not loaded.");
    }

	free(BUFFER_RAWSOUND[id]);
	BUFFER_RAWSOUND[id] = NULL;

    // Reset sound metadata
    RAWSOUND[id].size = 0;
	RAWSOUND[id].freq = 0;
	RAWSOUND[id].format = SoundFormat_8Bit;
	RAWSOUND[id].available = true;
}

u8 playSound(u8 id, u8 volume, u8 pan, bool loop, u16 loopfrom) {
	if ((id < 0) || (id >= SLOTS_RAWSOUND)) {
        throwError("Sound index out of range");
    }

	if (RAWSOUND[id].available) {
        throwError("Sound index not loaded.");
    }

	return soundPlaySample(
        BUFFER_RAWSOUND[id],
        RAWSOUND[id].format,
        RAWSOUND[id].size,
        RAWSOUND[id].freq,
        volume,
        pan,
        loop,
        loopfrom
    );
}
