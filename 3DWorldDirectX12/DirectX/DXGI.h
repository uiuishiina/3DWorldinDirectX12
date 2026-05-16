#pragma once
#include<dxgi1_6.h>
#include<d3d12.h>
#include"addfiles/d3dx12.h"
#include<wrl/client.h>

//@brief	/=== DXGIクラス ===/
class DXGI final 
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== DXGI初期化関数 ===
	//@return	初期化の可否
	[[nodiscard]] bool initialize_DXGI();

	//@brief	===  DXGIFactory取得関数  ===
	//@return	DXGIFactoryポインター
	[[nodsicard]] IDXGIFactory6* get_factory()const noexcept;

	//@brief	===  DXGIAdaptor取得関数  ===
	//@return	DXGIAdaptorポインター
	[[nodiscard]] IDXGIAdapter4* get_adaptor()const noexcept;

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	Microsoft::WRL::ComPtr<IDXGIFactory6> factory_{};	//DXGIFactory
	Microsoft::WRL::ComPtr<IDXGIAdapter4> adaptor_{};	//DXGIAdaptor

	//@brief	=== DXGIFactory作成関数 ===
	//@return	作成の可否
	[[nodiscard]] bool create_factory();

	//@brief	===  DXGIAdaptor選定関数  ===
	//@return	選定の可否
	[[nodiscard]] bool pick_adaptor();

	//@brief	=== DXGIAdaptor条件探索関数 ===
	//@brief	条件に合うAdaptor
	[[nodiscard]] Microsoft::WRL::ComPtr<IDXGIAdapter4> find_adaptor(DXGI_GPU_PREFERENCE preference);

	//----------------------------------------------------------------------------------------------------
};