#pragma once
#include "pch.h"
class MainWnd : public D2D1Wnd
{
private:
	class Bitmap* m_bitmap;
public:
	MainWnd(HINSTANCE _instance, const TCHAR _className[], const TCHAR _title[], DWORD _width, DWORD _height, int _ncmdShow);
	MainWnd(HINSTANCE _instance, HWND _parent, const TCHAR _className[], const TCHAR _title[], int _x, int _y, DWORD _width, DWORD _height, int _ncmdShow);
	MainWnd() {};
	virtual ~MainWnd() {};

private:
	bool m_gridState = false;

private:
	void MenuBind(int _menu);
	void CreateGrid(int _size);
	void ResourceLoad();
public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void			 Init() override;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
	virtual void			 Resize(UINT _width, UINT _height) override;
	virtual void			 Render() override;
};

