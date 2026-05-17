#include"DescriptorHeapFactory.h"

//----------------------------------------------------------------------------------------------------

//@brief	=== ディスクリプタヒープ作成関数 ===
//@param	type	ディスクリプタヒープタイプ
//@param	heap_size	ヒープのサイズ
//@param	shader_visible	シェーダー可視フラグ
//@return	作成したディスクリプタヒープ
std::optional<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> DescritporHeapFactory::create_descritpor_heap(
	D3D12_DESCRIPTOR_HEAP_TYPE type, int heap_size, bool shader_visible) {

	//ディスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Type = type;
	desc.NumDescriptors = heap_size;
	desc.Flags = shader_visible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//ディスクリプタヒープのポインター
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap{};

	//ディスクリプタヒープ作成
	const auto hr = Device::Instance().get_device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr)) {
		return std::nullopt;
	}
	return heap;
}

//----------------------------------------------------------------------------------------------------