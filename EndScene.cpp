#include "EndScene.h"
#include "Input.h"
#include "SceneManager.h"

void EndScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	sprite = Sprite::Create(6, { 0,0 });
}

void EndScene::Update(int& sceneNo)
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = SceneManager::SCENE_TITLE;
	}
}

void EndScene::Draw()
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	sprite->Draw();
	Sprite::PostDraw();
}
