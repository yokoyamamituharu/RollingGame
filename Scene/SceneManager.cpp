#include "SceneManager.h"
#include "safe_delete.h"

void SceneManager::Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera)
{
	ModelManager::GetIns()->Initialize();
	gameScene = new GameScene;
	gameScene->Initialize(dxCommon, input, mouse, camera);
	batlleScene = new BatlleScene;
	batlleScene->Initialize(dxCommon, input, mouse, camera, gameScene);
	titleScene = new TitleScene;
	titleScene->Initialize(dxCommon);
	endScene = new EndScene;
	endScene->Initialize(dxCommon);

	sceneNo = SCENE_TITLE;
	//sceneNo = SCENE_GAME;
}

void SceneManager::Update(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera)
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

	//ゲームリセット
	if (Input::GetInstance()->PushKey(DIK_R)||initFlag==true) {
		safe_delete(gameScene);
		safe_delete(batlleScene);
		gameScene = new GameScene();
		batlleScene = new BatlleScene();
		gameScene->Initialize(dxCommon, input, mouse, camera);
		batlleScene->Initialize(dxCommon, input, mouse, camera,gameScene);
		initFlag = false;
	}

	//シーンの更新
	if (sceneNo == SCENE_TITLE) {
		titleScene->Update(sceneNo, initFlag);
	}
	if (sceneNo == SCENE_END|| sceneNo == SCENE_KATI) {
		endScene->Update(sceneNo);
	}
	if (sceneNo == SCENE_GAME) {
		gameScene->Update(sceneNo,batlleScene);
	}
	if(sceneNo ==SCENE_BATTLE) {
		batlleScene->Update(sceneNo, gameScene);
	}
}

void SceneManager::Draw()
{
	if (sceneNo == SCENE_TITLE) {
		titleScene->Draw();
	}
	if (sceneNo == SCENE_END) {
		endScene->Draw(sceneNo);
	}
	if (sceneNo == SCENE_GAME) {
		gameScene->Draw();
	}
	if (sceneNo == SCENE_BATTLE) {
		batlleScene->Draw();
	}

}

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
