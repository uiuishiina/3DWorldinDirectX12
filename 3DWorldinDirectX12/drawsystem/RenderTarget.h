#pragma once
#include"Device.h"
#include<vector>
#include<memory>

class RTVHeapManager;
class RenderTarget final
{
	std::unique_ptr<RTVHeapManager> Manager_{};
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> Targets_{};
public:
	RenderTarget();
	~RenderTarget();

	[[nodiscard]] bool Create(IDXGISwapChain3* swapchain, int FrameBufferSize);

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(int FrameBufferIndex)noexcept;

	[[nodiscard]] ID3D12Resource* Get(int FrameBufferIndex) const noexcept;
};