#pragma once
#include<memory>
#include<vector>

struct HWND__;
using HWND = HWND__*;

class DXGI;
class Device;
class CommandQueue;
class CommandAllocator;
class CommandList;
class SwapChain;
class RenderTarget;
class Fence;

//@brief	/=== 描画機能クラス ===/
class Renderer final
{
public:
	//----------------------------------------------------------------------------------------------------

	//コンストラクタとデストラクタ
	//前方宣言を使用しているため cpp に実体を逃がす必要がある
	Renderer();
	~Renderer();

	//@brief	=== 描画機能初期化 ===
	//@param	hwnd_	ウィンドウハンドル
	//@param	width	ウィンドウの横幅
	//@param	height	ウィンドウの縦幅
	//@return	初期化の成否
	[[nodiscard]] bool initialize_renderer(HWND hwnd_, int width = 1920, int height = 1080);

	//@brief	=== 描画機能の更新 ===
	void update_renderer();

	//@brief	=== 描画機能の終了処理 ===
	void end_renderer();

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	std::pair<int,int>	window_size{};

	const int frame_buffer_size = 2;
	std::vector<UINT64>	frame_fence_value{};

	std::unique_ptr<DXGI>				dxgi_{};			//DXGIクラスインスタンス
	std::unique_ptr<CommandQueue>		queue_{};			//CommandQueueクラスインスタンス
	std::unique_ptr<CommandAllocator>	allocator_{};		//CommandAllocatorクラスインスタンス
	std::unique_ptr<CommandList>		list_{};			//CommandListクラスインスタンス
	std::unique_ptr<SwapChain>			swap_chain_{};		//SwapChainクラスインスタンス
	std::unique_ptr<RenderTarget>		render_target_{};	//RenderTargetクラスインスタンス
	std::unique_ptr<Fence>				fence_{};			//Fenceクラスインスタンス

	//----------------------------------------------------------------------------------------------------
};