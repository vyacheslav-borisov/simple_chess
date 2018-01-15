#pragma once

typedef std::vector<ChessShape*> ChessShapeList;

class ChessBoard: public cSingleton<ChessBoard>
{
public:
	ChessBoard(void);
	~ChessBoard(void);
	//void init(StartData* map);

    void init();
	void init(ChessBoardInitalMap& initalMap);
	void destroy();

	ChessShapeList* getChessShapeList() { return &mShapes; };
    
	void setShapeAt(int cellH, int cellV, ChessShape* shape);
	ChessShape* getShapeAt(int cellH, int cellV);
	ChessShape* findShape(ChessType t, ChessColor c);
	
	bool setStep(int HFrom, int VFrom, int HTo, int VTo, ChessEventQueye& events);	
private:   

	ChessShape* mBoard[BOARD_WIDTH][BOARD_HEIGHT];
	ChessShapeList mShapes;
    
	ChessEventQueye* m_pCurrentEventQueye;
private:
	static const StartData sDefaultInitalData[TOTAL_SHAPES];
};

#define TheGameBoard  ChessBoard::getInstance()