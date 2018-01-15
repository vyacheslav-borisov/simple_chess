#include "stdafx.h"
#include "../structs/singleton.h"
#include "GUICommon.h"

using namespace std;

CompositeSprite::CompositeSprite()
{
 
}


CompositeSprite::CompositeSprite(SPRITEID id, Sprite* pParent):
Sprite(id, pParent)
{
 
}

CompositeSprite::~CompositeSprite()
{
	for(vector<Sprite*>::iterator it = mChilds.begin();
		it != mChilds.end(); ++it)
	{
		Sprite* child = *it;
		delete child;
	}
	mChilds.clear();
}

//нарисовать спрайт
void 
CompositeSprite::draw(DEVICECONTEXT device)
{
 if(getState()==SS_INVISIBLE)
	 return;

 //нарисовать самого себя
 //вызвваем вариант метода подкласса
 Sprite::draw(device);
 
 //нарисовать потомков
 for(vector<Sprite*>::iterator it = mChilds.begin(); 
	 it != mChilds.end(); ++it)
 {
  Sprite* child = *it;
  child->draw(device);
 }
}
    
//отступ от начала родительского элемента в процентах
void 
CompositeSprite::setCoords(SCOORD x, SCOORD y)
{
	Sprite::setCoords(x, y);

	for(vector<Sprite*>::iterator it = mChilds.begin(); 
	 it != mChilds.end(); ++it)
	{
		 Sprite* child = *it;
		 child->recalkMatrix();
    }
}

//размеры в процентах относительно размеров родительского элемента
void 
CompositeSprite::setSize(SCOORD width, SCOORD height)
{
	Sprite::setSize(width, height);

	for(vector<Sprite*>::iterator it = mChilds.begin(); 
	 it != mChilds.end(); ++it)
	{
		 Sprite* child = *it;
		 child->recalkMatrix();
    }
}
    
//группа методов для работы с ИД
Sprite* 
CompositeSprite::findSpriteByID(const SPRITEID& id)
{
	if(getID()==id)
		return this;

	for(vector<Sprite*>::iterator it = mChilds.begin(); 
	 it != mChilds.end(); ++it)
	{
		 Sprite* child = *it;
		 Sprite* ret = child->findSpriteByID(id);
		 if(ret != NULL)
			 return ret;
    }
	
	return NULL;
}

//метод для реализации шаблона "прототип"
Sprite* 
CompositeSprite::clone()
{
 return new CompositeSprite();
}

//получить спрайт абсолютные координаты которого находяться в пределах
//искомого элемента
Sprite* 
CompositeSprite::findSprite(SCOORD x, SCOORD y)
{
 //если элемент невидим, значит о неактивен и извещения 
 //о событии получать не может
 if(mCurState == SS_INVISIBLE)
	 return NULL;
 
 if(!isCoordsInto(x, y))
	 return NULL;

 //просмотр проводим с конца, для проверки самых верхних слоев интерфейса
 //которые могут перекрывать остальные
 //это сделано для того что бы всплавающие диалоговые окна получали извещения о
 //событии первыми и могли их обработать
 /*
 for(size_t i  = mChilds.size() - 1; i > 0; i--)
 {
  Sprite* child = mChilds[i];
  Sprite* ret = child->findSprite(x, y);
  if(ret!=NULL)
	  return ret;
 }
 */
 for(std::vector<Sprite*>::reverse_iterator it = mChilds.rbegin();
     it != mChilds.rend(); it++)
 {
     Sprite* child = *it;
     Sprite* ret = child->findSprite(x, y);
     if(ret!=NULL)
	   return ret;
 } 

 return this;
}
    
//методы для реализации шаблона "компоновщик" 
void 
CompositeSprite::addChild(Sprite* pSprite)
{
 if(pSprite==NULL)
	 return;

 for(std::vector<Sprite*>::iterator it = mChilds.begin();
     it != mChilds.end(); it++)
 {
     //если такой элемент уже есть в списке
     //выходим, чтобы предотвратить дублирование
     if(pSprite == *it)
     {
         return;
     }
 }

 mChilds.push_back(pSprite);
}

void 
CompositeSprite::removeChild(Sprite* pSprite)
{
 if(pSprite==NULL)
	 return;

 for(vector<Sprite*>::iterator it = mChilds.begin(); 
	 it != mChilds.end(); ++it)
 {
  if(pSprite == (*it))
  {
	  mChilds.erase(it);
	  break;
  }//if(pSprite == (*it))
 }//for(vector<Sprite*>::iterator it = mChilds.begin();
}
