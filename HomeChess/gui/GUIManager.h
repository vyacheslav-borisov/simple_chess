#pragma once

using namespace std;

typedef int TEXTUREID;

enum SpriteType
{
 ST_LEAF = 1,
 ST_COMPOSITE = 2,
 ST_ROOT = 3,
 ST_LABEL = 4,
 ST_ITEMLIST = 5,
 ST_ITEM = 6
};

const SPRITEID GIU_ROOT_ELEMENT = 0;

class GUIManager: public cSingleton<GUIManager>
{
public:
 //����������� ������, ����������� ������ �� �������� ����������
	GUIManager();
 //���������� ������
	~GUIManager(); 
 //����� ������ ������� ��� ��������� ����������
	void setArrea(CURCOORD width, CURCOORD height);

 //����� ��� ���������� ������ �������� � ���������
	bool addElement(SPRITEID parent, Sprite* elem);
 //����� ��� �������� �������� ���������� �� ��� ��
	bool removeElement(SPRITEID elem);
 //����� ��� ��������� �������� ���������� �� ��� ��
	Sprite* getElement(SPRITEID elem);

 //����� ��� ��������� ������������ ����������� �������
	bool setHandler(SPRITEID elem, EventHandler* handler);
 //����� ��� ��������� ����������� ������� �� �� ��������
    EventHandler* getHandler(SPRITEID elem);
 //����� ��� �������� ����������� �������
 //�����! ����� �� ������� ���������� �� ������, � ������ ������� ���
 //������� ��� �� ���������� �������. �������������� ������������ ���
 //��� �������� ������������ ������� ��������� ��������������, ���� ����������
 //������ �������� ����� ��������
    void removeHandler(SPRITEID elem);

 //������ ������� - ������������� �������
 //������� ��� � �������� �������� (oLButtonDowm)
	virtual void onLButtonDown(CURCOORD x, CURCOORD y);
//������ ��� � �������� �������� (onLButtonUp)
	virtual void onLButtonUp(CURCOORD x, CURCOORD y);
//��������� ���� ��� �� �������� (onLButtonClick)
	virtual void onLButtonClick(CURCOORD x, CURCOORD y);
//������� ���� �� ��� �������� (onLButtonDbClick)
	virtual void onLButtonDbClick(CURCOORD x, CURCOORD y);

//������� ��� � �������� �������� (oLButtonDowm)
	virtual void onRButtonDown(CURCOORD x, CURCOORD y);
//������ ��� � �������� �������� (onLButtonUp)
	virtual void onRButtonUp(CURCOORD x, CURCOORD y);
//��������� ���� ��� �� �������� (onLButtonClick)
	virtual void onRButtonClick(CURCOORD x, CURCOORD y);
//������� ���� �� ��� �������� (onLButtonDbClick)
	virtual void onRButtonDbClick(CURCOORD x, CURCOORD y);
//����������� ������� ����
	virtual void onCursorMove(CURCOORD x, CURCOORD y, CURSTATE state);
//��������� ������ ����
	virtual void onCursorWheel(CURCOORD x, CURCOORD y, CURSTATE state, CURWHEEL delta);

//����� ��� ��������� ����������
	void draw(DEVICECONTEXT& device);
private:
 //����� ��� ������� �������� �������� �������
    void destroyAll();
 //������� ���������
	std::map<SPRITEID, Sprite*> mElements;
 //������� ������������ �������
	std::map<SPRITEID, EventHandler*> mHandlers;
 //�������� �������
	RootSprite mRoot;
 //�������, ����������� �� �������� ������������
	Sprite* mBlured;
 //�������, ����������� � ��������� ������
	Sprite* mFocused;

 //������� ����
   D3DXMATRIX mView;   
 //������� ��������
   D3DXMATRIX  mProject;
};

#define TheGUI GUIManager::getInstance()