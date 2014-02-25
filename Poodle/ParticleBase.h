
#pragma once
#include "public_define.h"

struct Particle
{
	D3DXVECTOR3 _pos ;
	D3DCOLOR _color;
	float _size ;
	static const DWORD FVF ;

	Particle(D3DXVECTOR3 pos, D3DCOLOR color , float size)
	{
		_pos   = pos ;
		_color = color ;
		_size  = size ;
	}
};

//粒子属性信息
struct ParticleAttr
{
	D3DXVECTOR3 _position ;
	D3DXVECTOR3 _velocity ;
	D3DXVECTOR3 _acceleration ;

	D3DCOLOR  _color ;
	D3DCOLOR _colorFade ;

	float _lifttime ;
	float _age ;
	float _isAlive ;
};

class CParticleBase
{
public:
	CParticleBase(void);
	~CParticleBase(void);

	//初始化设备
	virtual void  init(IDirect3DDevice9 *mdevice,string strPath);

	virtual void addParticle();	
	virtual void reset();
	virtual void ResetParticle(ParticleAttr *_attr) = 0;

	virtual void Update(float timeDelta ) = 0 ;

	virtual void preRender();   
	virtual void Render(float timeDelta );
	virtual void postRender();

protected:
	virtual void removeDeadParticles();
	
protected:

	IDirect3DDevice9* m_device ;
	IDirect3DTexture9* m_tex ;
	IDirect3DVertexBuffer9* m_vb ;

	list<ParticleAttr> m_particlelist ;
	int m_maxParticleNum ;
	DWORD m_vbsize ;
	DWORD m_vboffset ;
	DWORD m_vbBatchSize;

	//粒子状态更新相关
	D3DXVECTOR3 _orign ;
	float m_emiRate ;
	float m_size ;
};

