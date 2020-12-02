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
}
