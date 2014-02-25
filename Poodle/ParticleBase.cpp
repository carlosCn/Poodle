#include "ParticleBase.h"

const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE ;
CParticleBase::CParticleBase(void):
m_device(NULL),
m_tex(NULL),
m_vb(NULL),
m_maxParticleNum(2048),
m_size(1),
m_vbBatchSize(500),
m_vboffset(0),
m_vbsize(2048)
{

}

CParticleBase::~CParticleBase(void)
{

}

void  CParticleBase::init(IDirect3DDevice9 *mdevice,string strPath)
{
	if(NULL == mdevice)
		return ;
	m_device = mdevice ;


	m_device->CreateVertexBuffer(m_vbsize*sizeof(Particle),
								 D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY 
								,Particle::FVF,D3DPOOL_DEFAULT,&m_vb,NULL);
	if (!strPath.empty())
	{
		string str = strPath ;
		int Len=::MultiByteToWideChar (CP_ACP, 0, str.c_str(), -1, NULL, 0);
		LPWSTR *buf = new LPWSTR[Len];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, *buf, Len); 
		D3DXCreateTextureFromFile(m_device,*buf,&m_tex);
		delete []buf;
	}
}

//增加粒子
void CParticleBase::addParticle()
{
	ParticleAttr attr ;
	ResetParticle(&attr);
	m_particlelist.push_back(attr);
}

//设定系统中所有粒子的属性
void CParticleBase::reset()
{
	list<ParticleAttr>::iterator it = m_particlelist.begin();
	for ( ; it != m_particlelist.end() ; ++it)
	{
		ResetParticle(&(*it));
	}
}
//virtual void ResetParticle() = 0;

//virtual void Update(float timeDelta ) = 0 ;

//绘制状态
void CParticleBase::preRender()
{

}

//绘制
void CParticleBase::Render(float timeDelta )
{

}

//绘制后状态的修改
void CParticleBase::postRender()
{

}

//删除死去的点
void CParticleBase::removeDeadParticles()
{

}