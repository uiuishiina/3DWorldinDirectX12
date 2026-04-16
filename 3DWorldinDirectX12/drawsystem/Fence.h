#pragma once
#include"Device.h"
#include<memory>

class Fence;
class FenceManager final
{
	FenceManager();
	~FenceManager();
	
	UINT64 NextFenceValue_{};
	std::unique_ptr<Fence> Fence_{};

public:
	static FenceManager& Instance()noexcept {
		static FenceManager ins;
		return ins;
	}

	[[nodiscard]] bool Initialize();

	[[nodiscard]] UINT64 Signal(ID3D12CommandQueue* Queue)noexcept;

	void WaitEvent(UINT64 fenceValue)noexcept;

	[[nodiscard]] ID3D12Fence* GetFence()const noexcept;

	[[nodiscard]] UINT64 GetCompletedValue()const noexcept;
};