#pragma once
#include<d3d12.h>
#include<wrl/client.h>


class CommandList final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== コマンドリスト初期化関数 ===
	//@param	alocator	タイプ一致したコマンドアロケーター
	//@param	type	コマンドタイプ
	//@return	リスト作成の可否
	[[nodiscard]] bool initialize_list(ID3D12CommandAllocator* alocator, D3D12_COMMAND_LIST_TYPE type);

	//@brief	=== コマンドリスト取得関数 ===
	//@return	コマンドリストポインター
	[[nodiscard]] ID3D12GraphicsCommandList* get_list()const noexcept;

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------
	
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list_{};	//コマンドリスト

	//----------------------------------------------------------------------------------------------------
};