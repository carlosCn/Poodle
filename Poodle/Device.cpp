#include "Device.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) ;

CDeviceWIn32::CDeviceWIn32(void):
m_device(NULL)
{
	InitDevice();
}

CDeviceWIn32::CDeviceWIn32(stCreateParams& _params):
m_device(NULL)
{
	m_winSize = _params ;
	InitDevice();
}

CDeviceWIn32::~CDeviceWIn32(void)
{
	if (m_device!=NULL)
	{
		m_device->Release();
	}
	
}

void CDeviceWIn32::InitDevice()
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

	//wc.hIcon = (HICON)LoadImage(hInstance, __TEXT("irrlicht.ico"), IMAGE_ICON, 0,0, LR_LOADFROMFILE);

	if(NULL == RegisterClassEx(&wc))
	{
		return ;
	}

	HWND hwnd = 0;
	DWORD style = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_CLIPCHILDREN;
	hwnd = ::CreateWindowEx(0,L"Direct3D9App", L"Direct3D9App", 
		style ,
		0, 0, m_winSize.width, m_winSize.height,
		0 /*parent hwnd*/, 
		0 /* menu */, 
		hInstance,
		0 /*extra*/); 

	if (hwnd == NULL)
	{
		return ;
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
	d3dpp.BackBufferWidth            = m_winSize.width;
	d3dpp.BackBufferHeight           = m_winSize.height;
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
		D3DADAPTER_DEFAULT,     // primary adapter
		D3DDEVTYPE_HAL,         // device type
		hwnd,                   // window associated with device
		vp,                     // vertex processing
		&d3dpp,                 // present parameters
		&m_device);             // return created device

	if( FAILED(hr) )
	{
		d3d9->Release(); // done with d3d9 object
		return ;
	}

	d3d9->Release(); // done with d3d9 object

	ShowWindow(hwnd,SW_SHOW);
	UpdateWindow(hwnd);

}

void CDeviceWIn32::InitRenderState()
{
	if (m_device)
	{
		//m_device->SetRenderState(D3DRS_SPECULARENABLE , false) ;
		m_device->SetRenderState(D3DRS_LIGHTING , false);
		//m_device->SetRenderState(D3DRS_NORMALIZENORMALS,false);
		//device->SetLight(0,&light);
		//device->LightEnable(0,true);

		m_device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		m_device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		m_device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
		m_device->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
		m_device->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	}



}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}