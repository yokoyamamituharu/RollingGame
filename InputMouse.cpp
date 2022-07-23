#include "InputMouse.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")

// �f�o�C�X�������Ɏ��s�����
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

bool InputMouse::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInput�I�u�W�F�N�g�̐���	
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
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
	result = devmouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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
	//memcpy(keyPre, key, sizeof(key));
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

