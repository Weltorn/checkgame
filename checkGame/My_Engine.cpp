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
	delete(imgNames);		//�ͷ�ͼ���ļ����б�
	delete(randIconIdMap);  //�ͷ�ͼ��ֲ�ͼ

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

	randIconIdMap = (int*)malloc(row*column * sizeof(int));

	selectedIcon = -1;
	//����ͼ��������
	row = 15;
	column = 15;

	//����ͼ��
	RECT mrect;
	Graphics g(bufferDC);
	Image* thumbnail;

	wstring basepath = L"pngs/";
	wstring filepath;

	//���Ʊ���ͼƬ
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

			iconId = rand() % imgNames->size();//���ͼƬ
			*(randIconIdMap + r*column + c) = iconId;//����ͼƬ�ֲ�ͼ

			filepath = basepath + imgNames->at(iconId);
			Gdiplus::Image img(filepath.c_str());
			thumbnail = img.GetThumbnailImage(cell_width, cell_height, NULL, NULL);
			g.DrawImage(thumbnail, mrect.left, mrect.top);
			
		}
	}


}
// ��Ϸ�߼�����
void My_Engine::GameLogic()
{}
// ��Ϸ��������
void My_Engine::GameEnd()
{
	
}
// ����GAME_STATEֵ��ʾ��Ϸ����
void My_Engine::GamePaint(HDC hdc)
{
	
}
// ����KM_ACTIONֵ��������Ϊ
void My_Engine::GameKeyAction(int ActionType)
{}
// ����KM_ACTIONֵ���������Ϊ
 void My_Engine::GameMouseAction(int x, int y, int ActionType)
{
	 int cell_width = wnd_width / column;
	 int cell_height = wnd_height / row;
	 int clickedIcon = x / cell_width + y / cell_height*row;
	 if (ActionType == MOUSE_LCLICK)
	 {
		 if(selectedIcon == -1)		//��ǰ��ѡ�е�ͼ��
		 {
			 selectedIcon = clickedIcon;
		 }
		 else if(isSameIcon(clickedIcon))//ƥ��ɹ�
		 {
			 Util::myprintf(L"same icon\n");
			 //����ͼ��
			 //����д...
			 selectedIcon = -1;
		 }
		 else//ƥ��ʧ�ܣ�����Ϊ��ǰͼ��
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
