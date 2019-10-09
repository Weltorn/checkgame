#include "My_Engine.h"
#include "Util.h"

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
	delete(imgNames);		//释放图标文件名列表
	delete(randIconIdMap);  //释放图标分布图

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

	randIconIdMap = (int*)malloc(row*column * sizeof(int));

	selectedIcon = -1;
	//设置图标行列数
	row = 15;
	column = 15;

	//绘制图标
	RECT mrect;
	Graphics g(bufferDC);
	Image* thumbnail;

	wstring basepath = L"pngs/";
	wstring filepath;

	//绘制背景图片
	filepath = basepath + L"bg/bg.png";
	Gdiplus::Image img(filepath.c_str());
	thumbnail = img.GetThumbnailImage(wnd_width, wnd_height, NULL, NULL);
	g.DrawImage(thumbnail, 0,0);


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

			iconId = rand() % imgNames->size();//随机图片
			*(randIconIdMap + r*column + c) = iconId;//保存图片分布图

			filepath = basepath + imgNames->at(iconId);
			Gdiplus::Image img(filepath.c_str());
			thumbnail = img.GetThumbnailImage(cell_width, cell_height, NULL, NULL);
			g.DrawImage(thumbnail, mrect.left, mrect.top);
			
		}
	}


}
// 游戏逻辑处理
void My_Engine::GameLogic()
{}
// 游戏结束处理
void My_Engine::GameEnd()
{
	
}
// 根据GAME_STATE值显示游戏界面
void My_Engine::GamePaint(HDC hdc)
{
	
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
		 else if(isSameIcon(clickedIcon))//匹配成功
		 {
			 Util::myprintf(L"same icon\n");
			 //消除图标
			 //待编写...
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
