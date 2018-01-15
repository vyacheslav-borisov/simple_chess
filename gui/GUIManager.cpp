#include "stdafx.h"
#include "../structs/singleton.h"
#include "GUICommon.h"

using namespace std;


//����������� ������, ����������� ������ �� �������� ����������
GUIManager::GUIManager():
cSingleton(*this), mBlured(NULL), mFocused(NULL)
{
 mElements[GIU_ROOT_ELEMENT] = (Sprite*)&mRoot;
 mHandlers[GIU_ROOT_ELEMENT] = new EventHandler();
 mHandlers[GIU_ROOT_ELEMENT]->setGUI(this);
}

//���������� ������
GUIManager::~GUIManager()
{
 destroyAll();
}

void 
GUIManager::destroyAll()
{
 //������� ����������� �������
 for(map<SPRITEID, EventHandler*>::iterator it = mHandlers.begin();
		it != mHandlers.end(); ++it)
 {
	 EventHandler* handler = it->second;
	 delete handler;
 }
 

 //�������� ����� ������������
 mHandlers.clear();
 //�������� ����� ���������
 mElements.clear();
 
}
/*
 ����� ��� ��������� ������� ��������� ����������,
 ����������� ��� ������ ��������� �������� ����
*/
void 
GUIManager::setArrea(CURCOORD width, CURCOORD height)
{
 D3DXVECTOR3 eye((FLOAT)(width / 2), (FLOAT)(height / 2), -1.0f);
 D3DXVECTOR3 at((FLOAT)(width / 2), (FLOAT)(height / 2), 0.0f);
 D3DXVECTOR3 up(0.0f, -1.0f, 0.0f);

 D3DXMatrixLookAtRH(&mView, &eye, &at, &up);
 D3DXMatrixOrthoRH(&mProject, (FLOAT)width, (FLOAT)height, 0.0f, 10.0f);

 mRoot.setSize((SCOORD)width, (SCOORD)height);
}



//����� ��� ���������� ������ �������� � ���������
bool 
GUIManager::addElement(SPRITEID parent, Sprite* elem)
{
 //��������� ���� �� �������������� ������������ ���� 
 map<SPRITEID, Sprite*>::iterator parent_it = mElements.find(parent);
 //���� ��� - �������
 if(parent_it == mElements.end())
  return false;
 //��������� ��� �� � ������� ��������� �������� � �� elem
 map<SPRITEID, Sprite*>::iterator elem_it = mElements.find(elem->getID());
 if(elem_it == mElements.end())
 {
  //���� ���  ��������� ������� � �������
  SPRITEID key = elem->getID();
  mElements[key] = elem;

  setHandler(key, new EventHandler());
 }else
 {
  //���� ���� ������ ������� �� ������ �������� ��� ��������
  Sprite* pParentSprite = elem->getParent();
  pParentSprite->removeChild(elem);
 }
 //������������ ������� � ��������������� ����
 mElements[parent]->addChild(elem);
 return true;
}

//����� ��� �������� �������� ���������� �� ��� ��
bool 
GUIManager::removeElement(SPRITEID elem)
{
 map<SPRITEID, Sprite*>::iterator elem_it = mElements.find(elem);
 if(elem_it != mElements.end())
 {
	 Sprite* pElement = mElements[elem];
	 Sprite* pParent = pElement->getParent();
	 //������ ��� �� ������������� ��������
	 pParent->removeChild(pElement);
	 //� ����� �� ������� ���������
	 mElements.erase(elem_it);
     //������� ������ �� ������� ����������� �������
     //��� �������, ����������� � ������, ���� ���
     //��� � ������ ������ ��������� �� ��������� ������
     //����� �������� �� ������ ������ ��������� �� ������ ��������
     if(mBlured == pElement)
     {
         mBlured = NULL;
     }
     if(mFocused == pElement)
     {
         mFocused = NULL;
     }
	 //�� ������ ��� ������
	 delete pElement;
	 //������� ��������� ������� ��� ��������
	 map<SPRITEID, EventHandler*>::iterator handle_it = mHandlers.find(elem);
	 if(handle_it != mHandlers.end())
	 {
		 EventHandler* handler = handle_it->second;
         if(handler != NULL)
         {
             delete handler;
         }
		 mHandlers.erase(handle_it);
	 }
 }
 return false;
}

//����� ��� ��������� �������� ���������� �� ��� ��
Sprite* 
GUIManager::getElement(SPRITEID elem)
{
 map<SPRITEID, Sprite*>::iterator elem_it = mElements.find(elem);
 if(elem_it != mElements.end())
	 return mElements[elem];
 return NULL;
}

//����� ��� ��������� ������������ ����������� �������
bool 
GUIManager::setHandler(SPRITEID elem, EventHandler* handler)
{
 map<SPRITEID, EventHandler*>::iterator handle_it = mHandlers.find(elem);
 if(handle_it != mHandlers.end())
 {
	 EventHandler* oldHandler = handle_it->second;
	 delete oldHandler;	 
 }

 mHandlers[elem] = handler;
 mHandlers[elem]->setGUI(this);
 
 return true;
}

//����� ��� ��������� ����������� ������� �� �� ��������
EventHandler* 
GUIManager::getHandler(SPRITEID elem)
{
 map<SPRITEID, EventHandler*>::iterator handle_it = mHandlers.find(elem);
 if(handle_it != mHandlers.end())
	 return handle_it->second;
 else
	 return NULL;
}

void 
GUIManager::removeHandler(SPRITEID elem)
{
    map<SPRITEID, EventHandler*>::iterator handle_it = mHandlers.find(elem);
    if(handle_it != mHandlers.end())
    {
        mHandlers.erase(handle_it);
    }
}
 
 //������ ������� - ������������� �������
 
//������� ��� � �������� �������� (oLButtonDowm)
void 
GUIManager::onLButtonDown(CURCOORD x, CURCOORD y)
{
 //�������� ������ �� �������, � �������� �������� ��������
 Sprite* newFocus =	mRoot.findSprite(x, y);
 //���� ������ �������� ���, ������ ���������� ������ - �������
 if(NULL == newFocus)
	 return;
 //�������� ���������� ������� �������������� ��������
 EventHandler* NFhandler = getHandler(newFocus->getID());
 //���� ��� ������, ������ ���-�� �� ���
 if(NULL == NFhandler)
	 return;
 //���� ����� ������� ����� �������
 if(mFocused != newFocus)
 {
  //������� ������� "������ ������" ��� ������� �������, ���� ������� ����
  if(NULL != mFocused)
  {
	  EventHandler* OFhandler = getHandler(mFocused->getID());
	  if(NULL!=OFhandler)
		  OFhandler->onKillFocus(mFocused, x, y, CS_LBUTTONDOWN, newFocus);
  }//if(NULL != mFocused)
  //������� ������� "��������� ������" ��� ������ ��������
  NFhandler->onSetFocus(newFocus, x, y, CS_LBUTTONDOWN, mFocused);
  //�������� ������ �� ������� � ����
  mFocused = newFocus;
 }
 //������� ������� "������� ����� ������ ����" ��� ��������
 NFhandler->onLButtonDown(newFocus, x, y);
}

//������ ��� � �������� �������� (onLButtonUp)
void 
GUIManager::onLButtonUp(CURCOORD x, CURCOORD y)
{
 //������� ������ �� ������� � �������� �������� ���������� ������
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //������� ���������� ������� ��� ����
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //���� ���������� ����, �� ������� �������
 handler->onLButtonUp(control, x, y);
}

//��������� ���� ��� �� �������� (onLButtonClick)
void 
GUIManager::onLButtonClick(CURCOORD x, CURCOORD y)
{
 //������� ������ �� ������� � �������� �������� ���������� ������
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //������� ���������� ������� ��� ����
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //���� ���������� ����, �� ������� �������
 handler->onLButtonClick(control, x, y);
}

//������� ���� �� ��� �������� (onLButtonDbClick)
void 
GUIManager::onLButtonDbClick(CURCOORD x, CURCOORD y)
{
 //������� ������ �� ������� � �������� �������� ���������� ������
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //������� ���������� ������� ��� ����
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //���� ���������� ����, �� ������� �������
 handler->onLButtonDbClick(control, x, y);
}

//������� ��� � �������� �������� (oLButtonDowm)
//����� �������, ��� ������� ������ ������ ���� �� ����� �
//�������� ������ ��������
void 
GUIManager::onRButtonDown(CURCOORD x, CURCOORD y)
{
 //������� ������ �� ������� � �������� �������� ���������� ������
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //������� ���������� ������� ��� ����
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //���� ���������� ����, �� ������� �������
 handler->onRButtonDown(control, x, y);
}

//������ ��� � �������� �������� (onLButtonUp)
void 
GUIManager::onRButtonUp(CURCOORD x, CURCOORD y)
{
 //������� ������ �� ������� � �������� �������� ���������� ������
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //������� ���������� ������� ��� ����
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //���� ���������� ����, �� ������� �������
 handler->onRButtonUp(control, x, y);
}

//��������� ���� ��� �� �������� (onLButtonClick)
void 
GUIManager::onRButtonClick(CURCOORD x, CURCOORD y)
{
 //������� ������ �� ������� � �������� �������� ���������� ������
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //������� ���������� ������� ��� ����
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //���� ���������� ����, �� ������� �������
 handler->onRButtonClick(control, x, y);
}

//������� ���� �� ��� �������� (onLButtonDbClick)
void 
GUIManager::onRButtonDbClick(CURCOORD x, CURCOORD y)
{
 //������� ������ �� ������� � �������� �������� ���������� ������
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //������� ���������� ������� ��� ����
 EventHandler* handler = getHandler(control->getID());
 if(NULL == handler)
	 return;
 //���� ���������� ����, �� ������� �������
 handler->onRButtonDbClick(control, x, y);
}

//����������� ������� ����
void 
GUIManager::onCursorMove(CURCOORD x, CURCOORD y, CURSTATE state)
{
 //�������� ������� � �������� �������� ���������� ������
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 //������� ���������� �������, �������������� ��������
 EventHandler* curHandler = getHandler(control->getID());
 if(NULL == curHandler)
	 return;
 //���� ������ ���������� ��� ������ ���������
 if(control != mBlured)
 {
  //������� ������� "������ �������" ��� ����������� ��������
  if(NULL != mBlured)
  {
	  EventHandler* oldHandler = getHandler(mBlured->getID());
	  if(NULL != oldHandler)
		  oldHandler->onHower(mBlured, x, y, state);
  }
  //������� ������� "��������� �������" ��� ������ ��������
  curHandler->onBlur(control, x, y, state);
  mBlured = control;
 }
 //������� ������� "����������� �������" ��� ��������, ��� ������� ���������� ������
 curHandler->onCursorMove(control, x, y, state);
 //������� ������� "����������� �������" ��� �������� ������������ � ������
 if((NULL != mFocused)&&(mFocused != control))
 {
	 EventHandler* focHandler = getHandler(mFocused->getID());
	 if(NULL != focHandler)
		 focHandler->onCursorMove(mFocused, x, y, state);
 }
}

//��������� ������ ����
//����������� ��� �������� ��� ������� ���������� ������
//� ��� �������� ������������ � ������
void 
GUIManager::onCursorWheel(CURCOORD x, CURCOORD y, CURSTATE state, CURWHEEL delta)
{
 Sprite* control = mRoot.findSprite((SCOORD)x, (SCOORD)y);
 if(NULL == control)
	 return;
 EventHandler* curHandler = getHandler(control->getID());
 if(NULL == curHandler)
	 return;

 curHandler->onCursorWheel(control, x, y, state, delta);
 if((NULL != mFocused)&&(mFocused != control))
 {
	 EventHandler* focHandler = getHandler(mFocused->getID());
	 if(NULL != focHandler)
		 focHandler->onCursorWheel(mFocused, x, y, state, delta);
 }
}

//����� ��� ��������� ����������
void 
GUIManager::draw(DEVICECONTEXT& device)
{
 /*
 if(NULL == mDevice)
	 return;
 */
 //�������� ���������� ������ �� �����-������
 device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
 device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
 device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
 //�������� ������ � �������������
 device->SetRenderState(D3DRS_LIGHTING, FALSE);
 //��������� ���������
 device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
 //������������� �������
 device->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView);
 device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProject);
 //������������� ������ ������
 device->SetFVF(D3DFVF_XYZ|D3DFVF_TEX1);
 //������...
 mRoot.draw(device);
}
