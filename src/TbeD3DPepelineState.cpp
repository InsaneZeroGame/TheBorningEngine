#include "stdafx.h"
#include "TbeD3DPepelineState.h"


namespace TBE
{
    TbeD3DGraphicsPipelineState::TbeD3DGraphicsPipelineState()
    {
        ZeroMemory(&m_PSODesc, sizeof(m_PSODesc));
        m_PSODesc.NodeMask = 1;
        m_PSODesc.SampleMask = 0xFFFFFFFFu;
        m_PSODesc.SampleDesc.Count = 1;
        m_PSODesc.InputLayout.NumElements = 0;
    }

    void TbeD3DGraphicsPipelineState::SetBlendState(const D3D12_BLEND_DESC& BlendDesc)
    {
        m_PSODesc.BlendState = BlendDesc;
    }

    void TbeD3DGraphicsPipelineState::SetRasterizerState(const D3D12_RASTERIZER_DESC& RasterizerDesc)
    {
        m_PSODesc.RasterizerState = RasterizerDesc;
    }

    void TbeD3DGraphicsPipelineState::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& DepthStencilDesc)
    {
        m_PSODesc.DepthStencilState = DepthStencilDesc;
    }

    void TbeD3DGraphicsPipelineState::SetSampleMask(UINT SampleMask)
    {
        m_PSODesc.SampleMask = SampleMask;
    }

    void TbeD3DGraphicsPipelineState::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE TopologyType)
    {
        (TopologyType != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED, "Can't draw with undefined topology");
        m_PSODesc.PrimitiveTopologyType = TopologyType;
    }

    void TbeD3DGraphicsPipelineState::SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps)
    {
        m_PSODesc.IBStripCutValue = IBProps;
    }

    void TbeD3DGraphicsPipelineState::SetRenderTargetFormat(DXGI_FORMAT RTVFormat, DXGI_FORMAT DSVFormat, UINT MsaaCount, UINT MsaaQuality)
    {
        SetRenderTargetFormats(1, &RTVFormat, DSVFormat, MsaaCount, MsaaQuality);
    }

    void TbeD3DGraphicsPipelineState::SetRenderTargetFormats(UINT NumRTVs, const DXGI_FORMAT* RTVFormats, DXGI_FORMAT DSVFormat, UINT MsaaCount, UINT MsaaQuality)
    {
        (NumRTVs == 0 || RTVFormats != nullptr, "Null format array conflicts with non-zero length");
        for (UINT i = 0; i < NumRTVs; ++i)
            m_PSODesc.RTVFormats[i] = RTVFormats[i];
        for (UINT i = NumRTVs; i < m_PSODesc.NumRenderTargets; ++i)
            m_PSODesc.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
        m_PSODesc.NumRenderTargets = NumRTVs;
        m_PSODesc.DSVFormat = DSVFormat;
        m_PSODesc.SampleDesc.Count = MsaaCount;
        m_PSODesc.SampleDesc.Quality = MsaaQuality;
    }

    void TbeD3DGraphicsPipelineState::SetInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>&& pInputElementDescs)
    {
        m_InputLayouts = pInputElementDescs;
        m_PSODesc.InputLayout.NumElements = m_InputLayouts.size();
        m_PSODesc.InputLayout.pInputElementDescs = m_InputLayouts.data();

    }

    void TbeD3DGraphicsPipelineState::Finalize()
    {
        // Make sure the root signature is finalized first
        m_PSODesc.pRootSignature = m_RootSignature;
        CAULDRON_DX12::Device::GetDevice().GetD3DDevice()->CreateGraphicsPipelineState(&m_PSODesc,IID_PPV_ARGS(&m_PSO));
    }

    TbeD3DGraphicsPipelineState DefaultGraphicsPSO;


    void InitBuiltInPSOs()
    {
        //1.Default
        DefaultGraphicsPSO = {};
        DefaultGraphicsPSO.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
        DefaultGraphicsPSO.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
        DefaultGraphicsPSO.SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
        DefaultGraphicsPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
        DefaultGraphicsPSO.SetSampleMask(UINT_MAX);
    }

}