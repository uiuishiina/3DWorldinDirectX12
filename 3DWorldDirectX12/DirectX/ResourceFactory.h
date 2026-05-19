#pragma once
#include"Device.h"
#include<optional>

//@brief	/=== リソースファクトリークラス ===/
class ResourceFactory final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== リソース作成関数 ===
	//@param	heap_properties	ヒープの設定
	//@param	resource_desc	リソースの設定
	//@param	作成したリソース
	static std::optional<Microsoft::WRL::ComPtr<ID3D12Resource>> create_resorce(
		D3D12_HEAP_PROPERTIES& heap_properties, D3D12_RESOURCE_DESC& resource_desc);

	//----------------------------------------------------------------------------------------------------
};