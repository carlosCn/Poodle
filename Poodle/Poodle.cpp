// Poodle.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include<stdio.h>
#include<iostream>
#include <Windows.h>
#include "d3d9.h"
#include "d3dx9.h"


IDirect3DDevice9* device = 0; 

const int width  = 640;
const int height = 480;

IDirect3DVertexBuffer9* VB = 0;
IDirect3DIndexBuffer9*  IB = 0;


struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z)
	{
		_x = x;  _y = y;  _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		//if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//创建窗口
bool InitDevice()
{
	WNDCLASSEX wc;

	HINSTANCE hInstance = GetModuleHandle(0);
	wc.cbSize		 = sizeof(WNDCLASSEX) ;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"Direct3D9App";
	wc.hIconSm = 0 ;

	wc.hIcon = (HICON)LoadImage(hInstance, __TEXT("irrlicht.ico"), IMAGE_ICON, 0,0, LR_LOADFROMFILE);

	if(NULL == RegisterClassEx(&wc))
	{
		return false;
	}

	HWND hwnd = 0;
	DWORD style = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_CLIPCHILDREN;
	hwnd = ::CreateWindowEx(0,L"Direct3D9App", L"Direct3D9App", 
		style ,
		0, 0, width, height,
		0 /*parent hwnd*/, 
		0 /* menu */, 
		hInstance,
		0 /*extra*/); 
	if (hwnd == NULL)
	{
		return false;
	}

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);


	// Step 2: Check for hardware vp.
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.
	HRESULT hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		D3DDEVTYPE_HAL,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&device);            // return created device

	if( FAILED(hr) )
	{
		d3d9->Release(); // done with d3d9 object
		return false;
	}

	d3d9->Release(); // done with d3d9 object

	ShowWindow(hwnd,SW_SHOW);
	UpdateWindow(hwnd);

	return true ;
}

void ClearDevice()
{
	device->Release();
}

void PreDraw()
{
	if (!device)
		return ;

	device->CreateVertexBuffer(4*sizeof(Vertex),0,Vertex::FVF,D3DPOOL_MANAGED,&VB,NULL);
	device->CreateIndexBuffer(12*sizeof(DWORD),0,D3DFMT_INDEX32,D3DPOOL_MANAGED,&IB,NULL);

	Vertex *pBuffer = NULL ;
	VB->Lock(0,0,(void**)(&pBuffer),0);
	pBuffer[0] = Vertex(-2,0,0);
	pBuffer[1] = Vertex(2,0,0);
	pBuffer[2] = Vertex(0,0,2);
	pBuffer[3] = Vertex(0,3,0);


	VB->Unlock();

	DWORD *pIndex = NULL ;
	IB->Lock(0,0,(void**)(&pIndex),0);

	pIndex[0] = 0 ;
	pIndex[1] = 1 ;
	pIndex[2] = 2 ;

	pIndex[3] = 0 ;
	pIndex[4] = 1 ;
	pIndex[5] = 3 ;

	pIndex[6] = 0 ;
	pIndex[7] = 2 ;
	pIndex[8] = 3 ;

	pIndex[9] = 1 ;
	pIndex[10] = 2 ;
	pIndex[11] = 3 ;

	IB->Unlock();

	//世界坐标系-->视觉坐标系-->透视投影表换-->视口变换

	D3DXVECTOR3 CamPos(-5,5,5);
	D3DXVECTOR3 targetPos(0,0,0);
	D3DXVECTOR3 pUp(0,1,0);
	D3DXMATRIX pout;
	D3DXMatrixLookAtLH(&pout,&CamPos,&targetPos,&pUp);
	device->SetTransform(D3DTS_VIEW,&pout);
	
	D3DXMATRIX pPerspective;
	D3DXMatrixPerspectiveFovLH(&pPerspective,1.0 ,(float)width/(float)height,1.0,1000.0);
	device->SetTransform(D3DTS_PROJECTION,&pPerspective);

	//背面消隐
	device->SetRenderState(D3DRS_CULLMODE,TRUE);
	device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);

	D3DVIEWPORT9 vp;
	vp.X = 0 ; 
	vp.Y = 0 ;
	vp.Width = width ;
	vp.Height = height ;
	vp.MinZ = 0;
	vp.MaxZ = 1;
	device->SetViewport(&vp);
}

void Draw()
{
	//世界变换 
	D3DXMATRIX pX,pY,p ;
	static float angleX  = 1.0f;
	static float angleY  = 0.0f;
	D3DXMatrixRotationX(&pX,angleX);
	D3DXMatrixRotationX(&pY,angleY);

	//angleX += 0.05 ;
	//angleY += 0.05;
	//if (angleX > 6.28)
	//{
	//}

	//if (angleY > 6.28)
	//{
	//	angleY = 0 ;
	//}

	p = pX * pY ;

	device->SetTransform(D3DTS_WORLD,&p);
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	device->BeginScene();

	device->SetStreamSource(0,VB,0,sizeof(Vertex));
	device->SetFVF(Vertex::FVF);
	device->SetIndices(IB);
	//device->DrawPrimitive(D3DPT_TRIANGLELIST,0,4);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,4);

	device->EndScene();
	device->Present(0,0,0,0);

}

void  EndDraw()
{
	IB->Release();
	VB->Release();
}
int _tmain(int argc, _TCHAR* argv[])
{
	MSG msg;
	bool initSuccess = InitDevice();
	PreDraw();

	while(initSuccess)
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


		Draw();

	}

	EndDraw();
	ClearDevice();
	return 0;
}

