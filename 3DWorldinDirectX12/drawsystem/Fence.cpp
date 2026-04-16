#include"Fence.h"
#include"../debug/debugsystem.h"

class Fence final
{
	Microsoft::WRL::ComPtr<ID3D12Fence> Fence_{};
	HANDLE WaitEventHandle_{};
public:
	Fence() = default;
	~Fence() {
		if (WaitEventHandle_) {
			CloseHandle(WaitEventHandle_);
			WaitEventHandle_ = nullptr;
		}
	}

	[[nodiscard]] bool Create() {
		// ƒtƒFƒ“ƒX‚Ì¶¬
		const auto hr = Device::Instance().GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence_));
		DEBUG_HR_ASSERT(hr);
		if (FAILED(hr)) {
			return false;
		}

		WaitEventHandle_ = CreateEvent(nullptr, false, false, nullptr);
		if (!WaitEventHandle_) {
			return false;
		}
		return true;
	}

	[[nodiscard]] ID3D12Fence* Get()const noexcept {
		DEBUG_ASSERT(Fence_);
		return Fence_.Get();
	}

	void WaitEvent(UINT64 fenceValue)const noexcept {
		DEBUG_ASSERT(Fence_);
		if (!Fence_) {
			return;
		}

		if (Fence_->GetCompletedValue() < fenceValue) {
			Fence_->SetEventOnCompletion(fenceValue, WaitEventHandle_);
			WaitForSingleObject(WaitEventHandle_, INFINITE);
		}
	}
};



FenceManager::FenceManager() = default;
FenceManager::~FenceManager() = default;

[[nodiscard]] bool FenceManager::Initialize() {
	if (Fence_) {
		DEBUG_LOG_WARNING("Already Creation Fence_");
		return false;
	}

	Fence_ = std::make_unique<Fence>();
	if (!Fence_->Create()) {
		return false;
	}
	return true;
}

UINT64 FenceManager::Signal(ID3D12CommandQueue* Queue)noexcept {
	DEBUG_ASSERT(Queue);
	DEBUG_ASSERT(Fence_.get());
	NextFenceValue_++;
	Queue->Signal(Fence_->Get(), NextFenceValue_);
	return NextFenceValue_;
}

void FenceManager::WaitEvent(UINT64 fenceValue)noexcept {
	DEBUG_ASSERT(Fence_.get());
	Fence_->WaitEvent(fenceValue);
}

[[nodiscard]] ID3D12Fence* FenceManager::GetFence()const noexcept {
	DEBUG_ASSERT(Fence_.get());
	return Fence_->Get();
}

[[nodiscard]] UINT64 FenceManager::GetCompletedValue()const noexcept {
	DEBUG_ASSERT(Fence_.get());
	return Fence_->Get()->GetCompletedValue();
}