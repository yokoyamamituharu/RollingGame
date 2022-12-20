#include "InputMouse.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")
#include <cmath>

WinApp* InputMouse::winApp;

// �f�o�C�X�������Ɏ��s�����
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

	// DirectInput�I�u�W�F�N�g�̐���	
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//-------------------�}�E�X---------------------//

	// �}�E�X�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// ���̓f�[�^�`���̃Z�b�g
	result = devmouse->SetDataFormat(&c_dfDIMouse); // �W���`��
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	// �r�����䃌�x���̃Z�b�g
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

	//---------------------------�}�E�X------------------//
	result = devmouse->Acquire();	// �L�[�{�[�h����J�n

	// �O��̃}�E�X���͂�ۑ�
	oldmouse = mouse;

	// �}�E�X�̓���
	result = devmouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);

}


bool InputMouse::PushMouse(MouseDIK click)
{
	//���N���b�N
	if (click == MouseDIK::M_LEFT && mouse.rgbButtons[0] & (0x80))
	{
		return true;
	}

	//�E�N���b�N
	if (click == MouseDIK::M_RIGHT && mouse.rgbButtons[1] & (0x80))
	{
		return true;
	}

	return false;
}

bool InputMouse::TorigerMouse(MouseDIK click)
{
	bool check = oldmouse.rgbButtons[0] & (0x80);
	//���N���b�N
	if (click == MouseDIK::M_LEFT && mouse.rgbButtons[0] & (0x80) &&
		check == false)
	{
		return true;
	}
	check = oldmouse.rgbButtons[1] & (0x80);
	//�E�N���b�N
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
	//���N���b�N
	if (click == MouseDIK::M_LEFT && oldmouse.rgbButtons[0] & (0x80) &&
		check == false)
	{
		return true;
	}
	check = mouse.rgbButtons[1] & (0x80);
	//�E�N���b�N
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
	//�E�B���h�E�̊e���W���擾
	float left = rect.left;
	float right = rect.right;
	float top = rect.top;
	float bottom = rect.bottom;
	float x, y;

	//���̒��S�_���v�Z
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
	//�c�̒��S�_���v�Z
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

	//�J�[�\�����Œ�
	SetCursorPos(left + (x / 2), bottom - (y / 2));
}

