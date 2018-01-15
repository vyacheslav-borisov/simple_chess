#pragma once
#include "../stdafx.h"


template <typename T, int WIDTH, int HEIGHT>
class tHotPointMatrix
{
public:
	void addHotPoint(int h, int v, T turn);
	std::vector<T>& getHotPoint(int h, int v);
	void sortCells(void* lpFunc);
	void clearCells();
private:
	std::vector<T> mHotPoints[WIDTH][HEIGHT];
};

template <typename T, int WIDTH, int HEIGHT>
inline void 
tHotPointMatrix<T, WIDTH, HEIGHT>::addHotPoint(int h, int v, T turn)
{
 if((h < 0) || (h >= WIDTH))
	 return;
 if((v < 0) || (v >= HEIGHT))
	 return;

 mHotPoints[h][v].push_back(turn);
};

template <typename T, int WIDTH, int HEIGHT>
inline std::vector<T>& 
tHotPointMatrix<T, WIDTH, HEIGHT>::getHotPoint(int h, int v)
{
 if((h < 0) || (h >= WIDTH))
	 throw std::out_of_range("cell with specified coords is not exist");
 if((v < 0) || (v >= HEIGHT))
	 throw std::out_of_range("cell with specified coords is not exist");

 return mHotPoints[h][v];
};

template <typename T, int WIDTH, int HEIGHT>
inline void 
tHotPointMatrix<T, WIDTH, HEIGHT>::sortCells(void* lpFunc)
{
 for(int h = 0; h < WIDTH; h++)
 {
	 for(int v = 0; v < HEIGHT; v++)
	 {
		 std::vector<T>& vec = getHotPoint(h, v);
		 sort(vec.begin(), vec.end(), lpFunc);
	 }//for(int v = 0; v < HEIGHT; v++)
 }//for(int h = 0; h < WIDTH; h++)
};

template <typename T, int WIDTH, int HEIGHT>
inline void 
tHotPointMatrix<T, WIDTH, HEIGHT>::clearCells()
{
 for(int h = 0; h < WIDTH; h++)
 {
	 for(int v = 0; v < HEIGHT; v++)
	 {
		 std::vector<T>& vec = getHotPoint(h, v);
		 if(!vec.empty()) vec.clear();
	 }//for(int v = 0; v < HEIGHT; v++)
 }//for(int h = 0; h < WIDTH; h++)
};
