#pragma once
#include "T_Engine.h"

class My_Engine :
	public T_Engine
{
private:
	int wnd_width;
	int wnd_height;
public:
	vector<wstring> *imgNames;	//图片名称列表

	My_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
		WORD Icon = NULL, WORD SmIcon = NULL,
		int iWidth = WIN_WIDTH, int iHeight = WIN_HEIGHT,
		COLORREF bkColor = WIN_BKCLR);
	~My_Engine();
	DWORD EnumerateFileInPath(LPWSTR szPath, vector<wstring>* filelist);

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

