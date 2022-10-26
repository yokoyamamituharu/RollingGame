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

//立方体の当たり判定
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
//	//xとyはそれぞれ中心座標として計算する
//	return disX1 > disX2 && disX4 > disX3 && disY1 > disY2 && disY4 > disY3 && disZ1 > disZ2 && disZ4 > disZ3;
//}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT result;
	FbxManager* fbxManager = FbxManager::Create();

	//-----初期化処理-----//
	WinApp* winApp = nullptr;
	winApp = new WinApp();
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
	mouse = new InputMouse();
	mouse->Initialize(winApp->GetHInstance(), winApp->GetHwnd());
	//カメラ
	Camera* camera = nullptr;
	camera = new Camera(winApp->window_width, winApp->window_height);
	//スプライトの静的初期化
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	//モデルの静的初期化
	Model::StaticInitialize(dxCommon->GetDev());
	//3Dオブジェクト静的初期化
	OBJobject::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height, camera);
	//FBX		
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
	FBXObject::StaticInitialize(dxCommon->GetDev(), camera);

	//-----変数宣言-----//
	//ポストエフェクトの初期化
	PostEffect* postEffect = nullptr;
	postEffect = new PostEffect();
	postEffect->Initialize();

	SceneManager* sceneManager = nullptr;
	sceneManager = new SceneManager;
	sceneManager->Initialize(dxCommon, input, mouse, camera);

	bool isSetMousePoint = true;	//マウスのポインタの位置を固定するかどうか


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
		if (input->TriggerKey(DIK_ESCAPE)) {	//Mキーを押されたらマウスポインターの移動を停止
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


		//MessageBox(NULL, L"ぷり", L"うんち.exe", MB_OK);

		// マウス移動範囲の取得


		// マウス移動範囲の設定
		RECT rc;
		rc.left = 800;      // 左上隅のX座標
		rc.top = 400;      // 左上隅のY座標
		rc.right = 801;      // 右下隅のX座標
		rc.bottom = 401;      // 右下隅のY座標
		//ClipCursor(&rc);

		// マウス移動範囲の解除
		//ClipCursor(NULL);

		//SetCursorPos(800 , -1000 );

		//クリック確認用
		if (mouse->PushMouse(MouseDIK::M_RIGHT))
		{
			int a = 0;
		}
		if (mouse->PushMouse(MouseDIK::M_LEFT))
		{
			int a = 0;
		}


		//シーンの更新
		sceneManager->Update(dxCommon, input, mouse, camera);

		//-----描画処理-----//
		//ポストエフェクトの準備
		//postEffect->PreDrawScene(dxCommon->GetCmdList());
		//postEffect->PosDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		//postEffect->Draw(dxCommon->GetCmdList());		
		sceneManager->Draw();
		dxCommon->PostDraw();
	}

	//winAppの解放
	winApp->Finalize();
	delete winApp;
	winApp = nullptr;
	//DirectX解放
	delete dxCommon;
	//入力の解放
	//delete input;
	//シーンの開放
	delete sceneManager;
	//FBXの解放処理
	FbxLoader::GetInstance()->Finalize();
	return 0;
}