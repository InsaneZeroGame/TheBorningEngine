#include "../include/stdafx.h"
#include <TbeWindow.h>

TBE::BaseWindow::BaseWindow(LPCSTR name):FrameworkWindows(name)
{
}

TBE::BaseWindow::~BaseWindow()
{
}

void TBE::BaseWindow::OnParseCommandLine(LPSTR lpCmdLine, uint32_t* pWidth, uint32_t* pHeight, bool* pbFullScreen)
{
}

void TBE::BaseWindow::OnCreate(HWND hWnd)
{
}

void TBE::BaseWindow::OnDestroy()
{
}

void TBE::BaseWindow::OnRender()
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
