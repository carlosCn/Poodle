//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "camera.h"
#include "Device.h"

CamDir::CamDir()
{
	_cameraType = AIRCRAFT;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

CamDir::CamDir(CameraType cameraType)
{
	_cameraType = cameraType;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

CamDir::~CamDir()
{

}

void CamDir::getPosition(D3DXVECTOR3* pos)
{
	*pos = _pos;
}

void CamDir::setPosition(D3DXVECTOR3* pos)
{
	_pos = *pos;
}

void CamDir::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void CamDir::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void CamDir::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}

void CamDir::walk(float units)
{
	// move only on xz plane for land object
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _look * units;
}

void CamDir::strafe(float units)
{
	// move only on xz plane for land object
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _right * units;
}

void CamDir::fly(float units)
{
	// move only on y-axis for land object
	if( _cameraType == LANDOBJECT )
		_pos.y += units;

	if( _cameraType == AIRCRAFT )
		_pos += _up * units;
}

//ÇãÐ±
void CamDir::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right,	angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up,&_up, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

//Æ«º½
void CamDir::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( _cameraType == LANDOBJECT )
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if( _cameraType == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

//·­¹ö
void CamDir::roll(float angle)
{
	// only roll for aircraft type
	if( _cameraType == AIRCRAFT )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right,&_right, &T);
		D3DXVec3TransformCoord(&_up,&_up, &T);
	}
}

void CamDir::getViewMatrix(D3DXMATRIX* V)
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&_look, &_look);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	(*V)(0,0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

void CamDir::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}


void Camera::InitCam(CDeviceWIn32* pDevice)
{
	if (pDevice && pDevice->GetVedioDevice())
	{
		m_perspective.aspect = (float)pDevice->GetWinParams().width/(float)pDevice->GetWinParams().height ;
		D3DXMATRIX pPerspective;
		D3DXMatrixPerspectiveFovLH(&pPerspective,m_perspective.fov ,m_perspective.aspect,m_perspective.zn,m_perspective.zf);
		pDevice->GetVedioDevice()->SetTransform(D3DTS_PROJECTION,&pPerspective);

		//±³ÃæÏûÒþ
		pDevice->GetVedioDevice()->SetRenderState(D3DRS_CULLMODE,true);
		pDevice->GetVedioDevice()->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
		pDevice->GetVedioDevice()->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
	}
}
void Camera::InitCam(CDeviceWIn32* pDevice,stPerspectiveParams& _param)
{
	m_perspective = _param ;
	InitCam(pDevice);
}

void Camera::SetPos(CDeviceWIn32* pDevice,D3DXVECTOR3& _campos , D3DXVECTOR3& _targetpos)
{
	if (pDevice && pDevice->GetVedioDevice())
	{
		D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &_campos, &_targetpos, &up);

		pDevice->GetVedioDevice()->SetTransform(D3DTS_VIEW, &V);
	}
}

void Camera::ChangeViewDir(CDeviceWIn32* pDevice,float timeDelta)
{
	if( ::GetAsyncKeyState('W') & 0x8000f )
		m_CamDir.walk(4.0f * timeDelta);

	if( ::GetAsyncKeyState('S') & 0x8000f )
		m_CamDir.walk(-4.0f * timeDelta);

	if( ::GetAsyncKeyState('A') & 0x8000f )
		m_CamDir.strafe(-4.0f * timeDelta);

	if( ::GetAsyncKeyState('D') & 0x8000f )
		m_CamDir.strafe(4.0f * timeDelta);

	if( ::GetAsyncKeyState('R') & 0x8000f )
		m_CamDir.fly(4.0f * timeDelta);

	if( ::GetAsyncKeyState('F') & 0x8000f )
		m_CamDir.fly(-4.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
		m_CamDir.pitch(1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
		m_CamDir.pitch(-1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
		m_CamDir.yaw(-1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
		m_CamDir.yaw(1.0f * timeDelta);

	if( ::GetAsyncKeyState('N') & 0x8000f )
		m_CamDir.roll(1.0f * timeDelta);

	if( ::GetAsyncKeyState('M') & 0x8000f )
		m_CamDir.roll(-1.0f * timeDelta);

	if (pDevice&&pDevice->GetVedioDevice())
	{	
		D3DXMATRIX V;
		m_CamDir.getViewMatrix(&V);
		pDevice->GetVedioDevice()->SetTransform(D3DTS_VIEW, &V);
	}
}