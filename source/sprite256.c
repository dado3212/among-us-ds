
// NightFox LIB - Funciones de Sprites a 256 colores
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413





// Includes devKitPro
#include <nds.h>
#include <filesystem.h>
#include <fat.h>

// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

// Includes propios
#include "basic.h"
#include "2d.h"
#include "sprite256.h"



// Define los Buffers para almacenar los Sprites
char* BUFFER_SPR256GFX[SLOTS_SPR256GFX];
char* BUFFER_SPR256PAL[SLOTS_SPR256PAL];

// Define la estructura de datos de los Graficos de los Sprites
TYPE_SPR256GFX_INFO SPR256GFX[SLOTS_SPR256GFX];
// Define la estructura de datos de las Paletas de los Sprites
TYPE_SPR256PAL_INFO SPR256PAL[SLOTS_SPR256PAL];

// Define la estructura de Gfx en VRAM
TYPE_SPR256VRAM_INFO SPR256VRAM[2][128];
// Datos de paletas de Sprites en VRAM (en uso, slot en ram, etc)
TYPE_SPRPALSLOT_INFO SPRPALSLOT[2][16];

// Define la estructura de datos del OAM (Sprites)
TYPE_SPRITEOAM_INFO SPRITEOAM[2][128];		// 2 pantallas, 128 sprites

// Define la esturctura de control de la VRAM para Sprites
TYPE_SPRVRAM_INFO SPRVRAM[2];		// Informacion VRAM de Sprites en ambas pantallas





// Funcion InitSpriteBuffers()
void InitSpriteBuffers(void) {

	u16 n = 0;	// Variable comun

	// Inicializa Buffers de GFX
	for (n = 0; n < SLOTS_SPR256GFX; n ++) {
		BUFFER_SPR256GFX[n] = NULL;			// Inicializa puntero
		SPR256GFX[n].size = 0;				// Tama�o (en bytes) del grafico (GFX)
		SPR256GFX[n].width = 0;				// Ancho del Gfx
		SPR256GFX[n].height = 0;				// Altura del Gfx
		SPR256GFX[n].available = true;		// Disponibilidat del Slot
	}

	// Inicializa Buffers de PAL
	for (n = 0; n < SLOTS_SPR256PAL; n ++) {
		BUFFER_SPR256PAL[n] = NULL;		// Inicializa puntero
		SPR256PAL[n].size = 0;			// Tama�o (en bytes) de la paleta (PAL)
		SPR256PAL[n].available = true;	// Disponibilidat del Slot
	}

}



// Funcion ResetSpriteBuffers()
void ResetSpriteBuffers(void) {

	u16 n = 0;	// Variable comun

	// Borra los Buffers de GFX
	for (n = 0; n < SLOTS_SPR256GFX; n ++) {
		free(BUFFER_SPR256GFX[n]);
	}

	// Borra los Buffers de PAL
	for (n = 0; n < SLOTS_SPR256PAL; n ++) {
		free(BUFFER_SPR256PAL[n]);
	}

	// Reinicia el sistema de Sprites
	InitSpriteBuffers();

}



// Funcion InitSpriteSys();
void InitSpriteSys(int screen, ...) {

	// Analiza los parametros variables de la funcion
	va_list options;
	va_start(options, screen);
	u8 mode = va_arg(options, int);
	va_end(options);


	// Variables
	u8 n = 0;	// Uso comun

	// Inicializa la estructura de Gfx en VRAM
	// y la estructura de datos del OAM (Sprites)
	for (n = 0; n < 128; n ++) {	// 128 sprites
		// Gfx en la VRAM (128 Gfx x pantalla)
		SPR256VRAM[screen][n].size = 0;				// Tama�o (en bytes) del Gfx
		SPR256VRAM[screen][n].width = 0;				// Ancho del Gfx
		SPR256VRAM[screen][n].height = 0;			// Altura del Gfx
		SPR256VRAM[screen][n].address = 0;			// Posicion en la VRAM
		SPR256VRAM[screen][n].ramid = 0;				// Numero de Slot en RAM del que provienes
		SPR256VRAM[screen][n].framesize = 0;			// Tama�o del frame (en bytes)
		SPR256VRAM[screen][n].lastframe = 0;			// Ultimo frame
		SPR256VRAM[screen][n].keepframes = false;	// Si es un Sprite animado, debes de mantener los frames en RAM ?
		SPR256VRAM[screen][n].inuse = false;			// Esta en uso ?
		// OAM (128 Sprites x pantalla)
		SPRITEOAM[screen][n].index = n;			// Numero de Sprite (Index = N)
		SPRITEOAM[screen][n].x = 0;				// Coordenada X del Sprite (0 por defecto)
		SPRITEOAM[screen][n].y = 0;				// Coordenada Y del Sprite (0 por defecto)
		SPRITEOAM[screen][n].layer = 0;			// Prioridad en las capas (0 por defecto)
		SPRITEOAM[screen][n].pal = 0;			// Paleta que usaras (0 por defecto)
		SPRITEOAM[screen][n].size = SpriteSize_8x8;					// Tama�o del Sprite (macro) (8x8 por defecto)
		SPRITEOAM[screen][n].color = SpriteColorFormat_256Color;		// Modo de color (macro) (256 colores)
		SPRITEOAM[screen][n].gfx = NULL;				// Puntero al grafico usado
		SPRITEOAM[screen][n].rot = -1;				// Id de rotacion (-1 por defecto) (0 - 31 Id de rotacion)
		SPRITEOAM[screen][n].doublesize = false;		// Usar el "double size" al rotar ? ("NO" por defecto)
		SPRITEOAM[screen][n].hide = true;			// Ocultar el Sprite ("SI" por defecto)
		SPRITEOAM[screen][n].hflip = false;			// Volteado Horizontal ("NO" por defecto)
		SPRITEOAM[screen][n].vflip = false;			// Volteado Vertical ("NO" por defecto)
		SPRITEOAM[screen][n].mosaic = false;			// Mosaico ("NO" por defecto)
		SPRITEOAM[screen][n].gfxid = 0;				// Numero de Gfx usado
		SPRITEOAM[screen][n].frame = 0;				// Frame actual
		SPRITEOAM[screen][n].framesize = 0;			// Tama�o del frame (en bytes)
		SPRITEOAM[screen][n].lastframe = 0;			// Ultimo frame
		SPRITEOAM[screen][n].created = false;		// Esta creado este sprite ?
	}

	// Inicializa la estructura de datos de la VRAM de Sprites
	if (mode == 128) {
		SPRVRAM[screen].max = 131072;
	} else {
		SPRVRAM[screen].max = 65536;
	}
	SPRVRAM[screen].free = SPRVRAM[screen].max;		// Memoria VRAM libre (64kb/128kb)
	SPRVRAM[screen].last = 0;							// Ultima posicion usada
	SPRVRAM[screen].deleted = 0;							// Ningun Gfx borrado
	SPRVRAM[screen].fragmented = 0;						// Memoria VRAM fragmentada
	SPRVRAM[screen].inarow = SPRVRAM[screen].max;		// Memoria VRAM contigua
	for (n = 0; n < 128; n ++) {
		SPRVRAM[screen].pos[n] = 0;		// Posicion en VRAM para reusar despues de un borrado
		SPRVRAM[screen].size[n] = 0;		// Tama�o del bloque libre para reusar
	}

	// Inicializa los datos de las paletas
	for (n = 0; n < 16; n ++) {
		SPRPALSLOT[screen][n].inuse = false;
		SPRPALSLOT[screen][n].ramslot = 0;
	}

	// Configura el Motor 2D y VRAM segun la pantalla de destino
	if (screen == 0) {

		// Configura la pantalla 0
		REG_DISPCNT |= (DISPLAY_SPR_ACTIVE);			// Activa los Sprites en la pantalla superior
		vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);	// Banco B de la VRAM para Sprites (128kb)
		memset((void*)0x06400000, 0, 131072);			// Borra el contenido del banco B
		SPRVRAM[screen].next = (0x06400000);			// Guarda la primera posicion de VRAM para Gfx
		vramSetBankF(VRAM_F_LCD);						// Banco F de la VRAM para paletas extendidas (Sprites) (8kb de 16kb)
		memset((void*)0x06890000, 0, 8192);				// Borra el contenido del banco F
		if (mode == 128) {
			oamInit(&oamMain, SpriteMapping_1D_128, true);	// Inicializa el OAM (Mapeado de 128 bytes, Paletas extendidas)
		} else {
			oamInit(&oamMain, SpriteMapping_1D_64, true);	// Inicializa el OAM (Mapeado de 64 bytes, Paletas extendidas)
		}

	} else {

		// Configura la pantalla 1
		REG_DISPCNT_SUB |= (DISPLAY_SPR_ACTIVE);		// Activa los Sprites en la pantalla inferior
		vramSetBankD(VRAM_D_SUB_SPRITE);				// Banco D de la VRAM para Sprites (128kb)
		memset((void*)0x06600000, 0, 131072);			// Borra el contenido del banco D
		SPRVRAM[screen].next = (0x06600000);			// Guarda la primera posicion de VRAM para Gfx
		vramSetBankI(VRAM_I_LCD);						// Banco I de la VRAM para paletas extendidas (Sprites) (8kb de 16kb)
		memset((void*)0x068A0000, 0, 8192);				// Borra el contenido del banco I
		if (mode == 128) {
			oamInit(&oamSub, SpriteMapping_1D_128, true);	// Inicializa el OAM (Mapeado de 128 bytes, Paletas extendidas)
		} else {
			oamInit(&oamSub, SpriteMapping_1D_64, true);	// Inicializa el OAM (Mapeado de 64 bytes, Paletas extendidas)
		}

	}

}



// Funcion LoadSpriteGfx();
void LoadSpriteGfx(const char* file, u16 id,  u16 width, u16 height) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= SLOTS_SPR256GFX)) {
		Error(106, "Sprite GFX", SLOTS_SPR256GFX);
	}

	// Verifica si la Id esta libre
	if (!SPR256GFX[id].available) {
		Error(109, "Sprite GFX", id);
	}

	// Vacia los buffers que se usaran
	free(BUFFER_SPR256GFX[id]);
	BUFFER_SPR256GFX[id] = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Carga el archivo .IMG
	sprintf(filename, "/%s.img", file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tama�o del archivo
		fseek(file_id, 0, SEEK_END);
		SPR256GFX[id].size = ftell(file_id);
		rewind(file_id);
		// Reserva el espacio en RAM
		BUFFER_SPR256GFX[id] = (char*) calloc (SPR256GFX[id].size, sizeof(char));
		if (BUFFER_SPR256GFX[id] == NULL) {		// Si no hay suficiente RAM libre
			Error(102, NULL, SPR256GFX[id].size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(BUFFER_SPR256GFX[id], 1, SPR256GFX[id].size, file_id);
	} else {	// Si el archivo no existe...
		Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Guarda las medidas del grafico
	SPR256GFX[id].width = width;		// Ancho del Gfx
	SPR256GFX[id].height = height;	// Altura del Gfx

	// Y marca esta ID como usada
	SPR256GFX[id].available = false;

}



// Funcion UnloadSpriteGfx();
void UnloadSpriteGfx(u16 id) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= SLOTS_SPR256GFX)) {
		Error(106, "Sprite GFX", SLOTS_SPR256GFX);
	}

	// Verifica si la Id esta libre
	if (SPR256GFX[id].available) {
		Error(110, "Sprite GFX", id);
	}

	// Vacia el buffer
	free(BUFFER_SPR256GFX[id]);

	// Y reinicia las variables
	BUFFER_SPR256GFX[id] = NULL;			// Inicializa puntero
	SPR256GFX[id].size = 0;				// Tama�o (en bytes) del grafico (GFX)
	SPR256GFX[id].width = 0;				// Ancho del Gfx
	SPR256GFX[id].height = 0;			// Altura del Gfx
	SPR256GFX[id].available = true;		// Disponibilidat del Slot

}



// Funcion LoadSpritePal();
void LoadSpritePal(const char* file, u8 id) {

	// Variable temporal del tama�o de la paleta
	u32 pal_size = 0;

	// Verifica el rango de Id's
	if ((id < 0) || (id >= SLOTS_SPR256PAL)) {
		Error(106, "Sprite PAL", SLOTS_SPR256PAL);
	}

	// Verifica si la Id esta libre
	if (!SPR256PAL[id].available) {
		Error(109, "Sprite PAL", id);
	}

	// Vacia los buffers que se usaran
	free(BUFFER_SPR256PAL[id]);
	BUFFER_SPR256PAL[id] = NULL;

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Variable para almacenar el path al archivo
	char filename[256];

	// Carga el archivo .PAL
	sprintf(filename, "/%s.pal", file);
	file_id = fopen(filename, "rb");
	if (file_id) {	// Si el archivo existe...
		// Obten el tama�o del archivo
		fseek(file_id, 0, SEEK_END);
		pal_size = ftell(file_id);
		SPR256PAL[id].size = pal_size;
		rewind(file_id);
		// Si el tama�o es inferior a 512 bytes, ajustalo
		if (SPR256PAL[id].size < 512) SPR256PAL[id].size = 512;
		// Reserva el espacio en RAM
		BUFFER_SPR256PAL[id] = (char*) calloc (SPR256PAL[id].size, sizeof(char));
		if (BUFFER_SPR256PAL[id] == NULL) {		// Si no hay suficiente RAM libre
			Error(102, NULL, SPR256PAL[id].size);
		}
		// Lee el archivo y ponlo en la RAM
		fread(BUFFER_SPR256PAL[id], 1, pal_size, file_id);
	} else {	// Si el archivo no existe...
		Error(101, filename, 0);
	}
	fclose(file_id);		// Cierra el archivo

	// Y marca esta ID como usada
	SPR256PAL[id].available = false;

}



// Funcion UnloadSpritePal();
void UnloadSpritePal(u8 id) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= SLOTS_SPR256PAL)) {
		Error(106, "Sprite PAL", SLOTS_SPR256PAL);
	}

	// Verifica si la Id esta libre
	if (SPR256PAL[id].available) {
		Error(110, "Sprite PAL", id);
	}

	// Vacia el buffer
	free(BUFFER_SPR256PAL[id]);

	// Y reinicia las variables
	BUFFER_SPR256PAL[id] = NULL;		// Inicializa puntero
	SPR256PAL[id].size = 0;			// Tama�o (en bytes) de la paleta (PAL)
	SPR256PAL[id].available = true;	// Disponibilidat del Slot

}



// Funcion VramSpriteGfx();
void VramSpriteGfx(u8 screen, u16 ram, u16 vram, bool keepframes) {

	// Verifica el rango de Id's de RAM
	if ((ram < 0) || (ram >= SLOTS_SPR256GFX)) {
		Error(106, "Sprite GFX", (SLOTS_SPR256GFX - 1));
	}

	// Verifica si slot de RAM esta vacio
	if (SPR256GFX[ram].available) {
		Error(110, "Sprite GFX", ram);
	}

	// Verifica el rango de Id's de VRAM
	if ((vram < 0) || (vram > 127)) {
		Error(106, "VRAM GFX", 127);
	}

	// Verifica si el slot de VRAM esta libre
	if (SPR256VRAM[screen][vram].inuse) {
		Error(109, "VRAM", vram);
	}

	// Variables de uso general
	s16 n = 0;				// General
	s16 id = 255;			// Id del posible bloque libre
	s16 last_reuse = 0;		// N� del ultimo bloque reusable
	u32 gfxsize = 0;		// Tama�o de los datos que se copiaran
	u32 size = 0;			// Diferencia de tama�os entre bloque libre y datos
	u8 width = 0;			// Calculo de las medidas
	u8 height = 0;
	bool organize = true;	// Se debe de reorganizar el array de bloques libres ?

	// Auto calcula el tama�o de 1 frame
	width = (SPR256GFX[ram].width >> 3);		// (width / 8)
	height = (SPR256GFX[ram].height >> 3);	// (height / 8)
	SPR256VRAM[screen][vram].framesize = ((width * height) << 6);	// ((width * height) * 64)
	// Auto calcula el ultimo frame de la animacion
	SPR256VRAM[screen][vram].lastframe = ((int)(SPR256GFX[ram].size / SPR256VRAM[screen][vram].framesize)) - 1;
	SPR256VRAM[screen][vram].inuse = true;						// Slot ocupado

	// Calcula el tama�o del grafico a copiar segun si debes o no copiar todos los frames
	if (keepframes) {	// Si debes de mantener los frames en RAM, solo copia el primero
		gfxsize = SPR256VRAM[screen][vram].framesize;
	} else {			// Si no, copialos todos
		gfxsize = SPR256GFX[ram].size;
	}

	// Actualiza la VRAM disponible
	SPRVRAM[screen].free -= gfxsize;

	// Si no hay suficiente VRAM, error
	if (SPRVRAM[screen].free < 0) {
		Error(113, "Sprites", gfxsize);
	}

	// Si hay que aprovechar algun bloque borrado... (tama�o identico, preferente)
	if (SPRVRAM[screen].deleted > 0) {
		// Busca un bloque vacio del tama�o identico
		for (n = 0; n < SPRVRAM[screen].deleted; n ++) {
			if (SPRVRAM[screen].size[n] == gfxsize) {		// Si el bloque tiene el tama�o suficiente
				id = n;		// Guarda la Id
				n = SPRVRAM[screen].deleted;	// y sal
			}
		}
		// Si no habia ningun bloque de tama�o identico, busca el mas parecido (produce fragmentacion)
		if (id == 255) {
			// Busca un bloque vacio del tama�o suficiente
			for (n = 0; n < SPRVRAM[screen].deleted; n ++) {
				if (SPRVRAM[screen].size[n] > gfxsize) {		// Si el bloque tiene el tama�o suficiente
					id = n;		// Guarda la Id
					n = SPRVRAM[screen].deleted;	// y sal
				}
			}
		}
	}
	
	// Si hay algun bloque borrado libre del tama�o suficiente...
	if (id != 255) {

		// Transfiere el grafico a la VRAM
		DmaMemCopy((void*)SPRVRAM[screen].pos[id], BUFFER_SPR256GFX[ram], gfxsize);
		// Guarda el puntero donde lo has almacenado
		SPR256VRAM[screen][vram].address = SPRVRAM[screen].pos[id];

		// Si no has usado todo el tama�o, deja constancia
		if (gfxsize < SPRVRAM[screen].size[id]) {

			// Calcula el tama�o del nuevo bloque libre
			size = (SPRVRAM[screen].size[id] - gfxsize);
			// Actualiza los datos
			SPRVRAM[screen].pos[id] += gfxsize;			// Nueva direccion
			SPRVRAM[screen].size[id] = size;				// Nuevo tama�o
			SPRVRAM[screen].fragmented -= gfxsize;		// Actualiza el contador de VRAM fragmentada
			organize = false;								// No se debe de reorganizar el array de bloques

		} else {	// Si has usado todo el tama�o, deja constancia

			SPRVRAM[screen].fragmented -= SPRVRAM[screen].size[id];	// Actualiza el contador de VRAM fragmentada

		}

		// Se tiene que reorganizar el array de bloques libres ?
		if (organize) {
			last_reuse = (SPRVRAM[screen].deleted - 1);
			if (
			(last_reuse > 0)	// Si hay mas de un bloque borrado
			&&
			(id != last_reuse)	// Y no es la ultima posicion
			) {
				// Coloca los valores de la ultima posicion en esta
				SPRVRAM[screen].pos[id] = SPRVRAM[screen].pos[last_reuse];		// Nueva direccion
				SPRVRAM[screen].size[id] = SPRVRAM[screen].size[last_reuse];		// Nuevo tama�o
			}
			SPRVRAM[screen].deleted --;		// Actualiza el contador de bloques libres, borrando el ultimo registro
		}

	} else {	// Si no habia ningun bloque borrado o con el tama�o suficiente, colacalo al final de la VRAM ocupada

		// Actualiza la VRAM contigua disponible (mayor bloque libre al final)
		SPRVRAM[screen].inarow -= gfxsize;

		// Si no hay suficiente VRAM (contigua), error
		if (SPRVRAM[screen].inarow < 0) {
			Error(113, "Sprites", gfxsize);
		}

		// Transfiere el grafico a la VRAM
		DmaMemCopy((void*)SPRVRAM[screen].next, BUFFER_SPR256GFX[ram], gfxsize);
		// Guarda el puntero donde lo has almacenado
		SPR256VRAM[screen][vram].address = SPRVRAM[screen].next;
		// Guarda la direccion actual como la ultima usada
		SPRVRAM[screen].last = SPRVRAM[screen].next;
		// Calcula la siguiente posicion libre
		SPRVRAM[screen].next += gfxsize;

	}

	// Guarda los datos del Gfx que se copiara a la VRAM.
	SPR256VRAM[screen][vram].size = gfxsize;						// Tama�o en bytes de los datos copiados
	SPR256VRAM[screen][vram].width = SPR256GFX[ram].width;	// Alto (px)
	SPR256VRAM[screen][vram].height = SPR256GFX[ram].height;	// Ancho (px)
	SPR256VRAM[screen][vram].ramid = ram;						// Slot RAM de origen
	SPR256VRAM[screen][vram].keepframes = keepframes;			// Debes guardar los frames en RAM o copiarlos a la VRAM?

}



// Funcion FreeSpriteGfx();
void FreeSpriteGfx(u8 screen, u16 id) {

	// Verifica si hay un grafico cargado en esa Id.
	if (!SPR256VRAM[screen][id].inuse) {
		Error(110, "Sprite Gfx", id);
	}

	// Borra el Gfx de la VRAM (pon a 0 todos los Bytes)
	memset((void*)SPR256VRAM[screen][id].address, 0, SPR256VRAM[screen][id].size);

	// Actualiza la cantidad de VRAM disponible
	SPRVRAM[screen].free += SPR256VRAM[screen][id].size;

	// Guarda la posicion y tama�o del bloque borrado para su reutilizacion
	SPRVRAM[screen].pos[SPRVRAM[screen].deleted] = SPR256VRAM[screen][id].address;
	SPRVRAM[screen].size[SPRVRAM[screen].deleted] = SPR256VRAM[screen][id].size;

	// Incrementa en contador de bloques borrados
	SPRVRAM[screen].deleted ++;

	// Incrementa el contador de memoria fragmentada
	SPRVRAM[screen].fragmented += SPR256VRAM[screen][id].size;

	// Reinicia los datos de esta Id. de gfx
	SPR256VRAM[screen][id].size = 0;			// Tama�o en bytes
	SPR256VRAM[screen][id].width = 0;		// Alto (px)
	SPR256VRAM[screen][id].height = 0;		// Ancho (px)
	SPR256VRAM[screen][id].address = 0;		// Puntero en VRAM
	SPR256VRAM[screen][id].framesize = 0;	// Tama�o del frame (en bytes)
	SPR256VRAM[screen][id].lastframe = 0;	// Ultimo frame
	SPR256VRAM[screen][id].inuse = false;

	// Debes desfragmentar la VRAM
	if (SPRVRAM[screen].fragmented >= (SPRVRAM[screen].inarow >> 1)) {
		VramSpriteGfxDefrag(screen);
	}

}



// Funcion VramSpriteGfxDefrag();
void VramSpriteGfxDefrag(u8 screen) {

	// Calcula la VRAM en uso y crea un buffer para guardarla
	u32 used_vram = ((SPRVRAM[screen].max - SPRVRAM[screen].free) + 1);
	char* buffer;
	buffer = (char*) calloc (used_vram, sizeof(char));
	if (buffer == NULL) {		// Si no hay suficiente RAM libre
		Error(102, NULL, used_vram);
	}

	char* address[128];		// Guarda la direccion en RAM
	u32 size[128];			// Guarda el tama�o
	u32 ram = 0;			// Puntero inicial de RAM
	u8 n = 0;				// Variable General
	u32 frame_address = 0;	// Guarda la direccion de VRAM del frame


	// Copia los datos de la VRAM a la RAM
	for (n = 0; n < 128; n ++) {
		// Si esta en uso
		if (SPR256VRAM[screen][n].inuse) {
			// Copia el Gfx a la RAM
			address[n] = (buffer + ram);		// Calcula el puntero
			size[n] = SPR256VRAM[screen][n].size;		// Almacena el tama�o
			DmaMemCopy(address[n], (void*)SPR256VRAM[screen][n].address, size[n]);	// Copialo a la VRAM
			ram += size[n];		// Siguiente posicion en RAM (relativa)
		}
	}

	// Inicializa la estructura de datos de la VRAM de Sprites
	SPRVRAM[screen].free = SPRVRAM[screen].max;		// Memoria VRAM libre (128kb)
	SPRVRAM[screen].last = 0;							// Ultima posicion usada
	SPRVRAM[screen].deleted = 0;							// Ningun Gfx borrado
	SPRVRAM[screen].fragmented = 0;						// Memoria VRAM fragmentada
	SPRVRAM[screen].inarow = SPRVRAM[screen].max;		// Memoria VRAM contigua
	for (n = 0; n < 128; n ++) {
		SPRVRAM[screen].pos[n] = 0;		// Posicion en VRAM para reusar despues de un borrado
		SPRVRAM[screen].size[n] = 0;		// Tama�o del bloque libre para reusar
	}
	// Aplica la direccion de inicio de la VRAM
	if (screen == 0) {
		SPRVRAM[screen].next = (0x06400000);
	} else {
		SPRVRAM[screen].next = (0x06600000);
	}

	// Ahora, copia de nuevo los datos a la VRAM, pero alineados
	for (n = 0; n < 128; n ++) {
		// Si esta en uso
		if (SPR256VRAM[screen][n].inuse) {
			DmaMemCopy((void*)SPRVRAM[screen].next, address[n], size[n]);		// Vuelve a colocar la el Gfx en VRAM
			SPR256VRAM[screen][n].address = SPRVRAM[screen].next;				// Guarda la nueva posicion en VRAM
			SPRVRAM[screen].free -= size[n];		// Ram libre
			SPRVRAM[screen].inarow -= size[n];	// Ram libre en bloque
			SPRVRAM[screen].last = SPRVRAM[screen].next;	// Guarda la posicion como ultima usada
			SPRVRAM[screen].next += size[n];		// Y calcula la siguiente posicion a escribir
		}
	}

	// Reasigna a los sprites las nuevas posiciones de los graficos que usan
	for (n = 0; n < 128; n ++) {
		if (SPRITEOAM[screen][n].created) {
			if (SPR256VRAM[screen][SPRITEOAM[screen][n].gfxid].keepframes) {
				// Si la opcion de animacion KEEP FRAMES esta activada,
				// simplemente asigna la nueva direccion en VRAM del grafico.
				SPRITEOAM[screen][n].gfx = (u32*)SPR256VRAM[screen][SPRITEOAM[screen][n].gfxid].address;
			} else {
				// Si la opcion KEEP FRAMES esta desactivada,
				// calcula el desplazamiento dentro de la nueva direccion asignada.
				frame_address = (SPR256VRAM[screen][SPRITEOAM[screen][n].gfxid].address + (SPRITEOAM[screen][n].framesize * SPRITEOAM[screen][n].frame));
				SPRITEOAM[screen][n].gfx = (u32*)frame_address;
			}
		}
	}

	// Vacia el buffer
	free(buffer);
	buffer = NULL;

}



// Funcion VramSpritePal();
void VramSpritePal(u8 screen, u8 id, u8 slot) {

	// Verifica el rango de Id's
	if ((id < 0) || (id >= SLOTS_SPR256PAL)) {
		Error(106, "Sprite PAL", SLOTS_SPR256PAL);
	}

	// Verifica si la Id esta libre
	if (SPR256PAL[id].available) {
		Error(110, "Sprite PAL", id);
	}

	// Verifica si te has salido de rango (Paleta)
	if ((slot < 0) || (slot > 15)) {
		Error(106, "Sprite Palette Slot", 15);
	}

	// Copia la paleta a la VRAM, segun la pantalla y el Slot
	u32 address = 0;
	if (screen == 0) {
		address = (0x06890000) + (slot << 9);			// Calcula donde guardaras la paleta
		vramSetBankF(VRAM_F_LCD);						// Bloquea el banco F para escribir las paletas
		DmaMemCopy((void*)address, BUFFER_SPR256PAL[id], SPR256PAL[id].size);	// Copia la paleta al banco F
		vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);		// Pon el banco F en modo paleta extendida
	} else {
		address = (0x068A0000) + (slot << 9);			// Calcula donde guardaras la paleta
		vramSetBankI(VRAM_I_LCD);						// Bloquea el banco I para escribir las paletas
		DmaMemCopy((void*)address, BUFFER_SPR256PAL[id], SPR256PAL[id].size);	// Copia la paleta al banco I
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);	// Pon el banco I en modo paleta extendida
	}

	SPRPALSLOT[screen][slot].inuse = true;			// Marca el SLOT de paleta como en uso
	SPRPALSLOT[screen][slot].ramslot = id;			// Guarda el slot de RAM donde esta la paleta original

}



// Funcion CreateSprite();
void CreateSprite(u8 screen, u8 id, u16 gfx, u8 pal, s16 x, s16 y) {

	// Verifica el rango de Id's de Sprites
	if ((id < 0) || (id > 127)) {
		Error(106, "Sprite", 127);
	}

	// Verifica el rango de Id's de Gfx
	if ((gfx < 0) || (gfx > 127)) {
		Error(106, "Sprite GFX", 127);
	}

	// Verifica si esta el Gfx en VRAM
	if (!SPR256VRAM[screen][gfx].inuse) {
		Error(111, "Sprite GFX", gfx);
	}

	// Verifica el rango de slots de paletas
	if ((pal < 0) || (pal > 15)) {
		Error(106, "Sprite Palette Slot", 15);
	}

	// Verifica si esta la paleta en VRAM
	if (!SPRPALSLOT[screen][pal].inuse) {
		Error(111, "Sprite PAL", pal);
	}

	// // Informa al array de OAM del Id
	SPRITEOAM[screen][id].index = id;

	// Informa al array de OAM del Gfx a usar
	SPRITEOAM[screen][id].gfx = (u32*)SPR256VRAM[screen][gfx].address;

	// Informa al array de OAM de la Paleta a usar
	SPRITEOAM[screen][id].pal = pal;

	// Informa al array de OAM de la coordenada X
	SPRITEOAM[screen][id].x = x;

	// Informa al array de OAM de la coordenada X
	SPRITEOAM[screen][id].y = y;

	// Informa al array de OAM del numero de colores
	SPRITEOAM[screen][id].color = SpriteColorFormat_256Color;

	// Informa al array de OAM de que debe mostrar el sprite
	SPRITEOAM[screen][id].hide = false;

	// Informa al array de OAM del Id de Gfx usado
	SPRITEOAM[screen][id].gfxid = gfx;

	// Informa al array de OAM de que el sprite se ha creado
	SPRITEOAM[screen][id].created = true;

	// Informa al array de OAM del tama�o
	if ((SPR256VRAM[screen][gfx].width == 8) && (SPR256VRAM[screen][gfx].height == 8)) {	// 8x8
		if (SPRVRAM[screen].max != 131072) {		// En modo 1D_128, este tama�o es ilegal
			SPRITEOAM[screen][id].size = SpriteSize_8x8;
		} else {
			Error(120, NULL, id);
		}
	}
	if ((SPR256VRAM[screen][gfx].width == 16) && (SPR256VRAM[screen][gfx].height == 16)) {	// 16x16
		SPRITEOAM[screen][id].size = SpriteSize_16x16;
	}
	if ((SPR256VRAM[screen][gfx].width == 32) && (SPR256VRAM[screen][gfx].height == 32)) {	// 32x32
		SPRITEOAM[screen][id].size = SpriteSize_32x32;
	}
	if ((SPR256VRAM[screen][gfx].width == 64) && (SPR256VRAM[screen][gfx].height == 64)) {	// 64x64
		SPRITEOAM[screen][id].size = SpriteSize_64x64;
	}
	if ((SPR256VRAM[screen][gfx].width == 16) && (SPR256VRAM[screen][gfx].height == 8)) {	// 16x8
		SPRITEOAM[screen][id].size = SpriteSize_16x8;
	}
	if ((SPR256VRAM[screen][gfx].width == 32) && (SPR256VRAM[screen][gfx].height == 8)) {	// 32x8
		SPRITEOAM[screen][id].size = SpriteSize_32x8;
	}
	if ((SPR256VRAM[screen][gfx].width == 32) && (SPR256VRAM[screen][gfx].height == 16)) {	// 32x16
		SPRITEOAM[screen][id].size = SpriteSize_32x16;
	}
	if ((SPR256VRAM[screen][gfx].width == 64) && (SPR256VRAM[screen][gfx].height == 32)) {	// 64x32
		SPRITEOAM[screen][id].size = SpriteSize_64x32;
	}
	if ((SPR256VRAM[screen][gfx].width == 8) && (SPR256VRAM[screen][gfx].height == 16)) {	// 8x16
		SPRITEOAM[screen][id].size = SpriteSize_8x16;
	}
	if ((SPR256VRAM[screen][gfx].width == 8) && (SPR256VRAM[screen][gfx].height == 32)) {	// 8x32
		SPRITEOAM[screen][id].size = SpriteSize_8x32;
	}
	if ((SPR256VRAM[screen][gfx].width == 16) && (SPR256VRAM[screen][gfx].height == 32)) {	// 16x32
		SPRITEOAM[screen][id].size = SpriteSize_16x32;
	}
	if ((SPR256VRAM[screen][gfx].width == 32) && (SPR256VRAM[screen][gfx].height == 64)) {	// 32x64
		SPRITEOAM[screen][id].size = SpriteSize_32x64;
	}

	// Informa al array de OAM del ultimo frame del Sprite
	SPRITEOAM[screen][id].lastframe = SPR256VRAM[screen][gfx].lastframe;

	// Informa al array de OAM del tama�o del frame del Sprite (en bytes)
	SPRITEOAM[screen][id].framesize = SPR256VRAM[screen][gfx].framesize;

	// Por defecto, el primer frame (0)
	SPRITEOAM[screen][id].frame = 0;

}



// Funcion DeleteSprite();
void DeleteSprite(u8 screen, u8 id) {

	// Verifica el rango de Id's de Sprites
	if ((id < 0) || (id > 127)) {
		Error(106, "Sprite", 127);
	}

	// Verifica si el Sprite esta creado
	if (!SPRITEOAM[screen][id].created) {
		char text[3];
		sprintf(text, "%d", screen);
		Error(112, text, id);
	}

	// Reinicia todas las variables de ese Sprite
	SPRITEOAM[screen][id].index = id;	// Numero de Sprite
	SPRITEOAM[screen][id].x = 0;			// Coordenada X del Sprite (0 por defecto)
	SPRITEOAM[screen][id].y = 0;			// Coordenada Y del Sprite (0 por defecto)
	SPRITEOAM[screen][id].layer = 0;		// Prioridad en las capas (0 por defecto)
	SPRITEOAM[screen][id].pal = 0;		// Paleta que usaras (0 por defecto)
	SPRITEOAM[screen][id].size = SpriteSize_8x8;					// Tama�o del Sprite (macro) (8x8 por defecto)
	SPRITEOAM[screen][id].color = SpriteColorFormat_256Color;	// Modo de color (macro) (256 colores)
	SPRITEOAM[screen][id].gfx = NULL;			// Puntero al grafico usado
	SPRITEOAM[screen][id].rot = -1;				// Id de rotacion (-1 ninguno) (0 - 31 Id de rotacion)
	SPRITEOAM[screen][id].doublesize = false;	// Usar el "double size" al rotar ? ("NO" por defecto)
	SPRITEOAM[screen][id].hide = true;			// Ocultar el Sprite ("SI" por defecto)
	SPRITEOAM[screen][id].hflip = false;			// Volteado Horizontal ("NO" por defecto)
	SPRITEOAM[screen][id].vflip = false;			// Volteado Vertical ("NO" por defecto)
	SPRITEOAM[screen][id].mosaic = false;		// Mosaico ("NO" por defecto)
	SPRITEOAM[screen][id].gfxid = 0;				// Numero de Gfx usado
	SPRITEOAM[screen][id].frame = 0;				// Frame actual
	SPRITEOAM[screen][id].framesize = 0;			// Tama�o del frame (en bytes)
	SPRITEOAM[screen][id].lastframe = 0;			// Ultimo frame
	SPRITEOAM[screen][id].created = false;		// Esta creado este sprite ?

}



// Funcion SpriteOamSet();
void SpriteOamSet(u8 screen) {

	u8 n  = 0;	// Variable de uso general

	if (screen == 0) {

		for (n = 0; n < 128; n ++) {
			oamSet(&oamMain,							// OAM pantalla superior (Main, 0)
				SPRITEOAM[screen][n].index,			// Numero de Sprite
				SPRITEOAM[screen][n].x,				// Coordenada X del Sprite
				SPRITEOAM[screen][n].y,				// Coordenada Y del Sprite
				SPRITEOAM[screen][n].layer,			// Prioridad en las capas
				SPRITEOAM[screen][n].pal,			// Paleta que usaras
				SPRITEOAM[screen][n].size,			// Tama�o del Sprite (macro)
				SPRITEOAM[screen][n].color,			// Modo de color (macro)
				SPRITEOAM[screen][n].gfx,			// Puntero al grafico usado
				SPRITEOAM[screen][n].rot,			// Valor de la rotacion
				SPRITEOAM[screen][n].doublesize,		// Usar el "double size" al rotar ?
				SPRITEOAM[screen][n].hide,			// Ocultar el Sprite
				SPRITEOAM[screen][n].hflip,			// Volteado Horizontal
				SPRITEOAM[screen][n].vflip,			// Volteado Vertical
				SPRITEOAM[screen][n].mosaic);		// Mosaico
		}

	} else {

		for (n = 0; n < 128; n ++) {
			oamSet(&oamSub,								// OAM pantalla superior (Main, 0)
				SPRITEOAM[screen][n].index,			// Numero de Sprite
				SPRITEOAM[screen][n].x,				// Coordenada X del Sprite
				SPRITEOAM[screen][n].y,				// Coordenada Y del Sprite
				SPRITEOAM[screen][n].layer,			// Prioridad en las capas
				SPRITEOAM[screen][n].pal,			// Paleta que usaras
				SPRITEOAM[screen][n].size,			// Tama�o del Sprite (macro)
				SPRITEOAM[screen][n].color,			// Modo de color (macro)
				SPRITEOAM[screen][n].gfx,			// Puntero al grafico usado
				SPRITEOAM[screen][n].rot,			// Valor de la rotacion
				SPRITEOAM[screen][n].doublesize,		// Usar el "double size" al rotar ?
				SPRITEOAM[screen][n].hide,			// Ocultar el Sprite
				SPRITEOAM[screen][n].hflip,			// Volteado Horizontal
				SPRITEOAM[screen][n].vflip,			// Volteado Vertical
				SPRITEOAM[screen][n].mosaic);		// Mosaico
		}

	}

}



// Funcion SpriteSetPalColor();
void SpriteSetPalColor(u8 screen, u8 pal, u8 number, u8 r, u8 g, u8 b) {

	// Verifica si esta la paleta en VRAM
	if (!SPRPALSLOT[screen][pal].inuse) {
		Error(111, "Sprite PAL", pal);
	}

	// Calcula el valor RGB
	u16 rgb = ((r)|((g) << 5)|((b) << 10));
	// Direccion en VRAM
	u32 address = 0;

	// Modifica la paleta
	if (screen == 0) {
		address = (0x06890000) + (pal << 9) + (number << 1);	// Calcula donde guardaras el color de la paleta
		vramSetBankF(VRAM_F_LCD);								// Bloquea el banco F para escribir las paletas
		*((u16*)address) = rgb;									// Cambia el color
		vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);				// Pon el banco F en modo paleta extendida
	} else {
		address = (0x068A0000) + (pal << 9) + (number << 1);	// Calcula donde guardaras el color de la paleta
		vramSetBankI(VRAM_I_LCD);								// Bloquea el banco I para escribir las paletas
		*((u16*)address) = rgb;									// Cambia el color
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);			// Pon el banco I en modo paleta extendida
	}

}



// Funcion SpriteEditPalColor();
void SpriteEditPalColor(u8 screen, u8 pal, u8 number, u8 r, u8 g, u8 b) {

	// Verifica si esta la paleta en VRAM
	if (!SPRPALSLOT[screen][pal].inuse) {
		Error(111, "Sprite PAL", pal);
	}

	// Calcula el valor RGB
	u16 rgb = ((r)|((g) << 5)|((b) << 10));

	// Calcula los valores para el HI-Byte y el LO-Byte
	u8 hibyte = ((rgb >> 8) & 0xff);
	u8 lobyte = (rgb & 0xff);

	// Graba los bytes
	*(BUFFER_SPR256PAL[SPRPALSLOT[screen][pal].ramslot] + (number << 1)) = lobyte;
	*(BUFFER_SPR256PAL[SPRPALSLOT[screen][pal].ramslot] + ((number << 1) + 1)) = hibyte;

}



// Funcion 	SpriteUpdatePalette();
void SpriteUpdatePalette(u8 screen, u8 pal) {

	// Verifica si esta la paleta en VRAM
	if (!SPRPALSLOT[screen][pal].inuse) {
		Error(111, "Sprite PAL", pal);
	}

	// Direccion en VRAM
	u32 address = 0;

	// Obten el slot donde esta la paleta en RAM
	u8 slot = SPRPALSLOT[screen][pal].ramslot;

	// Actualiza la paleta en VRAM
	if (screen == 0) {
		address = (0x06890000) + (pal << 9);			// Calcula donde guardaras la paleta
		vramSetBankF(VRAM_F_LCD);					// Bloquea el banco F para escribir las paletas
		DmaMemCopy((void*)address, BUFFER_SPR256PAL[slot], SPR256PAL[slot].size);	// Copia la paleta al banco F
		vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);	// Pon el banco F en modo paleta extendida
	} else {
		address = (0x068A0000) + (pal << 9);			// Calcula donde guardaras la paleta
		vramSetBankI(VRAM_I_LCD);					// Bloquea el banco I para escribir las paletas
		DmaMemCopy((void*)address, BUFFER_SPR256PAL[slot], SPR256PAL[slot].size);	// Copia la paleta al banco I
		vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);	// Pon el banco I en modo paleta extendida
	}

}



// Funcion SpriteGetPalColor();
void SpriteGetPalColor(u8 screen, u8 pal, u8 number, u8* r, u8* g, u8* b) {

	// Verifica si esta la paleta en VRAM
	if (!SPRPALSLOT[screen][pal].inuse) {
		Error(111, "Sprite PAL", pal);
	}

	// Obten los bytes
	u8 lobyte = *(BUFFER_SPR256PAL[SPRPALSLOT[screen][pal].ramslot] + (number << 1));
	u8 hibyte = *(BUFFER_SPR256PAL[SPRPALSLOT[screen][pal].ramslot] + ((number << 1) + 1));

	// Calcula el RGB (compuesto)
	u16 rgb = ((hibyte << 8) | lobyte);

	// Calcula los RGB
	*r = (rgb & 0x1F);
	*g = ((rgb >> 5) & 0x1F);
	*b = ((rgb >> 10) & 0x1F);

}
