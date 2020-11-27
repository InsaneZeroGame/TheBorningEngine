#include "TbeD3DRenderer.h"

TBE::TbeD3DRenderer::TbeD3DRenderer()
{
	m_device = Device::GetDevice().GetD3DDevice();
}

TBE::TbeD3DRenderer::~TbeD3DRenderer()
{
}

void TBE::TbeD3DRenderer::Update()
{
}
