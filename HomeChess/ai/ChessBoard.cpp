#include "stdafx.h"
#include "../structs/singleton.h"
#include "ChessCommon.h"

using namespace std;

//функция сортировки 
//возращает true, если a < b и false, если a >= b

bool shapeSortFunc(ChessShape* a, ChessShape* b)
{
    if(a->getColor() < b->getColor())
    {
        return false;
    }
    if(a->getColor() > b->getColor())
    {
        return true;
    }

    if(a->getType() < b->getType())
    {
        return false;
    }

    if(a->getType() > b->getType())
    {
        return true;
    }

    return false;
}

const StartData ChessBoard::sDefaultInitalData[TOTAL_SHAPES] = 
{
	{CC_BLACK, CT_PESHKA, 0, 1, true},
	{CC_BLACK, CT_PESHKA, 1, 1, true},
	{CC_BLACK, CT_PESHKA, 2, 1, true},
	{CC_BLACK, CT_PESHKA, 3, 1, true},
	{CC_BLACK, CT_PESHKA, 4, 1, true},
	{CC_BLACK, CT_PESHKA, 5, 1, true},
	{CC_BLACK, CT_PESHKA, 6, 1, true},
	{CC_BLACK, CT_PESHKA, 7, 1, true},

	{CC_BLACK, CT_LADYA, 0, 0, true},
	{CC_BLACK, CT_KON,   1, 0, true},
    {CC_BLACK, CT_SLON,  2, 0, true},
	{CC_BLACK, CT_KOROL, 3, 0, true},
	{CC_BLACK, CT_FERZ,  4, 0, true},
    {CC_BLACK, CT_SLON,  5, 0, true},
	{CC_BLACK, CT_KON,   6, 0, true},
    {CC_BLACK, CT_LADYA, 7, 0, true},


	{CC_WHITE, CT_PESHKA, 0, 6, true},
	{CC_WHITE, CT_PESHKA, 1, 6, true},
	{CC_WHITE, CT_PESHKA, 2, 6, true},
	{CC_WHITE, CT_PESHKA, 3, 6, true},
	{CC_WHITE, CT_PESHKA, 4, 6, true},
	{CC_WHITE, CT_PESHKA, 5, 6, true},
	{CC_WHITE, CT_PESHKA, 6, 6, true},
	{CC_WHITE, CT_PESHKA, 7, 6, true},

	{CC_WHITE, CT_LADYA, 0, 7, true},
	{CC_WHITE, CT_KON,   1, 7, true},
    {CC_WHITE, CT_SLON,  2, 7, true},
    {CC_WHITE, CT_KOROL, 3, 7, true},
	{CC_WHITE, CT_FERZ,  4, 7, true},
    {CC_WHITE, CT_SLON,  5, 7, true},
	{CC_WHITE, CT_KON,   6, 7, true},
    {CC_WHITE, CT_LADYA, 7, 7, true}
};

ChessBoard::ChessBoard(void): cSingleton(*this), m_pCurrentEventQueye(NULL)
{
	for(int h = 0; h < BOARD_WIDTH; h++)
	{
		for(int v = 0; v < BOARD_HEIGHT; v++)
			mBoard[h][v] = NULL;
	}	
}

ChessBoard::~ChessBoard(void)
{
	destroy();
}


void 
ChessBoard::init()
{
	ChessBoardInitalMap initalMap;

	for(int i = 0; i < TOTAL_SHAPES; i++)
	{
		initalMap.push_back(sDefaultInitalData[i]);
	}

	init(initalMap);
}

void 
ChessBoard::init(ChessBoardInitalMap& initalMap)
{
	for(ChessBoardInitalMap::iterator it = initalMap.begin(); 
		it!= initalMap.end(); ++it)
	{
        StartData sd = *it;

		ChessShape* pShape;
		switch(sd.Type)
		{

				    case CT_PESHKA:
					{
						pShape = new Peshka();
						break;
					}
					case CT_KON:
					{
						pShape = new Kon();
						break;
					}
					case CT_SLON:
					{
						pShape = new Slon();
						break;
					}
					case CT_LADYA:
					{
						pShape = new Ladya();
						break;
					}
					case CT_FERZ:
					{
						pShape = new Ferz();
						break;
					}
					case CT_KOROL:
					{
						pShape = new Korol();
						break;
					}
					default:
					{
						break;
					}
        }

		pShape->init(this, sd.Color, sd.Horiz, sd.Vert);
        setShapeAt(sd.Horiz, sd.Vert, pShape);
		
		pShape->setActive(sd.isActive);
		mShapes.push_back(pShape);

    }

	sort(mShapes.begin(), mShapes.end(), shapeSortFunc);	
}

void 
ChessBoard::destroy()
{
	for(vector<ChessShape*>::iterator it = mShapes.begin();
		it!=mShapes.end(); ++it)
	{
	 ChessShape* sh =  *it;
	 if(sh) delete sh;
	}
	mShapes.clear();

    for(int h = 0; h < BOARD_WIDTH; h++)
	{
		for(int v = 0; v < BOARD_HEIGHT; v++)
			mBoard[h][v] = NULL;
	}
}

void 
ChessBoard::setShapeAt(int cellH, int cellV, ChessShape* shape)
{
 if((cellH < 0) || (cellH>=BOARD_WIDTH))
	 return;
 if((cellV < 0) || (cellV>=BOARD_HEIGHT))
	 return;
 mBoard[cellH][cellV] = shape;
}

ChessShape* 
ChessBoard::getShapeAt(int cellH, int cellV)
{
 if((cellH < 0) || (cellH>=BOARD_WIDTH))
	 return NULL;
 if((cellV < 0) || (cellV>=BOARD_HEIGHT))
	 return NULL;
 
 ChessShape* pShape = mBoard[cellH][cellV];
 
 if((pShape != NULL) && (m_pCurrentEventQueye!= NULL))
 {
	 pShape->setEventQueye(m_pCurrentEventQueye);
 }
 
 return mBoard[cellH][cellV];
}

bool
ChessBoard::setStep(int HFrom, int VFrom, int HTo, int VTo, ChessEventQueye& events)
{
 m_pCurrentEventQueye = &events;

 ChessShape* pShape = getShapeAt(HFrom, VFrom);
 if(pShape == NULL)
	 return false;
 if(!pShape->isActive())
	 return false;
 
 return pShape->setNewCell(HTo, VTo);
}



ChessShape* 
ChessBoard::findShape(ChessType t, ChessColor c)
{
	for(ChessShapeList::iterator it = mShapes.begin(); 
		it!= mShapes.end(); ++it)
	{
		ChessShape* pShape = *it;
		if((pShape->getType()==t) && (pShape->getColor() == c))
		 return pShape;
	}
	return NULL;
}