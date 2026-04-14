#pragma once
#include"Device.h"
#include<memory>

class DescriptorHeap;

class RTVHeapManager final
{
	std::unique_ptr<DescriptorHeap> RTVHeap_{};
public:
	RTVHeapManager();
	~RTVHeapManager();

	[[nodiscard]] bool CreateRTV(int FrameBufferSize);

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(int FrameBufferIndex)noexcept;
};