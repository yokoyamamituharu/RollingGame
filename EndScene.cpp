#include "EndScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "safe_delete.h"

EndScene::EndScene()
{
}

EndScene::~EndScene()
{
	safe_delete(maleSprite);
	safe_delete(katisprite);
}

void EndScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	maleSprite = Sprite::Create(SpriteManager::end, { 0,0 });
	katisprite = Sprite::Create(SpriteManager::kati, { 0,0 });
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
		maleSprite->Draw();
	}
	
	Sprite::PostDraw();
}
