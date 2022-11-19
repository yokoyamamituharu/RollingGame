#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include <DirectXMath.h>
#include "../Useful.h"
#include "safe_delete.h"

using namespace DirectX;

bool CubeCollision1(XMFLOAT3 object1, XMFLOAT3 radius1, XMFLOAT3 object2, XMFLOAT3 radius2) {
	float disX1 = object1.x + radius1.x;
	float disX2 = object2.x - radius2.x;
	float disX3 = object1.x - radius1.x;
	float disX4 = object2.x + radius2.x;
	float disY1 = object1.y + radius1.y;
	float disY2 = object2.y - radius2.y;
	float disY3 = object1.y - radius1.y;
	float disY4 = object2.y + radius2.y;
	float disZ1 = object1.z + radius1.z;
	float disZ2 = object2.z - radius2.z;
	float disZ3 = object1.z - radius1.z;
	float disZ4 = object2.z + radius2.z;

	//xとyはそれぞれ中心座標として計算する
	return disX1 > disX2 && disX4 > disX3 && disY1 > disY2 && disY4 > disY3 && disZ1 > disZ2 && disZ4 > disZ3;
}
BatlleScene::BatlleScene()
{
}

BatlleScene::~BatlleScene()
{
	//スプライト解放
	safe_delete(canvas);

	//3Dオブジェクト解放
	safe_delete(ground);
	safe_delete(tenQ);
	enemy1.reset();
}

void BatlleScene::Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera, GameScene* gameScene)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;

	this->player = gameScene->GetPlayer();

	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	claerSprite = Sprite::Create(2, { 100.0f,100.0f });


	ground = OBJobject::Create();
	ground->SetModel(ModelManager::GetModel("battlegrund"));
	ground->SetScale({ 1000.0f,1.0f,1000.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	tenQ = OBJobject::Create();
	tenQ->SetModel(ModelManager::GetModel("IntenQ"));
	tenQ->SetScale({ 10.0f,1.0f,10.0f });
	tenQ->SetPosition({ 0.0f,-10.0f,0.0f });
	tenQ->SetRotation({ 0.0f,0.0f,0.0f });
	tenQ->Update();

	canvas = new Canvas();
	canvas->Initialize();
}

void BatlleScene::Update(int& sceneNo, GameScene* gameScene)
{
	//カメラ操作
	if (input->PushKey(DIK_RIGHT)) {
		camera->matRot *= XMMatrixRotationY(0.1f);
	}
	else if (input->PushKey(DIK_LEFT)) {
		camera->matRot *= XMMatrixRotationY(-0.1f);
	}
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		cameraToMouse *= -1;
	}
	if (cameraToMouse > 0) {
		camera->matRot *= XMMatrixRotationY(0.8f * mouse->MoveMouseVector('x') / 1000);
	}


	EnemyZako::isAction = 1;
	//EnemyZako::Action();

	XMFLOAT3 rote = player->object->GetRotation();
	XMFLOAT3 pos = player->object->GetPosition();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, camera->matRot);
	movement *= XMVECTOR{ -1, -1, -1 };

	matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	camera->SetEye({ player->object->GetPosition().x + movement.m128_f32[0] * 80, player->object->GetPosition().y + movement.m128_f32[1] * 80,
		 player->object->GetPosition().z + movement.m128_f32[2] * 80 });
	camera->eye.y = 20;
	camera->target = player->object->GetPosition();
	camera->target.y = -6;



	//敵の情報を外シーンから取得できていたら処理
	if (enemy1 != 0) {
		//死亡判定があったらエネミーを消す
		enemy1->GetEnemies().remove_if([](std::unique_ptr<EnemyZako>& enemy) {return enemy->IsDead(); });
		//敵とプレイヤーのローリング攻撃の当たり判定
		for (std::unique_ptr<EnemyZako>& enemy : enemy1->GetEnemies()) {
			if (CubeCollision1(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })
				) {
				if (player->attackFlag == true) {
					enemy->SetDead();
					XMVECTOR pos1 = XMLoadFloat3(&player->object->GetPosition());
					XMVECTOR pos2 = XMLoadFloat3(&enemy->object->GetPosition());
					XMVECTOR vec = pos1 - pos2;
					vec = XMVector3Normalize(vec);
					vec.m128_f32[1] = 0;//ここを0にしないとプレイヤーと敵のY座標のずれで敵の突進方向がずれる
					player->Res(true, Use::LoadXMVECTOR(vec));
				}
				else if (enemy->GetAttack()) {
					player->Damage(1);
				}
			}
		}
		for (std::unique_ptr<EnemyZako>& enemy : enemy1->GetEnemies()) {
			enemy->SetPlayer(player);
			enemy->Update();
		}
	}

	const int maxEnemy = 12;
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//player->Res(true);
	}

	camera->Update();
	ground->Update();
	//player->Res();
	player->Update();
	//player->Res();
	tenQ->Update();

	canvas->SetHp(player->GetMaxHp(), player->GetHp());


	//バトルシーンから脱出するシーン
	if (enemy1->GetEnemies().size() == 0) {
		player->object->SetPosition(player->outPos);
		player->Stop();
		player->breakEnemy++;	//敵の撃破数を増やす
		enemy1.reset();
		sceneNo = SceneManager::SCENE_GAME;
	}
	if (player->GetHp() <= 0) {
		player->Stop();
		player->Cure(5);
		sceneNo = SceneManager::SCENE_END;
	}
	//if (Input::GetInstance()->TriggerKey(DIK_B)) {
	//	gameScene->SetEnemy(enemy1);
	//	player->object->SetPosition(player->outPos);
	//	sceneNo = SceneManager::SCENE_GAME;
	//}

}

void BatlleScene::Draw()
{
	OBJobject::PreDraw(dxCommon->GetCmdList());
	tenQ->Draw();

	player->object->Draw();
	ground->Draw();
	//fbxobject->Draw(dxCommon->GetCmdList());

	int aliveNum = 0;

	if (enemy1 != 0) {
		for (std::unique_ptr<EnemyZako>& enemy : enemy1->GetEnemies()) {
			enemy->Draw();
			aliveNum++;
		}
	}



	OBJobject::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	spriteBG->Draw();
	if (aliveNum == 0) {
		claerSprite->Draw();
	}
	canvas->Draw();
	Sprite::PostDraw();
}