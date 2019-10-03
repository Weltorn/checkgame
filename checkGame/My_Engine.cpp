#include "My_Engine.h"
#include "Util.h"
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
using namespace std;



My_Engine::My_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
	WORD Icon, WORD SmIcon,
	int iWidth, int iHeight,
	COLORREF bkColor):T_Engine(hInstance, szWindowClass,  szTitle,
		 Icon,  SmIcon, iWidth,  iHeight,	bkColor)
{
	wnd_width = iWidth;
	wnd_height = iHeight;
}


My_Engine::~My_Engine()
{
}


/**
	* @function: 获取cate_dir目录下的所有文件名
	* @param: cate_dir - string类型
	* @result：vector<string>类型
	*/

DWORD My_Engine::EnumerateFileInPath(LPWSTR szPath, vector<wstring>* filelist)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	WCHAR szFilePath[MAX_PATH];

	// 构造代表子目录和文件夹路径的字符串，使用通配符"*"
	lstrcpy(szFilePath, szPath);
	// 注释的代码可以用于查找所有以“.txt”结尾的文件
	// lstrcat(szFilePath, "\\*.txt");
	lstrcat(szFilePath, L"\\*");

	// 查找第一个文件/目录，获得查找句柄
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	// 判断句柄
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		Util::myprintf(L"错误：%d\n", GetLastError());
		return 1;
	}
	else
	{
		do
		{
			/* 如果不想显示代表本级目录和上级目录的“.”和“..”，
			可以使用注释部分的代码过滤
			if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
			lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
			continue;
			}
			*/

			// 打印文件名、目录名
			Util::myprintf(L"%ws\t\t", FindFileData.cFileName);
			// 判断文件属性，是否为加密文件或者文件夹
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
			{
				Util::myprintf(L"<加密> ");
			}
			// 判断文件属性，是否为隐藏文件或文件夹
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				Util::myprintf(L"<隐藏> ");
			}
			// 判断文件属性，是否为目录
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Util::myprintf(L"<DIR> ");
			}
			else
			{
				filelist->push_back(FindFileData.cFileName);
			}
			// 读者可根据文件属性表中的内容自行添加、判断文件属性
			Util::myprintf(L"\n");
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}
void My_Engine::GameInit()
{
	imgNames = new vector<wstring>();
	EnumerateFileInPath(L"pngs",imgNames);		//加载图片名称

	for (int size =0; size < imgNames->size(); size++)
	{
		Util::myprintf(L"%ws\n", imgNames->at(size).c_str());
	}
}
// 游戏逻辑处理
void My_Engine::GameLogic()
{}
// 游戏结束处理
void My_Engine::GameEnd()
{}
// 根据GAME_STATE值显示游戏界面
void My_Engine::GamePaint(HDC hdc)
{
	/*HGDIOBJ mbrush, oldbrush;
	HGDIOBJ mpen, oldpen;
	RECT mrect;
	int cell_width = wnd_width / 20;
		int cell_height = wnd_height / 20;
		for (int r = 0; r<20; r++)
		{
			for (int c = 0; c<20; c++)
			{
					mpen = CreatePen(PS_SOLID, 0, RGB(240, 240, 240));
					oldpen = SelectObject(hdc, mpen);
					mbrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
					oldbrush = SelectObject(hdc, mbrush);

					mrect.left = c*cell_width;
					mrect.right = mrect.left + cell_width;
					mrect.top = r*cell_height;
					mrect.bottom = mrect.top + cell_height;
					FillRect(hdc, &mrect, (HBRUSH)mbrush);
					Rectangle(hdc, mrect.left, mrect.top, mrect.right, mrect.bottom);

					SelectObject(hdc, oldbrush);
					DeleteObject(mbrush);

					SelectObject(hdc, oldpen);
					DeleteObject(mpen);
			}
		}
		*/
	
	Graphics g(hdc);
	Gdiplus::Image img(L"pngs/bat.png");
	Image* thumbnail = img.GetThumbnailImage(50,50,NULL,NULL);
	g.DrawImage(thumbnail, PointF());

}
// 根据KM_ACTION值处理按键行为
void My_Engine::GameKeyAction(int ActionType)
{}
// 根据KM_ACTION值处理鼠标行为
 void My_Engine::GameMouseAction(int x, int y, int ActionType)
{}
