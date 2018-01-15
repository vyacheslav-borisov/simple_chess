#include "stdafx.h"
#include "../structs/singleton.h"
#include "ChessCommon.h"


static const StartData startMap[TOTAL_SHAPES] =
{
	{CC_BLACK, CT_PESHKA, 1, 0, true},
	{CC_BLACK, CT_PESHKA, 1, 1, true},
	{CC_BLACK, CT_PESHKA, 1, 2, true},
	{CC_BLACK, CT_PESHKA, 1, 3, true},
	{CC_BLACK, CT_PESHKA, 1, 4, true},
	{CC_BLACK, CT_PESHKA, 1, 5, true},
	{CC_BLACK, CT_PESHKA, 1, 6, true},
	{CC_BLACK, CT_PESHKA, 1, 7, true},

	{CC_BLACK, CT_LADYA, 0, 0, true},
	{CC_BLACK, CT_SLON,  0, 1, true},
	{CC_BLACK, CT_KON,   0, 2, true},
	{CC_BLACK, CT_FERZ,  0, 3, true},
	{CC_BLACK, CT_KOROL, 0, 4, true},
	{CC_BLACK, CT_KON,   0, 5, true},
	{CC_BLACK, CT_SLON,  0, 6, true},
	{CC_BLACK, CT_LADYA, 0, 7, true},


	{CC_WHITE, CT_PESHKA, 6, 0, true},
	{CC_WHITE, CT_PESHKA, 6, 1, true},
	{CC_WHITE, CT_PESHKA, 6, 2, true},
	{CC_WHITE, CT_PESHKA, 6, 3, true},
	{CC_WHITE, CT_PESHKA, 6, 4, true},
	{CC_WHITE, CT_PESHKA, 6, 5, true},
	{CC_WHITE, CT_PESHKA, 6, 6, true},
	{CC_WHITE, CT_PESHKA, 6, 7, true},

	{CC_BLACK, CT_LADYA, 7, 0, true},
	{CC_BLACK, CT_SLON,  7, 1, true},
	{CC_BLACK, CT_KON,   7, 2, true},
	{CC_BLACK, CT_FERZ,  7, 3, true},
	{CC_BLACK, CT_KOROL, 7, 4, true},
	{CC_BLACK, CT_KON,   7, 5, true},
	{CC_BLACK, CT_SLON,  7, 6, true},
	{CC_BLACK, CT_LADYA, 7, 7, true}
};