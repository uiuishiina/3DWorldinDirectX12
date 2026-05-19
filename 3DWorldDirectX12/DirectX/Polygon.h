#pragma once
#include<d3d12.h>
#include<wrl/client.h>
#include<DirectXMath.h>

//@brief    /=== ポリゴンクラス ===/
class Polygon final
{
public:
    //----------------------------------------------------------------------------------------------------

    //@brief    === ポリゴン初期化関数 ===
    //@return   ポリゴン作成の成否
    [[nodiscard]] bool initialize_Polygon();


    //@brief    === ポリゴン描画関数 
    //@param    command_list    描画用コマンドリスト
    void draw_polygon(ID3D12GraphicsCommandList* command_list) const noexcept;

    //----------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------

    struct Vertex {
        DirectX::XMFLOAT3 pos;
    };

    Microsoft::WRL::ComPtr<ID3D12Resource>      vertex_buffer_;
    Microsoft::WRL::ComPtr<ID3D12Resource>      index_buffer_;

    D3D12_VERTEX_BUFFER_VIEW    vertex_buffer_view_{};
    D3D12_INDEX_BUFFER_VIEW     index_buffer_view_{};

    D3D_PRIMITIVE_TOPOLOGY   topology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

    Vertex vertices_[4] = {
    {{ -0.5f,  0.5f, 0.0f } },
    {{  0.5f,  0.5f, 0.0f } },
    {{ -0.5f, -0.5f, 0.0f } },
    {{  0.5f, -0.5f, 0.0f } }
    };

    UINT indices_[4] = { 0, 1, 2, 3 };

    //@brief    === 頂点作成関数 ===
    //@return   作成の成否
    [[nodiscard]] bool create_vertex_buffer();

    //@brief    === インデックス作成関数 ===
    //@brief    作成の成否
    [[nodiscard]] bool create_index_buffer();

    //----------------------------------------------------------------------------------------------------
};