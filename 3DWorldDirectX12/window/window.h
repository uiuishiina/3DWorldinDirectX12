#pragma once
#include<Windows.h>

//@brief	/=== ウィンドウクラス ===/
class window final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== ウィンドウ作成クラス ===
	//@param	hInstance	ウィンドウインスタンス
	//@param	width	ウィンドウの横幅
	//@param	height	ウィンドウの縦幅
	//@return	ウィンドウ作成の可否
	[[nodiscard]] bool create_window(HINSTANCE hInstance, UINT16 width, UINT16 height);

	//@brief	=== ウィンドウメッセージ受付関数 ===
	//@param	hwnd	ウィンドウハンドル
	//@param	msg		ウィンドウメッセージ
	//@param	wParam	ウィンドウメッセージのパラメーター
	//@param	lParam	ウィンドウメッセージのパラメーター
	LRESULT	window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//@brief	=== ウィンドウハンドル取得関数 ===
	//@return	ウィンドウハンドル
	[[nodiscard]] HWND get_hwnd()const noexcept { return hwnd_; }

	//@brief	/=== ウィンドウサイズ用構造体 ===/
	struct window_size {
		UINT16 width;	//横幅
		UINT16 height;	//縦幅
	};
	
	//@brief	=== ウィンドウサイズ取得関数 ===
	//@return	ウィンドウサイズ
	[[nodiscard]] window_size get_size()const noexcept { return size_; }

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	HWND hwnd_{};			//ウィンドウハンドル
	window_size size_{};	//ウィンドウサイズ

	//----------------------------------------------------------------------------------------------------
};