#include"SwapChain.h"
#include"Device.h"

//----------------------------------------------------------------------------------------------------

//@brief	=== スワップチェーン作成関数 ===
//@param	direct_queue	Directタイプのコマンドキュー
//@param	hwnd	ウィンドウハンドル
//@param	width	ウィンドウの横幅
//@param	height	ウィンドウの縦幅
//@param	frame_buffer_size	描画サイクルのバッファサイズ
//@return	スワップチェーン作成の可否
[[nodiscard]] bool SwapChain::create_swap_chain(IDXGIFactory6* factory, ID3D12CommandQueue* direct_queue, HWND hwnd, int width, int height, int frame_buffer_size) {

    //スワップチェーンの設定
    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //Windows互換がBRGAなためこちらに合わせることにする
    desc.Stereo = FALSE;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   //レンダーターゲット用として設定
    desc.BufferCount = frame_buffer_size;
    desc.Scaling = DXGI_SCALING_STRETCH;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;  //Present()の内部処理の方法
    desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    desc.Flags = 0;

    //下位のスワップチェーン作成
    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1{};
    auto hr = factory->CreateSwapChainForHwnd(direct_queue, hwnd, &desc, nullptr, nullptr, &swapchain1);
    if (FAILED(hr)) {
        return false;
    }

    //キャストして上位のスワップチェーンに変換
    hr = swapchain1.As(&swap_chain_);
    if (FAILED(hr)) {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== スワップチェーン取得関数 ===
//@return	スワップチェーンのポインター
[[nodiscard]] IDXGISwapChain3* SwapChain::get_swap_chain()const noexcept {
    return swap_chain_.Get();
}