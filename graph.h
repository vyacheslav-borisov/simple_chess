#pragma once

typedef LPDIRECT3DDEVICE9 DEVICECONTEXT;

class cGraphManager
{
public:
	cGraphManager(HWND hWindow);
	~cGraphManager();
	
	BOOL init(int nScreenWidth = 0, int nScreenHeight = 0, BOOL bWindowed = TRUE);
	BOOL render();
	void destroy();
	
	HRESULT restoreDevice();
	DEVICECONTEXT getDeviceContext() { return m_lpd3dDevice; };
private:
   HWND m_hWnd;
   BOOL m_bWindowed;
   int m_nScreenWidth;
   int m_nScreenHeight;

   LPDIRECT3D9 m_lpd3dMain;
   LPDIRECT3DDEVICE9 m_lpd3dDevice;

   LPDIRECT3DDEVICE9 createWindowedDevice();
   LPDIRECT3DDEVICE9 createFullScreenDevice();
   

   D3DPRESENT_PARAMETERS m_d3dpp;
};

