#pragma once
#include"Device.h"

class RootSignature final
{
	Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature_{};
public:
	[[nodiscard]] bool Create();

	[[nodiscard]] ID3D12RootSignature* Get()const noexcept;
};