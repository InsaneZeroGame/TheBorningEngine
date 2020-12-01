#include "TbeD3DRenderer.h"

TBE::TbeD3DRenderer::TbeD3DRenderer():
	m_swapchain(new SwapChain)
{
	m_device = Device::GetDevice().GetD3DDevice();
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
	InitFrameDependentResource();
}

void TBE::TbeD3DRenderer::InitFrameDependentResource()
{
}
