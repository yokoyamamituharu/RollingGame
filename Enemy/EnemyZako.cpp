#include "EnemyZako.h"
using namespace DirectX;



/// �ÓI�����o�ϐ��̎���
const float EnemyZako::groundInPos = -3.0f;
const float EnemyZako::groundOutPos = -3.0f;
int EnemyZako::isAction = 1;
Model* EnemyZako::enemyModel = nullptr;

float EnemyZako::Nitenkan(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	float distance = sqrtf(((pos1.x - pos2.x) * (pos1.x - pos2.x)) +
		((pos1.z - pos2.z) * (pos1.z - pos2.z)));

	return distance;
}

EnemyZako::EnemyZako()
{
}

EnemyZako::~EnemyZako()
{
}

void EnemyZako::Initialize(int filedFlag, Camera* camera, XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos)
{
	this->isFiled = filedFlag;
	this->camera = camera;
	this->isTarget = isTarget;
	this->targetPos.m128_f32[0] = targetPos.x;
	this->targetPos.m128_f32[1] = targetPos.y;
	this->targetPos.m128_f32[2] = targetPos.z;
	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(enemyModel);

	//�O�p�̓G�Ƃ��ăC���X�^���X���������ꂽ�ꍇ
	if (filedFlag == 1) {
		object->SetPosition(pos);
		object->SetScale({ 4.0f,4.0f, 4.0f });

		//���̓G�����V�[���Ɉړ��������Ɏ����Ă��鏬�G�̏���ǉ�
		int enemyNum = rand() % 2 + 5;
		for (int i = 0; i < enemyNum; i++)
		{
			//�G�����X�g�ɒǉ����Ă���
			std::unique_ptr<EnemyZako> newEnemyZako = std::make_unique<EnemyZako>();
			newEnemyZako->Initialize(FIELD_IN, camera);
			//���X�g�ɓo�^
			enemies.push_back(std::move(newEnemyZako));
		}
	}
	//���p�̓G�Ƃ��ăC���X�^���X���������ꂽ�ꍇ
	else if (filedFlag == 2) {
		//���W�̃Z�b�g
		float x = rand() % 200 - 100;
		float z = rand() % 200 - 100;
		object->SetPosition({ x,groundInPos,z });
		//�T�C�Y�̃Z�b�g
		object->SetScale({ 4.0f,4.0f, 4.0f });
	}

	if (isTarget == true) {
		//�ړ�����������v�Z����
		XMVECTOR pos1;
		pos1.m128_f32[0] = object->GetPosition().x;
		pos1.m128_f32[1] = object->GetPosition().y;
		pos1.m128_f32[2] = object->GetPosition().z;
		targetVec = pos1 - this->targetPos;
		targetVec = XMVector3Normalize(targetVec);
		targetVec.m128_f32[1] = 0;//������0�ɂ��Ȃ��ƃv���C���[�ƓG��Y���W�̂���œG�̓ːi�����������
	}
}

void EnemyZako::Update()
{
	scale = hp / maxHp;
	if (scale <= 0) {
		scale = 1;
		object->SetRotation({ 180,90,0 });
	}
	object->SetScale(maxScale * scale);


	//�O�V�[���ł̏���
	if (isFiled == FIELD_OUT && isAction > 0) {
		if (isTarget == true) {
			//�ړI�n�Ɍ������Ē��i			
			XMFLOAT3 pos = object->GetPosition() - targetVec * 1;
			object->SetPosition(pos);
		}
	}
	//���V�[���ł̏���
	else if (isFiled == FIELD_IN && isAction > 0) {

		//�ړ�����
		//�v���C���[���牓��������߂Â��A�߂�������v���C���[�̎�����܂��
		if (attackFlag == false&& stopFlag==false) {
			//�v���C���[�̕���������
			Direction(player);
			//�v���C���[�ƓG�̋������v�Z
			float distance1 = Nitenkan(object->GetPosition(), player->object->GetPosition());

			//���X�Ƀv���C���[�ɋ߂Â�����
			if (distance1 > 50 && nearFlag == false || distance1 > 100 && nearFlag == true) {
				nearFlag = false;
				//�v���C���[�ɋ߂Â�
				DirectionWotch();
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

		if (maeburiFlag == true)Maeburi();
		if (stopFlag == true)Stop();

	}
	//�I�u�W�F�N�g�̍X�V
	object->Update();
}

void EnemyZako::Draw()
{
	object->Draw();
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

void EnemyZako::EnemyCreateModel()
{
	//�G�p���f����ǂݍ���
	enemyModel = Model::Create("enemy");
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

void EnemyZako::DirectionWotch()
{
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
	enemypos -= direction * 0.5;
	enemypos.m128_f32[1] = object->GetPosition().y;
	object->SetPosition({ enemypos.m128_f32[0],enemypos.m128_f32[1] ,enemypos.m128_f32[2] });
}

void EnemyZako::Maeburi()
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
	else if(jumpTime <= mtime*2)
	{
		object->VecSetPosition(mvec);
	}
	else {
		jumpTime = 0;
		maeburiFlag = false;
	}


}

