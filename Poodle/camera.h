//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __cameraH__
#define __cameraH__

#include <d3dx9.h>

class CDeviceWIn32 ;
class CamDir
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };

	CamDir();
	CamDir(CameraType cameraType);
	~CamDir();

	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward
	
	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void getViewMatrix(D3DXMATRIX* V); 
	void setCameraType(CameraType cameraType); 
	void getPosition(D3DXVECTOR3* pos); 
	void setPosition(D3DXVECTOR3* pos); 

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);


private:
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;
};


struct stPerspectiveParams
{
	float fov ;
	float aspect ;
	float zn ;
	float zf ;

	stPerspectiveParams()
	{
		fov    = 1.2 ;
		aspect = 0.75 ;
		zn     = 1.0 ;
		zf     = 1000.0 ;
	}

	stPerspectiveParams(float _fov , float _aspect , float _zn , float _zf)
	{
		fov = _fov ;
		aspect = _aspect ;
		zn = _zn ;
		zf = _zf ;
	}

	stPerspectiveParams& operator= (stPerspectiveParams& _other)
	{
		fov = _other.fov;
		aspect = _other.aspect ;
		zn = _other.zn ;
		zf = _other.zf ;

		return *this ;
	}
};
class Camera
{
public:

	Camera()
	{

	}
	~Camera()
	{

	}
	void InitCam(CDeviceWIn32* pDevice );
	void InitCam(CDeviceWIn32* pDevice , stPerspectiveParams& _param);
	void ChangeViewDir(CDeviceWIn32* pDevice,float timeDelta);

private:
	CamDir m_CamDir;
	stPerspectiveParams m_perspective ;
};
#endif // __cameraH__