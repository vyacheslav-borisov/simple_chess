#pragma once



/*
class Texture;
class GraphManager;
class Matrix4x4;
*/

typedef FLOAT SCOORD;
typedef IDirect3DTexture9* SPRITESKIN;
typedef IDirect3DDevice9* DEVICECONTEXT;
typedef unsigned int SPRITEID;
typedef D3DXMATRIX SPRITEMATRIX;

const SPRITEID kSpriteDefaultId = -1;

/*
enum SpriteState
{
 SS_INVISIBLE = 0,
 SS_NEITRAL = 1,
 SS_BLURED = 2,
 SS_FOCUSED = 3,
 SS_DROPPED = 4,
 SS_TOPVAL = 5
};
*/
typedef int SpriteState;
const SpriteState SS_INVISIBLE = 0;
const SpriteState SS_NEITRAL = 1;
const SpriteState SS_BLURED = 2;
const SpriteState SS_HOVER = 2;
const SpriteState SS_FOCUSED = 3;
const SpriteState SS_DROPPED = 4;
const SpriteState SS_TOPVAL = 5;

struct SpriteVertex
{
 FLOAT x, y, z;
 FLOAT tx, ty;
};



/*
 Данный класс представляет инкапсулирует свойства спрайта
 элемента графического интерфейса пользователя. В основу положена идея вложенности
 таких элементов по принципу часть-целое, поэтому класс реализован по шаблону "компоновщик"

 В классе и его потомках установлено соглашение: 
 внутренние координаты являються относительными, 
 и только корневой элемент содержит абсолютные координаты
 
 Относительные координаты задаються в процентах и показывают сколько процентов данный
 элемент занимает относительно от величины родительского элемента. В свою очередь координаты
 отступа показывают величину отступа в процентах от размеров родительского элемента
*/

class Sprite
{
public:
	Sprite();
	Sprite(SPRITEID id, Sprite* pParent);
	virtual ~Sprite();
    //нарисовать спрайт
	virtual void draw(DEVICECONTEXT device);
    
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
    
	//изменить состояние элемента в зависимости от события
	virtual void setState(SpriteState state);
	virtual SpriteState getState();
    
	//задать текстуру для отдельного состояния
	virtual void setStateSkin(SpriteState state, SPRITESKIN skin);
	virtual SPRITESKIN getStateSkin(SpriteState state); 

	virtual void setParent(Sprite* pParent);
	virtual Sprite* getParent();

	//группа методов для работы с ИД
    virtual void setID(const SPRITEID& id);
	virtual SPRITEID getID();
	virtual Sprite* findSpriteByID(const SPRITEID& id);

	//группа методов для работы с матрицами
	virtual void setMatrix(const SPRITEMATRIX& mat);
    virtual SPRITEMATRIX* getMatrix();
	virtual void recalkMatrix();

	//метод для реализации шаблона "прототип"
	virtual Sprite* clone();

    //проверить находяться ли абсолютные координаты внутри элемента
	virtual bool isCoordsInto(SCOORD x, SCOORD y);
	//получить спрайт абсолютные координаты которого находяться в пределах
	//искомого элемента
	virtual Sprite* findSprite(SCOORD x, SCOORD y);
    
	//методы для реализации шаблона "компоновщик" 
	virtual void addChild(Sprite* pSprite);
	virtual void removeChild(Sprite* pSprite);
protected:
	SPRITEID mID;

	SCOORD mX;
	SCOORD mY;
	SCOORD mWidth;
	SCOORD mHeight;

	SPRITEMATRIX mWorldMatrix;

    SpriteState mCurState;
	std::map<SpriteState, SPRITESKIN> mSkins;

	Sprite* mParent;
private:
	static const SpriteVertex sVertices[6];
};
