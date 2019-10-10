#pragma once
#include "T_Engine.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <AtlBase.h>

#ifdef WIN32
#include <direct.h>
#include <io.h>
#endif
using namespace std;

class My_Engine :
	public T_Engine
{
private:
	int wnd_width;
	int wnd_height;

	//图标行列数
	int row;
	int column;

	vector<wstring> *imgNames;	//图片名称列表
	int* randIconIdMap;			//图标分布图
	int selectedIcon;			//当前选中的图标

	HDC hIconLayerDC;
	HBITMAP hIconLayerBmp;

	HDC hBkDC;
	HBITMAP hBkBmp;

	Gdiplus::Bitmap * pTransBmp;
public:
	

	My_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
		WORD Icon = NULL, WORD SmIcon = NULL,
		int iWidth = WIN_WIDTH, int iHeight = WIN_HEIGHT,
		COLORREF bkColor = WIN_BKCLR);
	~My_Engine();

	
	//检查点击的图标是否与已选择的图标相同
	boolean My_Engine::isSameIcon(int clicked);

	void GameInit();
	// 游戏逻辑处理
	void GameLogic();
	// 游戏结束处理
	void GameEnd();
	// 根据GAME_STATE值显示游戏界面
	void GamePaint(HDC hdc);
	// 根据KM_ACTION值处理按键行为
	void GameKeyAction(int ActionType = KEY_SYS_NONE);
	// 根据KM_ACTION值处理鼠标行为
	void GameMouseAction(int x, int y, int ActionType);
};

