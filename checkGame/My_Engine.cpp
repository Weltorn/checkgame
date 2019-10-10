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
			g->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			g->DrawImage(&bmp, 0, 0, wnd_width, wnd_height);
		}
		DeleteObject(g);
	}

	//从pTransBmp更新图标层透明背景
	Color clr(0, 0, 0, 0);
	if (hIconLayerBmp != NULL)		
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
	delete(pTransBmp);		//图标透明背景
	DeleteObject(hIconLayerBmp);
}



void My_Engine::GameInit()
{	
	//加载图片名称
	imgNames = new vector<wstring>();
	Util::EnumerateFileInPath(L"pngs",imgNames);	

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
	//绘制图标图层
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
