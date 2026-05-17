#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl/client.h>
#include<vector>

//@brief	/=== レンダーターゲットクラス ===/
class RenderTarget final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== レンダーターゲット作成関数 ===
	//@param	swapchain	紐づけるスワップチェーン
	//@param	frame_buffer_size	描画サイクルのバッファサイズ
	//@return	レンダーターゲット作成の成否
	[[nodiscard]] bool create_render_target(IDXGISwapChain4* swapchain, int frame_buffer_size);

	//@brief	=== レンダーターゲットに設定済みのディスクリプタヒープ取得関数 ===
	//@param	index	取得したいインデックス番号
	//@return	オフセット済みのディスクリプタハンドル
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE get_rtv_handle(size_t index) const noexcept;

	//@brief	=== レンダーターゲット取得関数 ===
	//@param	index	取得したいインデックス番号
	//@return	レンダーターゲットポインター
	[[nodiscard]] ID3D12Resource* get_render_target(size_t index) const noexcept;

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> render_targets_;			//レンダーターゲット配列
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		rtv_heap{};					//RTV用ディスクリプタヒープ
	D3D12_CPU_DESCRIPTOR_HANDLE							rtv_handle_{};				//rtv_heapの先頭ハンドル
	UINT												rtv_descriptor_size_ = 0;	//RTV用ディスクリプタヒープのメモリサイズ

	//----------------------------------------------------------------------------------------------------
};