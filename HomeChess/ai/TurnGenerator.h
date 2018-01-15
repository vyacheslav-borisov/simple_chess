#pragma once



class TurnGenerator
{
public:
	TurnGenerator(void);
	TurnGenerator(std::vector<ChessShape*>* pShapes);
	~TurnGenerator(void);
	void init(std::vector<ChessShape*>* pShapes);
	void destroy();
	bool generateTurn(TurnInfo& turn);
private:
 bool chooseAttackTurn(TurnInfo& turn);
 bool chooseNeitralTurn(TurnInfo& turn);
 bool chooseBadTurn(TurnInfo& turn);
 bool chooseContrTurn(ChessShape* shape, TurnInfo& turn);
 bool chooseEscapeTurn(ChessShape* shape, TurnInfo& turn);
 bool chooseUmbrellaTurn(ChessShape* shape, TurnInfo& turn);
 bool chooseProtectTurn(ChessShape* shape, TurnInfo& turn);
 bool chooseWardTurn(ChessShape* shape, TurnInfo& turn);

 HotPointMatrix mWhiteHP;
 HotPointMatrix mBlackHP;
 std::vector<TurnInfo> mAttackTurns;
 std::vector<TurnInfo> mNeitralTurns;
 std::vector<TurnInfo> mBadTurns;
 std::vector<ChessShape*>* mShapes;
};
