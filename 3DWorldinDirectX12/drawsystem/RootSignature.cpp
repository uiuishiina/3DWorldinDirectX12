#include"RootSignature.h"
#include"../debug/debugsystem.h"

[[nodiscard]] bool RootSignature::Create() {
    // ルートシグネチャの設定
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.pParameters = nullptr;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> signature{};
    Microsoft::WRL::ComPtr<ID3DBlob> error{};

    {
        const auto hr = D3D12SerializeRootSignature(
            &rootSignatureDesc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            &signature,
            &error);

        DEBUG_HR_ASSERT(hr);
        if (FAILED(hr)) {
            if (error) {
                char* p = static_cast<char*>(error->GetBufferPointer());
                DEBUG_LOG_ERROR(p);
            }
            return false;
        }
    }
    {
        const auto hr = Device::Instance().GetDevice()->CreateRootSignature(
            0,
            signature->GetBufferPointer(),
            signature->GetBufferSize(),
            IID_PPV_ARGS(&RootSignature_));

        DEBUG_HR_ASSERT(hr);
        if (FAILED(hr)) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] ID3D12RootSignature* RootSignature::Get()const noexcept {
    DEBUG_ASSERT(RootSignature_);
    return RootSignature_.Get();
}