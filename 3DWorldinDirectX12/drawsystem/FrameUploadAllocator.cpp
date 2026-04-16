#include"DescriptorHeap.h"
#include"FrameUploadAllocator.h"
#include"../debug/debugsystem.h"

FrameUploadAllocator::FrameUploadAllocator() = default;
FrameUploadAllocator::~FrameUploadAllocator() = default;

class FrameBasedLinearAllocator final
{
	size_t HeapMaxSize_{};		//ヒープ最大数
	size_t UsableEndIndex_{};	//使用可能な最後尾のインデックス
	size_t TailIndex_{};		//使用している最後尾のインデックス
	size_t UsedSize_{};			//使用しているヒープサイズ

	std::vector<size_t> ByFrameBuffer_{};	//フレーム別の使用中最後尾インデックス

	bool IsFull()const noexcept {
		return UsedSize_ == HeapMaxSize_;
	}
	bool IsEmpty()const noexcept {
		return UsedSize_ == 0;
	}
public:
	FrameBasedLinearAllocator() = default;
	~FrameBasedLinearAllocator() = default;
	[[nodiscard]] bool Initialize(int HeapSize, int FrameBufferSize) {
		if (!ByFrameBuffer_.empty()) {
			return false;
		}

		ByFrameBuffer_.resize(FrameBufferSize);
		HeapMaxSize_ = HeapSize;
		UsableEndIndex_ = HeapSize - 1;
		return true;
	}

	//使用済みにプッシュ
	std::optional<size_t> GetHeapIndex(int FrameBufferIndex) {
		if (IsFull()) {
			return std::nullopt;
		}
		ByFrameBuffer_[FrameBufferIndex] = TailIndex_;
		const auto index = TailIndex_;
		TailIndex_ = (TailIndex_ + 1) % HeapMaxSize_;
		UsedSize_++;
		return index;
	}

	//使用済みから外す
	void ReturnHeapIndex(int FrameBufferIndex) {
		UsableEndIndex_ = ByFrameBuffer_[FrameBufferIndex];

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

[[nodiscard]] bool FrameUploadAllocator::CreationInstructions(int HeapSize,int FrameBufferSize) {
	if (UpLoadHeap_) {
		DEBUG_LOG_WARNING("Already Creation UpLoadHeap");
		return false;
	}

	UpLoadHeap_ = std::make_unique<DescriptorHeap>();
	if (!UpLoadHeap_->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, HeapSize, true)) {
		DEBUG_LOG_ERROR("UpLoadHeap Creation Failed");
		return false;
	}

	LinearAllocator_ = std::make_unique<FrameBasedLinearAllocator>();
	if (!LinearAllocator_->Initialize(HeapSize, FrameBufferSize)) {
		DEBUG_LOG_ERROR("RingBuffer_ Creation Failed");
		return false;
	}
	
	return true;
}

[[nodiscard]] std::optional<AllocateHeap> FrameUploadAllocator::AllocateHeapInstructions(int FrameBufferIndex) {
	DEBUG_ASSERT(UpLoadHeap_.get());
	DEBUG_ASSERT(LinearAllocator_.get());

	const auto index = LinearAllocator_->GetHeapIndex(FrameBufferIndex);
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

void FrameUploadAllocator::ResetFrameBuffer(int FrameBufferIndex) {
	DEBUG_ASSERT(UpLoadHeap_.get());
	DEBUG_ASSERT(LinearAllocator_.get());

	LinearAllocator_->ReturnHeapIndex(FrameBufferIndex);
}