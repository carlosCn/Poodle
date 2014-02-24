#pragma once
#include "public_define.h"
#include "DrawObject.h"

class CPMeshTest:
	 public CDrawObject
{
public:
	CPMeshTest(void);
	~CPMeshTest(void);

	 void preRender();
	 void Render(float timeDelta );
	 void afterRender();
	 void changedir(float timeDelta);

private:

	ID3DXMesh* pMesh ;
	ID3DXPMesh* pPMesh;

	ID3DXMesh* pBoundMesh;
	vector<IDirect3DTexture9*> mTextureList ;
	vector<D3DMATERIAL9> mMtrlList ;

	ID3DXBuffer* adjbuffer;
	ID3DXBuffer* mtrlbuffer;
	DWORD m_mtrlnum ;
	
	DWORD m_facenum ;

};

