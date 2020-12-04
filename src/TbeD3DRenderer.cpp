#include "TbeD3DRenderer.h"

TBE::TbeD3DRenderer::TbeD3DRenderer():
	m_swapchain(new SwapChain),
	m_device(&Device::GetDevice()),
	m_d3dDevice(m_device->GetD3DDevice()),
	m_cpuFrameIndex(0),
	m_gpuFrameIndex(0)
{
	m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&l_clAllocator));
	m_d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, l_clAllocator, nullptr, IID_PPV_ARGS(&l_cl));
	l_cl->Close();
	m_frameEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_frameFence));
}

TBE::TbeD3DRenderer::~TbeD3DRenderer()
{
	if (m_swapchain)
	{
		delete m_swapchain;
	}
}

void TBE::TbeD3DRenderer::OnInit()
{
	InitGpuResource();
}

void TBE::TbeD3DRenderer::OnUpdate()
{
	m_cpuFrameIndex++;

	D3D12_COMMAND_QUEUE_DESC l_desc = {};
	l_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	l_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	
	l_cl->Reset(l_clAllocator, nullptr);
	
	l_cl->OMSetRenderTargets(1, m_swapchain->GetCurrentBackBufferRTV(), false, nullptr);
	float clearColor[4] = {1.0,1.0,0.0f,1.0f};
	D3D12_RECT l_rect = {0,0,m_targetWindow->GetWidth(),m_targetWindow->GetHeight()};
	l_cl->ClearRenderTargetView(*m_swapchain->GetCurrentBackBufferRTV(), clearColor, 1, &l_rect);
	std::vector<ID3D12CommandList*> l_lists = {l_cl};

	l_cl->Close();
	m_device->GetGraphicsQueue()->ExecuteCommandLists(l_lists.size(), l_lists.data());
	m_device->GetGraphicsQueue()->Signal(m_frameFence, m_cpuFrameIndex);

	if (m_cpuFrameIndex - m_gpuFrameIndex > 3)
	{
		m_frameFence->SetEventOnCompletion(m_cpuFrameIndex, m_frameEvent);
		WaitForSingleObject(m_frameEvent, INFINITE);
		l_clAllocator->Reset();
		m_gpuFrameIndex = m_cpuFrameIndex;
	}
	m_swapchain->WaitForSwapChain();
	m_swapchain->Present();
}

void TBE::TbeD3DRenderer::InitGpuResource()
{
	m_swapchain->OnCreate(&Device::GetDevice(), BACKBUFFER_COUNT, m_targetWindow->GetNativeWindow());
	
    // Initialize helpers

    // Create a commandlist ring for the Direct queue
    m_CommandListRing.OnCreate(m_device, BACKBUFFER_COUNT, commandListsPerBackBuffer, m_device->GetGraphicsQueue()->GetDesc());

    // Create a 'dynamic' constant buffer
    const uint32_t constantBuffersMemSize = 20 * 1024 * 1024;
    m_ConstantBufferRing.OnCreate(m_device, BACKBUFFER_COUNT, constantBuffersMemSize, &ResourceViewHeaps::GetViewHeaps());

    // Create a 'static' pool for vertices, indices and constant buffers
    const uint32_t staticGeometryMemSize = 128 * 1024 * 1024;
    m_VidMemBufferPool.OnCreate(m_device, staticGeometryMemSize, USE_VID_MEM, "StaticGeom");

	InitFrameDependentResource();
}

void TBE::TbeD3DRenderer::InitFrameDependentResource()
{
}
