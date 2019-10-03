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
	* @function: ��ȡcate_dirĿ¼�µ������ļ���
	* @param: cate_dir - string����
	* @result��vector<string>����
	*/

DWORD My_Engine::EnumerateFileInPath(LPWSTR szPath, vector<wstring>* filelist)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	WCHAR szFilePath[MAX_PATH];

	// ���������Ŀ¼���ļ���·�����ַ�����ʹ��ͨ���"*"
	lstrcpy(szFilePath, szPath);
	// ע�͵Ĵ���������ڲ��������ԡ�.txt����β���ļ�
	// lstrcat(szFilePath, "\\*.txt");
	lstrcat(szFilePath, L"\\*");

	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ��
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	// �жϾ��
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		Util::myprintf(L"����%d\n", GetLastError());
		return 1;
	}
	else
	{
		do
		{
			/* ���������ʾ������Ŀ¼���ϼ�Ŀ¼�ġ�.���͡�..����
			����ʹ��ע�Ͳ��ֵĴ������
			if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
			lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
			continue;
			}
			*/

			// ��ӡ�ļ�����Ŀ¼��
			Util::myprintf(L"%ws\t\t", FindFileData.cFileName);
			// �ж��ļ����ԣ��Ƿ�Ϊ�����ļ������ļ���
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
			{
				Util::myprintf(L"<����> ");
			}
			// �ж��ļ����ԣ��Ƿ�Ϊ�����ļ����ļ���
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				Util::myprintf(L"<����> ");
			}
			// �ж��ļ����ԣ��Ƿ�ΪĿ¼
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Util::myprintf(L"<DIR> ");
			}
			else
			{
				filelist->push_back(FindFileData.cFileName);
			}
			// ���߿ɸ����ļ����Ա��е�����������ӡ��ж��ļ�����
			Util::myprintf(L"\n");
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}
void My_Engine::GameInit()
{
	imgNames = new vector<wstring>();
	EnumerateFileInPath(L"pngs",imgNames);		//����ͼƬ����

	for (int size =0; size < imgNames->size(); size++)
	{
		Util::myprintf(L"%ws\n", imgNames->at(size).c_str());
	}
}
// ��Ϸ�߼�����
void My_Engine::GameLogic()
{}
// ��Ϸ��������
void My_Engine::GameEnd()
{}
// ����GAME_STATEֵ��ʾ��Ϸ����
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
// ����KM_ACTIONֵ��������Ϊ
void My_Engine::GameKeyAction(int ActionType)
{}
// ����KM_ACTIONֵ���������Ϊ
 void My_Engine::GameMouseAction(int x, int y, int ActionType)
{}
