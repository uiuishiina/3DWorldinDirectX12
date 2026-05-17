#include"CommandFactory.h"

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドキュー作成関数 ===
//@param	type	コマンドタイプ
//@retirn	作成したコマンドキュー ...失敗ならnullptr
std::optional<Microsoft::WRL::ComPtr<ID3D12CommandQueue>> CommandFactory::create_command_queue(D3D12_COMMAND_LIST_TYPE type) {

    //コマンドキュー設定
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = type;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    //コマンドキューポインター
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue{};

    //コマンドキュー作成
    const auto hr = Device::Instance().get_device()->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
    if (FAILED(hr)) {
        return std::nullopt;
    }
    return queue;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドアロケーター作成関数 ===
//@param	type	コマンドタイプ
//@oaram	frame_baffer_size	描画サイクルのバッファサイズ
//@return	作成したコマンドアロケータ配列 ...失敗ならnullptr
std::optional<std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>> CommandFactory::create_command_allocator(
    D3D12_COMMAND_LIST_TYPE type, int frame_baffer_size) {

    //アロケーター配列初期化
    std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> allocator{};
    allocator.resize(frame_baffer_size);

    //順に作成
    for (int i = 0; i < frame_baffer_size; i++) {
        const auto hr = Device::Instance().get_device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator[i]));
        if (FAILED(hr)) {
            return std::nullopt;
        }
    }
    return allocator;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドリスト作成関数 ===
//@param	allocator	タイプ一致したコマンドアロケーター
//@param	type	コマンドタイプ
//@return	作成したコマンドリスト
std::optional<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> CommandFactory::create_command_list(
    ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type) {

    //コマンドリストポインター
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   list{};

    //コマンドリスト作成
    const auto hr = Device::Instance().get_device()->CreateCommandList(0, type, allocator, nullptr, IID_PPV_ARGS(&list));
    if (FAILED(hr)) {
        return std::nullopt;
    }

    //コマンドリストを閉じる
    list->Close();
    return list;
}