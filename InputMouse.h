#pragma once
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
#include <dinput.h>

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
		/// <summary>
		/// ������
		/// </summary>
		/// <returns>����</returns>
		bool Initialize(HINSTANCE hInstance, HWND hwnd);

		/// <summary>
		/// ���t���[������
		/// </summary>
		void Update();

		bool PushMouse(MouseDIK click);

		bool IsMoveMouse(char h);

		float MoveMouseVector(char h);

	private: // �����o�ϐ�
		ComPtr<IDirectInput8> dinput;

		ComPtr<IDirectInputDevice8> devmouse;
		DIMOUSESTATE2 mouse;
		DIMOUSESTATE2 oldmouse;

	};
