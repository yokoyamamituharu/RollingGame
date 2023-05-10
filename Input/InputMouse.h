#pragma once
#include <Windows.h>
#include <wrl.h>
#include <dinput.h>
#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include "WinApp.h"

enum MouseDIK
{
	M_RIGHT,
	M_LEFT,
	M_SCROLL
};
class InputMouse
{
public:
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectXを省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // メンバ関数

	static InputMouse* GetInstance();


	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(WinApp* winApp);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	bool PushMouse(MouseDIK click);

	bool TorigerMouse(MouseDIK click);

	bool ReleaseMouse(MouseDIK click);

	bool IsMoveMouse(char h);

	/// <summary>
	/// スクリーン上でのカーソルの座標
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 GetScreanPos();

	/// <summary>
	/// ウィンドウの中でのカーソルの座標
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 GetWindowPos();

	/// <summary>
	/// 前回のフレームからのカーソルの移動量
	/// </summary>
	/// <param name="h"></param>
	/// <returns></returns>
	XMFLOAT2 MoveMouseVector();

	/// <summary>
	/// カーソルを表示するか否か
	/// </summary>
	/// <param name="flag">true=表示 false=非表示</param>
	static void ShowMouseCursor(bool flag);

	/// <summary>
	/// カーソルをウィンドウの中心に固定
	/// </summary>
	void SetCenterCoursolPos();

	static XMFLOAT2 GetScreanWindowCenterPos();

private: // メンバ変数
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devmouse;
	DIMOUSESTATE2 mouse;
	DIMOUSESTATE2 oldmouse;

	static WinApp* winApp;
};
