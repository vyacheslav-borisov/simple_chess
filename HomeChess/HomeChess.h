#pragma once

#include "resource.h"

const SPRITEID GAME_BOARD = GIU_ROOT_ELEMENT + 1;

//состояния игры
typedef unsigned int GAMESTATE;
const GAMESTATE GS_WAIT_PLAYER_TURN = 1; //ожидание ход игрока
const GAMESTATE GS_ANALIZ_PLAYER_TURN = 2; //анализ хода игрока
const GAMESTATE GS_WAIT_AI_TURN = 3; //ожидание хода противника
const GAMESTATE GS_ANALIZ_AI_TURN = 4; //анализ хода противника
const GAMESTATE GS_PLAYER_WIN = 5; //игрок победил
const GAMESTATE GS_PLAYER_LOSE = 6; //игрок проиграл
const GAMESTATE GS_PLAYER_CHOOSE_SHAPE = 7; //игрок выбирает замену пешке
const GAMESTATE GS_AI_CHOOSE_SHAPE = 8; //противник выбирает замену пешке

//остальные состояния добавим позже при необходимости

class HomeChessApllication: 
	public GameApplication, public cSingleton<HomeChessApllication>
{
public:
	//конструктор/деструктор
	HomeChessApllication(HINSTANCE appHandle, bool bWindowed, WNDPROC lpProc):
	  GameApplication(bWindowed, lpProc), cSingleton(*this), m_appHandle(appHandle) {};
public:
	virtual LRESULT wndProc(HWND hWnd, UINT mes, WPARAM wPar, LPARAM lPar);

public:
	//методы, вызываемые клиентами класса
    void addAnimation(Animator* pAnimator);
	void setGameState(GAMESTATE gs) { m_currentGameState = gs; };
	GAMESTATE getGameState() { return m_currentGameState; };
	bool makeTurn(int hFrom, int vFrom, int hTo, int vTo);
   	static SPRITEID generateGameObjectID(ChessShape* pShape);
	ChessEventQueye& getTurnEventQueye() { return m_TurnEventQueye; };
protected:
	//переопределенные методы, отвечающие за жизненный цикл игры
    virtual void gameInit();
	virtual void gameMain();
    virtual void gameShutdown();
    virtual HRESULT drawScene(DEVICECONTEXT device);
    virtual HRESULT frameMove();
protected:
	//переопределенные методы, определяющие свойства главного окна приложения
	virtual LPCTSTR getWindowTitle();
    virtual void getWindowPlacement(int& nX, int& nY, int& nWidth, int& nHeight);
    virtual LPCTSTR getWindowMenu();
    virtual HICON getWindowIcon();
    virtual DWORD getWindowStyle();
private:
	//обьекты-менеджеры
    GUIManager m_GUIManager;
    ChessBoard m_GameBoard;
	ChessEventQueye m_TurnEventQueye;

	HINSTANCE m_appHandle;

	//ресурсы
    static const int m_TexIdentifiers[37];
    std::map<int, LPDIRECT3DTEXTURE9> m_TextureTable;
    std::vector<Animator*> m_Animators;

    std::map<SPRITEID, LPDIRECT3DTEXTURE9> m_NeitralSkins;
    std::map<SPRITEID, LPDIRECT3DTEXTURE9> m_HoverSkins;
    std::map<SPRITEID, LPDIRECT3DTEXTURE9> m_DroppedSkins;
private:
	//внтуриигровые параметры
	GAMESTATE m_currentGameState;
private:
	//внутренние функции, реализующие функциональность приложения
	bool loadResources();
	bool releaseResources();
	bool initLookupTables();
	bool destroyLookupTables();
	bool initGameInterface();
	bool destroyGameInterface();
    bool initGameObjects();
	bool destroyGameObjects();
    bool setViewport();

	bool newGame();
	bool loadGame();
	bool saveGame();
	bool unloadGame();
};


#define TheGameHost HomeChessApllication::getInstance()