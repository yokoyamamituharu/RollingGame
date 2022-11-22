#include "Player.h"
#include "../Useful.h"
#include "ModelManager.h"
#include "safe_delete.h"

using namespace DirectX;

int Player::breakEnemy = 0;

Player::Player()
{
}

Player::~Player()
{
	safe_delete(object);
}

void Player::Initialize( Camera* camera)
{
	this->camera = camera;
	//�v���C���[�p���f����ǂݍ���
	playermodel = ModelManager::GetModel("player");
	playerSpheremodel = ModelManager::GetModel("playerSphere");

	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(playermodel);
	breakEnemy = 0;
}

void Player::Update()
{
	//if (Input::GetInstance()->PushKey(DIK_3)) {
	//	object->SetRotation({
	//		object->GetRotation().x,
	//		object->GetRotation().y ,
	//		object->GetRotation().z + 1.0f });
	//}

	if (Input::GetInstance()->TriggerKey(DIK_9)) {
		sceneType *= -1;
	}

	if (muteki == true) {
		mutekiTime++;
		if (mutekiTime > 60) {
			muteki = false;
			mutekiTime = 0;
		}
	}

	Move();
	Res();
	//�I�u�W�F�N�g�̃A�b�v�f�[�g
	object->Update();
}

void Player::Move()
{
	DirectX::XMVECTOR forvardvec = {};

	if (sphereFlag == false) {
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
	}

#pragma region ��]�ړ�
	//��]�ړ�
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}

	if (isSphere) {
		//���f����ς���
		object->SetModel(playerSpheremodel);
		//�}�E�X�̉��ւ̈ړ��ʂ�ۑ��i���ɉ�����΁{�A��ɂ�����΁\�i0�ȉ��ɂ͂Ȃ�Ȃ��j�j
		rollingSpeed += InputMouse::GetInstance()->MoveMouseVector('y') / 30;
		if (rollingSpeed < 0) {
			rollingSpeed = 0;
		}
		//�}�E�X�̈ړ��ʂ��v���C���[�̉�]���x�ɂ�����
		spiralSpeed.z = rollingSpeed;
		//�}�E�X�𗣂����Ƃ��A�ړ��ʂ���������v���C���[�𒼐i������

		//���̎��̃v���C���[�̉�]���x�̓v���C���[�̈ړ����x�Ɉˑ�

		sphereFlag = true;
	}
	else if (rollingSpeed > 0) {
		if (rollingSpeed > 400) {
			forvardvec.m128_f32[2] += 10;
		}
		else if (rollingSpeed > 300) {
			forvardvec.m128_f32[2] += 7;
		}
		else if (rollingSpeed > 100) {
			forvardvec.m128_f32[2] += 5;
		}
		else {
			forvardvec.m128_f32[2] += 3;
		}
		rollingSpeed -= 1;
		attackFlag = true;
	}
	else {
		rollingSpeed = 0.0f;
		spiralSpeed.z = 0;
		object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
		object->SetModel(playermodel);
		attackFlag = false;
		sphereFlag = false;
	}
	SpiralVector(spiralSpeed);
#pragma region �J�N�X
	//if (sceneType == 1)
	//{
	//}
	//else
	//{
	//	if (isSphere) {
	//		//���f����ς���
	//		object->SetModel(playerSpheremodel);
	//		//�}�E�X�̉��ւ̈ړ��ʂ�ۑ��i���ɉ�����΁{�A��ɂ�����΁\�i0�ȉ��ɂ͂Ȃ�Ȃ��j�j
	//		rollingSpeed += mouse->MoveMouseVector('y') / 30;
	//		if (rollingSpeed < 0) {
	//			rollingSpeed = 0;
	//		}
	//		//�}�E�X�̈ړ��ʂ��v���C���[�̉�]���x�ɂ�����
	//		spiralSpeed.z = rollingSpeed;
	//		//�}�E�X�𗣂����Ƃ��A�ړ��ʂ���������v���C���[�𒼐i������

	//		//���̎��̃v���C���[�̉�]���x�̓v���C���[�̈ړ����x�Ɉˑ�

	//		sphereFlag = true;
	//	}
	//	else if (rollingSpeed > 0) {
	//		rollingSpeed = 10;
	//		forvardvec.m128_f32[2] += rollingSpeed * 0.5;
	//		attackFlag = true;
	//		rollingTime++;
	//		if (rollingTime > 30) {
	//			rollingSpeed = 0;
	//			rollingTime = 0;
	//		}
	//	}
	//	else {
	//		rollingSpeed = 0.0f;
	//		spiralSpeed.z = 0;
	//		object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
	//		object->SetModel(playermodel);
	//		attackFlag = false;
	//		sphereFlag = false;
	//	}
	//	SpiralVector(spiralSpeed);
	//}
#pragma endregion


#pragma endregion

	//����͐i�ޕ����Ƀv���C���[���������鏈��
	////�ړ��̔��f
	//XMVECTOR playermatrot = { forvardvec };
	////��]�s���������
	//playermatrot = XMVector3Normalize(playermatrot);
	//playermatrot = XMVector3Transform(playermatrot, camera->matRot);
	////���K������
	//playermatrot = XMVector3Normalize(playermatrot);

	forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
	//forvardvec = XMVector3TransformNormal(forvardvec, object->GetMatRot());
	float speed = 1.2f;
	move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });

	//�v���C���[��^���ʂɌ�������
	//float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
	//object->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
}

void Player::Res(bool flag, XMFLOAT3 vec)
{
	object->VecSetPosition(backVec);

	//���~����
	if (resFlag2 == true) {
		if (gravity <= 2.0f) {
			gravity += 0.15f;
		}
		object->SetPosY({ object->GetPosition().y - gravity });
		if (object->GetPosition().y <= grundHeight) {
			resFlag2 = false;
			gravity = 0.0f;
			object->SetPosY(-6.0f);
			backVec = { 0,0,0 };
		}
	}

	//�㏸����
	if (resFlag1 == true) {
		timer += 0.0625f;
		float un = Ease(timer, 1.6);
		object->SetPosY({ posY + riseValue * un });
		if (timer >= 1) {
			resFlag1 = false;
			resFlag2 = true;
		}
	}

	//�o�E���h���N��
	if (flag == 1 && resFlag1 == false) {
		posY = object->GetPosition().y;
		resFlag1 = true;
		timer = 0;
		rollingSpeed = 0;
		backVec = Use::LoadXMVECTOR(-XMVector3Normalize(XMLoadFloat3(&move)));
	}
}

void Player::Stop()
{
	rollingSpeed = 0;
	object->SetPosition(outPos);
	resFlag1 = false;
	resFlag2 = false;
	gravity = 0.0f;	
	backVec = { 0,0,0 };
}

void Player::Draw()
{
	object->Draw();
}

float Player::Ease(float x, float s)
{
	if (x < 1.0f) {
		double num = pow(1 - x, s);
		double num2 = 1 - num;
		return num2;
	}
	else
	{
		//x��1�𒴂����l����pow�̕Ԃ�l�����������Ȃ�̂�
		return 1.0f;
	}
}

void Player::Damage(int damage)
{
	if (muteki == false) {
		hp -= damage;
		muteki = true;
	}
}


