
#include <stdafx.h>

#include "structs/singleton.h"
#include "graph.h"
#include "game_application.h"
#include "animation.h"
#include "gui/GUICommon.h"
#include "ai/ChessCommon.h"

#include "hcgraph.h"
#include "HomeChess.h"

#include "GameObject.h"

/*****************************************************************/
//  ������� ������� ������
/*****************************************************************/
BaseGameObject::BaseGameObject(ChessShape* pShape, Sprite* pParent)
{
    setParent(pParent);
    
    SCOORD scale = (SCOORD)(100.0f / 8.0f);
    //������ ������� ������� (1/8 �� ����� � ������ �� ��������
	//������������� ��������
    setSize(scale, scale);
	//������ ��������� �� ������ ��������� �������� ������ �� �����
    //(�������������� ���������)
	SCOORD x = scale * pShape->getHCell();
	SCOORD y = scale * pShape->getVCell();

	setCoords(x, y);
    //������ ��������� �� ������ ��������� �������� ������
	if(pShape->isActive())
	{
		setState(SS_NEITRAL);
	}else
	{
		setState(SS_INVISIBLE);
	}
    //������������� �� �� ������ �������� ����� � ���� �������� ������
	SPRITEID id = TheGameHost.generateGameObjectID(pShape);
	setID(id);
    //���������������� ������� ������ ��� ������ � ��������� GUI
	TheGUI.addElement(pParent->getID(), static_cast<Sprite*>(this));
    //���������������� ������� ������ ��� ���������� ������� � ��������� GUI
	TheGUI.setHandler(getID(), static_cast<EventHandler*>(this));
}

BaseGameObject::~BaseGameObject()
{
   //�������� ����� ��� �������� (��� ������ �������� delete)
   //����������� �������, ����������� ����������� � ��������� EventHandler ��������� 
   //�� ��������� ������� ������
   //��� ����� ��� �������������� ���������� �������� �� ������ ������� ������� ������
   //����� ������ �� EventHandler, ��������� ����� ���� ������ ����� ������ ������ �����
   //������ �� Sprite. ��������� ������ �������������� �������������(((
   TheGUI.removeHandler(getID()); 
}

//��������� ������� �� ������� (onBlur)
void
BaseGameObject::onBlur(Sprite* sender, CURCOORD x, CURCOORD y, CURSTATE state)
{
  //���� ������� ���������� � ����������� ���������
  if(sender->getState() == SS_NEITRAL)
  {
     //��������� ��� � �����������
     sender->setState(SS_HOVER);
  }
  EventHandler::onBlur(sender, x, y, state);
  
}

//������ ������� ��������� (onHower)
void 
BaseGameObject::onHower(Sprite* sender, CURCOORD x, CURCOORD y, CURSTATE state)
{
   //���� ������� ���������� � ����������� ���������
   if(sender->getState() == SS_HOVER)
   {
     //��������� � ����������
     sender->setState(SS_NEITRAL);
   }
   EventHandler::onHower(sender, x, y, state); 
}

/*************************************************************************/
//  ��������� ������ ������
/*************************************************************************/

PlayerChessShape::PlayerChessShape(ChessShape* pShape, Sprite* pParent):
 BaseGameObject(pShape, pParent), m_pChessShape(pShape), mPrevX(0.0f), mPrevY(0.0f)
{
  //���������������� ���� ��� ����������� � ������ ������
  pShape->addObserver(static_cast<IChessShapeObserver*>(this));
}

 PlayerChessShape::~PlayerChessShape()
 {
   //������� ���� �� ������������
   m_pChessShape->removeObserver(static_cast<IChessShapeObserver*>(this));
 }

void 
PlayerChessShape::update(ChessShape* sender)
{
    //���� �������� ������ ����� ���������
	if(!sender->isActive())
	{
		//������� � ��������� ���������
		setState(SS_INVISIBLE);
	}
    //������ ��������� ������� � ��������� � ���������� ������ �� �����
	SCOORD scale = (SCOORD)(100 / 8.0f);
    
	SCOORD x = scale * sender->getHCell();
	SCOORD y = scale * sender->getVCell();

	setCoords(x, y);
}

//������� ��� � �������� �������� (oLButtonDowm)
void 
PlayerChessShape::onLButtonDown(Sprite* sender, CURCOORD x, CURCOORD y)
{
    //�������� ������� ��������� ����
	GAMESTATE state = TheGameHost.getGameState();
    //���� ��������� ���� �� ��������� ��������� ���� ������
	if(state != GS_WAIT_PLAYER_TURN)
	{
        //�����
		return;
	}
 
    //������� � ��������� "���������������"
	setState(SS_DROPPED);
	//��������� ������� ���������� � ���������� mPrevX � mPrevY
	mPrevX = getX();
	mPrevY = getY();
}

//������ ��� � �������� �������� (onLButtonUp)
void 
PlayerChessShape::onLButtonUp(Sprite* sender, CURCOORD x, CURCOORD y)
{
    //���� ������� ��������� �� "���������������"
    if(getState() != SS_DROPPED)
    {
		//�����
		return;
    }

	//������� � "�����������" ���������
	setState(SS_NEITRAL);

    //�������������� ������� ��������� ������� � ����������� ���������
    //� ������� �� ��������� �����
    Sprite* pParent = getParent();

    SCOORD parentLeft = pParent->getX();
    SCOORD parentTop = pParent->getY();
	SCOORD parentWidth = pParent->getWidth();
	SCOORD parentHeight = pParent->getHeight();
    	
	SCOORD localTo_X = (SCOORD)x - parentLeft;
	SCOORD localTo_Y = (SCOORD)y - parentTop;
 	
	int chessBoardFrom_X = m_pChessShape->getHCell();
	int chessBoardFrom_Y = m_pChessShape->getVCell();
	int chessBoardTo_X = (int)floor((localTo_X * 8) / parentWidth);
	int chessBoardTo_Y = (int)floor((localTo_Y * 8) / parentHeight); 
	
	//����� ������ ��������� ����� "������� ���"
	bool bResult = TheGameHost.makeTurn(chessBoardFrom_X, chessBoardFrom_Y, 
		chessBoardTo_X, chessBoardTo_Y);
 
	//���� ��� ����������
	if(bResult)
	{
		//����������� ������ �� ����� ������
		SCOORD scale = (SCOORD)(100.0f / 8.0f);
        SCOORD newX = chessBoardTo_X * scale;
        SCOORD newY = chessBoardTo_Y * scale;

		setCoords(newX, newY);
		//������ ������� ��������� ���� "������ ���� ������"
		TheGameHost.setGameState(GS_ANALIZ_PLAYER_TURN);
	}else
	{
      //���� ��� ������������
      //������� ������ �� ������� �����
	  SCOORD scale = (SCOORD)(100.0f / 8.0f);
      SCOORD newX = chessBoardFrom_X * scale;
      SCOORD newY = chessBoardFrom_Y * scale;

	  setCoords(newX, newY);

	  //��������� �������� ���� ����� ��� �������
	  //������� ��� �� ���������
	  /*
	  ChessEventQueye& eventQueye = TheGameHost.getTurnEventQueye();
	  while(!eventQueye.empty())
	  {
		  ChessEvent ce = eventQueye.front();
		  
		  if(ce._type == CET_TURN_ERROR)
		  {
			  MessageBoxA(NULL, ce._message.c_str(), "������ ����", MB_OK|MB_ICONEXCLAMATION);
		  }

		  eventQueye.pop();
	  }//while(!eventQueye.empty())
	  */
	}
 
}

//����������� ������� ����
void
PlayerChessShape::onCursorMove(Sprite* sender, CURCOORD x, CURCOORD y, CURSTATE state)
{
   //���� ������ � ��������������� ���������
	if(sender->getState() == SS_DROPPED)
    {
		//��������� � ������ ������� ���������� �������
        //���, ����� ��� ����� ��������� ��� ��������
        SCOORD localX = floor((SCOORD)x - (sender->getWidth() / 2));
        if(localX < 0) localX = 0;
         
        SCOORD localY = floor((SCOORD)y - (sender->getHeight() / 2));
        if(localY < 0) localY = 0;

        Sprite* pParent = sender->getParent();
        localX = ((localX - pParent->getX()) * 100) / pParent->getWidth();
        localY = ((localY - pParent->getY()) * 100) / pParent->getHeight();

        sender->setCoords(localX, localY);
    }
    EventHandler::onCursorMove(sender, x, y, state);      
}


/****************************************************************/
//       ��������� ������ ����������
/****************************************************************/


AIChessShape::AIChessShape(ChessShape* pShape, Sprite* pParent):
 BaseGameObject(pShape, pParent), m_pChessShape(pShape)
{
  //���������������� ���� ��� ����������� � ������ ������
  pShape->addObserver(static_cast<IChessShapeObserver*>(this));

  mPrevHCell = pShape->getHCell();
  mPrevVCell = pShape->getVCell();
}

AIChessShape::~AIChessShape()
{
  m_pChessShape->removeObserver(static_cast<IChessShapeObserver*>(this));
}

void 
AIChessShape::update(ChessShape* sender)
{
    //���� �������� ������ ����� ���������
	if(!sender->isActive())
	{
		//������� � ��������� ���������
		setState(SS_INVISIBLE);
	}

	//���� ������ �������� ��������� �� �����
	if((sender->getHCell() != mPrevHCell) || (sender->getVCell() != mPrevVCell))
	{
		//������ �������� ����������� ������
		//�� ����� ������

        //��������� ����� ����������
        mPrevHCell = sender->getHCell();
        mPrevVCell = sender->getVCell();

		Sprite* pParent = getParent();
		SCOORD targetX = pParent->getX() + ((pParent->getWidth() / 8.0f) * sender->getHCell());
		SCOORD targetY = pParent->getY() + ((pParent->getHeight() / 8.0f) * sender->getVCell());
        
		Animator* pAnimator = new Animator(getMatrix());
        AnimationKey key1, key2;
        
		SCOORD selfWidth = getWidth();
		SCOORD selfHeight = getHeight();

        key1.setTime(0);
        key1.setScaling(selfWidth, selfHeight, 1.0f);
        key1.setTranslation(getX(), getY(), 0.0f);
        pAnimator->addAnimationKey(key1);
		
		//����� �������� - ���� �������, ����� ����� ��������� ������ ��������
        key2.setTime(1000);
        key2.setScaling(selfWidth, selfHeight, 1.0f);
        key2.setTranslation(targetX, targetY, 0.0f);
        pAnimator->addAnimationKey(key2);
		
		//��������� ����������� ��� ��������� ��������� �� ��������� ��������
		pAnimator->addObserver(static_cast<IAnimationObserver*>(this));

        TheGameHost.addAnimation(pAnimator);		
	}
}

//���������� �� ��������� ��������
void 
AIChessShape::update(Animator* sender)
{
  //������ ���������� ������� �� ����� ������
  SCOORD scale = (SCOORD)(100.0f / 8.0f);
  SCOORD newX = m_pChessShape->getHCell() * scale;
  SCOORD newY = m_pChessShape->getVCell() * scale;

  setCoords(newX, newY);  
  //������ ������� ��������� ���� "������ ���� ����������"
  TheGameHost.setGameState(GS_ANALIZ_AI_TURN);
}