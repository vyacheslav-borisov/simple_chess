#pragma once
#include "Sprite.h"
#include "ChessShape.h"

class ChessShapeSprite :
	public Sprite
{
public:
	ChessShapeSprite(): mShape(NULL) {};
	ChessShapeSprite(SPRITEID id, Sprite* pParent): Sprite(id, pParent),
	mShape(NULL) {};
	virtual ~ChessShapeSprite() {};
    //���������� ������
	virtual void draw(DEVICECONTEXT device);
    
	//����� ��� ���������� ������� "��������"
	virtual Sprite* clone();
	
	//������ ��� ������ �� ���������
	void setShape(ChessShape* pShape);
	ChessShape* getShape();    
private:
	//������ �� ������, � ������� ��������������
	//��������� �������
	ChessShape* mShape;
};
