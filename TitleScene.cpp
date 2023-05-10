#include "TitleScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "safe_delete.h"

//#include <D3dx9math.h>

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	safe_delete(titleSprite);
	safe_delete(black);
	for (int i = 0; i < 5; i++) {
		safe_delete(sceneEffect[i]);
	}
	safe_delete(scene);
	safe_delete(titleObj);
	safe_delete(tenQ);
	safe_delete(camera);
}

//void a(VECTOR4* pOut, VECTOR4* pV, DirectX::XMMATRIX* pM)
//{
//	float vx, vy, vz, vw;
//	VECTOR4 vec;
//	float* pF1, * pF2, * pF3, * pF4;
//
//	vx = pV->x;
//	vy = pV->y;
//	vz = pV->z;
//	vw = pV->w;
//
//	pF1 = pM->m[0];
//	pF2 = pM->m[1];
//	pF3 = pM->m[2];
//	pF4 = pM->m[3];
//	vec.x = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + vw * (*pF4);
//	pF1++; pF2++; pF3++; pF4++;
//
//	vec.y = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + vw * (*pF4);
//	pF1++; pF2++; pF3++; pF4++;
//
//	vec.z = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + vw * (*pF4);
//	pF1++; pF2++; pF3++; pF4++;
//
//	vec.w = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + vw * (*pF4);
//
//	*pOut = vec;
//}

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	titleSprite = Sprite::Create(SpriteManager::title, { 0,0 });
	black = Sprite::Create(SpriteManager::black, { 0,0 });
	blackOutFlag = false;
	blackOutAlpha = 0.0f;
	blackInAlpha = 1.0f;
	black->SetAlpha(blackOutAlpha);

	sceneEffect[0] = Sprite::Create(SpriteManager::effect_1, { 0,0 });
	sceneEffect[1] = Sprite::Create(SpriteManager::effect_2, { 0,0 });
	sceneEffect[2] = Sprite::Create(SpriteManager::effect_3, { 0,0 });
	sceneEffect[3] = Sprite::Create(SpriteManager::effect_4, { 0,0 });
	sceneEffect[4] = Sprite::Create(SpriteManager::effect_5, { 0,0 });

	camera = Camera::Create();
	ObjectObj::SetCamera(camera);
	titleObj = ObjectObj::Create(ModelManager::GetModel("enemy"));
	tenQ = ObjectObj::Create(ModelManager::GetModel("tenQ"));

	scene = new SceneLoader();
	scene->Initialize("title");
}

void TitleScene::Update(int& sceneNo, bool& initFlag)
{
	//ShowCursor(true);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || InputMouse::GetInstance()->PushMouse(M_LEFT)) {
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

	//titleObj->VecSetPosition({ float(Input::GetInstance()->PushKey(DIK_D))/2.0f - float(Input::GetInstance()->PushKey(DIK_A)) / 2.0f,
	//	float(Input::GetInstance()->PushKey(DIK_Q)) / 2.0f - float(Input::GetInstance()->PushKey(DIK_E)) / 2.0f,
	//	float(Input::GetInstance()->PushKey(DIK_W)) / 2.0f - float(Input::GetInstance()->PushKey(DIK_S)) / 2.0f });
	titleObj->Update();
	tenQ->Update();

	scene->Update();

	XMMATRIX mat = DirectX::XMMatrixIdentity();
	mat = DirectX::XMMatrixMultiply(mat, XMMatrixRotationX(XMConvertToRadians(100.0f)));
	mat = DirectX::XMMatrixMultiply(mat, XMMatrixRotationY(XMConvertToRadians(100.0f)));
	XMVECTOR target;

}

void TitleScene::Draw()
{
	ObjectObj::PreDraw(dxCommon->GetCmdList());
	titleObj->Draw();
	tenQ->Draw();
	scene->Draw();
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	titleSprite->Draw();
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
