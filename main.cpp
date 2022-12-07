#pragma region include関連
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
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

#include "SpriteManager.h"

//#include "SafeDelete.h"
//#include "Otamesi.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT result;
	FbxManager* fbxManager = FbxManager::Create();

	//-----初期化処理-----//
	WinApp* winApp = nullptr;
	winApp = WinApp::GetInstance();
	winApp->Initialize();
	DirectXCommon* dxCommon = nullptr;
	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	//入力
	Input* input = nullptr;
	input = Input::GetInstance();
	input->Initialize(winApp->GetHInstance(), winApp->GetHwnd());
	//マウス
	InputMouse* mouse = nullptr;
	mouse = InputMouse::GetInstance();
	mouse->Initialize(winApp);
	//カメラ
	Camera* camera = Camera::Create();
	//スプライトの静的初期化
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	//モデルの静的初期化
	Model::StaticInitialize(dxCommon->GetDev());
	//3Dオブジェクト静的初期化
	ObjectObj::StaticInitialize(dxCommon->GetDev(), camera);
	//FBX		
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
	ObjectFBX::StaticInitialize(dxCommon->GetDev(), camera);

	ParticleManager::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	//-----変数宣言-----//
	//ポストエフェクトの初期化
	//PostEffect* postEffect = nullptr;
	//postEffect = new PostEffect();
	//postEffect->Initialize();

	SpriteManager spriteManager;
	spriteManager.Initialize();

	SceneManager* sceneManager = nullptr;
	sceneManager = new SceneManager;
	sceneManager->Initialize(dxCommon);

	ModelManager* m = ModelManager::GetIns();
	m->Initialize();

	FPSLock fpsLock;

	//ParticleManager* particleMan = ParticleManager::Create();


	while (true)  // ゲームループ
	{
		//if (input->TriggerKey(DIK_SPACE)) {
		//	//パーティクルの追加
		//	for (int i = 0; i < 8; i++)
		//	{
		//		//X,Y,Z全てで[-5.0f,-5.0f]でランダムに分布
		//		const float rnd_pos = 10.0f;
		//		XMFLOAT3 pos{};
		//		//pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//		//pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//		//pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//		pos.x = 0;
		//		pos.y = 0;
		//		pos.z = 0;
		//		//X,Y,Z全て[-0.0f,+0.05f]でランダムに分布
		//		const float rnd_vel = 0.1f;
		//		XMFLOAT3 vel{};
		//		//vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f * 5;
		//		//vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f * 5;
		//		//vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f * 5;
		//		int rndVel = 3.0f;
		//		vel.x = rand() % (rndVel * 2) - rndVel;
		//		vel.y = rand() % (rndVel * 2) - rndVel;
		//		vel.z = rand() % (rndVel * 2) - rndVel;
		//		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		//		XMFLOAT3 acc{};
		//		const float rnd_acc = 0.001f;
		//		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//		//追加
		//		particleMan->Add(60, pos, vel, acc, 10.0f, 5.0f);
		//	}
		//}
		//particleMan->Update();

		//fpsLock.Update();

		// ブロック内はページ右側を参照
		// メッセージがある？
		if (winApp->ProcessMessage())
		{
			break;
		}

		//-----更新処理-----//
		//入力の更新処理
		input->Update();
		mouse->Update();
		

		//シーンの更新
		sceneManager->Update();

		//-----描画処理-----//
		//ポストエフェクトの準備
		//postEffect->PreDrawScene(dxCommon->GetCmdList());
		//ParticleManager::PreDraw(dxCommon->GetCmdList());
		//particleMan->Draw();
		//ParticleManager::PostDraw();
		//sceneManager->Draw();
		//postEffect->PosDrawScene(dxCommon->GetCmdList());


		dxCommon->PreDraw();
		ObjectObj::PreDraw(dxCommon->GetCmdList());
		//scene.Draw();
		ObjectObj::PostDraw();
		sceneManager->Draw();
		dxCommon->PostDraw();

		//dxCommon->PreDraw();
		//sceneManager->gameScene->PostDraw();				
		//dxCommon->PostDraw();


	}



	//シーンの開放
	safe_delete(sceneManager);
	//ポストエフェクトの開放
	//safe_delete(postEffect);
	//パーティクルマネージャーの開放
	//safe_delete(particleMan);
	//入力の解放
	//safe_delete(mouse);
	//カメラの開放
	safe_delete(camera);
	//FBXの解放処理
	FbxLoader::GetInstance()->Finalize();
	//winAppの解放
	winApp->Finalize();
	//safe_delete(winApp);
	//DirectX解放
	safe_delete(dxCommon);

	//input->Finalize();
	//safe_delete(input);
	return 0;
}