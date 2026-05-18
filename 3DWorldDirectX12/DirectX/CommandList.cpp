#include"CommandList.h"
#include"CommandFactory.h"

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドリスト初期化関数 ===
//@param	alocator	タイプ一致したコマンドアロケーター
//@param	type	コマンドタイプ
//@return	リスト作成の可否
[[nodiscard]] bool CommandList::initialize_list(ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type) {

	//ファクトリーに作成依頼
	auto value = CommandFactory::create_command_list(allocator, type);

	//作成できたかチェック
	if (!value.has_value()) {
		return false;
	}

	//できているなら保存
	list_ = value.value();
	return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドリスト取得関数 ===
//@return	コマンドリストポインター
[[nodiscard]] ID3D12GraphicsCommandList* CommandList::get_list()const noexcept {
	return list_.Get();
}

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドリストリセット関数 ===
//@param	allocator	タイプ一致したコマンドアロケーター
void CommandList::reset_list(ID3D12CommandAllocator* allocator) {
	list_->Reset(allocator, nullptr);
}