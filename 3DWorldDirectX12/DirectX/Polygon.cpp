#include"Polygon.h"
#include"ResourceFactory.h"

//----------------------------------------------------------------------------------------------------

//@brief    === 頂点作成関数 ===
//@return   作成の成否
[[nodiscard]] bool Polygon::create_vertex_buffer() {

	//頂点配列のメモリサイズ
	const UINT vertex_buffer_size = sizeof(vertices_);

	//ヒープの設定
	D3D12_HEAP_PROPERTIES heap_properties;
	heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;	//	アップロードヒープ
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_properties.CreationNodeMask = 1;
	heap_properties.VisibleNodeMask = 1;

	//リソースの設定
	D3D12_RESOURCE_DESC resource_desc{};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource_desc.Width = vertex_buffer_size;
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//リソース作成依頼
	auto resource = ResourceFactory::create_resorce(heap_properties, resource_desc);
	if (!resource.has_value()) {
		return false;
	}

	//できているなら保存
	vertex_buffer_ = resource.value();

	//頂点配列を用意
	Vertex* vertex_data_begin;
	const auto hr = vertex_buffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertex_data_begin));
	if (FAILED(hr)) {
		return false;
	}

	//頂点配列をコピー
	memcpy(vertex_data_begin, vertices_, vertex_buffer_size);
	vertex_buffer_->Unmap(0, nullptr);

	//ビューを作成
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.StrideInBytes = sizeof(Vertex);
	vertex_buffer_view_.SizeInBytes = vertex_buffer_size;
	return true;
}

//----------------------------------------------------------------------------------------------------

//@brief    === インデックス作成関数 ===
//@brief    作成の成否
[[nodiscard]] bool Polygon::create_index_buffer() {

	//インデックス配列のメモリサイズ
	const UINT index_buffer_size = sizeof(indices_);

	//ヒープの設定
	D3D12_HEAP_PROPERTIES heap_properties;
	heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_properties.CreationNodeMask = 1;
	heap_properties.VisibleNodeMask = 1;

	//リソースの設定
	D3D12_RESOURCE_DESC resource_desc{};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource_desc.Width = index_buffer_size;
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//リソース作成依頼
	auto resource = ResourceFactory::create_resorce(heap_properties, resource_desc);
	if (!resource.has_value()) {
		return false;
	}

	//できているなら保存
	index_buffer_ = resource.value();

	//インデックス配列を用意
	UINT* index_data_begin;
	const auto hr = index_buffer_->Map(0, nullptr, reinterpret_cast<void**>(&index_data_begin));
	if (FAILED(hr)) {
		return false;
	}

	//インデックス配列をコピー
	memcpy_s(index_data_begin, index_buffer_size, indices_, index_buffer_size);
	index_buffer_->Unmap(0, nullptr);

	//ビューを作成
	index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();
	index_buffer_view_.Format = DXGI_FORMAT_R32_UINT;
	index_buffer_view_.SizeInBytes = index_buffer_size;
	return true;
}

//----------------------------------------------------------------------------------------------------

//@brief    === ポリゴン初期化関数 ===
//@return   ポリゴン作成の成否
[[nodiscard]] bool Polygon::initialize_Polygon() {
	if (!create_vertex_buffer()) {
		return false;
	}
	if (!create_index_buffer()) {
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------

//@brief    === ポリゴン描画関数 
//@param    command_list    描画用コマンドリスト
void Polygon::draw_polygon(ID3D12GraphicsCommandList* command_list) const noexcept {

	//頂点設定
	command_list->IASetVertexBuffers(0, 1, &vertex_buffer_view_);

	//インデックス設定
	command_list->IASetIndexBuffer(&index_buffer_view_);

	//描画方法設定
	command_list->IASetPrimitiveTopology(topology_);

	//描画
	command_list->DrawIndexedInstanced(_countof(indices_), 1, 0, 0, 0);
}