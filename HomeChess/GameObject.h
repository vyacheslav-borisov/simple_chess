#pragma once



class BaseGameObject: public Sprite, public EventHandler
{
public:
    BaseGameObject(ChessShape* pShape, Sprite* pParent);
    virtual ~BaseGameObject();
public:
    //��������� ������� �� ������� (onBlur)
	virtual void onBlur(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state); 
//������ ������� ��������� (onHower)
	virtual void onHower(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state);
};

class PlayerChessShape: public BaseGameObject, public IChessShapeObserver
{
public:
    PlayerChessShape(ChessShape* pShape, Sprite* pParent);
    virtual ~PlayerChessShape();
public:
    virtual void update(ChessShape* sender);
public:
    //������� ��� � �������� �������� (oLButtonDowm)
	virtual void onLButtonDown(Sprite* sender, CURCOORD x, CURCOORD y);
    //������ ��� � �������� �������� (onLButtonUp)
	virtual void onLButtonUp(Sprite* sender, CURCOORD x, CURCOORD y);
    //����������� ������� ����
	virtual void onCursorMove(Sprite* sender, 
		CURCOORD x, CURCOORD y, CURSTATE state);
private:
    SCOORD mPrevX;
    SCOORD mPrevY;

	ChessShape* m_pChessShape;
};

class AIChessShape: public BaseGameObject, public IChessShapeObserver, public IAnimationObserver
{
public:
    AIChessShape(ChessShape* pShape, Sprite* pParent);
    virtual ~AIChessShape();
public:
    virtual void update(ChessShape* sender);
public:
    virtual void update(Animator* sender);
private:
	int mPrevHCell;
	int mPrevVCell;

    ChessShape* m_pChessShape; 
};