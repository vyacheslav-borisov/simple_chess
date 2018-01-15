#include "stdafx.h"
#include "../structs/singleton.h"
#include "ChessCommon.h"

using namespace std;

unsigned int ChessShape::sChessShapeId = 1;

//конструктор базового класса: начальная инициализация свойств подкласса
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

//группа методов для реализации шаблона "наблюдатель"
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

//метод инициализации
//присвоение обьекту конкретного состояния
void 
ChessShape::init(ChessBoard* board, ChessColor color, int cellH, int cellV)
{
 mOwner = board;
 mCellH = cellH;
 mCellV = cellV;
 mColor = color;
 mActive = true;
}

//активируете/деактивирует фигуру
//деактивированная фигура считаеться "сьеденной"
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
  addChessEvent(CET_TURN_ERROR, "выход за границу горизонтального диапазона", NULL);
  return false;
 }
 if((VCell < 0) || (VCell >= BOARD_HEIGHT))
 {
  addChessEvent(CET_TURN_ERROR, "выход за границу вертикального диапазона", NULL);
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
        strColor = "белый";
    }

    if(getColor() == CC_BLACK)
    {
        strColor = "черный";
    }

    switch(getType())
    {
    case CT_EMPTY:
        {
            strType = "пустая клетка";
            break;
        }
    case CT_PESHKA:
        {
            strType = "пешка";
            break;
        }
    case CT_KON:
        {
            strType = "конь";
            break;
        }
    case CT_SLON:
        {
            strType = "слон";
            break;
        }
    case CT_LADYA:
        {
            strType = "ладья";
            break;
        }
    case CT_FERZ:
        {
            strType = "ферзь";
            break;
        }
    case CT_KOROL:
        {
            strType = "король";
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

//возвращает признак: активна или неактивна фигура
bool 
ChessShape::isActive()
{
 return mActive;
}
	
//возвращает номер ячейки по горизонтали
inline int 
ChessShape::getHCell()
{
 return mCellH;
}

//возвращает номер ячейки по вертикали
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
 Переопределяемые методы подклассов
*/

Peshka::Peshka(): ChessShape()
{
 //если этот флаг установлен, 
 //то это значит для пешки ход первый
 mFirstStep = true;
}

//пешка: сделать ход
bool 
Peshka::setNewCell(int HCell, int VCell)
{
 //первая проверка: проверяем вхождение новой ячейки в диапазон допустимых
 if(!checkRange(HCell, VCell)) return false;

 int vectorV = VCell - mCellV;
 int abs = (vectorV < 0) ? (vectorV*(-1)) :  vectorV;
 int stepV = (abs != 0) ? (vectorV / abs) : 0;

 if((getColor() == CC_BLACK) && (stepV < 0))
 {
	 addChessEvent(CET_TURN_ERROR, "пешка не может двигаться назад", NULL);
     return false;
 }
 if((getColor() == CC_WHITE) && (stepV > 0))
 {
	 addChessEvent(CET_TURN_ERROR, "пешка не может двигаться назад", NULL);
     return false;
 }
 if(stepV == 0)
 {
	 addChessEvent(CET_TURN_ERROR, "вы не сделали ход", NULL);
     return false;
 }

 bool bTwo = (abs == 2); //допустим ход на вде клетки вперед если ход первый
 bool bOne = (abs == 1);//допустим ход на одну клетку вперед если ход не первый
  
 ChessShape* pCell = mOwner->getShapeAt(mCellH, (mCellV + stepV));
 bool bEmptyFirst = (pCell == NULL); //признак того что первая клетка пуста
 
 pCell = mOwner->getShapeAt(mCellH, (mCellV + (stepV*2)));
 bool bEmptySecond = (pCell == NULL);//признак того что вторая клетка пуста

 bool bVertical = mCellH == HCell; //признак того, что ход строго по вертикали
 
 //если ход строго по вертикали, это первый ход на две клетки
 //и на следующих двух клетках нет фигур - допустить ход
 if(bVertical && mFirstStep && bTwo && bEmptyFirst && bEmptySecond)
 {
  //снимаем себя с предыущей клетки
  mOwner->setShapeAt(mCellH, mCellV, NULL);
  //записываем новые координаты
  mCellH =  HCell;
  mCellV = VCell;
  //ставим себя на новою клетку
  mOwner->setShapeAt(HCell, VCell, this);
  //сбрасываем флаг "первого хода"
  mFirstStep = false;
  if((VCell == 0) || (VCell == (BOARD_HEIGHT-1)))
  {
	  addChessEvent(CET_PAWN_IS_QUEEN, "пешка дошла до края доски", this);	  
  }
  notifyObservers();
  return true;
 }
 //если ход строго по вертикали, на одну клетку вперед 
 //и на этой клетке нет других фигур - допустить ход
 if(bVertical && bOne && bEmptyFirst)
 {
  //снимаем себя с предыущей клетки
  mOwner->setShapeAt(mCellH, mCellV, NULL);
  //записываем новые координаты
  mCellH =  HCell;
  mCellV = VCell;
  //ставим себя на новою клетку
  mOwner->setShapeAt(HCell, VCell, this);
  //сбрасываем флаг "первого хода"
  mFirstStep = false;
  if((VCell == 0) || (VCell == (BOARD_HEIGHT-1)))
  {
	  addChessEvent(CET_PAWN_IS_QUEEN, "пешка дошла до края доски", this);	  
  }
  notifyObservers();
  return true;
 }

 int vectorH = HCell - mCellH;
 int absH = (vectorH < 0) ? (vectorH*(-1)) :  vectorH;
 bool bHorizontal  = (absH == 1); //признак того что фигуру перемещают на диагональную клетку
 pCell = mOwner->getShapeAt(HCell, VCell);
 bool bEnemy = false;
 if(pCell!=NULL)
 {
  //целевая ячейка не пуста и содержит вражескую фигуру
	 bEnemy  = (pCell->getColor() !=mColor);
 }

 //если ход на одну клетку вперед и на одну клетку по горизонтали
 //и на этой клетке вражеская фигура - допустить ход
 if(bOne && bHorizontal && bEnemy)
 {
  //сьедаем вражескую фигуру
  ChessShape*  pEnemy = mOwner->getShapeAt(HCell, VCell);
  pEnemy->setActive(false);
  //снимаем себя с предыущей клетки
  mOwner->setShapeAt(mCellH, mCellV, NULL);
  //записываем новые координаты
  mCellH =  HCell;
  mCellV = VCell;
  //ставим себя на новою клетку
  mOwner->setShapeAt(HCell, VCell, this);
  //сбрасываем флаг "первого хода"
  mFirstStep = false;
  if((VCell == 0) || (VCell == (BOARD_HEIGHT-1)))
  {
	  addChessEvent(CET_PAWN_IS_QUEEN, "пешка дошла до края доски", this);	  
  }
  notifyObservers();
  return true;
 }
 addChessEvent(CET_TURN_ERROR, "недопустимый ход для пешки", NULL);
 return false;
}

//конь: сделать ход
bool 
Kon::setNewCell(int HCell, int VCell)
{
 //первая проверка: проверяем вхождение новой ячейки в диапазон допустимых
 if(!checkRange(HCell, VCell)) return false;
 
 //массив допустимых смещений по горизонтали и по вертикали
 int vOffset[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
 int hOffset[8] = {1, 2, 2, 1, -1, -2, -2, -1};
 
 //проверим, попадает ли целевая клетка в одну из допустимых
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
  addChessEvent(CET_TURN_ERROR, "недопустимый ход для коня", NULL);
  return false;
 }
 
 ChessShape* pCell = mOwner->getShapeAt(HCell, VCell);
 
 //если целевая клетка непуста...
 if(pCell!=NULL)
 {
	 if(pCell->getColor()!=mColor)
	 {
		 pCell->setActive(false);//вражеская фигура, "сьедаем её"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "это поле занято вашей фигурой", NULL);
		 return false; //своя фигура - ход недопустим
	 }
 }
 //снимаем себя с предыущей клетки
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //записываем новые координаты
 mCellH =  HCell;
 mCellV = VCell;
 //ставим себя на новою клетку
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

//слон: сделать ход
bool 
Slon::setNewCell(int HCell, int VCell)
{
 //первая проверка: проверяем вхождение новой ячейки в диапазон допустимых
 if(!checkRange(HCell, VCell)) return false;
 
 int vectorV = VCell - mCellV;
 int absV = (vectorV < 0) ? (vectorV*(-1)) :  vectorV;
 int stepV = (absV != NULL) ? (vectorV / absV) : 0;

 int vectorH = HCell - mCellH;
 int absH = (vectorH < 0) ? (vectorH*(-1)) :  vectorH;
 int stepH = (absH != NULL) ? (vectorH / absH) : 0;
 
 //признак того что фигура перемещаеться по вертикали
 //смещение по горизонтали и вертикали равно
 if(absV != absH)
 {
  addChessEvent(CET_TURN_ERROR, "слон может ходить только по диагонали", NULL);
  return false;
 }
 
 //проверим, чтобы на пути слона не стояло других фигур
 //проверка нужна в случае, если перемещение больше чем на одну клетку
 if(absH > 1)
 {
	 for(int i = 1; i < absH; i++)
     {
		 int h = mCellH + (i*stepH);
		 int v = mCellV + (i*stepV);

		 ChessShape* pCell = mOwner->getShapeAt(h, v);
		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "слон не может прыгать через другие фигуры", NULL);
			 return false;
         }
     }
 }
 //проверим не стоит ли на целевой клетке другой фигуры
 ChessShape* pShape = mOwner->getShapeAt(HCell, VCell);
 
 if(pShape!=NULL)
 {
	 if(pShape->getColor()!=mColor)
	 {
		 pShape->setActive(false); //вражеская фигура, "сьедаем её"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "это поле занято вашей фигурой", NULL);
		 return false; //своя фигура - ход недопустим
	 }
 }
 //снимаем себя с предыущей клетки
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //записываем новые координаты
 mCellH =  HCell;
 mCellV = VCell;
 //ставим себя на новою клетку
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

//ладья: сделать ход
bool 
Ladya::setNewCell(int HCell, int VCell)
{
 //первая проверка: проверяем вхождение новой ячейки в диапазон допустимых
 if(!checkRange(HCell, VCell)) return false;
 
 int vectorV = VCell - mCellV;
 int absV = (vectorV < 0) ? (vectorV*(-1)) :  vectorV;
 int stepV = (absV != 0) ? (vectorV / absV) : 0;

 int vectorH = HCell - mCellH;
 int absH = (vectorH < 0) ? (vectorH*(-1)) :  vectorH;
 int stepH = (absH != 0) ? (vectorH / absH) : 0;
 
 //ладья может двигаться либо по горизонтали
 //либо по вертикали
 bool bV = (absV > 0) && (absH == 0);
 bool bH = (absV == 0) && (absH > 0);
 if(!(bV || bH))
 {
	 addChessEvent(CET_TURN_ERROR, "ладья может двигаться только по горизонтали или по вертикали", NULL);
	 return false;
 }
 
 //проверим, чтобы на пути ладьи не стояло других фигур
 //проверка по горизонтали, имеет значение только если перемещение больше чем на одну клетку
 if(absH > 1)
 {
	 for(int i = 1; i < absH; i++)
	 {
		 int h = mCellH + (i*stepH);
		 int v = mCellV;

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "ладья не может перепрыгивать через другие фигуры", NULL);
			 return false;
         }
     }
 }

 //проверка по вертикали, имеет значение только если перемещение больше чем на одну клетку
 if(absV > 1)
 {
	 for(int i = 1; i < absV; i++)
	 {
		 int h = mCellH;
		 int v = mCellV + (i*stepV);

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "ладья не может перепрыгивать через другие фигуры", NULL);
			 return false;
         }
     }
 }

 //проверим не стоит ли на целевой клетке другой фигуры
 ChessShape* pShape = mOwner->getShapeAt(HCell, VCell);
 
 if(pShape!=NULL)
 {
	 if(pShape->getColor()!=mColor)
	 {
		 pShape->setActive(false); //вражеская фигура, "сьедаем её"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "это поле занято вашей фигурой", NULL);
		 return false; //своя фигура - ход недопустим
	 }
 }
 //снимаем себя с предыущей клетки
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //записываем новые координаты
 mCellH =  HCell;
 mCellV = VCell;
 //ставим себя на новою клетку
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

//ферзь: сделать ход
bool 
Ferz::setNewCell(int HCell, int VCell)
{
 //первая проверка: проверяем вхождение новой ячейки в диапазон допустимых
 if(!checkRange(HCell, VCell)) return false;
 
 int vectorV = VCell - mCellV;
 int absV = (vectorV < 0) ? (vectorV*(-1)) :  vectorV;
 int stepV = (absV != 0) ? (vectorV / absV) : 0;

 int vectorH = HCell - mCellH;
 int absH = (vectorH < 0) ? (vectorH*(-1)) :  vectorH;
 int stepH = (absH != 0) ? (vectorH / absH) : 0;
 
 //ферзь может двигаться либо по горизонтали,
 //вертикали и диагонали
 bool bV = (absV > 0) && (absH == 0);
 bool bH = (absV == 0) && (absH > 0);
 bool bD = (absV == absH);
 if(!(bV || bH || bD))
 {
  addChessEvent(CET_TURN_ERROR, "ферзь может двигаться только по диагонали, горизонтали или вертикали", NULL);
  return false;
 }
 
 //проверим, чтобы на пути ферзя не стояло других фигур
 //проверка на вертикальное перемещение
 if(bV && (absV > 1))
 {
	 for(int i = 1; i < absV; i++)
     {
		 int h = mCellH;
		 int v = mCellV + (i*stepV);

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "ферзь не может перепрыгивать через другие фигуры", NULL);
			 return false;
         }
     }
 }
 //проверка на горизонтальное перемещение
 if(bH && (absH > 1))
 {
	 for(int i = 1; i < absH; i++)
     {
		 int h = mCellH + (i*stepH);
		 int v = mCellV;

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "ферзь не может перепрыгивать через другие фигуры", NULL);
			 return false;
         }
     }
 }
 //проверка на диагональное перемещение
 if(bD && (absH > 1))
 {
	 for(int i = 1; i < absH; i++)
     {
		 int h = mCellH + (i*stepH);
		 int v = mCellV + (i*stepV);

		 ChessShape* pCell = mOwner->getShapeAt(h, v);

		 if(pCell!=NULL)
         {
			 addChessEvent(CET_TURN_ERROR, "ферзь не может перепрыгивать через другие фигуры", NULL);
			 return false;
         }
     }
 }

 //проверим не стоит ли на целевой клетке другой фигуры
 ChessShape* pShape = mOwner->getShapeAt(HCell, VCell);
 
 if(pShape!=NULL)
 {
	 if(pShape->getColor()!=mColor)
	 {
		 pShape->setActive(false); //вражеская фигура, "сьедаем её"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "это поле занято вашей фигурой", NULL);
		 return false; //своя фигура - ход недопустим
	 }
 }
 //снимаем себя с предыущей клетки
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //записываем новые координаты
 mCellH =  HCell;
 mCellV = VCell;
 //ставим себя на новою клетку
 mOwner->setShapeAt(HCell, VCell, this);
 
 notifyObservers();
 return true;
}

//король: сделать ход
bool 
Korol::setNewCell(int HCell, int VCell)
{
 //первая проверка: проверяем вхождение новой ячейки в диапазон допустимых
 if(!checkRange(HCell, VCell)) return false;

 //массив допустимых смещений по горизонтали и по вертикали
 int vOffset[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
 int hOffset[8] = {1, 1, 1, 0, -1, -1, -1, 0};
 
 //проверим, попадает ли целевая клетка в одну из допустимых
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
  addChessEvent(CET_TURN_ERROR, "недопустимый ход для короля", NULL);
  return false;
 }
 
 ChessShape* pCell = mOwner->getShapeAt(HCell, VCell);
 
 //если целевая клетка непуста...
 if(pCell!=NULL)
 {
	 if(pCell->getColor()!=mColor)
	 {
		 pCell->setActive(false); //вражеская фигура, "сьедаем её"
	 }else
	 {
		 addChessEvent(CET_TURN_ERROR, "это поле занято вашей фигурой", NULL);
		 return false; //своя фигура - ход недопустим
	 }
 }
 //снимаем себя с предыущей клетки
 mOwner->setShapeAt(mCellH, mCellV, NULL);
 //записываем новые координаты
 mCellH =  HCell;
 mCellV = VCell;
 //ставим себя на новою клетку
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
	 std::string message((getColor()==CC_WHITE) ? "король белых пал" : "король черных пал");
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
	//вариант первый
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

	//вариант второй
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

	//массив допустимых смещений по горизонтали и по вертикали
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

	//массив допустимых смещений по горизонтали и по вертикали
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
 
 //на две клетки вперед если это первый ход и две следующие клетки пусты
 if(mFirstStep && bSecondEmpty && bFirstEmpty)
 {
  turn.hto = getHCell();
  turn.vto = getVCell() + 2*stepV;
  turn.dest = CT_EMPTY;

  turnList.push_back(turn);
 }
 //на одну клетку вперед если следующая клетка пуста
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


 //массив допустимых смещений по горизонтали и по вертикали
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
 
 //массив допустимых смещений по горизонтали и по вертикали
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