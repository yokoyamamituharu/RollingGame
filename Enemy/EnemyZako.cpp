#include "EnemyZako.h"
using namespace DirectX;
#include "Useful.h"
#include "ModelManager.h"
#include "safe_delete.h"
#include "Collision.h"


/// �ÓI�����o�ϐ��̎���
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

	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("enemy"));
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
		std::unique_ptr<EnemyZako> newEnemyZako = std::make_unique<EnemyZako>();
		newEnemyZako->InitializeIn(FIELD_IN);
		//���X�g�ɓo�^
		enemies.push_back(std::move(newEnemyZako));
	}

	//HP���v�Z
	maxHp = enemies.size();
	hp = maxHp;
}

void EnemyZako::InitializeIn(int filedFlag)
{
	this->isFiled = filedFlag;

	targetIndex = 1;
	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("enemy"));
	//�e���쐬
	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 10,1,10 });

	//���W�̃Z�b�g
	float x = rand() % 200 - 100;
	float z = rand() % 200 - 100;
	object->SetPosition({ x,groundInPos,z });
	//�T�C�Y�̃Z�b�g
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
			//�ړI�n�Ɍ������Ē��i			
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
	//�I�u�W�F�N�g�̍X�V
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
		//�ړ�����
		//�v���C���[���牓��������߂Â��A�߂�������v���C���[�̎�����܂��
		if (attackFlag == false && stopFlag == false) {
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
		//�U������
		else if (attackFlag == true && maeburiFlag == false) {
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

		if (maeburiFlag == true)PreliminaryOperation();
		if (stopFlag == true)Stop();

	}
	//�I�u�W�F�N�g�̍X�V
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
	//�X�g�b�v����
	if (stopTime <= 60) {
	}
	else {
		stopTime = 0;
		stopFlag = false;
	}
}
void EnemyZako::ApproachPlayer()
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

void EnemyZako::PreliminaryOperation()
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

