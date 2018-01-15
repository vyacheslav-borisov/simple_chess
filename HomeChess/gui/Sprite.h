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
 ������ ����� ������������ ������������� �������� �������
 �������� ������������ ���������� ������������. � ������ �������� ���� �����������
 ����� ��������� �� �������� �����-�����, ������� ����� ���������� �� ������� "�����������"

 � ������ � ��� �������� ����������� ����������: 
 ���������� ���������� ��������� ��������������, 
 � ������ �������� ������� �������� ���������� ����������
 
 ������������� ���������� ��������� � ��������� � ���������� ������� ��������� ������
 ������� �������� ������������ �� �������� ������������� ��������. � ���� ������� ����������
 ������� ���������� �������� ������� � ��������� �� �������� ������������� ��������
*/

class Sprite
{
public:
	Sprite();
	Sprite(SPRITEID id, Sprite* pParent);
	virtual ~Sprite();
    //���������� ������
	virtual void draw(DEVICECONTEXT device);
    
	//������ �� ������ ������������� �������� � ���������
    virtual void setCoords(SCOORD x, SCOORD y);
	//�������� ���������� ���������� ��������
	virtual SCOORD getX();
	virtual SCOORD getY();
	
	//������� � ��������� ������������ �������� ������������� ��������
	virtual void setSize(SCOORD width, SCOORD height);
    //�������� ���������� ������� ��������
	virtual SCOORD getWidth();
	virtual SCOORD getHeight();
    
	//�������� ��������� �������� � ����������� �� �������
	virtual void setState(SpriteState state);
	virtual SpriteState getState();
    
	//������ �������� ��� ���������� ���������
	virtual void setStateSkin(SpriteState state, SPRITESKIN skin);
	virtual SPRITESKIN getStateSkin(SpriteState state); 

	virtual void setParent(Sprite* pParent);
	virtual Sprite* getParent();

	//������ ������� ��� ������ � ��
    virtual void setID(const SPRITEID& id);
	virtual SPRITEID getID();
	virtual Sprite* findSpriteByID(const SPRITEID& id);

	//������ ������� ��� ������ � ���������
	virtual void setMatrix(const SPRITEMATRIX& mat);
    virtual SPRITEMATRIX* getMatrix();
	virtual void recalkMatrix();

	//����� ��� ���������� ������� "��������"
	virtual Sprite* clone();

    //��������� ���������� �� ���������� ���������� ������ ��������
	virtual bool isCoordsInto(SCOORD x, SCOORD y);
	//�������� ������ ���������� ���������� �������� ���������� � ��������
	//�������� ��������
	virtual Sprite* findSprite(SCOORD x, SCOORD y);
    
	//������ ��� ���������� ������� "�����������" 
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
