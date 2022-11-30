#include "pch.h"
#include "Controller.h"
#include "MainFrameWnd.h"
#include "MainWnd.h"
#include "TreeViewWnd.h"
Controller* Controller::m_instance = nullptr;


void Controller::Render()
{
	if (m_mWnd)
		m_mWnd->Render();

	if (m_treeViewWnd)
		m_treeViewWnd->Render();
}

Controller* Controller::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new Controller();
	return m_instance;
}
