#pragma once
#include <Windows.h>
#include "d3d9.h"
#include "d3dx9.h"

struct stCreateParams
{
	int width ;
	int height ;
	stCreateParams()
	{
		width = 600 ;
		height = 800 ;
	}
	stCreateParams(int _width , int _height)
	{
		width  = _width ;
		height = _height ;
	}

	stCreateParams& operator= (stCreateParams& _other)
	{
		width = _other.width ;
		height = _other.height ;

		return *this;
	}
};

class CDeviceWIn32
{
public:
	CDeviceWIn32(void);
	CDeviceWIn32(stCreateParams& _params);
	~CDeviceWIn32(void);

	IDirect3DDevice9* GetVedioDevice()
	{
		return m_device ;
	}

	const stCreateParams&  GetWinParams() const 
	{
		return m_winSize ;
	}

	void InitRenderState();
private:
	IDirect3DDevice9* m_device; 
	stCreateParams m_winSize;

	void InitDevice();


};

