#include"CommandList.h"
#include"../debug/debugsystem.h"

[[nodiscard]] bool CommandList::Create(ID3D12CommandAllocator* Allocator) {
    const auto hr = Device::Instance().GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, nullptr, IID_PPV_ARGS(&List_));
    DEBUG_HR_ASSERT(hr);
    if (FAILED(hr)) {
        return false;
    }

    List_->Close();
    return true;
}

[[nodiscard]] ID3D12GraphicsCommandList* CommandList::GetList()const noexcept {
    DEBUG_ASSERT(List_);
    return List_.Get();
}

void CommandList::Reset(ID3D12CommandAllocator* allocator)noexcept {
    DEBUG_ASSERT(List_);
    List_->Reset(allocator, nullptr);
}