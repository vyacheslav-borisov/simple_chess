
#include <stdafx.h>

#include "structs/singleton.h"
#include "graph.h"
#include "game_application.h"
#include "animation.h"
#include "gui/GUICommon.h"
#include "ai/ChessCommon.h"

#include "hcgraph.h"
#include "HomeChess.h"

#include "GameObject.h"

/*****************************************************************/
//  базовый игровой объект
/*****************************************************************/
BaseGameObject::BaseGameObject(ChessShape* pShape, Sprite* pParent)
{
    setParent(pParent);
    
    SCOORD scale = (SCOORD)(100.0f / 8.0f);
    //задать размеры спрайта (1/8 по длине и ширине от размеров
	//родительского элемента
    setSize(scale, scale);
	//задать положение на основе положения исходной фигуры на доске
    //(преобразование координат)
	SCOORD x = scale * pShape->getHCell();
	SCOORD y = scale * pShape->getVCell();

	setCoords(x, y);
    //задать состояние на основе состояния исходной фигуры
	if(pShape->isActive())
	{
		setState(SS_NEITRAL);
	}else
	{
		setState(SS_INVISIBLE);
	}
    //сгенерировать ИД на основе констант цвета и типа исходной фигуры
	SPRITEID id = TheGameHost.generateGameObjectID(pShape);
	setID(id);
    //зарегистрировать текущий обьект как спрайт в менеджере GUI
	TheGUI.addElement(pParent->getID(), static_cast<Sprite*>(this));
    //зарегистрировать текущий обьект как обработчик событий в менеджере GUI
	TheGUI.setHandler(getID(), static_cast<EventHandler*>(this));
}

BaseGameObject::~BaseGameObject()
{
   //вызываем метод для удаления (без вызова операции delete)
   //обработчика событий, являющегося приведением к подклассу EventHandler указателя 
   //на экземпляр данного класса
   //это нужно для предотвращения повторного удаления из памяти обьекта данного класса
   //через ссылку на EventHandler, поскольку перед этим обьект будет удален раньше через
   //ссылку на Sprite. Подводный камень множественного наслдедования(((
   TheGUI.removeHandler(getID()); 
}

//наведение курсора на элемент (onBlur)
void
BaseGameObject::onBlur(Sprite* sender, CURCOORD x, CURCOORD y, CURSTATE state)
{
  //если элемент находиться в нейтральном состоянии
  if(sender->getState() == SS_NEITRAL)
  {
     //перевести его в подсвеченое
     sender->setState(SS_HOVER);
  }
  EventHandler::onBlur(sender, x, y, state);
  
}

//потеря курсора элементом (onHower)
void 
BaseGameObject::onHower(Sprite* sender, CURCOORD x, CURCOORD y, CURSTATE state)
{
   //если элемент находиться в подсвеченом состоянии
   if(sender->getState() == SS_HOVER)
   {
     //перевести в нейтралное
     sender->setState(SS_NEITRAL);
   }
   EventHandler::onHower(sender, x, y, state); 
}

/*************************************************************************/
//  шахматная фигура игрока
/*************************************************************************/

PlayerChessShape::PlayerChessShape(ChessShape* pShape, Sprite* pParent):
 BaseGameObject(pShape, pParent), m_pChessShape(pShape), mPrevX(0.0f), mPrevY(0.0f)
{
  //зарегистрировать себя как наблюдателя у данной фигуры
  pShape->addObserver(static_cast<IChessShapeObserver*>(this));
}

 PlayerChessShape::~PlayerChessShape()
 {
   //удалить себя из наблюдателей
   m_pChessShape->removeObserver(static_cast<IChessShapeObserver*>(this));
 }

void 
PlayerChessShape::update(ChessShape* sender)
{
    //если исходная фигура стала нективной
	if(!sender->isActive())
	{
		//перейти в невидимое состояние
		setState(SS_INVISIBLE);
	}
    //задать положение спрайта в состоянии с положением фигуры на доске
	SCOORD scale = (SCOORD)(100 / 8.0f);
    
	SCOORD x = scale * sender->getHCell();
	SCOORD y = scale * sender->getVCell();

	setCoords(x, y);
}

//нажатие ЛКМ в пределах элемента (oLButtonDowm)
void 
PlayerChessShape::onLButtonDown(Sprite* sender, CURCOORD x, CURCOORD y)
{
    //получить текущее состояние игры
	GAMESTATE state = TheGameHost.getGameState();
    //если состоянии игры не являеться ожиданием хода игрока
	if(state != GS_WAIT_PLAYER_TURN)
	{
        //выход
		return;
	}
 
    //перейти в состояние "перетаскиваемое"
	setState(SS_DROPPED);
	//запомнить текущие координаты в переменных mPrevX и mPrevY
	mPrevX = getX();
	mPrevY = getY();
}

//отпуск ЛКМ в пределах элемента (onLButtonUp)
void 
PlayerChessShape::onLButtonUp(Sprite* sender, CURCOORD x, CURCOORD y)
{
    //если текущее состояние не "перетаскиваемое"
    if(getState() != SS_DROPPED)
    {
		//выход
		return;
    }

	//переход в "нейтральное" состояние
	setState(SS_NEITRAL);

    //преобразование текущих координат курсора и запомненных координат
    //в позицию на шахматной доске
    Sprite* pParent = getParent();

    SCOORD parentLeft = pParent->getX();
    SCOORD parentTop = pParent->getY();
	SCOORD parentWidth = pParent->getWidth();
	SCOORD parentHeight = pParent->getHeight();
    	
	SCOORD localTo_X = (SCOORD)x - parentLeft;
	SCOORD localTo_Y = (SCOORD)y - parentTop;
 	
	int chessBoardFrom_X = m_pChessShape->getHCell();
	int chessBoardFrom_Y = m_pChessShape->getVCell();
	int chessBoardTo_X = (int)floor((localTo_X * 8) / parentWidth);
	int chessBoardTo_Y = (int)floor((localTo_Y * 8) / parentHeight); 
	
	//вызов метода шахматной доски "сделать ход"
	bool bResult = TheGameHost.makeTurn(chessBoardFrom_X, chessBoardFrom_Y, 
		chessBoardTo_X, chessBoardTo_Y);
 
	//если ход правильный
	if(bResult)
	{
		//переместить фигуру на новую клетку
		SCOORD scale = (SCOORD)(100.0f / 8.0f);
        SCOORD newX = chessBoardTo_X * scale;
        SCOORD newY = chessBoardTo_Y * scale;

		setCoords(newX, newY);
		//задать текущим состояние игры "анализ хода игрока"
		TheGameHost.setGameState(GS_ANALIZ_PLAYER_TURN);
	}else
	{
      //если ход неправильный
      //вернуть фигуру на прежнее место
	  SCOORD scale = (SCOORD)(100.0f / 8.0f);
      SCOORD newX = chessBoardFrom_X * scale;
      SCOORD newY = chessBoardFrom_Y * scale;

	  setCoords(newX, newY);

	  //следующий фрагмент кода нужен для отдалки
	  //удалить его по окончании
	  /*
	  ChessEventQueye& eventQueye = TheGameHost.getTurnEventQueye();
	  while(!eventQueye.empty())
	  {
		  ChessEvent ce = eventQueye.front();
		  
		  if(ce._type == CET_TURN_ERROR)
		  {
			  MessageBoxA(NULL, ce._message.c_str(), "ошибка хода", MB_OK|MB_ICONEXCLAMATION);
		  }

		  eventQueye.pop();
	  }//while(!eventQueye.empty())
	  */
	}
 
}

//перемещение курсора мыши
void
PlayerChessShape::onCursorMove(Sprite* sender, CURCOORD x, CURCOORD y, CURSTATE state)
{
   //если фигура в перетаскиваемом состоянии
	if(sender->getState() == SS_DROPPED)
    {
		//вычислить и задать текущие координаты спрайта
        //так, чтобы его центр находился под курсором
        SCOORD localX = floor((SCOORD)x - (sender->getWidth() / 2));
        if(localX < 0) localX = 0;
         
        SCOORD localY = floor((SCOORD)y - (sender->getHeight() / 2));
        if(localY < 0) localY = 0;

        Sprite* pParent = sender->getParent();
        localX = ((localX - pParent->getX()) * 100) / pParent->getWidth();
        localY = ((localY - pParent->getY()) * 100) / pParent->getHeight();

        sender->setCoords(localX, localY);
    }
    EventHandler::onCursorMove(sender, x, y, state);      
}


/****************************************************************/
//       шахматная фигура противника
/****************************************************************/


AIChessShape::AIChessShape(ChessShape* pShape, Sprite* pParent):
 BaseGameObject(pShape, pParent), m_pChessShape(pShape)
{
  //зарегистрировать себя как наблюдателя у данной фигуры
  pShape->addObserver(static_cast<IChessShapeObserver*>(this));

  mPrevHCell = pShape->getHCell();
  mPrevVCell = pShape->getVCell();
}

AIChessShape::~AIChessShape()
{
  m_pChessShape->removeObserver(static_cast<IChessShapeObserver*>(this));
}

void 
AIChessShape::update(ChessShape* sender)
{
    //если исходная фигура стала нективной
	if(!sender->isActive())
	{
		//перейти в невидимое состояние
		setState(SS_INVISIBLE);
	}

	//если фигура изменила положение на доске
	if((sender->getHCell() != mPrevHCell) || (sender->getVCell() != mPrevVCell))
	{
		//задать анимацию перемещения фигуры
		//на новую клетку

        //запомнить новые координаты
        mPrevHCell = sender->getHCell();
        mPrevVCell = sender->getVCell();

		Sprite* pParent = getParent();
		SCOORD targetX = pParent->getX() + ((pParent->getWidth() / 8.0f) * sender->getHCell());
		SCOORD targetY = pParent->getY() + ((pParent->getHeight() / 8.0f) * sender->getVCell());
        
		Animator* pAnimator = new Animator(getMatrix());
        AnimationKey key1, key2;
        
		SCOORD selfWidth = getWidth();
		SCOORD selfHeight = getHeight();

        key1.setTime(0);
        key1.setScaling(selfWidth, selfHeight, 1.0f);
        key1.setTranslation(getX(), getY(), 0.0f);
        pAnimator->addAnimationKey(key1);
		
		//время анимации - одна секунда, можно будет подобрать другое значение
        key2.setTime(1000);
        key2.setScaling(selfWidth, selfHeight, 1.0f);
        key2.setTranslation(targetX, targetY, 0.0f);
        pAnimator->addAnimationKey(key2);
		
		//добавляем наблюдатель для получения сообщения об окончании анимации
		pAnimator->addObserver(static_cast<IAnimationObserver*>(this));

        TheGameHost.addAnimation(pAnimator);		
	}
}

//вызваеться по окончании анимации
void 
AIChessShape::update(Animator* sender)
{
  //задать координаты спрайта на новой клетке
  SCOORD scale = (SCOORD)(100.0f / 8.0f);
  SCOORD newX = m_pChessShape->getHCell() * scale;
  SCOORD newY = m_pChessShape->getVCell() * scale;

  setCoords(newX, newY);  
  //задать текущее состояние игры "анализ хода противника"
  TheGameHost.setGameState(GS_ANALIZ_AI_TURN);
}