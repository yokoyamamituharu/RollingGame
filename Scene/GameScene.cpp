#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include "StrongZakoEnemy.h"
#include "tuyoEnemy.h"
#include "safe_delete.h"
#include "ModelManager.h"
#include "Collision.h"

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
	safe_delete(postEffect);

	//3Dオブジェクト解放
	safe_delete(objectFBX);
	safe_delete(fbxmodel);
	safe_delete(kabe);
	safe_delete(kabe2);
	safe_delete(tenQ);
	safe_delete(ground);
	safe_delete(castle);
	safe_delete(suana);
	safe_delete(suana2);
	safe_delete(defenseTower);
	safe_delete(player);
	safe_delete(copyPlayer);
	safe_delete(copyCastle);
	safe_delete(copyGround);
	safe_delete(copyDefenseTower);		
	enemiesG.clear();
	dasu.clear();
	safe_delete(gameCamera);
	safe_delete(scene);
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

	//3Dオブジェクトの生成
	ground = ObjectObj::Create(ModelManager::GetModel("ground"));
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	castle = ObjectObj::Create(ModelManager::GetModel("castle"));
	castle->SetScale({ 10.0f,10.0f,10.0f });
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
	tenQ = ObjectObj::Create(ModelManager::GetModel("tenQ"));
	tenQ->SetScale({ 5,5,5 });

	//プレイヤーの生成処理
	player = Player::Create(gameCamera);
	player->object->SetPosition({ 0.0f,-6.0f,-50.0f });

	//敵の生成処理
	//std::unique_ptr<EnemyZako> enemy1 = std::make_unique<EnemyZako>();
	//enemy1->Initialize(EnemyZako::FIELD_OUT, camera, { -50, EnemyZako::groundOutPos,0 });
	//enemy1->SetPlayer(player);
	//enemy1->object->SetRotation({ 0,90,0 });
	//enemiesG.push_back(std::move(enemy1));
	enemiesG.clear();

	//タワーの生成処理
	defenseTower = DefenseTower::Create();
	defenseTower->GetObjectOBJ()->SetPosition({ 20,0,20 });

	//ポストエフェクトの生成処理
	postEffect = new PostEffect();
	postEffect->Initialize();

	//キャンバスの生成処理
	canvas = new Canvas();
	canvas->Initialize();

	index = 0;

	dasu[0] = { 0,1 };
	dasu[1] = { 0,1 };
	dasu[2] = { 0,1 };
	dasu[3] = { 0,1 };
	dasu[4] = { 0,1 };
	dasu[5] = { 0,1 };
	dasu[6] = { 0,1 };
	dasu[7] = { 0,1 };
	dasu[8] = { 0,1 };
	dasu[9] = { 0,1 };

	Player::breakEnemy = 0;

	subCamera = Camera::Create();
	subCamera->SetEye({ 0, 100, -1 });
	subCamera->SetTarget({ 0, 0, 0 });
	copyPlayer = new CopyObject;
	copyPlayer->InitializeC(player->object);
	copyGround = new CopyObject;
	copyGround->InitializeC(ground);

	copyCastle = new CopyObject;
	copyCastle->InitializeC(castle);
	copyDefenseTower = new CopyObject;
	copyDefenseTower->InitializeC(defenseTower->GetObjectOBJ());

	//postEffect->SetSize({ 100,100 });

	postEffect->SetSize({ 1,1 });
	gameCamera->SetPlayer(player->object);

	//postEffect->SetColor({ 0.5,0.5,0.5});	
	scene = new SceneLoader;
	scene->Initialize();
}

void GameScene::Update(int& sceneNo, BatlleScene* batlleScene)
{
	ObjectObj::SetCamera(gameCamera);

	if (Input::GetInstance()->TriggerKey(DIK_B))
	{
		blackStartFlag = true;
	}

	if (blackStartFlag == true) {
		blackTime -= 0.05;
		if (blackTime > 0) {
			postEffect->SetColor({ blackTime,blackTime,blackTime });
		}
		else {
			blackTime2 += 0.05f;
			postEffect->SetColor({ blackTime2,blackTime2,blackTime2 });
		}
		if (blackTime2 >= 1) {
			blackStartFlag = false;
			blackTime = 1.0f;
			blackTime2 = 0.0f;
		}
	}



	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (enemy->GetHp() <= 0) {
			Player::breakEnemy += 1;
		}
	}
	enemiesG.remove_if([](std::shared_ptr<EnemyZako>& enemy) {return enemy->GetDead(); });

	if (Input::GetInstance()->PushKey(DIK_UP)) {
		//spriteBG->SetSize({ 2, 2 });
		XMFLOAT2 postSize = postEffect->GetSize();
		postSize.x += 0.1;
		postSize.y += 0.1;
		postEffect->SetSize(postSize);
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		//spriteBG->SetSize({ 100, 100 });
		//postEffect->SetSize({ 100,100 });
		XMFLOAT2 postSize = postEffect->GetSize();
		postSize.x -= 0.1;
		postSize.y -= 0.1;
		postEffect->SetSize(postSize);
	}


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



	//敵を行動させるかさせないかのトルグスイッチ
	//EnemyZako::Action();

	//敵とプレイヤーの当たり判定
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (CollisitonBoxToBox(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			if (enemy->GetDead() == false) {
				//バトルシーンに行く処理
				batlleScene->SetEnemies(enemy);
				enemiesG.remove(enemy);
				player->outPos = player->object->GetPosition();
				player->Stop();
				//プレイヤーを原点に発生させる
				//player->object->SetPosition({ 0,-6,0 });
				sceneNo = SceneManager::SCENE_BATTLE;
				break;
			}
		}
		//敵と城の当たり判定
		if (CollisitonBoxToBox(enemy->object->GetPosition(), { 2.5,5,1 }, castle->GetPosition(), { 10,10,10 })) {
			//当たったら負け
			//sceneNo = SceneManager::SCENE_END;
		}
	}
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	player->Move();
	scene->Update();
	if (scene->Collision(player->object->GetPosition() + player->move, { 2.5,5,1 })) {
		player->move = { 0,0,0 };
	}

	//3Dオブジェクト更新
	player->Update();
	ground->Update();
	//defenseTower->Update(enemiesG);
	castle->Update();
	suana->Update();
	suana2->Update();
	kabe->Update();
	kabe2->Update();
	tenQ->Update();
	scene->Update();
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Update();
	}
	if (enemiesG.size() <= 0 && index >= 5) {
		sceneNo = SceneManager::SCENE_KATI;
	}

	//カメラのアップデート
	gameCamera->Update();
	subCamera->Update();


	copyPlayer->Update(player->object, subCamera);
	copyGround->Update(ground, subCamera);
	copyCastle->Update(castle, subCamera);
	copyDefenseTower->Update(defenseTower->GetObjectOBJ(), subCamera);
	PostReserve();	//ミニマップの描画前処理
	subCamera->SetTarget(player->object->GetPosition());
	subCamera->SetEye({ player->object->GetPosition().x + 1,player->object->GetPosition().y + 100, player->object->GetPosition().z });

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
	//objectFBX->Draw(dxCommon->GetCmdList());
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Draw();
	}
	defenseTower->Draw();
	//ground->Draw();	
	//castle->Draw();
	//suana->Draw();
	//suana2->Draw();
	//kabe->Draw();
	//kabe2->Draw();
	//tenQ->Draw();
	player->Draw();
	scene->Draw();
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	canvas->Draw();
	Sprite::PostDraw();
}

void GameScene::PostReserve()
{
	postEffect->PreDrawScene(dxCommon->GetCmdList());

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

	postEffect->PosDrawScene(dxCommon->GetCmdList());
}

void GameScene::PostDraw()
{
	postEffect->Draw(dxCommon->GetCmdList());
}
