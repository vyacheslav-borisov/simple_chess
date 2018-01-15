#pragma once



class CompositeSprite :
	public Sprite
{
 public:
	CompositeSprite();
	CompositeSprite(SPRITEID id, Sprite* pParent);
	virtual ~CompositeSprite();
    //нарисовать спрайт
	virtual void draw(DEVICECONTEXT device);
    
	//отступ от начала родительского элемента в процентах
    virtual void setCoords(SCOORD x, SCOORD y);
	//размеры в процентах относительно размеров родительского элемента
	virtual void setSize(SCOORD width, SCOORD height);
    
	//группа методов для работы с ИД
    virtual Sprite* findSpriteByID(const SPRITEID& id);

	//метод для реализации шаблона "прототип"
	virtual Sprite* clone();

    //получить спрайт абсолютные координаты которого находяться в пределах
	//искомого элемента
	virtual Sprite* findSprite(SCOORD x, SCOORD y);
    
	//методы для реализации шаблона "компоновщик" 
	virtual void addChild(Sprite* pSprite);
	virtual void removeChild(Sprite* pSprite);
protected:
	std::vector<Sprite*> mChilds;
};
