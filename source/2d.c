
// NightFox LIB - Funciones 2D comunes
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413



// Includes devKitPro
#include <nds.h>

// Includes C
#include <stdio.h>
#include <string.h>

// Includes propios
#include "basic.h"
#include "2d.h"
#include "tiledbg.h"
#include "sprite256.h"





// Funcion Set2D();
void Set2D(u8 screen, u8 mode) {

	if (screen == 0) {		// Pantalla Superior

		switch (mode) {		// Selecciona modo
			case 0:		
				videoSetMode(MODE_0_2D);
				break;
			case 2:		
				videoSetMode(MODE_2_2D);
				break;
			case 5:
				videoSetMode(MODE_5_2D);
				break;
		}

	} else {				// Pantalla inferior

		switch (mode) {		// Seleccion modo
			case 0:
				videoSetModeSub(MODE_0_2D);
				break;
			case 2:
				videoSetModeSub(MODE_2_2D);
				break;
			case 5:
				videoSetModeSub(MODE_5_2D);
				break;
		}

	}

}



// Funcion ShowBg();
void ShowBg(u8 screen, u8 layer) {

	if (screen == 0) {		// Pantalla Superior

		switch (layer) {	// Segun la capa
			case 0:
				REG_DISPCNT |= (DISPLAY_BG0_ACTIVE);
				break;
			case 1:
				REG_DISPCNT |= (DISPLAY_BG1_ACTIVE);
				break;
			case 2:
				REG_DISPCNT |= (DISPLAY_BG2_ACTIVE);
				break;
			case 3:
				REG_DISPCNT |= (DISPLAY_BG3_ACTIVE);
				break;
		}

	} else {				// Pantalla Inferior

		switch (layer) {	// Segun la capa
			case 0:
				REG_DISPCNT_SUB |= (DISPLAY_BG0_ACTIVE);
				break;
			case 1:
				REG_DISPCNT_SUB |= (DISPLAY_BG1_ACTIVE);
				break;
			case 2:
				REG_DISPCNT_SUB |= (DISPLAY_BG2_ACTIVE);
				break;
			case 3:
				REG_DISPCNT_SUB |= (DISPLAY_BG3_ACTIVE);
				break;
		}

	}

}



// Funcion HideBg();
void HideBg(u8 screen, u8 layer) {

	if (screen == 0) {		// Pantalla Superior

		switch (layer) {	// Segun la capa
			case 0:
				REG_DISPCNT &= ~(DISPLAY_BG0_ACTIVE);
				break;
			case 1:
				REG_DISPCNT &= ~(DISPLAY_BG1_ACTIVE);
				break;
			case 2:
				REG_DISPCNT &= ~(DISPLAY_BG2_ACTIVE);
				break;
			case 3:
				REG_DISPCNT &= ~(DISPLAY_BG3_ACTIVE);
				break;
		}

	} else {				// Pantalla Inferior

		switch (layer) {	// Segun la capa
			case 0:
				REG_DISPCNT_SUB &= ~(DISPLAY_BG0_ACTIVE);
				break;
			case 1:
				REG_DISPCNT_SUB &= ~(DISPLAY_BG1_ACTIVE);
				break;
			case 2:
				REG_DISPCNT_SUB &= ~(DISPLAY_BG2_ACTIVE);
				break;
			case 3:
				REG_DISPCNT_SUB &= ~(DISPLAY_BG3_ACTIVE);
				break;
		}

	}

}



// Funcion ScrollBg();
void ScrollBg(u8 screen, u8 layer, s16 x, s16 y) {

	// Variables temporales
	s16 sx = x;
	s16 sy = y;

	// Si el mapa es infinito... > 512
	if (TILEDBG_LAYERS[screen][layer].bgtype > 0) {

		// Variables temporales de Fondos infinitos
		u32 address = 0;		// Puntero a la VRAM
		u16 blockx = 0;		// N� de bloque en pantalla
		u16 blocky = 0;
		u32 mapmovex = 0;	// Desplazamiento de la copia de datos (block x 2048)
		u32 mapmovey = 0;
		u16 rowsize = 0;	// Calcula el espacio ocupado en RAM por cada fila
		
		// Calcula la direccion base del mapa
		if (screen == 0) {	// (VRAM_A)
			address = (0x6000000) + (TILEDBG_LAYERS[screen][layer].mapbase << 11);
		} else {			// (VRAM_C)
			address = (0x6200000) + (TILEDBG_LAYERS[screen][layer].mapbase << 11);
		}

		// Ajusta el valor maximo de las variables a los limites del scroll
		if (sx < 0) {
			sx = 0;
		}
		if (sx > (TILEDBG_LAYERS[screen][layer].bgwidth - 256)) {
			sx = (TILEDBG_LAYERS[screen][layer].bgwidth - 256);
		}
		if (sy < 0) {
			sy = 0;
		}
		if (sy > (TILEDBG_LAYERS[screen][layer].bgheight - 192)) {
			sy = (TILEDBG_LAYERS[screen][layer].bgheight - 192);
		}

		// Segun el tipo de mapa...
		switch (TILEDBG_LAYERS[screen][layer].bgtype) {

			case 1:	// 512x256  -  Bloque A y B (32x32) + (32x32) (2kb x 2 = 4kb)
				// Calcula el bloque
				blockx = (x >> 8);
				// Si has cambiado de bloque...
				if (TILEDBG_LAYERS[screen][layer].blockx != blockx) {
					// Calcula el desplazamiento de datos
					mapmovex = (blockx << 11);
					// Copias los Bloques A y B (32x32) + (32x32) (2kb x 2 = 4kb)
					DmaMemCopy((void*)address, (BUFFER_BGMAP[TILEDBG_LAYERS[screen][layer].bgslot] + mapmovex), 4096);
					// Y actualiza el bloque actual
					TILEDBG_LAYERS[screen][layer].blockx = blockx;
				}
				// Calcula la X del fondo
				sx = x - (blockx << 8);
				break;

			case 2:	// 256x512  -  Bloque A (32x64) (2kb x 2 = 4kb)
				// Calcula el bloque
				blocky = (y >> 8);
				// Si has cambiado de bloque...
				if (TILEDBG_LAYERS[screen][layer].blocky != blocky) {
					// Calcula el desplazamiento de datos
					mapmovey = (blocky << 11);
					// Copias los Bloques A y B (32x32) + (32x32) (2kb x 2 = 4kb)
					DmaMemCopy((void*)address, (BUFFER_BGMAP[TILEDBG_LAYERS[screen][layer].bgslot] + mapmovey), 4096);
					// Y actualiza el bloque actual
					TILEDBG_LAYERS[screen][layer].blocky = blocky;
				}
				// Calcula la X del fondo
				sy = y - (blocky << 8);
				break;

			case 3: // >512x>512
				rowsize = (((((TILEDBG_LAYERS[screen][layer].bgwidth - 1) >> 8)) + 1) << 11);
				// Calcula los bloques
				blockx = (x >> 8);
				blocky = (y >> 8);
				if (	// Si se ha cambiado de bloque en alguna direccion...
					(TILEDBG_LAYERS[screen][layer].blockx != blockx)
					||
					(TILEDBG_LAYERS[screen][layer].blocky != blocky)
					) {
					// Y el desplazamiento de datos
					mapmovex = (blocky * rowsize) + (blockx << 11);
					mapmovey = mapmovex + rowsize;
					// Bloque A y B (32x32) + (32x32) (2kb x 2 = 4kb)
					DmaMemCopy((void*)address, (BUFFER_BGMAP[TILEDBG_LAYERS[screen][layer].bgslot] + mapmovex), 4096);
					// Bloque (+4096) C y D (32x32) + (32x32) (2kb x 2 = 4kb)
					DmaMemCopy((void*)(address + 4096), (BUFFER_BGMAP[TILEDBG_LAYERS[screen][layer].bgslot] + mapmovey), 4096);
					// Y actualiza el bloque actual
					TILEDBG_LAYERS[screen][layer].blockx = blockx;
					TILEDBG_LAYERS[screen][layer].blocky = blocky;
				}
				// Calcula la X e Y del fondo
				sx = x - (blockx << 8);
				sy = y - (blocky << 8);
				break;

		}

	}

	// Mueve el fondo usando los registros
	if (screen == 0) {		// Pantalla Superior

		switch (layer) {	// Segun la capa
			case 0:
				REG_BG0HOFS = sx;
				REG_BG0VOFS = sy;
				break;
			case 1:
				REG_BG1HOFS = sx;
				REG_BG1VOFS = sy;
				break;
			case 2:
				REG_BG2HOFS = sx;
				REG_BG2VOFS = sy;
				break;
			case 3:
				REG_BG3HOFS = sx;
				REG_BG3VOFS = sy;
				break;
		}

	} else {

		switch (layer) {	// Segun la capa
			case 0:
				REG_BG0HOFS_SUB = sx;
				REG_BG0VOFS_SUB = sy;
				break;
			case 1:
				REG_BG1HOFS_SUB = sx;
				REG_BG1VOFS_SUB = sy;
				break;
			case 2:
				REG_BG2HOFS_SUB = sx;
				REG_BG2VOFS_SUB = sy;
				break;
			case 3:
				REG_BG3HOFS_SUB = sx;
				REG_BG3VOFS_SUB = sy;
				break;
		}

	}

}



// Funcion MoveSprite();
void MoveSprite(u8 screen, u8 id, s16 x, s16 y) {

	SPRITEOAM[screen][id].x = x;		// Coordenada X
	SPRITEOAM[screen][id].y = y;		// Coordenada Y

}



// Funcion SpriteLayer();
void SpriteLayer(u8 screen, u8 id, u8 layer) {

	SPRITEOAM[screen][id].layer = layer;		// Capa sobre la que esta el sprite

}



// Funcion ShowSprite();
void ShowSprite(u8 screen, u8 id, bool show) {

	SPRITEOAM[screen][id].hide = !show;		// Muestra o oculta el sprite

}



// Funcion HflipSprite();
void HflipSprite(u8 screen, u8 id, bool hflip) {

	SPRITEOAM[screen][id].hflip = hflip;		// Volteado horizontal;

}



// Funcion GetSpriteHflip();
bool GetSpriteHflip(u8 screen, u8 id) {
	return SPRITEOAM[screen][id].hflip;
}



// Funcion VflipSprite();
void VflipSprite(u8 screen, u8 id, bool vflip) {

	SPRITEOAM[screen][id].vflip = vflip;		// Volteado vertical;

}



// Funcion GetSpriteVflip();
bool GetSpriteVflip(u8 screen, u8 id) {
	return SPRITEOAM[screen][id].vflip;
}



// Funcion SpriteFrame();
void SpriteFrame(u8 screen, u8 id, u16 frame) {

	// Verifica el rango de Id's de Sprites
	if ((id < 0) || (id > 127)) {
		Error(106, "Sprite", 127);
	}

	// Verifica el rango de frames del Sprite
	if (frame > SPRITEOAM[screen][id].lastframe) {
		Error(106, "Sprite frame", SPRITEOAM[screen][id].lastframe);
	}


	// Verifica si el frame necesita ser actualizado
	if (SPRITEOAM[screen][id].frame != frame) {

		// Si debes de copiar el nuevo frame desde la RAM a la VRAM...
		if (SPR256VRAM[screen][SPRITEOAM[screen][id].gfxid].keepframes) {

			// Variables temporales
			char* source;			// Puntero de origen
			u32 destination = 0;	// Puntero de destino
			u16 ramid = 0;			// Slot de RAM donde se encuentra el Gfx

			// Calcula el origen y destino del nuevo frame a copiar
			ramid =  SPR256VRAM[screen][SPRITEOAM[screen][id].gfxid].ramid;
			source = BUFFER_SPR256GFX[ramid] + (SPRITEOAM[screen][id].framesize * frame);
			destination = SPR256VRAM[screen][SPRITEOAM[screen][id].gfxid].address;

			// Copialo
			DmaMemCopy((void*)destination, source, SPRITEOAM[screen][id].framesize);

		} else {	// Si todos los frames ya estan en VRAM...

			// Calcula la direccion del Gfx del frame
			u32 address = 0;
			address = SPR256VRAM[screen][SPRITEOAM[screen][id].gfxid].address + (SPRITEOAM[screen][id].framesize * frame);
			SPRITEOAM[screen][id].gfx = (u32*)address;

		}

		// Almacena el frame actual
		SPRITEOAM[screen][id].frame = frame;

	}

}



// Funcion EnableSpriteRotScale();
void EnableSpriteRotScale(u8 screen, u8 sprite, u8 id, bool doublesize) {

	// Verifica el rango de Id's de Sprites
	if ((sprite < 0) || (sprite > 127)) {
		Error(106, "Sprite", 127);
	}

	// Verifica el rango de Id's de Rotacion
	if ((id < 0) || (id > 31)) {
		Error(106, "RotScale", 127);
	}

	// Verifica si el Sprite esta creado
	if (!SPRITEOAM[screen][sprite].created) {
		char text[3];
		sprintf(text, "%d", screen);
		Error(112, text, sprite);
	}

	SPRITEOAM[screen][sprite].rot = id;					// Id de rotacion (-1 ninguno) (0 - 31 Id de rotacion)
	SPRITEOAM[screen][sprite].doublesize = doublesize;	// Usar el "double size" al rotar ? ("NO" por defecto)

}



// Funcion DisableSpriteRotScale();
void DisableSpriteRotScale(u8 screen, u8 sprite) {

	// Verifica el rango de Id's de Sprites
	if ((sprite < 0) || (sprite > 127)) {
		Error(106, "Sprite", 127);
	}

	// Verifica si el Sprite esta creado
	if (!SPRITEOAM[screen][sprite].created) {
		char text[3];
		sprintf(text, "%d", screen);
		Error(112, text, sprite);
	}

	SPRITEOAM[screen][sprite].rot = -1;					// Id de rotacion (-1 ninguno) (0 - 31 Id de rotacion)
	SPRITEOAM[screen][sprite].doublesize = false;		// Usar el "double size" al rotar ? ("NO" por defecto)

}



// Funcion SpriteRotScale();
void SpriteRotScale(u8 screen, u8 id, s16 angle, u16 sx, u16 sy) {

	// Variables temporales
	s16 in = 0;		// Angulo dado
	s16 out = 0;	// Angulo convertido

	in = angle;

	// Limites del angulo
	if (in < -512) {
		in += 512;
	}
	if (in > 512) {
		in -= 512;
	}
	// Limites del factor X
	if (sx < 0) {
		sx = 0;
	}
	if (sx > 512) {
		sx = 512;
	}
	// Limites del factor Y
	if (sy < 0) {
		sy = 0;
	}
	if (sy > 512) {
		sy = 512;
	}
	
	// Si es un numero negativo...
	if (in < 0) {
		in = -in;			// Pasa a positivo (para poder hacer el bitshift)
		out = (in << 6);	// (in * 64); Pasa de base 512 a base 32768
		// Dejalo en positivo para que <0 gire a la izquierda
	} else {
		out = (in << 6);
		out = -out;			// Pasalo a negativo para que >0 gire a la derecha
	}

	// Actualiza el RotScale del OAM
	if (screen == 0) {
		oamRotateScale(&oamMain, id, out, (512 - sx), (512 - sy)); 
	} else {
		oamRotateScale(&oamSub, id, out, (512 - sx), (512 - sy));
	}

}
