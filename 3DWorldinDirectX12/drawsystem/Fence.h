#pragma once
#include"Device.h"

class Fence final
{
	Microsoft::WRL::ComPtr<ID3D12Fence> Fence_{};
	HANDLE WaitEventHandle_{};
public:
	Fence() = default;
	~Fence() = default;

	[[nodiscard]] bool Create();

	[[nodiscard]] ID3D12Fence* Get()const noexcept;

	void WaitEvent(UINT64 fenceValue)const noexcept;
};