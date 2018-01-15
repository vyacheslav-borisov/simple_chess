#include "stdafx.h"
#include "../structs/singleton.h"
#include "GUICommon.h"

using namespace std;

RootSprite::RootSprite()
{
 mParent = this;
}

RootSprite::RootSprite(SPRITEID id)
:CompositeSprite(id, this)
{

}

RootSprite::~RootSprite()
{
 mParent = this;
}

     
//������ �� ������ ������������� �������� � ���������
void 
RootSprite::setCoords(SCOORD x, SCOORD y)
{
 if((x < 0) || (y < 0))
	 return;

 mX = x;
 mY = y;
}

//�������� ���������� ���������� ��������
SCOORD 
RootSprite::getX()
{
 return mX;
}

SCOORD 
RootSprite::getY()
{
 return mY;
}
	
//������� � ��������� ������������ �������� ������������� ��������
void 
RootSprite::setSize(SCOORD width, SCOORD height)
{
 if((width < 0)||(height < 0))
	 return;

 mWidth = width;
 mHeight = height;
}

//�������� ���������� ������� ������
SCOORD 
RootSprite::getWidth()
{
 return mWidth;
}

SCOORD 
RootSprite::getHeight()
{
 return mHeight;
}
    	
void 
RootSprite::setParent(Sprite* pParent)
{
 //�� ������ ������, � ����� �� ����� ���� ���������
}

Sprite* 
RootSprite::getParent()
{
 return NULL;
}

//����� ��� ���������� ������� "��������"
Sprite* 
RootSprite::clone()
{
 return new RootSprite();
}


