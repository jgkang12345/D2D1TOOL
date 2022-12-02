#pragma once
#include "pch.h"
class Bitmap;
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
	int** m_mapData;
	std::map<char*, Bitmap*> m_bitmapDB;
	ID2D1BitmapRenderTarget* m_brt;
	HWND  m_scroll;
	FLOAT m_magnification = 1.0f;
private:
	void GridRender();
	void ResourceLoad();	
	void FileOpenProc();
	void SaveFile();
	void MapBinaryFileSave(TCHAR* _path);
	void EreaseRender();
	void ClickEvent(int _x, int _y);
	void EventHandler(class Event* _obj, const Pos& _pos);
	Pos  GetMousePosXY(int _x, int _y);
	Pos  GetMousePosXYMap(int _x, int _y);
public:
	virtual	LRESULT CALLBACK DisPatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual	void			 Init() override;
	virtual void			 CreateWnd(const TCHAR _className[], const TCHAR _titleName[], int _width, int _height, HINSTANCE _instance) override;
	virtual void			 Resize(UINT _width, UINT _height) override;
	virtual void			 Render() override;
public:
	static BOOL				 NewMapDiallog(HWND hwndDig, UINT message, WPARAM wParam, LPARAM IParam);
	void					 GridSet(int _xSize, int _ySzie, int _width);
	void					 GridUnSet();
	void					 TreeViewClickEventBind(ResourceObj* _obj);
	void					 SetScroll(HWND _hwnd) { m_scroll = _hwnd; }
	void					 Vscroll(WPARAM wParam, LPARAM lParam);
	void					 MouseWheel(WPARAM wParam, LPARAM lParam);
	void					 KeyDown(WPARAM _param);
}
;