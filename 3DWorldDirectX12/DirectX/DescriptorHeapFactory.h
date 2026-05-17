#pragma once
#include"Device.h"
#include<optional>

//@brief	/=== ディスクリプタヒープファクトリークラス ===/
class DescritporHeapFactory final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== ディスクリプタヒープ作成関数 ===
	//@param	type	ディスクリプタヒープタイプ
	//@param	heap_size	ヒープのサイズ
	//@param	shader_visible	シェーダー可視フラグ
	//@return	作成したディスクリプタヒープ
	static std::optional<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> create_descritpor_heap(
		D3D12_DESCRIPTOR_HEAP_TYPE type, int heap_size, bool shader_visible);
	
	//----------------------------------------------------------------------------------------------------
};