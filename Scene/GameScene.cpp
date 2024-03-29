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
	safe_delete(pose);
	safe_delete(tikaiSprite);
	safe_delete(playerSprte);
	safe_delete(towerSprte);
	safe_delete(miniMapPost);
	//3Dオブジェクト解放
	safe_delete(particleM);
	safe_delete(scene);
	safe_delete(defenseTower);
	safe_delete(player);
	safe_delete(copyGround);
	safe_delete(copyCastle);
	safe_delete(copyDefenseTower);
	safe_delete(copyPlayer);
	enemiesG.clear();
	dasu.clear();
	safe_delete(gameCamera);
	safe_delete(subCamera);
	safe_delete(breakCastle);
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
	spriteBG = Sprite::Create(SpriteManager::torisetu, { 0.0f,0.0f });
	pose = Sprite::Create(SpriteManager::pose, { 0,0 });
	//キャンバスの生成処理
	canvas = new Canvas();
	canvas->Initialize();
	tikaiSprite = Sprite::Create(SpriteManager::tikai, { 300,-50 });


	collisionManager = CollisionManager::GetInstance();

	//3Dオブジェクトの生成
	//kabe1 = TouchableObject::Create(ModelManager::GetModel("kabe"));
	//kabe2 = TouchableObject::Create(ModelManager::GetModel("kabe"));
	//kabe3 = TouchableObject::Create(ModelManager::GetModel("kabe"));
	//kabe4 = TouchableObject::Create(ModelManager::GetModel("kabe"));

	//kabe1->SetPosition({ 100,0,-100 });
	//kabe2->SetPosition({ -1000,0,-100 });
	//kabe3->SetPosition({ -100,0,-1010 });
	//kabe3->SetRotation({ 0,90,0 });
	//kabe4->SetPosition({ -100,0,00 });
	//kabe4->SetRotation({ 0,90,0 });
	//タワーの生成処理
	defenseTower = DefenseTower::Create();
	defenseTower->GetObjectObj()->SetPosition({ 450,0,-350 });
	defenseTower->GetObjectObj()->SetScale({ 15,15,15 });

	castle = ObjectObj::Create(ModelManager::GetModel("castle"));
	castle->SetPosition({ 0,5,0 });
	castle->SetScale({ 5,5,5 });
	breakCastle = ObjectObj::Create(ModelManager::GetModel("tower1"));
	breakCastle->SetPosition({ 0,5,0 });
	breakCastle->SetScale({ 5,5,5 });
	breaktenQ = ObjectObj::Create(ModelManager::GetModel("tenQ"));
	breaktenQ->SetScale({ 10,10,10 });
	breakGround = ObjectObj::Create(ModelManager::GetModel("ground"));
	breakGround->SetScale({ 10,10,10 });


	//プレイヤーの生成処理
	player = Player::Create(gameCamera, 1);
	player->SetHp(5);
	player->object->SetPosition({ 0,Player::groundHeight,0 });
	//敵の撃破数のリセット
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
	scene->Initialize("level", &towers);
	playerSprte = Sprite::Create(SpriteManager::sprite_0001, { 0,0 });
	towerSprte = Sprite::Create(SpriteManager::sprite_0002, { 0,0 });

	Route::GetIns()->Set();

	particleM = new Particle;
	particleM->Initialize();

}

void GameScene::Update(int& sceneNo, BatlleScene* batlleScene)
{
	ObjectObj::SetCamera(gameCamera);
	if (SceneManager::hitEnemyToPlayer || SceneManager::WinBattle) {
		return;
	}
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		poseFlag = !poseFlag;
	}
	if (poseFlag == true) return;
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		if (enemy->GetHp() <= 0) {
			Player::breakEnemy += 1;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_B)) {
		for (int i = 0; i < 15; i++) {
			std::unique_ptr<ParticleTubu> particle = std::make_unique<ParticleTubu>();
			particle->obj = std::make_unique<ObjectObj>();
			particle->obj->Initialize(ModelManager::GetModel("particle"));
			particle->obj->SetScale({ 5,5,5 });
			particle->end_frame = rand() % 5 + 30;
			particle->position = { 0,0,0 };
			particle->position.z -= 30;
			//tubu->scale = { 10,10,10 };
			//const float rnd_vel = 0.1f;
			int rndVel = 3.0f;
			particle->velocity.x = rand() % (rndVel * 2) - rndVel;
			particle->velocity.y = rand() % (rndVel * 2) - rndVel;
			particle->velocity.z = rand() % (rndVel * 2) - rndVel;
			particleM->Add(std::move(particle));
		}
		player->object->SetPosition({ 0, 0, 0 });
		isEvent = true;
		gameCamera->SetKeisu(100);
		gameCamera->SetFlag(true);
		castle->Update();
		breakCastle->Update();
		breaktenQ->Update();
		breakGround->Update();
	}
	if (isEvent) {
		breakTimer++;
		player->object->SetPosition({ 0, 0, 0 });
	}
	if (breakTimer > 50) {
		breaktenQ->Update();
		breakGround->Update();
	}


	//死亡フラグが立っている敵を消す、その際に死亡時演出のパーティクルを発生させる
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		if (enemy->GetDead()) {
			for (int i = 0; i < 5; i++) {
				std::unique_ptr<ParticleTubu> particle = std::make_unique<ParticleTubu>();
				particle->obj = std::make_unique<ObjectObj>();
				particle->obj->Initialize(ModelManager::GetModel("particle"));
				particle->obj->SetScale({ 5,5,5 });
				particle->end_frame = rand() % 5 + 30;
				particle->position = enemy->object->GetPosition();
				//tubu->scale = { 10,10,10 };
				//const float rnd_vel = 0.1f;
				int rndVel = 3.0f;
				particle->velocity.x = rand() % (rndVel * 2) - rndVel;
				particle->velocity.y = rand() % (rndVel * 2) - rndVel;
				particle->velocity.z = rand() % (rndVel * 2) - rndVel;
				particleM->Add(std::move(particle));
			}
		}
	}
	enemiesG.remove_if([](std::shared_ptr<BaseEnemy>& enemy) {return enemy->GetDead(); });

	//土煙
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
		particleM->Add(std::move(particle));
	}

	//敵生成処理
	SpownEnemy();

	//敵の当たり判定
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		//死亡しているエネミーだった場合スキップ
		if (enemy->GetDead() == true) {
			continue;
		}
		//敵とプレイヤー
		if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			//バトルシーンに行く処理
			batlleScene->SetEnemies(enemy);
			//enemiesG.remove(enemy);
			player->outPos = player->object->GetPosition();
			player->StopOut();
			//プレイヤーを原点に発生させる
			//player->object->SetPosition({ 0,-6,0 });
			SceneManager::hitEnemyToPlayer = true;
			SceneManager::BattleInit = true;
			//sceneNo = SceneManager::SCENE_BATTLE;
			break;
		}
		//敵と城が近いかどうか
		if (enemy->tikai == false) {
			if (Collision::CheckDistance(scene->GetObjectObj("castle")->GetPosition(), enemy->object->GetPosition()) < 200) {
				enemy->tikai = true;
				isTikai = true;
				tikaiStack.push_back(true);
			}
		}
		//敵と城
		if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, scene->GetObjectObj("castle")->GetPosition(), { 20,20,20 })) {
			//当たったら負け
			//sceneNo = SceneManager::SCENE_END;
		}
	}

	//敵と城が近い場合に警告文を出す
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

	//3Dオブジェクト更新
	player->UpdateOut(gameCamera);
	defenseTower->SetPlayer(player);
	defenseTower->Update(enemiesG);
	scene->Update();
	//touchGround->Update();
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		enemy->UpdateOut();
		if (enemy->GetDead()) {
			for (int i = 0; i < 5; i++) {
				std::unique_ptr<ParticleTubu> particle = std::make_unique<ParticleTubu>();
				particle->obj = std::make_unique<ObjectObj>();
				particle->obj->Initialize(ModelManager::GetModel("particle"));
				particle->obj->SetScale({ 5,5,5 });
				particle->end_frame = rand() % 5 + 30;
				particle->position = enemy->object->GetPosition();
				//tubu->scale = { 10,10,10 };
				//const float rnd_vel = 0.1f;
				int rndVel = 3.0f;
				particle->velocity.x = rand() % (rndVel * 2) - rndVel;
				particle->velocity.y = rand() % (rndVel * 2) - rndVel;
				particle->velocity.z = rand() % (rndVel * 2) - rndVel;
				particleM->Add(std::move(particle));
			}
		}
	}
	if (enemiesG.size() <= 0 && index >= 7) {
		sceneNo = SceneManager::SCENE_KATI;
	}

	for (std::shared_ptr<DefenseTower>& tower : towers) {
		tower->SetPlayer(player);
		tower->Update(enemiesG);
	}



	gameCamera->Update();
	gameCamera->UpdateView();
	player->object->Update();
	player->shadowObj->Update();
	particleM->Update();

	//if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
	//	kabe1->VecSetPosition({ 10,0,0 });
	//}
	//else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
	//	kabe1->VecSetPosition({ -10,0,0 });
	//}

	//kabe1->Update();
	//kabe2->Update();
	//kabe3->Update();
	//kabe4->Update();

	//カメラのアップデート	
	subCamera->Update();

	//キャンバスにプレイヤーの情報をセット
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());
}

void GameScene::Draw()
{
	if (Input::GetInstance()->TriggerKey(DIK_M)) {
		//mapFlag = !mapFlag;
	}
	if (mapFlag == true) {
		PostDraw();	//ミニマップの描画
	}

	ObjectObj::PreDraw(dxCommon->GetCmdList());
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		enemy->Draw();
	}
	for (std::shared_ptr<DefenseTower>& tower : towers) {
		tower->Draw();
	}
	if (isEvent == false) {
		scene->Draw();
	}
	else {
		if (breakTimer < 50) {
			castle->Draw();
		}
		else {
			breakCastle->Draw();
		}
		breaktenQ->Draw();
		breakGround->Draw();
	}
	defenseTower->Draw();
	player->Draw();
	//kabe1->Draw();
	//kabe2->Draw();
	//kabe3->Draw();
	//kabe4->Draw();
	particleM->Draw();
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	if (isEvent == false) {
		//spriteBG->Draw();
		canvas->Draw();
		if (poseFlag == true) {
			pose->Draw();
		}
		if (isTikai) {
			tikaiSprite->Draw();
		}
		if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
			player->arrowSymbolSprite->Draw();
		}
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
	//towerSprte->SetPosition({ defenseTower->object->GetPosition().x - gameCamera->GetEye().x ,defenseTower->object->GetPosition().z - gameCamera->GetEye().z });
	towerSprte->Draw();
	Sprite::PostDraw();

	miniMapPost->PosDrawScene(dxCommon->GetCmdList());
}

void GameScene::PostDraw()
{
	miniMapPost->Draw(dxCommon->GetCmdList());
}

void GameScene::SpownEnemy()
{
	if (index < maxEnemy) {
		dasu[index].timer--;
		while (dasu[index].timer <= 0) {
			if (dasu[index].hole == HOLE1) {
				//タワーがある方
				std::shared_ptr<YowaiEnemy> newEnemy = YowaiEnemy::Create(true, Route::GetRoute(3));
				enemiesG.push_back(std::move(newEnemy));
			}
			if (dasu[index].hole == HOLE2) {
				std::shared_ptr<EnemyZako> newEnemy = EnemyZako::Create(true, Route::GetRoute(4));
				enemiesG.push_back(std::move(newEnemy));
			}
			index++;
			if (index >= maxEnemy) {
				break;
			}
		}
	}
}

void GameScene::CreaEffect()
{
}

void GameScene::GameEndEffect()
{
}
