#pragma once
#include"Device.h"

class CommandQueue final
{
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> Queue_{};
public:
	CommandQueue() = default;
	~CommandQueue() = default;

	[[nodiscard]] bool Create();

	[[nodiscard]] ID3D12CommandQueue* GetQueue()const noexcept;
};