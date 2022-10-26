#pragma region include�֘A
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <time.h>
#include "fbxsdk.h"

#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "PostEffect.h"
#include "Camera.h"
#include "Sprite.h"
#include "OBJobject.h"
#include "FBXObject.h"
#include "GameScene.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;
#pragma endregion

#include "InputMouse.h"
#include "Player.h"
#include "SceneManager.h"

#include "FPSLock.h"

//�����̂̓����蔻��
//bool CubeCollision(XMFLOAT3 object1, XMFLOAT3 radius1, XMFLOAT3 object2, XMFLOAT3 radius2) {
//	float disX1 = object1.x + radius1.x;
//	float disX2 = object2.x - radius2.x;
//	float disX3 = object1.x - radius1.x;
//	float disX4 = object2.x + radius2.x;
//	float disY1 = object1.y + radius1.y;
//	float disY2 = object2.y - radius2.y;
//	float disY3 = object1.y - radius1.y;
//	float disY4 = object2.y + radius2.y;
//	float disZ1 = object1.z + radius1.z;
//	float disZ2 = object2.z - radius2.z;
//	float disZ3 = object1.z - radius1.z;
//	float disZ4 = object2.z + radius2.z;
//
//	//x��y�͂��ꂼ�ꒆ�S���W�Ƃ��Čv�Z����
//	return disX1 > disX2 && disX4 > disX3 && disY1 > disY2 && disY4 > disY3 && disZ1 > disZ2 && disZ4 > disZ3;
//}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT result;
	FbxManager* fbxManager = FbxManager::Create();

	//-----����������-----//
	WinApp* winApp = nullptr;
	winApp = new WinApp();
	winApp->Initialize();
	DirectXCommon* dxCommon = nullptr;
	//DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	//����
	Input* input = nullptr;
	input = Input::GetInstance();
	input->Initialize(winApp->GetHInstance(), winApp->GetHwnd());
	//�}�E�X
	InputMouse* mouse = nullptr;
	mouse = new InputMouse();
	mouse->Initialize(winApp->GetHInstance(), winApp->GetHwnd());
	//�J����
	Camera* camera = nullptr;
	camera = new Camera(winApp->window_width, winApp->window_height);
	//�X�v���C�g�̐ÓI������
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	//���f���̐ÓI������
	Model::StaticInitialize(dxCommon->GetDev());
	//3D�I�u�W�F�N�g�ÓI������
	OBJobject::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height, camera);
	//FBX		
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
	FBXObject::StaticInitialize(dxCommon->GetDev(), camera);

	//-----�ϐ��錾-----//
	//�|�X�g�G�t�F�N�g�̏�����
	PostEffect* postEffect = nullptr;
	postEffect = new PostEffect();
	postEffect->Initialize();

	SceneManager* sceneManager = nullptr;
	sceneManager = new SceneManager;
	sceneManager->Initialize(dxCommon, input, mouse, camera);

	bool isSetMousePoint = true;	//�}�E�X�̃|�C���^�̈ʒu���Œ肷�邩�ǂ���


	FPSLock fpsLock;
	while (true)  // �Q�[�����[�v
	{
		//fpsLock.Update();

		// �u���b�N���̓y�[�W�E�����Q��
		// ���b�Z�[�W������H
		if (winApp->ProcessMessage())
		{
			break;
		}
		//-----�X�V����-----//
		//���͂̍X�V����
		input->Update();
		mouse->Update();
		if (input->TriggerKey(DIK_ESCAPE)) {	//M�L�[�������ꂽ��}�E�X�|�C���^�[�̈ړ����~
			if (isSetMousePoint) { isSetMousePoint = false; }
			else { isSetMousePoint = true; }
		}

	
		HWND hWnd;
		RECT rect;
		hWnd = GetDesktopWindow();
		GetWindowRect(winApp->GetHwnd(), &rect);

		float left = rect.left;
		float right = rect.right;
		float top = rect.top;
		float bottom = rect.bottom;

		if (isSetMousePoint) {
			float x, y;
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
			//SetCursorPos(left + (right - left) / 2, bottom + (top - bottom) / 2);
			SetCursorPos(left + (x / 2), bottom - (y / 2));
		}


		//MessageBox(NULL, L"�Ղ�", L"����.exe", MB_OK);

		// �}�E�X�ړ��͈͂̎擾


		// �}�E�X�ړ��͈͂̐ݒ�
		RECT rc;
		rc.left = 800;      // �������X���W
		rc.top = 400;      // �������Y���W
		rc.right = 801;      // �E������X���W
		rc.bottom = 401;      // �E������Y���W
		//ClipCursor(&rc);

		// �}�E�X�ړ��͈͂̉���
		//ClipCursor(NULL);

		//SetCursorPos(800 , -1000 );

		//�N���b�N�m�F�p
		if (mouse->PushMouse(MouseDIK::M_RIGHT))
		{
			int a = 0;
		}
		if (mouse->PushMouse(MouseDIK::M_LEFT))
		{
			int a = 0;
		}


		//�V�[���̍X�V
		sceneManager->Update(dxCommon, input, mouse, camera);

		//-----�`�揈��-----//
		//�|�X�g�G�t�F�N�g�̏���
		//postEffect->PreDrawScene(dxCommon->GetCmdList());
		//postEffect->PosDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		//postEffect->Draw(dxCommon->GetCmdList());		
		sceneManager->Draw();
		dxCommon->PostDraw();
	}

	//winApp�̉��
	winApp->Finalize();
	delete winApp;
	winApp = nullptr;
	//DirectX���
	delete dxCommon;
	//���͂̉��
	//delete input;
	//�V�[���̊J��
	delete sceneManager;
	//FBX�̉������
	FbxLoader::GetInstance()->Finalize();
	return 0;
}