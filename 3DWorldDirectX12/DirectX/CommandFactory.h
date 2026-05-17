#pragma once
#include"Device.h"
#include<vector>
#include<optional>

//@brief	/=== コマンドファクトリークラス ===/
class CommandFactory final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== コマンドキュー作成関数 ===
	//@param	type	コマンドタイプ
	//@retirn	作成したコマンドキュー ...失敗ならnullptr
	static std::optional<Microsoft::WRL::ComPtr<ID3D12CommandQueue>> create_command_queue(D3D12_COMMAND_LIST_TYPE type);

	//----------------------------------------------------------------------------------------------------


	//@brief	=== コマンドアロケーター作成関数 ===
	//@param	type	コマンドタイプ
	//@oaram	frame_baffer_size	描画サイクルのバッファサイズ
	//@return	作成したコマンドアロケータ配列 ...失敗ならnullptr
	static std::optional<std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>> create_command_allocator(
		D3D12_COMMAND_LIST_TYPE type, int frame_baffer_size);

	//----------------------------------------------------------------------------------------------------
};