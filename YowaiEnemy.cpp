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

	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("yowaienemy"));
	object->SetPosition({ route[0].x, groundOutPos,route[0].y });
	object->SetScale({ 4.0f,4.0f, 4.0f });
	oldPos = object->GetPosition();

	//�G�̉e���쐬
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//�ړI�n���ݒ肳��Ă�����
	if (isTarget == true) {
		//�ړ�����������v�Z����
		XMVECTOR pos1 = XMLoadFloat3(&object->GetPosition());
		targetVec = XMLoadFloat3(&XMFLOAT3{ route[1].x, groundOutPos,route[1].y }) - pos1;
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
	yazirusi = ObjectObj::Create(ModelManager::GetModel("yazirusi"));
	yazirusi->SetScale({ 10,10,10 });
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
	yazirusi = ObjectObj::Create(ModelManager::GetModel("yazirusi"));
	yazirusi->SetScale({ 10,10,10 });
}
