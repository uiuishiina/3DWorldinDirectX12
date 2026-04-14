#pragma once
#include"Device.h"

class DescriptorHeap final
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Heap_{};
	UINT HeapMemorySize_{};
public:
	DescriptorHeap() = default;
	~DescriptorHeap() = default;

	[[nodiscard]] bool Create(D3D12_DESCRIPTOR_HEAP_TYPE type, int HeapSize, bool shadervisible);

	[[nodiscard]] ID3D12DescriptorHeap* GetHeap()const noexcept;

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle()const noexcept;

	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle()const noexcept;

	[[nodiscard]] UINT GetHeapMemorySize()const noexcept;
};