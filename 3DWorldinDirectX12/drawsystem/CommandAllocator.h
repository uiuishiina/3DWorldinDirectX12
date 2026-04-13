#pragma once
#include"Device.h"
#include<vector>

class CommandAllocator final
{
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> Allocator_{};
public:
	[[nodiscard]] bool Create(const int BufferSize);

	[[nodiscard]] ID3D12CommandAllocator* GetAllocator(const int BufferIndex)const noexcept;
};