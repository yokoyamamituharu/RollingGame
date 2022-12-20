#include "InputMouse.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")
#include <cmath>

WinApp* InputMouse::winApp;

// デバイス発見時に実行される
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

InputMouse* InputMouse::GetInstance()
{
	static InputMouse instance;
	return &instance;
}

bool InputMouse::Initialize(WinApp* winApp)
{
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成	
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//-------------------マウス---------------------//

	// マウスデバイスの生成	
	result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = devmouse->SetDataFormat(&c_dfDIMouse); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	// 排他制御レベルのセット
	result = devmouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	return true;
}

void InputMouse::Update()
{
	HRESULT result;

	//---------------------------マウス------------------//
	result = devmouse->Acquire();	// キーボード動作開始

	// 前回のマウス入力を保存
	oldmouse = mouse;

	// マウスの入力
	result = devmouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);

}


bool InputMouse::PushMouse(MouseDIK click)
{
	//左クリック
	if (click == MouseDIK::M_LEFT && mouse.rgbButtons[0] & (0x80))
	{
		return true;
	}

	//右クリック
	if (click == MouseDIK::M_RIGHT && mouse.rgbButtons[1] & (0x80))
	{
		return true;
	}

	return false;
}

bool InputMouse::TorigerMouse(MouseDIK click)
{
	bool check = oldmouse.rgbButtons[0] & (0x80);
	//左クリック
	if (click == MouseDIK::M_LEFT && mouse.rgbButtons[0] & (0x80) &&
		check == false)
	{
		return true;
	}
	check = oldmouse.rgbButtons[1] & (0x80);
	//右クリック
	if (click == MouseDIK::M_RIGHT && mouse.rgbButtons[1] & (0x80) &&
		check == false)
	{
		return true;
	}

	return false;
}

bool InputMouse::ReleaseMouse(MouseDIK click)
{
	bool check = mouse.rgbButtons[0] & (0x80);
	//左クリック
	if (click == MouseDIK::M_LEFT && oldmouse.rgbButtons[0] & (0x80) &&
		check == false)
	{
		return true;
	}
	check = mouse.rgbButtons[1] & (0x80);
	//右クリック
	if (click == MouseDIK::M_RIGHT && oldmouse.rgbButtons[1] & (0x80) &&
		check == false)
	{
		return true;
	}

	return false;
}

bool InputMouse::IsMoveMouse(char h)
{
	if (h == 'x')
	{
		if (mouse.lX > 0)
		{
			return 1;
		}
		if (mouse.lX < 0)
		{
			return 2;
		}
	}
	if (h == 'y')
	{
		if (mouse.lY > 0)
		{
			return 3;
		}
		if (mouse.lY < 0)
		{
			return 4;
		}
	}
	return 0;
}

DirectX::XMFLOAT2 InputMouse::GetPos()
{
	POINT mouse_p;
	GetCursorPos(&mouse_p);
	return XMFLOAT2{ (float)mouse_p.x,(float) mouse_p.y };
}


float InputMouse::MoveMouseVector(char h)
{
	if (h == 'x')
	{
		return mouse.lX;
	}
	if (h == 'y')
	{
		return mouse.lY;
	}
	return 0;
}

void InputMouse::SetCenterCoursolPos()
{
	RECT rect;
	GetWindowRect(WinApp::GetInstance()->GetHwnd(), &rect);
	//ウィンドウの各座標を取得
	float left = rect.left;
	float right = rect.right;
	float top = rect.top;
	float bottom = rect.bottom;
	float x, y;

	//横の中心点を計算
	if (left > 0 && right >= 0 || left <= 0 && right <= 0) {
		float high, min;
		if (abs(left) > abs(right)) {
			high = left;
			min = right;
		}
		else {
			high = right;
			min = left;
		}
		x = abs(high) - abs(min);
	}
	else {
		x = abs(left) + abs(right);
	}
	//縦の中心点を計算
	if (top > 0 && bottom >= 0 || top <= 0 && bottom <= 0) {
		float high, min;
		if (abs(top) > abs(bottom)) {
			high = top;
			min = bottom;
		}
		else {
			high = bottom;
			min = top;
		}
		y = abs(high) - abs(min);
	}
	else {
		y = abs(top) + abs(bottom);
	}

	//カーソルを固定
	SetCursorPos(left + (x / 2), bottom - (y / 2));
}

