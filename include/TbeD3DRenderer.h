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
#include "TbeD3DResource.h"
#include "TbeD3DPepelineState.h"



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

		void InitPipelineStates() override;

		void InitShaders() override;

	private:
		Device* m_device;
		ID3D12Device* m_d3dDevice;
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
		ID3D12CommandAllocator* l_clAllocator = nullptr;
		ID3D12GraphicsCommandList* l_cl = nullptr;
		ID3D12Fence* m_frameFence;
		HANDLE m_frameEvent;
		uint64_t m_cpuFrameIndex;
		uint64_t m_gpuFrameIndex;

		struct Vertex
		{
			float pos[4];
		};


		struct RendererResource
		{
			TbeD3DStaticBuffer* m_staticBuffer;
			TbeD3DDynamicUploadBuffer* m_uploadBuffer;
		}m_rendererResource;

		struct SceneResource
		{
		
		}m_sceneResource;

		struct FrameResource
		{
		
		}m_frameResource;

		struct Shader
		{
			ID3DBlob* m_defaultShdaderVS;
			ID3DBlob* m_defaultShdaderPS;

		}m_shaders;
	};
}