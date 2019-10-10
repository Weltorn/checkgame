#pragma once
#include "T_Engine.h"
class GameBoard
{
private:
	HDC hdc;	//���ƴ��ڻ���

	//ͼ��������
	int row;
	int column;

	//����ͼ��
	HDC hBkDC;
	HBITMAP hBkBmp;
	
	//͸��ͼƬ������ʵ��͸��Ч��
	Gdiplus::Bitmap * pTransBmp;

	RECT playRect;	//��Ϸ����

	//ͼ�����ͼ��
	HDC hIconLayerDC;
	HBITMAP hIconLayerBmp;

	vector<wstring> *imgNames;	//ͼƬ�����б�
	int* randIconIdMap;			//ͼ��ֲ�ͼ
	int selectedIcon;			//��ǰѡ�е�ͼ��

public:
	GameBoard();
	GameBoard(HDC hdc, RECT playRect, int row, int column);
	~GameBoard();
	bool initBk(HWND hWnd);
	bool initGameBoard(HWND Hwnd, HDC bufferDC);
	void setRect(RECT playRect);
	void setGrid(int row, int column);
	bool getClickOnPlayArea(int x, int y);
	void initIcon();
	//�������ͼ���Ƿ�����ѡ���ͼ����ͬ
	boolean isSameIcon(int clicked);
};

