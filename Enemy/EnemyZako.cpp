#include "EnemyZako.h"
using namespace DirectX;

float EnemyZako::Nitenkan(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	float distance = sqrtf(((pos1.x - pos2.x) * (pos1.x - pos2.x)) +
		((pos1.z - pos2.z) * (pos1.z - pos2.z)));

	return distance;
}

void EnemyZako::Initialize(Input* input, InputMouse* mouse, Camera* camera)
{
	assert(input);
	assert(mouse);
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;
	//�G�p���f����ǂݍ���
	enemyModel = Model::Create("enemy");
	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(enemyModel);
}

void EnemyZako::Update()
{
	//�G���s�������邩�����Ȃ���
	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		actionFlag *= -1;
	}

	Direction(player);

	//�G�̍s��
	if (actionFlag > 0) {
		//�ړ�
		if (attackFlag == false) {
			//�v���C���[�ƓG�̋������v�Z
			float distance1 = Nitenkan(object->GetPosition(), player->object->GetPosition());

			//���X�Ƀv���C���[�ɋ߂Â�����
			if (distance1 > 50 && nearFlag == false || distance1 > 100 && nearFlag == true) {
				nearFlag = false;

				//�G�̂���ʒu����v���C���[�̂���������v�Z
				XMVECTOR pos1;
				pos1.m128_f32[0] = object->GetPosition().x;
				pos1.m128_f32[1] = object->GetPosition().y;
				pos1.m128_f32[2] = object->GetPosition().z;
				XMVECTOR pos2;
				pos2.m128_f32[0] = player->object->GetPosition().x;
				pos2.m128_f32[1] = player->object->GetPosition().y;
				pos2.m128_f32[2] = player->object->GetPosition().z;
				XMVECTOR direction = pos1 - pos2;
				direction = XMVector3Normalize(direction);
				//�G���v���C���[�̂�������ɐi�܂���
				XMVECTOR enemypos;
				enemypos.m128_f32[0] = object->GetPosition().x;
				enemypos.m128_f32[1] = object->GetPosition().y;
				enemypos.m128_f32[2] = object->GetPosition().z;
				enemypos -= direction * 1;
				enemypos.m128_f32[1] = object->GetPosition().y;
				object->SetPosition({ enemypos.m128_f32[0],enemypos.m128_f32[1] ,enemypos.m128_f32[2] });
			}
			//�߂�������v���C���[�̎�����܂��悤�ɂ��邽�߂̏���
			else if (nearFlag == false) {
				nearFlag = true;
				moveTime = 0;
				abaramoveTime = rand() % 100 + 100;
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
				if (moveTime > abaramoveTime) {
					attackFlag = true;
					moveTime = 0;

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
				}
			}

		}
		//�U������
		else if (attackFlag == true) {
			//�v���C���[�ɓːi���ɂ�������
			XMVECTOR pos;
			pos.m128_f32[0] = object->GetPosition().x;
			pos.m128_f32[1] = object->GetPosition().y;
			pos.m128_f32[2] = object->GetPosition().z;
			//�ːi����
			pos -= attackDirection * 2;
			object->SetPosition({ pos.m128_f32[0],pos.m128_f32[1] ,pos.m128_f32[2] });
			//�w�肵�����ԓːi������U������߂�
			attackTime++;
			if (attackTime > 60) {
				attackTime = 0;
				attackFlag = false;
				nearFlag = false;
			}
		}
	}

	//�I�u�W�F�N�g�̍X�V
	object->Update();
}



void EnemyZako::GoTarget(XMFLOAT3 target)
{
	float speed = 10.0f;

	////�U���p���[�J���ϐ�
	//XMFLOAT3 direction = object->GetPosition();
	////direction= XMVector3Normalize(direction);

	//enemypos = {direction.x* speed, direction.y* speed, direction.z* speed};
	//enemypos1 = { enemypos1.x * direction.x, enemypos1.y * direction.y, enemypos1.z * direction.z };
	//object->SetPosition(enemypos1);

	float posX = object->GetPosition().x;
	float posY = object->GetPosition().y;
	float posZ = object->GetPosition().z;
	float playerPosX = player->object->GetPosition().x;
	float playerPosZ = player->object->GetPosition().z;
	float distanceX = 0;
	float distanceZ = 0;
	//oldPlayerPos = player->object->GetPosition();

	distanceX = posX - target.x;
	distanceZ = posZ - target.z;

	posX -= distanceX / speed;
	posZ -= distanceZ / speed;

	object->SetPosition(XMFLOAT3(posX, posY, posZ));
}

void EnemyZako::Direction(Player* player)
{
	const float direction = 90.0f;
	XMFLOAT3 pos = object->GetPosition();
	XMFLOAT3 playerPos = player->object->GetPosition();

	XMFLOAT3 distance = { pos.x - playerPos.x,pos.y - playerPos.y,pos.z - playerPos.z };

	angle = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;

	object->SetRotation(XMFLOAT3(0.0f, angle, 0.0f));
}

void EnemyZako::Mawarikomi(Player* player)
{
	num -= 0.005;
	float x = object->GetPosition().x + 0.3f;
	float y = object->GetPosition().y;
	float z = object->GetPosition().z + num;

	object->SetPosition({ x, y, z });
}

