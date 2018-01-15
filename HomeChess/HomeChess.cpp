// HomeChess.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "structs/singleton.h"
#include "graph.h"
#include "game_application.h"
#include "animation.h"
#include "gui/GUICommon.h"
#include "ai/ChessCommon.h"

#include "hcgraph.h"
#include "HomeChess.h"
#include "GameObject.h"

#define GENERATE_GAMEOBJECT_ID(color, type) (((color)<<16)|(type))


const int HomeChessApllication::m_TexIdentifiers[37] = {
   HOMECHESS_WHITE_BISHOP, 
   HOMECHESS_WHITE_KNIGHT, 
   HOMECHESS_WHITE_ROOK,
   HOMECHESS_WHITE_QUEEN, 
   HOMECHESS_WHITE_KING, 
   HOMECHESS_BLACK_PAWN,
   HOMECHESS_BLACK_BISHOP,
   HOMECHESS_BLACK_ROOK,
   HOMECHESS_BLACK_QUEEN,
   HOMECHESS_BLACK_KING, 
   HOMECHESS_BOARD, 
   HOMECHESS_BLACK_KNIGHT,
   HOMECHESS_WHITE_PAWN,
   
   HOMECHESS_HIGHLIGTH_WHITE_PAWN,
   HOMECHESS_HIGHLIGTH_WHITE_KNIGHT,
   HOMECHESS_HIGHLIGTH_WHITE_BISHOP,
   HOMECHESS_HIGHLIGTH_WHITE_ROOK,
   HOMECHESS_HIGHLIGTH_WHITE_QUEEN,
   HOMECHESS_HIGHLIGTH_WHITE_KING,
   HOMECHESS_HIGHLIGTH_BLACK_PAWN,
   HOMECHESS_HIGHLIGTH_BLACK_KNIGHT,
   HOMECHESS_HIGHLIGTH_BLACK_BISHOP,
   HOMECHESS_HIGHLIGTH_BLACK_ROOK,
   HOMECHESS_HIGHLIGTH_BLACK_QUEEN,
   HOMECHESS_HIGHLIGTH_BLACK_KING,

   HOMECHESS_DROPPED_WHITE_PAWN,
   HOMECHESS_DROPPED_WHITE_KNIGHT,
   HOMECHESS_DROPPED_WHITE_BISHOP,
   HOMECHESS_DROPPED_WHITE_ROOK,
   HOMECHESS_DROPPED_WHITE_QUEEN,
   HOMECHESS_DROPPED_WHITE_KING,
   HOMECHESS_DROPPED_BLACK_PAWN,
   HOMECHESS_DROPPED_BLACK_KNIGHT,
   HOMECHESS_DROPPED_BLACK_BISHOP,
   HOMECHESS_DROPPED_BLACK_ROOK,
   HOMECHESS_DROPPED_BLACK_QUEEN,
   HOMECHESS_DROPPED_BLACK_KING
};

void debug_outputTurn(TurnInfo turn)
{
    string turnType = "";
    switch(turn._type)
    {
    case TURNTYPE_UNKNOWN:
        {
            turnType = "unknown";
            break;
        }
    case TURNTYPE_ATTACK:
        {
            turnType = "attack";
            break;
        }
    case TURNTYPE_NEITRAL:
        {
            turnType = "neitral";
            break;
        }
    case TURNTYPE_BAD:
        {
            turnType = "bad";
            break;
        }
    case TURNTYPE_CONTR:
        {
            turnType = "contr";
            break;
        }
    case TURNTYPE_ESCAPE:
        {
            turnType = "escape";
            break;
        }
    case TURNTYPE_UMBRELLA:
        {
            turnType = "umbrella";
            break;
        }
    case TURNTYPE_PROTECT:
        {
            turnType = "protect";
            break;
        }
    case TURNTYPE_WARD:
        {
            turnType = "ward";
            break;
        }
    default:
        {
            turnType = "unknown";
            break;
        }
    }

    char windowTitle[512];
    sprintf(windowTitle, "%s [%d, %d - %d, %d]", 
        turnType.c_str(), turn.hfrom, turn.vfrom, turn.hto, turn.vto);
    SetWindowTextA(TheGameHost.getHwnd(), windowTitle);
}


DWORD WINAPI AIThread(LPVOID lpParameter)
{
    ChessShapeList* pList =  TheGameBoard.getChessShapeList();
	TurnGenerator aTurnGen(pList);
    
    TurnInfo turn;
    bool bRes = aTurnGen.generateTurn(turn);
    /*
	if(bRes)
    {
        debug_outputTurn(turn);
    }else
    {
        MessageBox(TheGameHost.getHwnd(), _T("противник не смог сгенерировать ход"), 
            _T("ошибка!"), MB_OK|MB_ICONINFORMATION);
    }
	*/


    bRes = TheGameHost.makeTurn(turn.hfrom, turn.vfrom, turn.hto, turn.vto);
    if(!bRes)
    {
        ChessEventQueye& turnQueye = TheGameHost.getTurnEventQueye();
        while(!turnQueye.empty())
        {
            ChessEvent ce = turnQueye.front();
            MessageBoxA(TheGameHost.getHwnd(), ce._message.c_str(), 
            "ошибка!", MB_OK|MB_ICONINFORMATION);

            turnQueye.pop();
        }
    }

    return TRUE;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


#define MAX_LOADSTRING 100

LRESULT 
HomeChessApllication::wndProc(HWND hWnd, UINT mes, WPARAM wPar, LPARAM lPar)
{
    int wmId, wmEvent;
	PAINTSTRUCT ps;
    HDC dc;

    switch(mes)
    {
      case WM_CREATE:
          {
              break;
          }
      case WM_COMMAND:
          {
              wmId    = LOWORD(wPar);
              wmEvent = HIWORD(wPar);

              switch (wmId)
                  {
                      case IDM_ABOUT:
                          {
                              DialogBox(getHInstance(), 
                                  MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                              break;
                          }
                      case IDM_EXIT:
                          {
                              DestroyWindow(hWnd);
                              break;
                          }
                      case ID_NEWGAME:
                          {
                              newGame();
                              break;
                          }
                      case ID_LOADGAME:
                          {
                              loadGame();
                              break;
                          }
                      case ID_SAVEGAME:
                          {
                              saveGame();
                              break;
                          }
                      default:
                          {
                              return DefWindowProc(hWnd, mes, wPar, lPar);
                          }
                    }
              break;
          }
       case WM_ACTIVATE:
           {
               if(LOWORD(wPar)==WA_INACTIVE)
                   {
                       m_bActive = false;
                   }
               if(LOWORD(wPar)==WA_ACTIVE)
                   {
                       m_bActive = true;
                   }
               break;
           }
       case WM_DESTROY:
           {
               PostQuitMessage(0);
               break;
           }

      case WM_PAINT:
          {
              dc = BeginPaint(hWnd,&ps);
                            
              EndPaint(hWnd,&ps);
              ReleaseDC(hWnd,dc);
              break;
          }
      case WM_KEYDOWN:
          {
              if(wPar==VK_ESCAPE)
                  DestroyWindow(hWnd);
              break;
          }
      case WM_MOUSEMOVE:
      {
          CURCOORD x = (CURCOORD)LOWORD(lPar);
          CURCOORD y = (CURCOORD)HIWORD(lPar);
          CURSTATE curState = (CURSTATE)wPar;
          

          m_GUIManager.onCursorMove(x, y, curState);
          break;
      }
      case WM_LBUTTONDOWN:
      {
          CURCOORD x = (CURCOORD)LOWORD(lPar);
          CURCOORD y = (CURCOORD)HIWORD(lPar);
          
          m_GUIManager.onLButtonDown(x, y);
          break;
      }
      case WM_LBUTTONUP:
      {
          CURCOORD x = (CURCOORD)LOWORD(lPar);
          CURCOORD y = (CURCOORD)HIWORD(lPar);
          
          m_GUIManager.onLButtonUp(x, y);
      }
      default:
          {
              return DefWindowProc(hWnd,mes,wPar,lPar);
          }
 
    }
    return 0;
}


/***************************************************************************************/
//   переопределенные методы, определяющие свойства главного окна приложения
/***************************************************************************************/
LPCTSTR 
HomeChessApllication::getWindowTitle()
{
  static TCHAR szTitle[MAX_LOADSTRING];
  LoadString(getHInstance(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

  return szTitle;
}

void 
HomeChessApllication::getWindowPlacement(int& nX, int& nY, int& nWidth, int& nHeight)
{
  nX = CW_USEDEFAULT;
  nY = CW_USEDEFAULT;
  nWidth = 512;
  nHeight = 512;
}

LPCTSTR 
HomeChessApllication::getWindowMenu()
{
    return MAKEINTRESOURCE(IDC_HOMECHESS);
}

HICON 
HomeChessApllication::getWindowIcon()
{
    return LoadIcon(getHInstance(), MAKEINTRESOURCE(IDI_HOMECHESS));
}

DWORD 
HomeChessApllication::getWindowStyle()
{
    return WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_THICKFRAME);
}

/***************************************************************************************/
//   переопределенные методы, отвечающие за жизненный цикл игры
/***************************************************************************************/


void 
HomeChessApllication::gameInit()
{
    GameApplication::gameInit();

    loadResources();
    initLookupTables();
    initGameInterface();
    newGame();
}

void 
HomeChessApllication::gameShutdown()
{
    unloadGame();
    destroyGameInterface();
    destroyLookupTables();
    releaseResources();

    GameApplication::gameShutdown();
}

void 
HomeChessApllication::gameMain()
{
	GAMESTATE state = getGameState();

	if(state == GS_ANALIZ_PLAYER_TURN)
	{
		//анализ последствий хода игрока
		bool isPlayerWin = false;
        setGameState(GS_WAIT_AI_TURN);
        //setGameState(GS_WAIT_PLAYER_TURN);

        while(!m_TurnEventQueye.empty())
        {
            ChessEvent ce = m_TurnEventQueye.front();
            if(ce._type == CET_PAWN_IS_QUEEN)
            {
                ChessShape* pShape = (ChessShape*)ce._param;
                if(pShape != NULL)
                {
                    if(pShape->getColor() == CC_WHITE)
                    {
                        setGameState(GS_PLAYER_CHOOSE_SHAPE);
                    }
                }//if(pShape != NULL)
            }//if(ce._type == CET_PAWN_IS_QUEEN)

            if(ce._type == CET_BLACK_KING_KILLED)
            {
                isPlayerWin = true;
                setGameState(GS_PLAYER_WIN);
            }//if(ce._type == CET_BLACK_KING_KILLED)
            
            m_TurnEventQueye.pop();            
        }//while(!m_TurnEventQueye.empty())

        if(isPlayerWin)
        {
            setGameState(GS_PLAYER_WIN);
        }

        if(getGameState() == GS_WAIT_AI_TURN)
        {
            DWORD dwThreadID;
            HANDLE hThread = CreateThread(NULL, 0, AIThread, NULL, 0, &dwThreadID);
            CloseHandle(hThread);
        }
	}

	if(state == GS_ANALIZ_AI_TURN)
	{
		//анализ последствий хода противника
        bool isPlayerLose = false;
        setGameState(GS_WAIT_PLAYER_TURN);

        while(!m_TurnEventQueye.empty())
        {
            ChessEvent ce = m_TurnEventQueye.front();
            if(ce._type == CET_PAWN_IS_QUEEN)
            {
                ChessShape* pShape = (ChessShape*)ce._param;
                if(pShape != NULL)
                {
                    if(pShape->getColor() == CC_BLACK)
                    {
                        setGameState(GS_AI_CHOOSE_SHAPE);
                    }
                }//if(pShape != NULL)
            }//if(ce._type == CET_PAWN_IS_QUEEN)

            if(ce._type == CET_WHITE_KING_KILLED)
            {
                isPlayerLose = true;
                setGameState(GS_PLAYER_LOSE);
            }//if(ce._type == CET_WHITE_KING_KILLED)
            
            m_TurnEventQueye.pop();            
        }//while(!m_TurnEventQueye.empty())

        if(isPlayerLose)
        {
            setGameState(GS_PLAYER_LOSE);
        }
	}

    if(state == GS_PLAYER_WIN)
    {
        int iRes = MessageBox(getHwnd(), _T("Поздравляем, вы выиграли партию! Хотите сыграть еще?"), 
            _T("заглушка"), MB_YESNO|MB_ICONINFORMATION);
        if(iRes == IDYES)
        {
           newGame();
           setGameState(GS_WAIT_PLAYER_TURN);
        }else
        {
            DestroyWindow(getHwnd());
        }
    }

    if(state == GS_PLAYER_LOSE)
    {
        int iRes = MessageBox(getHwnd(), _T("Вы проиграли. Но вы можете попытаться сыграть снова. Хотите сыграть еще?"), 
            _T("заглушка"), MB_YESNO|MB_ICONINFORMATION);
        if(iRes == IDYES)
        {
           newGame();
           setGameState(GS_WAIT_PLAYER_TURN);
        }else
        {
            DestroyWindow(getHwnd());
        }
    }

    if(state == GS_PLAYER_CHOOSE_SHAPE)
    {
        MessageBox(NULL, _T("Ваша пешка дошла до конца доски. Вы можете заменить её любой другой фигурой...позже"), 
            _T("заглушка"), MB_OK|MB_ICONINFORMATION);
        setGameState(GS_WAIT_PLAYER_TURN);
    }
    


	frameMove();
	render();
}

HRESULT 
HomeChessApllication::drawScene(DEVICECONTEXT device)
{
    m_GUIManager.draw(device);

    return S_OK;
}

HRESULT 
HomeChessApllication::frameMove()
{
    for(size_t i = 0; i < m_Animators.size(); i++)
    {
        Animator* pAnim = m_Animators[i];

        if(pAnim == NULL) continue;

        DWORD dwPos = 0;
        pAnim->animate(dwPos);
        /*
        TCHAR coords[256];
        swprintf(coords, _T("position = %d"), dwPos);
        SetWindowText(getHwnd(), coords);
        */

        if(dwPos == Animator::k_nAnimationComplete)
        {
            delete pAnim;
            m_Animators[i] = NULL;
        }
    }

    return S_OK;
}

/*****************************************************************************************/
//   методы, вызываемые клиентами класса
/*****************************************************************************************/
void 
HomeChessApllication::addAnimation(Animator* pAnimator)
{
    for(size_t i = 0; i < m_Animators.size(); i++)
    {
        Animator* pAnim = m_Animators[i];

        if(pAnim == NULL)
        {
            m_Animators[i] = pAnimator;
            return;
        }
    }
    
    m_Animators.push_back(pAnimator);
}

SPRITEID 
HomeChessApllication::generateGameObjectID(ChessShape* pShape)
{
	if(pShape == NULL)
	{
		return kSpriteDefaultId;
	}

    return GAME_BOARD + pShape->getID();
}

bool 
HomeChessApllication::makeTurn(int hFrom, int vFrom, int hTo, int vTo)
{
	return m_GameBoard.setStep(hFrom, vFrom, hTo, vTo, m_TurnEventQueye);
}

/*****************************************************************************************/
// внутренние функции, реализующие функциональность приложения
/*****************************************************************************************/
bool 
HomeChessApllication::loadResources()
{
	if(m_pGraphManager == NULL)
    {
        return false;
    }
    
    DEVICECONTEXT device = m_pGraphManager->getDeviceContext();
    if(device == NULL)
    {
        return false;
    }
       
    
	HRESULT hr;
    for(int i = 0; i < 37; i++)
    {
        LPDIRECT3DTEXTURE9 pTex = NULL;
         
        if(m_TexIdentifiers[i] == HOMECHESS_BOARD)
        {
            continue;
        }
		        
        hr = D3DXCreateTextureFromResourceEx(device,
		                                     m_appHandle, 
		  								     MAKEINTRESOURCE(m_TexIdentifiers[i]),
											 D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
											 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
											 D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 
											 D3DCOLOR_XRGB(0,0,255), 
											 NULL, NULL, &pTex);
		if(FAILED(hr))
		{
			return false;
		}

        m_TextureTable.insert(make_pair(m_TexIdentifiers[i], pTex));        
    }

    LPDIRECT3DTEXTURE9 pGameBoardTexture = NULL;
    hr = D3DXCreateTextureFromResource(device, m_appHandle, MAKEINTRESOURCE(HOMECHESS_BOARD), &pGameBoardTexture);
	if(FAILED(hr))
	{
		return false;
	}
        
    m_TextureTable.insert(make_pair(HOMECHESS_BOARD, pGameBoardTexture));
    
	return true;
}

bool 
HomeChessApllication::releaseResources()
{
	for(std::map<int, LPDIRECT3DTEXTURE9>::iterator it = m_TextureTable.begin();
        it != m_TextureTable.end(); ++it)
    {
        LPDIRECT3DTEXTURE9 pTexture = it->second;
        if(pTexture != NULL)
        {
            pTexture->Release();
        }
    }
    m_TextureTable.clear();

    for(size_t i = 0; i < m_Animators.size(); i++)
    {
        Animator* pAnim = m_Animators[i];

        if(pAnim != NULL)
        {
            delete pAnim;
            m_Animators[i] = NULL;
        }        
    }
    m_Animators.clear();

	return true;
}

bool 
HomeChessApllication::initLookupTables()
{
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_PESHKA)] = m_TextureTable[HOMECHESS_WHITE_PAWN];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_KON)]    = m_TextureTable[HOMECHESS_WHITE_KNIGHT];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_SLON)]   = m_TextureTable[HOMECHESS_WHITE_BISHOP];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_LADYA)]  = m_TextureTable[HOMECHESS_WHITE_ROOK];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_FERZ)]   = m_TextureTable[HOMECHESS_WHITE_QUEEN];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_KOROL)]  = m_TextureTable[HOMECHESS_WHITE_KING];

    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_PESHKA)] = m_TextureTable[HOMECHESS_BLACK_PAWN];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_KON)]    = m_TextureTable[HOMECHESS_BLACK_KNIGHT];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_SLON)]   = m_TextureTable[HOMECHESS_BLACK_BISHOP];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_LADYA)]  = m_TextureTable[HOMECHESS_BLACK_ROOK];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_FERZ)]   = m_TextureTable[HOMECHESS_BLACK_QUEEN];
    m_NeitralSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_KOROL)]  = m_TextureTable[HOMECHESS_BLACK_KING];

    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_PESHKA)] = m_TextureTable[HOMECHESS_HIGHLIGTH_WHITE_PAWN];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_KON)]    = m_TextureTable[HOMECHESS_HIGHLIGTH_WHITE_KNIGHT];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_SLON)]   = m_TextureTable[HOMECHESS_HIGHLIGTH_WHITE_BISHOP];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_LADYA)]  = m_TextureTable[HOMECHESS_HIGHLIGTH_WHITE_ROOK];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_FERZ)]   = m_TextureTable[HOMECHESS_HIGHLIGTH_WHITE_QUEEN];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_KOROL)]  = m_TextureTable[HOMECHESS_HIGHLIGTH_WHITE_KING];

    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_PESHKA)] = m_TextureTable[HOMECHESS_HIGHLIGTH_BLACK_PAWN];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_KON)]    = m_TextureTable[HOMECHESS_HIGHLIGTH_BLACK_KNIGHT];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_SLON)]   = m_TextureTable[HOMECHESS_HIGHLIGTH_BLACK_BISHOP];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_LADYA)]  = m_TextureTable[HOMECHESS_HIGHLIGTH_BLACK_ROOK];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_FERZ)]   = m_TextureTable[HOMECHESS_HIGHLIGTH_BLACK_QUEEN];
    m_HoverSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_KOROL)]  = m_TextureTable[HOMECHESS_HIGHLIGTH_BLACK_KING];

    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_PESHKA)] = m_TextureTable[HOMECHESS_DROPPED_WHITE_PAWN];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_KON)]    = m_TextureTable[HOMECHESS_DROPPED_WHITE_KNIGHT];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_SLON)]   = m_TextureTable[HOMECHESS_DROPPED_WHITE_BISHOP];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_LADYA)]  = m_TextureTable[HOMECHESS_DROPPED_WHITE_ROOK];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_FERZ)]   = m_TextureTable[HOMECHESS_DROPPED_WHITE_QUEEN];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_WHITE, CT_KOROL)]  = m_TextureTable[HOMECHESS_DROPPED_WHITE_KING];

    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_PESHKA)] = m_TextureTable[HOMECHESS_DROPPED_BLACK_PAWN];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_KON)]    = m_TextureTable[HOMECHESS_DROPPED_BLACK_KNIGHT];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_SLON)]   = m_TextureTable[HOMECHESS_DROPPED_BLACK_BISHOP];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_LADYA)]  = m_TextureTable[HOMECHESS_DROPPED_BLACK_ROOK];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_FERZ)]   = m_TextureTable[HOMECHESS_DROPPED_BLACK_QUEEN];
    m_DroppedSkins[GENERATE_GAMEOBJECT_ID(CC_BLACK, CT_KOROL)]  = m_TextureTable[HOMECHESS_DROPPED_BLACK_KING];

	return true;
}

bool 
HomeChessApllication::destroyLookupTables()
{
    m_NeitralSkins.clear();
    m_HoverSkins.clear();
    m_DroppedSkins.clear();

	return true;
}

bool 
HomeChessApllication::initGameInterface()
{
    setViewport();

    Sprite* pRoot = m_GUIManager.getElement(GIU_ROOT_ELEMENT);
    CompositeSprite* pGameBoard = new CompositeSprite(GAME_BOARD, pRoot);
    
    //pGameBoard->setID(GAME_BOARD);
    pGameBoard->setCoords(0.0f, 0.0f);
    pGameBoard->setSize(100.0f, 100.0f);
    
    LPDIRECT3DTEXTURE9 pTexture = m_TextureTable[HOMECHESS_BOARD];
    for(SpriteState i = SS_INVISIBLE; i < SS_TOPVAL; i++)
    {
        pGameBoard->setStateSkin(i, pTexture);
    }
    pGameBoard->setState(SS_NEITRAL);

    m_GUIManager.addElement(GIU_ROOT_ELEMENT, pGameBoard);

	return true;
}

bool 
HomeChessApllication::destroyGameInterface()
{
    m_GUIManager.removeElement(GAME_BOARD);

	return true;
}

bool 
HomeChessApllication::initGameObjects()
{
    Sprite* pGameBoard = m_GUIManager.getElement(GAME_BOARD);
    if(pGameBoard == NULL)
    {
        return false;
    }

    ChessShapeList* pList = m_GameBoard.getChessShapeList();
    
    for(ChessShapeList::iterator it = pList->begin();
        it != pList->end(); ++it)
    {
        ChessShape* pShape = *it;
        
        BaseGameObject* pGameObject = NULL;
        if(pShape->getColor() == CC_WHITE)
        {
            pGameObject = new PlayerChessShape(pShape, pGameBoard);
        }else
        {
            pGameObject = new AIChessShape(pShape, pGameBoard);
        }

        SPRITEID key = GENERATE_GAMEOBJECT_ID(pShape->getColor(), pShape->getType());
        
        SPRITESKIN neitralSkin = m_NeitralSkins[key];
        SPRITESKIN hoverSkin   = m_HoverSkins[key];
        SPRITESKIN droppedSkin = m_DroppedSkins[key];

        pGameObject->setStateSkin(SS_INVISIBLE, neitralSkin);
        pGameObject->setStateSkin(SS_NEITRAL, neitralSkin);
        pGameObject->setStateSkin(SS_HOVER, hoverSkin);
        pGameObject->setStateSkin(SS_DROPPED, droppedSkin);
        pGameObject->setStateSkin(SS_FOCUSED, neitralSkin);        
    }

	return true;
}

bool 
HomeChessApllication::destroyGameObjects()
{
    ChessShapeList* pList = m_GameBoard.getChessShapeList();
    
    for(ChessShapeList::iterator it = pList->begin();
        it != pList->end(); ++it)
    {
        ChessShape* pShape = *it;
        SPRITEID id = generateGameObjectID(pShape);

        m_GUIManager.removeElement(id);
    }

	return true;
}

bool 
HomeChessApllication::newGame()
{
    unloadGame();
    m_GameBoard.init();
    initGameObjects();
    
    setGameState(GS_WAIT_PLAYER_TURN);

	return true;
}

bool 
HomeChessApllication::loadGame()
{
	return false;
}

bool 
HomeChessApllication::saveGame()
{
	return false;
}

bool 
HomeChessApllication::unloadGame()
{
    destroyGameObjects();
    
    m_GameBoard.destroy();
	
	while(!m_TurnEventQueye.empty())
	{
		m_TurnEventQueye.pop();
	}

	return false;
}

bool 
HomeChessApllication::setViewport()
{
    int width, height;
    RECT rect;
    
	GetClientRect(getHwnd(), &rect);
    
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
    
    m_GUIManager.setArrea((CURCOORD)width, (CURCOORD)height);

    return true;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return TheGameHost.wndProc(hWnd, message, wParam, lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	HomeChessApllication app(hInstance, true, WndProc);
    if(!app.create())
	{
		return FALSE;
	}
    
	return app.run();
}