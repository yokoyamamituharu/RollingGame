#include "InputMouse.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")
#include <cmath>
#include <windowsx.h>


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

DirectX::XMFLOAT2 InputMouse::GetScreanPos()
{
	POINT mouse_p;
	GetCursorPos(&mouse_p);
	return XMFLOAT2{ (float)mouse_p.x,(float)mouse_p.y };
}

DirectX::XMFLOAT2 InputMouse::GetWindowPos()
{
	POINT mouse_p;
	GetCursorPos(&mouse_p);
	bool check = ScreenToClient(WinApp::GetInstance()->GetHwnd(), &mouse_p);
	return XMFLOAT2{ (float)mouse_p.x,(float)mouse_p.y };
}


DirectX::XMFLOAT2 InputMouse::MoveMouseVector()
{
	return XMFLOAT2({ float(mouse.lX),float(mouse.lY) });
}

void InputMouse::ShowMouseCursor(bool flag)
{
	int c;
	if (flag == true) {
		do {
			c = ShowCursor(1);
		} while (c < 0);
	}
	if (flag == false) {
		do {
			c = ShowCursor(0);
		} while (c >= 0);
	}
}

void InputMouse::SetCenterCoursolPos()
{
	//�J�[�\�����Œ�
	SetCursorPos(int(GetScreanWindowCenterPos().x), int(GetScreanWindowCenterPos().y));
}

DirectX::XMFLOAT2 InputMouse::GetScreanWindowCenterPos()
{
	//�X�N���[�����W��̃E�B���h�E���W���擾���E�B���h�E�̒����̍��W���擾
	RECT rect;
	GetWindowRect(WinApp::GetInstance()->GetHwnd(), &rect);
	return XMFLOAT2(rect.left + WinApp::window_width / 2, rect.top + WinApp::window_height / 2);
}

