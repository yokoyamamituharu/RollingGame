#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include "StrongZakoEnemy.h"
#include "tuyoEnemy.h"
#include "safe_delete.h"
#include "ModelManager.h"

DirectX::XMFLOAT3 initTarget = { 0,-10,20 };
DirectX::XMFLOAT3 initEye = { 0,20,-25 };
using namespace DirectX;

//立方体の当たり判定
bool CubeCollision(XMFLOAT3 object1, XMFLOAT3 radius1, XMFLOAT3 object2, XMFLOAT3 radius2) {
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
	//safe_delete(objectFBX);
	//safe_delete(fbxmodel);
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
}

void GameScene::Initialize(DirectXCommon* dxCommon, Camera* camera)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	this->camera = camera;

	//カメラの初期位置、注視点
	camera->SetTarget(initTarget);
	camera->SetEye(initEye);

	//スプライトの生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	clearsprite = Sprite::Create(2, { 100.0f,100.0f });


	//モデルの読み込み
	//fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("cube");


	//3Dオブジェクトの生成
	//objectFBX = new ObjectFBX;
	//objectFBX->Initialize();
	//objectFBX->SetModel(fbxmodel);
	//objectFBX->PlayAnimetion();
	//objectFBX->SetPos({ 0,0,+80 });

	ground = ObjectObj::Create();
	ground->SetModel(ModelManager::GetModel("ground"));
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	castle = ObjectObj::Create();
	castle->SetModel(ModelManager::GetModel("castle"));
	castle->SetScale({ 10.0f,10.0f,10.0f });

	suana = ObjectObj::Create();
	suana->SetModel(ModelManager::GetModel("suana"));
	suana->SetPosition({ 100.0f,0.0f,100.0f });
	suana->SetScale({ 10.0f,10.0f,10.0f });
	suana->SetRotation({ 0,90,0 });

	suana2 = ObjectObj::Create();
	suana2->SetModel(ModelManager::GetModel("suana"));
	suana2->SetPosition({ -100.0f,0.0f,-100.0f });
	suana2->SetScale({ 10.0f,10.0f,10.0f });
	suana2->SetRotation({ 0,-90,0 });

	kabe = ObjectObj::Create();
	kabe->SetModel(ModelManager::GetModel("kabe"));
	kabe->SetPosition({ 70.0f,-5.0f,50.0f });
	kabe->SetScale({ 5.0f,5.0f,5.0f });
	kabe->SetRotation({ 0,0,0 });

	kabe2 = ObjectObj::Create();
	kabe2->SetModel(ModelManager::GetModel("kabe"));
	kabe2->SetPosition({ -60.0f,-5.0f,-50.0f });
	kabe2->SetScale({ 5.0f,5.0f,5.0f });
	kabe2->SetRotation({ 0,180,0 });

	tenQ = ObjectObj::Create();
	tenQ->SetModel(ModelManager::GetModel("tenQ"));
	tenQ->SetScale({ 5,5,5 });

	//プレイヤーの生成処理
	player = new Player();
	player->Initialize(camera);
	player->object->SetScale({ 1.0f,1.0f,1.0f });
	player->object->SetPosition({ 0.0f,-6.0f,-50.0f });
	player->object->SetRotation({ 0.0f,90.0f,0.0f });

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

	postEffect = new PostEffect();
	postEffect->Initialize();

	canvas = new Canvas();
	canvas->Initialize();

	index = 0;
	timer[0] = 0;
	timer[1] = 160;
	timer[2] = 100;
	timer[3] = 100;
	timer[4] = 210;
	timer[5] = 200;

	dasuteki[0] = 2;
	dasuteki[1] = 1;
	dasuteki[2] = 1;
	dasuteki[3] = 1;
	dasuteki[4] = 2;
	dasuteki[5] = 1;

	Player::breakEnemy = 0;

	subCamera = new Camera(1280, 700);
	subCamera->SetEye({ 0, 100, -1 });
	subCamera->SetTarget({ 0, 0, 0 });
	//ObjectObj::SetCamera(subCamera);
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
}

void GameScene::Update(int& sceneNo, BatlleScene* batlleScene)
{
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (enemy->GetHp() <= 0) {
			Player::breakEnemy += 1;
		}
	}
	enemiesG.remove_if([](std::shared_ptr<EnemyZako>& enemy) {return enemy->GetDead(); });
	//player->Update();
	//カメラ操作
	//if (input->PushKey(DIK_RIGHT)) {
	//	subCamera->matRot *= XMMatrixRotationY(0.1f);
	//}
	//else if (input->PushKey(DIK_LEFT)) {
	//	subCamera->matRot *= XMMatrixRotationY(-0.1f);
	//}	

	////カメラ操作
	//if (input->TriggerKey(DIK_UP)) {
	//	subCamera->MoveEyeVector(XMFLOAT3{ 0,0,1 });
	//}
	//else if (input->TriggerKey(DIK_DOWN)) {
	//	subCamera->MoveEyeVector(XMFLOAT3{ 0,0,-1 });
	//}


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
	timer[index]--;

	if (timer[index] <= 0) {
		if (dasuteki[index] == 1) {
			//タワーがある方
			std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
			newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, XMFLOAT3{ 0, 0, -100 }, XMFLOAT3{ -100,0,0 });
			enemiesG.push_back(std::move(newEnemy));
		}
		if (dasuteki[index] == 2) {
			std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
			newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana2->GetPosition().x,EnemyZako::groundOutPos,suana2->GetPosition().z }, true, XMFLOAT3{ 0, 0, +100 }, XMFLOAT3{ +100,0,0 });
			enemiesG.push_back(std::move(newEnemy));
		}

		index++;
	}

	//if (Input::GetInstance()->PushKey(DIK_H)) {
	//	int num = 0;
	//}
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		cameraToMouse *= -1;
	}

	//if (Input::GetInstance()->TriggerKey(DIK_0)) {
	//	sceneNo = 2;
	//}

	//敵を行動させるかさせないかのトルグスイッチ
	//EnemyZako::Action();

	////巣穴１から敵を生成
	//if (Input::GetInstance()->TriggerKey(DIK_1)) {
	//	std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
	//	newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana->GetPosition() + XMFLOAT3{ 0, 0, -50 });
	//	enemiesG.push_back(std::move(newEnemy));
	//}

	////巣穴２から敵を生成
	//if (Input::GetInstance()->TriggerKey(DIK_2)) {
	//	std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
	//	newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana2->GetPosition() + XMFLOAT3{ 0, 0, 50 });
	//	enemiesG.push_back(std::move(newEnemy));
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_3)) {
	//	std::shared_ptr<StrongZakoEnemy> newEnemy = std::make_shared<StrongZakoEnemy>();
	//	newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana2->GetPosition() + XMFLOAT3{ 0, 0, 50 });
	//	enemiesG.push_back(std::move(newEnemy));
	//}


	//std::list<std::unique_ptr<EnemyZako>>enemies1 = std::move(enemy1->GetEnemies());

	//敵とプレイヤーの当たり判定
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (CubeCollision(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			if (enemy->GetDead() == false) {
				//バトルシーンに行く処理
				batlleScene->SetEnemies(enemy);
				enemiesG.remove(enemy);
				player->outPos = player->object->GetPosition();
				player->Stop();
				//プレイヤーを原点に発生させる
				player->object->SetPosition({ 0,-6,0 });
				sceneNo = SceneManager::SCENE_BATTLE;
				break;
			}
		}

		//敵と城の当たり判定
		if (CubeCollision(enemy->object->GetPosition(), { 2.5,5,1 }, castle->GetPosition(), { 10,10,10 })) {
			//当たったら負け
			//sceneNo = SceneManager::SCENE_END;
		}
	}
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	//if (Input::GetInstance()->TriggerKey(DIK_PGUP)) {
	//	player->Cure(1);
	//}
	//if (Input::GetInstance()->TriggerKey(DIK_PGDN)) {
	//	player->Damage(1);
	//}

	//3Dオブジェクト更新
	//objectFBX->Update();
	player->Update();
	ground->Update();

	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		//if (enemy->GetDead() == false) {
		enemy->Update();
		//	}
	}
	//defenseTower->Update(enemiesG);
	castle->Update();
	suana->Update();
	suana2->Update();
	kabe->Update();
	kabe2->Update();
	tenQ->Update();
	if (enemiesG.size() <= 0 && index >= 5) {
		sceneNo = SceneManager::SCENE_KATI;
	}

	//カメラ操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		camera->matRot *= XMMatrixRotationY(0.02f);
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		camera->matRot *= XMMatrixRotationY(-0.02f);
	}
	if (cameraToMouse == 1) {
		camera->matRot *= XMMatrixRotationY(0.8f * InputMouse::GetInstance()->MoveMouseVector('x') / 1000);
	}
	//XMFLOAT3 rote = player->object->GetRotation();
	XMFLOAT3 pos = player->object->GetPosition();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, camera->matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	//matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	camera->SetEye({ player->object->GetPosition().x + movement.m128_f32[0] * 80, player->object->GetPosition().y + movement.m128_f32[1] * 80,
		 player->object->GetPosition().z + movement.m128_f32[2] * 80 });
	camera->eye.y = 20;
	camera->target = player->object->GetPosition();
	//プレイヤーがジャンプした時視点だけ上に向くのを防止するための処理敵
	//camera->target.y = player->graundheight;



	//カメラの更新
	//camera->SetEye({ player->object->GetPosition().x + initEye.x,player->object->GetPosition().y + initEye.y,
	//	player->object->GetPosition().z + initEye.z });
	//camera->SetTarget({ player->object->GetPosition().x + initTarget.x,player->object->GetPosition().y + initTarget.y,
	//	player->object->GetPosition().z + initTarget.z });
	camera->SetWorldMatrix(player->object->GetWorldMatrix());
	camera->Update();
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
	ground->Draw();
	defenseTower->Draw();
	castle->Draw();
	suana->Draw();
	suana2->Draw();
	kabe->Draw();
	kabe2->Draw();
	tenQ->Draw();
	player->Draw();

	ObjectObj::PostDraw();

	//ObjectObj::PreDraw(dxCommon->GetCmdList());
	//player->object->VecSetPosition({ 0,20,0 });
	//player->object->Update();
	//player->Draw();	
	//ObjectObj::PostDraw();
	//player->object->VecSetPosition({ 0,-20,0 });
	//player->object->Update();

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
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		//enemy->Draw();
	}
	//ground->Draw();
	//defenseTower->Draw();
	//castle->Draw();
	//suana->Draw();
	//suana2->Draw();
	//kabe->Draw();
	//kabe2->Draw();
	//tenQ->Draw();
	//player->object->Draw();
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
