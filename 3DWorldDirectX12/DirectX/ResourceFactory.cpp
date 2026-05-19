#include"ResourceFactory.h"

//@brief	=== リソース作成関数 ===
//@param	heap_properties	ヒープの設定
//@param	resource_desc	リソースの設定
//@param	作成したリソース
static std::optional<Microsoft::WRL::ComPtr<ID3D12Resource>> create_resorce(
	D3D12_HEAP_PROPERTIES& heap_properties, D3D12_RESOURCE_DESC& resource_desc) {

    Microsoft::WRL::ComPtr<ID3D12Resource> resource{};

    auto hr = Device::Instance().get_device()->CreateCommittedResource(
        &heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &resource_desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&resource)
    );
    if (FAILED(hr)) {
        return std::nullopt;
    }

    return resource;
}