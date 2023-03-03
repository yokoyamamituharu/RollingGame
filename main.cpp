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

	/*----ゲームベース初期化処理----*/
	//ウィンドウの初期化
	WinApp* winApp = nullptr;
	winApp = WinApp::GetInstance();
	winApp->Initialize();
	//DirectXの初期化
	DirectXCommon* dxCommon = nullptr;	
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	//キーボード
	Input* input = nullptr;
	input = Input::GetInstance();
	input->Initialize(winApp->GetHInstance(), winApp->GetHwnd());
	//マウス
	InputMouse* mouse = nullptr;
	mouse = InputMouse::GetInstance();
	mouse->Initialize(winApp);
	//カメラ
	Camera* gameCamera = Camera::Create();

	/*----静的初期化処理----*/
	//スプライトの静的初期化
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	//モデルの静的初期化
	Model::StaticInitialize(dxCommon->GetDev());
	//3Dオブジェクト静的初期化
	ObjectObj::StaticInitialize(dxCommon->GetDev(), gameCamera);
	//FBX		
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
	ObjectFBX::StaticInitialize(dxCommon->GetDev(), gameCamera);	

	//-----変数宣言-----//
	SceneManager* sceneManager = new SceneManager;	
	sceneManager->Initialize(dxCommon);
	
	//FPSロック
	FPSLock fpsLock;	
	while (true)  // ゲームループ
	{

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



	//シーンの開放
	safe_delete(sceneManager);
	//ポストエフェクトの開放
	//safe_delete(postEffect);
	//入力の解放
	//safe_delete(mouse);
	//カメラの開放
	safe_delete(gameCamera);
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