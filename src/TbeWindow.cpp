#include "../include/stdafx.h"
#include <TbeWindow.h>

TBE::BaseWindow::BaseWindow(LPCSTR name, uint32_t width, uint32_t height) :
	FrameworkWindows(name)
{
	m_Width = width;
	m_Height = height;

}

TBE::BaseWindow::~BaseWindow()
{
}

HWND TBE::BaseWindow::GetNativeWindow() const
{
	return m_hwnd;
}



void TBE::BaseWindow::OnCreate(HWND hWnd)
{
	m_hwnd = hWnd;
}

void TBE::BaseWindow::OnDestroy()
{
	
}

bool TBE::BaseWindow::OnEvent(MSG msg)
{
	return false;
}

void TBE::BaseWindow::OnResize(uint32_t Width, uint32_t Height)
{
}

void TBE::BaseWindow::SetFullScreen(bool fullscreen)
{
}
