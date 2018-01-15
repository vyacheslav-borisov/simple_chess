
#include <stdafx.h>
#include "animation.h"

/*******************************************************************/
//   ключ анимации
/*******************************************************************/

//сбросить афинные преобразования
void 
AnimationKey::reset()
{
   D3DXMatrixIdentity(&m_Frame);      
}

//перенос
void 
AnimationKey::setTranslation(FLOAT x, FLOAT y, FLOAT z)
{
   D3DXMATRIX op;
   
   D3DXMatrixIdentity(&op);
   D3DXMatrixTranslation(&op, x, y, z);
   D3DXMatrixMultiply(&m_Frame, &m_Frame, &op);
}

//масштабирование
void 
AnimationKey::setScaling(FLOAT x, FLOAT y, FLOAT z)
{
   D3DXMATRIX op;
   
   D3DXMatrixIdentity(&op);
   D3DXMatrixScaling(&op, x, y, z);
   D3DXMatrixMultiply(&m_Frame, &m_Frame, &op);
}

//поворот по оси Х
void 
AnimationKey::setRotationX(FLOAT angle)
{
   D3DXMATRIX op;
   
   D3DXMatrixIdentity(&op);
   D3DXMatrixRotationX(&op, angle);
   D3DXMatrixMultiply(&m_Frame, &m_Frame, &op);

}

//поворот по оси Y
void 
AnimationKey::setRotationY(FLOAT angle)
{
   D3DXMATRIX op;
   
   D3DXMatrixIdentity(&op);
   D3DXMatrixRotationY(&op, angle);
   D3DXMatrixMultiply(&m_Frame, &m_Frame, &op);
}

//поворот по оси Z
void 
AnimationKey::setRotationZ(FLOAT angle)
{
   D3DXMATRIX op;
   
   D3DXMatrixIdentity(&op);
   D3DXMatrixRotationZ(&op, angle);
   D3DXMatrixMultiply(&m_Frame, &m_Frame, &op);
}

/*******************************************************************/
//   контроллер анимации
/*******************************************************************/

Animator::Animator(D3DXMATRIX* obj): m_AnimatedObject(obj)
{
    reset();
}
    
Animator::~Animator()
{
    m_Frames.clear();
	mObservers.clear();
}
    
//добавить ключ анимации
void 
Animator::addAnimationKey(const AnimationKey& key)
{
    m_Frames.push_back(key);  
}

//сбросить таймер анимации
void 
Animator::reset()
{
    m_dwStartTime = timeGetTime();
}

//анимировать обьект 
void 
Animator::animate(DWORD& dwTime)
{
    DWORD dwTimePos = timeGetTime() - m_dwStartTime;

    if(dwTimePos > getLength())
    {
        dwTime = (DWORD)k_nAnimationComplete;
        
		if(m_Frames.size() > 0)
		{
			D3DXMATRIX lastFrame = m_Frames.back().getMatrix();
			(*m_AnimatedObject) = lastFrame;
		}
		
		notifyObservers();
        return;
    }else
    {
        dwTime = dwTimePos;
    }

    DWORD dwTotalTime = 0;
    D3DXMATRIX op1, op2;
    
    for(size_t i = 0; i < m_Frames.size(); i++)
    {
        dwTotalTime+= m_Frames[i].getTime();
        
        if(dwTotalTime >= dwTimePos)
        {
           op2 = m_Frames[i].getMatrix();
           if(i > 0)
           {
               op1 = m_Frames[i-1].getMatrix();
           }else
           {
               op1  = op2;
           }

           DWORD dwFrameLength = m_Frames[i].getTime();
           if(dwFrameLength == 0)
           {
               return;
           }

           DWORD localTimePos = dwTimePos - dwTotalTime + dwFrameLength;
           float fTimeScale = (localTimePos * 1.0) / (dwFrameLength * 1.0);
           
           (*m_AnimatedObject) = op1 + (op2 - op1) * fTimeScale;
           
           return;
        }
    }
}

//получить длину анимации 
DWORD 
Animator::getLength()
{
    DWORD totalTime = 0;

    for(AnimationSet::iterator it = m_Frames.begin();
        it != m_Frames.end(); ++it)
    {
        totalTime+= (*it).getTime();
    }

    return totalTime;
}

void 
Animator::addObserver(IAnimationObserver* pObserv)
{
	if(pObserv == NULL)
	{
		return;
	}

	mObservers.push_back(pObserv);
}

void 
Animator::removeObserver(IAnimationObserver* pObserv)
{
	for(AnimationObserverList::iterator it = mObservers.begin();
        it != mObservers.end(); ++it)
	{
		if(pObserv == *it)
		{
			mObservers.erase(it);
			break;
		}
	}
}

void 
Animator::notifyObservers()
{
	for(AnimationObserverList::iterator it = mObservers.begin();
        it != mObservers.end(); ++it)
	{
		(*it)->update(this);
	}
}
