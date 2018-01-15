#pragma once


typedef tHotPointMatrix<TurnInfo, BOARD_WIDTH, BOARD_HEIGHT> HotPointMatrix;

class IChessShapeObserver
{
 public:
   virtual void update(ChessShape* sender) = 0;
};

typedef std::vector<IChessShapeObserver*> ChessShapeObserverList; 

class ChessShape
{
public:
	ChessShape(void);
	virtual ~ChessShape();

    unsigned int getID() { return mShapeId; };
	
	void init(ChessBoard* board, ChessColor color, int cellH, int cellV);
	
	virtual void setActive(bool active);
	bool isActive();
	
	virtual bool setNewCell(int HCell, int VCell) = 0;
	virtual ChessType getType() = 0;
	virtual void findHotPoint(HotPointMatrix& hp) = 0;
	virtual void getTurnList(std::vector<TurnInfo>& turnList) = 0;

	int getHCell();
	int getVCell();
	ChessColor getColor();
	
	void setEventQueye(ChessEventQueye* eventQueye) { mEventQueye = eventQueye; };

	void addObserver(IChessShapeObserver* pObserv);
	void removeObserver(IChessShapeObserver* pObserv);
protected:
    bool checkRange(int HCell, int VCell);
	void addChessEvent(ChessEventType eventType, 
		const std::string& message, void* param);
    void notifyObservers();

    unsigned int mShapeId;
	ChessBoard* mOwner;
	int mCellH;
	int mCellV;
	ChessColor mColor;
	bool mActive;

    ChessEventQueye* mEventQueye;
	ChessShapeObserverList mObservers;
private:
    static unsigned int sChessShapeId;
    static unsigned int getNextId() { return sChessShapeId++; };
};


class Peshka: public ChessShape
{
public:
   Peshka(void);
   virtual bool setNewCell(int HCell, int VCell);
   virtual ChessType getType() { return CT_PESHKA; };
   virtual void findHotPoint(HotPointMatrix& hp);
   virtual void getTurnList(std::vector<TurnInfo>& turnList);
private:
	bool mFirstStep;
};

class Kon: public ChessShape
{
public:
   virtual bool setNewCell(int HCell, int VCell);
   virtual ChessType getType() { return CT_KON; };
   virtual void findHotPoint(HotPointMatrix& hp);
   virtual void getTurnList(std::vector<TurnInfo>& turnList);
};

class Slon: public ChessShape
{
public:
   virtual bool setNewCell(int HCell, int VCell);
   virtual ChessType getType() { return CT_SLON; };
   virtual void findHotPoint(HotPointMatrix& hp);
   virtual void getTurnList(std::vector<TurnInfo>& turnList);
};

class Ladya: public ChessShape
{
public:
   virtual bool setNewCell(int HCell, int VCell);
   virtual ChessType getType() { return CT_LADYA; };
   virtual void findHotPoint(HotPointMatrix& hp);
   virtual void getTurnList(std::vector<TurnInfo>& turnList);
};

class Ferz: public ChessShape
{
public:
   virtual bool setNewCell(int HCell, int VCell);
   virtual ChessType getType() { return CT_FERZ; };
   virtual void findHotPoint(HotPointMatrix& hp);
   virtual void getTurnList(std::vector<TurnInfo>& turnList);
};

class Korol: public ChessShape
{
public:
   virtual bool setNewCell(int HCell, int VCell);
   virtual ChessType getType() { return CT_KOROL; };
   virtual void setActive(bool active);
   virtual void findHotPoint(HotPointMatrix& hp);
   virtual void getTurnList(std::vector<TurnInfo>& turnList);
};