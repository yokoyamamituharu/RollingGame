#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include <DirectXMath.h>
#include "Useful.h"
#include "safe_delete.h"
#include "../Collider/Collision.h"

using namespace DirectX;


BatlleScene::BatlleScene()
{
}

BatlleScene::~BatlleScene()
{
	//スプライト解放
	safe_delete(canvas);
	safe_delete(spriteBG);

	//3Dオブジェクト解放
	safe_delete(tenQ);
	safe_delete(ground);
	safe_delete(player);
	safe_delete(area);
	enemy1.reset();
	safe_delete(gameCamera);
}

void BatlleScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;

	gameCamera = GameCamera::Create();

	player = Player::Create(gameCamera, 2);
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	gameCamera->SetPlayer(player->object);

	ground = ObjectObj::Create(ModelManager::GetModel("battlegrund"));	
	ground->SetScale({ 1000.0f,1.0f,1000.0f });
	ground->SetPosition({ 0.0f,-10.1f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	tenQ = ObjectObj::Create(ModelManager::GetModel("IntenQ"));	
	tenQ->SetScale({ 10.0f,1.0f,10.0f });
	tenQ->SetPosition({ 0.0f,-10.0f,0.0f });
	tenQ->SetRotation({ 0.0f,0.0f,0.0f });
	tenQ->Update();

	area = ObjectObj::Create(ModelManager::GetModel("area"));
	area->SetScale({ 50.0f,1.0f,50.0f });
	area->SetPosition({ 0.0f,-10.0f,0.0f });
	//area->SetRotation({ 0.0f,0.0f,0.0f });
	

	canvas = new Canvas();
	canvas->Initialize();
}

void BatlleScene::Update(int& sceneNo, GameScene* gameScene)
{
	EnemyZako::isAction = 1;
	//EnemyZako::Action();


	ObjectObj::SetCamera(gameCamera);
	if (SceneManager::hitEnemyToPlayer || SceneManager::WinBattle) {
		return;
	}

	if (SceneManager::BattleInit == true) {
		for (std::unique_ptr<EnemyZako>& enemy : enemy1->GetEnemies()) {
			enemy->NotDead();
		}
		SceneManager::BattleInit = false;
	}

	//敵の情報を外シーンから取得できていたら処理
	if (enemy1 != 0) {
		//死亡判定があったらエネミーを消す
		enemy1->GetEnemies().remove_if([](std::unique_ptr<EnemyZako>& enemy) {return enemy->GetDead(); });
		//敵とプレイヤーのローリング攻撃の当たり判定
		for (std::unique_ptr<EnemyZako>& enemy : enemy1->GetEnemies()) {
			if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
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
			for (std::unique_ptr<EnemyZako>& enemy : enemy1->GetEnemies()) {
				enemy->SetPlayer(player);
				enemy->UpdateIn();
			}
		}




		//バトルシーンから脱出するシーン
		if (enemy1->GetEnemies().size() == 0) {
			player->object->SetPosition(player->outPos);
			player->Stop();
			player->breakEnemy++;	//敵の撃破数を増やす
			enemy1.reset();
			//sceneNo = SceneManager::SCENE_GAME;
			SceneManager::WinBattle = true;
		}
		if (player->GetHp() <= 0) {
			player->Stop();
			player->Cure(5);
			sceneNo = SceneManager::SCENE_END;
		}
	}

	const int maxEnemy = 8;
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//player->Res(true);
	}

	gameCamera->Update();
	ground->Update();
	player->Res();
	player->Move();
	//プレイヤーをエリア内に収める
	if (Collision::CheckDistance(player->object->GetPosition() + player->move, { 0,0,0 }) > 125.0f) {
		player->move = { 0,0,0 };
	}
	player->Update();
	player->Res();
	tenQ->Update();
	area->Update();

	canvas->SetHp(player->GetMaxHp(), player->GetHp());



}

void BatlleScene::Draw()
{
	ObjectObj::PreDraw(dxCommon->GetCmdList());
	tenQ->Draw();
	area->Draw();

	player->Draw();
	ground->Draw();
	if (enemy1 != 0) {
		for (std::unique_ptr<EnemyZako>& enemy : enemy1->GetEnemies()) {
			enemy->Draw();
		}
	}
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	spriteBG->Draw();
	canvas->Draw();
	Sprite::PostDraw();
}