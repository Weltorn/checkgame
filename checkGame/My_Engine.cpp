#include "My_Engine.h"
#include "Util.h"

using namespace std;



My_Engine::My_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
	WORD Icon, WORD SmIcon,
	int iWidth, int iHeight,
	COLORREF bkColor) :T_Engine(hInstance, szWindowClass, szTitle,
		Icon, SmIcon, iWidth, iHeight, bkColor)
{
	wnd_width = iWidth;
	wnd_height = iHeight;

	//创建图标图层及设备环境
	HDC hdc = GetDC(m_hWnd);
	hIconLayerDC = CreateCompatibleDC(hdc);
	
	hBkDC = CreateCompatibleDC(hdc);
	hBkBmp = CreateCompatibleBitmap(hdc, wnd_width, wnd_height);
	SelectObject(hBkDC, hBkBmp);
	ReleaseDC(m_hWnd, hdc); hdc = NULL;
	
	//加载透明图片为图标背景
	Bitmap bmp(L"pngs/transparent.png");
	//修改透明背景图片大小
	pTransBmp = new Gdiplus::Bitmap(wnd_width, wnd_height, bmp.GetPixelFormat());
	if (pTransBmp)
	{
		Gdiplus::Graphics * g = Gdiplus::Graphics::FromImage(pTransBmp);
		if (g)
		{
			// use the best interpolation mode
			g->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			g->DrawImage(&bmp, 0, 0, wnd_width, wnd_height);
		}
		DeleteObject(g);
	}

	Color clr(0, 0, 0, 0);
	if (hIconLayerBmp != NULL)		//从pTransBmp更新透明背景
		DeleteObject(hIconLayerBmp), hIconLayerBmp = NULL;

	if (Gdiplus::Ok == pTransBmp->GetHBITMAP(clr, &hIconLayerBmp))
	{		
		SelectObject(hIconLayerDC, hIconLayerBmp);
	}
	
}


My_Engine::~My_Engine()
{
	delete(imgNames);		//释放图标文件名列表
	delete(randIconIdMap);  //释放图标分布图
	delete(pTransBmp);
	DeleteObject(hIconLayerBmp);
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
			//不显示代表本级目录和上级目录的“.”和“..”，			
			if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
			lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
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
			{		//透明图片id为0
				if (lstrcmp(FindFileData.cFileName, TEXT("transparent.png")) == 0)

				{
					filelist->insert(filelist->begin(), FindFileData.cFileName);
				}
				else
				{
					filelist->push_back(FindFileData.cFileName);
				}
			}
			// 读者可根据文件属性表中的内容自行添加、判断文件属性
			Util::myprintf(L"\n");
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}
void My_Engine::GameInit()
{	
	//加载图片名称
	imgNames = new vector<wstring>();
	EnumerateFileInPath(L"pngs",imgNames);	

	//设置图标行列数
	row = 15;
	column = 15;

	//图标分布图
	randIconIdMap = (int*)malloc(row*column * sizeof(int));

	//初始化选中图标
	selectedIcon = -1;

	RECT mrect;	
	Image* thumbnail;

	//图片路径
	wstring basepath = L"pngs/";
	wstring filepath;

	//绘制背景图片（hBkDC）
	Graphics g(hBkDC);
	filepath = basepath + L"bg/bg.png";
	Gdiplus::Image img(filepath.c_str());
	thumbnail = img.GetThumbnailImage(wnd_width, wnd_height, NULL, NULL);
	g.DrawImage(thumbnail, 0,0);


	//绘制图标（hIconLayerDC）
	Graphics gi(hIconLayerDC);
	srand(time(NULL));
	int cell_width = wnd_width / column;
	int cell_height = wnd_height / row;
	int iconId;
	for (int r = 0; r<row; r++)
	{
		for (int c = 0; c<column; c++)
		{			
			mrect.left = c*cell_width;
			mrect.right = mrect.left + cell_width;
			mrect.top = r*cell_height;
			mrect.bottom = mrect.top + cell_height;

			iconId = rand() % (imgNames->size()-1)+1;//随机图片,从1号开始（0号为透明图片）
			*(randIconIdMap + r*column + c) = iconId;//保存图片分布图

			filepath = basepath + imgNames->at(iconId);
			Gdiplus::Image img(filepath.c_str());
			thumbnail = img.GetThumbnailImage(cell_width, cell_height, NULL, NULL);
			gi.DrawImage(thumbnail, mrect.left, mrect.top);
			
		}
	}

	//绘制背景图层
	BitBlt(bufferDC, 0, 0, WIN_WIDTH, WIN_HEIGHT,
		hBkDC, 0, 0, SRCCOPY);
	//叠加到bufferDC
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(bufferDC,
		0, 0, wnd_width, wnd_height,
		hIconLayerDC,
		0, 0, wnd_width, wnd_height,
		blendfunc);

	DeleteObject(&gi);
	DeleteObject(&g);
}
// 游戏逻辑处理
void My_Engine::GameLogic()
{
}
// 游戏结束处理
void My_Engine::GameEnd()
{
	
}
// 根据GAME_STATE值显示游戏界面
void My_Engine::GamePaint(HDC hdc)
{
	

	////绘制图标图层
	//Color clr(0, 0, 0, 0);
	//if (hIconLayerBmp != NULL)		//从pTransBmp更新透明背景
	//	DeleteObject(hIconLayerBmp), hIconLayerBmp = NULL;
	//if (Gdiplus::Ok == pTransBmp->GetHBITMAP(clr, &hIconLayerBmp))
	//{
	//	SelectObject(hIconLayerDC, hIconLayerBmp);
	//}

	////绘制图标（hIconLayerDC）
	//Graphics gi(hIconLayerDC);
	//RECT rect;
	//Image* thumbnail;
	//int cell_width = wnd_width / column;
	//int cell_height = wnd_height / row;
	//wstring basepath = L"pngs/";
	//wstring filepath;

	//for (int r = 0; r<row; r++)
	//{
	//	for (int c = 0; c<column; c++)
	//	{
	//		rect.left = c*cell_width;
	//		rect.right = rect.left + cell_width;
	//		rect.top = r*cell_height;
	//		rect.bottom = rect.top + cell_height;

	//		filepath = basepath + imgNames->at(*(randIconIdMap+r*column+c));
	//		Gdiplus::Image img(filepath.c_str());
	//		thumbnail = img.GetThumbnailImage(cell_width, cell_height, NULL, NULL);
	//		gi.DrawImage(thumbnail, rect.left, rect.top);

	//	}
	//}
	
}
// 根据KM_ACTION值处理按键行为
void My_Engine::GameKeyAction(int ActionType)
{}
// 根据KM_ACTION值处理鼠标行为
 void My_Engine::GameMouseAction(int x, int y, int ActionType)
{
	 int cell_width = wnd_width / column;
	 int cell_height = wnd_height / row;
	 int clickedIcon = x / cell_width + y / cell_height*row;
	 if (ActionType == MOUSE_LCLICK)
	 {
		 if(selectedIcon == -1)		//当前无选中的图标
		 {
			 selectedIcon = clickedIcon;
		 }
		 else if(clickedIcon != selectedIcon && isSameIcon(clickedIcon))//匹配成功
		 {
			 //消除图标
			 //替换为透明图标
			 //*(randIconIdMap + selectedIcon) = 0;
			// *(randIconIdMap + clickedIcon) = 0;
			 int iconpx = selectedIcon % column*cell_width;
			 int iconpy = selectedIcon / row*cell_height;
			 BitBlt(bufferDC, iconpx, iconpy, cell_width, cell_height,
				 hBkDC, iconpx, iconpy, SRCCOPY);

			 iconpx = clickedIcon % column*cell_width;
			 iconpy = clickedIcon / row*cell_height;
			 BitBlt(bufferDC, iconpx, iconpy, cell_width, cell_height,
				 hBkDC, iconpx, iconpy, SRCCOPY);

			 InvalidateRect(m_hWnd,NULL,false);
			 selectedIcon = -1;
		 }
		 else//匹配失败，更新为当前图标
		 {
			 selectedIcon = clickedIcon;
		 }


		 Util::myprintf(L"selected icon: y:%d,x:%d,id:%d\n", selectedIcon/row, selectedIcon%column, *(randIconIdMap + selectedIcon));
	 }
 }

 boolean My_Engine::isSameIcon(int clicked)
 {
	 if (selectedIcon == -1)
		 return false;
	 if (*(randIconIdMap + clicked) == *(randIconIdMap + selectedIcon))
		 return true;
	 return false;
 }
