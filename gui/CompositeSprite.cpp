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

//���������� ������
void 
CompositeSprite::draw(DEVICECONTEXT device)
{
 if(getState()==SS_INVISIBLE)
	 return;

 //���������� ������ ����
 //�������� ������� ������ ���������
 Sprite::draw(device);
 
 //���������� ��������
 for(vector<Sprite*>::iterator it = mChilds.begin(); 
	 it != mChilds.end(); ++it)
 {
  Sprite* child = *it;
  child->draw(device);
 }
}
    
//������ �� ������ ������������� �������� � ���������
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

//������� � ��������� ������������ �������� ������������� ��������
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
    
//������ ������� ��� ������ � ��
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

//����� ��� ���������� ������� "��������"
Sprite* 
CompositeSprite::clone()
{
 return new CompositeSprite();
}

//�������� ������ ���������� ���������� �������� ���������� � ��������
//�������� ��������
Sprite* 
CompositeSprite::findSprite(SCOORD x, SCOORD y)
{
 //���� ������� �������, ������ � ��������� � ��������� 
 //� ������� �������� �� �����
 if(mCurState == SS_INVISIBLE)
	 return NULL;
 
 if(!isCoordsInto(x, y))
	 return NULL;

 //�������� �������� � �����, ��� �������� ����� ������� ����� ����������
 //������� ����� ����������� ���������
 //��� ������� ��� ���� ��� �� ����������� ���������� ���� �������� ��������� �
 //������� ������� � ����� �� ����������
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
    
//������ ��� ���������� ������� "�����������" 
void 
CompositeSprite::addChild(Sprite* pSprite)
{
 if(pSprite==NULL)
	 return;

 for(std::vector<Sprite*>::iterator it = mChilds.begin();
     it != mChilds.end(); it++)
 {
     //���� ����� ������� ��� ���� � ������
     //�������, ����� ������������� ������������
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
