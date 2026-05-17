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
	//@retirn	作成したコマンドキュー
	static std::optional<Microsoft::WRL::ComPtr<ID3D12CommandQueue>> create_command_queue(D3D12_COMMAND_LIST_TYPE type);

	//----------------------------------------------------------------------------------------------------


	//@brief	=== コマンドアロケーター作成関数 ===
	//@param	type	コマンドタイプ
	//@oaram	frame_baffer_size	描画サイクルのバッファサイズ
	//@return	作成したコマンドアロケータ配列
	static std::optional<std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>> create_command_allocator(
		D3D12_COMMAND_LIST_TYPE type, int frame_baffer_size);

	//----------------------------------------------------------------------------------------------------


	//@brief	=== コマンドリスト作成関数 ===
	//@param	allocator	タイプ一致したコマンドアロケーター
	//@param	type	コマンドタイプ
	//@return	作成したコマンドリスト
	static std::optional<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> create_command_list(
		ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type);

	//----------------------------------------------------------------------------------------------------
};