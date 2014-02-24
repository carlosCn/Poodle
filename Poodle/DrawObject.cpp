#include "DrawObject.h"

const DWORD Vertex::FVF = D3DFVF_XYZ /*| D3DFVF_DIFFUSE*/| D3DFVF_NORMAL | D3DFVF_TEX1  ;

CDrawObject::CDrawObject(void):
m_device(NULL),
m_vb(NULL),
m_ib(NULL),
m_pos(NULL)
{
}


CDrawObject::~CDrawObject(void)
{
	if(m_vb){m_vb->Release(); m_vb = 0;}
	if(m_ib){m_ib->Release(); m_ib = 0;}
}
