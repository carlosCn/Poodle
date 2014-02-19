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


class CBlendTest:
	public CDrawObject 
{
public:
	CBlendTest(void);
	~CBlendTest(void);

	void preRender()   ;
    void Render(float timeDelta)      ;
    void afterRender() ;
    void changedir(float timeDelta)
	{

	}
private:

	ID3DXMesh* m_pMesh;
	D3DMATERIAL9 pMeterial[4] ;
	IDirect3DTexture9 *CrateTex ;
	IDirect3DTexture9 *pBKTeture  ;

};

