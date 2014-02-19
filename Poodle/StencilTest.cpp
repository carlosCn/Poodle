#include "StencilTest.h"

const DWORD StencilVetex::FVF = D3DFVF_XYZ /*| D3DFVF_DIFFUSE*/| D3DFVF_NORMAL | D3DFVF_TEX1  ;

CStencilTest::CStencilTest(void):
m_pTeapotMesh(NULL),
m_pFloor(NULL),
m_pMirror(NULL),
m_pWall(NULL)
{
	TeapotPosition.x = 5 ;
	TeapotPosition.y = 10 ;
	TeapotPosition.z = -10 ;
}


CStencilTest::~CStencilTest(void)
{
	m_pWall->Release();
	m_pFloor->Release();
	m_pMirror->Release();
	m_pTeapotMesh->Release();
}

void CStencilTest::preRender() 
{
	//创建墙面，镜面，地板 ,茶壶
	m_device->CreateVertexBuffer(24 * sizeof(StencilVetex),0,StencilVetex::FVF,D3DPOOL_MANAGED,&m_vb,NULL);
	if(!m_vb)
		return ;

	StencilVetex *pBuffer = NULL ;
	m_vb->Lock(0,0,(void**)(&pBuffer),0);

	//floor
	int i = 0 ; 
	pBuffer[i++] = StencilVetex(0,0,-10,0,1,0,  1,0);
	pBuffer[i++] = StencilVetex(0,0,0,  0,1,0,  0,0);
	pBuffer[i++] = StencilVetex(15,0,0, 0,1,0,  0,1);

	pBuffer[i++] = StencilVetex(0,0,-10, 0,1,0,  1,0);
	pBuffer[i++] = StencilVetex(15,0,0,  0,1,0,  0,1);
	pBuffer[i++] = StencilVetex(15,0,-10,0,1,0,  1,1);

	//wall 1
	pBuffer[i++] = StencilVetex(0, 0,  0,  0,0,-1,  1,0);
	pBuffer[i++] = StencilVetex(0, 10, 0,  0,0,-1,  0,0);
	pBuffer[i++] = StencilVetex(5, 10, 0,  0,0,-1,  0,1);

	pBuffer[i++] = StencilVetex(0, 0,  0,  0,0,-1,  1,0);
	pBuffer[i++] = StencilVetex(5, 10, 0,  0,0,-1,  0,1);
	pBuffer[i++] = StencilVetex(5, 0, 0,  0,0,-1,  1,1);

	//wall 2
	pBuffer[i++] = StencilVetex(10, 0,  0,  0,0,-1,  1,0);
	pBuffer[i++] = StencilVetex(10, 10, 0,  0,0,-1,  0,0);
	pBuffer[i++] = StencilVetex(15, 10, 0,  0,0,-1,  0,1);

	pBuffer[i++] = StencilVetex(10, 0,  0,  0,0,-1,  1,0);
	pBuffer[i++] = StencilVetex(15, 10, 0,  0,0,-1,  0,1);
	pBuffer[i++] = StencilVetex(15, 0,  0,  0,0,-1,  1,1);


	//mirror
	pBuffer[i++] = StencilVetex(5, 0,  0,  0,0,-1,  1,0);
	pBuffer[i++] = StencilVetex(5, 10, 0,  0,0,-1,  0,0);
	pBuffer[i++] = StencilVetex(10, 10, 0,  0,0,-1,  0,1);

	pBuffer[i++] = StencilVetex(5, 0,  0,  0,0,-1,  1,0);
	pBuffer[i++] = StencilVetex(10, 10, 0,  0,0,-1,  0,1);
	pBuffer[i++] = StencilVetex(10, 0,  0,  0,0,-1,  1,1);

	m_vb->Unlock();

	//teapot
	D3DXCreateTeapot(m_device,&m_pTeapotMesh,NULL);

	//load texture
	D3DXCreateTextureFromFile(m_device,L"..\\media\\brick0.jpg",&m_pWall);
	D3DXCreateTextureFromFile(m_device,L"..\\media\\ice.bmp",&m_pMirror);
	D3DXCreateTextureFromFile(m_device,L"..\\media\\checker.jpg",&m_pFloor);


	m_mteapot = RED_MTRL; 
	m_mwall   = WHITE_MTRL ;
	m_mmirror = WHITE_MTRL ;
	m_mfloor  = WHITE_MTRL ;
	//灯光
	m_light = initDirectLight(D3DXVECTOR3(0.707f, -0.707f, 0.707f),WHITE);
	m_device->SetRenderState(D3DRS_SPECULARENABLE,TRUE);
	m_device->SetRenderState(D3DRS_NORMALIZENORMALS,true);
	m_device->SetRenderState(D3DRS_LIGHTING,true);

	m_device->SetLight(0,&m_light);
	m_device->LightEnable(0,true);

	TeapotPosition.x = 10 ;
	TeapotPosition.y = 10 ; 
	TeapotPosition.z = -10 ;



}
void CStencilTest::Render(float timeDelta)
{
	if(m_device)
	{
		changedir(timeDelta);
		m_device->SetStreamSource(0,m_vb,0,sizeof(StencilVetex));
		m_device->SetFVF(StencilVetex::FVF);

		m_device->SetTexture(0,m_pFloor);
		m_device->SetMaterial(&m_mfloor);
		m_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		m_device->SetTexture(0,m_pWall);
		m_device->SetMaterial(&m_mwall);
		m_device->DrawPrimitive(D3DPT_TRIANGLELIST,6,4);

		m_device->SetTexture(0,m_pMirror);
		m_device->SetMaterial(&m_mmirror);
		m_device->DrawPrimitive(D3DPT_TRIANGLELIST,18,2);

		m_device->SetMaterial(&m_mteapot);
		D3DXMATRIX v ;
		D3DXMatrixTranslation(&v,TeapotPosition.x,TeapotPosition.y,TeapotPosition.z);
		m_device->SetTransform(D3DTS_WORLD,&v);
		m_pTeapotMesh->DrawSubset(0);
	}

}

void CStencilTest::changedir(float timeDelta)   
{
	static float radius = 20.0f;

	if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
		TeapotPosition.x -= 3.0f * timeDelta;

	if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
		TeapotPosition.x += 3.0f * timeDelta;

	if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
		radius -= 2.0f * timeDelta;

	if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
		radius += 2.0f * timeDelta;


	static float angle = (3.0f * D3DX_PI) / 2.0f;

	if( ::GetAsyncKeyState('A') & 0x8000f )
		angle -= 0.5f * timeDelta;

	if( ::GetAsyncKeyState('S') & 0x8000f )
		angle += 0.5f * timeDelta;

	D3DXVECTOR3 position( cosf(angle) * radius, 20.0f, sinf(angle) * radius );
	D3DXVECTOR3 target(10.0f, 5.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	m_device->SetTransform(D3DTS_VIEW, &V);

}

void CStencilTest::afterRender()
{

}