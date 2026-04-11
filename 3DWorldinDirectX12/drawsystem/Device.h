#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

class Device final
{
	Device() = default;
	~Device() = default;

	Microsoft::WRL::ComPtr<IDXGIFactory4>	Factory_{};
	Microsoft::WRL::ComPtr<IDXGIAdapter1>	Adapter_{};
	Microsoft::WRL::ComPtr<ID3D12Device>	Device_{};

	
	[[nodiscard]] bool CreateFactory();
	[[nodiscard]] bool CreateAdaptor();
	[[nodiscard]] bool CreateDevice();
public:
	static Device& Instance()noexcept {
		static Device ins;
		return ins;
	}

	[[nodiscard]] bool Create();

	[[nodiscard]] IDXGIFactory4* GetFactory()const noexcept;
	[[nodiscard]] IDXGIAdapter1* GetAdaptor()const noexcept;
	[[nodiscard]] ID3D12Device*	 GetDevice()const noexcept;
};