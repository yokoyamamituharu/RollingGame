#include "Player.h"
using namespace DirectX;

void Player::Initialize(Input* input, InputMouse* mouse, Camera* camera)
{
	assert(input);
	assert(mouse);
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;
	//�v���C���[�p���f����ǂݍ���
	playermodel = Model::Create("player");
	playerSpheremodel = Model::Create("playerSphere");

	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(playermodel);
}

void Player::Update()
{
	if (Input::GetInstance()->PushKey(DIK_3)) {
		object->SetRotation({
			object->GetRotation().x,
			object->GetRotation().y ,
			object->GetRotation().z + 1.0f });
	}

	if (Input::GetInstance()->TriggerKey(DIK_9)) {
		sceneType *= -1;
	}

	Move();
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
	if (mouse->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}


	if (sceneType == 1)
	{
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
	}
	else
	{
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

			sphereFlag = true;
		}
		else if (rollingSpeed > 0) {
			rollingSpeed = 10;
			forvardvec.m128_f32[2] += rollingSpeed * 0.5;
			attackFlag = true;
			rollingTime++;
			if (rollingTime > 30) {
				rollingSpeed = 0;
				rollingTime = 0;
			}
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
	}
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
	XMFLOAT3 move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });

	//�v���C���[��^���ʂɌ�������
	//float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
	//object->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
}

void Player::Res(bool flag)
{


	//���~����
	if (resFlag2 == true) {
		//num -= 0.5;
		//object->SetPosY({ object->GetPosition().y - 0.5f });
		//if (object->GetPosition().y < -6) {
		//	resFlag2 = false;
		//	object->SetPosY(-6.0f);
		//	num = 0;
		//}
	}

	//�㏸����
	if (resFlag1 == true) {

		float un2 = Ease(timer, 1.6);
		float xnumm = object->GetPosition().x - backValue * un2;
		timer += 0.0625f;
		float un = Ease(timer, 1.6);
		un2 = Ease(timer, 1.6);

		object->SetPosY({ posY + riseValue * un });
		object->SetPosX({ xnumm + backValue * un2 });


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
		xvec = 4;
	}
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


