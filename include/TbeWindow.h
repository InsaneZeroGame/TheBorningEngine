#pragma once
#include <Cauldron/src/common/Misc/FrameworkWindows.h>

namespace TBE
{
	class BaseWindow : public FrameworkWindows
	{
	public:
		BaseWindow(LPCSTR name, uint32_t width, uint32_t height);
		~BaseWindow();

	private:

		// Inherited via FrameworkWindows
		virtual void OnParseCommandLine(LPSTR lpCmdLine, uint32_t* pWidth, uint32_t* pHeight, bool* pbFullScreen) override;
		virtual void OnCreate(HWND hWnd) override;
		virtual void OnDestroy() override;
		virtual void OnRender() override;
		virtual bool OnEvent(MSG msg) override;
		virtual void OnResize(uint32_t Width, uint32_t Height) override;
		virtual void SetFullScreen(bool fullscreen) override;
	};

	int App(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) {
		BaseWindow* l_newWindows = new BaseWindow("The Borning Game", 800, 600);
		RunFramework(hInstance, lpCmdLine, nCmdShow, l_newWindows);
		delete l_newWindows;
		return 0;
	}
}





