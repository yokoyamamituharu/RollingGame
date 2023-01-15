#pragma once
#include <Windows.h>
#include <wrl.h>
#include <dinput.h>
#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
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
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �����o�֐�

	static InputMouse* GetInstance();


	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(WinApp* winApp);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	bool PushMouse(MouseDIK click);

	bool TorigerMouse(MouseDIK click);

	bool ReleaseMouse(MouseDIK click);

	bool IsMoveMouse(char h);

	XMFLOAT2 GetScreanPos();

	XMFLOAT2 GetWindowPos();

	float MoveMouseVector(char h);

	/// <summary>
	/// �J�[�\����\�����邩�ۂ�
	/// </summary>
	/// <param name="flag">true=�\�� false=��\��</param>
	void ShowMouseCursor(bool flag);

	/// <summary>
	/// �J�[�\�����E�B���h�E�̒��S�ɌŒ�
	/// </summary>
	void SetCenterCoursolPos();

private: // �����o�ϐ�
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devmouse;
	DIMOUSESTATE2 mouse;
	DIMOUSESTATE2 oldmouse;

	static WinApp* winApp;
};
