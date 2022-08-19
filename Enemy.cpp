#include "Enemy.h"
using namespace DirectX;

void Enemy::Initialize(Input* input, InputMouse* mouse, Camera* camera)
{
	assert(input);
	assert(mouse);
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;
	//�v���C���[�p���f����ǂݍ���
	playermodel = new Model();
	playermodel->CreateFromOBJ("enemy");
	playerSpheremodel = new Model();
	playerSpheremodel->CreateFromOBJ("playerSphere");

	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(playermodel);
}

void Enemy::Update()
{
	//Move();



	//�I�u�W�F�N�g�̍X�V
	object->Update();
}

void Enemy::Move()
{
	DirectX::XMVECTOR forvardvec = {};
	if (Input::GetInstance()->PushKey(DIK_W)) {
		forvardvec.m128_f32[2] += 1;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		forvardvec.m128_f32[2] -= 1;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		forvardvec.m128_f32[0] -= 1;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		forvardvec.m128_f32[0] += 1;
	}


#pragma region ��]�ړ�
	//��]�ړ�
	if (mouse->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}


	if (isSphere) {
		//���f����ς���
		object->SetModel(playerSpheremodel);
		//�}�E�X�̉��ւ̈ړ��ʂ�ۑ��i���ɉ�����΁{�A��ɂ�����΁\�i0�ȉ��ɂ͂Ȃ�Ȃ��j�j
		rollingSpeed += mouse->MoveMouseVector('y') / 30;
		if (rollingSpeed < 0) {
			rollingSpeed = 0;
		}
		//�}�E�X�̈ړ��ʂ��v���C���[�̉�]���x�ɂ�����
		spiralSpeed.z = rollingSpeed;
		//�}�E�X�𗣂����Ƃ��A�ړ��ʂ���������v���C���[�𒼐i������

		//���̎��̃v���C���[�̉�]���x�̓v���C���[�̈ړ����x�Ɉˑ�

	}
	else if (rollingSpeed > 0) {
		forvardvec.m128_f32[2] += rollingSpeed * 0.5;
		rollingSpeed -= 1;
		attackFlag = true;
	}
	else {
		rollingSpeed = 0.0f;
		spiralSpeed.z = 0;
		object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
		object->SetModel(playermodel);
		attackFlag = false;
	}
	SpiralVector(spiralSpeed);
#pragma endregion


	//�ړ��̔��f
	XMVECTOR playermatrot = { forvardvec };
	//��]�s���������
	playermatrot = XMVector3Normalize(playermatrot);
	playermatrot = XMVector3Transform(playermatrot, camera->matRot);
	//���K������
	playermatrot = XMVector3Normalize(playermatrot);

	forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
	float speed = 1.0f;
	XMFLOAT3 move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });

	//�v���C���[��^���ʂɌ�������
	//float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
	//object->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
}

void Enemy::GoTarget(Player* player)
{
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
	oldPlayerPos = player->object->GetPosition();

	distanceX = posX - oldPlayerPos.x;
	distanceZ = posZ - oldPlayerPos.z;

	posX -= distanceX / speed;
	posZ -= distanceZ / speed;

	object->SetPosition(XMFLOAT3(posX, posY, posZ));
}

void Enemy::Direction(Player* player)
{
	const float direction = 90.0f;
	XMFLOAT3 pos = object->GetPosition();
	XMFLOAT3 playerPos = player->object->GetPosition();

	XMFLOAT3 distance = { pos.x - playerPos.x,pos.y - playerPos.y,pos.z - playerPos.z };

	angle = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;

	object->SetRotation(XMFLOAT3(0.0f, angle, 0.0f));
}

void Enemy::Mawarikomi(Player* player)
{
	num -= 0.005;
	float x = object->GetPosition().x + 0.3f;
	float y = object->GetPosition().y;
	float z = object->GetPosition().z + num;

	object->SetPosition({ x, y, z });
}
