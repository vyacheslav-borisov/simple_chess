#include "stdafx.h"
#include "../structs/singleton.h"
#include "ChessCommon.h"

using namespace std;



struct ProtectPoint
{
 ChessType shapes;
 int hoffset;
 int voffset;
};

const int PROTECT_POINT_COUNT = 16;
ProtectPoint ProtectPointMap[PROTECT_POINT_COUNT] =
{
	{ CT_PSF, -1, -1 }, //1 пешка, слон, ферзь
	{ CT_LFKr, -1, 0 }, //2 король, ладья, ферзь
	{ CT_SF, -1, 1 }, //3 слон, ферзь
	{ CT_LFKr, 0, 1 },//4 король, ладья, ферзь
	{ CT_SF, 1, 1 },//5 слон, ферзь
	{ CT_LFKr, 1, 0 },//6 король, ладья, ферзь
	{ CT_PSF, 1, -1 },//7 пешка, слон, ферзь
	{ CT_LFKr, 0, -1 },//8 король, ладья, ферзь
	{ CT_KON, -2, -1 },// 2, конь
	{ CT_KON, -2, 1 },// 4, конь
	{ CT_KON, -1, 2 },// 6, конь
	{ CT_KON, 1, 2 },// 8, конь
	{ CT_KON, 2, 1 },// 10, конь
	{ CT_KON, 2, -1 },// 12, конь
	{ CT_KON, 1, -2 },// 14, конь
	{ CT_KON, -1, -2 }// 16, конь
};

typedef std::vector<TurnInfo> TURNLIST, HOTPOINT;
typedef std::vector<TurnInfo>::iterator TURNLIST_ITERATOR, HOTPOINT_ITERATOR;

typedef std::vector<ChessShape*> SHAPELIST;
typedef std::vector<ChessShape*>::iterator SHAPELIST_ITERATOR;

bool hotPointIsEmpty(HOTPOINT& hp)
{
	return (hp.size() == 0);
}


bool attackTurnsSort(const TurnInfo& a, const TurnInfo& b)
{
    if(a.dest < b.dest) return false;
    if(a.dest > b.dest) return true;
    if(a.src < b.src) return true;
    if(a.src > b.src) return false;

    return false;
}

bool badTurnsSort(const TurnInfo& a, const TurnInfo& b)
{
	return a.src < b.src; 
}

ChessShape* g_pShape = NULL;
bool neitralTurnsSort(const TurnInfo& a, const TurnInfo& b)
{
    if(g_pShape == NULL)
    {
        return false;
    }
    int aVerticalRange = abs(g_pShape->getVCell() - a.vto);
    int bVerticalRange = abs(g_pShape->getVCell() - b.vto);
    if(aVerticalRange < bVerticalRange) return true;
    if(aVerticalRange > bVerticalRange) return false;

    int aHorizontalRange = abs(g_pShape->getHCell() - a.hto);
    int bHorizontalRange = abs(g_pShape->getHCell() - b.hto);
    if(aHorizontalRange < bHorizontalRange) return true;
    if(aHorizontalRange > bHorizontalRange) return false;
/*
	if(a.hfrom < b.hfrom) return true;
	if(a.hfrom > b.hfrom) return false;
	if(a.vfrom < b.vfrom) return true;
	if(a.vfrom > b.vfrom) return false;
*/	
    return false;
}



TurnGenerator::TurnGenerator(void)
{
	mShapes = NULL;
	srand((unsigned)time(NULL));
}

TurnGenerator::~TurnGenerator(void)
{
 destroy();
}

TurnGenerator::TurnGenerator(std::vector<ChessShape*>* pShapes)
{
 init(pShapes);
 srand((unsigned)time(NULL));
}

void 
TurnGenerator::init(std::vector<ChessShape*>* pShapes)
{
 if(!pShapes) return;
 mShapes = pShapes;
 //максимально возможное количество ходов в конкретной ситуации
 //величина вычислена математически
 const int kMaxTurnCount = 140;
 destroy();
 //выделить необходимый обем памяти для списков
 std::vector<TurnInfo> turns;
 turns.reserve(kMaxTurnCount);
 //составить список ходов для черных фигур
 for(std::vector<ChessShape*>::iterator it = mShapes->begin();
	 it!= mShapes->end(); ++it)
 {
	 ChessShape* pShape = *it;
	 if(!pShape->isActive())
		 continue;
	 if(pShape->getColor()==CC_BLACK)
	 {
	  pShape->getTurnList(turns);
	  pShape->findHotPoint(mBlackHP);
	 }else
	 {
	  pShape->findHotPoint(mWhiteHP);
	 }
 }
 //отсортировать результат по трем спискам
 mAttackTurns.reserve((kMaxTurnCount >> 2));
 mNeitralTurns.reserve((kMaxTurnCount >> 2));
 mBadTurns.reserve((kMaxTurnCount >> 2));

 for(std::vector<TurnInfo>::iterator it = turns.begin();
	 it!=turns.end(); ++it)
 {
  TurnInfo turn = *it;
  if(turn.dest!=CT_EMPTY)
  {
   mAttackTurns.push_back(turn);
   continue;
  }
  std::vector<TurnInfo>& hp = mWhiteHP.getHotPoint(turn.hto, turn.vto);
  if(!hp.size())
	  mNeitralTurns.push_back(turn);
  else
	  mBadTurns.push_back(turn);
 }
 //отсортировать списки по определенным критериям
 std::sort(mAttackTurns.begin(), mAttackTurns.end(), attackTurnsSort);
 std::sort(mBadTurns.begin(), mBadTurns.end(), badTurnsSort);
 //не вижу смысла пока что в данной сортировке
 g_pShape = TheGameBoard.findShape(CT_KOROL, CC_WHITE);
 std::sort(mNeitralTurns.begin(), mNeitralTurns.end(), neitralTurnsSort);
}

void 
TurnGenerator::destroy()
{
	mWhiteHP.clearCells();
	mBlackHP.clearCells();
	if(!mAttackTurns.empty()) mAttackTurns.clear();
	if(!mNeitralTurns.empty()) mNeitralTurns.clear();
	if(!mBadTurns.empty()) mBadTurns.clear();
}


bool
TurnGenerator::generateTurn(TurnInfo& turn)
{
 //для каждой черной фигуры последовательно выберем
 //наступательный, маневрирующий, прикрывающий и устранительный ходы
 TurnInfo manevr;
 bool bManevrTurn = false;
 for(std::vector<ChessShape*>::iterator it = mShapes->begin();
	 it != mShapes->end(); ++it)
 {
  ChessShape* shape = *it;
  //если фигура не черная или не активная - продолжаем
  if((shape->getColor()!=CC_BLACK)||(!shape->isActive()))
	  continue;
  //если фигура не находиться в горячей точке - продолжаем
  HOTPOINT& hp = mWhiteHP.getHotPoint(shape->getHCell(), shape->getVCell());
  if(hotPointIsEmpty(hp))
  {
      continue;
  }

  if(chooseContrTurn(shape, manevr))
  {
       manevr._type = TURNTYPE_CONTR;
       bManevrTurn = true;
       break;
  }
  if(chooseEscapeTurn(shape, manevr))
  {
      manevr._type = TURNTYPE_ESCAPE;
      bManevrTurn = true;
      break;
  }
  if(chooseUmbrellaTurn(shape, manevr))
  {
      manevr._type = TURNTYPE_UMBRELLA;
      bManevrTurn = true;
      break;
  }
  if(chooseProtectTurn(shape, manevr))
  {
      manevr._type = TURNTYPE_PROTECT;
      bManevrTurn = true;
      break;
  }
 }//for(std::vector<ChessShape*>::iterator it = mShapes->begin();
 TurnInfo attack, neitral, bad, mvr;
 bool bAttackTurn = chooseAttackTurn(attack);
 attack._type = TURNTYPE_ATTACK;
 bool bNeitralTurn = chooseNeitralTurn(neitral);
 neitral._type = TURNTYPE_NEITRAL;
 bool bBadTurn = chooseBadTurn(bad);
 bad._type = TURNTYPE_BAD;
 
 
     if(bManevrTurn)
     {
            if(bAttackTurn)
            {
               if(attack.dest > manevr.src)
               {
                   turn = attack;
                   return true;
               }
             }//if(bAttackTurn)

             turn = manevr;
             return true;
     }//if(bManevrTurn)

     if(bAttackTurn)
     {
         turn = attack;
         return true;
     }
     if(bNeitralTurn)
     {
         turn = neitral;
         return true;
     }
     if(bBadTurn)
     {
         turn = bad;
         return true;
     }

 return false;
}

/*
 Выбрать атакующий ход.

 Просматриваем список наступательных ходов, начиная с первого.
 Если сьедаемая фигура не прикрыта другой фигурой, или она рангом выше
 ходящей - выбираем ход

 функция возвращает true, если ход выбран и  false, если ход выбрать не удалось
*/
bool 
TurnGenerator::chooseAttackTurn(TurnInfo& turn)
{
 for(TURNLIST_ITERATOR it = mAttackTurns.begin();
		it!= mAttackTurns.end(); ++it)
 {
  TurnInfo info = *it;
  HOTPOINT& hp = mWhiteHP.getHotPoint(info.hto, info.vto);
  if(hotPointIsEmpty(hp))
  {
   turn = info;
   return true;
  }
  if(info.src < info.dest)
  {
   turn = info;
   return true;
  }
 }
 return false;
}

/*
 Выбрать нейтральный ход.
 
 Нейтральный ход выбираеться случайным способом из списка
 нейтральных ходов
 
 функция возвращает true, если ход выбран и  false, если ход выбрать не удалось
*/
bool 
TurnGenerator::chooseNeitralTurn(TurnInfo& turn)
{

 srand(timeGetTime());
 if(mNeitralTurns.size())
 {
  //написать программу-тест для этого выражения и проверить результаты
  int index = (double)rand() / (RAND_MAX + 1) * (mNeitralTurns.size());
  turn = mNeitralTurns[index];
  return true;
 }
/*
 if(mNeitralTurns.size())
 {
     turn = mNeitralTurns[0];
     return true;
 }
*/ 
 return false;
}

/*
 Выбрать плохой ход.

 Выбираем первый ход из отсортированного списка плохих ходов

 функция возвращает true, если ход выбран и  false, если ход выбрать не удалось
*/
bool 
TurnGenerator::chooseBadTurn(TurnInfo& turn)
{
 if(mBadTurns.size())
 {
  turn = mBadTurns[0];
  return true;
 }
 return false;
}

/*
 Выбрать предупреждающий ход.

 Просматриваем список ходов угрожающих фигур. если просматриваемая фигура находиться
 в "горячей" точке, просматриваем список её врагов. Повторяем до тех пор пока 
 не встретим ход от фигуры совпадающей с нашей или пока списки не окажуться изчерпанными

 функция возвращает true, если ход выбран и  false, если ход выбрать не удалось
*/
bool 
TurnGenerator::chooseContrTurn(ChessShape* shape, TurnInfo& turn)
{
 HOTPOINT& selfhp = mWhiteHP.getHotPoint(shape->getHCell(), shape->getVCell());
 
 for(HOTPOINT_ITERATOR it = selfhp.begin();
	 it!= selfhp.end(); ++it)
 {
  TurnInfo enemy = *it;
  HOTPOINT& bhp = mBlackHP.getHotPoint(enemy.hfrom, enemy.vfrom);
  HOTPOINT& whp = mWhiteHP.getHotPoint(enemy.hfrom, enemy.vfrom);
  bool bProtected = (whp.size() > 0);

  for(HOTPOINT_ITERATOR iit = bhp.begin();
	 iit!= bhp.end(); ++iit)
  {
   TurnInfo info = *iit;
   if((info.hfrom == shape->getHCell()) && (info.vfrom == shape->getVCell()))
   {
	bool bRangeGreater = (info.src < info.dest);
	if(bRangeGreater || (!bProtected))
	{
     turn = info;
	 return true;
	}//if(bRangeGreater || (!bProtected))
   }//if((info.hfrom == shape->getHCell()) && (info.vfrom == shape->getVCell()))
  }//for(std::vector<TurnInfo>::iterator iit = bhp.begin(...
 }//for(std::vector<TurnInfo>::iterator it = selfhp.begin();

 return false;
}

/*
 Выбрать маневренный ход.

 Просматриваем список нейтральных ходов, начиная с первого.
 Выбираем первый ход для нашей фигуры

 функция возвращает true, если ход выбран и  false, если ход выбрать не удалось
*/
bool 
TurnGenerator::chooseEscapeTurn(ChessShape* shape, TurnInfo& turn)
{
 if(mNeitralTurns.size())
 {
  for(TURNLIST_ITERATOR it = mNeitralTurns.begin();
	 it!= mNeitralTurns.end(); ++it)
  {
	  TurnInfo info = *it;
	  if((info.hfrom == shape->getHCell()) && (info.vfrom == shape->getVCell()))
	  {
          HOTPOINT& whp = mWhiteHP.getHotPoint(info.hto, info.vto);
          if(hotPointIsEmpty(whp))
          {
              turn = info;
              return true;
          }
	  }
  }//for(std::vector<TurnInfo>::iterator it = mNeitralTurns.begin();
 }//if(mNeitralTurns.size())
 return false;
}

/*
 Выбрать прикрывающий ход.

 Выбираеться в случае если у фигуры нет прикрытия а угрожающие фигуры
 все рангом выше или равны исходной. Просматриваем клетки вокруг фигуры,
 постепенно увеличивая радиус. При обнаружении горячей точки для черных,
 просматриваем ходы в посиках тех фигур, которые могли бы с этой клетки защитить
 исходную фигуру. Горячая точка для черных не должна быть при этом горячей 
 точкой для белых фигур

 функция возвращает true, если ход выбран и  false, если ход выбрать не удалось
*/
bool 
TurnGenerator::chooseUmbrellaTurn(ChessShape* shape, TurnInfo& turn)
{
 HOTPOINT& selfhp = mBlackHP.getHotPoint(shape->getHCell(), shape->getVCell());
 //истина, если фигура под прикрытием
 bool bProtected = (!hotPointIsEmpty(selfhp));
 bool bRangeGreater = true;
 
 HOTPOINT& wselfhp = mWhiteHP.getHotPoint(shape->getHCell(), shape->getVCell());
 for(HOTPOINT_ITERATOR it = wselfhp.begin(); it!= wselfhp.end(); ++it)
 {
     //если хотя бы одна из угрожающих фигур
     //рангом ниже исходной - прикрытие нам не поможет
     //игрок скорее всего пожертвует ей чтоб срубить нашу
	 if(it->src < shape->getType())
		 bRangeGreater = false;
 }
 //если фигура под прикрытием и все угрожающие фигуры
 //рангом выше или равны исходной, то нет необходимости в генерировании
 //данного хода
 if(bProtected && bRangeGreater)
	 return false;
 //если хотя бы одна из угрожающих фигур
 //рангом ниже исходной - прикрытие нам не поможет
 //игрок скорее всего пожертвует ей чтоб срубить нашу
 if(!bRangeGreater)
	 return false;
 
 //если мы дошли до этой точки, значит у нас нет прикрытия
 //а все угрожающие фигуры рангом выше или равны исходной
 //следовательно имеет смысл поискать его
 for(int i = 0; i < PROTECT_POINT_COUNT; i++)
 {
  //ProtectPointMap - карта смещений, в соседние клетки
  //относительно данной фигуры
  //с каждой целевой клетки, фигуру 
  //могут прикрывать фигуры определенного типа, которые могут
  //на следующем ходу занять ту клетку, на которой находиться
  //данная фигура 
  ProtectPoint pp = ProtectPointMap[i];
  int h = shape->getHCell() + pp.hoffset;
  int v = shape->getVCell() + pp.voffset;

  //если целевая клетка выходить за пределы доски - выходим.
  if((h < 0)||(h >= BOARD_WIDTH))
	  continue;
  if((v < 0)||(v >= BOARD_HEIGHT))
	  continue;

  HOTPOINT& bhp = mBlackHP.getHotPoint(h, v); //черная горячая точка
  HOTPOINT& whp = mWhiteHP.getHotPoint(h, v); //белая горячая точка
  //если клетка являеться горячей точкой для черных
  //и при этом не являеться горячей точкой для белых
  if((!hotPointIsEmpty(bhp)) && (hotPointIsEmpty(whp)))
  {
   ChessShape* pShape =  TheGameBoard.getShapeAt(h, v);
   if(pShape != NULL)
   {
       //если целевая клетка занята нашей же фигурой
       //пропускаем её.
       if(pShape->getColor() == CC_BLACK) continue;
   }
   //просматриваем все возможные ходы в эту клетку для черных фигур
   for(HOTPOINT_ITERATOR it = bhp.begin(); it!= bhp.end(); ++it)
   {
	TurnInfo info = *it;
    //если исходная фигура для данного хода подходит на роль
    //"протектора" для данной точки - выбираем данный ход и выходим
	if(info.src && pp.shapes)
	{
        //специальный случай:
        //если исходная фигура - пешка, а на целевой клетке нет белой фигуры
        //ход не подходит
        if(info.src == CT_PESHKA)
        {
            if(pShape == NULL) continue;
            if(pShape->getColor() == CC_BLACK) continue;
        }
		turn = info;
		return true;
	}
   }//for(HOTPOINT_ITERATOR it = bhp.begin(); it!= bhp.end(); ++it)
  }//if((!hotPointIsEmpty(bhp)) && (hotPointIsEmpty(whp)))
 }
 return false;
}

/*
 Выбрать устраняющий ход.

 Просматриваем список угрожающих фигур, в поисках тех 
 что находяться в горячих точках для черных. Выбираем ход по приницпу выбора настпательного
 хода - если фигура не под прикрытием или рангом ниже чем исходная.

 функция возвращает true, если ход выбран и  false, если ход выбрать не удалось
*/
bool 
TurnGenerator::chooseProtectTurn(ChessShape* shape, TurnInfo& turn)
{
 HOTPOINT& wselfhp = mWhiteHP.getHotPoint(shape->getHCell(), shape->getVCell());
 
 for(HOTPOINT_ITERATOR it = wselfhp.begin(); it != wselfhp.end(); ++it)
 {
  HOTPOINT& bhp = mBlackHP.getHotPoint(it->hfrom, it->vfrom);
  
  if(hotPointIsEmpty(bhp)) continue;
  
  HOTPOINT& whp = mWhiteHP.getHotPoint(it->hfrom, it->vfrom);
  bool bProtected = (!hotPointIsEmpty(whp));
  
  for(HOTPOINT_ITERATOR iit = bhp.begin(); iit != bhp.end(); ++iit)
  {
   TurnInfo info = *iit;
   bool bRangeGreater = (info.src < info.dest);
   if((!bProtected)||(bRangeGreater))
   {
	   turn = info;
	   return true;
   }//if((!bProtected)||(bRangeGreater))
  
  }//for(HOTPOINT_ITERATOR iit = bhp.begin(); iit != bhp.end(); ++iit)
 
 }//for(HOTPOINT_ITERATOR it = wselfhp.begin(); it != wselfhp.end(); ++it)
 return false;
}

