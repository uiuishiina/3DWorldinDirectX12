//drawsystem.c
#include"drawsystem.h"
#include"../debug/debugsystem.h"
#include"Device.h"
#include"CommandQueue.h"
#include"CommandAllocator.h"
#include"CommandList.h"
#include"SwapChain.h"
#include"RenderTarget.h"
#include"Fence.h"

//各種作成関数チェックマクロ
#define CHECK_CREATE_CLEAR(expr)\
	do{\
		if(!(expr)){\
			DEBUG_LOG_WARNING(#expr,"S_FALSE");\
			return false;\
		}\
		DEBUG_LOG(#expr,"S_OK");\
	} while (0)

//-------------------------------------------------------------------------------------------------------

class DrawSystem::Impl final
{
	CommandQueue		Queue_{};
	CommandAllocator	Allocator_{};
	CommandList			List_{};
	SwapChain			SwapChain_{};
	RenderTarget		Target_{};
	Fence				Fence_{};
	

	const int BufferSize_ = 2;
	std::vector<int> FrameFenceValue_{};
	int NextFenceValue_{};
public:
	Impl() = default;
	~Impl() = default;

	//初期化関数
	[[nodiscard]] bool Initialize(HWND HWND, int width, int height) {

		CHECK_CREATE_CLEAR(Device::Instance().Create());

		CHECK_CREATE_CLEAR(Queue_.Create());
		CHECK_CREATE_CLEAR(Allocator_.Create(BufferSize_));
		CHECK_CREATE_CLEAR(List_.Create(Allocator_.GetAllocator(0)));

		CHECK_CREATE_CLEAR(SwapChain_.Create(Queue_.GetQueue(), HWND, width, height, BufferSize_));
		CHECK_CREATE_CLEAR(Target_.Create(SwapChain_.Get(), BufferSize_));
		CHECK_CREATE_CLEAR(Fence_.Create());

		FrameFenceValue_.resize(BufferSize_);
		DEBUG_LINELOG(100);
		return true;
	}

	//フレーム待機関数
	const UINT WaitFrame() {
		const auto backBufferIndex = SwapChain_.Get()->GetCurrentBackBufferIndex();

		if (FrameFenceValue_[backBufferIndex] != 0) {
			Fence_.WaitEvent(FrameFenceValue_[backBufferIndex]);
		}
		return backBufferIndex;
	}

	//描画ループ開始関数
	void BeginFrame(const UINT BackBufferIndex) {
		Allocator_.Reset(BackBufferIndex);
		List_.Reset(Allocator_.GetAllocator(BackBufferIndex));
	}

	//描画ループ中処理本体
	void UpdateFrame(const UINT BackBufferIndex) {
		auto pToRT = ResourceBarrier(Target_.Get(BackBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		List_.GetList()->ResourceBarrier(1, &pToRT);

		D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { Target_.GetHandle(BackBufferIndex) };
		List_.GetList()->OMSetRenderTargets(1, handles, false, nullptr);

		const float clearColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		List_.GetList()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

		auto rtToP = ResourceBarrier(Target_.Get(BackBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		List_.GetList()->ResourceBarrier(1, &rtToP);

		List_.GetList()->Close();

		ID3D12CommandList* ppCommandLists[] = { List_.GetList() };
		Queue_.GetQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		SwapChain_.Get()->Present(1, 0);

		Queue_.GetQueue()->Signal(Fence_.Get(), NextFenceValue_);
		FrameFenceValue_[BackBufferIndex] = NextFenceValue_;
		NextFenceValue_++;
	}

	//リソースバリア変更関数
	[[nodiscard]] D3D12_RESOURCE_BARRIER ResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resource;
		barrier.Transition.StateBefore = from;
		barrier.Transition.StateAfter = to;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		return barrier;
	}

	//描画ループ終了待機関数
	void WaitForGPUIdle() {
		Queue_.GetQueue()->Signal(Fence_.Get(), NextFenceValue_);

		Fence_.WaitEvent(NextFenceValue_);
		NextFenceValue_++;
	}
};

//--------------------------------------------------------------------------------------------------------
DrawSystem::DrawSystem() = default;
DrawSystem::~DrawSystem() = default;

[[nodiscard]] bool DrawSystem::Initialize(HWND HWND, int width, int height) {

	impl_ = std::make_unique<Impl>();

	if (!impl_->Initialize(HWND, width, height)) {
		DEBUG_LOG_ERROR("Impl::Initialize() Abnormal Termination")
		return false;
	}
	DEBUG_LOG_INFO("Impl::Initialize() Normal Termination");

	DEBUG_LOG_INFO("DrawSystem::Initialize() Normal Termination");
	DEBUG_LINELOG(100);
	return true;
}

void DrawSystem::Rendering() {
	auto BackBufferIndex = impl_->WaitFrame();
	impl_->BeginFrame(BackBufferIndex);
	impl_->UpdateFrame(BackBufferIndex);
}

void DrawSystem::EndRendering() {
	impl_->WaitForGPUIdle();
	DEBUG_LINELOG(100);
	DEBUG_LOG("EndRendering");
	DEBUG_LINELOG(100);
}
//---------------------------------------------------------------------------------------------------------

