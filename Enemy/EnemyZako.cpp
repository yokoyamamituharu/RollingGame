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
	Direction(player);
	//Move();
	//if (flag1)
	if (0)
	{

		if (waitTime <= 0) {
			waitTime = rand() % 360 + 240;
			//waitTime = 60;
			num1 = waitTime;
		}
		if (waitTime > 0 && attackFlag == false)
		{
			//�ړ�����
			if (num1 >= waitTime) {
				num1 = waitTime - (rand() % 60 + 60);
				num2 = rand() % 2 + 1;
			}

			float a = 0.2f;
			if (num2 == 1) {
				move = XMFLOAT3(a, 0.0f, 0.0f);
			}
			else if (num2 == 2) {
				move = XMFLOAT3(-a, 0.0f, 0.0f);
			}
			else if (num2 == 3) {
				move = XMFLOAT3(0.0f, 0.0f, -a);
			}
			else if (num2 == 4) {
				move = XMFLOAT3(0.0f, 0.0f, a);
			}
			MoveVector(move);

			waitTime--;
			if (waitTime <= 0) {
				attackFlag = true;
				targetPos = player->object->GetPosition();
				timer1 = 120;
			}
		}

		if (attackFlag)
		{
			timer1--;
			GoTarget(targetPos);
			if (object->GetPosition().x == targetPos.x && object->GetPosition().z == targetPos.z || timer1 <= 0) {
				attackFlag = false;
			}
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		flag *= -1;
	}
	if (flag > 0) {
		//�G�̈ړ�
		if (attackFlag == false) {
			float distance1 = Nitenkan(object->GetPosition(), player->object->GetPosition());
			if (distance1 > 50 && nearFlag == false || distance1 > 100 && nearFlag == true) {
				nearFlag = false;
				//���X�Ƀv���C���[�ɋ߂Â�����
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
				XMVECTOR enemypos;
				enemypos.m128_f32[0] = object->GetPosition().x;
				enemypos.m128_f32[1] = object->GetPosition().y;
				enemypos.m128_f32[2] = object->GetPosition().z;
				//�G���v���C���[�̂���ʒu�ɋ߂Â��鏈��
				enemypos -= direction * 1;
				object->SetPosition({ enemypos.m128_f32[0],enemypos.m128_f32[1] ,enemypos.m128_f32[2] });
			}
			//�߂�������v���C���[�̎�����܂��
			else if(nearFlag ==false) {
				nearFlag = true;
				rollPoint = player->object->GetPosition();
				XMFLOAT3 pos1 = object->GetPosition();
				XMFLOAT3 pos2 = player->object->GetPosition();
				//�E��
				if (pos1.x > pos2.x&&pos1.y >pos2.y) {

				}
				//����
				if (pos1.x < pos2.x && pos1.y > pos2.y) {

				}
				//����
				if (pos1.x < pos2.x && pos1.y < pos2.y) {

				}
				//�E��
				if (pos1.x > pos2.x && pos1.y < pos2.y) {

				}
			}

			//�v���C���[�̂܂����܂�鏈��
			if (nearFlag == true) {
				float radius = m_Angle * 3.14f / 180.0f;
				float addx = cos(radius) * m_Length;
				float addy = sin(radius) * m_Length;
				m_CenterX = rollPoint.x;
				m_CenterY = rollPoint.z;
				m_PosX = m_CenterX + addx;
				m_PosY = m_CenterY + addy;
				m_Angle += 0.5f;
				object->SetPosition({ m_PosX,0,m_PosY });

				moveTime++;
				if (moveTime > 300) {
					attackFlag = true;
					moveTime = 0;
					attackPoint = player->object->GetPosition();
				}
			}

		}
		//�U������
		else if (attackFlag == true) {
			//�v���C���[�ɓːi���ɂ�������
			//�ŏ��ɓːi���������ݒ�
			if (attackFlag2 == false) {
				XMVECTOR pos1;
				pos1.m128_f32[0] = object->GetPosition().x;
				pos1.m128_f32[1] = object->GetPosition().y;
				pos1.m128_f32[2] = object->GetPosition().z;
				XMVECTOR pos2;
				pos2.m128_f32[0] = player->object->GetPosition().x;
				pos2.m128_f32[1] = object->GetPosition().y;
				pos2.m128_f32[2] = player->object->GetPosition().z;
				direction1 = pos1 - pos2;
				direction1 = XMVector3Normalize(direction1);
				attackFlag2 = true;
			}
			XMVECTOR enemypos;
			enemypos.m128_f32[0] = object->GetPosition().x;
			enemypos.m128_f32[1] = object->GetPosition().y;
			enemypos.m128_f32[2] = object->GetPosition().z;
			//�ːi����
			enemypos -= direction1 * 2;
			object->SetPosition({ enemypos.m128_f32[0],enemypos.m128_f32[1] ,enemypos.m128_f32[2] });
			attackTime++;
			if (attackTime > 60) {
				attackTime = 0;
				attackFlag = false;
				attackFlag2 = false;
				m_Angle += 180;
			}
		}
	}


	{
		//object->SetPosition(player->object->GetPosition());
	}
	tossintime++;
	if (tossintime > 60) {
		tossinFlag = false;
	}
	if (tossintime > 120) {
		tossintime = 0;
		tossinFlag = true;
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

