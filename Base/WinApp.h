#pragma once
#include <wrl.h>

class WinApp
{
public://�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://�����o�֐�
	//������
	void Initialize();
	//���b�Z�[�W����
	bool ProcessMessage();
	//�I��
	void Finalize();

	//getter
	HWND GetHwnd() { return hwnd; };

	HINSTANCE GetHInstance() { return w.hInstance; }

public://�����o�ϐ�
			// �E�B���h�E�T�C�Y
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��

private://�����o�ϐ�


	WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�

	HWND hwnd = nullptr;


};