#pragma once
#include "../include/TbeWindow.h"
#include "../include/TbeD3DRenderer.h"

namespace TBE
{
	class Application
	{
	public:
		Application();
		~Application();

	private:

	};

	int RunApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, FrameworkWindows* pFramework);

	
	static int App(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, uint32_t p_width, uint32_t p_height) {
		BaseWindow* l_newWindows = new BaseWindow("The Borning Game", p_width, p_height);
		TbeD3DRenderer* l_renderer = new TbeD3DRenderer();
		l_newWindows->SetRenderer(l_renderer);
		RunApp(hInstance, lpCmdLine, nCmdShow, l_newWindows);
		return 0;
	}
}