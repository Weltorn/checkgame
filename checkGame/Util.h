#pragma once

#include "T_Engine.h"
class Util
{
public:
	Util();
	~Util();
	static void createDoubleBuffer(HWND,HDC&,HBITMAP&);
	static void drawLine(HDC, POINT begin, POINT end);
	//输出调试信息到控制台
	static void myprintf(TCHAR * szFormat, ...);
	static void DrawBackground(HWND hWnd, HDC hdc);
	static void  draw_image(HWND hWnd, wchar_t* file);
	Image* LoadPNGFromStaticRes(HMODULE hModule, UINT nResId);
};


