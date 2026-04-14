#include"SwapChain.h"
#include"../debug/debugsystem.h"

[[nodiscard]] bool SwapChain::Create(ID3D12CommandQueue* Queue, HWND HWND, int width, int height, int FrameBufferSize) {
    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.BufferCount    = FrameBufferSize;
    desc.Width          = width;
    desc.Height         = height;
    desc.Format         = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage    = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect     = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain{};
    {
        const auto hr = Device::Instance().GetFactory()->CreateSwapChainForHwnd(
            Queue, HWND,
            &desc, nullptr, nullptr, tempSwapChain.GetAddressOf());
        DEBUG_HR_ASSERT(hr);
        if (FAILED(hr)) {
            return false;
        }
    }
    {
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&SwapChain_));
        DEBUG_HR_ASSERT(hr);
        if (FAILED(hr)) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] IDXGISwapChain3* SwapChain::Get()const noexcept {
    DEBUG_ASSERT(SwapChain_);
    return SwapChain_.Get();
}