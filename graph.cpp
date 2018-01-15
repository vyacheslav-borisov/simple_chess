#include <stdafx.h>
#include "graph.h"

using namespace std; 



bool modeSortFunc(const D3DDISPLAYMODE& el1, const D3DDISPLAYMODE& el2)
{
	if(el1.Width < el2.Width)
	{
		return false;
	}
	if(el1.Height < el2.Height)
	{
		return false;
	}
	if(el1.RefreshRate < el2.RefreshRate)
	{
		return false;
	}
	return true;
}

cGraphManager::cGraphManager(HWND hWindow)
{
 m_hWnd = hWindow;
 m_bWindowed = FALSE;
 m_nScreenWidth = 0;
 m_nScreenHeight = 0;

 m_lpd3dMain = NULL;
 m_lpd3dDevice = NULL;
}

cGraphManager::~cGraphManager()
{
 destroy();
}

BOOL 
cGraphManager::init(int nScreenWidth, int nScreenHeight, BOOL bWindowed)
{
 m_bWindowed = bWindowed;
 m_nScreenWidth = nScreenWidth;
 m_nScreenHeight = nScreenHeight;

 if(m_lpd3dMain == NULL)
 {
  m_lpd3dMain = Direct3DCreate9(D3D_SDK_VERSION);  
 }
 if(m_lpd3dMain == NULL)
	 return FALSE;

 if(bWindowed)
 {
  m_lpd3dDevice = createWindowedDevice(); 
 }else
 {
  m_lpd3dDevice = createFullScreenDevice();
 }
 
 if(m_lpd3dDevice == NULL)
 {
	 return FALSE;
 }
 
 return TRUE;
}

BOOL 
cGraphManager::render()
{
 HRESULT hr;
 static BOOL sbDeviceLost = false;
 if(sbDeviceLost)
 {
  hr = m_lpd3dDevice->TestCooperativeLevel();
  if(FAILED(hr))
  {
   if(hr == D3DERR_DEVICELOST)
	  return TRUE;
   if(hr == D3DERR_DEVICENOTRESET)
   { 
	  hr = restoreDevice();
	  if(FAILED(hr))
		  return FALSE;
   }//if(hr == D3DERR_DEVICENOTRESET)

   
  }//if(FAILED(hr))
  sbDeviceLost = false;
 }//if(sbDeviceLost)
 
 hr = m_lpd3dDevice->Clear(0, NULL,
	 D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 
	 D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
 if(FAILED(hr))
 {
	 return hr;
 }

 hr = m_lpd3dDevice->BeginScene();
 if(FAILED(hr))
 {
	 return hr;
 }
 
 //DrawScene

 hr = m_lpd3dDevice->EndScene();
 if(FAILED(hr))
 {
	 return hr;
 }

 hr = m_lpd3dDevice->Present(NULL, NULL, 0, NULL);
 if(hr == D3DERR_DEVICELOST)
 {
	 sbDeviceLost = true;
 }
 return TRUE;
}

void 
cGraphManager::destroy()
{
 if(m_lpd3dDevice != NULL)
 {
	 m_lpd3dDevice->Release();
	 m_lpd3dDevice = NULL;
 }
 if(m_lpd3dMain != NULL)
 { 
	 m_lpd3dMain->Release();
	 m_lpd3dMain = NULL;
 } 
}

LPDIRECT3DDEVICE9 
cGraphManager::createWindowedDevice()
{
	D3DDISPLAYMODE d3ddm;
	HRESULT hr;

	if(m_lpd3dMain == NULL)
	{
		return NULL;
	}
    
	hr = m_lpd3dMain->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if(FAILED(hr))
	{
		return NULL;
	}

	memset(&m_d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferWidth = d3ddm.Width;
	m_d3dpp.BackBufferHeight = d3ddm.Height;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
	
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	LPDIRECT3DDEVICE9 pResult = NULL;

	hr = m_lpd3dMain->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &pResult);
	if(FAILED(hr))
	{
		return NULL;
	}

	return pResult;
}

LPDIRECT3DDEVICE9 
cGraphManager::createFullScreenDevice()
{
	const int k_nNumFormats = 6;
	D3DFORMAT vFormats[k_nNumFormats] = 
	{
		D3DFMT_A2R10G10B10, D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8,  
        D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5
	};
    
	HRESULT hr;
	D3DFORMAT fmtFormat;
	for(int i = 0; i < k_nNumFormats; i++)
	{
		hr = m_lpd3dMain->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			vFormats[i], vFormats[i], FALSE);
		if(SUCCEEDED(hr))
		{
			fmtFormat = vFormats[i];
			break;
		}
	}

	UINT uModeCount = m_lpd3dMain->GetAdapterModeCount(D3DADAPTER_DEFAULT, fmtFormat);
	D3DDISPLAYMODE d3ddm;
	std::vector<D3DDISPLAYMODE> vModes;
	for(UINT i = 0; i < uModeCount; i++)
	{
		m_lpd3dMain->EnumAdapterModes(D3DADAPTER_DEFAULT, fmtFormat, i, &d3ddm);
		vModes.push_back(d3ddm);
	}
	sort(vModes.begin(), vModes.end(), modeSortFunc);
     
    D3DDISPLAYMODE chosedMode = vModes[0];

	const int k_nDepthStencilFormatCount = 5; 
    D3DFORMAT vDepthStencilFormats[k_nDepthStencilFormatCount] =
	{
	 D3DFMT_D24S8, 
     D3DFMT_D24X8, 
     D3DFMT_D24X4S4, 
     D3DFMT_D24FS8, 
     D3DFMT_D15S1	 
	};
	D3DFORMAT fmtDepthStencilFormat;
	for(int i = 0; i < k_nDepthStencilFormatCount; i++)
	{
		hr = m_lpd3dMain->CheckDeviceFormat(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, chosedMode.Format,
			D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, vDepthStencilFormats[i]);
		if(FAILED(hr))
			continue;
		hr = m_lpd3dMain->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, chosedMode.Format, fmtFormat, vDepthStencilFormats[i]);
		if(SUCCEEDED(hr))
		{
			fmtDepthStencilFormat = vDepthStencilFormats[i];
			break;
		}				
	}

	memset(&m_d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dpp.BackBufferWidth = chosedMode.Width;
	m_d3dpp.BackBufferHeight = chosedMode.Height;
	m_d3dpp.BackBufferFormat = fmtFormat;
	m_d3dpp.FullScreen_RefreshRateInHz = chosedMode.RefreshRate;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = fmtDepthStencilFormat;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.Windowed = FALSE;
	
	LPDIRECT3DDEVICE9 pResult = NULL;

	hr = m_lpd3dMain->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &pResult);
	if(FAILED(hr))
	{
		return NULL;
	}
	return pResult;
}

HRESULT 
cGraphManager::restoreDevice()
{
 if(m_bWindowed)
 {
	 D3DDISPLAYMODE d3ddm;
	 m_lpd3dMain->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	 m_d3dpp.BackBufferFormat = d3ddm.Format;
	
 }
 //InvalidateDeviceObjects
 return m_lpd3dDevice->Reset(&m_d3dpp);
}