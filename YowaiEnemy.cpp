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
}

YowaiEnemy::~YowaiEnemy()
{
}

void YowaiEnemy::InitializeOut(bool isTarget, XMFLOAT2 route[])
{
	this->isFiled = FIELD_OUT;
	this->isTarget = isTarget;
	this->targetPos1 = targetPos1;
	this->targetPos2 = targetPos2;
	targetIndex = 1;
	for (int i = 0; i < 3; i++) {
		this->route[i] = route[i];
	}

	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("yowaienemy"));
	object->SetPosition({ route[0].x, groundOutPos,route[0].y });
	object->SetScale({ 4.0f,4.0f, 4.0f });
	oldPos = object->GetPosition();

	//敵の影を作成
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//目的地が設定されていたら
	if (isTarget == true) {
		//移動する方向を計算する
		XMVECTOR pos1 = XMLoadFloat3(&object->GetPosition());
		targetVec = XMLoadFloat3(&XMFLOAT3{ route[1].x, groundOutPos,route[1].y }) - pos1;
		targetVec = XMVector3Normalize(targetVec);
		targetVec.m128_f32[1] = 0;//ここを0にしないとプレイヤーと敵のY座標のずれで敵の突進方向がずれる
	}

	//この敵が中シーンに移動した時に持っている小敵の情報を追加
	int maxEnemyNum = rand() % 2 + 5;
	for (int i = 0; i < maxEnemyNum; i++)
	{
		//敵をリストに追加していく
		std::unique_ptr<EnemyZako> newEnemyZako = std::make_unique<YowaiEnemy>();
		newEnemyZako->InitializeIn();
		//リストに登録
		enemies.push_back(std::move(newEnemyZako));
	}
	yazirusi = ObjectObj::Create(ModelManager::GetModel("yazirusi"));
	yazirusi->SetScale({ 10,10,10 });
	//HPを計算
	outmaxHp = enemies.size();
	outhp = outmaxHp;
}

void YowaiEnemy::InitializeIn()
{
	this->isFiled = FIELD_IN;

	targetIndex = 1;
	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("yowaienemy"));
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
	inhp = 1;
	yazirusi = ObjectObj::Create(ModelManager::GetModel("yazirusi"));
	yazirusi->SetScale({ 10,10,10 });
}
