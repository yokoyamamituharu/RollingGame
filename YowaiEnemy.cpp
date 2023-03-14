#include "YowaiEnemy.h"
using namespace DirectX;
#include "Useful.h"
#include "ModelManager.h"
#include "safe_delete.h"
#include "Collision.h"
#include "Particle.h"
#include "SphereCollider.h"

YowaiEnemy::YowaiEnemy()
{
	modelName = "yowaienemy";
}

YowaiEnemy::~YowaiEnemy()
{
}

void YowaiEnemy::CreateEnemy()
{
	//この敵が中シーンに移動した時に持っている小敵の情報を追加
	int maxEnemyNum = rand() % 2 + 5;
	for (int i = 0; i < maxEnemyNum; i++)
	{
		//敵をリストに追加していく
		std::unique_ptr<YowaiEnemy> newBaseEnemy = std::make_unique<YowaiEnemy>();
		newBaseEnemy->InitializeIn();
		//リストに登録
		enemies.push_back(std::move(newBaseEnemy));
	}
	//HPを計算
	outmaxHp = enemies.size();
	outhp = outmaxHp;
}

std::shared_ptr<YowaiEnemy> YowaiEnemy::Create(bool isTarget, XMFLOAT2 route[])
{
	std::shared_ptr<YowaiEnemy> enemy = std::make_shared<YowaiEnemy>();
	enemy->InitializeOut(isTarget, route);
	enemy->CreateEnemy();
	return enemy;
}

void YowaiEnemy::InitializeIn()
{
	this->isFiled = FIELD_IN;

	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("yowaienemy"));
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

	inhp = 1;
}

//void YowaiEnemy::InitIndividualSetUp()
//{
//	object->SetModel(ModelManager::GetModel("yowaienemy"));
//}
