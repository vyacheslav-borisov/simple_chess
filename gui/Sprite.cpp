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



//����������� �����������
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

//���������������� �����������
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

//����������
Sprite::~Sprite()
{
	mSkins.clear();
}

//���������� ������
void 
Sprite::draw(DEVICECONTEXT device)
{
 if(getState()==SS_INVISIBLE)
	 return;
 device->SetTransform(D3DTS_WORLD, (D3DMATRIX*)getMatrix());
 device->SetTexture(0, getStateSkin(getState()));
 device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, sVertices, sizeof(SpriteVertex));
}
    
//������ �� ������ ������������� �������� � ���������
void 
Sprite::setCoords(SCOORD x, SCOORD y)
{
 //��� �������� ������ ���� � �������� �� 0 �� 100
 if((x > 100) || (x < 0))
	 return;
 if((y > 100) || (y < 0))
	 return;
 //����� ������� � �������� �������� �� ������ ��������� 100 ��������� 
 if((mWidth + x) <= 100)
	 mX = x;
 if((mHeight + y) <= 100)
	 mY = y;
 
 //����������� �������
 recalkMatrix();
}

//�������� ���������� ���������� ��������
SCOORD 
Sprite::getX()
{
 //���� ����������� ������ ����� ��������, ���� ��� �� ��� - ��������� ������
 if(mParent == NULL)
 {
	 throw std::logic_error("�� ������ ������ �� ������������ ����!");
 }
 
 SCOORD offset = (mParent->getWidth() * mX) / 100;
 return (offset + mParent->getX());
}

SCOORD 
Sprite::getY()
{
 //���� ����������� ������ ����� ��������, ���� ��� �� ��� - ��������� ������
 if(mParent == NULL)
 {
	 throw std::logic_error("�� ������ ������ �� ������������ ����!");
 }
 
 SCOORD offset = (mParent->getHeight() * mY) / 100;
 return (offset + mParent->getY());
}
	
//������� � ��������� ������������ �������� ������������� ��������
void 
Sprite::setSize(SCOORD width, SCOORD height)
{
 //��� �������� ������ ���� � �������� �� 0 �� 100
 if((width > 100) || (width < 0))
	 return;
 if((height > 100) || (height < 0))
	 return;
 if((mX + width) <= 100)
	 mWidth = width;
 if((mY + height) <= 100)
	 mHeight = height;
 
 //����������� �������
 recalkMatrix();
}

//�������� ���������� ������� ��������
SCOORD 
Sprite::getWidth()
{
 //���� ����������� ������ ����� ��������, ���� ��� �� ��� - ��������� ������
 if(mParent == NULL)
 {
	 throw std::logic_error("�� ������ ������ �� ������������ ����!");
 }
 
 return ((mWidth * mParent->getWidth()) / 100);
}

SCOORD 
Sprite::getHeight()
{
 //���� ����������� ������ ����� ��������, ���� ��� �� ��� - ��������� ������
 if(mParent == NULL)
 {
	 throw std::logic_error("�� ������ ������ �� ������������ ����!");
 }

 return ((mHeight * mParent->getHeight()) / 100);
}
    
//�������� ��������� �������� � ����������� �� �������
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
    
//������ �������� ��� ���������� ���������
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
	 throw std::logic_error("�� ������ ������ �� ������������ ����!");
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

//��������� ���������� �� ���������� ���������� ������ ��������
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

//�������� ������ ���������� ���������� �������� ���������� � ��������
//�������� ��������
Sprite* 
Sprite::findSprite(SCOORD x, SCOORD y)
{
 if(mCurState == SS_INVISIBLE)
	 return NULL;

 if(!isCoordsInto(x, y))
	 return NULL;

 return this;
}

//������� ��� ���������� ������� "�����������" 
void 
Sprite::addChild(Sprite* pSprite)
{
 //���������� �� ������ ������ �� ������������ ������� ��������
}

void 
Sprite::removeChild(Sprite* pSprite)
{
 //���������� �� ������ ������ �� ������������ ������� ��������
}

////////////////////////////////////////////////////////////////////////////////////////
//������ ������� ��� ������ � ��
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

//������ ������� ��� ������ � ���������
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
 //���������� ������ ������� �� ���������� ������� �������
 //��������� �������
 D3DXMATRIX scale, trans;
 D3DXMatrixScaling(&scale, getWidth(), getHeight(), 1.0f);
 D3DXMatrixTranslation(&trans, getX(), getY(), 0.0f);
 
 mWorldMatrix = scale * trans;
}

//����� ��� ���������� ������� "��������"
Sprite* 
Sprite::clone()
{
 return new Sprite();
}




