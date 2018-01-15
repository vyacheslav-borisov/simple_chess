#include "StdAfx.h"
#include "ChessShapeSprite.h"


//���������� ������
void 
ChessShapeSprite::draw(DEVICECONTEXT device)
{
 if(!mShape) return;

 if(!mShape->isActive())
	 return;

 Sprite::draw(device);
}
    
//����� ��� ���������� ������� "��������"
Sprite* 
ChessShapeSprite::clone()
{
 return new ChessShapeSprite();
}
	
//������ ��� ������ �� ���������
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
