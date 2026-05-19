#include"ShaderContainer.h"
#include <D3Dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")



//@brief	=== シェーダー作成関数 ===
//@param	key_Name	ハッシュ登録名
//@param	shader_path	シェーダーパス
//@param	entry_point	シェーダーのエントリーポイント
//@param	version	シェーダーのバージョン
//@return	シェーダー作成の可否
[[nodiscard]] bool ShaderContainer::register_shader(std::string_view key_Name,
	const std::wstring& shader_path, const std::string& entry_point, const std::string& version) {

    //エラーが出る場合情報取得
    UINT compile_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    compile_flags |= D3DCOMPILE_DEBUG;
#endif

    //シェーダーコンパイル
    Microsoft::WRL::ComPtr<ID3DBlob> shader_blob{};
    Microsoft::WRL::ComPtr<ID3DBlob> error_blob{};
    const auto hr = D3DCompileFromFile(shader_path.data(), nullptr, nullptr, entry_point.data(), version.data(), compile_flags, 0, &shader_blob, &error_blob);
    if (FAILED(hr)) {
        if (error_blob) {
            
        }
        return false;
    }

    shader_map.emplace(std::string{ key_Name }, std::move(shader_blob));

    return true;
}

//@brief	=== シェーダー取得関数 ===
//@param	key_Name	シェーダー登録名
//@return	登録されているシェーダーポインター ...ないならnullptr
[[nodiscard]] ID3DBlob* ShaderContainer::get_shader(std::string_view key_Name)const noexcept {

    const auto it = shader_map.find(std::string{ key_Name });

    if (it == shader_map.end()) {
        return nullptr;
    }

    return it->second.Get();
}