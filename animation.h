#pragma once

class Animator;

class IAnimationObserver
{
 public:
   virtual void update(Animator* sender) = 0;
};

typedef std::vector<IAnimationObserver*> AnimationObserverList;

class AnimationKey
{
public:
    AnimationKey(): m_dwTime(0) { reset(); };
    AnimationKey(const D3DXMATRIX& frame, DWORD time): m_dwTime(time), m_Frame(frame) {};

    void setTime(DWORD time) { m_dwTime = time; };
    DWORD getTime() { return m_dwTime; };
    
    //�������� ������� ��������������
    void reset();
    //�������
    void setTranslation(FLOAT x, FLOAT y, FLOAT z);
    //���������������
    void setScaling(FLOAT x, FLOAT y, FLOAT z);
    //������� �� ��� �
    void setRotationX(FLOAT angle);
    //������� �� ��� Y
    void setRotationY(FLOAT angle);
    //������� �� ��� Z
    void setRotationZ(FLOAT angle);

    D3DXMATRIX getMatrix() { return m_Frame; };
private:
    //������� ��������� ��������� ������� 
    D3DXMATRIX m_Frame;
    //����� ��������
    DWORD m_dwTime;
};

typedef std::vector<AnimationKey> AnimationSet;

class Animator
{
public:
    enum
    {
        k_nAnimationComplete = -1
    };
public:
    Animator(D3DXMATRIX* obj);
    ~Animator();
    
    //�������� ���� ��������
    void addAnimationKey(const AnimationKey& key);
    //�������� ������ ��������
    void reset();
    //����������� ������ 
    void animate(DWORD& dwTime);
    //�������� ����� �������� 
    DWORD getLength();
public:
	void addObserver(IAnimationObserver* pObserv);
	void removeObserver(IAnimationObserver* pObserv);
private:
    void notifyObservers();

    //������ �� ����������� ������
    D3DXMATRIX*  m_AnimatedObject;
    //������ ������ ��������
    AnimationSet m_Frames;
    //��������� �����
    DWORD m_dwStartTime;

	AnimationObserverList mObservers;
};