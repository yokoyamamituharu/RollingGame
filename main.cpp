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
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
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

#include "ParticleManager.h"
#include "Bullet.h"

#include "safe_delete.h"

//#include "SafeDelete.h"
//#include "Otamesi.h"

//�}�E�X�J�[�\�����E�B���h�E�̐^�񒆂ɌŒ肷��֐�
void SetCenterCoursolPos(WinApp* winApp)
{
	//�}�E�X�̍��W�Œ菈��
	RECT rect;
	GetWindowRect(winApp->GetHwnd(), &rect);
	float left = rect.left;
	float right = rect.right;
	float top = rect.top;
	float bottom = rect.bottom;

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
	SetCursorPos(left + (x / 2), bottom - (y / 2));
}


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
	ObjectObj::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height, camera);
	//FBX		
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
	ObjectFBX::StaticInitialize(dxCommon->GetDev(), camera);

	ParticleManager::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

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

	ParticleManager* particleMan = ParticleManager::Create();


	while (true)  // �Q�[�����[�v
	{
		if (input->TriggerKey(DIK_SPACE)) {
			//�p�[�e�B�N���̒ǉ�
			for (int i = 0; i < 8; i++)
			{
				//X,Y,Z�S�Ă�[-5.0f,-5.0f]�Ń����_���ɕ��z
				const float rnd_pos = 10.0f;
				XMFLOAT3 pos{};
				//pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
				//pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
				//pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
				pos.x = 0;
				pos.y = 0;
				pos.z = 0;
				//X,Y,Z�S��[-0.0f,+0.05f]�Ń����_���ɕ��z
				const float rnd_vel = 0.1f;
				XMFLOAT3 vel{};
				//vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f * 5;
				//vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f * 5;
				//vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f * 5;
				int rndVel = 3.0f;
				vel.x = rand() % (rndVel * 2) - rndVel;
				vel.y = rand() % (rndVel * 2) - rndVel;
				vel.z = rand() % (rndVel * 2) - rndVel;
				//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				//�ǉ�
				particleMan->Add(60, pos, vel, acc, 10.0f, 5.0f);
			}
		}
		particleMan->Update();

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
		//�E�B���h�E���A�N�e�B�u��ԂȂ珈��
		if (winApp->GetHwnd() == GetActiveWindow() && isSetMousePoint == true)
		{
			SetCenterCoursolPos(winApp);
		}


		//�V�[���̍X�V
		sceneManager->Update(dxCommon, input, mouse, camera);

		//-----�`�揈��-----//
		//�|�X�g�G�t�F�N�g�̏���
		//postEffect->PreDrawScene(dxCommon->GetCmdList());
		//ParticleManager::PreDraw(dxCommon->GetCmdList());
		//particleMan->Draw();
		//ParticleManager::PostDraw();
		//sceneManager->Draw();
		//postEffect->PosDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		sceneManager->Draw();
		//postEffect->Draw(dxCommon->GetCmdList());
		dxCommon->PostDraw();
	}

	//winApp�̉��
	winApp->Finalize();
	safe_delete(winApp);	
	//DirectX���
	safe_delete(dxCommon);	
	//���͂̉��
	safe_delete(mouse);
	//�J�����̊J��
	safe_delete(camera);
	//�V�[���̊J��
	safe_delete(sceneManager);
	//�|�X�g�G�t�F�N�g�̊J��
	safe_delete(postEffect);
	//�p�[�e�B�N���}�l�[�W���[�̊J��
	safe_delete(particleMan);
	//FBX�̉������
	FbxLoader::GetInstance()->Finalize();

	//input->Finalize();
	//safe_delete(input);
	return 0;
}