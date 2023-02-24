#include "EnemyZako.h"
#include "YowaiEnemy.h"
#include "safe_delete.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "Particle.h"
#include "ModelManager.h"
#include "Useful.h"

using namespace DirectX;

EnemyZako::EnemyZako()
{
}

EnemyZako::~EnemyZako()
{
}

void EnemyZako::InitializeOut(XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1, XMFLOAT3 targetPos2)
{
	this->isFiled = FIELD_OUT;
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
			enemies.push_back(std::move(newBaseEnemy));
		}
	}

	//HPを計算
	outmaxHp = enemies.size();
	outhp = outmaxHp;
}

void EnemyZako::InitializeIn()
{
	this->isFiled = FIELD_IN;

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
	//object->SetCollider(new SphereCollider({ 0,0,0 }, 10.0f));
	//object->collider->SetAttribute(COLLISION_ATTR_ALLIES);

	inhp = 5;
}
