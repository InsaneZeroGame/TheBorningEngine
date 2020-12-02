#include "TbeD3DRenderer.h"

TBE::TbeD3DRenderer::TbeD3DRenderer():
	m_swapchain(new SwapChain)
{
	m_device = &Device::GetDevice();
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
