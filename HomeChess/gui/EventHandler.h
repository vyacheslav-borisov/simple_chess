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

//��������� ������� �� ������� (onBlur)
	virtual void onBlur(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state); 
//������ ������� ��������� (onHower)
	virtual void onHower(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state);
//������ ������ ��������� (���� �� ��������)(onSetFocus)
	virtual void onSetFocus(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state, Sprite* oldFocus);
//������ ������ ��������� (���� �� ������� ��������) (onKillFocus)
	virtual void onKillFocus(Sprite* sender,
		CURCOORD x, CURCOORD y, CURSTATE state, Sprite* newFocus);

//������� ��� � �������� �������� (oLButtonDowm)
	virtual void onLButtonDown(Sprite* sender, CURCOORD x, CURCOORD y);
//������ ��� � �������� �������� (onLButtonUp)
	virtual void onLButtonUp(Sprite* sender, CURCOORD x, CURCOORD y);
//��������� ���� ��� �� �������� (onLButtonClick)
	virtual void onLButtonClick(Sprite* sender, CURCOORD x, CURCOORD y);
//������� ���� �� ��� �������� (onLButtonDbClick)
	virtual void onLButtonDbClick(Sprite* sender, CURCOORD x, CURCOORD y);

//������� ��� � �������� �������� (oLButtonDowm)
	virtual void onRButtonDown(Sprite* sender, CURCOORD x, CURCOORD y);
//������ ��� � �������� �������� (onLButtonUp)
	virtual void onRButtonUp(Sprite* sender, CURCOORD x, CURCOORD y);
//��������� ���� ��� �� �������� (onLButtonClick)
	virtual void onRButtonClick(Sprite* sender, CURCOORD x, CURCOORD y);
//������� ���� �� ��� �������� (onLButtonDbClick)
	virtual void onRButtonDbClick(Sprite* sender, CURCOORD x, CURCOORD y);
//����������� ������� ����
	virtual void onCursorMove(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state);
//��������� ������ ����
	virtual void onCursorWheel(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state, CURWHEEL delta);
private:
	GUIManager* mGUI;
};
