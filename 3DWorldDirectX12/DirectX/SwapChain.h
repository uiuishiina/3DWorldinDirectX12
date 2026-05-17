#pragma once
#include<dxgi1_6.h>
#include<d3d12.h>
#include<wrl/client.h>


struct HWND__;
using HWND = HWND__*;

//@brief	/=== スワップチェーンクラス ===/
class SwapChain final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== スワップチェーン作成関数 ===
	//@param	direct_queue	Directタイプのコマンドキュー
	//@param	hwnd	ウィンドウハンドル
	//@param	width	ウィンドウの横幅
	//@param	height	ウィンドウの縦幅
	//@param	frame_buffer_size	描画サイクルのバッファサイズ
	//@return	スワップチェーン作成の可否
	[[nodiscard]] bool create_swap_chain(IDXGIFactory6* factory,ID3D12CommandQueue* direct_queue, HWND hwnd, int width, int height, int frame_buffer_size);

	//@brief	=== スワップチェーン取得関数 ===
	//@return	スワップチェーンのポインター
	[[nodiscard]] IDXGISwapChain3* get_swap_chain()const noexcept;

	//----------------------------------------------------------------------------------------------------
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain3> swap_chain_{};	//スワップチェーン
};