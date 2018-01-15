#pragma once


const int MILLISECONDS_PER_TICK  = 10;
const LPCTSTR GA_WINDOW_CLASSNAME = _T("DREAM3D GAME ENGINE WINDOW CLASS");
const LPCTSTR GA_WINDOW_TITLE = _T("DREAM3D GAME APPLICATION");

const int GAMESATE_GAMEOVER = 0;

typedef LPDIRECT3DDEVICE9 DEVICECONTEXT;

class GameApplication
{
public:
 GameApplication(bool bWindowed, WNDPROC lpProc);
 virtual ~GameApplication();
 
 int create();
 int run();
 
 virtual LRESULT wndProc(HWND hWnd, UINT mes, WPARAM wPar, LPARAM lPar);

 //get-functions
 HINSTANCE getHInstance() { return (HINSTANCE)GetModuleHandle(NULL); };
 HWND getHwnd() { return m_hWnd; };

protected:
 virtual void gameInit();
 virtual void gameMain();
 virtual void gameShutdown();

 virtual HRESULT invalidateDeviceObjects(); //on device lost
 virtual HRESULT restoreDeviceObjects(); //retore device and other objects
 virtual HRESULT render(); //render scene
 virtual HRESULT drawScene(DEVICECONTEXT device); //draw scene
 virtual HRESULT frameMove();
 virtual HRESULT updateGameState();
protected:
 virtual LPCTSTR getWindowTitle();
 virtual void getWindowPlacement(int& nX, int& nY, int& nWidth, int& nHeight);
 virtual LPCTSTR getWindowMenu();
 virtual HICON getWindowIcon();
 virtual DWORD getWindowStyle();

 HWND m_hWnd;
 BOOL m_bActive;
 DWORD m_dwEllapsed;

 bool m_bWindowed;
 WNDPROC m_lpfnWndProc;

 cGraphManager* m_pGraphManager;
};


