#pragma once

#include "d3d9.h"
#include "d3dx9.h"

class CDrawObject
{
public:
    CDrawObject(void);
	virtual ~CDrawObject(void);
    void SetDevice(IDirect3DDevice9* _device)
	{
		m_device = _device ;
	}
    void SetPos(D3DXMATRIX* _mat) 
	{
		m_pos = _mat ;
	}

	void SetLight(D3DLIGHT9 _light)
	{
		m_light = _light ;
	}
	virtual void preRender()   = 0;
	virtual void Render()      = 0 ;
	virtual void afterRender() = 0  ;

protected:
	IDirect3DDevice9*       m_device;
	IDirect3DVertexBuffer9* m_vb;
	IDirect3DIndexBuffer9*  m_ib;
	D3DXMATRIX * m_pos ;
	D3DLIGHT9   m_light ;
};

