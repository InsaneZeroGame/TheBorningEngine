#pragma once
#include "../DX12/base/Device.h"

namespace TBE
{
    using namespace CAULDRON_DX12;

	class TbeD3DResource : public NoCopy
	{
	public:
		TbeD3DResource();
		~TbeD3DResource();

		ID3D12Resource* operator->() { return m_pResource; }
		const ID3D12Resource* operator->() const { return m_pResource; }

		ID3D12Resource* GetResource() { return m_pResource; }
		const ID3D12Resource* GetResource() const { return m_pResource; }

		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_GpuVirtualAddress; }


	protected:
		ID3D12Resource* m_pResource = nullptr;
		D3D12_RESOURCE_STATES m_UsageState = D3D12_RESOURCE_STATE_COMMON;
		D3D12_RESOURCE_STATES m_TransitioningState = D3D12_RESOURCE_STATE_COMMON;
		D3D12_GPU_VIRTUAL_ADDRESS m_GpuVirtualAddress = 0;
        Device* m_device = nullptr;

	};

    class TbeD3DGpuBuffer : public TbeD3DResource
    {
    public:
        virtual ~TbeD3DGpuBuffer() { }

        // Create a buffer.  If initial data is provided, it will be copied into the buffer using the default command context.
        void Create(const std::wstring& name, uint32_t NumElements, uint32_t ElementSize,
            const void* initialData = nullptr);

        // Sub-Allocate a buffer out of a pre-allocated heap.  If initial data is provided, it will be copied into the buffer using the default command context.
        void CreatePlaced(const std::wstring& name, ID3D12Heap* pBackingHeap, uint32_t HeapOffset, uint32_t NumElements, uint32_t ElementSize,
            const void* initialData = nullptr);

        const D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV(void) const { return m_UAV; }
        const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV(void) const { return m_SRV; }

        D3D12_GPU_VIRTUAL_ADDRESS RootConstantBufferView(void) const { return m_GpuVirtualAddress; }

        D3D12_CPU_DESCRIPTOR_HANDLE CreateConstantBufferView(uint32_t Offset, uint32_t Size) const;

        D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t Offset, uint32_t Size, uint32_t Stride) const;
        D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t BaseVertexIndex = 0) const
        {
            size_t Offset = BaseVertexIndex * m_ElementSize;
            return VertexBufferView(Offset, (uint32_t)(m_BufferSize - Offset), m_ElementSize);
        }

        D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t Offset, uint32_t Size, bool b32Bit = false) const;
        D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t StartIndex = 0) const
        {
            size_t Offset = StartIndex * m_ElementSize;
            return IndexBufferView(Offset, (uint32_t)(m_BufferSize - Offset), m_ElementSize == 4);
        }

        size_t GetBufferSize() const { return m_BufferSize; }
        uint32_t GetElementCount() const { return m_ElementCount; }
        uint32_t GetElementSize() const { return m_ElementSize; }

    protected:

        TbeD3DGpuBuffer(void) : m_BufferSize(0), m_ElementCount(0), m_ElementSize(0)
        {
            m_ResourceFlags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            m_UAV.ptr = 0;
            m_SRV.ptr = 0;
        }

        D3D12_RESOURCE_DESC DescribeBuffer(void);
        virtual void CreateDerivedViews(void) = 0;

        D3D12_CPU_DESCRIPTOR_HANDLE m_UAV;
        D3D12_CPU_DESCRIPTOR_HANDLE m_SRV;

        size_t m_BufferSize;
        uint32_t m_ElementCount;
        uint32_t m_ElementSize;
        D3D12_RESOURCE_FLAGS m_ResourceFlags;
    };

    inline D3D12_VERTEX_BUFFER_VIEW TbeD3DGpuBuffer::VertexBufferView(size_t Offset, uint32_t Size, uint32_t Stride) const
    {
        D3D12_VERTEX_BUFFER_VIEW VBView;
        VBView.BufferLocation = m_GpuVirtualAddress + Offset;
        VBView.SizeInBytes = Size;
        VBView.StrideInBytes = Stride;
        return VBView;
    }

    inline D3D12_INDEX_BUFFER_VIEW TbeD3DGpuBuffer::IndexBufferView(size_t Offset, uint32_t Size, bool b32Bit) const
    {
        D3D12_INDEX_BUFFER_VIEW IBView;
        IBView.BufferLocation = m_GpuVirtualAddress + Offset;
        IBView.Format = b32Bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
        IBView.SizeInBytes = Size;
        return IBView;
    }
}