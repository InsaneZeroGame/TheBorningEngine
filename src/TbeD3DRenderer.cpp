#include "TbeD3DRenderer.h"
#include <d3dcompiler.h>

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
	InitPipelineStates();

}

void TBE::TbeD3DRenderer::OnUpdate()
{
	m_cpuFrameIndex++;
	l_cl->Reset(l_clAllocator, nullptr);
	
	l_cl->OMSetRenderTargets(1, m_swapchain->GetCurrentBackBufferRTV(), false, nullptr);
	float clearColor[4] = {1.0,1.0,0.0f,1.0f};

	D3D12_VIEWPORT l_viewPort = {
		0,0,(float)m_targetWindow->GetWidth(),(float)m_targetWindow->GetHeight(),
	0.0f,1.0f};

	D3D12_RECT l_rect = {0,0,(LONG)m_targetWindow->GetWidth(),(LONG)m_targetWindow->GetHeight()};
	l_cl->RSSetViewports(1, &l_viewPort);
	l_cl->ClearRenderTargetView(*m_swapchain->GetCurrentBackBufferRTV(), clearColor, 1, &l_rect);
	std::vector<ID3D12CommandList*> l_lists = {l_cl};

	l_cl->Close();
	m_device->GetGraphicsQueue()->ExecuteCommandLists(static_cast<uint32_t>(l_lists.size()), l_lists.data());
	m_device->GetGraphicsQueue()->Signal(m_frameFence, m_cpuFrameIndex);

	if (m_cpuFrameIndex - m_gpuFrameIndex > BACKBUFFER_COUNT)
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
	//1.Swapchain
	m_swapchain->OnCreate(&Device::GetDevice(), BACKBUFFER_COUNT, m_targetWindow->GetNativeWindow());
	
	//2.Renderer Resource
	{
		m_rendererResource.m_staticBuffer = new TbeD3DStaticBuffer;
		m_rendererResource.m_staticBuffer->Create(L"VertexIndex Buffer", 3, sizeof(Vertex), nullptr);
		m_rendererResource.m_uploadBuffer = new TbeD3DDynamicUploadBuffer;
		m_rendererResource.m_uploadBuffer->Create(L"UploadBuffer", 3, sizeof(Vertex));
		void* l_data = m_rendererResource.m_uploadBuffer->Map();
		std::vector<Vertex> l_vertices = 
		{
			{0.0f,1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f,1.0f},
			{-1.0f,0.0f,0.0f,1.0f},
		};

	}


	InitFrameDependentResource();
}

void TBE::TbeD3DRenderer::InitFrameDependentResource()
{
}

void TBE::TbeD3DRenderer::InitPipelineStates()
{
	InitShaders();
	InitBuiltInPSOs();
}

void TBE::TbeD3DRenderer::InitShaders()
{
#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	//UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#else
	UINT compileFlags = 0;
#endif
	D3DCompileFromFile(L"C:\\Dev\\FlyCore\\Renderer\\default_vs.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlags, 0, &m_shaders.m_defaultShdaderVS, nullptr);
	D3DCompileFromFile(L"C:\\Dev\\FlyCore\\Renderer\\default_ps.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlags, 0, &m_shaders.m_defaultShdaderPS, nullptr);
	
}
