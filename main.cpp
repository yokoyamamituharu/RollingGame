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
#include "ObjectObj.h"
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

#include "Bullet.h"

#include "safe_delete.h"

#include "SpriteManager.h"

//#include "SafeDelete.h"
//#include "Otamesi.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT result;
	FbxManager* fbxManager = FbxManager::Create();

	/*----�Q�[���x�[�X����������----*/
	//�E�B���h�E�̏�����
	WinApp* winApp = nullptr;
	winApp = WinApp::GetInstance();
	winApp->Initialize();
	//DirectX�̏�����
	DirectXCommon* dxCommon = nullptr;	
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	//�L�[�{�[�h
	Input* input = nullptr;
	input = Input::GetInstance();
	input->Initialize(winApp->GetHInstance(), winApp->GetHwnd());
	//�}�E�X
	InputMouse* mouse = nullptr;
	mouse = InputMouse::GetInstance();
	mouse->Initialize(winApp);
	//�J����
	Camera* gameCamera = Camera::Create();

	/*----�ÓI����������----*/
	//�X�v���C�g�̐ÓI������
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	//���f���̐ÓI������
	Model::StaticInitialize(dxCommon->GetDev());
	//3D�I�u�W�F�N�g�ÓI������
	ObjectObj::StaticInitialize(dxCommon->GetDev(), gameCamera);
	//FBX		
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
	ObjectFBX::StaticInitialize(dxCommon->GetDev(), gameCamera);	

	//-----�ϐ��錾-----//
	SceneManager* sceneManager = new SceneManager;	
	sceneManager->Initialize(dxCommon);
	
	//FPS���b�N
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
		

		//�V�[���̍X�V
		sceneManager->Update();

		//-----�`�揈��-----//
		//�|�X�g�G�t�F�N�g�̏���
		//postEffect->PreDrawScene(dxCommon->GetCmdList());
		//sceneManager->Draw();
		//postEffect->PosDrawScene(dxCommon->GetCmdList());


		dxCommon->PreDraw();
		//ObjectObj::PreDraw(dxCommon->GetCmdList());
		//scene.Draw();
		//ObjectObj::PostDraw();
		sceneManager->Draw();
		dxCommon->PostDraw();

		//dxCommon->PreDraw();
		//sceneManager->gameScene->PostDraw();				
		//dxCommon->PostDraw();


	}



	//�V�[���̊J��
	safe_delete(sceneManager);
	//�|�X�g�G�t�F�N�g�̊J��
	//safe_delete(postEffect);
	//���͂̉��
	//safe_delete(mouse);
	//�J�����̊J��
	safe_delete(gameCamera);
	//FBX�̉������
	FbxLoader::GetInstance()->Finalize();
	//winApp�̉��
	winApp->Finalize();
	//safe_delete(winApp);
	//DirectX���
	safe_delete(dxCommon);

	//input->Finalize();
	//safe_delete(input);
	return 0;
}