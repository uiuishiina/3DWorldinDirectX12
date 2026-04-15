#include"CompileShader.h"
#include"../debug/debugsystem.h"
#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

[[nodiscard]] bool CompileShader::Create(const std::string& path, const std::string& entrypoint, const std::string& version) {

    const std::wstring temp = std::wstring(path.begin(), path.end());
    Microsoft::WRL::ComPtr<ID3DBlob> error{};

    const auto hr = D3DCompileFromFile(temp.data(), nullptr, nullptr, entrypoint.data(), version.data(), D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &Shader_, &error);
    DEBUG_HR_ASSERT(hr);
    if (FAILED(hr)) {
        if (error) {
            char* p = static_cast<char*>(error.Get()->GetBufferPointer());
            DEBUG_LOG_ERROR(p);
        }
        return false;
    }

    return true;
}

[[nodiscard]] ID3DBlob* CompileShader::Get()const noexcept {
    DEBUG_ASSERT(Shader_);
    return Shader_.Get();
}