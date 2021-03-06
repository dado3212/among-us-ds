#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TILEDBG_H__
#define __TILEDBG_H__





// NightFox LIB - Include de Fondos con tiles
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413





// Includes devKitPro
#include <nds.h>





// Define los slots maximos para los fondos
#define SLOTS_TBG 64			// Datos de los fondos

// Define el numero maximo de bancos para tiles y mapas
#define MAX_BANKS_TILES 8
#define MAX_BANKS_MAPS 16

// Define el numero de bancos de Mapas y Tiles
extern u8 BANKS_TILES[2];	// (1 banks = 16kb)	Cada banco de tiles puede alvergar 8 bancos de Mapas
extern u8 BANKS_MAPS[2];		// (1 bank = 2kb)	Usar multiplos de 8. Cada set de 8 bancos consume 1 banco de tiles

// Define los Buffers para almacenar los fondos
extern char* BUFFER_BGTILES[SLOTS_TBG];
extern char* BUFFER_BGMAP[SLOTS_TBG];
extern char* BUFFER_BGPAL[SLOTS_TBG];


// Define estructura para almacenar la info de los fondos
typedef struct {
	char name[32];		// Nombre del fondo
	u32 tilesize;		// Tama�o del Tileset
	u32 mapsize;		// Tama�o del Map
	u32 palsize;		// Tama�o de la Paleta
	u16 width;			// Ancho del fondo
	u16 height;			// Altura del fondo
	bool available;		// Disponibilidat del Slot
} TYPE_TBG_INFO;
extern TYPE_TBG_INFO TILEDBG[SLOTS_TBG];	// Datos de los fondos

// Define estructura para almacenar la info de los fondos en pantalla
typedef struct {
	u8 tilebase;		// Bloque de inicio en VRAM del Tileset
	u8 tileblocks;		// Bloques usados por el Tileset
	u8 mapbase;			// Bloque de inicio en VRAM del Map
	u8 mapblocks;		// Bloques usados por el Map
	u16 bgwidth;		// Ancho del fondo
	u16 bgheight;		// Altura del fondo
	u16 mapwidth;		// Ancho del mapa
	u16 mapheight;		// Altura del mapa
	u8 bgtype;			// Tipo de mapa
	u8 bgslot;			// Buffer de graficos usado (BUFFER_BGMAP)
	u8 blockx;			// Bloque de mapa (horizontal)
	u8 blocky;			// bloque de mapa (vertical)
	bool created;		// Flag de si esta creado
} TYPE_TBGLAYERS_INFO;
// El hardware de la DS no permite mapas mayores de 512x512
// Asi que informaremos si nuestor mapa lo gestionara el hardware si es menor o 
// igual a 512x512, o usaremos nuestro motor de Tile Swaping
// bgtype 0: Normal (maximo 512 x 512)
// bgtype 1: >512 x 256
// bgtype 2: 256 x >512
// bgtype 3: >512 x >512
extern TYPE_TBGLAYERS_INFO TILEDBG_LAYERS[2][4];	//[screen][layer]



// Define el array de bloques libres
extern u8 TILEBLOCKS[2][MAX_BANKS_TILES];
extern u8 MAPBLOCKS[2][MAX_BANKS_MAPS];



// Funcion InitTiledBgBuffers();
void InitTiledBgBuffers(void);
// Inicializa los buffers y estructuras de control para usar los fondos "tileados"
// Se debe usar antes de cargar o usar cualquier fondo
// No uses esta funcion mas de una vez en tu codigo



// Funcion ResetTiledBgBuffers();
void ResetTiledBgBuffers(void);
// Borra todos los buffers y reinicia las estructuras de fondos "tileados"
// Usala para los cambios de nivel y similares



// Funcion InitTiledBgSys();
void InitTiledBgSys(u8 screen);
// Inicializa las variables de control de tiles, mapas y paletas
// Asigna 128kb de RAM para fondos tileados
// Se debe especificar la pantalla (0 o 1)



// Funcion LoadTiledBg();
void LoadTiledBg(const char* file, const char* name, u16 width, u16 height);
// Carga un fondo tileado desde FAT
// Debes de especificar el archivo que se cargara (sin extension) y el nombre
// que le quieres dar y las medidas en pixeles



// Funcion LoadTilesForBg();
void LoadTilesForBg(const char* file, const char* name, u16 width, u16 height, u16 tile_start, u16 tile_end);
// Carga desde la FAT los tiles especificados y su paleta
// Ademas, crea un mapa vacio de la medida especificada
// Esta funcion es util para cargar varios tiles y despues generar fondos
// modificando el MAP desde cogido (Generador de escenarios, animaciones, etc)



// Funcion UnloadTiledBg();
void UnloadTiledBg(const char* name);
// Borra de la RAM un fondo cargado con LoadTiledBg();
// Debes especificar el nombre que le diste al fondo



// Funcion CreateTiledBg();
void CreateTiledBg(u8 screen, u8 layer, const char* name);
// Crea un fondo con los parametros dados, indicale la pantalla, capa y nombre



// Funcion DeleteTiledBg();
void DeleteTiledBg(u8 screen, u8 layer);
// Borra un fondo de la memoria VRAM
// Debes especificar la pantalla y numero de capa





// Funcion GetTileMapAddress();
extern u32 GetTileMapAddress(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Funcion de uso interno de la libreria
// Devuelve la direccion en el buffer de un tile concreto





// Funcion GetTileOfMap();
extern u16 GetTileOfMap(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Obten el valor del tile del mapa indicado en las coordenadas (en tiles) indicadas.




// Funcion SetTileOfMap();
void SetTileOfMap(u8 screen, u8 layer, u16 tile_x, u16 tile_y, u16 tile);
// Cambia el valor del tile del mapa indicado en las coordenadas (en tiles) indicadas.




// Funcion UpdateVramMap();
void UpdateVramMap(u8 screen, u8 layer);
// Actualiza en VRAM el contenido del mapa seleccionado.




// Funcion BgSetPalColor();
void BgSetPalColor(u8 screen, u8 layer, u8 number, u8 r, u8 g, u8 b);
// Cambia al momento el valor de un color de la paleta
// Cuidado! Funcion Muy lenta, usar solo para 2 o 3 colores por ciclo
// Cambia el color directamente en la VRAM





// Funcion BgEditPalColor();
void BgEditPalColor(u8 screen, u8 layer, u8 number, u8 r, u8 g, u8 b);
// Edita un color de la paleta seleccionada.
// El color se edita en el buffer de RAM, para que sea efectivo,
// mandala a la VRAM con UpdatePalette();





// Funcion 	BgUpdatePalette();
void BgUpdatePalette(u8 screen, u8 layer);
// Actualiza la paleta en VRAM con la que se encuentra en el buffer de RAM





// Funcion BgGetPalColor();
void BgGetPalColor(u8 screen, u8 layer, u8 number, u8* r, u8* g, u8* b);
// Obtiene el valor de un color de la paleta que se encuentra en la RAM





// Funcion GetTilePal();
extern u8 GetTilePal(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Devuelve que numero de paleta (0 - 15) esta usando el tile del fondo especificado.
// Por defecto, todos los tiles usan la paleta del Slot n�0
// Los datos se obtienen de la compia en RAM del mapa del fondo.





// Funcion SetTilePal();
void SetTilePal(u8 screen, u8 layer, u16 tile_x, u16 tile_y, u8 pal);
// Cambia el numero de paleta (0 - 15) que usara el tile del fondo especificado.
// Por defecto, todos los tiles usan la paleta del Slot n�0
// Los datos se escriben de la compia en RAM del mapa del fondo, por lo que no seran
// visibles hasta que ejecutes la funcion UpdateVramMap();


// Funcion SetTileHflip();
void SetTileHflip(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Invierte horizontalmente el estado actual del tile seleccionado
// Los cambios no seran visibles hasta que actualices el mapa
// con la funcion UpdateVramMap();





// Funcion SetTileVflip();
void SetTileVflip(u8 screen, u8 layer, u16 tile_x, u16 tile_y);
// Invierte verticalmente el estado actual del tile seleccionado
// Los cambios no seran visibles hasta que actualices el mapa
// con la funcion UpdateVramMap();




// Funcion RotateTileGfx();
void RotateTileGfx(u8 slot, u16 tile, u8 rotation);
// Rota el grafico de un tile especificado. Rota el tile almacenado en un buffer de fondos
// que se encuentra en RAM. Debes especificar el SLOT del buffer, numero de tile en el buffer
// y el angulo de la rotacion.
// 1 - 90� a la derecha
// 2 - 90� a la izquierda
// 3 - 180�





#endif

#ifdef __cplusplus
}
#endif
