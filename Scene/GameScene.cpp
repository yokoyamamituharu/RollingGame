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
#include "YowaiEnemy.h"
#include "Route.h"

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
	safe_delete(suana);
	safe_delete(suana2);
	safe_delete(defenseTower);
	safe_delete(player);
	safe_delete(copyGround);
	safe_delete(copyCastle);
	safe_delete(copyDefenseTower);
	safe_delete(copyPlayer);
	enemiesG.clear();
	dasu.clear();
	safe_delete(gameCamera);
	safe_delete(touchGround);
	safe_delete(subCamera);
	safe_delete(touchObj);
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
	pose = Sprite::Create(26, { 0,0 });
	//キャンバスの生成処理
	canvas = new Canvas();
	canvas->Initialize();
	tikaiSprite = Sprite::Create(29, { 300,600 });


	collisionManager = CollisionManager::GetInstance();

	//3Dオブジェクトの生成
	suana = ObjectObj::Create(ModelManager::GetModel("suana"));
	suana->SetPosition({ 100.0f,0.0f,100.0f });
	suana->SetScale({ 10.0f,10.0f,10.0f });
	suana->SetRotation({ 0,0,0 });
	suana2 = ObjectObj::Create(ModelManager::GetModel("suana"));
	suana2->SetPosition({ -100.0f,0.0f,-100.0f });
	suana2->SetScale({ 10.0f,10.0f,10.0f });
	suana2->SetRotation({ 0,-90,0 });
	//touchGround = TouchableObject::Create(ModelManager::GetModel("ground"));
	//touchGround->SetScale({ 10.0f,1.0f,10.0f });
	//touchGround->SetPosition({ 0.0f,-15.0f,0.0f });


	tamesi = ObjectObj::Create(ModelManager::GetModel("level"));
	tamesi->SetPosition({ 0.0f,0.0f,0.0f });
	tamesi->SetScale({ 1.0f,1.0f,1.0f });
	tamesi->SetRotation({ 0,0,0 });

	//タワーの生成処理
	defenseTower = DefenseTower::Create();
	defenseTower->GetObjectObj()->SetPosition({ 20,0,35 });
	defenseTower->GetObjectObj()->SetScale({ 15,15,15 });

	//プレイヤーの生成処理
	player = Player::Create(gameCamera, 1);
	player->SetHp(5);
	Player::breakEnemy = 0;
	//ゲームカメラにプレイヤーをセット
	gameCamera->SetPlayer(player->object);

	index = 0;
	dasu[0] = { 0,1 };
	dasu[1] = { 0,2 };
	dasu[2] = { 240,1 };
	dasu[3] = { 200,1 };
	dasu[4] = { 80,2 };
	dasu[5] = { 230,2 };
	dasu[6] = { 120,1 };
	dasu[7] = { 180,2 };
	dasu[8] = { 40,1 };
	dasu[9] = { 80,1 };


	//ミニマップ用カメラの生成
	subCamera = Camera::Create();
	subCamera->SetEye({ 0, 100, -1 });
	subCamera->SetTarget({ 0, 0, 0 });
	//ミニマップ用オブジェクトの初期化	
	//copyGround = CopyObject::Create(ground);
	//copyCastle = CopyObject::Create(scene->GetObjectObj("castle"));
	copyDefenseTower = CopyObject::Create(defenseTower->GetObjectObj());
	copyPlayer = CopyObject::Create(player->object);
	//ミニマップ用ポストエフェクト生成処理
	miniMapPost = new PostEffect();
	miniMapPost->Initialize();
	miniMapPost->SetSize({ 1,1 });

	scene = new SceneLoader;
	//scene->Initialize("level");
	playerSprte = Sprite::Create(31, { 0,0 });
	towerSprte = Sprite::Create(32, { 0,0 });

	Route* route = Route::GetIns();
	route->Set();
	
	objects.push_back(std::move(ObjectObj::CreateS(ModelManager::GetModel("tenQ"), {}, {}, { 5,5,5 })));//天球
	objects.push_back(std::move(ObjectObj::CreateS(ModelManager::GetModel("ground"), {}, {}, { 100,1,100 })));//地面

	touchObj = TouchableObject::Create(ModelManager::GetModel("kabe"),{0,5,-10},{},{5,1,5});
}

void GameScene::Update(int& sceneNo, BatlleScene* batlleScene)
{
	ObjectObj::SetCamera(gameCamera);
	if (SceneManager::hitEnemyToPlayer || SceneManager::WinBattle) {
		return;
	}

	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		if (enemy->GetHp() <= 0) {
			Player::breakEnemy += 1;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		if (poseFlag == false) { poseFlag = true; }
		else { poseFlag = false; }
	}

	if (poseFlag == true) {
		return;
	}

	enemiesG.remove_if([](std::shared_ptr<BaseEnemy>& enemy) {return enemy->GetDead(); });

	//敵を行動させるかさせないかのトルグスイッチ
	//EnemyZako::Action();

	for (int i = 0; i < 2; i++) {

		std::unique_ptr<ParticleTubu> particle = std::make_unique<ParticleTubu>();
		particle->obj = std::make_unique<ObjectObj>();
		particle->obj->Initialize(ModelManager::GetModel("cloudBurst"));
		int scale = rand() % 20 + 1;
		particle->startScale = rand() % 4 + 1;
		particle->obj->SetScale({ float(scale),float(scale),float(scale) });
		particle->obj->SetRotation({ 0,0,0 });
		//プレイヤーの足元に生成
		particle->position = { player->object->GetPosition().x + rand() % 3 - 1, player->object->GetPosition().y - 4 ,player->object->GetPosition().z + rand() % 3 - 1 };

		particle->end_frame = rand() % 5 + 10;
		//int rndVel = 3.0f;
		//particle->velocity.x = rand() % (rndVel * 2) - rndVel;
		//particle->velocity.y = rand() % (rndVel * 2) - rndVel;
		//particle->velocity.z = rand() % (rndVel * 2) - rndVel;
		//tubu->velocity.x = 0;
		//tubu->velocity.y = 0;
		//tubu->velocity.z = -rand() % (rndVel * 2);
		//Particle::GetIns()->Add(std::move(particle));
	}


	//敵生成処理
	if (index <= 6) {
		dasu[index].timer--;
		while (dasu[index].timer <= 0) {
			if (dasu[index].timer <= 0) {
				if (dasu[index].basyo == 1) {
					//タワーがある方
					std::shared_ptr<EnemyZako> newEnemy = EnemyZako::Create(true, Route::GetRoute(1));
					enemiesG.push_back(std::move(newEnemy));
				}
				if (dasu[index].basyo == 2) {
					std::shared_ptr<YowaiEnemy> newEnemy = YowaiEnemy::Create(true, Route::GetRoute(2));
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
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			if (enemy->GetDead() == false) {
				//バトルシーンに行く処理
				batlleScene->SetEnemies(enemy);
				enemiesG.remove(enemy);
				player->outPos = player->object->GetPosition();
				player->StopOut();
				//プレイヤーを原点に発生させる
				//player->object->SetPosition({ 0,-6,0 });
				SceneManager::hitEnemyToPlayer = true;
				SceneManager::BattleInit = true;
				//sceneNo = SceneManager::SCENE_BATTLE;
				break;
			}
		}
		//敵と城の当たり判定
		//if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, { 0,0,0 }, { 10,10,10 })) {
		//	//当たったら負け
		//	sceneNo = SceneManager::SCENE_END;
		//}

		//敵と城が近いかどうか
		if (enemy->tikai == false) {
			//if (Collision::CheckDistance(scene->GetObjectObj("castle")->GetPosition(), enemy->object->GetPosition()) < 30) {
			//	enemy->tikai = true;
			//	isTikai = true;
			//	tikaiStack.push_back(true);
			//}
		}
	}

	if (tikaiStack.size() > 0) {
		isTikai = true;
		tikaiTime++;
		if (tikaiTime < 120) {
			tikaiSprite->SetPosition({ tikaiSprite->GetPosition().x,tikaiSprite->GetPosition().y - 1 });
		}
		else if (tikaiTime < 200) {
			int kyomu = 0;
		}
		else if (tikaiTime < 320) {
			tikaiSprite->SetPosition({ tikaiSprite->GetPosition().x,tikaiSprite->GetPosition().y + 1 });
		}
		else {
			tikaiTime = 0;
			isTikai = false;
			tikaiStack.pop_front();
		}
	}

	//プレイヤーとシーンオブジェクトの当たり判定
	//3Dオブジェクト更新
	player->UpdateOut(gameCamera);
	defenseTower->Update(enemiesG);
	suana->Update();
	suana2->Update();
	scene->Update();
	//touchGround->Update();
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		enemy->UpdateOut();
	}
	if (enemiesG.size() <= 0 && index >= 7) {
		sceneNo = SceneManager::SCENE_KATI;
	}
	for (std::unique_ptr<ObjectObj>& obj : objects) {
		obj->Update();
	}

	gameCamera->Update();
	gameCamera->UpdateView();
	player->object->Update();
	player->shadowObj->Update();
	tamesi->Update();
	touchObj->Update();

	//カメラのアップデート	
	subCamera->Update();

	//キャンバスにプレイヤーの情報をセット
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	//ミニマップ用オブジェクトの更新
	copyPlayer->Update(player->object, subCamera);
	//copyGround->Update(ground, subCamera);
	//copyCastle->Update(scene->GetObjectObj("castle"), subCamera);
	copyDefenseTower->Update(defenseTower->GetObjectObj(), subCamera);
	subCamera->SetTarget(player->object->GetPosition());
	subCamera->SetEye({ player->object->GetPosition().x + 1,player->object->GetPosition().y + 100, player->object->GetPosition().z });
	PostReserve();	//ミニマップの描画前処理
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
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		enemy->Draw();
	}
	scene->Draw();
	suana->Draw();
	suana2->Draw();
	defenseTower->Draw();
	player->Draw();
	//tamesi->Draw();
	for (std::unique_ptr<ObjectObj>& obj : objects) {
		obj->Draw();
	}
	touchObj->Draw();
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	canvas->Draw();
	if (poseFlag == true) {
		pose->Draw();
	}
	if (isTikai) {
		tikaiSprite->Draw();
	}
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		player->yazirusi->Draw();
	}

	Sprite::PostDraw();
}

void GameScene::PostReserve()
{
	miniMapPost->PreDrawScene(dxCommon->GetCmdList());

	//ポストエフェクトさせたいオブジェクト
	ObjectObj::PreDraw(dxCommon->GetCmdList());
	copyPlayer->Draw();
	//copyGround->Draw();
	//copyCastle->Draw();
	//copyDefenseTower->Draw();
	ObjectObj::PostDraw();

	//ポストエフェクトさせたいスプライト
	Sprite::PreDraw(dxCommon->GetCmdList());
	spriteBG->Draw();
	playerSprte->SetPosition({ player->object->GetPosition().x - gameCamera->GetEye().x ,player->object->GetPosition().z - gameCamera->GetEye().z });
	playerSprte->Draw();
	towerSprte->SetPosition({ defenseTower->object->GetPosition().x - gameCamera->GetEye().x ,defenseTower->object->GetPosition().z - gameCamera->GetEye().z });
	towerSprte->Draw();
	Sprite::PostDraw();

	miniMapPost->PosDrawScene(dxCommon->GetCmdList());
}

void GameScene::PostDraw()
{
	miniMapPost->Draw(dxCommon->GetCmdList());
}
