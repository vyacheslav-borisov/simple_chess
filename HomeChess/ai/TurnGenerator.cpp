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
	{ CT_PSF, -1, -1 }, //1 �����, ����, �����
	{ CT_LFKr, -1, 0 }, //2 ������, �����, �����
	{ CT_SF, -1, 1 }, //3 ����, �����
	{ CT_LFKr, 0, 1 },//4 ������, �����, �����
	{ CT_SF, 1, 1 },//5 ����, �����
	{ CT_LFKr, 1, 0 },//6 ������, �����, �����
	{ CT_PSF, 1, -1 },//7 �����, ����, �����
	{ CT_LFKr, 0, -1 },//8 ������, �����, �����
	{ CT_KON, -2, -1 },// 2, ����
	{ CT_KON, -2, 1 },// 4, ����
	{ CT_KON, -1, 2 },// 6, ����
	{ CT_KON, 1, 2 },// 8, ����
	{ CT_KON, 2, 1 },// 10, ����
	{ CT_KON, 2, -1 },// 12, ����
	{ CT_KON, 1, -2 },// 14, ����
	{ CT_KON, -1, -2 }// 16, ����
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
 //����������� ��������� ���������� ����� � ���������� ��������
 //�������� ��������� �������������
 const int kMaxTurnCount = 140;
 destroy();
 //�������� ����������� ���� ������ ��� �������
 std::vector<TurnInfo> turns;
 turns.reserve(kMaxTurnCount);
 //��������� ������ ����� ��� ������ �����
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
 //������������� ��������� �� ���� �������
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
 //������������� ������ �� ������������ ���������
 std::sort(mAttackTurns.begin(), mAttackTurns.end(), attackTurnsSort);
 std::sort(mBadTurns.begin(), mBadTurns.end(), badTurnsSort);
 //�� ���� ������ ���� ��� � ������ ����������
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
 //��� ������ ������ ������ ��������������� �������
 //��������������, �������������, ������������ � �������������� ����
 TurnInfo manevr;
 bool bManevrTurn = false;
 for(std::vector<ChessShape*>::iterator it = mShapes->begin();
	 it != mShapes->end(); ++it)
 {
  ChessShape* shape = *it;
  //���� ������ �� ������ ��� �� �������� - ����������
  if((shape->getColor()!=CC_BLACK)||(!shape->isActive()))
	  continue;
  //���� ������ �� ���������� � ������� ����� - ����������
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
  if(chooseWardTurn(shape, manevr))
  {
      manevr._type = TURNTYPE_WARD;
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
 ������� ��������� ���.

 ������������� ������ �������������� �����, ������� � �������.
 ���� ��������� ������ �� �������� ������ �������, ��� ��� ������ ����
 ������� - �������� ���

 ������� ���������� true, ���� ��� ������ �  false, ���� ��� ������� �� �������
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
 ������� ����������� ���.
 
 ����������� ��� ����������� ��������� �������� �� ������
 ����������� �����
 
 ������� ���������� true, ���� ��� ������ �  false, ���� ��� ������� �� �������
*/
bool 
TurnGenerator::chooseNeitralTurn(TurnInfo& turn)
{

 srand(timeGetTime());
 if(mNeitralTurns.size())
 {
  //�������� ���������-���� ��� ����� ��������� � ��������� ����������
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
 ������� ������ ���.

 �������� ������ ��� �� ���������������� ������ ������ �����

 ������� ���������� true, ���� ��� ������ �  false, ���� ��� ������� �� �������
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
 ������� ��������������� ���.

 ������������� ������ ����� ���������� �����. ���� ��������������� ������ ����������
 � "�������" �����, ������������� ������ � ������. ��������� �� ��� ��� ���� 
 �� �������� ��� �� ������ ����������� � ����� ��� ���� ������ �� ��������� ������������

 ������� ���������� true, ���� ��� ������ �  false, ���� ��� ������� �� �������
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
 ������� ����������� ���.

 ������������� ������ ����������� �����, ������� � �������.
 �������� ������ ��� ��� ����� ������

 ������� ���������� true, ���� ��� ������ �  false, ���� ��� ������� �� �������
*/
bool 
TurnGenerator::chooseEscapeTurn(ChessShape* shape, TurnInfo& turn)
{
 //������ ��� �������� ���-�����, ����� ����������� ������ ���������� �����
 HOTPOINT& hp = mWhiteHP.getHotPoint(shape->getHCell(), shape->getVCell());
 if(hotPointIsEmpty(hp))
 {
     //������ �� ��� �������, �������
     return false;
 }
 for(HOTPOINT_ITERATOR it = hp.begin();
	 it!= hp.end(); ++it)
 {
     //��������� ���������� ������:
     //���� ����� ��� ���� �����, ����� ��� ����
     //�� ������ ���������, �� ��������� �� �� �� ��� ������
     //����� ������ �����
     TurnInfo info = *it;
     if((info.src == CT_SLON) || (info.src == CT_LADYA) || (info.src == CT_FERZ))
     {
         int hVector = info.hto - info.hfrom;
         int vVector = info.vto - info.vfrom;
         hVector = (hVector != 0) ? (hVector / abs(hVector)) : 0;
         vVector = (vVector != 0) ? (vVector / abs(vVector)) : 0;
         
         //����� ��� �������������� ������������ �����
         //� �� ���� �� ���....
         if((hVector == 0) && (vVector == 0)) continue;

         int i = 1;
         bool bContinue = true;
         while(TRUE)
         {
             int h = info.hto + (i*hVector);
             int v = info.vto + (i*vVector);

             if((h < 0) || (h >= BOARD_WIDTH)) break;
             if((v < 0) || (v >= BOARD_HEIGHT)) break;
             
             ChessShape* pShape = TheGameBoard.getShapeAt(h, v);
             if(pShape != NULL)
             {
                 if(pShape->getType() > shape->getType())
                 {
                     //������������ ������ ����� �������� ����� - �������
                     return false;
                 }else
                 {
                     //����� ����� ������ - ��������� �������� ����
                     bContinue = false;
                 }
             }
             i++;
         }//while(TRUE)
              
         if(!bContinue) break;
     }
 }//for(HOTPOINT_ITERATOR it = hp.begin();
 
 //��� ������ ��������� ������� �� ���������� �����
 if(mAttackTurns.size())
 {
  for(TURNLIST_ITERATOR it = mAttackTurns.begin();
	 it!= mAttackTurns.end(); ++it)
  {
	  TurnInfo info = *it;
	  if((info.hfrom == shape->getHCell()) && (info.vfrom == shape->getVCell()))
	  {
          HOTPOINT& whp = mWhiteHP.getHotPoint(info.hto, info.vto);
          if(hotPointIsEmpty(whp))
          {
              //�������������� ������� ��������
              /*
              bool bContinue = false;
              for(HOTPOINT_ITERATOR iit = hp.begin();
	              iit!= hp.end(); ++iit)
              {
                  TurnInfo ti = *iit;
                  if((ti.src == CT_SLON) || (ti.src == CT_LADYA) || (CT_FERZ))
                  {
                      int hNormal1 = ti.hto - ti.hfrom;
                      int vNormal1 = ti.vto - ti.vfrom;
                      hNormal1 = (hNormal1 != 0) ? (hNormal1 / abs(hNormal1)) : 0;
                      vNormal1 = (vNormal1 != 0) ? (vNormal1 / abs(vNormal1)) : 0;

                      int hNormal2 = info.hto - ti.hfrom;
                      int vNormal2 = info.vto - ti.vfrom;
                      hNormal2 = (hNormal2 != 0) ? (hNormal2 / abs(hNormal2)) : 0;
                      vNormal2 = (vNormal2 != 0) ? (vNormal2 / abs(vNormal2)) : 0;

                      if((hNormal1 == hNormal2) && (vNormal1 == vNormal2)) bContinue = true;
                  }
              }
              if(bContinue) continue;
              */
              turn = info;
              return true;
          }
	  }
  }//for(std::vector<TurnInfo>::iterator it = mNeitralTurns.begin();
 }//if(mNeitralTurns.size())

 //���� ��� ����������� ��������� - �������� �� ����������� �����
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
              //�������������� ������� ��������
              /*
              bool bContinue = false;
              for(HOTPOINT_ITERATOR iit = hp.begin();
	              iit!= hp.end(); ++iit)
              {
                  TurnInfo ti = *iit;
                  if((ti.src == CT_SLON) || (ti.src == CT_LADYA) || (CT_FERZ))
                  {
                      int hNormal1 = ti.hto - ti.hfrom;
                      int vNormal1 = ti.vto - ti.vfrom;
                      hNormal1 = (hNormal1 != 0) ? (hNormal1 / abs(hNormal1)) : 0;
                      vNormal1 = (vNormal1 != 0) ? (vNormal1 / abs(vNormal1)) : 0;

                      int hNormal2 = info.hto - ti.hfrom;
                      int vNormal2 = info.vto - ti.vfrom;
                      hNormal2 = (hNormal2 != 0) ? (hNormal2 / abs(hNormal2)) : 0;
                      vNormal2 = (vNormal2 != 0) ? (vNormal2 / abs(vNormal2)) : 0;

                      if((hNormal1 == hNormal2) && (vNormal1 == vNormal2)) bContinue = true;
                  }
              }
              if(bContinue) continue;
              */
              turn = info;
              return true;
          }
	  }
  }//for(std::vector<TurnInfo>::iterator it = mNeitralTurns.begin();
 }//if(mNeitralTurns.size())
 return false;
}

/*
 ������� ������������ ���.

 ����������� � ������ ���� � ������ ��� ��������� � ���������� ������
 ��� ������ ���� ��� ����� ��������. ������������� ������ ������ ������,
 ���������� ���������� ������. ��� ����������� ������� ����� ��� ������,
 ������������� ���� � ������� ��� �����, ������� ����� �� � ���� ������ ��������
 �������� ������. ������� ����� ��� ������ �� ������ ���� ��� ���� ������� 
 ������ ��� ����� �����

 ������� ���������� true, ���� ��� ������ �  false, ���� ��� ������� �� �������
*/
bool 
TurnGenerator::chooseUmbrellaTurn(ChessShape* shape, TurnInfo& turn)
{
 HOTPOINT& selfhp = mBlackHP.getHotPoint(shape->getHCell(), shape->getVCell());
 //������, ���� ������ ��� ����������
 bool bProtected = (!hotPointIsEmpty(selfhp));
 bool bRangeGreater = true;
 
 HOTPOINT& wselfhp = mWhiteHP.getHotPoint(shape->getHCell(), shape->getVCell());
 for(HOTPOINT_ITERATOR it = wselfhp.begin(); it!= wselfhp.end(); ++it)
 {
     //���� ���� �� ���� �� ���������� �����
     //������ ���� �������� - ��������� ��� �� �������
     //����� ������ ����� ���������� �� ���� ������� ����
	 if(it->src < shape->getType())
		 bRangeGreater = false;
 }
 //���� ������ ��� ���������� � ��� ���������� ������
 //������ ���� ��� ����� ��������, �� ��� ������������� � �������������
 //������� ����
 if(bProtected && bRangeGreater)
	 return false;
 //���� ���� �� ���� �� ���������� �����
 //������ ���� �������� - ��������� ��� �� �������
 //����� ������ ����� ���������� �� ���� ������� ����
 if(!bRangeGreater)
	 return false;
 
 //���� �� ����� �� ���� �����, ������ � ��� ��� ���������
 //� ��� ���������� ������ ������ ���� ��� ����� ��������
 //������������� ����� ����� �������� ���
 for(int i = 0; i < PROTECT_POINT_COUNT; i++)
 {
  //ProtectPointMap - ����� ��������, � �������� ������
  //������������ ������ ������
  //� ������ ������� ������, ������ 
  //����� ���������� ������ ������������� ����, ������� �����
  //�� ��������� ���� ������ �� ������, �� ������� ����������
  //������ ������ 
  ProtectPoint pp = ProtectPointMap[i];
  int h = shape->getHCell() + pp.hoffset;
  int v = shape->getVCell() + pp.voffset;

  //���� ������� ������ �������� �� ������� ����� - �������.
  if((h < 0)||(h >= BOARD_WIDTH))
	  continue;
  if((v < 0)||(v >= BOARD_HEIGHT))
	  continue;

  HOTPOINT& bhp = mBlackHP.getHotPoint(h, v); //������ ������� �����
  HOTPOINT& whp = mWhiteHP.getHotPoint(h, v); //����� ������� �����
  //���� ������ ��������� ������� ������ ��� ������
  //� ��� ���� �� ��������� ������� ������ ��� �����
  if((!hotPointIsEmpty(bhp)) && (hotPointIsEmpty(whp)))
  {
   ChessShape* pShape =  TheGameBoard.getShapeAt(h, v);
   if(pShape != NULL)
   {
       //���� ������� ������ ������ ����� �� �������
       //���������� �.
       if(pShape->getColor() == CC_BLACK) continue;
   }
   //������������� ��� ��������� ���� � ��� ������ ��� ������ �����
   for(HOTPOINT_ITERATOR it = bhp.begin(); it!= bhp.end(); ++it)
   {
	TurnInfo info = *it;
    //���� �������� ������ ��� ������� ���� �������� �� ����
    //"����������" ��� ������ ����� - �������� ������ ��� � �������
	if(info.src && pp.shapes)
	{
        //����������� ������:
        //���� �������� ������ - �����, � �� ������� ������ ��� ����� ������
        //��� �� ��������
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
 ������� ����������� ���.

 ������������� ������ ���������� �����, � ������� ��� 
 ��� ���������� � ������� ������ ��� ������. �������� ��� �� �������� ������ ��������������
 ���� - ���� ������ �� ��� ���������� ��� ������ ���� ��� ��������.

 ������� ���������� true, ���� ��� ������ �  false, ���� ��� ������� �� �������
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

/*
 ��� ��� ����� �����, ���� ���������� ������ ���� � ��� ���������
 ������ ������ ��� ������ 
*/
bool 
TurnGenerator::chooseWardTurn(ChessShape* shape, TurnInfo& turn)
{
    HOTPOINT& wselfhp = mWhiteHP.getHotPoint(shape->getHCell(), shape->getVCell());
    if((wselfhp.size() > 1) || (wselfhp.size() == 0))
    {
        return false;
    }
    TurnInfo ti = wselfhp[0];

    if((ti.src != CT_SLON) && (ti.src != CT_LADYA) && (ti.src != CT_FERZ))
    {
        return false;
    }

    int hNormVector = ti.hto - ti.hfrom;
    int vNormVector = ti.vto - ti.vfrom;
    hNormVector = (hNormVector != 0) ? (hNormVector / abs(hNormVector)) : 0;
    vNormVector = (vNormVector != 0) ? (vNormVector / abs(vNormVector)) : 0;

    if((hNormVector == 0) && (vNormVector == 0))
    {
        return false;
    }
    
    std::vector<TurnInfo> wardTurns;
    int i = 0;
    while(TRUE)
    {
        int h = ti.hfrom + (i*hNormVector);
        int v = ti.vfrom + (i*vNormVector);
        
        if((h < 0) || (h >= BOARD_WIDTH)) break;
        if((v < 0) || (v >= BOARD_HEIGHT)) break;
        if((h == shape->getHCell()) && (v == shape->getVCell())) break;

        HOTPOINT bhp = mBlackHP.getHotPoint(h, v);
        for(HOTPOINT_ITERATOR it = bhp.begin(); it != bhp.end(); ++it)
        {
            TurnInfo info = *it;
            if(info.src >= shape->getType()) continue;
            //������������ ������ - ����� �� ����� ����� ������ �� ������
            if((i == 0) && (info.src == CT_PESHKA) && (h == info.hfrom)) continue;
            if(info.src == CT_PESHKA)
            {
                ChessShape* pShape = TheGameBoard.getShapeAt(info.hto, info.vto);
                if(pShape == NULL) continue;
                if(pShape->getColor() == CC_BLACK) continue; 
            }
            wardTurns.push_back(info);
        }

        i++;
    }

    if(wardTurns.size() > 0)
    {
        sort(wardTurns.begin(), wardTurns.end(), badTurnsSort);
        turn = wardTurns[0];
        return true;
    }

    return false;
}

