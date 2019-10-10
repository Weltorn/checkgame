#pragma once
#include "T_Engine.h"
class GameBoard
{
private:
	RECT boardRect;
	int row;
	int column;

public:
	GameBoard();
	GameBoard(RECT,int row,int column);

	~GameBoard();
};

