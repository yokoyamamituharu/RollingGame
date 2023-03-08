#pragma once
#include <wrl.h>
#include <DirectXMath.h>

class WinApp
{
public://�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static DirectX::XMFLOAT2 GetWindowSize();

public://�����o�֐�
	//������
	void Initialize();
	//���b�Z�[�W����
	bool ProcessMessage();
	//�I��
	void Finalize();

	static WinApp* GetInstance();

	//getter
	HWND GetHwnd() { return hwnd; };
	HINSTANCE GetHInstance() { return w.hInstance; }

public://�����o�ϐ�		
	// �E�B���h�E�T�C�Y
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��

	LPARAM lparam;
private://�����o�ϐ�
	WNDCLASSEX w{};		// �E�B���h�E�N���X�̐ݒ�
	HWND hwnd = nullptr;
};