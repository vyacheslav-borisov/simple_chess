#pragma once


typedef int CURCOORD;
typedef int CURSTATE;
typedef int CURWHEEL;

const CURSTATE CS_LBUTTONDOWN = 0x001;
const CURSTATE CS_RBUTTONDOWN = 0x002;
const CURSTATE CS_MBUTTONDOWN = 0x010;
const CURSTATE CS_X1BUTTONDOWN = 0x0020;
const CURSTATE CS_X2BUTTONDOWN = 0x0040;
const CURSTATE CS_SHIFTDOWN = 0x0004;
const CURSTATE CS_CONTROLDOWN = 0x0008;


class GUIManager;

class EventHandler
{
public:
	EventHandler(): mGUI(NULL) {};
	virtual ~EventHandler() {};
	void setGUI(GUIManager* pGUI) { mGUI = pGUI; }; 
	GUIManager* getGUI() { return mGUI; };

//наведение курсора на элемент (onBlur)
	virtual void onBlur(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state); 
//потеря курсора элементом (onHower)
	virtual void onHower(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state);
//захват фокуса элементом (клик по элементу)(onSetFocus)
	virtual void onSetFocus(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state, Sprite* oldFocus);
//потеря фокуса элементом (клик по другому элементу) (onKillFocus)
	virtual void onKillFocus(Sprite* sender,
		CURCOORD x, CURCOORD y, CURSTATE state, Sprite* newFocus);

//нажатие ЛКМ в пределах элемента (oLButtonDowm)
	virtual void onLButtonDown(Sprite* sender, CURCOORD x, CURCOORD y);
//отпуск ЛКМ в пределах элемента (onLButtonUp)
	virtual void onLButtonUp(Sprite* sender, CURCOORD x, CURCOORD y);
//одинарный клик ЛКМ по элементу (onLButtonClick)
	virtual void onLButtonClick(Sprite* sender, CURCOORD x, CURCOORD y);
//двойной клик по ЛКМ элементу (onLButtonDbClick)
	virtual void onLButtonDbClick(Sprite* sender, CURCOORD x, CURCOORD y);

//нажатие ПКМ в пределах элемента (oLButtonDowm)
	virtual void onRButtonDown(Sprite* sender, CURCOORD x, CURCOORD y);
//отпуск ПКМ в пределах элемента (onLButtonUp)
	virtual void onRButtonUp(Sprite* sender, CURCOORD x, CURCOORD y);
//одинарный клик ПКМ по элементу (onLButtonClick)
	virtual void onRButtonClick(Sprite* sender, CURCOORD x, CURCOORD y);
//двойной клик по ПКМ элементу (onLButtonDbClick)
	virtual void onRButtonDbClick(Sprite* sender, CURCOORD x, CURCOORD y);
//перемещение курсора мыши
	virtual void onCursorMove(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state);
//прокрутка колеса мыши
	virtual void onCursorWheel(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state, CURWHEEL delta);
private:
	GUIManager* mGUI;
};
