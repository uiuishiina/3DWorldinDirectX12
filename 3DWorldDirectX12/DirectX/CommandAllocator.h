#pragma once
#include<d3d12.h>
#include<wrl/client.h>
#include<vector>

//@brief	/=== コマンドアロケータークラス ===/
class CommandAllocator final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== コマンドアロケーター初期化関数 ===
	//@param	type	コマンドタイプ
	//@param	frame_buffer_size	描画サイクルのバッファサイズ
	//@return	アロケーター作成の可否
	[[nodiscard]] bool initialize_allocator(D3D12_COMMAND_LIST_TYPE type, int frame_buffer_size);

	//@brief	=== コマンドアロケーター取得関数 ===
	//@param	index	取得したいインデックス番号
	//@return	コマンドアロケーターポインター
	[[nodiscard]] ID3D12CommandAllocator* get_allocator(size_t index)const noexcept;

	//@brief	=== コマンドアロケーターリセット関数 ===
	//@param	リセットするコマンドアロケーターのインデックス番号
	void reset_allocator(size_t index);

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> allocator_;	//コマンドアロケーター配列

	//----------------------------------------------------------------------------------------------------
};