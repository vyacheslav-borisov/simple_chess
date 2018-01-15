#include "stdafx.h"
#include "../structs/singleton.h"
#include "ChessCommon.h"

using namespace std;

unsigned int ChessShape::sChessShapeId = 1;

//����������� �������� ������: ��������� ������������� ������� ���������
ChessShape::ChessShape(void)
{
 mShapeId = getNextId();
 mOwner = NULL;
 mCellH = 0;
 mCellV = 0;
 mColor = CC_BLACK;
 mActive = false;
 mEventQueye = NULL;
}

ChessShape::~ChessShape()
{
	mObservers.clear();
}

//������ ������� ��� ���������� ������� "�����������"
void 
ChessShape::addObserver(IChessShapeObserver* pObserv)
{
	if(pObserv == NULL)
	{
		return;
	}

	mObservers.push_back(pObserv);
}

void 
ChessShape::removeObserver(IChessShapeObserver* pObserv)
{
    for(ChessShapeObserverList::iterator it = mObservers.begin();
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
ChessShape::notifyObservers()
{
	for(ChessShapeObserverList::iterator it = mObservers.begin();
        it != mObservers.end(); ++it)
	{
		(*it)->update(this);
	}
}

//����� �������������
//���������� ������� ����������� ���������
void 
ChessShape::init(ChessBoard* board, ChessColor color, int cellH, int cellV)
{
 mOwner = board;
 mCellH = cellH;
 mCellV = cellV;
 mColor = color;
 mActive = true;
}

//�����������/������������ ������
//���������������� ������ ���������� "���������"
void 
ChessShape::setActive(bool active)
{
 mActive = active;
 notifyObservers();
}

bool 
ChessShape::checkRange(int HCell, int VCell)
{
 if((HCell < 0) || (HCell >= BOARD_WIDTH))
 {
  addChessEvent(CET_TURN_ERROR, "����� �� ������� ��������������� ���������", NULL);
  return false;
 }
 if((VCell < 0) || (VCell >= BOARD_HEIGHT))
 {
  addChessEvent(CET_TURN_ERROR, "����� �� ������� ������������� ���������", NULL);
  return false;
 }

 return true;
}

void 
ChessShape::addChessEvent(ChessEventType eventType, 
						  const std::string& message,
						  void* param)
{
	if(mEventQueye == NULL)
		return;

    std::string strType = "unknown";
    std::string strColor = "unknown";

    if(getColor() == CC_WHITE)
    {
        strColor = "�����";
    }

    if(getColor() == CC_BLACK)
    {
        strColor = "������";
    }

    switch(getType())
    {
    case CT_EMPTY:
        {
            strType = "������ ������";
            break;
        }
    case CT_PESHKA:
        {
            strType = "�����";
            break;
        }
    case CT_KON:
        {
            strType = "����";
            break;
        }
    case CT_SLON:
        {
            strType = "����";
            break;
        }
    case CT_LADYA:
        {
            strType = "�����";
            break;
        }
    case CT_FERZ:
        {
            strType = "�����";
            break;
        }
    case CT_KOROL:
        {
            strType = "������";
            break;
        }
    default:
        {
            strType = "unknown";
        }
    }
    
    char coords[256];
    sprintf(coords, "[%d, %d]", getHCell(), getVCell()); 

    std::string finalMessage = coords;
    finalMessage+= " ";
    finalMessage+= strType;
    finalMessage+= ", ";
    finalMessage+= strColor;
    finalMessage+= "\n\r";
    finalMessage+= message;

	ChessEvent eventObj;
	eventObj._type = eventType;
	eventObj._message = finalMessage;
	eventObj._param = param;

	mEventQueye->push(eventObj);
}

//���������� �������: ������� ��� ��������� ������
bool 
ChessShape::isActive()
{
 return mActive;
}
	
//���������� ����� ������ �� �����������
inline int 
ChessShape::getHCell()
{
 return mCellH;
}

//���������� ����� ������ �� ���������
inline int 
ChessShape::getVCell()
{
 return mCellV;
}

inline ChessColor 
ChessShape::getColor()
{
 return mColor; 
}

/*
 ���������������� ������ ����������
*/

Peshka::Peshka(): ChessShape()
{
 //���� ���� ���� ����������, 
 //�� ��� ������ ��� ����� ��� ������
 mFirstStep = true;
}

//�����: ������� ���
bool 
Peshka::setNewCell(int HCell, int VCell)
{
 //������ ��������: ��������� ��������� ����� ������ � �������� ����������
 if(!checkRange(HCell, VCell)) return false;

 int vectorV = VCell - mCellV;
 int abs = (vectorV < 0) ? (vectorV*(-1)) :  vectorV;
 int stepV = (abs != 0) ? (vectorV / abs) : 0;

 if((getColor() == CC_BLACK) && (stepV < 0))
 {
	 addChessEvent(CET_TURN_ERROR, "����� �� ����� ��������� �����", NULL);
     return false;
 }
 if((getColor() == CC_WHITE) && (stepV > 0))
 {
	 addChessEvent(CET_TURN_ERROR, "����� �� ����� ��������� �����", NULL);
     return false;
 }
 if(stepV == 0)
 {
	 addChessEvent(CET_TURN_ERROR, "�� �� ������� ���", NULL);
     return false;
 }

 bool bTwo = (abs == 2); //�������� ��� �� ��� ������ ������ ���� ��� ������
 bool bOne = (abs == 1);//�������� ��� �� ���� ������ ������ ���� ��� �� ������
  
 ChessShape* pCell = mOwner->getShapeAt(mCellH, (mCellV + stepV));
 bool bEmptyFirst = (pCell == NULL); //������� ���� ��� ������ ������ �����
 
 pCell = mOwner->getShapeAt(mCellH, (mCellV + (stepV*2)));
 bool bEmptySecond = (pCell == NULL);//������� ���� ��� ������ ������ �����

 bool bVertical = mCellH == HCell; //������� ����, ��� ��� ������ �� ���������
 
 //���� ��� ������ �� ���������, ��� ������ ��� �� ��� ������
 //� �� ��������� ���� ������� ��� ����� - ��������� ���
 if(bVertical && mFirstStep && bTwo && bEmptyFirst && bEmptySecond)
 {
  //������� ���� � ��������� ������
  mOwner->setShapeAt(mCellH, mCellV, NULL);
  //���������� ����� ����������
  mCellH =  HCell;
  mCellV = VCell;
  //������ ���� �� ����� ������
  mOwner->setShapeAt(HCell, VCell, this);
  //���������� ���� "������� ����"
  mFirstStep = false;
  if((VCell == 0) || (VCell == (BOARD_HEIGHT-1)))
  {
	  addChessEvent(CET_PAWN_IS_QUEEN, "����� ����� �� ���� �����", this);	  
  }
  notifyObservers();
  return true;
 }
 //���� ��� ������ �� ���������, �� ���� ������ ������ 
 //� �� ���� ������ ��� ������ ����� - ��������� ���
 if(bVertical && bOne && bEmptyFirst)
 {
  //������� ���� � ��������� ������
  mOwner->setShapeAt(mCellH, mCellV, NULL);
  //���������� ����� ����������
  mCellH =  HCell;
  mCellV = VCell;
  //������ ���� �� ����� ������
  mOwner->setShapeAt(HCell, VCell, this);
  //���������� ���� "������� ����"
  mFirstStep = false;
  if((VCell == 0) || (VCell == (BOARD_HEIGHT-1)))
  {
	  addChessEvent(CET_PAWN_IS_QUEEN, "����� ����� �� ���� �����", this);	  
  }
  notifyObservers();
  return true;
 }

 int vectorH = HCell - mCellH;
 int absH = (vectorH < 0) ? (vectorH*(-1)) :  vectorH;
 bool bHorizontal  = (absH == 1); //������� ���� ��� ������ ���������� �� ������������ ������
 pCell = mOwner->getShapeAt(HCell, VCell);
 bool bEnemy = false;
 if(pCell!=NULL)
 {
  //������� ������ �� ����� � �������� ��������� ������
	 bEnemy  = (pCell->getColor() !=mColor);
 }

 //���� ��� �� ���� ������ ������ � �� ���� ������ �� �����������
 //� �� ���� ������ ��������� ������ - ��������� ���
 if(bOne && bHorizontal && bEnemy)
 {
  //������� ��������� ������
  ChessShape*  pEnemy = mOwner->getShapeAt(HCell, VCell);
  pEnemy->setActive(false);
  //������� ���� � ��������� ������
  mOwner->setShapeAt(mCellH, mCellV, NULL);
  //���������� ����� ����������
  mCellH =  HCell;
  mCellV = VCell;
  //������ ���� �� ����� ������
  mOwner->setShapeAt(HCell, VCell, this);
  //���������� ���� "������� ����"
  mFirstStep = false;
  if((VCell == 0) || (VCell == (BOARD_HEIGHT-1)))
  {
	  addChessEvent(CET_PAWN_IS_QUEEN, "����� ����� �� ���� �����", this);	  
  }
  notifyObservers();
  return true;
 }
 addChessEvent(CET_TURN_ERROR, "������������ ��� ��� �����", NULL);
 return false;
}

//����: ������� ���
bool 
Kon::setNewCell(int HCell, int VCell)
{
 //������ ��������: ��������� ��������� ����� ������ � �������� ����������
 if(!checkRange(HCell, VCell)) return false;
 
 //������ ���������� �������� �� ����������� � �� ���������
 int vOffset[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
 int hOffset[8] = {1, 2, 2, 1, -1, -2, -2, -1};
 
 //��������, �������� �� ������� ������ � ���� �� ����������
 bool bValidCell = false;
 for(int i = 0; i < 8; i++)
 {
  bool bValidH = ((mCellH + hOffset[i]) == HCell);
  bool bValidV = ((mCellV + vOffset[i]) == VCell);
  if(bValidH && bValidV)
  {
   bValidCell = true;
   break;
  }
 }
 if(!bValidCell)
 {
  addChessEvent(CET_TURN_ERROR, "������������ ��� ��� ����", NULL);
  return false;
 }
 
 ChessShape* pCell = mOwner->getShapeAt(HCell, VCell);
 
 //���� ������� ������ �������...
 if(pCell!=NULL)
 {
	 if(pCell->getColor()!=mColor)
	 {
		 pCell->setActive(false);//��������� ������, "������� �"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "��� ���� ������ ����� �������", NULL);
		 return false; //���� ������ - ��� ����������
	 }
 }
 //������� ���� � ��������� ������
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //���������� ����� ����������
 mCellH =  HCell;
 mCellV = VCell;
 //������ ���� �� ����� ������
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

//����: ������� ���
bool 
Slon::setNewCell(int HCell, int VCell)
{
 //������ ��������: ��������� ��������� ����� ������ � �������� ����������
 if(!checkRange(HCell, VCell)) return false;
 
 int vectorV = VCell - mCellV;
 int absV = (vectorV < 0) ? (vectorV*(-1)) :  vectorV;
 int stepV = (absV != NULL) ? (vectorV / absV) : 0;

 int vectorH = HCell - mCellH;
 int absH = (vectorH < 0) ? (vectorH*(-1)) :  vectorH;
 int stepH = (absH != NULL) ? (vectorH / absH) : 0;
 
 //������� ���� ��� ������ ������������� �� ���������
 //�������� �� ����������� � ��������� �����
 if(absV != absH)
 {
  addChessEvent(CET_TURN_ERROR, "���� ����� ������ ������ �� ���������", NULL);
  return false;
 }
 
 //��������, ����� �� ���� ����� �� ������ ������ �����
 //�������� ����� � ������, ���� ����������� ������ ��� �� ���� ������
 if(absH > 1)
 {
	 for(int i = 1; i < absH; i++)
     {
		 int h = mCellH + (i*stepH);
		 int v = mCellV + (i*stepV);

		 ChessShape* pCell = mOwner->getShapeAt(h, v);
		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "���� �� ����� ������� ����� ������ ������", NULL);
			 return false;
         }
     }
 }
 //�������� �� ����� �� �� ������� ������ ������ ������
 ChessShape* pShape = mOwner->getShapeAt(HCell, VCell);
 
 if(pShape!=NULL)
 {
	 if(pShape->getColor()!=mColor)
	 {
		 pShape->setActive(false); //��������� ������, "������� �"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "��� ���� ������ ����� �������", NULL);
		 return false; //���� ������ - ��� ����������
	 }
 }
 //������� ���� � ��������� ������
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //���������� ����� ����������
 mCellH =  HCell;
 mCellV = VCell;
 //������ ���� �� ����� ������
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

//�����: ������� ���
bool 
Ladya::setNewCell(int HCell, int VCell)
{
 //������ ��������: ��������� ��������� ����� ������ � �������� ����������
 if(!checkRange(HCell, VCell)) return false;
 
 int vectorV = VCell - mCellV;
 int absV = (vectorV < 0) ? (vectorV*(-1)) :  vectorV;
 int stepV = (absV != 0) ? (vectorV / absV) : 0;

 int vectorH = HCell - mCellH;
 int absH = (vectorH < 0) ? (vectorH*(-1)) :  vectorH;
 int stepH = (absH != 0) ? (vectorH / absH) : 0;
 
 //����� ����� ��������� ���� �� �����������
 //���� �� ���������
 bool bV = (absV > 0) && (absH == 0);
 bool bH = (absV == 0) && (absH > 0);
 if(!(bV || bH))
 {
	 addChessEvent(CET_TURN_ERROR, "����� ����� ��������� ������ �� ����������� ��� �� ���������", NULL);
	 return false;
 }
 
 //��������, ����� �� ���� ����� �� ������ ������ �����
 //�������� �� �����������, ����� �������� ������ ���� ����������� ������ ��� �� ���� ������
 if(absH > 1)
 {
	 for(int i = 1; i < absH; i++)
	 {
		 int h = mCellH + (i*stepH);
		 int v = mCellV;

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "����� �� ����� ������������� ����� ������ ������", NULL);
			 return false;
         }
     }
 }

 //�������� �� ���������, ����� �������� ������ ���� ����������� ������ ��� �� ���� ������
 if(absV > 1)
 {
	 for(int i = 1; i < absV; i++)
	 {
		 int h = mCellH;
		 int v = mCellV + (i*stepV);

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "����� �� ����� ������������� ����� ������ ������", NULL);
			 return false;
         }
     }
 }

 //�������� �� ����� �� �� ������� ������ ������ ������
 ChessShape* pShape = mOwner->getShapeAt(HCell, VCell);
 
 if(pShape!=NULL)
 {
	 if(pShape->getColor()!=mColor)
	 {
		 pShape->setActive(false); //��������� ������, "������� �"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "��� ���� ������ ����� �������", NULL);
		 return false; //���� ������ - ��� ����������
	 }
 }
 //������� ���� � ��������� ������
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //���������� ����� ����������
 mCellH =  HCell;
 mCellV = VCell;
 //������ ���� �� ����� ������
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

//�����: ������� ���
bool 
Ferz::setNewCell(int HCell, int VCell)
{
 //������ ��������: ��������� ��������� ����� ������ � �������� ����������
 if(!checkRange(HCell, VCell)) return false;
 
 int vectorV = VCell - mCellV;
 int absV = (vectorV < 0) ? (vectorV*(-1)) :  vectorV;
 int stepV = (absV != 0) ? (vectorV / absV) : 0;

 int vectorH = HCell - mCellH;
 int absH = (vectorH < 0) ? (vectorH*(-1)) :  vectorH;
 int stepH = (absH != 0) ? (vectorH / absH) : 0;
 
 //����� ����� ��������� ���� �� �����������,
 //��������� � ���������
 bool bV = (absV > 0) && (absH == 0);
 bool bH = (absV == 0) && (absH > 0);
 bool bD = (absV == absH);
 if(!(bV || bH || bD))
 {
  addChessEvent(CET_TURN_ERROR, "����� ����� ��������� ������ �� ���������, ����������� ��� ���������", NULL);
  return false;
 }
 
 //��������, ����� �� ���� ����� �� ������ ������ �����
 //�������� �� ������������ �����������
 if(bV && (absV > 1))
 {
	 for(int i = 1; i < absV; i++)
     {
		 int h = mCellH;
		 int v = mCellV + (i*stepV);

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "����� �� ����� ������������� ����� ������ ������", NULL);
			 return false;
         }
     }
 }
 //�������� �� �������������� �����������
 if(bH && (absH > 1))
 {
	 for(int i = 1; i < absH; i++)
     {
		 int h = mCellH + (i*stepH);
		 int v = mCellV;

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "����� �� ����� ������������� ����� ������ ������", NULL);
			 return false;
         }
     }
 }
 //�������� �� ������������ �����������
 if(bD && (absH > 1))
 {
	 for(int i = 1; i < absH; i++)
     {
		 int h = mCellH + (i*stepH);
		 int v = mCellV + (i*stepV);

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "����� �� ����� ������������� ����� ������ ������", NULL);
			 return false;
         }
     }
 }

 //�������� �� ����� �� �� ������� ������ ������ ������
 ChessShape* pShape = mOwner->getShapeAt(HCell, VCell);
 
 if(pShape!=NULL)
 {
	 if(pShape->getColor()!=mColor)
	 {
		 pShape->setActive(false); //��������� ������, "������� �"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "��� ���� ������ ����� �������", NULL);
		 return false; //���� ������ - ��� ����������
	 }
 }
 //������� ���� � ��������� ������
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //���������� ����� ����������
 mCellH =  HCell;
 mCellV = VCell;
 //������ ���� �� ����� ������
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

//������: ������� ���
bool 
Korol::setNewCell(int HCell, int VCell)
{
 //������ ��������: ��������� ��������� ����� ������ � �������� ����������
 if(!checkRange(HCell, VCell)) return false;

 //������ ���������� �������� �� ����������� � �� ���������
 int vOffset[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
 int hOffset[8] = {1, 1, 1, 0, -1, -1, -1, 0};
 
 //��������, �������� �� ������� ������ � ���� �� ����������
 bool bValidCell = false;
 for(int i = 0; i < 8; i++)
 {
  bool bValidH = ((mCellH + hOffset[i]) == HCell);
  bool bValidV = ((mCellV + vOffset[i]) == VCell);
  if(bValidH && bValidV)
  {
   bValidCell = true;
   break;
  }
 }
 if(!bValidCell)
 {
  addChessEvent(CET_TURN_ERROR, "������������ ��� ��� ������", NULL);
  return false;
 }
 
 ChessShape* pCell = mOwner->getShapeAt(HCell, VCell);
 
 //���� ������� ������ �������...
 if(pCell!=NULL)
 {
	 if(pCell->getColor()!=mColor)
	 {
		 pCell->setActive(false); //��������� ������, "������� �"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "��� ���� ������ ����� �������", NULL);
		 return false; //���� ������ - ��� ����������
	 }
 }
 //������� ���� � ��������� ������
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //���������� ����� ����������
 mCellH =  HCell;
 mCellV = VCell;
 //������ ���� �� ����� ������
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

void 
Korol::setActive(bool active)
{
 mActive = active;
 if(mActive == false)
 {
	 ChessEventType eventType = (getColor()==CC_WHITE) ? CET_WHITE_KING_KILLED : CET_BLACK_KING_KILLED;
	 std::string message((getColor()==CC_WHITE) ? "������ ����� ���" : "������ ������ ���");
	 addChessEvent(eventType, message, this);
 }
 notifyObservers();
}
//////////////////////////////////////////////////////////////////////////////////////////
void 
Peshka::findHotPoint(HotPointMatrix& hp)
{
	TurnInfo turn;
	turn.hfrom = getHCell();
	turn.vfrom = getVCell();
	turn.src = getType();

	int stepV = (getColor() == CC_WHITE) ? -1 : 1;
    int v = getVCell() + stepV;
	//������� ������
    int h = getHCell() + 1;
	if(checkRange(h, v))
	{
		turn.hto = h;
		turn.vto = v;
		ChessShape* pCell = mOwner->getShapeAt(h, v);
		
		if(pCell == NULL)
			turn.dest = CT_EMPTY;
		else
			turn.dest = pCell->getType();
		hp.addHotPoint(h, v, turn);
	}

	//������� ������
    h = getHCell() - 1;
	if(checkRange(h, v))
	{
		turn.hto = h;
		turn.vto = v;
		ChessShape* pCell = mOwner->getShapeAt(h, v);
		
		if(pCell == NULL)
			turn.dest = CT_EMPTY;
		else
			turn.dest = pCell->getType();
		hp.addHotPoint(h, v, turn);
	}
}

void 
Kon::findHotPoint(HotPointMatrix& hp)
{
    TurnInfo turn;
	turn.hfrom = getHCell();
	turn.vfrom = getVCell();
	turn.src = getType();

	//������ ���������� �������� �� ����������� � �� ���������
    int vOffset[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
    int hOffset[8] = {1, 2, 2, 1, -1, -2, -2, -1};
	
	for(int i = 0; i < 8; i++)
    {
		int h = getHCell() + hOffset[i];
		int v = getVCell() + vOffset[i];
		if(checkRange(h, v))
        {
			turn.hto = h;
			turn.vto = v;
			ChessShape* pCell = mOwner->getShapeAt(h, v);
			
			if(pCell!=NULL)
				turn.dest = pCell->getType();
			else
				turn.dest = CT_EMPTY;

			hp.addHotPoint(h, v, turn);
        }
    }//for(int i = 0; i < 8; i++)
}

void 
Slon::findHotPoint(HotPointMatrix& hp)
{
 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();

 int offsets[4][2] =  { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
 for(int i = 0; i < 4; i++)
 {
  int h = getHCell() + offsets[i][0];
  int v = getVCell() + offsets[i][1];
  while(checkRange(h, v))
  {
   ChessShape* pCell = mOwner->getShapeAt(h, v);
   
   if(pCell == NULL)
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = CT_EMPTY;
	hp.addHotPoint(h, v, turn);
   }else
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = pCell->getType();
	hp.addHotPoint(h, v, turn);
	
    break; 
   }//}else
   h+= offsets[i][0];
   v+= offsets[i][1];
  }//while(checkRange(h, v))
 }//for(int i = 0; i < 4; i++)

}

void 
Ladya::findHotPoint(HotPointMatrix& hp)
{
 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();
 
 int offsets[4][2] =  { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
 for(int i = 0; i < 4; i++)
 {
  int h = getHCell() + offsets[i][0];
  int v = getVCell() + offsets[i][1];
  while(checkRange(h, v))
  {
   ChessShape* pCell = mOwner->getShapeAt(h, v);
   
   if(pCell == NULL)
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = CT_EMPTY;
	hp.addHotPoint(h, v, turn);
   }else
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = pCell->getType();
	hp.addHotPoint(h, v, turn);
	
    break; 
   }//}else
   h+= offsets[i][0];
   v+= offsets[i][1];
  }//while(checkRange(h, v))
 }//for(int i = 0; i < 4; i++)

}

void 
Ferz::findHotPoint(HotPointMatrix& hp)
{
 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();

 int offsets[8][2] =  { 
	 {1, 0}, {-1, 0}, {0, 1}, {0, -1},
     {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
 for(int i = 0; i < 8; i++)
 {
  int h = getHCell() + offsets[i][0];
  int v = getVCell() + offsets[i][1];
  while(checkRange(h, v))
  {
   ChessShape* pCell = mOwner->getShapeAt(h, v);
   
   if(pCell == NULL)
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = CT_EMPTY;
	hp.addHotPoint(h, v, turn);
   }else
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = pCell->getType();
	hp.addHotPoint(h, v, turn);
	
    break; 
   }//}else
   h+= offsets[i][0];
   v+= offsets[i][1];
  }//while(checkRange(h, v))
 }//for(int i = 0; i < 4; i++)

}

void 
Korol::findHotPoint(HotPointMatrix& hp)
{
	TurnInfo turn;
	turn.hfrom = getHCell();
	turn.vfrom = getVCell();
	turn.src = getType();

	//������ ���������� �������� �� ����������� � �� ���������
    int vOffset[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int hOffset[8] = {1, 1, 1, 0, -1, -1, -1, 0};

	for(int i = 0; i < 8; i++)
    {
		int h = getHCell() + hOffset[i];
		int v = getVCell() + vOffset[i];
		if(checkRange(h, v))
        {
			turn.hto = h;
			turn.vto = v;
			ChessShape* pCell = mOwner->getShapeAt(h, v);
			
			if(pCell!=NULL)
				turn.dest = pCell->getType();
			else
				turn.dest = CT_EMPTY;

			hp.addHotPoint(h, v, turn);
        }
    }//for(int i = 0; i < 8; i++)
}

////////////////////////////////////////////////////////////////////////////////
void 
Peshka::getTurnList(vector<TurnInfo>& turnList)
{
 int stepV = (getColor()==CC_WHITE) ? -1 : 1;
 bool bFirstEmpty = false;
 bool bSecondEmpty = false;
 int h,v;
 ChessShape* pCell;

 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();

 h = getHCell();
 v = getVCell() + stepV*2;
 if(checkRange(h, v))
 {
  pCell = mOwner->getShapeAt(h, v);
  
  bSecondEmpty = (pCell == NULL);
 }
 
 v = getVCell() + stepV;
 if(checkRange(h, v))
 {
  pCell = mOwner->getShapeAt(h, v);
  bFirstEmpty = (pCell == NULL);
 }
 
 //�� ��� ������ ������ ���� ��� ������ ��� � ��� ��������� ������ �����
 if(mFirstStep && bSecondEmpty && bFirstEmpty)
 {
  turn.hto = getHCell();
  turn.vto = getVCell() + 2*stepV;
  turn.dest = CT_EMPTY;

  turnList.push_back(turn);
 }
 //�� ���� ������ ������ ���� ��������� ������ �����
 if(bFirstEmpty)
 {
  turn.hto = getHCell();
  turn.vto = getVCell() + stepV;
  turn.dest = CT_EMPTY;

  turnList.push_back(turn);
 }
 
 h = getHCell() + 1;
 v = getVCell() + stepV;
 if(checkRange(h, v))
 {
  pCell = mOwner->getShapeAt(h, v);
  if(pCell!=NULL)
  {
	  if(pCell->getColor()!=getColor())
	  {
       turn.hto = h;
	   turn.vto = v;
	   turn.dest = pCell->getType();
	   
       turnList.push_back(turn); 
	  }//if(pCell->getColor()!=getColor())
  }//if(pCell!=NULL)
 }//if(checkRange(h, v))
 
 h = getHCell() - 1;
 if(checkRange(h, v))
 {
  pCell = mOwner->getShapeAt(h, v);
  if(pCell!=NULL)
  {
	  if(pCell->getColor()!=getColor())
	  {
       turn.hto = h;
	   turn.vto = v;
	   turn.dest = pCell->getType();
	   
       turnList.push_back(turn); 
	  }//if(pCell->getColor()!=getColor())
  }//if(pCell!=NULL)
 }//if(checkRange(h, v))

}

void 
Kon::getTurnList(vector<TurnInfo>& turnList)
{
 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();


 //������ ���������� �������� �� ����������� � �� ���������
 int vOffset[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
 int hOffset[8] = {1, 2, 2, 1, -1, -2, -2, -1};
 
 for(int i = 0; i < 8; i++)
 {
  int h = getHCell() + hOffset[i];
  int v = getVCell() + vOffset[i];
  if(checkRange(h, v))
  {
   turn.hto = h;
   turn.vto = v;
   ChessShape* pCell = mOwner->getShapeAt(h, v);
   if(pCell!=NULL)
   {
	   if(pCell->getColor()!=getColor())
	    turn.dest = pCell->getType();
	   else
		   continue;
   }else
   {
	   turn.dest = CT_EMPTY;
   }
   
   turnList.push_back(turn);
  }
 }//for(int i = 0; i < 8; i++)
}

void 
Slon::getTurnList(vector<TurnInfo>& turnList)
{
 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();
 
 int offsets[4][2] =  { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
 for(int i = 0; i < 4; i++)
 {
  int h = getHCell() + offsets[i][0];
  int v = getVCell() + offsets[i][1];
  while(checkRange(h, v))
  {
   ChessShape* pCell = mOwner->getShapeAt(h, v);
   if(pCell == NULL)
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = CT_EMPTY;
	turnList.push_back(turn);
   }else
   {
	if(pCell->getColor()!=getColor())
	{
     turn.hto = h;
	 turn.vto = v;
	 turn.dest = pCell->getType();
	 turnList.push_back(turn);
	}
    break; 
   }//}else
   h+= offsets[i][0];
   v+= offsets[i][1];
  }//while(checkRange(h, v))
 }//for(int i = 0; i < 4; i++)
}

void 
Ladya::getTurnList(vector<TurnInfo>& turnList)
{
 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();
 
 int offsets[4][2] =  { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
 for(int i = 0; i < 4; i++)
 {
  int h = getHCell() + offsets[i][0];
  int v = getVCell() + offsets[i][1];
  while(checkRange(h, v))
  {
   ChessShape* pCell = mOwner->getShapeAt(h, v);
   if(pCell == NULL)
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = CT_EMPTY;
	turnList.push_back(turn);
   }else
   {
	if(pCell->getColor()!=getColor())
	{
     turn.hto = h;
	 turn.vto = v;
	 turn.dest = pCell->getType();
	 turnList.push_back(turn);
	}
    break; 
   }//}else
   h+= offsets[i][0];
   v+= offsets[i][1];
  }//while(checkRange(h, v))
 }//for(int i = 0; i < 4; i++)
}

void 
Ferz::getTurnList(vector<TurnInfo>& turnList)
{
 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();

 int offsets[8][2] =  { 
	 {1, 0}, {-1, 0}, {0, 1}, {0, -1},
     {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
 
 for(int i = 0; i < 8; i++)
 {
  int h = getHCell() + offsets[i][0];
  int v = getVCell() + offsets[i][1];
  while(checkRange(h, v))
  {
   ChessShape* pCell = mOwner->getShapeAt(h, v);
   if(pCell == NULL)
   {
	turn.hto = h;
	turn.vto = v;
	turn.dest = CT_EMPTY;
	turnList.push_back(turn);
   }else
   {
	if(pCell->getColor()!=getColor())
	{
     turn.hto = h;
	 turn.vto = v;
	 turn.dest = pCell->getType();
	 turnList.push_back(turn);
	}
    break; 
   }//}else
   h+= offsets[i][0];
   v+= offsets[i][1];
  }//while(checkRange(h, v))
 }//for(int i = 0; i < 8; i++)
}

void 
Korol::getTurnList(vector<TurnInfo>& turnList)
{
 TurnInfo turn;
 turn.hfrom = getHCell();
 turn.vfrom = getVCell();
 turn.src = getType();
 
 //������ ���������� �������� �� ����������� � �� ���������
 int vOffset[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
 int hOffset[8] = {1, 1, 1, 0, -1, -1, -1, 0};

 for(int i = 0; i < 8; i++)
 {
  int h = getHCell() + hOffset[i];
  int v = getVCell() + vOffset[i];
  if(checkRange(h, v))
  {
   turn.hto = h;
   turn.vto = v;
   ChessShape* pCell = mOwner->getShapeAt(h, v);
   if(pCell!=NULL)
   {
	   if(pCell->getColor()!=getColor())
		   turn.dest = pCell->getType();
	   else
		   continue;
   }else
   {
	   turn.dest = CT_EMPTY;
   }
   
   turnList.push_back(turn);
  }
 }//for(int i = 0; i < 8; i++)
}