#pragma once

class RootSprite :
	public CompositeSprite
{
public:
    RootSprite();
	RootSprite(SPRITEID id);
	virtual ~RootSprite();
        
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
    	
	virtual void setParent(Sprite* pParent);
	virtual Sprite* getParent();

	//����� ��� ���������� ������� "��������"
	virtual Sprite* clone();

	
};
