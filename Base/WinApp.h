#pragma once
#include <wrl.h>
#include <DirectXMath.h>

class WinApp
{
public://静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static DirectX::XMFLOAT2 GetWindowSize();

public://メンバ関数
	//初期化
	void Initialize();
	//メッセージ処理
	bool ProcessMessage();
	//終了
	void Finalize();

	static WinApp* GetInstance();

	//getter
	HWND GetHwnd() { return hwnd; };
	HINSTANCE GetHInstance() { return w.hInstance; }

public://メンバ変数		
	// ウィンドウサイズ
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅

	LPARAM lparam;
private://メンバ変数
	WNDCLASSEX w{};		// ウィンドウクラスの設定
	HWND hwnd = nullptr;
};