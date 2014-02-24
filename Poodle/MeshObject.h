#pragma once
#include "public_define.h"
#include "DrawObject.h"

class CMeshObject:public CDrawObject
{
public:
	CMeshObject(void);
	~CMeshObject(void);
	 void preRender();
	 void Render(float timeDelta ) ;
	 void afterRender();
	 void changedir(float timeDelta)  ;
private:
	ID3DXMesh *pMesh; 
	vector<IDirect3DTexture9*> pTextureList;	
	const static int NumSubsets = 3 ;
};

