#include <nds.h>
#include <filesystem.h>
#include <fat.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "nf_basic.h"
#include "sound.h"

char* BUFFER_RAWSOUND[SLOTS_RAWSOUND];
RAWSOUND_INFO RAWSOUND[SLOTS_RAWSOUND];

void soundInit(void) {
	for (u8 n = 0; n < SLOTS_RAWSOUND; n++) {
		BUFFER_RAWSOUND[n] = NULL;			// Inicializa puntero
		RAWSOUND[n].available = true;		// Disponibilidad del slot
		RAWSOUND[n].size = 0;				// Tama�o del archivo
		RAWSOUND[n].freq = 0;				// Frecuencia del sample
		RAWSOUND[n].format = SoundFormat_8Bit;				// Formato del sample
	}
}

void soundReset(void) {

	// Borra los datos de los buffers de sonido en RAW
	for (u8 n = 0; n < SLOTS_RAWSOUND; n++) {
		free(BUFFER_RAWSOUND[n]);			// Borra el contenido puntero
	}

	// Reinicia las estructuras de datos
	soundInit();

}

void loadSound(const char* file, u16 id, u16 freq, SoundFormat format) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= SLOTS_RAWSOUND)) {
		NF_Error(106, "Raw Sound", SLOTS_RAWSOUND);
	}

	// Verifica si la Id esta libre
	if (!RAWSOUND[id].available) {
		NF_Error(109, "Raw Sound", id);
	}

	// Vacia los buffers que se usaran
	free(BUFFER_RAWSOUND[id]);
	BUFFER_RAWSOUND[id] = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Carga el archivo .RAW
	sprintf(filename, "%s/%s.raw", NF_ROOTFOLDER, file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tama�o del archivo
		fseek(file_id, 0, SEEK_END);
		RAWSOUND[id].size = ftell(file_id);
		rewind(file_id);
		// Si excede del tama�o maximo, error
		if (RAWSOUND[id].size > (1 << 18)) NF_Error(116, filename, (1 << 18));
		// Reserva el espacio en RAM
		BUFFER_RAWSOUND[id] = (char*) calloc (RAWSOUND[id].size, sizeof(char));
		if (BUFFER_RAWSOUND[id] == NULL) {		// Si no hay suficiente RAM libre
			NF_Error(102, NULL, RAWSOUND[id].size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(BUFFER_RAWSOUND[id], 1, RAWSOUND[id].size, file_id);
	} else {	// Si el archivo no existe...
		NF_Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo
	// swiWaitForVBlank();		// Espera al cierre del archivo (Usar en caso de corrupcion de datos)

	// Guarda los parametros del archivo de sonido
	RAWSOUND[id].freq = freq;		// Frequencia del sample (en Hz)
	RAWSOUND[id].format = format;	// Formato del sample (0 - > 8 bits, 1 - > 16 bits, 2 -> ADPCM)

	// Y marca esta ID como usada
	RAWSOUND[id].available = false;
}

void unloadSound(u8 id) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= SLOTS_RAWSOUND)) NF_Error(106, "RAW Sound", SLOTS_RAWSOUND); 

	// Verifica si el sonido existe
	if (RAWSOUND[id].available) NF_Error(110, "RAW Sound", id);

	// Vacia los buffers de la Id. seleccionada
	free(BUFFER_RAWSOUND[id]);
	BUFFER_RAWSOUND[id] = NULL;

	// Resetea las variables
	RAWSOUND[id].freq = 0;		// Frequencia del sample (en Hz)
	RAWSOUND[id].format = SoundFormat_8Bit;	// Formato del sample (0 - > 8 bits, 1 - > 16 bits, 2 -> ADPCM)

	// Y marca esta ID como libre
	RAWSOUND[id].available = true;
}

u8 playSound(u8 id, u8 volume, u8 pan, bool loop, u16 loopfrom) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= SLOTS_RAWSOUND)) NF_Error(106, "RAW Sound", SLOTS_RAWSOUND); 

	// Verifica si el sonido existe
	if (RAWSOUND[id].available) NF_Error(110, "RAW Sound", id);

	return soundPlaySample(BUFFER_RAWSOUND[id], RAWSOUND[id].format, RAWSOUND[id].size, RAWSOUND[id].freq, volume, pan, loop, loopfrom);

}
