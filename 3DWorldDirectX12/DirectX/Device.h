#pragma once
#include<dxgi1_6.h>
#include<d3d12.h>
#include"addfiles/d3dx12.h"
#include<wrl/client.h>

class Device final
{
public:
	//----------------------------------------------------------------------------------------------------
	
	//@brief	=== Deviceインスタンス取得関数 ===
	static Device& Instance()noexcept {
		static Device instance;
		return instance;
	}

	//@brief	=== Device作成関数 ===
	//@return	作成の可否
	[[nodiscard]] bool create_device(IDXGIAdapter4* factory);

	//@brief	=== Device取得関数 ===
	//@return	ID3D12Deviceポインター
	[[nodiscard]] ID3D12Device* get_device()const noexcept;

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	//@brief	=== Device選定関数 ===
	//@return	作成したデバイス
	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Device> select_device_by_level(IDXGIAdapter4* adapter, D3D_FEATURE_LEVEL featureLevel);
};