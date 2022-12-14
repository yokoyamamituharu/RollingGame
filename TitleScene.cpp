#include "TitleScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "safe_delete.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	safe_delete(sprite);
	safe_delete(black);
}

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	sprite = Sprite::Create(5, { 0,0 });
	black = Sprite::Create(7, { 0,0 });
	blackOutFlag = false;
	blackOutAlpha = 0.0f;
	blackInAlpha = 1.0f;
	black->SetAlpha(blackOutAlpha);

	sceneEffect[0] = Sprite::Create(21, { 0,0 });
	sceneEffect[1] = Sprite::Create(22, { 0,0 });
	sceneEffect[2] = Sprite::Create(23, { 0,0 });
	sceneEffect[3] = Sprite::Create(24, { 0,0 });
	sceneEffect[4] = Sprite::Create(25, { 0,0 });
}

void TitleScene::Update(int& sceneNo, bool& initFlag)
{
	ShowCursor(true);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)||InputMouse::GetInstance()->PushMouse(M_LEFT)) {
		SceneManager::blackStartFlag = true;
	}
	//if (Input::GetInstance()->TriggerKey(DIK_0)) {
	//	blackInFlag = true;
	//}

	if (BlackOut() == black_end) {
		sceneNo = SceneManager::SCENE_GAME;
		initFlag = true;
	}
	BlackIn();

	//if (Input::GetInstance()->TriggerKey(DIK_E)) {
	//	effectFlag = true;
	//}
	if (effectFlag == true) {
		effectTime++;
		if (effectTime > 10) {
			effectIndex++;
			effectTime = 0;
		}
		if (effectIndex >= 5) {
			effectIndex = 0;
			effectTime = 0;
			effectFlag = false;
		}
	}
}

void TitleScene::Draw()
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	sprite->Draw();
	black->Draw();	
	if (effectFlag == true) {
		sceneEffect[effectIndex]->Draw();
	}	
	Sprite::PostDraw();
}

int TitleScene::BlackOut()
{
	if (blackOutFlag == true) {
		float blackOutSpeed = 0.05;
		blackOutAlpha += blackOutSpeed;
		if (blackOutAlpha > 1) {
			blackOutFlag = false;
			blackOutAlpha = 0.0f;
			black->SetAlpha(0.0f);
			return black_end;
		}
		black->SetAlpha(blackOutAlpha);
		return black_being;
	}
	return black_none;
}

int TitleScene::BlackIn()
{
	if (blackInFlag == true) {
		float blackInSpeed = -0.05;
		blackInAlpha += blackInSpeed;
		if (blackInAlpha < 0) {
			blackInFlag = false;
			blackInAlpha = 1.0f;
			black->SetAlpha(0.0f);
			return black_end;
		}
		black->SetAlpha(blackInAlpha);
		return black_being;
	}
	return black_none;
}
