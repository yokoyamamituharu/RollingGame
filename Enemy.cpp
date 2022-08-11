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
	//�I�u�W�F�N�g�̃A�b�v�f�[�g
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
