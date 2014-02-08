
//窗口基类

#pragma once
#include<stdio.h>
#include<iostream>
#include <Windows.h>
#include "d3d9.h"
#include "d3dx9.h"
#include "Device.h"

#define  COLOR_RED    D3DCOLOR_XRGB(255,0,0)
#define  COLOR_BLUE   D3DCOLOR_XRGB(0,0,255)
#define  COLOR_GREEN  D3DCOLOR_XRGB(0,255,0)
#define  COLOR_WHITE  D3DCOLOR_XRGB(255,255,255)
//初始化材质
D3DMATERIAL9 initMat(D3DCOLORVALUE a , D3DCOLORVALUE d , D3DCOLORVALUE s ,
	D3DCOLORVALUE e , float p);

D3DLIGHT9 initPointLight(D3DXVECTOR3 position , D3DXCOLOR color) ;
D3DLIGHT9 initDirectLight(D3DXVECTOR3 direction , D3DXCOLOR color) ;
D3DLIGHT9 initSpotLight(D3DXVECTOR3 position, D3DXVECTOR3 direction , D3DXCOLOR color) ;


