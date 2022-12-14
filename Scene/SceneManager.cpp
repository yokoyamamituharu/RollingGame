#include "SceneManager.h"
#include "safe_delete.h"
#include "SpriteManager.h"
#include "Useful.h"

bool SceneManager::blackStartFlag = false;
bool SceneManager::hitEnemyToPlayer = false;
bool SceneManager::WinBattle = false;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	safe_delete(titleScene);
	safe_delete(endScene);
	safe_delete(gameScene);
	safe_delete(batlleScene);
	ModelManager::Finalize();
}


void SceneManager::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	this->camera = camera;

	//モデルマネージャーの読み込み
	ModelManager::GetIns()->Initialize();
	//スプライトマネージャーの読み込み
	SpriteManager::GetIns()->Initialize();

	//各シーンの生成
	titleScene = new TitleScene;
	titleScene->Initialize(dxCommon);
	endScene = new EndScene;
	endScene->Initialize(dxCommon);
	gameScene = new GameScene;
	gameScene->Initialize(dxCommon);
	//batlleScene = new BatlleScene;
	//batlleScene->Initialize(dxCommon);

	sceneNo = SCENE_TITLE;
	//sceneNo = SCENE_GAME;
	post = new PostEffect;
	post->Initialize();

	sceneEffect[0] = Sprite::Create(21, { 0,0 });
	sceneEffect[1] = Sprite::Create(22, { 0,0 });
	sceneEffect[2] = Sprite::Create(23, { 0,0 });
	sceneEffect[3] = Sprite::Create(24, { 0,0 });
	sceneEffect[4] = Sprite::Create(25, { 0,0 });
}

void SceneManager::Update()
{
	//if (Input::GetInstance()->PushKey(DIK_1)) {
	//	sceneNo = SCENE_GAME;
	//}
	//else if (Input::GetInstance()->PushKey(DIK_2)) {
	//	sceneNo = SCENE_END;
	//}
	//else if (Input::GetInstance()->PushKey(DIK_3)) {
	//	sceneNo = SCENE_TITLE;
	//}
	//else if (Input::GetInstance()->PushKey(DIK_4)) {
	//	sceneNo = SCENE_BATTLE;
	//}

	if (Input::GetInstance()->TriggerKey(DIK_E)) {
		effectFlag = true;
	}
	if (hitEnemyToPlayer == true) {
		effectTime++;
		if (effectTime > 10) {
			effectIndex++;
			effectTime = 0;
		}
		if (effectIndex >= 5) {
			effectIndex = 0;
			effectTime = 0;
			effectFlag = 0;
			hitEnemyToPlayer = false;
			sceneNo = SCENE_BATTLE;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_B))
	{
		blackStartFlag = true;
	}
	static XMFLOAT3 color = { 0, 0, 0 };
	//if (Input::GetInstance()->PushKey(DIK_UP)) {
	//	post->value += 0.001;
	//	color = color - XMFLOAT3{ 0.05, 0.05, 0.05 };
	//	post->SetColor(color);
	//}
	//if (Input::GetInstance()->PushKey(DIK_DOWN)) {
	//	post->value -= 0.001;
	//	color = color + XMFLOAT3{ 0.05, 0.05, 0.05 };
	//	post->SetColor(color);
	//}

	if (blackStartFlag == true) {
		blackTime -= 0.05;
		if (blackTime > 0) {
			post->SetColor({ blackTime,blackTime,blackTime });
		}
		else if (loadEndFlag == false) {
			blackFlag = true;
		}

		if (loadEndFlag == true) {
			blackTime2 += 0.05f;
			post->SetColor({ blackTime2,blackTime2,blackTime2 });
		}
		if (blackTime2 >= 1) {
			blackStartFlag = false;
			blackFlag = false;
			loadEndFlag = false;
			blackTime = 1.0f;
			blackTime2 = 0.0f;
		}
	}

	if (WinBattle == true) {
		winblackTime -= 0.05;
		if (winblackTime > 0) {
			post->SetColor({ winblackTime,winblackTime,winblackTime });
		}
		else if (winloadEndFlag == false) {
			winblackFlag = true;
		}

		if (winloadEndFlag == true) {
			winblackTime2 += 0.05f;
			post->SetColor({ winblackTime2,winblackTime2,winblackTime2 });
		}
		if (winblackTime2 >= 1) {
			WinBattle = false;
			winblackFlag = false;
			winloadEndFlag = false;
			winblackTime = 1.0f;
			winblackTime2 = 0.0f;
		}
	}

	if (winblackFlag) {
		sceneNo = SCENE_GAME;
		winloadEndFlag = true;
	}

	//ゲームリセット
	if (Input::GetInstance()->TriggerKey(DIK_R) || initFlag == true) {
		GameSceneReset();
		if (Input::GetInstance()->TriggerKey(DIK_R)) {
			sceneNo = SCENE_TITLE;
		}
	}
	GameSceneReset();

	//シーンの更新
	if (sceneNo == SCENE_TITLE) {
		titleScene->Update(sceneNo, initFlag);
	}
	else if (sceneNo == SCENE_END || sceneNo == SCENE_KATI) {
		endScene->Update(sceneNo);
	}
	else if (sceneNo == SCENE_GAME) {
		gameScene->Update(sceneNo, batlleScene);
	}
	else if (sceneNo == SCENE_BATTLE) {
		batlleScene->Update(sceneNo, gameScene);
	}


	//ポストエフェクト描画前処理
	post->PreDrawScene(dxCommon->GetCmdList());
	if (sceneNo == SCENE_TITLE) {
		titleScene->Draw();
	}
	if (sceneNo == SCENE_END || sceneNo == SCENE_KATI) {
		endScene->Draw(sceneNo);
	}
	if (sceneNo == SCENE_GAME) {
		gameScene->Draw();
	}
	if (sceneNo == SCENE_BATTLE) {
		batlleScene->Draw();
	}
	post->PosDrawScene(dxCommon->GetCmdList());

	//if (Input::GetInstance()->TriggerKey(DIK_E)) {
	//	effectFlag = true;
	//}
	//if (effectFlag == true) {
	//	effectTime++;
	//	if (effectTime > 10) {
	//		effectIndex++;
	//		effectTime = 0;
	//	}
	//	if (effectIndex >= 5) {
	//		effectIndex = 0;
	//		effectTime = 0;
	//		effectFlag = false;
	//	}
	//}
}

void SceneManager::Draw()
{
	//if (sceneNo == SCENE_TITLE) {
	//	titleScene->Draw();
	//}
	//if (sceneNo == SCENE_END || sceneNo == SCENE_KATI) {
	//	endScene->Draw(sceneNo);
	//}
	//if (sceneNo == SCENE_GAME) {
	//	gameScene->Draw();
	//}
	//if (sceneNo == SCENE_BATTLE) {
	//	batlleScene->Draw();
	//}

	post->Draw(dxCommon->GetCmdList());

	Sprite::PreDraw(dxCommon->GetCmdList());
	if (SceneManager::hitEnemyToPlayer == true) {
		sceneEffect[effectIndex]->Draw();
	}
	Sprite::PostDraw();
}

void SceneManager::GameSceneReset()
{
	if (blackFlag == true) {
		safe_delete(gameScene);
		safe_delete(batlleScene);
		gameScene = new GameScene();		
		gameScene->Initialize(dxCommon);
		batlleScene = new BatlleScene();
		batlleScene->Initialize(dxCommon);
		initFlag = false;
		loadEndFlag = true;
		blackFlag = false;
		sceneNo = SCENE_GAME;
	}
}