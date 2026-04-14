#include"DescriptorHeap.h"
#include"../debug/debugsystem.h"

[[nodiscard]] bool DescriptorHeap::Create(D3D12_DESCRIPTOR_HEAP_TYPE type, int HeapSize, bool shadervisible) {
    D3D12_DESCRIPTOR_HEAP_DESC Desc{};
    Desc.Type = type;
    Desc.NumDescriptors = HeapSize;
    Desc.Flags = shadervisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    HRESULT hr = Device::Instance().GetDevice()->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&Heap_));
    DEBUG_HR_ASSERT(hr);
    if (FAILED(hr)) {
        return false;
    }
    HeapMemorySize_ = Device::Instance().GetDevice()->GetDescriptorHandleIncrementSize(type);

    return true;
}

[[nodiscard]] ID3D12DescriptorHeap* DescriptorHeap::GetHeap()const noexcept {
    DEBUG_ASSERT(Heap_);
    return Heap_.Get();
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUHandle()const noexcept {
    DEBUG_ASSERT(Heap_);
    return Heap_->GetCPUDescriptorHandleForHeapStart();
}

[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUHandle()const noexcept {
    DEBUG_ASSERT(Heap_);
    return Heap_->GetGPUDescriptorHandleForHeapStart();
}

[[nodiscard]] UINT DescriptorHeap::GetHeapMemorySize()const noexcept {
    DEBUG_ASSERT(Heap_);
    return HeapMemorySize_;
}