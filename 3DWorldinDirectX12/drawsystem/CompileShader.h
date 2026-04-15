#pragma once
#include"Device.h"
#include<string>

class CompileShader final
{
	Microsoft::WRL::ComPtr<ID3DBlob> Shader_{};
public:
	CompileShader() = default;
	~CompileShader() = default;

	[[nodiscard]] bool Create(const std::string& path, const std::string& entrypoint, const std::string& version);

	[[nodiscard]] ID3DBlob* Get()const noexcept;
};