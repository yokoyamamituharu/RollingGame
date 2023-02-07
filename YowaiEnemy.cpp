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

void YowaiEnemy::InitializeOut(XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1, XMFLOAT3 targetPos2)
{
	this->isFiled = FIELD_OUT;
	this->isTarget = isTarget;
	this->targetPos1 = targetPos1;
	this->targetPos2 = targetPos2;
	targetIndex = 1;

	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("yowaienemy"));
	object->SetPosition(pos);
	object->SetScale({ 4.0f,4.0f, 4.0f });
	oldPos = object->GetPosition();

	//�G�̉e���쐬
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//�ړI�n���ݒ肳��Ă�����
	if (isTarget == true) {
		//�ړ�����������v�Z����
		XMVECTOR pos1 = XMLoadFloat3(&object->GetPosition());
		targetVec = pos1 - XMLoadFloat3(&targetPos1);
		targetVec = XMVector3Normalize(targetVec);
		targetVec.m128_f32[1] = 0;//������0�ɂ��Ȃ��ƃv���C���[�ƓG��Y���W�̂���œG�̓ːi�����������
	}

	//���̓G�����V�[���Ɉړ��������Ɏ����Ă��鏬�G�̏���ǉ�
	int maxEnemyNum = rand() % 2 + 5;
	for (int i = 0; i < maxEnemyNum; i++)
	{
		//�G�����X�g�ɒǉ����Ă���
		std::unique_ptr<EnemyZako> newEnemyZako = std::make_unique<YowaiEnemy>();
		newEnemyZako->InitializeIn();
		//���X�g�ɓo�^
		enemies.push_back(std::move(newEnemyZako));
	}

	//HP���v�Z
	outmaxHp = enemies.size();
	outhp = outmaxHp;
}

void YowaiEnemy::InitializeIn()
{
	this->isFiled = FIELD_IN;

	targetIndex = 1;
	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("yowaienemy"));
	//�e���쐬
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//���W�̃Z�b�g
	float x = rand() % 200 - 100;
	float z = rand() % 200 - 100;
	object->SetPosition({ x,groundInPos,z });
	//�T�C�Y�̃Z�b�g
	object->SetScale({ 4.0f,4.0f, 4.0f });
	//object->SetCollider(new SphereCollider({ 0,0,0 }, 10.0f));
	//object->collider->SetAttribute(COLLISION_ATTR_ALLIES);
	inhp = 1;
}
