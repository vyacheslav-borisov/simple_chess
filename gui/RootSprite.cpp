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

     
//отступ от начала родительского элемента в процентах
void 
RootSprite::setCoords(SCOORD x, SCOORD y)
{
 if((x < 0) || (y < 0))
	 return;

 mX = x;
 mY = y;
}

//получить абсолютные координаты элемента
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
	
//размеры в процентах относительно размеров родительского элемента
void 
RootSprite::setSize(SCOORD width, SCOORD height)
{
 if((width < 0)||(height < 0))
	 return;

 mWidth = width;
 mHeight = height;
}

//получить абсолютные размеры элемен
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
 //не делаем ничего, у корня не может быть родителей
}

Sprite* 
RootSprite::getParent()
{
 return NULL;
}

//метод для реализации шаблона "прототип"
Sprite* 
RootSprite::clone()
{
 return new RootSprite();
}


