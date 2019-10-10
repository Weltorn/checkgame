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

	LONG play_width = wnd_width * 4 / 5;
	LONG play_height = wnd_height * 4 / 5;

	LONG margin_l = (wnd_width - play_width)/2;
	LONG margin_v = (wnd_height - play_height)/2;

	RECT play_rect;
	play_rect.left = margin_l;
	play_rect.right = wnd_width -margin_l;
	play_rect.top = margin_v;
	play_rect.bottom = wnd_height-margin_v;

	gameBoard = new GameBoard(bufferDC, play_rect, 12, 12);

}


My_Engine::~My_Engine()
{
	delete(gameBoard);
}



void My_Engine::GameInit()
{	
	gameBoard->initGameBoard(m_hWnd, bufferDC);
	gameBoard->initIcon();
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
	 if (ActionType == MOUSE_LCLICK)
	 {
		 if (gameBoard->getClickOnPlayArea(x, y))
			 InvalidateRect(m_hWnd, NULL, false);
	 }
 }

 
