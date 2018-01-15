#include "stdafx.h"
#include "../structs/singleton.h"
#include "GUICommon.h"

using namespace std;


/*
0, 0      1, 0
 A---------B
 |		/  |
 |   /     |
 | /       |
 C---------D 1,1
*/
const SpriteVertex Sprite::sVertices[6] = 
{
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },//A
	{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },//B
	{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },//C

	{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },//C
	{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },//B
	{ 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },//D
};



//стандартный конструктор
Sprite::Sprite():
mX(0), mY(0), mWidth(0), mHeight(0), 
mCurState(SS_NEITRAL), mParent(NULL),
mID(kSpriteDefaultId)
{
 for(SpriteState state = SS_NEITRAL; 
	 state < SS_TOPVAL; state++)
 {
   mSkins.insert(make_pair(state, reinterpret_cast<SPRITESKIN>(NULL)));
 }
}

//инициализирующий конструктор
Sprite::Sprite(SPRITEID id, Sprite* pParent):
mX(0), mY(0), mWidth(0), mHeight(0), 
mCurState(SS_NEITRAL), mParent(pParent),
mID(id)
{
	for(SpriteState state = SS_NEITRAL; 
	 state < SS_TOPVAL; state++)
	{
      mSkins.insert(make_pair(state, reinterpret_cast<SPRITESKIN>(NULL)));
	}
}

//деструктор
Sprite::~Sprite()
{
	mSkins.clear();
}

//нарисовать спрайт
void 
Sprite::draw(DEVICECONTEXT device)
{
 if(getState()==SS_INVISIBLE)
	 return;
 device->SetTransform(D3DTS_WORLD, (D3DMATRIX*)getMatrix());
 device->SetTexture(0, getStateSkin(getState()));
 device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, sVertices, sizeof(SpriteVertex));
}
    
//отступ от начала родительского элемента в процентах
void 
Sprite::setCoords(SCOORD x, SCOORD y)
{
 //эти величины должны быть в пределах от 0 до 100
 if((x > 100) || (x < 0))
	 return;
 if((y > 100) || (y < 0))
	 return;
 //сумма отступа и величины элемента не должна превышать 100 процентов 
 if((mWidth + x) <= 100)
	 mX = x;
 if((mHeight + y) <= 100)
	 mY = y;
 
 //пересчитать матрицу
 recalkMatrix();
}

//получить абсолютные координаты элемента
SCOORD 
Sprite::getX()
{
 //узел обязательно должен иметь родителя, если это не так - произошла ошибка
 if(mParent == NULL)
 {
	 throw std::logic_error("не задана ссылка на родительский узел!");
 }
 
 SCOORD offset = (mParent->getWidth() * mX) / 100;
 return (offset + mParent->getX());
}

SCOORD 
Sprite::getY()
{
 //узел обязательно должен иметь родителя, если это не так - произошла ошибка
 if(mParent == NULL)
 {
	 throw std::logic_error("не задана ссылка на родительский узел!");
 }
 
 SCOORD offset = (mParent->getHeight() * mY) / 100;
 return (offset + mParent->getY());
}
	
//размеры в процентах относительно размеров родительского элемента
void 
Sprite::setSize(SCOORD width, SCOORD height)
{
 //эти величины должны быть в пределах от 0 до 100
 if((width > 100) || (width < 0))
	 return;
 if((height > 100) || (height < 0))
	 return;
 if((mX + width) <= 100)
	 mWidth = width;
 if((mY + height) <= 100)
	 mHeight = height;
 
 //пересчитать матрицу
 recalkMatrix();
}

//получить абсолютные размеры элемента
SCOORD 
Sprite::getWidth()
{
 //узел обязательно должен иметь родителя, если это не так - произошла ошибка
 if(mParent == NULL)
 {
	 throw std::logic_error("не задана ссылка на родительский узел!");
 }
 
 return ((mWidth * mParent->getWidth()) / 100);
}

SCOORD 
Sprite::getHeight()
{
 //узел обязательно должен иметь родителя, если это не так - произошла ошибка
 if(mParent == NULL)
 {
	 throw std::logic_error("не задана ссылка на родительский узел!");
 }

 return ((mHeight * mParent->getHeight()) / 100);
}
    
//изменить состояние элемента в зависимости от события
void 
Sprite::setState(SpriteState state)
{
 if((state < 0) || (state >= SS_TOPVAL))
	 return;
 mCurState = state;
}

SpriteState 
Sprite::getState()
{
 return mCurState;
}
    
//задать текстуру для отдельного состояния
void 
Sprite::setStateSkin(SpriteState state, SPRITESKIN skin)
{
 if((state < 0) || (state >= SS_TOPVAL))
	 return;

 mSkins[state] = skin;
}

SPRITESKIN 
Sprite::getStateSkin(SpriteState state)
{
 if((state < 0) || (state >= SS_TOPVAL))
	 return NULL;
 std::map<SpriteState, SPRITESKIN>::iterator it = mSkins.find(state);
 if(it != mSkins.end())
	 return it->second;
 
 return NULL;
}

void 
Sprite::setParent(Sprite* pParent)
{
 if(pParent==NULL)
	 return;
 
 if(mParent != NULL)
 {
	 mParent->removeChild(this);     
 }
 mParent = pParent;
 mParent->addChild(this);
/* 
 if(mParent == NULL)
 {
	 throw std::logic_error("не задана ссылка на родительский узел!");
 }
 
 mParent->removeChild(this);
 mParent = pParent;
 mParent->addChild(this);
*/
}

Sprite* 
Sprite::getParent()
{
 return mParent;
}

//проверить находяться ли абсолютные координаты внутри элемента
bool 
Sprite::isCoordsInto(SCOORD x, SCOORD y)
{
 SCOORD x1, x2, y1, y2;
 
 x1 = getX();
 x2 = x1 + getWidth();

 if((x < x1) || (x > x2))
	 return false;

 y1 = getY();
 y2 = y1 + getHeight();
 
 if((y < y1) || (y > y2))
	 return false;

 return true;
}

//получить спрайт абсолютные координаты которого находяться в пределах
//искомого элемента
Sprite* 
Sprite::findSprite(SCOORD x, SCOORD y)
{
 if(mCurState == SS_INVISIBLE)
	 return NULL;

 if(!isCoordsInto(x, y))
	 return NULL;

 return this;
}

//функции для реализации шаблона "компоновщик" 
void 
Sprite::addChild(Sprite* pSprite)
{
 //реализация на данном уровне не предполагает никаких действий
}

void 
Sprite::removeChild(Sprite* pSprite)
{
 //реализация на данном уровне не предполагает никаких действий
}

////////////////////////////////////////////////////////////////////////////////////////
//группа методов для работы с ИД
void 
Sprite::setID(const SPRITEID& id)
{
 mID = id;
}

SPRITEID 
Sprite::getID()
{
 return mID;
}

Sprite* 
Sprite::findSpriteByID(const SPRITEID& id)
{
 if(mID != id)
 {
	 return NULL;
 }

 return this;
}

//группа методов для работы с матрицами
void 
Sprite::setMatrix(const SPRITEMATRIX& mat)
{
 mWorldMatrix = mat;
}

SPRITEMATRIX* 
Sprite::getMatrix()
{
 return &mWorldMatrix;
}

void 
Sprite::recalkMatrix()
{
 //содержимое метода зависит от реализации мировой матрицы
 //определим позднее
 D3DXMATRIX scale, trans;
 D3DXMatrixScaling(&scale, getWidth(), getHeight(), 1.0f);
 D3DXMatrixTranslation(&trans, getX(), getY(), 0.0f);
 
 mWorldMatrix = scale * trans;
}

//метод для реализации шаблона "прототип"
Sprite* 
Sprite::clone()
{
 return new Sprite();
}




