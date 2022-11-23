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

		bool IsMoveMouse(char h);

		float MoveMouseVector(char h);
		
		/// <summary>
		/// カーソルをウィンドウの中心に固定
		/// </summary>
		void SetCenterCoursolPos();

	private: // メンバ変数
		ComPtr<IDirectInput8> dinput;
		ComPtr<IDirectInputDevice8> devmouse;
		DIMOUSESTATE2 mouse;
		DIMOUSESTATE2 oldmouse;

		static WinApp* winApp;
	};
