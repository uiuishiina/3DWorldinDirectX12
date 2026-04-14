#pragma once
#include"Device.h"

class SwapChain final
{
	Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain_{};
public:
	SwapChain() = default;
	~SwapChain() = default;

	[[nodiscard]] bool Create(ID3D12CommandQueue* Queue, HWND HWND, int width, int height, int FrameBufferSize);

	[[nodiscard]] IDXGISwapChain3* Get()const noexcept;
};