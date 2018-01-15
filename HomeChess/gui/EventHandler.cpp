#include "stdafx.h"
#include "../structs/singleton.h"
#include "GUICommon.h"

using namespace std;

//��������� ������� �� ������� (onBlur)
void 
EventHandler::onBlur(Sprite* sender,
					 CURCOORD x, 
					 CURCOORD y, 
					 CURSTATE state)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onBlur(pParent, x, y, state);
} 

//������ ������� ��������� (onHower)
void 
EventHandler::onHower(Sprite* sender,
					  CURCOORD x,
					  CURCOORD y,
					  CURSTATE state)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onHower(pParent, x, y, state);
}

//������ ������ ��������� (���� �� ��������)(onSetFocus)
void 
EventHandler::onSetFocus(Sprite* sender,
						 CURCOORD x, 
						 CURCOORD y, 
						 CURSTATE state, 
						 Sprite* oldFocus)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onSetFocus(pParent, x, y, state, oldFocus);
}

//������ ������ ��������� (���� �� ������� ��������) (onKillFocus)
void 
EventHandler::onKillFocus(Sprite* sender,
						  CURCOORD x, 
						  CURCOORD y, 
						  CURSTATE state, 
						  Sprite* newFocus)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onKillFocus(pParent, x, y, state, newFocus);
}

//������� ��� � �������� �������� (oLButtonDowm)
void 
EventHandler::onLButtonDown(Sprite* sender, 
							CURCOORD x, 
							CURCOORD y)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onLButtonDown(pParent, x, y);
}

//������ ��� � �������� �������� (onLButtonUp)
void 
EventHandler::onLButtonUp(Sprite* sender, 
						  CURCOORD x, 
						  CURCOORD y)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onLButtonUp(pParent, x, y);
}

//��������� ���� ��� �� �������� (onLButtonClick)
void 
EventHandler::onLButtonClick(Sprite* sender, 
							 CURCOORD x, 
							 CURCOORD y)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onLButtonClick(pParent, x, y);
}

//������� ���� �� ��� �������� (onLButtonDbClick)
void 
EventHandler::onLButtonDbClick(Sprite* sender, 
							   CURCOORD x, 
							   CURCOORD y)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onLButtonDbClick(pParent, x, y);
}

//������� ��� � �������� �������� (oLButtonDowm)
void 
EventHandler::onRButtonDown(Sprite* sender, 
							CURCOORD x, 
							CURCOORD y)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onRButtonDown(pParent, x, y);
}

//������ ��� � �������� �������� (onLButtonUp)
void 
EventHandler::onRButtonUp(Sprite* sender, 
						  CURCOORD x, 
						  CURCOORD y)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onRButtonUp(pParent, x, y);
}

//��������� ���� ��� �� �������� (onLButtonClick)
void 
EventHandler::onRButtonClick(Sprite* sender, 
							 CURCOORD x, 
							 CURCOORD y)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onRButtonClick(pParent, x, y);
}

//������� ���� �� ��� �������� (onLButtonDbClick)
void 
EventHandler::onRButtonDbClick(Sprite* sender, 
							   CURCOORD x, 
							   CURCOORD y)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onRButtonDbClick(pParent, x, y);
}

//����������� ������� ����
void 
EventHandler::onCursorMove(Sprite* sender,
						   CURCOORD x, 
						   CURCOORD y, 
						   CURSTATE state)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onCursorMove(pParent, x, y, state);
}

//��������� ������ ����
void 
EventHandler::onCursorWheel(Sprite* sender,
							CURCOORD x, 
							CURCOORD y, 
							CURSTATE state, 
							CURWHEEL delta)
{
 //�������� ��������� �� ������ ���
 GUIManager* gui = getGUI();
 //�������� ��������� �� ������������ �������
 Sprite* pParent = sender->getParent();
 //���� �������� �� ������ �������
 if(pParent==NULL)
	 return;
 //�������� �� ��������
 SPRITEID id = pParent->getID();
 //����������� ��� �� ������ ����������� ��� ��������
 EventHandler* handler = gui->getHandler(id);
 //���� �� ������ �������
 if(handler==NULL)
	 return;
 //������� �������������� ����� �����������
 handler->onCursorWheel(pParent, x, y, state, delta); 
}


