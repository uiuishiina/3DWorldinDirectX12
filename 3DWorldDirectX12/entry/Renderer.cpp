#include"../DirectX/DXGI.h"
#include"../DirectX/Device.h"
#include"../DirectX/CommandQueue.h"
#include"../DirectX/CommandAllocator.h"
#include"../DirectX/CommandList.h"
#include"../DirectX/SwapChain.h"
#include"../DirectX/RenderTarget.h"
#include"../DirectX/Fence.h"

#include"Renderer.h"

//----------------------------------------------------------------------------------------------------

namespace {
//----------------------------------------------------------------------------------------------------

//@brief	=== 関数失敗チェックマクロ ===
#define Check_Failed(expr)\
    do {\
        if(!(expr)) {\
            return false;\
        }\
    } while (0)

//----------------------------------------------------------------------------------------------------

    //@brief    直接実行型
    D3D12_COMMAND_LIST_TYPE Direct_type = D3D12_COMMAND_LIST_TYPE_DIRECT;

//----------------------------------------------------------------------------------------------------

    [[nodiscard]] D3D12_RESOURCE_BARRIER resource_Barrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = resource;
        barrier.Transition.StateBefore = from;
        barrier.Transition.StateAfter = to;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        return barrier;
    }

//----------------------------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------------------------

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

//----------------------------------------------------------------------------------------------------

//@brief	=== 描画機能初期化 ===
//@return	初期化の成否
[[nodiscard]] bool Renderer::initialize_renderer(HWND hwnd_, int width, int height) {

    //DXGIクラス初期化
	dxgi_ = std::make_unique<DXGI>();
	Check_Failed(dxgi_->initialize_DXGI());

    //Deviceクラス初期化
	Check_Failed(Device::Instance().create_device(dxgi_->get_adaptor()));

    //CommandQueueクラス作成
    queue_ = std::make_unique<CommandQueue>();
    Check_Failed(queue_->initialize_queue(Direct_type));

    //CommandAllocatorクラス作成
    allocator_ = std::make_unique<CommandAllocator>();
    Check_Failed(allocator_->initialize_allocator(Direct_type, frame_buffer_size));

    //Commandlistクラス作成
    list_ = std::make_unique<CommandList>();
    Check_Failed(list_->initialize_list(allocator_->get_allocator(0), Direct_type));

    //SwapChainクラス作成
    swap_chain_ = std::make_unique<SwapChain>();
    Check_Failed(swap_chain_->create_swap_chain(dxgi_->get_factory(), queue_->get_queue(), 
        hwnd_, width, height, frame_buffer_size));

	window_size = { width,height };

    //RenderTargetクラス作成
    render_target_ = std::make_unique<RenderTarget>();
    Check_Failed(render_target_->create_render_target(swap_chain_->get_swap_chain(), frame_buffer_size));

    //Fenceクラス作成
    fence_ = std::make_unique<Fence>();
    Check_Failed(fence_->create_fence());
    frame_fence_value.resize(frame_buffer_size, 0);

	return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== 描画機能の更新 ===
void Renderer::update_renderer() {

    const auto back_buffer_index = swap_chain_->get_swap_chain()->GetCurrentBackBufferIndex();

	if (frame_fence_value[back_buffer_index] != 0) {
		fence_->wait_event(frame_fence_value[back_buffer_index]);
	}

	allocator_->reset_allocator(back_buffer_index);

	list_->reset_list(allocator_->get_allocator(back_buffer_index));

	auto pToRT = resource_Barrier(render_target_->get_render_target(back_buffer_index), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	list_->get_list()->ResourceBarrier(1, &pToRT);

	D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { render_target_->get_rtv_handle(back_buffer_index) };
	list_->get_list()->OMSetRenderTargets(1, handles, false, nullptr);

	const float clearColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };  // 黒でクリア
	list_->get_list()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

	const auto [width, height] = window_size;
	// ビューポート設定
	D3D12_VIEWPORT viewport{};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	list_->get_list()->RSSetViewports(1, &viewport);

	// シザー矩形設定
	D3D12_RECT scissorRect{};
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = width;
	scissorRect.bottom = height;
	list_->get_list()->RSSetScissorRects(1, &scissorRect);

	//--------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------

	auto rtToP = resource_Barrier(render_target_->get_render_target(back_buffer_index), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	list_->get_list()->ResourceBarrier(1, &rtToP);

	list_->get_list()->Close();

	ID3D12CommandList* ppCommandLists[] = { list_->get_list() };
	queue_->get_queue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	swap_chain_->get_swap_chain()->Present(1, 0);

	const auto nextFenceValue = fence_->signal(queue_->get_queue());
	frame_fence_value[back_buffer_index] = nextFenceValue.value();
}

//----------------------------------------------------------------------------------------------------

//@brief	=== 描画機能の終了処理 ===
void Renderer::end_renderer() {
	const auto nextFenceValue = fence_->signal(queue_->get_queue());
	fence_->wait_event(nextFenceValue.value());
}