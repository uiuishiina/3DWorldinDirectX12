#pragma once
#include"Device.h"
#include<unordered_map>
#include<string>
#include<optional>

class ShaderContainer final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== シェーダー作成関数 ===
	//@param	key_Name	ハッシュ登録名
	//@param	shader_path	シェーダーパス
	//@param	entry_point	シェーダーのエントリーポイント
	//@param	version	シェーダーのバージョン
	//@return	シェーダー作成の可否
	[[nodiscard]] bool register_shader(std::string_view key_Name,
		const std::wstring& shader_path, const std::string& entry_point, const std::string& version);

	//@brief	=== シェーダー取得関数 ===
	//@param	key_Name	シェーダー登録名
	//@return	登録されているシェーダーポインター ...ないならnullptr
	[[nodiscard]] ID3DBlob* get_shader(std::string_view key_Name)const noexcept;

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3DBlob>>	shader_map{};

	//----------------------------------------------------------------------------------------------------
};