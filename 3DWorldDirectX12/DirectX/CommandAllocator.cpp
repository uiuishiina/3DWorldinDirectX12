#include"CommandAllocator.h"
#include"CommandFactory.h"

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドアロケーター初期化関数 ===
//@param	type	コマンドタイプ
//@param	frame_buffer_size	描画サイクルのバッファサイズ ...デフォルトで 2 指定
//@return	アロケーター作成の可否
[[nodiscard]] bool CommandAllocator::initialize_allocator(D3D12_COMMAND_LIST_TYPE type, int frame_buffer_size) {

	//ファクトリーに作成依頼
	auto value = CommandFactory::create_command_allocator(type, frame_buffer_size);

	//作成できたかチェック
	if (!value.has_value()) {
		return false;
	}

	//できているなら保存
	allocator_ = value.value();
	return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドアロケーター取得関数 ===
//@param	index	取得したいインデックス番号
//@return	コマンドアロケーターポインター
[[nodiscard]] ID3D12CommandAllocator* CommandAllocator::get_allocator(size_t index)const noexcept {
	return allocator_[index].Get();
}

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドアロケーターリセット関数 ===
//@param	リセットするコマンドアロケーターのインデックス番号
void CommandAllocator::reset_allocator(size_t index) {
	const auto hr = allocator_[index]->Reset();
	if (FAILED(hr)) {
		//失敗した時のログ処理用
	}
}