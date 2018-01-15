
#include <stdafx.h>

//следующие файлы потом нужно будет убрать
#include <windows.h>
#include <mmsystem.h>

#include "graph.h"
#include "game_application.h"


GameApplication::GameApplication(bool bWindowed, WNDPROC lpProc)	 
{
  m_hWnd = NULL;
  m_bActive = true;
  m_dwEllapsed = 0;

  m_bWindowed = bWindowed;
  m_lpfnWndProc = lpProc;
 };

GameApplication::~GameApplication()
{
 
}

LPCTSTR GameApplication::getWindowTitle()
{
   return GA_WINDOW_TITLE;
}

void 
GameApplication::getWindowPlacement(int& nX, int& nY, int& nWidth, int& nHeight)
{
   nX = CW_USEDEFAULT;
   nY = CW_USEDEFAULT;
   nWidth = CW_USEDEFAULT;
   nHeight = CW_USEDEFAULT;
}

LPCTSTR 
GameApplication::getWindowMenu()
{
   return NULL; 
}

HICON 
GameApplication::getWindowIcon()
{
   return LoadIcon(0, IDI_APPLICATION);
}

DWORD 
GameApplication::getWindowStyle()
{
  return m_bWindowed ? WS_OVERLAPPEDWINDOW : (WS_POPUP|WS_VISIBLE);   
}

int 
GameApplication::create()
{
 WNDCLASSEX wcl;
 HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);

 wcl.cbSize = sizeof(WNDCLASSEX);
 wcl.cbClsExtra = 0;
 wcl.cbWndExtra = 0;
 wcl.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
 wcl.hCursor = LoadCursor(0, IDC_ARROW);
 wcl.hIcon = getWindowIcon();
 wcl.hIconSm = getWindowIcon();
 wcl.hInstance = hInst;
 wcl.lpfnWndProc = m_lpfnWndProc;
 wcl.lpszClassName = GA_WINDOW_CLASSNAME;
 wcl.lpszMenuName = m_bWindowed ? getWindowMenu() : NULL;
 wcl.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC|CS_DBLCLKS;
 

 if(!RegisterClassEx(&wcl))
 {
	 return FALSE;
 }

 int x, y, width, height;
 getWindowPlacement(x, y, width, height);
 

 m_hWnd = CreateWindow(
	 GA_WINDOW_CLASSNAME,
	 getWindowTitle(),
	 getWindowStyle(),
	 x, y, width, height,
	 NULL, NULL, hInst, NULL);

 if(!m_hWnd)
 {
	 return FALSE;
 }

 /*
 if(m_bWindowed)
 {

	 RECT rect = { x, y, (x + width), (y + height) };

	 AdjustWindowRectEx(&rect, 
		 GetWindowLong(m_hWnd, GWL_STYLE), 
		 GetMenu(m_hWnd) != NULL,
		 GetWindowLong(m_hWnd, GWL_EXSTYLE));
     
	 width = rect.right - rect.left;
	 height = rect.bottom - rect.top;
	 MoveWindow(m_hWnd, x, y, width, height, TRUE);

	 ShowWindow(m_hWnd, SW_SHOW);
 }
 */
 
 ShowWindow(m_hWnd, SW_SHOW);
 UpdateWindow(m_hWnd);
 

 m_pGraphManager = new cGraphManager(m_hWnd);
 m_pGraphManager->init(width, height, m_bWindowed);

 gameInit();

 return TRUE;
}


int 
GameApplication::run()
{
 MSG msg;
 DWORD dwLastTime;

 while(TRUE)
 {
  dwLastTime = timeGetTime();
  
  if(PeekMessage(&msg,0,0,0,PM_REMOVE))
  {
	  if(msg.message==WM_QUIT)
		  break;
	  
	   TranslateMessage(&msg);
	   DispatchMessage(&msg);	  
  }
  
  if(m_bActive)
  {
    gameMain();
  }

  m_dwEllapsed+= timeGetTime() - dwLastTime;
 }
 gameShutdown();
 
 return (int)msg.wParam;
}

LRESULT 
GameApplication::wndProc(HWND hWnd, UINT mes, WPARAM wPar, LPARAM lPar)
{
 PAINTSTRUCT ps;
 HDC dc;

 switch(mes)
 {
 case WM_CREATE:
	 break;
 case WM_ACTIVATE:
	 if(LOWORD(wPar)==WA_INACTIVE)
	 {
      m_bActive = false;
	 }
	 if(LOWORD(wPar)==WA_ACTIVE)
	 {
      m_bActive = true;
	 }
	 break;
 case WM_DESTROY:
	 PostQuitMessage(0);
	 break;
 case WM_PAINT:
     dc = BeginPaint(hWnd,&ps);

	 EndPaint(hWnd,&ps);
	 ReleaseDC(hWnd,dc);
	 break;
 case WM_KEYDOWN:
	 if(wPar==VK_ESCAPE)
		 DestroyWindow(hWnd);
	 break;
 default:
	 return DefWindowProc(hWnd,mes,wPar,lPar);
 }
 return 0;
}

void 
GameApplication::gameInit()
{

}

void 
GameApplication::gameMain()
{
   frameMove();
   render();
}

void 
GameApplication::gameShutdown()
{
	if(m_pGraphManager != NULL)
	{
		delete m_pGraphManager;
	}
}


 HRESULT 
 GameApplication::invalidateDeviceObjects()
 {
  return S_OK;
 }

 HRESULT 
 GameApplication::restoreDeviceObjects()
 {
  return S_OK;
 }

HRESULT 
GameApplication::render()
{
	HRESULT hr;
    static BOOL sbDeviceLost = false;
    
	if(m_pGraphManager == NULL)
	{
		return E_FAIL;
	}
    DEVICECONTEXT device = m_pGraphManager->getDeviceContext();
	if(device == NULL)
	{
		return E_FAIL;
	}

	if(sbDeviceLost)
    {
      hr = device->TestCooperativeLevel();
      if(FAILED(hr))
      {
        if(hr == D3DERR_DEVICELOST)
	       return S_OK;
        if(hr == D3DERR_DEVICENOTRESET)
        { 
	      hr = m_pGraphManager->restoreDevice();
	      if(SUCCEEDED(hr))
		  {
             sbDeviceLost = false;  
             restoreDeviceObjects();		    
		  }else
		  {
			  return hr;
		  }
        }//if(hr == D3DERR_DEVICENOTRESET)   
       }//if(FAILED(hr))       
    }//if(sbDeviceLost)
 
     hr = device->Clear(0, NULL,
	 D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 
	 D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
     if(FAILED(hr))
     {
	   return hr;
     }

     hr = device->BeginScene();
     if(FAILED(hr))
     {
	    return hr;
     }
 
     drawScene(device);

    hr = device->EndScene();
    if(FAILED(hr))
    {
	 return hr;
    }

    hr = device->Present(NULL, NULL, 0, NULL);
    if(hr == D3DERR_DEVICELOST)
    {
	 sbDeviceLost = true;
	 invalidateDeviceObjects();
    }
    
	return S_OK;
}

HRESULT 
GameApplication::drawScene(DEVICECONTEXT device)
{
 return S_OK;
}


 
HRESULT 
GameApplication::frameMove()
{
 HANDLE hThread = GetCurrentThread();
 HRESULT hr;
 
 
 if(m_dwEllapsed > MILLISECONDS_PER_TICK)
 {
  SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);

  while(m_dwEllapsed > MILLISECONDS_PER_TICK)
  {
   hr = updateGameState();
   if(FAILED(hr))
   {
	   return hr;
   }

   m_dwEllapsed-=MILLISECONDS_PER_TICK;
  }

  SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);

  Sleep(0);
 }
 
 return S_OK;
}


HRESULT 
GameApplication::updateGameState()
{
 return S_OK;
}


