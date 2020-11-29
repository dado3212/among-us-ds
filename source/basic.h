#ifdef __cplusplus
extern "C" {
#endif

#ifndef __BASIC_H__
#define __BASIC_H__





// NightFox LIB - Include de funciones basicas
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413



// Includes devKitPro
#include <nds.h>

// Funcion Error();
void Error(u16 code, const char* text, u32 value);
// Errores para debug. Detiene el sistema e informa del error
// 101: Fichero no encontrado
// 102: Memoria insuficiente
// 103: No quedan Slots libres
// 104: Fondo no encontrado
// 105: Fondo no creado
// 106: Fuera de rango
// 107: Insuficientes bloques contiguos en VRAM (Tiles)
// 108: Insuficientes bloques contiguos en VRAM (Maps)
// 109: Id ocupada (ya esta en uso)
// 110: Id no cargada (en RAM)
// 111: Id no en VRAM
// 112: Sprite no creado
// 113:	Memoria VRAM insuficiente
// 114: La capa de Texto no existe
// 115:	Medidas del fondo no compatibles (no son multiplos de 256)
// 116:	Archivo demasiado grande
// 117: Medidas del fondo affine incorrectas
// 118: Capa de creacion del fondo affine incorrecta

// Funcion DmaMemCopy();
void DmaMemCopy(void* destination, const void* source, u32 size);
// Copia un bloque de memoria usando DMA (canal 3, halfwords) y vaciando previamente
// el cache. Con pruebas de bloques grandes (64kb o 128kb) he observado que memcpy(); 
// sigue siendo mas rapida.


#endif

#ifdef __cplusplus
}
#endif
