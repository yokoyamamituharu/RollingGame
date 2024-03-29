#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include <DirectXMath.h>
#include "Useful.h"
#include "safe_delete.h"
#include "Collision.h"
#include "CollisionAttribute.h"
#include "SphereCollider.h"
#include "SpriteManager.h"

using namespace DirectX;


BatlleScene::BatlleScene()
{
}

BatlleScene::~BatlleScene()
{
	//スプライト解放
	safe_delete(canvas);
	safe_delete(torisetu);
	for (int i = 0; i < 10; i++) {
		safe_delete(hitNum1[i]);
		safe_delete(hitNum2[i]);
	}
	safe_delete(hitSprite);
	safe_delete(mousePosS);

	//3Dオブジェクト解放
	safe_delete(tenQ);
	safe_delete(ground);
	safe_delete(player);
	safe_delete(area);
	safe_delete(particleM);
	enemies.reset();
	safe_delete(battleCamera);
}

void BatlleScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;

	battleCamera = BattleCamera::Create();

	player = Player::Create(battleCamera, 2);
	torisetu = Sprite::Create(SpriteManager::torisetu, { 0.0f,0.0f });
	battleCamera->SetPlayer(player->object);
	battleCamera->SetPlayer(player);

	int intervalWidth = 30;
	float height = 100;
	float width = 200;
	for (int i = 0; i < 10; i++) {
		hitNum2[i] = Sprite::Create(SpriteManager::num0 + i, { width + float(intervalWidth * 1),height });
		hitNum1[i] = Sprite::Create(SpriteManager::num0 + i, { width + float(intervalWidth * 2),height });
		hitNum2[i]->SetScale({ 1.6,1.6 });
		hitNum1[i]->SetScale({ 1.6,1.6 });
		hitNum2[i]->SetColor({ 0.9,0.9,0.9 });
		hitNum1[i]->SetColor({ 0.9,0.9,0.9 });
	}
	hitSprite = Sprite::Create(SpriteManager::hit, { width + float(intervalWidth * 3) ,height });
	hitSizeB = hitNum1[0]->GetSize();

	hitStopFlag = false;
	hitStopTime = 0;

	mousePosS = Sprite::Create(SpriteManager::mousepos, { 0,0 });

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

	hitNum = 0;

	//enemies = std::make_shared<EnemyZako>();
	//enemies->InitializeOut({ 0,0,0 }, false);

	//std::unique_ptr<EnemyZako> enemy1 = std::make_unique<EnemyZako>();
	//enemy1->InitializeIn();
	//enemy1->SetPlayer(player);
	//enemy1->object->SetRotation({ 0,90,0 });
	//enemy1->object->SetPosition({ 0,-4.0f,0 });
	//enemy1->object->SetCollider(new SphereCollider({ 0,0,0 }, 10.0f));
	//enemies->GetEnemies().push_back(std::move(enemy1));

	//std::unique_ptr<EnemyZako> enemy2 = std::make_unique<EnemyZako>();
	//enemy2->InitializeIn();
	//enemy2->SetPlayer(player);
	//enemy2->object->SetRotation({ 0,90,0 });
	//enemy2->object->SetPosition({ 10,-4.0f,0 });
	//enemy2->object->SetCollider(new SphereCollider({ 0,0,0 }, 10.0f));
	//enemies->GetEnemies().push_back(std::move(enemy2));

	//std::unique_ptr<EnemyZako> enemy3 = std::make_unique<EnemyZako>();
	//enemy3->InitializeIn();
	//enemy3->SetPlayer(player);
	//enemy3->object->SetRotation({ 0,90,0 });
	//enemy3->object->SetPosition({ 20,-4.0f,0 });
	//enemy3->object->SetCollider(new SphereCollider({ 0,0,0 }, 10.0f));
	//enemies->GetEnemies().push_back(std::move(enemy3));
	particleM = new Particle();
	particleM->Initialize();
}

void BatlleScene::Update(int& sceneNo, GameScene* gameScene)
{
	Update3D(sceneNo, gameScene);
	Update2D();
}

void BatlleScene::Update3D(int& sceneNo, GameScene* gameScene)
{
	//EnemyZako::isAction = -1;

	ObjectObj::SetCamera(battleCamera);
	if (SceneManager::hitEnemyToPlayer || SceneManager::WinBattle) {
		return;
	}

	//外シーンから中シーンに移行した直後の処理
	if (SceneManager::BattleInit == true) {
		for (std::unique_ptr<BaseEnemy>& enemy : enemies->GetEnemies()) {
			enemy->NotDead();
		}
		SceneManager::BattleInit = false;
	}

	//ヒットストップフラグが立っていたら更新をスキップ
	if (hitStopFlag == true) {
		if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT) == false) {
			//player->Res(true, Use::LoadXMVECTOR(vec));
		}
		else {
			player->garigariFlag = true;
		}
		return;
	}


	//敵の情報を外シーンから取得できていたら処理
	if (enemies != nullptr) {
		//死亡判定があったらエネミーを消す
		for (std::unique_ptr<BaseEnemy>& enemy : enemies->GetEnemies()) {
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
		enemies->GetEnemies().remove_if([](std::unique_ptr<BaseEnemy>& enemy) {return enemy->GetDead(); });
		for (std::unique_ptr<BaseEnemy>& enemy : enemies->GetEnemies()) {			
			if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
				if (player->attackFlag == true) {
					//enemy->SetDead();
					enemy->DamageIn(1);
					enemy->kazuFlag = true;
					XMVECTOR pos1 = XMLoadFloat3(&player->object->GetPosition());
					XMVECTOR pos2 = XMLoadFloat3(&enemy->object->GetPosition());
					XMVECTOR vec = pos1 - pos2;
					vec = XMVector3Normalize(vec);
					vec.m128_f32[1] = 0;
					if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT) == false) {
						player->Res(true, Use::LoadXMVECTOR(vec));
					}
					else {
						player->garigariFlag = true;
					}
					player->rollingSpeed = 0;
					player->attackDirection = { 0,0,0,0 };
					hitNum++;
					hitFlag = true;
					hitTime = 0;
					hitSize = 1.5f;
					hitStopFlag = true;
				}
				else if (enemy->GetAttack()) {
					player->Damage(1);
				}
			}

			Sphere* SphereA = dynamic_cast<Sphere*>(enemy->object->collider);
			if (SphereA != nullptr) {//コライダーをもっているか確認
				//プレイヤーの前に敵がいるかチェックするためのレイを用意
				Ray ray;
				ray.start = XMLoadFloat3(&XMFLOAT3(player->object->GetPosition().x, 2.0f, player->object->GetPosition().z));
				ray.dir = { 0,0,1,0 };
				//プレイヤーのY軸の回転をもとにベクトルを計算しレイの方向をそれにする
				float x = cos(player->object->GetRotation().y * 3.141592 / 180);
				float y = sin(player->object->GetRotation().y * 3.141592 / 180);
				ray.dir = { -x,0,y,0 };
				if (Collision::CheckRay2Sphere(ray, *SphereA) && player->isSphere) {
					enemy->yazirusiFlag = true;
				}
				else {
					enemy->yazirusiFlag = false;
				}
			}

			enemy->SetPlayer(player);
			enemy->UpdateIn();


			//if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT) && player->GetRes() && player->GetCrow() == false && enemy->GetDead() == false) {
			//	Sphere* SphereA = dynamic_cast<Sphere*>(enemy->object->collider);
			//	if (SphereA != nullptr) {//コライダーをもっているか確認
			//		//プレイヤーの前に敵がいるかチェックするためのレイを用意
			//		Ray ray;
			//		ray.start = XMLoadFloat3(&XMFLOAT3(player->object->GetPosition().x, 2.0f, player->object->GetPosition().z));
			//		ray.dir = { 0,0,1,0 };
			//		//プレイヤーのY軸の回転をもとにベクトルを計算しレイの方向をそれにする
			//		float x = cos(player->object->GetRotation().y * 3.141592 / 180);
			//		float y = sin(player->object->GetRotation().y * 3.141592 / 180);
			//		ray.dir = { -x,0,y,0 };
			//		if (Collision::CheckRay2Sphere(ray, *SphereA)) {
			//			//player->HitCrowAttack(enemy->object->GetPosition());
			//		}
			//	}
			//}
		}

		//キルコマンド
		if (Input::GetInstance()->TriggerKey(DIK_K)) {
			for (std::unique_ptr<BaseEnemy>& enemy : enemies->GetEnemies()) {
				//enemy->DamageIn(10000);
			}
		}


		//バトルシーンから脱出するシーン
		if (enemies->GetEnemies().size() == 0) {
			player->StopIn();
			player->breakEnemy++;	//敵の撃破数を増やす
			enemies->SetDead();
			//enemies.reset();			
			hitFlag = false;
			hitNum = 0;
			hitSize = 1.0f;
			hitStopFlag = false;
			hitStopTime = 0;
			//sceneNo = SceneManager::SCENE_GAME;
			SceneManager::WinBattle = true;
		}
		if (player->GetHp() <= 0) {
			player->StopIn();
			player->Cure(5);
			sceneNo = SceneManager::SCENE_END;
		}
	}



	particleM->Update();
	ground->Update();
	tenQ->Update();
	area->Update();
	player->MoveIn();
	player->Res();
	//プレイヤーをエリア内に収める
	if (Collision::CheckDistance(player->object->GetPosition() + player->move, { 0,0,0 }) > 125.0f) {
		player->move = { 0,0,0 };
	}
	player->UpdateIn();

	battleCamera->Update();

	battleCamera->UpdateView();

	player->object->Update();
	player->shadowObj->Update();

	const int maxEnemy = 7;
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());
}

void BatlleScene::Update2D()
{
	mousePosS->SetPosition(InputMouse::GetInstance()->GetWindowPos());



	//ヒット数表示
	if (hitFlag == true) {
		hitTime++;
		if (hitTime >= hitWaitTime) {
			hitFlag = false;
			hitNum = 0;
		}
		for (int i = 0; i < 10; i++) {
			hitNum1[i]->SetSize(hitSizeB * hitSize);
			hitNum2[i]->SetSize(hitSizeB * hitSize);
		}
		if (hitSize > 1.0f) {
			hitSize -= 0.1f;
		}
	}

	//ヒットストップ
	if (hitStopFlag == true) {
		hitStopTime++;
		if (hitStopTime >= 10) {
			hitStopFlag = false;
			hitStopTime = 0;
		}

		XMFLOAT3 oldEye = battleCamera->GetEye();
		XMFLOAT3 oldTarget = battleCamera->GetTarget();
		battleCamera->SetEye(battleCamera->GetEye() + float(rand() % 2 - 1));
		battleCamera->SetTarget(battleCamera->GetTarget() + float(rand() % 2 - 1));

		battleCamera->UpdateView();

		player->object->Update();
		player->shadowObj->Update();
		ground->Update();
		tenQ->Update();
		area->Update();
		for (std::unique_ptr<BaseEnemy>& enemy : enemies->GetEnemies()) {
			enemy->object->Update();
		}
		battleCamera->SetEye(oldEye);
		battleCamera->SetTarget(oldTarget);
	}
}

void BatlleScene::Draw()
{

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	Sprite::PostDraw();


	ObjectObj::PreDraw(dxCommon->GetCmdList());
	tenQ->Draw();
	area->Draw();

	player->Draw();
	ground->Draw();
	if (enemies != 0) {
		for (std::unique_ptr<BaseEnemy>& enemy : enemies->GetEnemies()) {
			enemy->Draw();
		}
	}
	particleM->Draw();
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	canvas->Draw();
	if (hitFlag) {
		hitSprite->Draw();
		int a = hitNum / 10;
		int b = hitNum - a;
		hitNum1[b]->Draw();
		hitNum2[a]->Draw();
	}
	//mousePosS->Draw();
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		player->arrowSymbolSprite->Draw();
	}
	Sprite::PostDraw();
}