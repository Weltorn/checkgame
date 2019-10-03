
#include "Util.h"
#include <tchar.h>     
#include <stdio.h>   



Util::Util()
{
}


Util::~Util()
{
}
void Util::createDoubleBuffer(HWND hwnd, HDC& g_hdc, HBITMAP& hbitmap)
{
	HDC hdc = ::GetDC(hwnd);
	RECT rect;
	::GetClientRect(hwnd, &rect);
	g_hdc = ::CreateCompatibleDC(hdc);
	hbitmap = ::CreateCompatibleBitmap(hdc, rect.right-rect.left, rect.bottom-rect.top);
	::ReleaseDC(hwnd, hdc);
}

void Util::drawLine(HDC hdc, POINT begin, POINT end)
{
	::MoveToEx(hdc, begin.x, begin.y, NULL);
	::LineTo(hdc, end.x, end.y);
}

//���������Ϣ
void Util::myprintf(TCHAR* szFormat, ...)
{
	TCHAR   szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	_vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR),
		szFormat, pArgList);
	va_end(pArgList);
	OutputDebugString(szBuffer);
}

void Util::DrawBackground(HWND hWnd,HDC hdc)
{
	HBRUSH brush = ::CreateSolidBrush(RGB(255, 255, 255));
	RECT rect;
	::GetClientRect(hWnd, &rect);
	::FillRect(hdc, &rect, brush);
	::DeleteObject(brush); brush = NULL;
}

void  Util::draw_image(HWND hWnd, wchar_t* file)
{
	HDC hdc;
	int width, height;

	//����ͼ��
	Image image(file);
	if (image.GetLastStatus() != Status::Ok) {
		MessageBox(hWnd, L"ͼƬ��Ч!", NULL, MB_OK);
		return;
	}

	//ȡ�ÿ�Ⱥ͸߶�
	width = image.GetWidth();
	height = image.GetHeight();


	hdc = GetDC(hWnd);

	//��ͼ
	Graphics graphics(hdc);
	graphics.DrawImage(&image, 0, 0, width, height);

	ReleaseDC(hWnd, hdc);

	return;
}

Image* Util::LoadPNGFromStaticRes(HMODULE hModule, UINT nResId)
{
	HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(nResId), TEXT("PNG"));
	DWORD dwResSize = SizeofResource(hModule, hRes);

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwResSize);
	CopyMemory(GlobalLock(hGlobal), LockResource(LoadResource(hModule, hRes)), dwResSize);

	IStream* pIStream;
	CreateStreamOnHGlobal(hGlobal, FALSE, &pIStream);
	Image* pImg = Image::FromStream(pIStream);
	pIStream->Release();

	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return pImg;
}

