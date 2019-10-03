#pragma once

#include "T_Engine.h"
class Util
{
public:
	Util();
	~Util();
	static void createDoubleBuffer(HWND,HDC&,HBITMAP&);
	static void drawLine(HDC, POINT begin, POINT end);
	//���������Ϣ������̨
	static void myprintf(TCHAR * szFormat, ...);
	static void DrawBackground(HWND hWnd, HDC hdc);
	static void  draw_image(HWND hWnd, wchar_t* file);
	Image* LoadPNGFromStaticRes(HMODULE hModule, UINT nResId);
};


