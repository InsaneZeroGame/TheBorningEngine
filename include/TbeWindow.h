#pragma once
#include "../common/Misc/FrameworkWindows.h"
#include "../DX12/base/SwapChain.h"
#include "TbeRenderer.h"


namespace TBE
{
	using namespace CAULDRON_DX12;

	class BaseWindow : public FrameworkWindows
	{
	public:
		BaseWindow(LPCSTR name, uint32_t width, uint32_t height);
		~BaseWindow();
		__forceinline void SetRenderer(Renderer* p_renderer) { m_renderer = p_renderer; }

	private:

		// Inherited via FrameworkWindows
		virtual void OnParseCommandLine(LPSTR lpCmdLine, uint32_t* pWidth, uint32_t* pHeight, bool* pbFullScreen) override;
		virtual void OnCreate(HWND hWnd) override;
		virtual void OnDestroy() override;
		virtual void OnRender() override;
		virtual bool OnEvent(MSG msg) override;
		virtual void OnResize(uint32_t Width, uint32_t Height) override;
		virtual void SetFullScreen(bool fullscreen) override;

	protected:
		Renderer* m_renderer = nullptr;

		SwapChain* m_swapchain = nullptr;

		HWND m_hwnd = nullptr;

		enum {BACKBUFFER_COUNT = 3};
	};

	
}





