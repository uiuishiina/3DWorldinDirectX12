#include"Polygon.h"
#include"../debug/debugsystem.h"
#include <DirectXMath.h>

struct Vertex
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
};

[[nodiscard]] bool PolygonBase::Create() {
	if (!CreateVertex()) {
		return false;
	}
	if (!CreateIndex()) {
		return false;
	}
	return true;
}

bool PolygonBase::CreateVertex() {
    
    Vertex triangleVertices[] = {
        {  {0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        { {0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
    };

    const auto vertexBufferSize = sizeof(triangleVertices);

    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = vertexBufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    {
        const auto hr = Device::Instance().GetDevice()->CreateCommittedResource(
            &heapProperty,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&VertexBuffer_));
        if (FAILED(hr)) {
            return false;
        }
    }

    
    Vertex* data{};

    const auto hr = VertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    DEBUG_HR_ASSERT(hr);
    if (FAILED(hr)) {
        return false;
    }

    memcpy_s(data, vertexBufferSize, triangleVertices, vertexBufferSize);

    VertexBuffer_->Unmap(0, nullptr);

    VertexBufferView_.BufferLocation    = VertexBuffer_->GetGPUVirtualAddress();
    VertexBufferView_.SizeInBytes       = vertexBufferSize;
    VertexBufferView_.StrideInBytes     = sizeof(Vertex);

    return true;
}
bool PolygonBase::CreateIndex() {
    uint16_t triangleIndices[] = {
        0, 1, 2
    };

    const auto indexBufferSize = sizeof(triangleIndices);

    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = indexBufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    {
        const auto hr = Device::Instance().GetDevice()->CreateCommittedResource(
            &heapProperty,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&IndexBuffer_));
        DEBUG_HR_ASSERT(hr);
        if (FAILED(hr)) {
            return false;
        }
    }

    uint16_t* data{};
    const auto hr = IndexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    DEBUG_HR_ASSERT(hr);
    if (FAILED(hr)) {
        return false;
    }

    memcpy_s(data, indexBufferSize, triangleIndices, indexBufferSize);

    IndexBuffer_->Unmap(0, nullptr);

    IndexBufferView_.BufferLocation = IndexBuffer_->GetGPUVirtualAddress();
    IndexBufferView_.SizeInBytes = indexBufferSize;
    IndexBufferView_.Format = DXGI_FORMAT_R16_UINT;

    return true;
}

void PolygonBase::Draw(ID3D12GraphicsCommandList* list)noexcept {
    // 頂点バッファの設定
    list->IASetVertexBuffers(0, 1, &VertexBufferView_);
    // インデックスバッファの設定
    list->IASetIndexBuffer(&IndexBufferView_);
    // プリミティブ形状の設定（三角形）
    list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // 描画コマンド
    list->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

[[nodiscard]] ID3D12Resource* PolygonBase::GetVertex()const noexcept {
	DEBUG_ASSERT(VertexBuffer_);
	return VertexBuffer_.Get();
}
[[nodiscard]] ID3D12Resource* PolygonBase::GetIndex()const noexcept {
	DEBUG_ASSERT(IndexBuffer_);
	return IndexBuffer_.Get();
}