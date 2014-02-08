#pragma once

#include "DrawObject.h"

struct BlendTestVertex
{
	BlendTestVertex(){}
	BlendTestVertex(float x, float y, float z , float nx, float ny, float nz ,float u , float v)
	{
		_x = x;  _y = y;  _z = z;
		_nx = nx;  _ny = ny;  _nz = nz;	
		_u = u ;
		_v = v  ;
		//_color = color ;
	}
	float _x, _y, _z;
	float _nx,_ny , _nz ;
	float _u, _v ;
	//D3DCOLOR _color ;
	static const DWORD FVF;
};
const DWORD BlendTestVertex::FVF = D3DFVF_XYZ /*| D3DFVF_DIFFUSE*/| D3DFVF_NORMAL | D3DFVF_TEX1  ;

class CBlendTest:
	public CDrawObject 
{
public:
	CBlendTest(void);
	~CBlendTest(void);

	void preRender()   ;
    void Render()      ;
    void afterRender() ;

private:

	ID3DXMesh* m_pMesh;
	D3DMATERIAL9 pMeterial[4] ;
	IDirect3DTexture9 *CrateTex ;
	IDirect3DTexture9 *pBKTeture  ;

};

