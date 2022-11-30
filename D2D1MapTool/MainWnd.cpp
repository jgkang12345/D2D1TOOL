#include "pch.h"
#include "MainWnd.h"
#include "resource.h"
#include "Bitmap.h"
#include "Controller.h"
MainWnd::MainWnd(HINSTANCE _instance, const TCHAR _className[], const TCHAR _title[], DWORD _width, DWORD _height, int _ncmdShow)
{
	m_instance = _instance;
	m_className = _className;
	m_titleName = _title;
	m_width = _width;
	m_height = _height;
	m_ncmdShow = _ncmdShow;
	Init();
}

MainWnd::MainWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow)
{
	m_instance = _instance;
	m_className = _className;
	m_titleName = _title;
	m_x = _x;
	m_y = _y;
	m_width = _width;
	m_height = _height;
	m_ncmdShow = _ncmdShow;
	m_hParent = _parent;
	Init();
}

void MainWnd::Init()
{
	WNDCLASSEX wecx;
	wecx.cbSize = sizeof(WNDCLASSEX);
	wecx.style = CS_HREDRAW | CS_VREDRAW;
	wecx.cbClsExtra = 0;
	wecx.cbWndExtra = sizeof(LONG_PTR);
	wecx.hInstance = m_instance;
	wecx.hIcon = LoadIcon(m_instance, IDI_APPLICATION);
	wecx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wecx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wecx.lpszClassName = m_className;
	wecx.lpszMenuName = MAKEINTRESOURCE(nullptr);
	wecx.hIconSm = LoadIcon(wecx.hInstance, IDI_APPLICATION);
	wecx.lpfnWndProc = Wnd::WndProc;  // 정적함수
	if (!RegisterClassEx(&wecx))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx Main failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);
		return;
	}
	CreateWnd(m_className, m_titleName, m_width, m_height, m_instance);
}

void MainWnd::CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance)
{
	m_hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_className,
		NULL,
		WS_CHILD | WS_VISIBLE,
		m_x, m_y,
		_width, _height,
		m_hParent,
		NULL,
		GetModuleHandle(NULL),
		this);
	Controller::GetInstance()->SetMainWnd(this);
	D2D1Core::GetInstance()->CreateRenderTarget(m_hwnd, &m_rt);
	D2D1Core::GetInstance()->SetFontFormat(&m_textFormat, L"나눔고딕", 15.0f);
	SetBrush(D2D1::ColorF::Black);
	SetBrush(D2D1::ColorF::Red, &m_brush1);
}


void MainWnd::Resize(UINT _width, UINT _height) 
{
	if (m_rt)
	{
		m_height = _height;
		m_width = _width;
		D2D1_SIZE_U resize = { m_width, m_height };
		m_rt->Resize(resize);
	}
}

void MainWnd::Render()
{
	m_rt->BeginDraw();
	m_rt->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));

	if (m_bitmap)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		FLOAT width = (float)(rc.right - rc.left);
		FLOAT height = (float)(rc.bottom - rc.top);

		if (width >= m_bitmap->GetWidht())
			width = m_bitmap->GetWidht();

		if (height >= m_bitmap->GetHeight())
			height = m_bitmap->GetHeight();

		int leftPos = 0 + m_scrollX;
		int topPos = 0 + m_scrollY;

		int rightPos = 0 + width + m_scrollX;
		int bottomPos = 0 + height + m_scrollY;

		D2D1_RECT_F source = D2D1::RectF(0, 0, m_bitmap->GetWidht(), m_bitmap->GetHeight());
		D2D1_RECT_F dest = D2D1::RectF(0 + m_scrollX, 0 + m_scrollY, m_bitmap->GetWidht() + m_scrollX, m_bitmap->GetHeight() + m_scrollY);
		m_rt->DrawBitmap(m_bitmap->GetBitmap(), dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
	}

	if (m_gridState)
		GridRender();

	m_rt->EndDraw();
}

BOOL MainWnd::NewMapDiallog(HWND hwndDig, UINT message, WPARAM wParam, LPARAM IParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		ShowWindow(hwndDig, NULL);
		UpdateWindow(hwndDig);
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDOK:
		{
			UINT XSize = GetDlgItemInt(hwndDig, IDC_X, NULL, 100);
			UINT YSize = GetDlgItemInt(hwndDig, IDC_Y, NULL, 100);
			Controller::GetInstance()->GetMainWnd()->GridSet(XSize, YSize, 16);
			Controller::GetInstance()->GetMainWnd()->FileOpenProc();
			
			EndDialog(hwndDig, TRUE);
		}
		break;

		case IDCANCEL:
			EndDialog(hwndDig, 0);
			return TRUE;
		}
		InvalidateRect(hwndDig, NULL, TRUE);

	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwndDig, &ps);
		EndPaint(hwndDig, &ps);
	}
	}
	return FALSE;
}

void MainWnd::GridSet(int _xSize, int _ySzie, int _width)
{
	m_gridXSize = _xSize;
	m_gridYSize = _ySzie;
	m_gridWidth = _width;
	m_gridState = true;
}

void MainWnd::GridUnSet()
{
	m_gridXSize = 0;
	m_gridYSize = 0;
	m_gridWidth = 0;
	m_gridState = false;
}

void MainWnd::MenuBind(int _menu)
{
	switch (_menu)
	{
	case ID_NEW_MAP:
		DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), m_hwnd, (DLGPROC)NewMapDiallog);
		break;

	default:
		break;
	}
}

void MainWnd::FileOpenProc()
{
	TCHAR* filePath = Wnd::FileOpen();
	TCHAR filePathCopy[256];
	if (filePath)
	{
		_tcscpy_s(filePathCopy, filePath);
		MainWnd::EraseBitmap();
		Bitmap* bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, filePathCopy);

		if (bitmap)
			SetBitmap(bitmap);
	}
}

void MainWnd::GridRender()
{
	for (int y = 0; y < m_gridYSize; y++)
	{
		for (int x = 0; x < m_gridXSize; x++)
		{
			D2D1_RECT_F rect = { x * m_gridWidth, y * m_gridWidth ,((x + 1) * m_gridWidth), ((y + 1) * m_gridWidth) };
			m_rt->DrawRectangle(rect, m_brush);
		}
	}
}

void MainWnd::ResourceLoad()
{
	TCHAR* _filePath = FileOpen();
	TCHAR filePath[256];
	TCHAR exp[256];
	if (_filePath == nullptr)
		return;

	_tcscpy_s(filePath, _filePath);

	if (filePath)
	{
		GetFileExp(filePath, exp);

		if (0 == _tcscmp(exp, _T("spr"))) 
		{
			// spirte 일때
		}
		else if (0 == _tcscmp(exp, _T("png")))
		{
			// png 일떄 
			m_bitmap = D2D1Core::GetInstance()->LoadBitmapByFileName(&m_rt, filePath);
		}
	}
}

LRESULT MainWnd::DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HMENU hMenu;
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_KEYUP:
		break;

	case WM_KEYDOWN:
		break;

	case WM_TIMER:
		break;

	case WM_SIZE:
		Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_COMMAND:
		break;
	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONUP:
		break;

	case WM_MOUSEWHEEL:
		break;

	case WM_VSCROLL:
		break;

	case WM_PAINT:
		Render();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}