#pragma once

class RootSprite :
	public CompositeSprite
{
public:
    RootSprite();
	RootSprite(SPRITEID id);
	virtual ~RootSprite();
        
	//отступ от начала родительского элемента в процентах
    virtual void setCoords(SCOORD x, SCOORD y);
	//получить абсолютные координаты элемента
	virtual SCOORD getX();
	virtual SCOORD getY();
	
	//размеры в процентах относительно размеров родительского элемента
	virtual void setSize(SCOORD width, SCOORD height);
    //получить абсолютные размеры элемента
	virtual SCOORD getWidth();
	virtual SCOORD getHeight();
    	
	virtual void setParent(Sprite* pParent);
	virtual Sprite* getParent();

	//метод для реализации шаблона "прототип"
	virtual Sprite* clone();

	
};
