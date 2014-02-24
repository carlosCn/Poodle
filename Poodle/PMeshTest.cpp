#include "PMeshTest.h"


CPMeshTest::CPMeshTest(void):
pMesh(NULL),
pPMesh(NULL)
{
}


CPMeshTest::~CPMeshTest(void)
{
	if(pMesh) pMesh->Release();
	if(pPMesh) pPMesh->Release();

}

void CPMeshTest::preRender()
{
	//读取X文件
	 D3DXLoadMeshFromX(
		L"..\\media\\bigship1.x",
		D3DXMESH_MANAGED,
		m_device,
		&adjbuffer,
		&mtrlbuffer,
		NULL,
		&m_mtrlnum,
		&pMesh);
	 
	//读取材质和纹理数据
    D3DXMATERIAL* mtrls = (D3DXMATERIAL*)(mtrlbuffer->GetBufferPointer());
	for(int i = 0 ; i < m_mtrlnum ; ++i)
	{
		mtrls[i].MatD3D.Ambient = 	mtrls[i].MatD3D.Diffuse ;
		mMtrlList.push_back(mtrls[i].MatD3D);

		if(mtrls[i].pTextureFilename!= NULL)
		{
			IDirect3DTexture9* tex = 0 ;

			string str = mtrls[i].pTextureFilename ;
			int Len=::MultiByteToWideChar (CP_ACP, 0, str.c_str(), -1, NULL, 0);
			LPWSTR *buf = new LPWSTR [Len];
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, *buf, Len); 

			D3DXCreateTextureFromFile(m_device,*buf,&tex);
			mTextureList.push_back(tex);
		}
		else
		{
			mTextureList.push_back(NULL);
		}	
	}
	
	mtrlbuffer->Release();

	////生成顶点法线
	if ( !(pMesh->GetFVF() & D3DFVF_NORMAL ))
	{
		ID3DXMesh* pTemplete = 0 ;

		pMesh->CloneMeshFVF(D3DXMESH_MANAGED,
						pMesh->GetFVF()| D3DFVF_NORMAL,
						m_device,
						&pTemplete);

		D3DXComputeNormals(pTemplete,NULL);

		pMesh->Release();
		pMesh = pTemplete  ;
	}

	//网格优化
	pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjbuffer->GetBufferPointer(),
		(DWORD*)adjbuffer->GetBufferPointer(), // new adjacency info
		0, 0);
	//生成渐进网格

	D3DXGeneratePMesh(pMesh,
					(DWORD*)(adjbuffer->GetBufferPointer()),
					NULL,
					NULL,
					1,
					D3DXMESHSIMP_FACE,
					&pPMesh);

	m_facenum = pPMesh->GetMaxFaces();
	pPMesh->SetNumFaces(m_facenum);


	//计算外接体的尺寸
	unsigned char* v = 0;
	D3DXVECTOR3 center ;
	float radius ;

	pMesh->LockVertexBuffer(0, (void**)&v);

	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(pMesh->GetFVF()),
		&center,
		&radius);

	pMesh->UnlockVertexBuffer();

	//产生外接体
	D3DXCreateSphere(m_device,radius,20,20,&pBoundMesh,NULL);

	//光照
	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = initDirectLight(dir,col);

	m_device->SetLight(0, &light);
	m_device->LightEnable(0, true);
	m_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_device->SetRenderState(D3DRS_SPECULARENABLE, true);
	m_device->SetRenderState(D3DRS_LIGHTING,true);
}

void CPMeshTest::Render(float timeDelta )
{
	//网格控制
	if (::GetAsyncKeyState('O'))
	{
		pPMesh->SetNumFaces(m_facenum -= 10 );
		if (m_facenum < 1)
		{
			m_facenum = pPMesh->GetMaxFaces();
		}
	}
	else if (::GetAsyncKeyState('P'))
	{
		pPMesh->SetNumFaces(m_facenum += 10 );
		if (m_facenum >= pPMesh->GetMaxFaces())
		{
			m_facenum = pPMesh->GetMinFaces();
		}
	}

	//绘制渐渐网格
	for (int i = 0 ; i< mMtrlList.size(); ++i )
	{
		D3DXMATRIX V ;
		D3DXMatrixTranslation(&V,0,0,0);
		m_device->SetTransform(D3DTS_WORLD,&V);
		m_device->SetMaterial(&mMtrlList[i]);
		m_device->SetTexture(0,mTextureList[i]);
		pPMesh->DrawSubset(i);

		D3DMATERIAL9 m = YELLOW_MTRL ;
		m_device->SetMaterial(&m);
		m_device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		pPMesh->DrawSubset(i);
		m_device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	}

	//绘制外接体
	
	D3DMATERIAL9 blue = BLUE_MTRL ;
	blue.Diffuse.a = 0.10f; // 10% opacity

	m_device->SetMaterial(&blue);
	m_device->SetTexture(0, 0); // disable texture

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pBoundMesh->DrawSubset(0);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

}
void CPMeshTest::afterRender()
{

}
void CPMeshTest::changedir(float timeDelta)
{

}