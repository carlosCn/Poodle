// Poodle.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include<stdio.h>
#include<iostream>
#include <Windows.h>
#include "d3d9.h"
#include "d3dx9.h"
#include "WindowBase.h"
#include "camera.h"


CDeviceWIn32 *mWin32Device = NULL ;
Camera *pCam = NULL ;

IDirect3DDevice9* device = NULL ; 
IDirect3DVertexBuffer9* VB = 0;


ID3DXMesh* pMesh[4] = {NULL , NULL , NULL , NULL};
D3DXMATRIX Worlds[4] ;
D3DMATERIAL9 pMeterial[4] ;
D3DLIGHT9 light ;

//背景纹理
IDirect3DTexture9 *pBKTeture = NULL ;

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z , float nx, float ny, float nz ,float u , float v)
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
const DWORD Vertex::FVF = D3DFVF_XYZ /*| D3DFVF_DIFFUSE*/| D3DFVF_NORMAL | D3DFVF_TEX1  ;

void ClearDevice()
{
	delete pCam ;
}

void PreDraw()
{
	pCam = new Camera ;

	if (!pCam)
		return ;

	if (!device)
		return ;

	{
		device->CreateVertexBuffer(6*sizeof(Vertex),0,Vertex::FVF,D3DPOOL_MANAGED,&VB,NULL);
		//device->CreateIndexBuffer(12*sizeof(DWORD),0,D3DFMT_INDEX32,D3DPOOL_MANAGED,&IB,NULL);

		Vertex *pBuffer = NULL ;
		VB->Lock(0,0,(void**)(&pBuffer),0);
		//pBuffer[0] = Vertex(-2,0,0,COLOR_RED);
		//pBuffer[1] = Vertex(2,0,0,COLOR_GREEN);
		//pBuffer[2] = Vertex(0,0,2,COLOR_BLUE);
		//pBuffer[3] = Vertex(0,3,0,COLOR_RED);

		//pBuffer[0] = Vertex(-2,-2,0, 0.0f, -0.707f, 0.707f, 0.0 ,1.0f);
		//pBuffer[1] = Vertex( 2,-2,0, 0.0f, -0.707f, 0.707f, 1.0 ,0.0f);
		//pBuffer[2] = Vertex(-2, 2,0, 0.0f, -0.707f, 0.707f, 0.0 ,1.0f);
		//pBuffer[3] = Vertex(-2, 2,0, 0.0f, -0.707f, 0.707f, 1.0 ,0.0f);


		pBuffer[0] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		pBuffer[1] = Vertex(-10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		pBuffer[2] = Vertex( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

		pBuffer[3] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		pBuffer[4] = Vertex( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		pBuffer[5] = Vertex( 10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

		VB->Unlock();	
	}


	D3DXCreateTeapot(device,&pMesh[0],NULL);

	//D3DXCreateSphere(device, 1.0f, 20, 20, &pMesh[1], 0);
	//D3DXCreateTorus(device, 0.5f, 1.0f, 20, 20 , &pMesh[2], 0);
	//D3DXCreateCylinder(device, 0.5f, 0.5f, 2.0f, 20, 20, &pMesh[3], 0);

	pMeterial[0] = initMat(
		D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)),
		D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)),
		D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)),
		D3DXCOLOR(D3DCOLOR_XRGB(0,0,0)),1.0);
	pMeterial[1] = initMat(D3DXCOLOR(D3DCOLOR_XRGB(255,0,0)),D3DXCOLOR(D3DCOLOR_XRGB(128,0,0)),D3DXCOLOR(D3DCOLOR_XRGB(255,0,0)),
		D3DXCOLOR(D3DCOLOR_XRGB(0,0,0)),1.0);
	//pMeterial[2] = initMat(D3DXCOLOR(D3DCOLOR_XRGB(255,255,0)),D3DXCOLOR(D3DCOLOR_XRGB(255,255,0)),D3DXCOLOR(D3DCOLOR_XRGB(255,255,0)),
	//	D3DXCOLOR(D3DCOLOR_XRGB(0,0,0)),1.0);
	//pMeterial[3] = initMat(D3DXCOLOR(D3DCOLOR_XRGB(0,0,255)),D3DXCOLOR(D3DCOLOR_XRGB(0,0,255)),D3DXCOLOR(D3DCOLOR_XRGB(0,0,255)),
	//	D3DXCOLOR(D3DCOLOR_XRGB(0,0,0)),1.0);



	//世界坐标系-->视觉坐标系-->透视投影表换-->视口变换


	//摄像机需要的变换在这里进行
//	pCam->InitCam(mWin32Device);
	pCam->InitCam(mWin32Device,stPerspectiveParams(2.4,0.75,1.0,1000.0));

	mWin32Device->InitRenderState();

	light = initDirectLight(D3DXVECTOR3(0,0,1),D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)));
    //light = initSpotLight(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,1),D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)));
	//light = initPointLight(D3DXVECTOR3(0,0,0),D3DXCOLOR(D3DCOLOR_XRGB(255,255,255)));
	device->SetRenderState(D3DRS_SPECULARENABLE , true) ;
	device->SetRenderState(D3DRS_LIGHTING , true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS,true);
	device->SetLight(0,&light);
	device->LightEnable(0,true);


	D3DXCreateTextureFromFile(device,L"crate.jpg",&pBKTeture);
	device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	device->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
	device->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);


	device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);

	device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

void Draw(float timeDelta)
{
	//世界变换 
	D3DXMATRIX pX,pY,p ;
	//static float angleX  = 1.0f;
	//static float angleY  = 1.0f;
	//D3DXMatrixRotationX(&pX,angleX);
	//D3DXMatrixRotationX(&pY,angleY);

	//angleX += 0.0005 ;
	//angleY += 0.0005;
	//if (angleX > 6.28)
	//{
	//	angleX = 0 ;
	//}

	//if (angleY > 6.28)
	//{
	//	angleY = 0 ;
	//}

	//p = pX * pY ;

	//device->SetTransform(D3DTS_WORLD,&p);


	//static float angle = (3.0f * D3DX_PI) / 2.0f;

	//if( ::GetAsyncKeyState(VK_NUMPAD6) & 0x8000f )
	//	light.Direction.x -= 0.5;

	//if( ::GetAsyncKeyState(VK_NUMPAD4) & 0x8000f )
	//	light.Direction.x += 0.5;

	//if( ::GetAsyncKeyState(VK_NUMPAD2) & 0x8000f )
	//	light.Direction.y -= 0.5;

	//if( ::GetAsyncKeyState(VK_NUMPAD8) & 0x8000f )
	//	light.Direction.y += 0.5;

	pCam->ChangeViewDir(mWin32Device,timeDelta);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	device->BeginScene();
	{
		D3DXMATRIX SRC ;
		D3DXMatrixTranslation(&SRC,  0.0f,  0.0f, 0.0f);
		device->SetTransform(D3DTS_WORLD, &SRC);
		device->SetStreamSource(0,VB,0,sizeof(Vertex));
		device->SetTexture(0,pBKTeture);
		device->SetMaterial(&pMeterial[0]);
		device->SetFVF(Vertex::FVF);
		device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);


		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		D3DXMatrixTranslation(&SRC,  0.0f,  0.0f, 2.0f);
		device->SetTransform(D3DTS_WORLD, &SRC);
		device->SetMaterial(&pMeterial[1]);
		pMesh[0]->DrawSubset(0);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	}

	device->EndScene();
	device->Present(0,0,0,0);
}

void  EndDraw()
{
	VB->Release();
}
int _tmain(int argc, _TCHAR* argv[])
{
	MSG msg;
	mWin32Device = new CDeviceWIn32(stCreateParams(800,600));
	if (!mWin32Device)
		return -1 ;

	device = mWin32Device->GetVedioDevice();

	if (!device)
		return -1;

	PreDraw();

	static float lastTime = (float)timeGetTime(); 
	while(device)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break ;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			Draw(timeDelta);
			lastTime = currTime;
		}
	}

	EndDraw();
	ClearDevice();
	return 0;
}

