#include"Device.h"
#include"../debug/debugsystem.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

[[nodiscard]] bool Device::Create() {

	if (!CreateFactory()) {
		return false;
	}
	if (!CreateAdaptor()) {
		return false;
	}
	if (!CreateDevice()) {
		return false;
	}

	return true;
}

[[nodiscard]] bool Device::CreateFactory() {
#if _DEBUG
	ID3D12Debug* debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
		debug->EnableDebugLayer();
	}
#endif

	UINT createFactoryFlags = 0;
#if _DEBUG
	createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&Factory_));
	DEBUG_HR_ASSERT(hr);
	if (FAILED(hr)) {
		return false;
	}
	return true;
}

[[nodiscard]] bool Device::CreateAdaptor() {
	auto                                  select = 0;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter{};
	//アダプター列挙取得
	while (Factory_->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {
		DXGI_ADAPTER_DESC1 desc{};
		dxgiAdapter->GetDesc1(&desc);

		select++;

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			dxgiAdapter->Release();
			continue;
		}

		if (FAILED(D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr))) {
			dxgiAdapter->Release();
			continue;
		}

		Adapter_ = dxgiAdapter;
		break;
	}
	DEBUG_ASSERT(Adapter_);
	return true;
}

[[nodiscard]] bool Device::CreateDevice() {
	const auto hr = D3D12CreateDevice(Adapter_.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&Device_));
	DEBUG_HR_ASSERT(hr);
	if (FAILED(hr)) {
		return false;
	}
	return true;
}

[[nodiscard]] IDXGIFactory4* Device::GetFactory()const noexcept {
	DEBUG_ASSERT(Factory_);
	return Factory_.Get();
}

[[nodiscard]] IDXGIAdapter1* Device::GetAdaptor()const noexcept {
	DEBUG_ASSERT(Adapter_);
	return Adapter_.Get();
}

[[nodiscard]] ID3D12Device* Device::GetDevice()const noexcept {
	DEBUG_ASSERT(Device_);
	return Device_.Get();
}