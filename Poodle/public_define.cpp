
#include "public_define.h"

D3DMATERIAL9 initMat(D3DCOLORVALUE a , D3DCOLORVALUE d , D3DCOLORVALUE s ,
	D3DCOLORVALUE e , float p)
{
	D3DMATERIAL9 meta ;
	meta.Ambient  = a ;
	meta.Diffuse  = d ;
	meta.Emissive = e ;
	meta.Specular = s ;
	meta.Power    = p ;

	return meta ;
}

//光源初始化
D3DLIGHT9 initPointLight(D3DXVECTOR3 position , D3DXCOLOR color)
{
	D3DLIGHT9 light ;
	memset(&light,0,sizeof(light));
	light.Type     = D3DLIGHT_POINT ;
	light.Position = position;
	light.Ambient  = color * 0.5f ;
	light.Diffuse  = color ;
	light.Specular = color * 0.5f ;

	light.Range    = 1000.0 ;
	light.Falloff  = 1.0 ;
	light.Attenuation0 = 0 ;
	light.Attenuation1 = 0 ;
	light.Attenuation2 = 1  ;

	return light ;
}
D3DLIGHT9 initDirectLight(D3DXVECTOR3 direction , D3DXCOLOR color)
{
	D3DLIGHT9 light ;
	memset(&light,0,sizeof(light));
	light.Type      =  D3DLIGHT_DIRECTIONAL ;
	light.Ambient   = color * 0.5f ;
	light.Diffuse   = color ;
	light.Specular  = color * 0.5f ;
	light.Direction = direction ;

	return light ;
}
D3DLIGHT9 initSpotLight(D3DXVECTOR3 position, D3DXVECTOR3 direction , D3DXCOLOR color) 
{
	D3DLIGHT9 light ;
	memset(&light,0,sizeof(light));
	light.Type     =  D3DLIGHT_SPOT ;
	light.Ambient  = color  * 0.5f ;
	light.Diffuse  = color ;
	light.Specular = color  * 0.5f ;

	light.Range        = 1000.0 ;
	light.Attenuation0 = 0.0 ;
	light.Attenuation1 = 0.0 ;
	light.Attenuation2 = 1.0  ;

	light.Position  = position  ;
	light.Direction = direction ;
	light.Falloff   = 1.0 ;
	light.Theta     = 0.2;
	light.Phi       = 1.4 ;

	return light ;
}