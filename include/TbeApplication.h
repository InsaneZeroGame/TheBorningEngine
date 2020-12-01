#pragma once
#include "../include/TbeWindow.h"
#include "../include/TbeD3DRenderer.h"

namespace TBE
{
	class Application
	{
	public:
		Application(uint32_t p_width, uint32_t p_height);
		~Application();

		BaseWindow* const GetWindow() const { return m_window; };

		Renderer* const GetRenderer() const { return m_renderer; }

		void OnParseCommandLine(LPSTR lpCmdLine, uint32_t* pWidth, uint32_t* pHeight, bool* pbFullScreen);

		void SetNativeWindow(HWND hWnd);

		void StartUpModules();
	private:
		BaseWindow* m_window = nullptr;

		Renderer* m_renderer = nullptr;

	};

	int RunApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, Application* pFramework);

	
	static int App(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, uint32_t p_width, uint32_t p_height) {
		auto* l_app = new Application(p_width, p_height);
		RunApp(hInstance, lpCmdLine, nCmdShow, l_app);
		return 0;
	}
}