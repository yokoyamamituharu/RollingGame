#include "BaseEnemy.h"
using namespace DirectX;
#include "Useful.h"
#include "ModelManager.h"
#include "safe_delete.h"
#include "Collision.h"
#include "Particle.h"
#include "SphereCollider.h"
#include "YowaiEnemy.h"

/// 静的メンバ変数の実体
bool BaseEnemy::isAction = 1;
const float BaseEnemy::groundPosOut = 6.0f;
const float BaseEnemy::groundPosIn = -4.0f;

void BaseEnemy::ParticleCreate()
{
	for (int i = 0; i < 5; i++) {
		std::unique_ptr<ParticleTubu> tubu = std::make_unique<ParticleTubu>();
		tubu->obj = std::make_unique<ObjectObj>();
		tubu->obj->Initialize(ModelManager::GetModel("particle"));
		tubu->obj->SetScale({ 5,5,5 });
		tubu->end_frame = rand() % 5 + 30;
		tubu->position = object->GetPosition();
		//tubu->scale = { 10,10,10 };
		//const float rnd_vel = 0.1f;
		int rndVel = 3.0f;
		tubu->velocity.x = rand() % (rndVel * 2) - rndVel;
		tubu->velocity.y = rand() % (rndVel * 2) - rndVel;
		tubu->velocity.z = rand() % (rndVel * 2) - rndVel;
		//Particle::GetIns()->Add(std::move(tubu));
	}
}

BaseEnemy::BaseEnemy()
{
	modelName = "enemy";
}

BaseEnemy::~BaseEnemy()
{
	//ParticleCreate();
	safe_delete(object);
	safe_delete(shadowObj);
	safe_delete(yazirusi);
	enemies.clear();
}

std::shared_ptr<BaseEnemy> BaseEnemy::Create(bool isTarget, XMFLOAT2 route[])
{
	std::shared_ptr<BaseEnemy> enemy = std::make_shared<BaseEnemy>();
	enemy->InitializeOut(isTarget, route);
	enemy->CreateEnemy();
	return enemy;
}

void BaseEnemy::DamageIn(int damage)
{
	inhp -= damage;
}

void BaseEnemy::DamageOut(int damage)
{
	outhp -= damage;
	int deleteNum = 0;
	for (std::unique_ptr<BaseEnemy>& enemy : enemies) {
		enemy->SetDead();
		deleteNum++;
		if (deleteNum >= damage) {
			return;
		}
	}
}

void BaseEnemy::InitializeOut(bool isTarget, XMFLOAT2 route[])
{
	this->isFiled = FIELD_OUT;
	this->isTarget = isTarget;
	targetIndex = 1;
	if (isTarget) {
		for (int i = 0; i < 3; i++) {
			this->route[i] = route[i];
		}
	}

	//初期処理
	InitGeneralSetUp();

	//敵を生成
	//CreateEnemy();

	////HPを計算
	//outmaxHp = enemies.size();
	//outhp = outmaxHp;
}

void BaseEnemy::InitGeneralSetUp()
{
	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel(modelName));
	object->SetPosition({ route[0].x, groundPosOut,route[0].y });
	object->SetScale({ 4.0f,4.0f, 4.0f });

	//敵の影を作成
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//目的地が設定されていたら
	if (isTarget == true) {
		//移動する方向を計算する
		XMVECTOR pos1 = XMLoadFloat3(&object->GetPosition());
		targetVec = XMLoadFloat3(&XMFLOAT3{ route[1].x, groundPosOut,route[1].y }) - pos1;
		targetVec = XMVector3Normalize(targetVec);
		targetVec.m128_f32[1] = 0;//Y軸の移動を0にする
	}
}

void BaseEnemy::CreateEnemy()
{
	//この敵が中シーンに移動した時に持っている小敵の情報を追加
	int maxEnemyNum = rand() % 2 + 5;
	for (int i = 0; i < maxEnemyNum; i++)
	{
		if (rand() % 10 > 4) {
			//敵をリストに追加していく
			std::unique_ptr<BaseEnemy> newBaseEnemy = std::make_unique<BaseEnemy>();
			newBaseEnemy->InitializeIn();
			//リストに登録
			enemies.push_back(std::move(newBaseEnemy));
		}
		else {
			//敵をリストに追加していく
			std::unique_ptr<YowaiEnemy> newBaseEnemy = std::make_unique<YowaiEnemy>();
			newBaseEnemy->InitializeIn();
			//リストに登録
			//enemie s.push_back(std::move(newBaseEnemy));
		}
	}
	//HPを計算
	outmaxHp = enemies.size();
	outhp = outmaxHp;
}

void BaseEnemy::InitializeIn()
{
	this->isFiled = FIELD_IN;

	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("enemy"));
	//影を作成
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });
	//攻撃進行に居たら表示する矢印
	yazirusi = ObjectObj::Create(ModelManager::GetModel("yazirusi"));
	yazirusi->SetScale({ 3,3,3 });

	//座標のセット
	float x = rand() % 200 - 100;
	float z = rand() % 200 - 100;
	object->SetPosition({ x,groundPosIn,z });
	//サイズのセット
	object->SetScale({ 4.0f,4.0f, 4.0f });
	//コライダーのセット
	object->SetCollider(new SphereCollider({ 0,0,0 }, 10.0f));
	object->collider->SetAttribute(COLLISION_ATTR_ALLIES);

	inhp = 5;
}

void BaseEnemy::UpdateOut()
{
	scale = outhp / outmaxHp;
	if (scale <= 0) {
		scale = 1;
		object->SetRotation({ 180,90,0 });
	}
	object->SetScale(maxScale * scale);

	if (isFiled == FIELD_IN) {
		return;
	}

	if (isAction > 0 && GetDead() == false) {
		if (isTarget == true) {
			float speed = 0.15;
			//目的地に向かって直進	

			XMFLOAT3 pos = object->GetPosition() + targetVec * 0.5;
			object->SetPosition(pos);
			//目的地を超えていたら
			if (Collision::CheckExceed(
				{ route[targetIndex - 1].x,groundPosOut,  route[targetIndex - 1].y },
				object->GetPosition(),
				{ route[targetIndex].x,groundPosOut,  route[targetIndex].y })) {
				targetIndex++;
				if (targetIndex >= 3) {
					targetIndex = 2;
				}
				//移動する方向を計算する
				XMVECTOR pos1 = XMLoadFloat3(&object->GetPosition());
				targetVec = XMLoadFloat3(&XMFLOAT3{ route[targetIndex].x, groundPosOut,route[targetIndex].y }) - pos1;
				targetVec = XMVector3Normalize(targetVec);
				targetVec.m128_f32[1] = 0;//ここを0にしないとプレイヤーと敵のY座標のずれで敵の突進方向がずれる
			}
		}
		for (std::unique_ptr<BaseEnemy>& enemy : enemies) {
			//enemy->SetDead();
		}
		enemies.remove_if([](std::unique_ptr<BaseEnemy>& enemy) {return enemy->GetDead(); });
		if (outhp <= 0) {
			SetDead();
		}
	}


	//オブジェクトの更新
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(groundPosOut - 6);
	shadowObj->Update();
}

void BaseEnemy::UpdateIn()
{
	if (isFiled == FIELD_OUT) {
		return;
	}

	if (inhp <= 0) {
		dead = true;
	}

	if (!isAction) {
		//オブジェクトの更新
		object->Update();
		shadowObj->SetPosition(object->GetPosition());
		shadowObj->SetPosY(-6 - 4);
		shadowObj->Update();
		return;
	}

	oldPos = object->GetPosition();

	//移動処理

	Reaction();
	if (kazuFlag == false) {
		object->SetRotation({ 0,0,0 });
		//プレイヤーから遠かったら近づき、近かったらプレイヤーの周りをまわる
		if (attackFlag == false && stopFlag == false) {
			Move();

		}
		//攻撃処理
		else if (attackFlag == true && maeburiFlag == false) {
			Attack();
		}
	}
	else {
		object->SetRotation({ 45,0,0 });
	}

	if (maeburiFlag == true)PreliminaryOperation();
	if (stopFlag == true)Stop();

	//敵がエリア外に出ないようにする
	if (Collision::CheckDistance(object->GetPosition(), { 0,0,0 }) > 125.0f) {
		object->SetPosition(oldPos);
	}

	//オブジェクトの更新
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(-6 - 4);
	shadowObj->Update();
	yazirusi->SetPosition(object->GetPosition());
	yazirusi->SetPosY(12.0f);
	yazirusi->Update();
}

void BaseEnemy::Move()
{
	//プレイヤーの方向を見る
	ViewpointPlayer(player);
	//プレイヤーと敵の距離を計算
	float distance1 = Collision::CheckDistance(object->GetPosition(), player->object->GetPosition());

	//徐々にプレイヤーに近づく処理
	if (distance1 > 50 && nearFlag == false || distance1 > 100 && nearFlag == true) {
		nearFlag = false;
		//プレイヤーに近づく
		ApproachPlayer();
	}
	//近かったらプレイヤーの周りをまわるようにするための準備
	else if (nearFlag == false) {
		nearFlag = true;
		moveTime = 0;

		rotaTime = rand() % 100 + 100;
		//回転する回転軸を入れる
		rollPoint = player->object->GetPosition();
		//回転するときの中心からの距離を入れる
		m_Length = distance1;
		//プレイヤーから敵への角度を求める
		float x = object->GetPosition().x - player->object->GetPosition().x;
		float z = object->GetPosition().z - player->object->GetPosition().z;
		float tan = atan2(z, x);
		m_Angle = (tan * 180) / 3.14;
	}

	//プレイヤーのまわりをまわる処理
	if (nearFlag == true) {
		float radius = m_Angle * 3.14f / 180.0f;
		float addx = cos(radius) * m_Length;
		float addy = sin(radius) * m_Length;
		float m_PosX = rollPoint.x + addx;
		float m_PosY = rollPoint.z + addy;
		m_Angle += 0.5f;
		object->SetPosition({ m_PosX,object->GetPosition().y,m_PosY });

		//回り始めてから一定時間経つとプレイヤーに突進する
		moveTime++;
		if (moveTime > rotaTime) {
			attackFlag = true;
			moveTime = 0;
			maeburiFlag = true;

			//突進する方向を計算する
			XMVECTOR pos1;
			pos1.m128_f32[0] = object->GetPosition().x;
			pos1.m128_f32[1] = object->GetPosition().y;
			pos1.m128_f32[2] = object->GetPosition().z;
			XMVECTOR pos2;
			pos2.m128_f32[0] = player->object->GetPosition().x;
			pos2.m128_f32[1] = object->GetPosition().y;
			pos2.m128_f32[2] = player->object->GetPosition().z;
			attackDirection = pos1 - pos2;
			attackDirection = XMVector3Normalize(attackDirection);
			attackDirection.m128_f32[1] = 0;//ここを0にしないとプレイヤーと敵のY座標のずれで敵の突進方向がずれる

			atodekesuROta = object->GetRotation();
		}
	}
}


void BaseEnemy::Attack()
{
	atodekesuROta.z = 90;
	object->SetRotation(atodekesuROta);
	//プレイヤーに突進しにいく処理
	XMVECTOR pos;
	pos.m128_f32[0] = object->GetPosition().x;
	pos.m128_f32[1] = object->GetPosition().y;
	pos.m128_f32[2] = object->GetPosition().z;
	//突進処理
	pos -= attackDirection * 1.1;
	object->SetPosition({ pos.m128_f32[0],pos.m128_f32[1] ,pos.m128_f32[2] });
	//指定した時間突進したら攻撃をやめる
	attackTime++;
	if (attackTime > 120) {
		attackTime = 0;
		attackFlag = false;
		nearFlag = false;
		stopFlag = true;
	}
}

void BaseEnemy::Draw()
{
	object->Draw();
	shadowObj->Draw();
	if (yazirusiFlag && isFiled == FIELD_IN) {
		yazirusi->Draw();
	}
}

void BaseEnemy::Reaction()
{
	if (kazuFlag) {
		attackFlag = false;
		kazuTimer++;
		if (kazuTimer > 50) {
			kazuTimer = 0;
			kazuFlag = false;
		}
	}
	else {

	}
}



void BaseEnemy::ViewpointPlayer(Player* player)
{
	const float direction = 90.0f;
	XMFLOAT3 pos = object->GetPosition();
	XMFLOAT3 playerPos = player->object->GetPosition();

	XMFLOAT3 distance = { pos.x - playerPos.x,pos.y - playerPos.y,pos.z - playerPos.z };

	float angleToPlayer = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;

	object->SetRotation(XMFLOAT3(0.0f, angleToPlayer, 0.0f));
}


void BaseEnemy::Stop()
{
	stopTime++;
	//ストップ処理
	if (stopTime <= 60) {
	}
	else {
		stopTime = 0;
		stopFlag = false;
	}
}

void BaseEnemy::ApproachPlayer()
{
	//敵のいる位置からプレイヤーのいる方向を計算
	XMVECTOR enemyVec = XMLoadFloat3(&object->GetPosition());
	XMVECTOR playerVec = XMLoadFloat3(&player->object->GetPosition());
	XMVECTOR direction = enemyVec - playerVec;
	direction = XMVector3Normalize(direction);
	//敵をプレイヤーのいる方向に進ませる
	enemyVec -= direction * 0.5;
	enemyVec.m128_f32[1] = object->GetPosition().y;	//高さは考慮しない
	object->SetPosition(Use::LoadXMVECTOR(enemyVec));
}

void BaseEnemy::PreliminaryOperation()
{
	jumpTime++;
	XMFLOAT3 vec = { 0,0.2,0 };
	XMFLOAT3 mvec = vec;
	mvec.y = -vec.y;
	int mtime = 30;
	//ジャンプしょり
	if (jumpTime <= mtime) {
		object->VecSetPosition(vec);
	}
	else if (jumpTime <= mtime * 2)
	{
		object->VecSetPosition(mvec);
	}
	else {
		jumpTime = 0;
		maeburiFlag = false;
	}
}

void BaseEnemy::BreakParticle()
{
}

