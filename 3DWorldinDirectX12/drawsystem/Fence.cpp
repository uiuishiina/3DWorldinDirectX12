#include"Fence.h"
#include"../debug/debugsystem.h"

[[nodiscard]] bool Fence::Create() {
	// ƒtƒFƒ“ƒX‚Ì¶¬
	const auto hr = Device::Instance().GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence_));
	DEBUG_HR_ASSERT(hr);
	if (FAILED(hr)) {
		return false;
	}
	
	WaitEventHandle_ = CreateEvent(nullptr, false, false, "WAIT_GPU");
	if (!WaitEventHandle_) {
		return false;
	}
	return true;
}

[[nodiscard]] ID3D12Fence* Fence::Get()const noexcept {
	DEBUG_ASSERT(Fence_);
	return Fence_.Get();
}

void Fence::WaitEvent(UINT64 fenceValue)const noexcept {
	DEBUG_ASSERT(Fence_);
	if (!Fence_) {
		return;
	}

	if (Fence_->GetCompletedValue() < fenceValue) {
		Fence_->SetEventOnCompletion(fenceValue, WaitEventHandle_);
		WaitForSingleObject(WaitEventHandle_, INFINITE);
	}
}