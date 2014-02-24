// Poodle.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "public_define.h"

#include "camera.h"
#include "Device.h"

#include "BlendTest.h"
#include "StencilTest.h"

#include "MeshObject.h"
#include "PMeshTest.h"

CDeviceWIn32 *mWin32Device = NULL ;
Camera *pCam = NULL ;
CDrawObject *pDrawObject  = NULL;

void ClearDevice()
{
	delete pCam ;
	delete mWin32Device ;
}

void PreDraw()
{
	if (NULL == pCam)  return ;
	if ( NULL == mWin32Device  || NULL == mWin32Device->GetVedioDevice())  return ;



	mWin32Device->InitRenderState();

	//pCam->InitCam(mWin32Device);
	pCam->InitCam(mWin32Device,stPerspectiveParams(2.0,0.75,1.0,1000.0));
	pCam->SetPos(mWin32Device,D3DXVECTOR3(-8, 4, -12),D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//绘制自定义$ss
	pDrawObject->preRender();
}

void Draw(float timeDelta)
{
	if (mWin32Device && mWin32Device->GetVedioDevice())
	{
		pCam->ChangeViewDir(mWin32Device,timeDelta);  // 控制视场视角

		mWin32Device->GetVedioDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		mWin32Device->GetVedioDevice()->BeginScene();

		pDrawObject->Render(timeDelta);

		mWin32Device->GetVedioDevice()->EndScene();
		mWin32Device->GetVedioDevice()->Present(0,0,0,0);
	}
}

void  EndDraw()
{
	delete pDrawObject ;
}
int _tmain(int argc, _TCHAR* argv[])
{
	MSG msg;

	mWin32Device = new CDeviceWIn32(stCreateParams(800,600));
	if (!mWin32Device) return -1 ;

	pCam = new Camera ;
	if (NULL == pCam ) return -1 ;

	//pDrawObject = new CBlendTest ;
	//pDrawObject = new CStencilTest ;
	//pDrawObject = new CMeshObject ;
	pDrawObject = new CPMeshTest ;

	if (pDrawObject == NULL) return -1 ;
	pDrawObject->SetDevice(mWin32Device->GetVedioDevice());

	PreDraw();

	static float lastTime = (float)timeGetTime(); 
	while(mWin32Device && mWin32Device->GetVedioDevice())
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

