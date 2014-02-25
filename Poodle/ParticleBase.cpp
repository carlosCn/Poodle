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

//��������
void CParticleBase::addParticle()
{
	ParticleAttr attr ;
	ResetParticle(&attr);
	m_particlelist.push_back(attr);
}

//�趨ϵͳ���������ӵ�����
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

//����״̬
void CParticleBase::preRender()
{

}

//����
void CParticleBase::Render(float timeDelta )
{

}

//���ƺ�״̬���޸�
void CParticleBase::postRender()
{

}

//ɾ����ȥ�ĵ�
void CParticleBase::removeDeadParticles()
{

}