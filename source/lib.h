#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NF_LIB_H__
#define __NF_LIB_H__





// NightFox LIB - Include General
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413



/*

Notas sobre BITSHIFT

(n >> x)	Divide			n / x
(n << x)	Multiplica		n * x

Valores de X
2	  =	1
4	  =	2
8	  =	3
16	  =	4
32	  =	5
64	  =	6
128	  =	7
256	  =	8
512	  =	9
1024  =	10
2048  =	11
4096  =	12
8192  =	13
16384 =	14
32768 =	15
65536 =	16

Dado que la DS no tiene unidad de coma flotante, siempre que dividas o
multipliques por numeros de base 2, usa el bitshift
Por ejemplo:
a = (512 / 8);
seria equivalente a
a = (512 >> 3);
Multiplicando
b = (3 * 2048);
seria con bitshift
b = (3 << 11);

*/




// Definiciones comunes
#include "defines.h"

// Libreria de funciones basicas y comunes
#include "basic.h"

// Libreria de funciones 2D comunes
#include "2d.h"

// Libreria de fondos con Tiles
#include "tiledbg.h"

// Libreria de fondos Affine
#include "affinebg.h"

// Libreria de fondos en modo Bitmap
#include "bitmapbg.h"

// Libreria de fondos en modo mixto (Tiled / Bitmap 8 bits)
#include "mixedbg.h"

// Libreria de sprites de 256 colores
#include "sprite256.h"

// Libreria de textos
#include "text.h"

// Libreria de textos de 16 pixeles
#include "text16.h"

// Libreria de colisiones
#include "colision.h"

// Libreria de archivos multimedia
#include "media.h"

// Libreria 3D, funciones comunes
#include "3d.h"

// Libreria 3D, Sprites
#include "sprite3d.h"





#endif

#ifdef __cplusplus
}
#endif
