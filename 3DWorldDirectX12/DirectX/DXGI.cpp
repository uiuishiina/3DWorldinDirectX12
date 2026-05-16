#include"DXGI.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

using Microsoft::WRL::ComPtr;

//----------------------------------------------------------------------------------------------------
namespace {
    //    要求するGPUの優先度のリスト...高性能GPU、デフォルトGPU、最低消費電力GPUの順で指定    
    DXGI_GPU_PREFERENCE preferences[] = {
        DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,   //    2
        DXGI_GPU_PREFERENCE_UNSPECIFIED,        //    0
        DXGI_GPU_PREFERENCE_MINIMUM_POWER       //    1
    };
}

//----------------------------------------------------------------------------------------------------

//@brief	=== DXGI初期化関数 ===
//@return	初期化の可否
[[nodiscard]] bool DXGI::initialize_DXGI() {
    if (!create_factory()) {
        return false;
    }
    if (!pick_adaptor()) {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== DXGIFactory作成関数 ===
//@return	作成の可否
[[nodiscard]] bool DXGI::create_factory() {
#if _DEBUG
    ComPtr<ID3D12Debug> debug{};
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
        debug->EnableDebugLayer();
    }
#endif
    UINT Flags = 0;
#if _DEBUG
    Flags |= DXGI_CREATE_FACTORY_DEBUG;    // デバッグフラグを設定
#endif
    const auto hr = CreateDXGIFactory2(Flags, IID_PPV_ARGS(&factory_));
    if (FAILED(hr)) {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	===  DXGIAdaptor選定関数  ===
//@return	選定の可否
[[nodiscard]] bool DXGI::pick_adaptor() {
    //    要求するGPUの優先度のリストを順に試して、最初に見つかった適切なGPUを選択
    for (const auto preference : preferences) {
        adaptor_ = find_adaptor(preference);
        if (adaptor_) {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== DXGIAdaptor条件探索関数 ===
//@brief	条件に合うAdaptor
[[nodiscard]] ComPtr<IDXGIAdapter4> DXGI::find_adaptor(DXGI_GPU_PREFERENCE preference) {
    for (int i = 0; ; ++i) {
        DXGI_ADAPTER_DESC1 desc{};
        ComPtr<IDXGIAdapter4> dxgiAdapter{};

        // GPUの優先度設定してアダプタを列挙
        if (FAILED(factory_->EnumAdapterByGpuPreference(i, preference, IID_PPV_ARGS(&dxgiAdapter)))) {
            break;
        }

        dxgiAdapter->GetDesc1(&desc);
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        return dxgiAdapter;
    }
    return nullptr;
}

//----------------------------------------------------------------------------------------------------

//@brief	===  DXGIFactory取得関数  ===
//@return	DXGIFactoryのポインター
[[nodiscard]] IDXGIFactory6* DXGI::get_factory() const noexcept {
    return factory_.Get();
}

//----------------------------------------------------------------------------------------------------

//@brief	===  DXGIAdaptor取得関数  ===
//@return	DXGIAdaptorのポインター
[[nodiscard]] IDXGIAdapter4* DXGI::get_adaptor() const noexcept {
    return adaptor_.Get();
}