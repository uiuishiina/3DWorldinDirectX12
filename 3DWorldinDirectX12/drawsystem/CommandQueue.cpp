#include"CommandQueue.h"
#include"../debug/debugsystem.h"

[[nodiscard]] bool CommandQueue::Create() {

    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    const auto hr = Device::Instance().GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&Queue_));
    DEBUG_HR_ASSERT(hr);
    if (FAILED(hr)) {
        return false;
    }
    return true;
}

[[nodiscard]] ID3D12CommandQueue* CommandQueue::GetQueue()const noexcept {
    DEBUG_ASSERT(Queue_);
    return Queue_.Get();
}