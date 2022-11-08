#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include "StrongZakoEnemy.h"
#include "/K020G1196/RollingGame_ryuunen/tuyoEnemy.h"

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
	//3Dオブジェクト解放
	delete fbxobject;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;

	//カメラの初期位置、注視点
	camera->SetTarget(initTarget);
	camera->SetEye(initEye);

	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/texture.png");
	Sprite::LoadTexture(1, L"Resources/torisetu.png");
	Sprite::LoadTexture(2, L"Resources/clear.png");

	//スプライトの生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	clearsprite = Sprite::Create(2, { 100.0f,100.0f });


	//弾の静的初期化
	Bullet::StaticInitialize();

	EnemyZako::EnemyCreateModel();

	StrongZakoEnemy::CreateStrongEnemyModel();

	//モデルの読み込み
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	groundmodel = Model::Create("ground");
	enemymodel = Model::Create("enemy");
	castleModel = Model::Create("castle");
	suanaModel = Model::Create("suana");
	kabeModel = Model::Create("kabe");
	tenqModel = Model::Create("tenQ");

	//3Dオブジェクトの生成
	fbxobject = new FBXObject;
	fbxobject->Initialize();
	fbxobject->SetModel(fbxmodel);
	fbxobject->PlayAnimetion();
	fbxobject->SetPos({ 0,0,+80 });

	ground = OBJobject::Create();
	ground->SetModel(groundmodel);
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	ground = OBJobject::Create();
	ground->SetModel(groundmodel);
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	castle = OBJobject::Create();
	castle->SetModel(castleModel);
	castle->SetScale({ 10.0f,10.0f,10.0f });

	suana = OBJobject::Create();
	suana->SetModel(suanaModel);
	suana->SetPosition({ 100.0f,0.0f,100.0f });
	suana->SetScale({ 10.0f,10.0f,10.0f });
	suana->SetRotation({ 0,90,0 });

	suana2 = OBJobject::Create();
	suana2->SetModel(suanaModel);
	suana2->SetPosition({ -100.0f,0.0f,-100.0f });
	suana2->SetScale({ 10.0f,10.0f,10.0f });
	suana2->SetRotation({ 0,-90,0 });

	kabe = OBJobject::Create();
	kabe->SetModel(kabeModel);
	kabe->SetPosition({ 70.0f,-5.0f,30.0f });
	kabe->SetScale({ 5.0f,5.0f,5.0f });
	kabe->SetRotation({ 0,0,0 });

	kabe2 = OBJobject::Create();
	kabe2->SetModel(kabeModel);
	kabe2->SetPosition({ -60.0f,-5.0f,-50.0f });
	kabe2->SetScale({ 5.0f,5.0f,5.0f });
	kabe2->SetRotation({ 0,180,0 });

	tenQ = OBJobject::Create();
	tenQ->SetModel(tenqModel);
	tenQ->SetScale({ 5,5,5 });

	//プレイヤーの生成処理
	player = new Player();
	player->Initialize(input, mouse, camera);
	player->object->SetScale({ 1.0f,1.0f,1.0f });
	player->object->SetPosition({ 0.0f,-6.0f,-50.0f });
	player->object->SetRotation({ 0.0f,90.0f,0.0f });

	//敵の生成処理
	std::unique_ptr<EnemyZako> enemy1 = std::make_unique<EnemyZako>();
	enemy1->Initialize(EnemyZako::FIELD_OUT, camera, { -50, EnemyZako::groundOutPos,0 });
	enemy1->SetPlayer(player);
	enemy1->object->SetRotation({ 0,90,0 });
	enemiesG.push_back(std::move(enemy1));

	//タワーの生成処理
	defenseTower = DefenseTower::Create();
	defenseTower->GetOBJObject()->SetPosition({ 20,0,20 });
	bullet = Bullet::Create();

}

void GameScene::Update(int& sceneNo, BatlleScene* batlleScene)
{
	//player->Update();
	//カメラ操作
	//if (input->PushKey(DIK_RIGHT)) {
	//	camera->matRot *= XMMatrixRotationY(0.1f);
	//}
	//else if (input->PushKey(DIK_LEFT)) {
	//	camera->matRot *= XMMatrixRotationY(-0.1f);
	//}

	if (Input::GetInstance()->PushKey(DIK_H)) {
		int num = 0;
	}
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		cameraToMouse *= -1;
	}

	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		sceneNo = 2;
	}

	//敵を行動させるかさせないかのトルグスイッチ
	EnemyZako::Action();

	//巣穴１から敵を生成
	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
		newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, suana->GetPosition(), true, suana->GetPosition() + XMFLOAT3{ 0, 0, -50 });
		enemiesG.push_back(std::move(newEnemy));
	}

	//巣穴２から敵を生成
	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
		newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, suana2->GetPosition(), true, suana2->GetPosition() + XMFLOAT3{ 0, 0, 50 });
		enemiesG.push_back(std::move(newEnemy));
	}

	if (Input::GetInstance()->TriggerKey(DIK_3)) {
		std::shared_ptr<StrongZakoEnemy> newEnemy = std::make_shared<StrongZakoEnemy>();
		newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, suana2->GetPosition(), true, suana2->GetPosition() + XMFLOAT3{ 0, 0, 50 });
		enemiesG.push_back(std::move(newEnemy));
	}


	//std::list<std::unique_ptr<EnemyZako>>enemies1 = std::move(enemy1->GetEnemies());

	//敵とプレイヤーの当たり判定
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (CubeCollision(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			//バトルシーンに行く処理
			batlleScene->SetEnemies(enemy);
			enemiesG.remove(enemy);
			sceneNo = SceneManager::SCENE_BATTLE;
			break;
		}
	}



	//3Dオブジェクト更新
	fbxobject->Update();
	player->Update();
	ground->Update();

	bullet->Update();
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Update();
	}
	defenseTower->Update(enemiesG);

	castle->Update();

	suana->Update();
	suana2->Update();


	kabe->Update();
	kabe2->Update();
	tenQ->Update();

	if (cameraToMouse == 1) {
		camera->matRot *= XMMatrixRotationY(0.8f * mouse->MoveMouseVector('x') / 1000);
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
}

void GameScene::Draw()
{
	OBJobject::PreDraw(dxCommon->GetCmdList());
	//fbxobject->Draw(dxCommon->GetCmdList());
	player->object->Draw();
	ground->Draw();
	defenseTower->Draw();
	bullet->Draw();

	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Draw();
	}
	castle->Draw();
	suana->Draw();
	suana2->Draw();
	kabe->Draw();
	kabe2->Draw();
	tenQ->Draw();

	OBJobject::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();	
	Sprite::PostDraw();

}
