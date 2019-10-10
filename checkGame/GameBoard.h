#pragma once
#include "T_Engine.h"
class GameBoard
{
private:
	HDC hdc;	//绘制窗口环境

	//图标行列数
	int row;
	int column;

	//背景图层
	HDC hBkDC;
	HBITMAP hBkBmp;
	
	//透明图片，用于实现透明效果
	Gdiplus::Bitmap * pTransBmp;

	RECT playRect;	//游戏区域

	//图标绘制图层
	HDC hIconLayerDC;
	HBITMAP hIconLayerBmp;

	vector<wstring> *imgNames;	//图片名称列表
	int* randIconIdMap;			//图标分布图
	int selectedIcon;			//当前选中的图标

public:
	GameBoard();
	GameBoard(HDC hdc, RECT playRect, int row, int column);
	~GameBoard();
	bool initBk(HWND hWnd);
	bool initGameBoard(HWND Hwnd, HDC bufferDC);
	void setRect(RECT playRect);
	void setGrid(int row, int column);
	bool getClickOnPlayArea(int x, int y);
	void initIcon();
	//检查点击的图标是否与已选择的图标相同
	boolean isSameIcon(int clicked);
};

