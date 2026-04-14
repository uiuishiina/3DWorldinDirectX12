#pragma once
#include"Device.h"
#include<unordered_map>
#include<memory>
#include<optional>

class DescriptorHeap;

class DescriptorHeapManager final
{
	DescriptorHeapManager();
	~DescriptorHeapManager();

	std::unordered_map<D3D12_DESCRIPTOR_HEAP_TYPE, std::unique_ptr<DescriptorHeap>> HeapMap_{};
public:
	static DescriptorHeapManager& Instance()noexcept {
		static DescriptorHeapManager ins;
		return ins;
	}

	[[nodiscard]] bool CreationInstructions(D3D12_DESCRIPTOR_HEAP_TYPE type, int HeapSize, int FrameBufferSize);

	[[nodiscard]] std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> AllocateAndGetHandle(D3D12_DESCRIPTOR_HEAP_TYPE type)noexcept;
};
