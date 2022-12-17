#include "EnemyZako.h"
using namespace DirectX;
#include "Useful.h"
#include "ModelManager.h"
#include "safe_delete.h"
#include "Collision.h"


/// 静的メンバ変数の実体
const float EnemyZako::groundInPos = -4.0f;
const float EnemyZako::groundOutPos = -4.0f;
int EnemyZako::isAction = 1;


EnemyZako* EnemyZako::CreateIn(int filedFlag, XMFLOAT3 pos, bool isTarget)
{
	EnemyZako* ins = new EnemyZako();
	ins->InitializeOut(filedFlag, pos, isTarget);
	return ins;
}

EnemyZako::EnemyZako()
{
}

EnemyZako::~EnemyZako()
{
	safe_delete(object);
	enemies.clear();
}

void EnemyZako::Damege(int attackPower)
{
	hp -= attackPower;
	int deleteNum = 0;
	for (std::unique_ptr<EnemyZako>& enemy : enemies) {
		enemy->SetDead();
		deleteNum++;
		if (deleteNum >= attackPower) { 
			return; 
		}
	}
}

void EnemyZako::InitializeOut(int filedFlag, XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1, XMFLOAT3 targetPos2)
{
	this->isFiled = filedFlag;
	this->isTarget = isTarget;
	this->targetPos1 = targetPos1;
	this->targetPos2 = targetPos2;
	targetIndex = 1;

	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("enemy"));
	object->SetPosition(pos);
	object->SetScale({ 4.0f,4.0f, 4.0f });
	oldPos = object->GetPosition();

	//敵の影を作成
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//目的地が設定されていたら
	if (isTarget == true) {
		//移動する方向を計算する
		XMVECTOR pos1 = XMLoadFloat3(&object->GetPosition());
		targetVec = pos1 - XMLoadFloat3(&targetPos1);
		targetVec = XMVector3Normalize(targetVec);
		targetVec.m128_f32[1] = 0;//ここを0にしないとプレイヤーと敵のY座標のずれで敵の突進方向がずれる
	}

	//この敵が中シーンに移動した時に持っている小敵の情報を追加
	int maxEnemyNum = rand() % 2 + 5;
	for (int i = 0; i < maxEnemyNum; i++)
	{
		//敵をリストに追加していく
		std::unique_ptr<EnemyZako> newEnemyZako = std::make_unique<EnemyZako>();
		newEnemyZako->InitializeIn(FIELD_IN);
		//リストに登録
		enemies.push_back(std::move(newEnemyZako));
	}

	//HPを計算
	maxHp = enemies.size();
	hp = maxHp;
}

void EnemyZako::InitializeIn(int filedFlag)
{
	this->isFiled = filedFlag;

	targetIndex = 1;
	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("enemy"));
	//影を作成
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//座標のセット
	float x = rand() % 200 - 100;
	float z = rand() % 200 - 100;
	object->SetPosition({ x,groundInPos,z });
	//サイズのセット
	object->SetScale({ 4.0f,4.0f, 4.0f });
}

void EnemyZako::UpdateOut()
{
	scale = hp / maxHp;
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
			//XMFLOAT3 pos = object->GetPosition() - targetVec * 1;
			//object->SetPosition(pos);
			if (targetIndex == 1) {
				if (targetPos1.z > 0) {
					object->VecSetPosition(XMFLOAT3{ 0,0,speed });
					if (object->GetPosition().z >= oldPos.z + targetPos1.z) {
						targetIndex = 2;
						oldPos = object->GetPosition();
					}
				}
				if (targetPos1.z < 0) {
					object->VecSetPosition(XMFLOAT3{ 0,0,-speed });
					if (object->GetPosition().z <= oldPos.z + targetPos1.z) {
						targetIndex = 2;
						oldPos = object->GetPosition();
					}
				}
			}
			else if (targetIndex == 2) {
				if (targetPos2.x > 0) {
					object->VecSetPosition(XMFLOAT3{ speed,0,0 });
					if (object->GetPosition().x >= targetPos2.x) {
						targetIndex = 2;
					}
				}
				if (targetPos2.x < 0) {
					object->VecSetPosition(XMFLOAT3{ -speed,0,0 });
					if (object->GetPosition().x <= targetPos2.x) {
						targetIndex = 2;
					}
				}
			}
		}
		for (std::unique_ptr<EnemyZako>& enemy : enemies) {
			//enemy->SetDead();
		}
		enemies.remove_if([](std::unique_ptr<EnemyZako>& enemy) {return enemy->GetDead(); });
		if (hp <= 0) {
			SetDead();
		}
	}
	//オブジェクトの更新
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(-6 - 4);
	shadowObj->Update();
}

void EnemyZako::UpdateIn()
{
	if (isFiled == FIELD_OUT) {
		return;
	}

	if (isAction > 0) {
		//移動処理
		//プレイヤーから遠かったら近づき、近かったらプレイヤーの周りをまわる
		if (attackFlag == false && stopFlag == false) {
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
		//攻撃処理
		else if (attackFlag == true && maeburiFlag == false) {
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

		if (maeburiFlag == true)PreliminaryOperation();
		if (stopFlag == true)Stop();

	}
	//オブジェクトの更新
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(-6 - 4);
	shadowObj->Update();
}

void EnemyZako::Draw()
{
	object->Draw();
	shadowObj->Draw();
}



void EnemyZako::ViewpointPlayer(Player* player)
{
	const float direction = 90.0f;
	XMFLOAT3 pos = object->GetPosition();
	XMFLOAT3 playerPos = player->object->GetPosition();

	XMFLOAT3 distance = { pos.x - playerPos.x,pos.y - playerPos.y,pos.z - playerPos.z };

	float angleToPlayer = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;

	object->SetRotation(XMFLOAT3(0.0f, angleToPlayer, 0.0f));
}


void EnemyZako::Stop()
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
void EnemyZako::ApproachPlayer()
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

void EnemyZako::PreliminaryOperation()
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

