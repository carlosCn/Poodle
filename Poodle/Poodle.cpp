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
#include "BlendTest.h"

CDeviceWIn32 *mWin32Device = NULL ;
Camera *pCam = NULL ;
IDirect3DDevice9* device = NULL ; 
CBlendTest *pBlendtest = NULL  ;

void ClearDevice()
{
	delete pCam ;
}

void PreDraw()
{

	if (NULL == pCam)  
		return ;

	if (NULL == device && NULL == mWin32Device)  
		return ;

	mWin32Device->InitRenderState();

	//pCam->InitCam(mWin32Device);
	pCam->InitCam(mWin32Device,stPerspectiveParams(2.4,0.75,1.0,1000.0));

	pBlendtest->preRender();
}

void Draw(float timeDelta)
{
	//世界变换 
	//D3DXMATRIX pX,pY,p ;
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

	pBlendtest->Render();

	device->EndScene();
	device->Present(0,0,0,0);
}

void  EndDraw()
{
	delete pBlendtest ;
}
int _tmain(int argc, _TCHAR* argv[])
{
	MSG msg;

	mWin32Device = new CDeviceWIn32(stCreateParams(800,600));
	if (!mWin32Device)
		return -1 ;

	pBlendtest = new CBlendTest ;
	if (pBlendtest == NULL)
		return -1 ;

	pBlendtest->SetDevice(mWin32Device->GetVedioDevice());

	pCam = new Camera ;
	if (NULL == pCam )
		return -1 ;


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

