#pragma once
#include "../common/Misc/FrameworkWindows.h"


namespace TBE
{

	class BaseWindow : public FrameworkWindows
	{
	public:
		BaseWindow(LPCSTR name, uint32_t width, uint32_t height);
		~BaseWindow();

		HWND GetNativeWindow() const;

		virtual void OnCreate(HWND hWnd) override;

	private:

		// Inherited via FrameworkWindows
		virtual void OnDestroy() override;
		virtual bool OnEvent(MSG msg) override;
		virtual void OnResize(uint32_t Width, uint32_t Height) override;
		virtual void SetFullScreen(bool fullscreen) override;

	protected:
		
		HWND m_hwnd = nullptr;

	};

	
}





