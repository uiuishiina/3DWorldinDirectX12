#include"../DirectX/DXGI.h"
#include"../DirectX/Device.h"
#include"../DirectX/CommandQueue.h"
#include"../DirectX/CommandAllocator.h"
#include"../DirectX/CommandList.h"

#include"Renderer.h"

//----------------------------------------------------------------------------------------------------

namespace {

//@brief	=== 関数失敗チェックマクロ ===
#define Check_Failed(expr)\
    do {\
        if(!(expr)) {\
            return false;\
        }\
    } while (0)

    //@brief    直接実行型
    D3D12_COMMAND_LIST_TYPE Direct_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
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

	return true;
}

//----------------------------------------------------------------------------------------------------