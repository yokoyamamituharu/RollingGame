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
	modelName = "enemy";
}

EnemyZako::~EnemyZako()
{
	//ParticleCreate();
	safe_delete(object);
	safe_delete(shadowObj);
	safe_delete(yazirusi);
	enemies.clear();
}

std::shared_ptr<EnemyZako> EnemyZako::Create(bool isTarget, XMFLOAT2 route[])
{
	std::shared_ptr<EnemyZako> enemy = std::make_shared<EnemyZako>();
	enemy->InitializeOut(isTarget, route);
	enemy->object->SetRotation({ 0,0,0 });
	enemy->CreateEnemy();
	return enemy;
}

void EnemyZako::CreateEnemy()
{
	//���̓G�����V�[���Ɉړ��������Ɏ����Ă��鏬�G�̏���ǉ�
	int maxEnemyNum = rand() % 2 + 5;
	for (int i = 0; i < maxEnemyNum; i++)
	{
		if (rand() % 10 > 4) {
			//�G�����X�g�ɒǉ����Ă���
			std::unique_ptr<BaseEnemy> newBaseEnemy = std::make_unique<BaseEnemy>();
			newBaseEnemy->InitializeIn();
			//���X�g�ɓo�^
			enemies.push_back(std::move(newBaseEnemy));
		}
		else {
			//�G�����X�g�ɒǉ����Ă���
			std::unique_ptr<YowaiEnemy> newBaseEnemy = std::make_unique<YowaiEnemy>();
			newBaseEnemy->InitializeIn();
			//���X�g�ɓo�^
			enemies.push_back(std::move(newBaseEnemy));
		}
	}
	//HP���v�Z
	outmaxHp = enemies.size();
	outhp = outmaxHp;
}
