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
#include"CompileShader.h"
#include"RootSignature.h"
#include"PiplineState.h"
#include"Polygon.h"
#include"FrameUploadAllocator.h"

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
	CompileShader		Vshader_{};
	CompileShader		Pshader_{};
	RootSignature		Root_{};
	PiplineState		pipline_{};
	PolygonBase			polygon_{};

	const int BufferSize_ = 2;
	std::vector<UINT64> FrameFenceValue_{};

	struct windowdata
	{
		HWND hwnd;
		int width;
		int height;

		windowdata() = default;
		windowdata(HWND HWND_, int w, int h) :
			hwnd(HWND_), width(w), height(h) {
		};
	};

	windowdata WData_{};
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
		CHECK_CREATE_CLEAR(FenceManager::Instance().Initialize());
		CHECK_CREATE_CLEAR(Vshader_.Create("../drawsystem/shader/VertexShader.hlsl","main","vs_5_0"));
		CHECK_CREATE_CLEAR(Pshader_.Create("../drawsystem/shader/VertexShader.hlsl","ps","ps_5_0"));
		CHECK_CREATE_CLEAR(Root_.Create());
		CHECK_CREATE_CLEAR(pipline_.Create(Vshader_.Get(), Pshader_.Get(), Root_.Get()));
		CHECK_CREATE_CLEAR(polygon_.Create());
		CHECK_CREATE_CLEAR(FrameUploadAllocator::Instance().CreationInstructions(4));
		

		FrameFenceValue_.resize(BufferSize_);
		DEBUG_LINELOG(100);
		WData_ = { HWND ,width ,height };
		return true;
	}

	//フレーム待機関数
	const UINT WaitFrame() {
		const auto backBufferIndex = SwapChain_.Get()->GetCurrentBackBufferIndex();

		if (FrameFenceValue_[backBufferIndex] != 0) {
			FenceManager::Instance().WaitEvent(FrameFenceValue_[backBufferIndex]);
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

		const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		List_.GetList()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

		List_.GetList()->SetPipelineState(pipline_.Get());
		// ルートシグネチャの設定
		List_.GetList()->SetGraphicsRootSignature(Root_.Get());

		const auto w = WData_.width;
		const auto h = WData_.height;
		D3D12_VIEWPORT viewport{};
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(w);
		viewport.Height = static_cast<float>(h);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		List_.GetList()->RSSetViewports(1, &viewport);

		// シザー矩形の設定
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = w;
		scissorRect.bottom = h;
		List_.GetList()->RSSetScissorRects(1, &scissorRect);

		FrameUploadAllocator::Instance().ResetFrameBuffer(FenceManager::Instance().GetCompletedValue());

		auto heap = FrameUploadAllocator::Instance().AllocateHeapInstructions();
		
		FrameUploadAllocator::Instance().Signal(FenceManager::Instance().Signal(Queue_.GetQueue()));

		// ポリゴンの描画
		polygon_.Draw(List_.GetList());

		auto rtToP = ResourceBarrier(Target_.Get(BackBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		List_.GetList()->ResourceBarrier(1, &rtToP);

		List_.GetList()->Close();

		ID3D12CommandList* ppCommandLists[] = { List_.GetList() };
		Queue_.GetQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		SwapChain_.Get()->Present(1, 0);

		FrameFenceValue_[BackBufferIndex] = FenceManager::Instance().Signal(Queue_.GetQueue());
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
		auto a = FenceManager::Instance().Signal(Queue_.GetQueue());
		DEBUG_CHECK_VALUE(a);
		FenceManager::Instance().WaitEvent(a);
	}
};

//--------------------------------------------------------------------------------------------------------
DrawSystem::DrawSystem() = default;
DrawSystem::~DrawSystem() = default;

[[nodiscard]] bool DrawSystem::Initialize(HWND HWND, int width, int height) {

	CHANGE_VALUE_3(HWND, width, height);
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