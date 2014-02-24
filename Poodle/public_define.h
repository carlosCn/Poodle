
#pragma  once

#include<stdio.h>
#include<iostream>
#include <vector>
#include <Windows.h>
#include "d3d9.h"
#include "d3dx9.h"

using namespace std ;

const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );


D3DMATERIAL9 initMat(D3DCOLORVALUE a , D3DCOLORVALUE d , D3DCOLORVALUE s ,D3DCOLORVALUE e , float p);

const D3DMATERIAL9 WHITE_MTRL  = initMat(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL    = initMat(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL  = initMat(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL   = initMat(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = initMat(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

D3DLIGHT9 initPointLight(D3DXVECTOR3 position , D3DXCOLOR color) ;
D3DLIGHT9 initDirectLight(D3DXVECTOR3 direction , D3DXCOLOR color) ;
D3DLIGHT9 initSpotLight(D3DXVECTOR3 position, D3DXVECTOR3 direction , D3DXCOLOR color) ;


