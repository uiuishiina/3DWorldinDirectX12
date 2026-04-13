#pragma once
#include"Device.h"

class CommandList final
{
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> List_{};
public:
	[[nodiscard]] bool Create(ID3D12CommandAllocator* Allocator);

	[[nodiscard]] ID3D12GraphicsCommandList* GetList()const noexcept;
};