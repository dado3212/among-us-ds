
#ifndef SOUND_H
#define SOUND_H

#include <nds.h>

// Define audio slots
#define SLOTS_RAWSOUND 32

// Define audio buffers
extern char* BUFFER_RAWSOUND[SLOTS_RAWSOUND];

// Define storage type
typedef struct {
	bool available;		// Free
	u32 size;			// Byte count of sound
	u16 freq;			// Frequency
	SoundFormat format;	// Sample format
} RAWSOUND_INFO;
extern RAWSOUND_INFO RAW_SOUND[SLOTS_RAWSOUND];	// Saved sound data

// Initialize buffers + sound structures for RAW files. Need to invoke
// this once before you can load or play sounds.
void soundInit(void);

// Reset all sound buffers. Clears everything in preparation for a new screen, etc.
void soundReset(void);

// Load a RAW file into RAM via FAT or EFS
//   file - name of the file without extension
//   id - slot for the sound effect, between 0 and 31
//   freq - frequency of the sound, en HZ (11025 or 22050 primarily)
//   format - sound format (SoundFormat_8Bit, SoundFormat_16Bit, SoundFormat_ADPCM)
void loadSound(const char* file, u16 id,  u16 freq, SoundFormat format);

// Clears ram of the given sound slot
void unloadSound(u8 id);

// Plays a loaded sound effect
//   id - slot of the sound effect, between 0 and 31
//   volume - volume to play, 0 - 127
//   pan - balance, 0 is left, 64 is center, 127 is right
//   loop - whether to loop
//   loopfrom - where to loop the song
extern u8 playSound(u8 id, u8 volume, u8 pan, bool loop, u16 loopfrom);

#endif
