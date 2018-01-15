#pragma once



class CompositeSprite :
	public Sprite
{
 public:
	CompositeSprite();
	CompositeSprite(SPRITEID id, Sprite* pParent);
	virtual ~CompositeSprite();
    //���������� ������
	virtual void draw(DEVICECONTEXT device);
    
	//������ �� ������ ������������� �������� � ���������
    virtual void setCoords(SCOORD x, SCOORD y);
	//������� � ��������� ������������ �������� ������������� ��������
	virtual void setSize(SCOORD width, SCOORD height);
    
	//������ ������� ��� ������ � ��
    virtual Sprite* findSpriteByID(const SPRITEID& id);

	//����� ��� ���������� ������� "��������"
	virtual Sprite* clone();

    //�������� ������ ���������� ���������� �������� ���������� � ��������
	//�������� ��������
	virtual Sprite* findSprite(SCOORD x, SCOORD y);
    
	//������ ��� ���������� ������� "�����������" 
	virtual void addChild(Sprite* pSprite);
	virtual void removeChild(Sprite* pSprite);
protected:
	std::vector<Sprite*> mChilds;
};
