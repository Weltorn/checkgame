#include "GameBoard.h"



GameBoard::GameBoard()
{
}

GameBoard::GameBoard(HDC hdc,RECT playRect, int row, int column)
{
	this->hdc = hdc;
	this->playRect = playRect;
	this->row = row;
	this->column = column;
}

void GameBoard::setRect(RECT playRect)
{
	this->playRect = playRect;
}

void GameBoard::setGrid(int row, int column)
{
	this->row = row;
	this->column = column;
}

//�ͻ�������¼�
bool GameBoard::getClickOnPlayArea(int x, int y)
{
	//��ȡ������λ��
	x = x- this->playRect.left;
	y = y- this->playRect.top;
	if (x < 0 || y < 0)
	{
		return false;
	}
	int play_width = this->playRect.right - this->playRect.left;
	int play_height = this->playRect.bottom - this->playRect.top;
	int cell_width = play_width / column;
	int cell_height = play_height / row;
	int clickedIcon = x / cell_width + y / cell_height*row;
	
		if (selectedIcon == -1)		//��ǰ��ѡ�е�ͼ��
		{
			selectedIcon = clickedIcon;
		}
		else if (clickedIcon != selectedIcon && isSameIcon(clickedIcon))//ƥ��ɹ�
		{
			//����ͼ��
			int iconpx = selectedIcon % column*cell_width + playRect.left;
			int iconpy = selectedIcon / row*cell_height + playRect.top;
			BitBlt(hdc, iconpx, iconpy, cell_width, cell_height,
				hBkDC, iconpx, iconpy, SRCCOPY);

			iconpx = clickedIcon % column*cell_width+ playRect.left;
			iconpy = clickedIcon / row*cell_height+ playRect.top;
			BitBlt(hdc, iconpx, iconpy, cell_width, cell_height,
				hBkDC, iconpx, iconpy, SRCCOPY);
			selectedIcon = -1;
			return true;
		}
		else//ƥ��ʧ�ܣ�����Ϊ��ǰͼ��
		{
			selectedIcon = clickedIcon;
		}

		Util::myprintf(L"selected icon: y:%d,x:%d,id:%d\n", selectedIcon / row, selectedIcon%column, *(randIconIdMap + selectedIcon));
		return false;
}

void GameBoard::initIcon()
{
	//����ͼƬ����
	imgNames = new vector<wstring>();
	Util::EnumerateFileInPath(L"pngs", imgNames);

	//ͼ��ֲ�ͼ
	randIconIdMap = (int*)malloc(row*column * sizeof(int));

	//��ʼ��ѡ��ͼ��
	selectedIcon = -1;

	RECT mrect;
	Image* thumbnail;

	//ͼƬ·��
	wstring basepath = L"pngs/";
	wstring filepath;


	int play_width = this->playRect.right - this->playRect.left;
	int play_height = this->playRect.bottom - this->playRect.top;
	
	//��ʼ��������ͼ�꣨hIconLayerDC��
	Graphics gi(hIconLayerDC);
	srand(time(NULL));
	int cell_width = play_width / column;
	int cell_height = play_height / row;
	int iconId;
	for (int r = 0; r<row; r++)
	{
		for (int c = 0; c<column; c++)
		{
			mrect.left = c*cell_width;
			mrect.right = mrect.left + cell_width;
			mrect.top = r*cell_height;
			mrect.bottom = mrect.top + cell_height;

			iconId = rand() % (imgNames->size() - 1) + 1;//���ͼƬ,��1�ſ�ʼ��0��Ϊ͸��ͼƬ��
			*(randIconIdMap + r*column + c) = iconId;//����ͼƬ�ֲ�ͼ

			filepath = basepath + imgNames->at(iconId);
			Gdiplus::Image img(filepath.c_str());
			thumbnail = img.GetThumbnailImage(cell_width, cell_height, NULL, NULL);
			gi.DrawImage(thumbnail, mrect.left, mrect.top);

		}
	}

	//����ͼ��ͼ��
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(hdc,
		playRect.left, playRect.top, play_width, play_height,
		hIconLayerDC,
		0, 0, play_width, play_height,
		blendfunc);

	DeleteObject(&gi);
}

boolean GameBoard::isSameIcon(int clicked)
{
	if (selectedIcon == -1)
		return false;
	if (*(randIconIdMap + clicked) == *(randIconIdMap + selectedIcon))
		return true;
	return false;
}

GameBoard::~GameBoard()
{
	delete(imgNames);		//�ͷ�ͼ���ļ����б�
	delete(randIconIdMap);  //�ͷ�ͼ��ֲ�ͼ
	delete(pTransBmp);		//ͼ��͸������
	DeleteObject(hIconLayerBmp);
}

bool GameBoard::initBk(HWND hWnd)
{
	RECT wnd_rect;
	::GetClientRect(hWnd, &wnd_rect);

	int wnd_width = wnd_rect.right - wnd_rect.left;
	int wnd_height = wnd_rect.bottom - wnd_rect.top;

	//��������
	HDC hdc = GetDC(hWnd);
	hBkDC = CreateCompatibleDC(hdc);
	hBkBmp = CreateCompatibleBitmap(hdc, wnd_width, wnd_height);
	SelectObject(hBkDC, hBkBmp);
	ReleaseDC(hWnd, hdc); hdc = NULL;

	//ͼƬ·��
	wstring basepath = L"pngs/";
	wstring filepath;

	//���Ʊ���ͼƬ��hBkDC��
	Graphics g(hBkDC);
	Image* thumbnail;
	filepath = basepath + L"bg/bg.png";
	Gdiplus::Image img(filepath.c_str());
	thumbnail = img.GetThumbnailImage(wnd_width, wnd_height, NULL, NULL);
	g.DrawImage(thumbnail, 0, 0);

	//���Ʊ���ͼ��
	BitBlt(this->hdc, 0, 0, wnd_width, wnd_height,
		hBkDC, 0, 0, SRCCOPY);
	return true;
}

bool GameBoard::initGameBoard(HWND hWnd, HDC bufferDC)
{
	//��ʼ������
	initBk(hWnd);

	int play_width = playRect.right - playRect.left;
	int play_height = playRect.bottom - playRect.top;


	//����ͼ��ͼ�㼰�豸����
	HDC hdc = GetDC(hWnd);
	hIconLayerDC = CreateCompatibleDC(hdc);

	//����͸��ͼƬΪͼ�걳��
	Bitmap bmp(L"pngs/transparent.png");
	//�޸�͸������ͼƬ��С
	pTransBmp = new Gdiplus::Bitmap(play_width, play_height, bmp.GetPixelFormat());
	if (pTransBmp)
	{
		Gdiplus::Graphics * g = Gdiplus::Graphics::FromImage(pTransBmp);
		if (g)
		{
			g->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			g->DrawImage(&bmp, 0, 0, play_width, play_height);
		}
		DeleteObject(g);
	}

	//��pTransBmp����ͼ���͸������
	Color clr(0, 0, 0, 0);
	if (hIconLayerBmp != NULL)
		DeleteObject(hIconLayerBmp), hIconLayerBmp = NULL;
	if (Gdiplus::Ok == pTransBmp->GetHBITMAP(clr, &hIconLayerBmp))
	{
		SelectObject(hIconLayerDC, hIconLayerBmp);
	}
	return false;
}
