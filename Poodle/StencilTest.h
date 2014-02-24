#pragma once
#include "public_define.h"
#include "DrawObject.h"
//模板测试 阴影实现 
struct StencilVetex 
{
	StencilVetex(){}
	StencilVetex(float x, float y, float z , float nx, float ny, float nz ,float u , float v)
	{
		_x = x;  _y = y;  _z = z;
		_nx = nx;  _ny = ny;  _nz = nz;	
		_u = u ;
		_v = v  ;
	}
	float _x, _y, _z;
	float _nx,_ny , _nz ;
	float _u, _v ;
	static const DWORD FVF;
};


class CStencilTest
	:public CDrawObject
{
public:
	CStencilTest(void);
	~CStencilTest(void);

	 void preRender()   ;
	 void Render(float timeDelta)      ;
	 void afterRender() ;

	 void changedir(float timeDelta)   ;

private:

	ID3DXMesh *m_pTeapotMesh ; 
	IDirect3DTexture9 *m_pFloor ;
	IDirect3DTexture9 *m_pWall;
	IDirect3DTexture9 *m_pMirror;

	D3DMATERIAL9 m_mteapot ;
	D3DMATERIAL9 m_mmirror ;
	D3DMATERIAL9 m_mwall ;
	D3DMATERIAL9 m_mfloor ;
	D3DXVECTOR3 TeapotPosition;
};

