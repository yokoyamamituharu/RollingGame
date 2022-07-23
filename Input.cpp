#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInput�I�u�W�F�N�g�̐���	
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �L�[�{�[�h�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// ���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	return true;
}

void Input::Update()
{
	HRESULT result;

	result = devkeyboard->Acquire();	// �L�[�{�[�h����J�n

	// �O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));

	// �L�[�̓���
	result = devkeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0�łȂ���Ή����Ă���
	if (key[keyNumber]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}
