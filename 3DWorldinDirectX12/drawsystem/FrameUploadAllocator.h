#pragma once
#include"Device.h"
#include<unordered_map>
#include<memory>
#include<optional>

struct AllocateHeap
{
	size_t IndexNum_; 
	D3D12_CPU_DESCRIPTOR_HANDLE CpuHand_;
	D3D12_GPU_DESCRIPTOR_HANDLE GpuHand_;

	AllocateHeap() = default;
	AllocateHeap(size_t index, D3D12_CPU_DESCRIPTOR_HANDLE cpu, D3D12_GPU_DESCRIPTOR_HANDLE gpu) :
		IndexNum_(index), CpuHand_(cpu), GpuHand_(gpu) {};
};

class DescriptorHeap;
class FrameResource;
class FrameBasedLinearAllocator;
class FrameUploadAllocator final
{
	FrameUploadAllocator();
	~FrameUploadAllocator();

	std::unique_ptr<DescriptorHeap> UpLoadHeap_{};
	std::unique_ptr<FrameResource>	UpLoadResource_{};
	std::unique_ptr<FrameBasedLinearAllocator> LinearAllocator_{};
public:
	static FrameUploadAllocator& Instance()noexcept {
		static FrameUploadAllocator ins;
		return ins;
	}

	[[nodiscard]] bool CreationInstructions(int HeapSize);

	[[nodiscard]] std::optional<AllocateHeap> AllocateHeapInstructions();

	void Signal(UINT64 signal);

	void ResetFrameBuffer(UINT64 signal);
};