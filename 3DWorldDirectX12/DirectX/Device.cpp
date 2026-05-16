#include"Device.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

using Microsoft::WRL::ComPtr;

//----------------------------------------------------------------------------------------------------

namespace {
    //    要求するD3D機能レベルのリスト...DirectX 12.2からDirectX 11.0までの機能レベルを指定
    D3D_FEATURE_LEVEL levels[] = {
    D3D_FEATURE_LEVEL_12_2,    //    49664
    D3D_FEATURE_LEVEL_12_1,    //    49408
    D3D_FEATURE_LEVEL_12_0,    //    49152
    D3D_FEATURE_LEVEL_11_1,    //    45056
    D3D_FEATURE_LEVEL_11_0    //    40960
    };
}

//----------------------------------------------------------------------------------------------------

//@brief	=== Device作成関数 ===
//@return	作成の可否
[[nodiscard]] bool Device::create_device(IDXGIAdapter4* adapter) {

    //リストに定義してあるレベルを試す
    for (const auto& level : levels) {
        device_ = select_device_by_level(adapter, level);
        if (device_) {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== Device選定関数 ===
//@return	作成したデバイス
[[nodiscard]] ComPtr<ID3D12Device> Device::select_device_by_level(IDXGIAdapter4* adapter, D3D_FEATURE_LEVEL featureLevel) {
    ComPtr<ID3D12Device> device;
    const auto hr = D3D12CreateDevice(
        adapter,
        featureLevel,
        IID_PPV_ARGS(&device)
    );
    if (FAILED(hr)) {
        return nullptr;
    }
    return device;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== Device取得関数 ===
//@return	ID3D12Deviceポインター
[[nodiscard]] ID3D12Device* Device::get_device() const noexcept {
    return device_.Get();
}