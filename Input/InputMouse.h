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

		bool IsMoveMouse(char h);

		float MoveMouseVector(char h);
		
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
