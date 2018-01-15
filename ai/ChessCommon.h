#pragma once

class ChessShape;
class ChessBoard;

const int BOARD_WIDTH = 8;
const int BOARD_HEIGHT = 8;
const int NUM_SHAPES = 16;
const int TOTAL_SHAPES = 32;

/*
enum ChessType
{
 CT_EMPTY = 0,
 CT_PESHKA = 1,
 CT_KON = 2,
 CT_SLON = 4,
 CT_LADYA = 8,
 CT_FERZ = 16,
 CT_KOROL = 32
};
*/

typedef unsigned int ChessType;
const ChessType CT_EMPTY = 0;
const ChessType CT_PESHKA = 1;
const ChessType CT_KON = 2;
const ChessType CT_SLON = 4;
const ChessType CT_LADYA = 8;
const ChessType CT_FERZ = 16;
const ChessType CT_KOROL = 32;

const ChessType CT_PSF = CT_PESHKA|CT_SLON|CT_FERZ; //1 пешка, слон, ферз
const ChessType CT_LFKr = CT_FERZ|CT_LADYA|CT_KOROL;//2 король, ладья, ферзь
const ChessType CT_SF = CT_SLON|CT_FERZ; //3 слон, ферзь

typedef unsigned int ChessColor;
const ChessColor CC_WHITE = 1;
const ChessColor CC_BLACK = 2;

/*
enum ChessColor
{
 CC_WHITE = 1,
 CC_BLACK = 2
};
*/

enum ChessEvents
{
 CE_NORMAL = 0,
 CE_WHITE_KING_KILLED = 1,
 CE_BLACK_KING_KILLED = 2,
 CE_PAWN_IS_QUEEN = 4
};

typedef unsigned int ChessEventType;
const ChessEventType CET_TURN_ERROR = 0;
const ChessEventType CET_WHITE_KING_KILLED = 1;
const ChessEventType CET_BLACK_KING_KILLED = 2;
const ChessEventType CET_PAWN_IS_QUEEN = 4;

struct ChessEvent
{
 ChessEventType _type;
 std::string _message;
 void* _param;
};

typedef std::queue<ChessEvent> ChessEventQueye;


struct StartData
{
 ChessColor Color;
 ChessType Type;
 int Horiz;
 int Vert;
 bool isActive;
};

typedef std::vector<StartData> ChessBoardInitalMap;

typedef unsigned int TurnType;
const TurnType TURNTYPE_UNKNOWN = 0;
const TurnType TURNTYPE_ATTACK = 1;
const TurnType TURNTYPE_NEITRAL = 2;
const TurnType TURNTYPE_BAD = 3;
const TurnType TURNTYPE_CONTR = 4;
const TurnType TURNTYPE_ESCAPE = 5;
const TurnType TURNTYPE_UMBRELLA = 6;
const TurnType TURNTYPE_PROTECT = 7;

struct TurnInfo
{
 int hfrom;
 int vfrom;
 int hto;
 int vto;
 ChessType src;
 ChessType dest;
 TurnType _type; 
};

extern const StartData startMap[TOTAL_SHAPES];


#include "hpm.h"
#include "ChessShape.h"
#include "ChessBoard.h"
#include "TurnGenerator.h"