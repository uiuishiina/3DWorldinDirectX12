#include"CommandQueue.h"
#include"CommandFactory.h"

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドキュー初期化関数 ===
//@return	キューの作成の成否
[[nodiscard]] bool CommandQueue::initialize_queue(D3D12_COMMAND_LIST_TYPE type) {

	//ファクトリーに作成依頼
	auto value = CommandFactory::create_command_queue(type);

	//作成できたかチェック
	if (!value.has_value()) {
		return false;
	}

	//できているなら保存
	queue_ = value.value();
	return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドキュー取得関数 ===
//@return	コマンドキューポインター
[[nodiscard]] ID3D12CommandQueue* CommandQueue::get_queue()const noexcept {
	return queue_.Get();
}