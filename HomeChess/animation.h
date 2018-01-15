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
    
    //сбросить афинные преобразования
    void reset();
    //перенос
    void setTranslation(FLOAT x, FLOAT y, FLOAT z);
    //масштабирование
    void setScaling(FLOAT x, FLOAT y, FLOAT z);
    //поворот по оси Х
    void setRotationX(FLOAT angle);
    //поворот по оси Y
    void setRotationY(FLOAT angle);
    //поворот по оси Z
    void setRotationZ(FLOAT angle);

    D3DXMATRIX getMatrix() { return m_Frame; };
private:
    //матрица конечного положения обьекта 
    D3DXMATRIX m_Frame;
    //длина анимации
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
    
    //добавить ключ анимации
    void addAnimationKey(const AnimationKey& key);
    //сбросить таймер анимации
    void reset();
    //анимировать обьект 
    void animate(DWORD& dwTime);
    //получить длину анимации 
    DWORD getLength();
public:
	void addObserver(IAnimationObserver* pObserv);
	void removeObserver(IAnimationObserver* pObserv);
private:
    void notifyObservers();

    //ссылка на анимируемый объект
    D3DXMATRIX*  m_AnimatedObject;
    //массив ключей анимации
    AnimationSet m_Frames;
    //стартовое время
    DWORD m_dwStartTime;

	AnimationObserverList mObservers;
};