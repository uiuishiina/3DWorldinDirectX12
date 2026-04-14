#include"RTVHeapManager.h"
#include"RenderTarget.h"
#include"../debug/debugsystem.h"

RenderTarget::RenderTarget() = default;
RenderTarget::~RenderTarget() = default;

[[nodiscard]] bool RenderTarget::Create(IDXGISwapChain3* swapchain, int FrameBufferSize) {
    Manager_ = std::make_unique<RTVHeapManager>();
    if (!Manager_->CreateRTV(FrameBufferSize)) {
        return false;
    }
    
    Targets_.resize(FrameBufferSize);

    for (int i = 0; i < FrameBufferSize; ++i) {
        const auto hr = swapchain->GetBuffer(i, IID_PPV_ARGS(&Targets_[i]));
        DEBUG_HR_ASSERT(hr);
        if (FAILED(hr)) {
            return false;
        }
        auto handle = Manager_->GetHandle(i);
        Device::Instance().GetDevice()->CreateRenderTargetView(Targets_[i].Get(), nullptr, handle);
    }
    DEBUG_ASSERT(!Targets_.empty());

    return true;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetHandle(int FrameBufferIndex)noexcept {
    DEBUG_ASSERT(!Targets_.empty());
    DEBUG_ASSERT(Targets_.size() > FrameBufferIndex);
    DEBUG_ASSERT(Targets_[FrameBufferIndex]);
    return Manager_->GetHandle(FrameBufferIndex);
}

[[nodiscard]] ID3D12Resource* RenderTarget::Get(int FrameBufferIndex) const noexcept {
    DEBUG_ASSERT(!Targets_.empty());
    DEBUG_ASSERT(Targets_.size() > FrameBufferIndex);
    DEBUG_ASSERT(Targets_[FrameBufferIndex]);
    return Targets_[FrameBufferIndex].Get();
}