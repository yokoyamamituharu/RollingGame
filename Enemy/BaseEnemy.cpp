#include "BaseEnemy.h"
using namespace DirectX;
#include "Useful.h"
#include "ModelManager.h"
#include "safe_delete.h"
#include "Collision.h"
#include "Particle.h"
#include "SphereCollider.h"
#include "YowaiEnemy.h"

/// �ÓI�����o�ϐ��̎���
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

	//��������
	InitGeneralSetUp();

	//�G�𐶐�
	//CreateEnemy();

	////HP���v�Z
	//outmaxHp = enemies.size();
	//outhp = outmaxHp;
}

void BaseEnemy::InitGeneralSetUp()
{
	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel(modelName));
	object->SetPosition({ route[0].x, groundPosOut,route[0].y });
	object->SetScale({ 4.0f,4.0f, 4.0f });

	//�G�̉e���쐬
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//�ړI�n���ݒ肳��Ă�����
	if (isTarget == true) {
		//�ړ�����������v�Z����
		XMVECTOR pos1 = XMLoadFloat3(&object->GetPosition());
		targetVec = XMLoadFloat3(&XMFLOAT3{ route[1].x, groundPosOut,route[1].y }) - pos1;
		targetVec = XMVector3Normalize(targetVec);
		targetVec.m128_f32[1] = 0;//Y���̈ړ���0�ɂ���
	}
}

void BaseEnemy::CreateEnemy()
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
			//enemie s.push_back(std::move(newBaseEnemy));
		}
	}
	//HP���v�Z
	outmaxHp = enemies.size();
	outhp = outmaxHp;
}

void BaseEnemy::InitializeIn()
{
	this->isFiled = FIELD_IN;

	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("enemy"));
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
			//�ړI�n�Ɍ������Ē��i	

			XMFLOAT3 pos = object->GetPosition() + targetVec * 0.5;
			object->SetPosition(pos);
			//�ړI�n�𒴂��Ă�����
			if (Collision::CheckExceed(
				{ route[targetIndex - 1].x,groundPosOut,  route[targetIndex - 1].y },
				object->GetPosition(),
				{ route[targetIndex].x,groundPosOut,  route[targetIndex].y })) {
				targetIndex++;
				if (targetIndex >= 3) {
					targetIndex = 2;
				}
				//�ړ�����������v�Z����
				XMVECTOR pos1 = XMLoadFloat3(&object->GetPosition());
				targetVec = XMLoadFloat3(&XMFLOAT3{ route[targetIndex].x, groundPosOut,route[targetIndex].y }) - pos1;
				targetVec = XMVector3Normalize(targetVec);
				targetVec.m128_f32[1] = 0;//������0�ɂ��Ȃ��ƃv���C���[�ƓG��Y���W�̂���œG�̓ːi�����������
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


	//�I�u�W�F�N�g�̍X�V
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
		//�I�u�W�F�N�g�̍X�V
		object->Update();
		shadowObj->SetPosition(object->GetPosition());
		shadowObj->SetPosY(-6 - 4);
		shadowObj->Update();
		return;
	}

	oldPos = object->GetPosition();

	//�ړ�����

	Reaction();
	if (kazuFlag == false) {
		object->SetRotation({ 0,0,0 });
		//�v���C���[���牓��������߂Â��A�߂�������v���C���[�̎�����܂��
		if (attackFlag == false && stopFlag == false) {
			Move();

		}
		//�U������
		else if (attackFlag == true && maeburiFlag == false) {
			Attack();
		}
	}
	else {
		object->SetRotation({ 45,0,0 });
	}

	if (maeburiFlag == true)PreliminaryOperation();
	if (stopFlag == true)Stop();

	//�G���G���A�O�ɏo�Ȃ��悤�ɂ���
	if (Collision::CheckDistance(object->GetPosition(), { 0,0,0 }) > 125.0f) {
		object->SetPosition(oldPos);
	}

	//�I�u�W�F�N�g�̍X�V
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
	//�v���C���[�̕���������
	ViewpointPlayer(player);
	//�v���C���[�ƓG�̋������v�Z
	float distance1 = Collision::CheckDistance(object->GetPosition(), player->object->GetPosition());

	//���X�Ƀv���C���[�ɋ߂Â�����
	if (distance1 > 50 && nearFlag == false || distance1 > 100 && nearFlag == true) {
		nearFlag = false;
		//�v���C���[�ɋ߂Â�
		ApproachPlayer();
	}
	//�߂�������v���C���[�̎�����܂��悤�ɂ��邽�߂̏���
	else if (nearFlag == false) {
		nearFlag = true;
		moveTime = 0;

		rotaTime = rand() % 100 + 100;
		//��]�����]��������
		rollPoint = player->object->GetPosition();
		//��]����Ƃ��̒��S����̋���������
		m_Length = distance1;
		//�v���C���[����G�ւ̊p�x�����߂�
		float x = object->GetPosition().x - player->object->GetPosition().x;
		float z = object->GetPosition().z - player->object->GetPosition().z;
		float tan = atan2(z, x);
		m_Angle = (tan * 180) / 3.14;
	}

	//�v���C���[�̂܂����܂�鏈��
	if (nearFlag == true) {
		float radius = m_Angle * 3.14f / 180.0f;
		float addx = cos(radius) * m_Length;
		float addy = sin(radius) * m_Length;
		float m_PosX = rollPoint.x + addx;
		float m_PosY = rollPoint.z + addy;
		m_Angle += 0.5f;
		object->SetPosition({ m_PosX,object->GetPosition().y,m_PosY });

		//���n�߂Ă����莞�Ԍo�ƃv���C���[�ɓːi����
		moveTime++;
		if (moveTime > rotaTime) {
			attackFlag = true;
			moveTime = 0;
			maeburiFlag = true;

			//�ːi����������v�Z����
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
			attackDirection.m128_f32[1] = 0;//������0�ɂ��Ȃ��ƃv���C���[�ƓG��Y���W�̂���œG�̓ːi�����������

			atodekesuROta = object->GetRotation();
		}
	}
}


void BaseEnemy::Attack()
{
	atodekesuROta.z = 90;
	object->SetRotation(atodekesuROta);
	//�v���C���[�ɓːi���ɂ�������
	XMVECTOR pos;
	pos.m128_f32[0] = object->GetPosition().x;
	pos.m128_f32[1] = object->GetPosition().y;
	pos.m128_f32[2] = object->GetPosition().z;
	//�ːi����
	pos -= attackDirection * 1.1;
	object->SetPosition({ pos.m128_f32[0],pos.m128_f32[1] ,pos.m128_f32[2] });
	//�w�肵�����ԓːi������U������߂�
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
	//�X�g�b�v����
	if (stopTime <= 60) {
	}
	else {
		stopTime = 0;
		stopFlag = false;
	}
}

void BaseEnemy::ApproachPlayer()
{
	//�G�̂���ʒu����v���C���[�̂���������v�Z
	XMVECTOR enemyVec = XMLoadFloat3(&object->GetPosition());
	XMVECTOR playerVec = XMLoadFloat3(&player->object->GetPosition());
	XMVECTOR direction = enemyVec - playerVec;
	direction = XMVector3Normalize(direction);
	//�G���v���C���[�̂�������ɐi�܂���
	enemyVec -= direction * 0.5;
	enemyVec.m128_f32[1] = object->GetPosition().y;	//�����͍l�����Ȃ�
	object->SetPosition(Use::LoadXMVECTOR(enemyVec));
}

void BaseEnemy::PreliminaryOperation()
{
	jumpTime++;
	XMFLOAT3 vec = { 0,0.2,0 };
	XMFLOAT3 mvec = vec;
	mvec.y = -vec.y;
	int mtime = 30;
	//�W�����v�����
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

