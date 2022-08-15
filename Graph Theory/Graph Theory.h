#pragma once

#include "resource.h"

class cGraphTheory
{
public:
	cGraphTheory() {
		nWidth = 1280;
		nHeight = 800;
		hwnd_pos_x = 0;
		hwnd_pos_y = 0;
		pD3D = NULL;
		pd3dDevice = NULL;
		d3dpp = {};
	}

	int nWidth, nHeight, hwnd_pos_x, hwnd_pos_y;

	LPDIRECT3D9              pD3D;
	LPDIRECT3DDEVICE9        pd3dDevice;
	D3DPRESENT_PARAMETERS    d3dpp;

	ATOM MyRegisterClass(HINSTANCE hInstance);
	void GetDesktopResolution();
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void ResetDevice();
};

extern cGraphTheory* GraphTheory;