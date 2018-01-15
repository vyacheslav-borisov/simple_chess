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
    //нарисовать спрайт
	virtual void draw(DEVICECONTEXT device);
    
	//метод для реализации шаблона "прототип"
	virtual Sprite* clone();
	
	//методы для работы со свойством
	void setShape(ChessShape* pShape);
	ChessShape* getShape();    
private:
	//ссылка на фигуру, с которой синхронизируем
	//состояние спрайта
	ChessShape* mShape;
};
