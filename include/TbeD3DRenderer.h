#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "TbeRenderer.h"
#include "../DX12/base/Device.h"
#include "../DX12/base/SwapChain.h"

using namespace CAULDRON_DX12;

namespace TBE
{
	class TbeD3DRenderer final : public Renderer
	{
	public:
		TbeD3DRenderer();

		~TbeD3DRenderer();

		void OnInit() override;

		void OnUpdate() override;

		void InitGpuResource() override;

		void InitFrameDependentResource() override;

	private:
		ID3D12Device* m_device;

		SwapChain* m_swapchain = nullptr;

		enum { BACKBUFFER_COUNT = 3 };

	};
}