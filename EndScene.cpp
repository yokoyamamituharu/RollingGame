#include "EndScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "safe_delete.h"

EndScene::EndScene()
{
}

EndScene::~EndScene()
{
	safe_delete(sprite);
	safe_delete(katisprite);
}

void EndScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	sprite = Sprite::Create(6, { 0,0 });
	katisprite = Sprite::Create(8, { 0,0 });
}

void EndScene::Update(int& sceneNo)
{
	//ShowCursor(true);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)/* || InputMouse::GetInstance()->PushMouse(M_LEFT)*/) {
		sceneNo = SceneManager::SCENE_TITLE;
	}
}

void EndScene::Draw(int& sceneNo)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	if (sceneNo == SceneManager::SCENE_KATI) {
		katisprite->Draw();
	}
	else{
		sprite->Draw();
	}
	
	Sprite::PostDraw();
}
