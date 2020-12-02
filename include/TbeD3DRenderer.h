#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "TbeRenderer.h"
#include "../DX12/base/Device.h"
#include "../DX12/base/SwapChain.h"
#include "../DX12/base/CommandListRing.h"
#include "../DX12/base/DynamicBufferRing.h"
#include "../DX12/base/ResourceViewHeaps.h"
#include "../DX12/base/StaticBufferPool.h"
#include "../DX12/base/UploadHeap.h"



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
		Device* m_device;
		SwapChain* m_swapchain = nullptr;
		UploadHeap                      m_UploadHeap;
		DynamicBufferRing               m_ConstantBufferRing;
		StaticBufferPool                m_VidMemBufferPool;
		CommandListRing                 m_CommandListRing;

		const bool  USE_VID_MEM = true;
		enum { 
			BACKBUFFER_COUNT = 3 ,
			commandListsPerBackBuffer = 1,

		};

	};
}