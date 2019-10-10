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

	//ͼ��������
	int row;
	int column;

	vector<wstring> *imgNames;	//ͼƬ�����б�
	int* randIconIdMap;			//ͼ��ֲ�ͼ
	int selectedIcon;			//��ǰѡ�е�ͼ��

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

	
	//�������ͼ���Ƿ�����ѡ���ͼ����ͬ
	boolean My_Engine::isSameIcon(int clicked);

	void GameInit();
	// ��Ϸ�߼�����
	void GameLogic();
	// ��Ϸ��������
	void GameEnd();
	// ����GAME_STATEֵ��ʾ��Ϸ����
	void GamePaint(HDC hdc);
	// ����KM_ACTIONֵ��������Ϊ
	void GameKeyAction(int ActionType = KEY_SYS_NONE);
	// ����KM_ACTIONֵ���������Ϊ
	void GameMouseAction(int x, int y, int ActionType);
};

