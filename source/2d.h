#ifdef __cplusplus
extern "C" {
#endif

#ifndef __2D_H__
#define __2D_H__





// NightFox LIB - Include de funciones 2D comunes
// Requiere DevkitARM
// Codigo por Cesar Rincon "NightFox"
// http://www.nightfoxandco.com/
// Version 20140413



// Includes devKitPro
#include <nds.h>





// Funcion Set2D();
void Set2D(u8 screen, u8 mode);
// Inicia el modo 2D seleccionado en la pantalla deseada



// Funcion ShowBg();
void ShowBg(u8 screen, u8 layer);
// Haz visible el fondo de la capa y pantalla dados



// Funcion HideBg();
void HideBg(u8 screen, u8 layer);
// Oculta el fondo de la capa y pantalla dados



// Funcion ScrollBg();
void ScrollBg(u8 screen, u8 layer, s16 x, s16 y);
// Mueve el fondo a las coordenadas especificadas.
// Debes de indicar la pantalla, capa, y coordenadas X e Y
// Si el mapa es mayor de 512 en alguna medida, debes de mantener el fondo en RAM



// Funcion MoveSprite();
void MoveSprite(u8 screen, u8 id, s16 x, s16 y);
// Mueve el Sprite a las coordenadas especificadas
// Debes de indicar la pantalla, id de sprite y coordenadas



// Funcion SpriteLayer();
void SpriteLayer(u8 screen, u8 id, u8 layer);
// Define la capa sobre la que el sprite sera dibujado
// Debes de indicar la pantalla, id del sprite y capa



// Funcion ShowSprite();
void ShowSprite(u8 screen, u8 id, bool show);
// Muestra o oculta el sprite
// Debes especificar la pantalla, id del sprite y el estado (true, false)



// Funcion HflipSprite();
void HflipSprite(u8 screen, u8 id, bool hflip);
// Voltea el Sprite horizontalmente
// Debes especificar la pantalla, id del sprite y el estado (true, false)



// Funcion GetSpriteHflip();
extern bool GetSpriteHflip(u8 screen, u8 id);
// Devuelve el estado del volteado horizontal de un sprite
// Debes especificar la pantalla y la id del sprite



// Funcion VflipSprite();
void VflipSprite(u8 screen, u8 id, bool vflip);
// Voltea el Sprite verticalmente
// Debes especificar la pantalla, id del sprite y el estado (true, false)



// Funcion GetSpriteVflip();
extern bool GetSpriteVflip(u8 screen, u8 id);
// Devuelve el estado del volteado vertical de un sprite
// Debes especificar la pantalla y la id del sprite



// Funcion SpriteFrame();
void SpriteFrame(u8 screen, u8 id, u16 frame);
// Cambia el frame de un Sprite
// Debes especificar la pantalla, el Id del sprite y el frame



// Funcion EnableSpriteRotScale();
void EnableSpriteRotScale(u8 screen, u8 sprite, u8 id, bool doublesize);
// Habilita el sprite como rotable y escalable
// Debes especificar la pantalla, n� de sprite, id de rotacion y si
// tiene que activarse el doublesize (tama�o x2)



// Funcion DisableSpriteRotScale();
void DisableSpriteRotScale(u8 screen, u8 sprite);
// Deshabilita un sprite como rotable y escalable
// Debes especificar la pantalla y el n� de sprite




// Funcion SpriteRotScale();
void SpriteRotScale(u8 screen, u8 id, s16 angle, u16 sx, u16 sy);
// Define el angulo de rotacion y escala de los Sprites asociados a la Id.
// El rango del angulo es desde -512 a 512, siendo 0 el valor central
// El rango de escalado va de 0 a 512, siendo 256 el valor normal (100%)





#endif

#ifdef __cplusplus
}
#endif
