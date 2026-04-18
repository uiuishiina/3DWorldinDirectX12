#include"DescriptorHeap.h"
#include<queue>
#include"FrameUploadAllocator.h"
#include"Fence.h"
#include"../debug/debugsystem.h"

FrameUploadAllocator::FrameUploadAllocator() = default;
FrameUploadAllocator::~FrameUploadAllocator() = default;

class FrameBasedLinearAllocator final
{
	size_t HeapMaxSize_{};		//ヒープ最大数
	size_t UsableEndIndex_{};	//使用可能な最後尾のインデックス
	size_t TailIndex_{};		//使用している最後尾のインデックス
	size_t UsedSize_{};			//使用しているヒープサイズ

	struct ByFenceTailIndex
	{
		UINT64 FenceSignal_{};
		size_t Tail_{};
	};

	std::queue<ByFenceTailIndex> ByFrameBuffer_{};	//fence別の使用中最後尾インデックス

	bool IsFull()const noexcept {
		return UsedSize_ == HeapMaxSize_;
	}
	bool IsEmpty()const noexcept {
		return UsedSize_ == 0;
	}
public:
	FrameBasedLinearAllocator() = default;
	~FrameBasedLinearAllocator() = default;
	[[nodiscard]] bool Initialize(int HeapSize) {
		
		HeapMaxSize_ = HeapSize;
		UsableEndIndex_ = HeapSize - 1;
		return true;
	}

	//使用済みにプッシュ
	std::optional<size_t> GetHeapIndex() {
		if (IsFull()) {
			return std::nullopt;
		}
		const auto index = TailIndex_;
		TailIndex_ = (TailIndex_ + 1) % HeapMaxSize_;
		UsedSize_++;
		return index;
	}

	void PushSignal(UINT64 signal)noexcept {
		ByFenceTailIndex index = { signal,TailIndex_ };
		ByFrameBuffer_.push(index);
	}

	//使用済みから外す
	void ReturnHeapIndex(UINT64 signal) {
		if (ByFrameBuffer_.empty()) {
			return;
		}
		const auto index = ByFrameBuffer_.front();
		ByFrameBuffer_.pop();

		FenceManager::Instance().WaitEvent(index.FenceSignal_);
		UsableEndIndex_ = index.Tail_;
		if (TailIndex_ >= UsableEndIndex_) {
			UsedSize_ = TailIndex_ - UsableEndIndex_;
		}
		else {
			UsedSize_ = HeapMaxSize_ - (UsableEndIndex_ - TailIndex_);
		}
	}
};

class FrameResource final
{

};

//----------------------------------------------------------------------------------------

[[nodiscard]] bool FrameUploadAllocator::CreationInstructions(int HeapSize) {
	if (UpLoadHeap_) {
		DEBUG_LOG_WARNING("Already Creation UpLoadHeap");
		return false;
	}

	UpLoadHeap_ = std::make_unique<DescriptorHeap>();
	if (!UpLoadHeap_->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, HeapSize, true)) {
		DEBUG_LOG_ERROR("UpLoadHeap Creation Failed");
		return false;
	}

	LinearAllocator_ = std::make_unique<FrameBasedLinearAllocator>();
	if (!LinearAllocator_->Initialize(HeapSize)) {
		DEBUG_LOG_ERROR("RingBuffer_ Creation Failed");
		return false;
	}
	
	return true;
}

[[nodiscard]] std::optional<AllocateHeap> FrameUploadAllocator::AllocateHeapInstructions() {
	DEBUG_ASSERT(UpLoadHeap_.get());
	DEBUG_ASSERT(LinearAllocator_.get());

	const auto index = LinearAllocator_->GetHeapIndex();
	if (!index.has_value()) {
		return std::nullopt;
	}

	auto msize = UpLoadHeap_->GetHeapMemorySize();

	auto cpuhand = UpLoadHeap_->GetCPUHandle();
	cpuhand.ptr += msize * index.value();
	
	auto gpuhand = UpLoadHeap_->GetGPUHandle();
	gpuhand.ptr += msize * index.value();

	AllocateHeap allocate{ index.value(),cpuhand,gpuhand };
	return allocate;
}

void FrameUploadAllocator::Signal(UINT64 signal) {
	DEBUG_ASSERT(UpLoadHeap_.get());
	DEBUG_ASSERT(LinearAllocator_.get());

	LinearAllocator_->PushSignal(signal);
}

void FrameUploadAllocator::ResetFrameBuffer(UINT64 signal) {
	DEBUG_ASSERT(UpLoadHeap_.get());
	DEBUG_ASSERT(LinearAllocator_.get());

	LinearAllocator_->ReturnHeapIndex(signal);
}