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
	l_cl->Reset(l_clAllocator, m_pipelineState.GetPipelineStateObject());
	
	l_cl->OMSetRenderTargets(1, m_swapchain->GetCurrentBackBufferRTV(), false, nullptr);
	float clearColor[4] = {1.0,1.0,0.0f,1.0f};

	D3D12_VIEWPORT l_viewPort = {
		0,0,(float)m_targetWindow->GetWidth(),(float)m_targetWindow->GetHeight(),
	0.0f,1.0f};

	D3D12_RECT l_rect = {0,0,(LONG)m_targetWindow->GetWidth(),(LONG)m_targetWindow->GetHeight()};
	l_cl->RSSetViewports(1, &l_viewPort);
	l_cl->RSSetScissorRects(1, &l_rect);
	l_cl->ClearRenderTargetView(*m_swapchain->GetCurrentBackBufferRTV(), clearColor, 1, &l_rect);
	std::vector<ID3D12CommandList*> l_lists = {l_cl};
	l_cl->SetPipelineState(m_pipelineState.GetPipelineStateObject());
	l_cl->SetGraphicsRootSignature(m_nullRS);
	l_cl->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	l_cl->IASetVertexBuffers(0, 1, &m_rendererResource.m_uploadBuffer->VertexBufferView(3,sizeof(float) * 4));
	l_cl->DrawInstanced(3, 1, 0, 0);
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
		memcpy(l_data, l_vertices.data(), sizeof(Vertex) * l_vertices.size());

	}


	InitFrameDependentResource();
}

void TBE::TbeD3DRenderer::InitFrameDependentResource()
{
}

void TBE::TbeD3DRenderer::InitPipelineStates()
{
	InitRootSignature();
	InitShaders();
	InitBuiltInPSOs();
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }

	};


	m_pipelineState = DefaultGraphicsPSO;
	m_pipelineState.SetRS(m_nullRS);
	m_pipelineState.SetVertexShader(m_shaders.m_defaultShdaderVS);
	m_pipelineState.SetPixelShader(m_shaders.m_defaultShdaderPS);
	auto l_swapchainFormat = m_swapchain->GetFormat();
	m_pipelineState.SetRenderTargetFormats(1, &l_swapchainFormat, DXGI_FORMAT_UNKNOWN);
	m_pipelineState.SetInputLayout(std::forward<std::vector<D3D12_INPUT_ELEMENT_DESC>>(inputElementDescs));
	m_pipelineState.Finalize();
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
	D3DCompileFromFile(L"C:\\Dev\\TheBorningEngine\\shaders\\default_vs.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlags, 0, &m_shaders.m_defaultShdaderVS, nullptr);
	D3DCompileFromFile(L"C:\\Dev\\TheBorningEngine\\shaders\\default_ps.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlags, 0, &m_shaders.m_defaultShdaderPS, nullptr);
	
}

void TBE::TbeD3DRenderer::InitRootSignature()
{
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;

	rootSignatureDesc.Init(0, nullptr, 0,nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* signature;
	ID3DBlob* error;
	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	Device::GetDevice().GetD3DDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_nullRS));

}
