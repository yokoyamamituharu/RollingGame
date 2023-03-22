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
	//���̓G�����V�[���Ɉړ��������Ɏ����Ă��鏬�G�̏���ǉ�
	int maxEnemyNum = rand() % 2 + 5;
	for (int i = 0; i < maxEnemyNum; i++)
	{
		//�G�����X�g�ɒǉ����Ă���
		std::unique_ptr<YowaiEnemy> newBaseEnemy = std::make_unique<YowaiEnemy>();
		newBaseEnemy->InitializeIn();
		//���X�g�ɓo�^
		enemies.push_back(std::move(newBaseEnemy));
	}
	//HP���v�Z
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

	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("yowaienemy"));
	//�e���쐬
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });
	//�U���i�s�ɋ�����\��������
	yazirusi = ObjectObj::Create(ModelManager::GetModel("yazirusi"));
	yazirusi->SetScale({ 3,3,3 });

	//���W�̃Z�b�g
	float x = rand() % 200 - 100;
	float z = rand() % 200 - 100;
	object->SetPosition({ x,groundPosIn,z });
	//�T�C�Y�̃Z�b�g
	object->SetScale({ 4.0f,4.0f, 4.0f });
	//�R���C�_�[�̃Z�b�g
	object->SetCollider(new SphereCollider({ 0,0,0 }, 10.0f));
	object->collider->SetAttribute(COLLISION_ATTR_ALLIES);

	inhp = 1;
}

//void YowaiEnemy::InitIndividualSetUp()
//{
//	object->SetModel(ModelManager::GetModel("yowaienemy"));
//}
