#include"RTVHeapManager.h"
#include"DescriptorHeap.h"
#include"../debug/debugsystem.h"

RTVHeapManager::RTVHeapManager() = default;
RTVHeapManager::~RTVHeapManager() = default;

[[nodiscard]] bool RTVHeapManager::CreateRTV(int FrameBufferSize) {
	if (RTVHeap_) {
		DEBUG_LOG_WARNING("Already Creation RTV");
		return false;
	}

	RTVHeap_ = std::make_unique<DescriptorHeap>();
	if (!RTVHeap_->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FrameBufferSize,false)) {
		DEBUG_LOG_ERROR("RTV Creation Failed")
		return false;
	}
	return true;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RTVHeapManager::GetHandle(int FrameBufferIndex)noexcept {
	DEBUG_ASSERT(RTVHeap_.get());
	auto handle_ = RTVHeap_->GetCPUHandle();
	handle_.ptr += FrameBufferIndex * RTVHeap_->GetHeapMemorySize();
	return handle_;
}