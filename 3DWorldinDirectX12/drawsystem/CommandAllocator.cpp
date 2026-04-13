#include"CommandAllocator.h"
#include"../debug/debugsystem.h"

[[nodiscard]] bool CommandAllocator::Create(const int BufferSize) {

    Allocator_.resize(BufferSize);
    for (int i = 0; i < BufferSize; i++) {
        const auto hr = Device::Instance().GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator_[0]));
        DEBUG_HR_ASSERT(hr);
        if (FAILED(hr)) {
            return false;
        }
    }
    
    return true;
}

[[nodiscard]] ID3D12CommandAllocator* CommandAllocator::GetAllocator(const int BufferIndex)const noexcept {
    DEBUG_ASSERT(!Allocator_.empty());
    DEBUG_ASSERT(Allocator_.size() > BufferIndex);
    DEBUG_ASSERT(Allocator_[BufferIndex]);
    return Allocator_[BufferIndex].Get();
}