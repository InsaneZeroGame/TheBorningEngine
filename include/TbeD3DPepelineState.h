#pragma once


namespace TBE
{
    class PSO
    {
    public:

        PSO() : m_RootSignature(nullptr) {}

        virtual ~PSO() {};

        ID3D12PipelineState* GetPipelineStateObject(void) const { return m_PSO; }

        __forceinline void SetRS(ID3D12RootSignature* p_rs) { m_RootSignature = p_rs; };

        __forceinline ID3D12RootSignature* GetRS() { return m_RootSignature; }
    protected:
        ID3D12RootSignature* m_RootSignature;

        ID3D12PipelineState* m_PSO;
    };


    class TbeD3DGraphicsPipelineState : public PSO
    {

    public:

        // Start with empty state
        TbeD3DGraphicsPipelineState();

        void SetBlendState(const D3D12_BLEND_DESC& BlendDesc);
        void SetRasterizerState(const D3D12_RASTERIZER_DESC& RasterizerDesc);
        void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& DepthStencilDesc);
        void SetSampleMask(UINT SampleMask);
        void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE TopologyType);
        void SetRenderTargetFormat(DXGI_FORMAT RTVFormat, DXGI_FORMAT DSVFormat, UINT MsaaCount = 1, UINT MsaaQuality = 0);
        void SetRenderTargetFormats(UINT NumRTVs, const DXGI_FORMAT* RTVFormats, DXGI_FORMAT DSVFormat, UINT MsaaCount = 1, UINT MsaaQuality = 0);
        void SetInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>&& pInputElementDescs);
        void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps);

        // These const_casts shouldn't be necessary, but we need to fix the API to accept "const void* pShaderBytecode"
        void SetVertexShader(const void* Binary, size_t Size) { m_PSODesc.VS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }
        void SetPixelShader(const void* Binary, size_t Size) { m_PSODesc.PS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }
        void SetGeometryShader(const void* Binary, size_t Size) { m_PSODesc.GS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }
        void SetHullShader(const void* Binary, size_t Size) { m_PSODesc.HS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }
        void SetDomainShader(const void* Binary, size_t Size) { m_PSODesc.DS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }

        void SetVertexShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.VS = Binary; }
        void SetPixelShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.PS = Binary; }
        void SetGeometryShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.GS = Binary; }
        void SetHullShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.HS = Binary; }
        void SetDomainShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.DS = Binary; }


        void SetVertexShader(ID3DBlob* Binary) { m_PSODesc.VS = CD3DX12_SHADER_BYTECODE(Binary); }
        void SetPixelShader(ID3DBlob* Binary) { m_PSODesc.PS = CD3DX12_SHADER_BYTECODE(Binary); }
        void SetGeometryShader(ID3DBlob* Binary) { m_PSODesc.GS = CD3DX12_SHADER_BYTECODE(Binary); }
        void SetHullShader(ID3DBlob* Binary) { m_PSODesc.HS = CD3DX12_SHADER_BYTECODE(Binary); }
        void SetDomainShader(ID3DBlob* Binary) { m_PSODesc.DS = CD3DX12_SHADER_BYTECODE(Binary); }

        // Perform validation and compute a hash value for fast state block comparisons
        void Finalize();

        D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetDesc() { return m_PSODesc; }

    private:

        D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PSODesc;
        std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayouts;
    };

    extern TbeD3DGraphicsPipelineState DefaultGraphicsPSO;

    extern void InitBuiltInPSOs();
}