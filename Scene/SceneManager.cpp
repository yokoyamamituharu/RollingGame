#include "SceneManager.h"

void SceneManager::Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera)
{
	gameScene = new GameScene;
	gameScene->Initialize(dxCommon, input, mouse, camera);
	batlleScene = new BatlleScene;
	batlleScene->Initialize(dxCommon, input, mouse, camera, gameScene);

	sceneNo = SCENE_GAME;
}

void SceneManager::Update(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera)
{
	if (Input::GetInstance()->PushKey(DIK_1)) {
		//sceneNo = GAMESCENE;
	}
	else if (Input::GetInstance()->PushKey(DIK_2)) {
		//sceneNo = BATTLESCENE;
	}

	//ゲームリセット
	if (Input::GetInstance()->PushKey(DIK_R)) {
		gameScene->Initialize(dxCommon, input, mouse, camera);
		batlleScene->Initialize(dxCommon, input, mouse, camera,gameScene);
	}

	//シーンの更新
	if (sceneNo == SCENE_GAME) {
		gameScene->Update(sceneNo,batlleScene);
	}
	else {
		batlleScene->Update(sceneNo, gameScene);
	}
}

void SceneManager::Draw()
{
	if (sceneNo ==SCENE_GAME) {
		gameScene->Draw();
	}
	else {
		batlleScene->Draw();
	}

}
