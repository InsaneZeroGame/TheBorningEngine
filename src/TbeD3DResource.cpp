#include "stdafx.h"
#include "TbeD3DResource.h"
#include "../DX12/base/ResourceViewHeaps.h"

TBE::TbeD3DResource::TbeD3DResource()
{
    m_device = &Device::GetDevice();
}

TBE::TbeD3DResource::~TbeD3DResource()
{
	m_pResource = nullptr;
	m_GpuVirtualAddress = 0;
}

namespace TBE
{
    void TbeD3DGpuBuffer::Create(const std::wstring& name, uint32_t NumElements, uint32_t ElementSize, const void* initialData)
    {

        m_ElementCount = NumElements;
        m_ElementSize = ElementSize;
        m_BufferSize = NumElements * ElementSize;

        D3D12_RESOURCE_DESC ResourceDesc = DescribeBuffer();

        m_UsageState = D3D12_RESOURCE_STATE_COMMON;

        D3D12_HEAP_PROPERTIES HeapProps;
        HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        HeapProps.CreationNodeMask = 1;
        HeapProps.VisibleNodeMask = 1;

        m_device->GetD3DDevice()->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE,
                &ResourceDesc, m_UsageState, nullptr, IID_PPV_ARGS(&m_pResource));

        m_GpuVirtualAddress = m_pResource->GetGPUVirtualAddress();

        if (initialData)
            //CommandContext::InitializeBuffer(*this, initialData, m_BufferSize);

#ifdef RELEASE
        (name);
#else
        m_pResource->SetName(name.c_str());
#endif

        CreateDerivedViews();
    }

    // Sub-Allocate a buffer out of a pre-allocated heap.  If initial data is provided, it will be copied into the buffer using the default command context.
    void TbeD3DGpuBuffer::CreatePlaced(const std::wstring& name, ID3D12Heap* pBackingHeap, uint32_t HeapOffset, uint32_t NumElements, uint32_t ElementSize,
        const void* initialData)
    {
        m_ElementCount = NumElements;
        m_ElementSize = ElementSize;
        m_BufferSize = NumElements * ElementSize;

        D3D12_RESOURCE_DESC ResourceDesc = DescribeBuffer();

        m_UsageState = D3D12_RESOURCE_STATE_COMMON;

       m_device->GetD3DDevice()->CreatePlacedResource(pBackingHeap, HeapOffset, &ResourceDesc, m_UsageState, nullptr, IID_PPV_ARGS(&m_pResource));

        m_GpuVirtualAddress = m_pResource->GetGPUVirtualAddress();

        if (initialData)
            //CommandContext::InitializeBuffer(*this, initialData, m_BufferSize);

#ifdef RELEASE
        (name);
#else
        m_pResource->SetName(name.c_str());
#endif

        CreateDerivedViews();

    }

    D3D12_CPU_DESCRIPTOR_HANDLE TbeD3DGpuBuffer::CreateConstantBufferView(uint32_t Offset, uint32_t Size) const
    {
        assert(Offset + Size <= m_BufferSize);

        assert(0 && "Fix Below");
        //Size = Math::AlignUp(Size, 16);

        D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
        CBVDesc.BufferLocation = m_GpuVirtualAddress + (size_t)Offset;
        CBVDesc.SizeInBytes = Size;

        D3D12_CPU_DESCRIPTOR_HANDLE hCBV = ResourceViewHeaps::GetViewHeaps().AllocCBV_SRV_UAVDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)->GetCPU();
        m_device->GetD3DDevice()->CreateConstantBufferView(&CBVDesc, hCBV);
        return hCBV;
    }

    D3D12_RESOURCE_DESC TbeD3DGpuBuffer::DescribeBuffer(void)
    {
        assert(m_BufferSize != 0);

        D3D12_RESOURCE_DESC Desc = {};
        Desc.Alignment = 0;
        Desc.DepthOrArraySize = 1;
        Desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        Desc.Flags = m_ResourceFlags;
        Desc.Format = DXGI_FORMAT_UNKNOWN;
        Desc.Height = 1;
        Desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        Desc.MipLevels = 1;
        Desc.SampleDesc.Count = 1;
        Desc.SampleDesc.Quality = 0;
        Desc.Width = (UINT64)m_BufferSize;
        return Desc;
    }
    void TbeD3DStaticBuffer::CreateDerivedViews(void)
    {
    }


    void TbeD3DDynamicUploadBuffer::Create(const std::wstring& name, uint32_t NumElements, uint32_t ElementSize)
    {
        D3D12_HEAP_PROPERTIES HeapProps;
        HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        HeapProps.CreationNodeMask = 1;
        HeapProps.VisibleNodeMask = 1;
        HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC ResourceDesc;
        ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ResourceDesc.Alignment = 0;
        ResourceDesc.Height = 1;
        ResourceDesc.DepthOrArraySize = 1;
        ResourceDesc.MipLevels = 1;
        ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        ResourceDesc.SampleDesc.Count = 1;
        ResourceDesc.SampleDesc.Quality = 0;
        ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        ResourceDesc.Width = NumElements * ElementSize;
        ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        (Device::GetDevice().GetD3DDevice()->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pResource)));

        m_pResource->SetName(name.c_str());

        m_GpuVirtualAddress = m_pResource->GetGPUVirtualAddress();
        m_CpuVirtualAddress = nullptr;
    }

    void TbeD3DDynamicUploadBuffer::Destroy(void)
    {
        if (m_pResource != nullptr)
        {
            if (m_CpuVirtualAddress != nullptr)
                Unmap();

            m_pResource = nullptr;
            m_GpuVirtualAddress = 0;
        }
    }

    void* TbeD3DDynamicUploadBuffer::Map(void)
    {
        assert(m_CpuVirtualAddress == nullptr&& "Buffer is already locked");
        (m_pResource->Map(0, nullptr, &m_CpuVirtualAddress));
        return m_CpuVirtualAddress;
    }

    void TbeD3DDynamicUploadBuffer::Unmap(void)
    {
        assert(m_CpuVirtualAddress != nullptr&& "Buffer is not locked");
        m_pResource->Unmap(0, nullptr);
        m_CpuVirtualAddress = nullptr;
    }

    D3D12_VERTEX_BUFFER_VIEW TbeD3DDynamicUploadBuffer::VertexBufferView(uint32_t NumVertices, uint32_t Stride, uint32_t Offset) const
    {
        D3D12_VERTEX_BUFFER_VIEW vbv;
        vbv.BufferLocation = m_GpuVirtualAddress + Offset;
        vbv.SizeInBytes = NumVertices * Stride;
        vbv.StrideInBytes = Stride;
        return vbv;
    }

    D3D12_INDEX_BUFFER_VIEW TbeD3DDynamicUploadBuffer::IndexBufferView(uint32_t NumIndices, bool _32bit, uint32_t Offset) const
    {
        D3D12_INDEX_BUFFER_VIEW ibv;
        ibv.BufferLocation = m_GpuVirtualAddress + Offset;
        ibv.Format = _32bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
        ibv.SizeInBytes = NumIndices * (_32bit ? 4 : 2);
        return ibv;
    }



}
