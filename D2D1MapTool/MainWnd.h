#pragma once
#include "pch.h"
class MainWnd : public D2D1Wnd
{
public:
	MainWnd(HINSTANCE _instance, const TCHAR _className[], const TCHAR _title[], DWORD _width, DWORD _height, int _ncmdShow);
	MainWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow);
	MainWnd() {};
	void MenuBind(int _menu);
	virtual ~MainWnd() {};

private:
	bool m_gridState = false;
	UINT m_gridXSize;
	UINT m_gridYSize;
	UINT m_gridWidth;

private:
	void GridRender();
	void ResourceLoad();	
	void FileOpenProc();
public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void			 Init() override;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
	virtual void			 Resize(UINT _width, UINT _height) override;
	virtual void			 Render() override;
	
public:
	static BOOL				NewMapDiallog(HWND hwndDig, UINT message, WPARAM wParam, LPARAM IParam);
	void					GridSet(int _xSize, int _ySzie, int _width);
	void					GridUnSet();
};

