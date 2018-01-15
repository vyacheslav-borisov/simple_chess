#include "StdAfx.h"
#include "ChessShapeSprite.h"


//нарисовать спрайт
void 
ChessShapeSprite::draw(DEVICECONTEXT device)
{
 if(!mShape) return;

 if(!mShape->isActive())
	 return;

 Sprite::draw(device);
}
    
//метод для реализации шаблона "прототип"
Sprite* 
ChessShapeSprite::clone()
{
 return new ChessShapeSprite();
}
	
//методы для работы со свойством
void 
ChessShapeSprite::setShape(ChessShape* pShape)
{
 if(pShape==NULL)
	 return;

 mShape = pShape;
}

ChessShape* 
ChessShapeSprite::getShape()
{
 return mShape;
}
