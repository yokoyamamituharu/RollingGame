#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include "StrongZakoEnemy.h"
#include "tuyoEnemy.h"
#include "safe_delete.h"
#include "ModelManager.h"
#include "../Collider/Collision.h"
#include "Useful.h"
#include "SphereCollider.h"

DirectX::XMFLOAT3 initTarget = { 0,-10,20 };
DirectX::XMFLOAT3 initEye = { 0,20,-25 };
using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//スプライト解放
	safe_delete(canvas);
	safe_delete(spriteBG);
	safe_delete(clearsprite);
	safe_delete(miniMapPost);
	//3Dオブジェクト解放
	safe_delete(scene);
	safe_delete(tenQ);
	safe_delete(ground);
	safe_delete(castle);
	safe_delete(suana);
	safe_delete(suana2);
	safe_delete(kabe);
	safe_delete(kabe2);
	safe_delete(defenseTower);
	safe_delete(player);
	safe_delete(copyGround);
	safe_delete(copyCastle);
	safe_delete(copyDefenseTower);
	safe_delete(copyPlayer);
	enemiesG.clear();
	dasu.clear();
	safe_delete(gameCamera);
	safe_delete(touchCastle);
	safe_delete(touchGround);
	safe_delete(subCamera);

}

void GameScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;

	//カメラの初期位置、注視点
	gameCamera = GameCamera::Create();
	gameCamera->SetTarget(initTarget);
	gameCamera->SetEye(initEye);
	ObjectObj::SetCamera(gameCamera);

	//スプライトの生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	clearsprite = Sprite::Create(2, { 100.0f,100.0f });
	//キャンバスの生成処理
	canvas = new Canvas();
	canvas->Initialize();

	collisionManager = CollisionManager::GetInstance();

	//3Dオブジェクトの生成
	tenQ = ObjectObj::Create(ModelManager::GetModel("tenQ"));
	tenQ->SetScale({ 5,5,5 });
	ground = ObjectObj::Create(ModelManager::GetModel("ground"));
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	castle = ObjectObj::Create(ModelManager::GetModel("castle"));
	castle->SetScale({ 10.0f,10.0f,10.0f });
	//touchCastle = TouchableObject::Create(ModelManager::GetModel("castle"));
	//touchCastle->SetScale({ 10.0f,10.0f,10.0f });
	suana = ObjectObj::Create(ModelManager::GetModel("suana"));
	suana->SetPosition({ 100.0f,0.0f,100.0f });
	suana->SetScale({ 10.0f,10.0f,10.0f });
	suana->SetRotation({ 0,90,0 });
	suana2 = ObjectObj::Create(ModelManager::GetModel("suana"));
	suana2->SetPosition({ -100.0f,0.0f,-100.0f });
	suana2->SetScale({ 10.0f,10.0f,10.0f });
	suana2->SetRotation({ 0,-90,0 });
	kabe = ObjectObj::Create(ModelManager::GetModel("kabe"));
	kabe->SetPosition({ 70.0f,-5.0f,50.0f });
	kabe->SetScale({ 5.0f,5.0f,5.0f });
	kabe2 = ObjectObj::Create(ModelManager::GetModel("kabe"));
	kabe2->SetPosition({ -60.0f,-5.0f,-50.0f });
	kabe2->SetScale({ 5.0f,5.0f,5.0f });
	kabe2->SetRotation({ 0,180,0 });
	touchGround = TouchableObject::Create(ModelManager::GetModel("ground"));
	touchGround->SetScale({ 10.0f,1.0f,10.0f });
	touchGround->SetPosition({ 0.0f,-15.0f,0.0f });
	//touchGround->collider.

	//タワーの生成処理
	defenseTower = DefenseTower::Create();
	defenseTower->GetObjectObj()->SetPosition({ 20,0,20 });

	//プレイヤーの生成処理
	player = Player::Create(gameCamera,1);
	Player::breakEnemy = 0;
	//ゲームカメラにプレイヤーをセット
	gameCamera->SetPlayer(player->object);

	index = 0;
	dasu[0] = { 0,1 };
	dasu[1] = { 0,2 };
	dasu[2] = { 30,1 };
	dasu[3] = { 10,1 };
	dasu[4] = { 40,2 };
	dasu[5] = { 50,2 };
	dasu[6] = { 30,1 };
	dasu[7] = { 80,2 };
	dasu[8] = { 40,1 };
	dasu[9] = { 10,1 };


	//ミニマップ用カメラの生成
	subCamera = Camera::Create();
	subCamera->SetEye({ 0, 100, -1 });
	subCamera->SetTarget({ 0, 0, 0 });
	//ミニマップ用オブジェクトの初期化	
	copyGround = CopyObject::Create(ground);
	copyCastle = CopyObject::Create(castle);
	copyDefenseTower = CopyObject::Create(defenseTower->GetObjectObj());
	copyPlayer = CopyObject::Create(player->object);
	//ミニマップ用ポストエフェクト生成処理
	miniMapPost = new PostEffect();
	miniMapPost->Initialize();
	miniMapPost->SetSize({ 1,1 });

	scene = new SceneLoader;
	scene->Initialize();

	//castle->SetCollider(new SphereCollider);
}

void GameScene::Update(int& sceneNo, BatlleScene* batlleScene)
{
	ObjectObj::SetCamera(gameCamera);
	if (SceneManager::hitEnemyToPlayer|| SceneManager::WinBattle) {
		return;
	}

	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (enemy->GetHp() <= 0) {
			Player::breakEnemy += 1;
		}
	}
	enemiesG.remove_if([](std::shared_ptr<EnemyZako>& enemy) {return enemy->GetDead(); });

	if (Input::GetInstance()->PushKey(DIK_UP)) {
		//spriteBG->SetSize({ 2, 2 });
		XMFLOAT2 postSize = miniMapPost->GetSize();
		postSize.x += 0.1;
		postSize.y += 0.1;
		miniMapPost->SetSize(postSize);
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		//spriteBG->SetSize({ 100, 100 });
		//miniMapPost->SetSize({ 100,100 });
		XMFLOAT2 postSize = miniMapPost->GetSize();
		postSize.x -= 0.1;
		postSize.y -= 0.1;
		miniMapPost->SetSize(postSize);
	}

	//敵を行動させるかさせないかのトルグスイッチ
	//EnemyZako::Action();

	//敵生成処理
	if (index <= 6) {
		dasu[index].timer--;
		while (dasu[index].timer <= 0) {
			if (dasu[index].timer <= 0) {
				if (dasu[index].basyo == 1) {
					//タワーがある方
					std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
					newEnemy->Initialize(EnemyZako::FIELD_OUT, gameCamera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, XMFLOAT3{ 0, 0, -100 }, XMFLOAT3{ -100,0,0 });
					enemiesG.push_back(std::move(newEnemy));
				}
				if (dasu[index].basyo == 2) {
					std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
					newEnemy->Initialize(EnemyZako::FIELD_OUT, gameCamera, { suana2->GetPosition().x,EnemyZako::groundOutPos,suana2->GetPosition().z }, true, XMFLOAT3{ 0, 0, +100 }, XMFLOAT3{ +100,0,0 });
					enemiesG.push_back(std::move(newEnemy));
				}
				index++;
			}
			if (index >= 6) {
				break;
			}
		}
	}

	//敵とプレイヤーの当たり判定
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			if (enemy->GetDead() == false) {
				//バトルシーンに行く処理
				batlleScene->SetEnemies(enemy);
				enemiesG.remove(enemy);
				player->outPos = player->object->GetPosition();
				player->Stop();
				//プレイヤーを原点に発生させる
				//player->object->SetPosition({ 0,-6,0 });
				SceneManager::hitEnemyToPlayer = true;
				//sceneNo = SceneManager::SCENE_BATTLE;
				break;
			}
		}
		//敵と城の当たり判定
		if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, castle->GetPosition(), { 10,10,10 })) {
			//当たったら負け
			//sceneNo = SceneManager::SCENE_END;
		}
	}

	//プレイヤーとシーンオブジェクトの当たり判定
	player->Move();
	//scene->Update();
	//if (scene->Collision(player->object->GetPosition() + player->move, { 2.5,5,1 })) {
	//	player->move = { 0,0,0 };
	//}

	//3Dオブジェクト更新
	player->Update();
	ground->Update();
	defenseTower->Update(enemiesG);
	castle->Update();
	//touchCastle->Update();
	suana->Update();
	suana2->Update();
	kabe->Update();
	kabe2->Update();
	tenQ->Update();
	scene->Update();
	touchGround->Update();
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Update();
	}
	if (enemiesG.size() <= 0 && index >= 5) {
		sceneNo = SceneManager::SCENE_KATI;
	}
	//カメラのアップデート
	gameCamera->Update();
	subCamera->Update();

	//キャンバスにプレイヤーの情報をセット
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	//ミニマップ用オブジェクトの更新
	copyPlayer->Update(player->object, subCamera);
	copyGround->Update(ground, subCamera);
	copyCastle->Update(castle, subCamera);
	copyDefenseTower->Update(defenseTower->GetObjectObj(), subCamera);
	subCamera->SetTarget(player->object->GetPosition());
	subCamera->SetEye({ player->object->GetPosition().x + 1,player->object->GetPosition().y + 100, player->object->GetPosition().z });
	PostReserve();	//ミニマップの描画前処理

	collisionManager->CheckAllCollisions();
	if (collisionManager->GetPlayerTikei()) {
		player->StopRolling();
	}
}

void GameScene::Draw()
{
	if (Input::GetInstance()->TriggerKey(DIK_M)) {
		if (mapFlag == true) { mapFlag = false; }
		else { mapFlag = true; }
	}
	if (mapFlag == true) {
		PostDraw();	//ミニマップの描画
	}

	ObjectObj::PreDraw(dxCommon->GetCmdList());
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Draw();
	}
	scene->Draw();
	//tenQ->Draw();
	//ground->Draw();	
	//castle->Draw();
	//touchCastle->Draw();
	suana->Draw();
	suana2->Draw();
	//kabe->Draw();
	//kabe2->Draw();
	defenseTower->Draw();
	player->Draw();
	//touchGround->Draw();
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	canvas->Draw();
	Sprite::PostDraw();
}

void GameScene::PostReserve()
{
	miniMapPost->PreDrawScene(dxCommon->GetCmdList());

	//ポストエフェクトさせたいオブジェクト
	ObjectObj::PreDraw(dxCommon->GetCmdList());
	copyPlayer->Draw();
	copyGround->Draw();
	copyCastle->Draw();
	copyDefenseTower->Draw();
	ObjectObj::PostDraw();

	//ポストエフェクトさせたいスプライト
	Sprite::PreDraw(dxCommon->GetCmdList());
	spriteBG->Draw();
	Sprite::PostDraw();

	miniMapPost->PosDrawScene(dxCommon->GetCmdList());
}

void GameScene::PostDraw()
{
	miniMapPost->Draw(dxCommon->GetCmdList());
}
