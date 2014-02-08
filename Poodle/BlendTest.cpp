#include "BlendTest.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "WindowBase.h"

CBlendTest::CBlendTest(void):
CrateTex(NULL),
pBKTeture(NULL),
m_pMesh(NULL)
{

}


CBlendTest::~CBlendTest(void)
{

}

void CBlendTest::preRender()
{
	if(NULL == m_device)
		return ;

	m_device->CreateVertexBuffer(6*sizeof(BlendTestVertex),0,BlendTestVertex::FVF,D3DPOOL_MANAGED,&m_vb,NULL);
	BlendTestVertex *pBuffer = NULL ;
	m_vb->Lock(0,0,(void**)(&pBuffer),0);
	pBuffer[0] = BlendTestVertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	pBuffer[1] = BlendTestVertex(-10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	pBuffer[2] = BlendTestVertex( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	pBuffer[3] = BlendTestVertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	pBuffer[4] = BlendTestVertex( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	pBuffer[5] = BlendTestVertex( 10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	m_vb->Unlock();	


	D3DXCreateTeapot(m_device,&m_pMesh,NULL);

	pMeterial[0] = initMat(D3DXCOLOR(D3DCOLOR_XRGB(0,0,0)),D3DXCOLOR(D3DCOLOR_XRGB(255,255,0)),D3DXCOLOR(D3DCOLOR_XRGB(0,0,0)),D3DXCOLOR(D3DCOLOR_XRGB(0,0,0)),1.0);
	pMeterial[1] = initMat(D3DXCOLOR(D3DCOLOR_XRGB(255,0,0)),D3DXCOLOR(D3DCOLOR_XRGB(255,0,0)),D3DXCOLOR(D3DCOLOR_XRGB(255,0,0)),D3DXCOLOR(D3DCOLOR_XRGB(0,0,0)),1.0);

	//pMeterial[0].Diffuse.a = 0.5 ;
	pMeterial[1].Diffuse.a = 0.5 ;

	D3DXCreateTextureFromFile(
		m_device,
		L"cratewalpha.dds",
		&CrateTex);


	m_light = initDirectLight(D3DXVECTOR3(0,0,1),D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)));
	//light = initSpotLight(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,1),D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)));
	//light = initPointLight(D3DXVECTOR3(0,0,0),D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)));
	m_device->SetRenderState(D3DRS_SPECULARENABLE , true) ;
	m_device->SetRenderState(D3DRS_LIGHTING , true);
	m_device->SetRenderState(D3DRS_NORMALIZENORMALS,true);
	m_device->SetLight(0,&m_light);
	m_device->LightEnable(0,true);

	D3DXCreateTextureFromFile(m_device,L"crate.jpg",&pBKTeture);
	m_device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	m_device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	m_device->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
	m_device->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);

	m_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	//m_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	m_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);

	m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

}
void CBlendTest::Render()
{
	if (m_device == NULL)
		return ;

	D3DXMATRIX SRC ;
	D3DXMatrixTranslation(&SRC,  0.0f,  0.0f, 0.0f);
	m_device->SetTransform(D3DTS_WORLD, &SRC);
	m_device->SetStreamSource(0,m_vb,0,sizeof(BlendTestVertex));
	m_device->SetTexture(0,pBKTeture);
	m_device->SetMaterial(&pMeterial[0]);
	m_device->SetFVF(BlendTestVertex::FVF);
	m_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//D3DXMatrixTranslation(&SRC,  0.0f,  0.0f, 2.0f);
	//device->SetTransform(D3DTS_WORLD, &SRC);
	//device->SetMaterial(&pMeterial[1]);
	//pMesh[0]->DrawSubset(0);

	D3DXMatrixTranslation(&SRC,  0.0f,  0.0f, 2.0f);
	m_device->SetTransform(D3DTS_WORLD, &SRC);

	D3DXMatrixScaling(&SRC,0.5,0.5,0.5);
	m_device->SetTransform(D3DTS_WORLD, &SRC);

	m_device->SetStreamSource(0,m_vb,0,sizeof(BlendTestVertex));
	m_device->SetTexture(0,CrateTex);
	m_device->SetMaterial(&pMeterial[1]);
	m_device->SetFVF(BlendTestVertex::FVF);
	m_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
void CBlendTest::afterRender()
{

}
