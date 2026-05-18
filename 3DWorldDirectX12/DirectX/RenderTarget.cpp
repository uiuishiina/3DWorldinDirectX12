#include"RenderTarget.h"
#include"DescriptorHeapFactory.h"

//----------------------------------------------------------------------------------------------------

//@brief	=== レンダーターゲット作成関数 ===
//@param	swapchain	紐づけるスワップチェーン
//@param	frame_buffer_size	描画サイクルのバッファサイズ
//@return	レンダーターゲット作成の成否
[[nodiscard]] bool RenderTarget::create_render_target(IDXGISwapChain4* swapchain, int frame_buffer_size) {

    //ファクトリーに作成依頼
	auto value = DescritporHeapFactory::create_descritpor_heap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, frame_buffer_size, false);

    //作成できたかチェック
	if (!value.has_value()) {
		return false;
	}

    //できているなら保存
	rtv_heap = value.value();

    //参照を繰り返さないように一時変数に保存
    auto device = Device::Instance().get_device();

    //レンダーターゲット配列をリサイズ
    render_targets_.resize(static_cast<size_t>(frame_buffer_size));

    //RTV用ディスクリプタヒープのメモリサイズを取得
    rtv_descriptor_size_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    //rtv_heapの先頭ハンドルを保存
    rtv_handle_ = rtv_heap->GetCPUDescriptorHandleForHeapStart();

    //ハンドルを移動できるように一時変数に保存
    auto handle = rtv_handle_;

    //ハンドルを移動させつつレンダーターゲットを作成し保存
    for (int i = 0; i < frame_buffer_size; ++i) {
        const auto hr = swapchain->GetBuffer(i, IID_PPV_ARGS(&render_targets_[i]));
        if (FAILED(hr)) {
            return false;
        }

        //RTV_viewの設定
        D3D12_RENDER_TARGET_VIEW_DESC rtv_desc{};
        rtv_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

        //RTV作成
        device->CreateRenderTargetView(render_targets_[i].Get(), &rtv_desc, handle);

        //ハンドルを移動
        handle.ptr += rtv_descriptor_size_;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== レンダーターゲットに設定済みのディスクリプタヒープ取得関数 ===
//@param	index	取得したいインデックス番号
//@return	オフセット済みのディスクリプタハンドル
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::get_rtv_handle(size_t index) const noexcept {

    //ハンドルを移動できるように一時変数に保存
    auto handle = rtv_handle_;

    //ハンドルをインデックス分移動
    handle.ptr += rtv_descriptor_size_ * index;
    return handle;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== レンダーターゲット取得関数 ===
//@param	index	取得したいインデックス番号
//@return	オフセット済みのレンダーターゲット
[[nodiscard]] ID3D12Resource* RenderTarget::get_render_target(size_t index) const noexcept {
    return render_targets_[index].Get();
}