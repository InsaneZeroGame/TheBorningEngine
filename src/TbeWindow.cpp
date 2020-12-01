#include "../include/stdafx.h"
#include <TbeWindow.h>

TBE::BaseWindow::BaseWindow(LPCSTR name, uint32_t width, uint32_t height) :
	FrameworkWindows(name),
	m_swapchain(new SwapChain)
{
	m_Width = width;
	m_Height = height;

}

TBE::BaseWindow::~BaseWindow()
{
}

void TBE::BaseWindow::OnParseCommandLine(LPSTR lpCmdLine, uint32_t* pWidth, uint32_t* pHeight, bool* pbFullScreen)
{
	*pWidth = m_Width;
	*pHeight = m_Height;
}

void TBE::BaseWindow::OnCreate(HWND hWnd)
{
	m_hwnd = hWnd;
	m_swapchain->OnCreate(&Device::GetDevice(), BACKBUFFER_COUNT, m_hwnd);
}

void TBE::BaseWindow::OnDestroy()
{
	if (m_swapchain)
	{
		delete m_swapchain;
	}
}

void TBE::BaseWindow::OnRender()
{
	if (!m_renderer) assert(0 && "No Renderer Set!");
	m_renderer->Update();
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
