#include "pch.h"
#include "MainWnd.h"
#include "resource.h"
#include "Bitmap.h"
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
	wecx.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
	m_hwnd = CreateWindow(
		_className,
		_titleName,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		_width, _height,
		NULL,
		NULL,
		_instance,
		this);
	D2D1Core::GetInstance()->CreateRenderTarget(m_hwnd, &m_rt);
	D2D1Core::GetInstance()->SetFontFormat(&m_textFormat, L"나눔고딕", 30.0f);
	SetBrush(D2D1::ColorF::Black);
	SetBrush(D2D1::ColorF::Red, &m_brush1);
	ShowWindow(m_hwnd, 10);
	UpdateWindow(m_hwnd);
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
	RECT rc = GetClientSizeRect();
	if (m_bitmap)
	{
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
		CreateGrid(16);

	m_rt->EndDraw();
}

void MainWnd::MenuBind(int _menu)
{
	switch (_menu)
	{
	case ID_FILE_OPEN:
		ResourceLoad();
		break;

	case ID_TOOL_GRID:
		m_gridState = !m_gridState;
		break;
	default:
		break;
	}
}

void MainWnd::CreateGrid(int _size)
{
	RECT cr = GetClientSizeRect();
	int yCount = cr.bottom / _size;
	int xCount = cr.right / _size;
	for (int i = 0; i < xCount + 1; i++)
	{
		D2D1_POINT_2F start = { i * _size, 0 };
		D2D1_POINT_2F end = { i * _size, cr.bottom };
		m_rt->DrawLine(start, end, m_brush, 1.0f, nullptr);
	}

	for (int i = 0; i < yCount + 1; i++)
	{
		D2D1_POINT_2F start = { 0, i * _size };
		D2D1_POINT_2F end = { cr.right, i * _size };
		m_rt->DrawLine(start, end, m_brush, 1.0f, nullptr);
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
		MenuBind(LOWORD(wParam));
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