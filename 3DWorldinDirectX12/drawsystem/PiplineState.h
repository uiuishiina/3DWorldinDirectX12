#pragma once
#include"Device.h"

class PiplineState final
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> Pipline_{};
public:
	[[nodiscard]] bool Create(ID3DBlob* vs, ID3DBlob* ps, ID3D12RootSignature* root);

	[[nodiscard]] ID3D12PipelineState* Get()const noexcept;
};