#pragma once
#include"Device.h"

class PolygonBase final
{
    Microsoft::WRL::ComPtr<ID3D12Resource> VertexBuffer_{};
    Microsoft::WRL::ComPtr<ID3D12Resource> IndexBuffer_{};

    D3D12_VERTEX_BUFFER_VIEW VertexBufferView_ = {};
    D3D12_INDEX_BUFFER_VIEW  IndexBufferView_ = {};

    bool CreateVertex();
    bool CreateIndex();
public:
    PolygonBase() = default;
    ~PolygonBase() = default;

    [[nodiscard]] bool Create();
    void Draw(ID3D12GraphicsCommandList* list)noexcept;

    [[nodiscard]] ID3D12Resource* GetVertex()const noexcept;
    [[nodiscard]] ID3D12Resource* GetIndex()const noexcept;
};